#include "akb.h"

#include <SDL2/SDL.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <unordered_set>
#include <vector>

#include "../bridge.h"
#include "../patches.h"
#include "../util.h"

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

static constexpr int DEVICE_RATE     = 44100;
static constexpr int DEVICE_CHANNELS = 2;
static constexpr int DEVICE_SAMPLES  = 2048;

static SDL_AudioDeviceID g_Device = 0;
static std::vector<AKBTrack*> g_Tracks;
static std::unordered_set<AKBTrack*> g_readyForDeletion;

static constexpr int RAW_PCM_RING_FRAMES = DEVICE_RATE;
static int16_t g_rawPCMRing[RAW_PCM_RING_FRAMES * DEVICE_CHANNELS];
static int g_rawPCMReadPos   = 0;
static int g_rawPCMWritePos  = 0;
static int g_rawPCMAvailable = 0;
static float g_rawPCMVolume  = 1.0f;

std::unordered_set<AKBTrack*> AKBSystem::shouldBeMonitored;

bool akbIsAkb1File(const unsigned char* data) {
    uint32_t magic = *reinterpret_cast<const uint32_t*>(data + 0x00);
    if (magic != 0x20424B41) {
        return false;
    }

    uint16_t version = *reinterpret_cast<const uint16_t*>(data + 0x04);
    if (version > 3) {
        return false;
    }

    uint16_t sizeTable[4] = { 0x20, 0x2c, 0x44, 0x44 };
    uint16_t requiredSize = sizeTable[version];

    uint16_t expectedSize = *reinterpret_cast<const uint16_t*>(data + 0x06);

    return (expectedSize == requiredSize);
}

bool akbIsAkb2File(const unsigned char* data) {
    uint32_t magic = *reinterpret_cast<const uint32_t*>(data + 0x00);
    if (magic != 0x32424B41) {
        return false;
    }

    uint8_t version = *(data + 0x04);
    if (version > 1) {
        return false;
    }

    uint16_t sizeTable[2] = { 0x10, 0x18 };
    uint16_t requiredSize = sizeTable[version];

    uint16_t expectedSize = *reinterpret_cast<const uint16_t*>(data + 0x06);

    return (expectedSize == requiredSize);
}

bool akbIsAkbFile(const unsigned char* data) {
    if (akbIsAkb1File(data))
        return true;
    return akbIsAkb2File(data);
}

bool akbMaterialIsLoopAudio(const unsigned char* data) {
    uint32_t loopStart = 0;
    uint32_t loopEnd   = 0;

    if (akbIsAkb1File(data)) {
        loopStart = *reinterpret_cast<const uint32_t*>(data + 0x14);
        loopEnd   = *reinterpret_cast<const uint32_t*>(data + 0x18);
    } else {
        loopStart = *reinterpret_cast<const uint32_t*>(data + 0x10);
        loopEnd   = *reinterpret_cast<const uint32_t*>(data + 0x14);
    }

    return (loopEnd > loopStart);
}

uint32_t akbMaterialGetLoopStart(const unsigned char* data) {
    if (akbIsAkb1File(data)) {
        return *reinterpret_cast<const uint32_t*>(data + 0x14);
    } else {
        return *reinterpret_cast<const uint32_t*>(data + 0x10);
    }
}

uint32_t akbMaterialGetLoopEnd(const unsigned char* data) {
    if (akbIsAkb1File(data)) {
        return *reinterpret_cast<const uint32_t*>(data + 0x18);
    } else {
        return *reinterpret_cast<const uint32_t*>(data + 0x14);
    }
}

uint32_t akbGetFileSize(const unsigned char* data) {
    if (akbIsAkbFile(data))
        return *reinterpret_cast<const uint32_t*>(data + 0x08);

    return 0;
}

uint16_t akbGetHeaderSize(const unsigned char* data) {
    if (akbIsAkbFile(data))
        return *reinterpret_cast<const uint32_t*>(data + 0x06);

    return 0xFFFF;
}

uint32_t akbGetSoundDataOffset(const unsigned char* data, uint32_t soundIndex) {
    if (!akbIsAkbFile(data))
        return 0;

    uint16_t headerSize  = *reinterpret_cast<const uint16_t*>(data + 0x06);
    uint16_t totalSounds = *reinterpret_cast<const uint16_t*>(data + 0x0C);

    if (soundIndex >= totalSounds) {
        return 0xFFFFFFFF;
    }

    uintptr_t entryOffset              = headerSize + (static_cast<uintptr_t>(soundIndex) << 4);
    const unsigned char* trackEntryPtr = data + entryOffset;

    uint32_t soundDataOffset = *reinterpret_cast<const uint32_t*>(trackEntryPtr + 0x04);

    return soundDataOffset;
}

const unsigned char* akbGetSoundData(const unsigned char* data, uint32_t soundIndex) {
    if (akbIsAkb1File(data)) {
        return data;
    }

    if (!akbIsAkb2File(data)) {
        return nullptr;
    }

    uint32_t offset = akbGetSoundDataOffset(data, soundIndex);

    return data + offset;
}

uint16_t akbGetVersion(const unsigned char* data) {
    if (akbIsAkb1File(data)) {
        return *reinterpret_cast<const uint16_t*>(data + 0x04);
    }

    if (akbIsAkb2File(data)) {
        return static_cast<uint16_t>(*(data + 0x04));
    }

    return 0xFFFF;
}

uint32_t akbMaterialGetAudioDataOffset(const unsigned char* data) {
    if (!akbIsAkb1File(data)) {
        uint16_t version = *reinterpret_cast<const uint16_t*>(data + 0x04);
        uint32_t loopEnd = *reinterpret_cast<const uint32_t*>(data + 0x18);

        return static_cast<uint32_t>(version) + loopEnd;
    }

    uint32_t offset = akbGetHeaderSize(data);

    if (akbIsAkb1File(data)) {
        if (akbGetVersion(data) != 0) {
            offset += *reinterpret_cast<const uint16_t*>(data + 0x28);
        }
    } else {
        offset += *reinterpret_cast<const uint16_t*>(data + 0x0C);
    }

    if (akbIsAkb1File(data)) {
        offset += *reinterpret_cast<const uint32_t*>(data + 0x1C);
    } else {
        offset += *reinterpret_cast<const uint32_t*>(data + 0x18);
    }

    return offset;
}

uint32_t akbMaterialGetAudioDataSize(const unsigned char* data) {
    if (!akbIsAkb1File(data)) {
        return *reinterpret_cast<const uint32_t*>(data + 0x08);
    }

    return akbGetFileSize(data) - akbMaterialGetAudioDataOffset(data);
}

const unsigned char* akbMaterialGetAudioData(const unsigned char* data) {
    akbIsAkb1File(data);
    return data + akbMaterialGetAudioDataOffset(data);
}

static float g_masterVolume[3] = { 1.0f, 1.0f, 1.0f };

static void AudioMixCallback(void* userdata, uint8_t* stream, int len) {
    const int outFrames = len / (sizeof(int16_t) * DEVICE_CHANNELS);

    float mixBuf[DEVICE_SAMPLES * DEVICE_CHANNELS] = { };

    int16_t decodeTmp[DEVICE_SAMPLES * DEVICE_CHANNELS];

    for (AKBTrack* t : g_Tracks) {
        if (AKBSystem::shouldBeMonitored.find(t) != AKBSystem::shouldBeMonitored.end())
            debugPrintf("AudioMixCallback %p Active: %s\n", t, t->active ? "true" : "false");
        if (!t->active || t->paused || !t->vorbis)
            continue;

        stb_vorbis* vs = static_cast<stb_vorbis*>(t->vorbis);

        const float p    = (t->pan / 128.0f) * (t->panRange / 127.0f);
        const float mixL = (p > 0.0f) ? (1.0f - p) : 1.0f;
        const float mixR = (p < 0.0f) ? (1.0f + p) : 1.0f;

        int written = 0;

        while (written < outFrames && t->active) {
            if (t->loopEnd != UINT32_MAX && t->streamFramePos >= static_cast<int>(t->loopEnd)) {
                if (t->loop) {
                    stb_vorbis_seek_frame(vs, t->loopStart);
                    t->streamFramePos = static_cast<int>(t->loopStart);
                } else {
                    t->active = false;
                    break;
                }
            }

            const int canRead = (t->loopEnd == UINT32_MAX)
                ? outFrames - written
                : std::min(static_cast<int>(t->loopEnd) - t->streamFramePos,
                      outFrames - written);

            if (canRead <= 0)
                continue;

            const int got = stb_vorbis_get_samples_short_interleaved(
                vs, DEVICE_CHANNELS, decodeTmp, canRead * DEVICE_CHANNELS);

            if (got <= 0) {
                if (t->loop) {
                    stb_vorbis_seek_frame(vs, t->loopStart);
                    t->streamFramePos = static_cast<int>(t->loopStart);
                } else {
                    t->active = false;
                }
                break;
            }

            for (int i = 0; i < got && t->active; ++i) {
                float vol = t->volume * g_masterVolume[t->type];
                if (t->volFadeLeft > 0) {
                    t->volume = std::clamp(t->volume + t->volStep, 0.0f, 1.0f);
                    if (--t->volFadeLeft == 0) {
                        t->volume = t->volTarget;
                        if (t->volTarget <= 0.0f) {
                            if (t->stopOnSilence) {
                                t->active = false;
                            } else if (t->pauseOnSilence) {
                                t->paused         = true;
                                t->pauseOnSilence = false;
                            } else if (t->tempStopOnSilence) {
                                t->streamFramePos = 0;
                                t->paused         = true;
                                t->pauseOnSilence = false;
                            }
                        }
                    }
                }

                const int mi = (written + i) * DEVICE_CHANNELS;
                mixBuf[mi + 0] += static_cast<float>(decodeTmp[i * DEVICE_CHANNELS + 0]) * vol * mixL;
                mixBuf[mi + 1] += static_cast<float>(decodeTmp[i * DEVICE_CHANNELS + 1]) * vol * mixR;
            }

            t->streamFramePos += got;
            written += got;
        }
    }

    {
        const int available = std::min(g_rawPCMAvailable, outFrames);
        for (int i = 0; i < available; ++i) {
            const int ringIdx = ((g_rawPCMReadPos + i) % RAW_PCM_RING_FRAMES) * DEVICE_CHANNELS;
            const int mi      = i * DEVICE_CHANNELS;
            mixBuf[mi + 0] += static_cast<float>(g_rawPCMRing[ringIdx + 0]) * g_rawPCMVolume;
            mixBuf[mi + 1] += static_cast<float>(g_rawPCMRing[ringIdx + 1]) * g_rawPCMVolume;
        }
        g_rawPCMReadPos = (g_rawPCMReadPos + available) % RAW_PCM_RING_FRAMES;
        g_rawPCMAvailable -= available;
    }

    int16_t* out = reinterpret_cast<int16_t*>(stream);
    for (int i = 0; i < outFrames * DEVICE_CHANNELS; ++i)
        out[i] = static_cast<int16_t>(std::clamp(static_cast<int>(mixBuf[i]), -32768, 32767));
}

bool AKBSystem::Init() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "AKBSystem: SDL_InitSubSystem failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_AudioSpec desired { }, obtained { };
    desired.freq     = DEVICE_RATE;
    desired.format   = AUDIO_S16SYS;
    desired.channels = DEVICE_CHANNELS;
    desired.samples  = DEVICE_SAMPLES;
    desired.callback = AudioMixCallback;
    desired.userdata = nullptr;

    g_Device = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);
    if (!g_Device) {
        fprintf(stderr, "AKBSystem: SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_PauseAudioDevice(g_Device, 0);
    return true;
}

void AKBSystem::Update() {
    Lock();
    auto it = g_readyForDeletion.begin();
    while (it != g_readyForDeletion.end()) {
        AKBTrack* track = *it;
        if (!track->active) {
            delete track;
            it = g_readyForDeletion.erase(it);
        } else {
            ++it;
        }
    }
    Unlock();
}

void AKBSystem::Quit() {
    if (g_Device) {
        SDL_CloseAudioDevice(g_Device);
        g_Device = 0;
    }
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void AKBSystem::Lock() {
    if (g_Device)
        SDL_LockAudioDevice(g_Device);
}

void AKBSystem::Unlock() {
    if (g_Device)
        SDL_UnlockAudioDevice(g_Device);
}

void AKBSystem::SetMasterVolume(TrackType type, float volume) {
    Lock();
    g_masterVolume[type] = volume;
    Unlock();
}

int AKBSystem::GetDeviceRate() {
    return DEVICE_RATE;
}

int AKBSystem::GetDeviceChannels() {
    return DEVICE_CHANNELS;
}

void AKBSystem::PushRawPCM(const int16_t* samples, int frames) {
    if (!samples || frames <= 0)
        return;

    Lock();

    int framesToWrite = frames;
    if (framesToWrite > RAW_PCM_RING_FRAMES)
        framesToWrite = RAW_PCM_RING_FRAMES;

    if (g_rawPCMAvailable + framesToWrite > RAW_PCM_RING_FRAMES) {
        int overflow    = (g_rawPCMAvailable + framesToWrite) - RAW_PCM_RING_FRAMES;
        g_rawPCMReadPos = (g_rawPCMReadPos + overflow) % RAW_PCM_RING_FRAMES;
        g_rawPCMAvailable -= overflow;
    }

    for (int i = 0; i < framesToWrite; ++i) {
        const int ringIdx         = ((g_rawPCMWritePos + i) % RAW_PCM_RING_FRAMES) * DEVICE_CHANNELS;
        g_rawPCMRing[ringIdx + 0] = samples[i * DEVICE_CHANNELS + 0];
        g_rawPCMRing[ringIdx + 1] = samples[i * DEVICE_CHANNELS + 1];
    }

    g_rawPCMWritePos = (g_rawPCMWritePos + framesToWrite) % RAW_PCM_RING_FRAMES;
    g_rawPCMAvailable += framesToWrite;

    Unlock();
}

void AKBSystem::ClearRawPCM() {
    Lock();
    g_rawPCMReadPos   = 0;
    g_rawPCMWritePos  = 0;
    g_rawPCMAvailable = 0;
    Unlock();
}

void AKBSystem::SetRawPCMVolume(float volume) {
    Lock();
    g_rawPCMVolume = volume;
    Unlock();
}

int AKBSystem::GetRawPCMAvailableFrames() {
    Lock();
    int frames = g_rawPCMAvailable;
    Unlock();
    return frames;
}

void AKBSystem::RegisterTrack(AKBTrack* track) {
    Lock();
    g_Tracks.push_back(track);
    Unlock();
}

void AKBSystem::UnregisterTrack(AKBTrack* track) {
    Lock();
    g_Tracks.erase(std::remove(g_Tracks.begin(), g_Tracks.end(), track), g_Tracks.end());
    Unlock();
}

AKBTrack::AKBTrack(TrackType type) {
    AKBSystem::RegisterTrack(this);
    this->type = type;
}

AKBTrack::~AKBTrack() {
    AKBSystem::UnregisterTrack(this);
    Unload();
}

bool AKBTrack::Load(const std::string& path) {
    AKBSystem::Lock();

    if (this->path == path) {
        AKBSystem::Unlock();
        return true;
    }

    this->path = path;

    int fileLen  = 0;
    uint8_t* buf = reinterpret_cast<uint8_t*>(bridge::loadFile(path.c_str(), &fileLen));
    if (!buf) {
        fprintf(stderr, "AKBTrack::Load: cannot open '%s'\n", path.c_str());
        AKBSystem::Unlock();
        return false;
    }

    if (!akbIsAkbFile(buf)) {
        fprintf(stderr, "AKBTrack::Load: file too small\n");
        free(buf);
        AKBSystem::Unlock();
        return false;
    }

    uint32_t rawLoopStart = akbMaterialGetLoopStart(buf);
    uint32_t rawLoopEnd   = akbMaterialGetLoopEnd(buf);
    this->loop            = akbMaterialIsLoopAudio(buf);

    if (vorbis != nullptr) {
        stb_vorbis_close(static_cast<stb_vorbis*>(vorbis));
        vorbis = nullptr;
    }
    if (fileData) {
        free(fileData);
        fileData = nullptr;
    }

    const uint8_t* oggData = akbMaterialGetAudioData(buf);
    const int oggLen       = akbMaterialGetAudioDataSize(buf);

    int error      = 0;
    stb_vorbis* vs = stb_vorbis_open_memory(oggData, oggLen, &error, nullptr);
    if (!vs) {
        fprintf(stderr, "AKBTrack::Load: stb_vorbis_open_memory failed (error %d) for '%s'\n",
            error, path.c_str());
        free(buf);
        AKBSystem::Unlock();
        return false;
    }

    this->fileData       = buf;
    this->fileDataLen    = fileLen;
    this->vorbis         = vs;
    this->loopStart      = this->loop ? rawLoopStart : 0u;
    this->loopEnd        = this->loop ? rawLoopEnd : UINT32_MAX;
    this->streamFramePos = 0;

    AKBSystem::Unlock();
    return true;
}

void AKBTrack::Unload() {
    AKBSystem::Lock();

    if (vorbis != nullptr) {
        stb_vorbis_close(static_cast<stb_vorbis*>(vorbis));
        vorbis = nullptr;
    }
    if (fileData) {
        free(fileData);
        fileData = nullptr;
    }

    path           = "";
    active         = false;
    fileDataLen    = 0;
    streamFramePos = 0;
    loopStart = loopEnd = 0;

    AKBSystem::Unlock();
}

void AKBTrack::Play() {
    AKBSystem::Lock();
    active            = true;
    paused            = false;
    volFadeLeft       = 0;
    stopOnSilence     = false;
    pauseOnSilence    = false;
    tempStopOnSilence = false;
    AKBSystem::Unlock();
}

void AKBTrack::Reset() {
    debugPrintf("AKBTrack Reset");
    AKBSystem::Lock();
    if (vorbis != nullptr) {
        stb_vorbis_seek_frame(static_cast<stb_vorbis*>(vorbis), 0);
        streamFramePos = 0;
    }
    AKBSystem::Unlock();
}

void AKBTrack::Stop(int fadeFrames) {
    AKBSystem::Lock();
    if (fadeFrames <= 0) {
        active = false;
        if (vorbis != nullptr) {
            stb_vorbis_seek_frame(static_cast<stb_vorbis*>(vorbis), 0);
            streamFramePos = 0;
        }
    } else {
        const int audioFrames = fadeFrames * DEVICE_RATE / bridge::framerate;
        const float step      = (audioFrames > 0 && volume > 0.0f)
            ? (-volume / static_cast<float>(audioFrames))
            : 0.0f;
        volTarget             = 0.0f;
        volStep               = step;
        volFadeLeft           = audioFrames;
        stopOnSilence         = true;
        pauseOnSilence        = false;
        tempStopOnSilence     = false;
    }
    AKBSystem::Unlock();
}

void AKBTrack::Pause(int fadeFrames) {
    AKBSystem::Lock();
    if (fadeFrames <= 0) {
        paused         = true;
        pauseOnSilence = false;
    } else {
        const int audioFrames = fadeFrames * DEVICE_RATE / bridge::framerate;
        const float step      = (audioFrames > 0 && volume > 0.0f)
            ? (-volume / static_cast<float>(audioFrames))
            : 0.0f;

        volTarget         = 0.0f;
        volStep           = step;
        volFadeLeft       = audioFrames;
        stopOnSilence     = false;
        pauseOnSilence    = true;
        tempStopOnSilence = false;
    }
    AKBSystem::Unlock();
}

void AKBTrack::TempStop(int fadeFrames) {
    AKBSystem::Lock();
    if (fadeFrames <= 0) {
        paused         = true;
        pauseOnSilence = false;
        streamFramePos = 0;
    } else {
        const int audioFrames = fadeFrames * DEVICE_RATE / bridge::framerate;
        const float step      = (audioFrames > 0 && volume > 0.0f)
            ? (-volume / static_cast<float>(audioFrames))
            : 0.0f;

        volTarget         = 0.0f;
        volStep           = step;
        volFadeLeft       = audioFrames;
        stopOnSilence     = false;
        pauseOnSilence    = false;
        tempStopOnSilence = true;
    }
    AKBSystem::Unlock();
}

void AKBTrack::Resume() {
    AKBSystem::Lock();
    paused = false;
    active = true;
    AKBSystem::Unlock();
}

void AKBTrack::SetVolume(int volume) {
    const float v = std::clamp(volume, 0, 127) / 127.0f;
    AKBSystem::Lock();
    this->volume            = v;
    this->volTarget         = v;
    this->volFadeLeft       = 0;
    this->stopOnSilence     = false;
    this->pauseOnSilence    = false;
    this->tempStopOnSilence = false;
    AKBSystem::Unlock();
}

void AKBTrack::SetVolume(float volume) {
    AKBSystem::Lock();
    this->volume            = volume;
    this->volTarget         = volume;
    this->volFadeLeft       = 0;
    this->stopOnSilence     = false;
    this->pauseOnSilence    = false;
    this->tempStopOnSilence = false;
    AKBSystem::Unlock();
}

void AKBTrack::MoveVolume(int targetVolume, int frames) {
    const float target = std::clamp(targetVolume, 0, 127) / 127.0f;
    AKBSystem::Lock();
    if (frames <= 0) {
        volume            = target;
        volTarget         = target;
        volFadeLeft       = 0;
        stopOnSilence     = false;
        pauseOnSilence    = false;
        tempStopOnSilence = false;
    } else {
        const int audioFrames = frames * DEVICE_RATE / bridge::framerate;
        volTarget             = target;
        volStep               = (audioFrames > 0)
            ? ((target - volume) / static_cast<float>(audioFrames))
            : 0.0f;
        volFadeLeft           = audioFrames;
        stopOnSilence         = false;
        pauseOnSilence        = false;
        tempStopOnSilence     = false;
    }
    AKBSystem::Unlock();
}

bool AKBTrack::IsActive() const { return active; }
bool AKBTrack::IsPlaying() const { return active && !paused; }
bool AKBTrack::IsPaused() const { return paused; }

void AKBTrack::SetTempo(int ratio) {
    AKBSystem::Lock();
    tempo = ratio;
    AKBSystem::Unlock();
}
void AKBTrack::SetPitch(int p) {
    AKBSystem::Lock();
    pitch = std::clamp(p, -32768, 32767);
    AKBSystem::Unlock();
}
void AKBTrack::SetPan(int p) {
    AKBSystem::Lock();
    pan = std::clamp(p, -128, 127);
    AKBSystem::Unlock();
}
void AKBTrack::SetPanRange(int pr) {
    AKBSystem::Lock();
    panRange = std::clamp(pr, 0, 127);
    AKBSystem::Unlock();
}

void AKBTrack::MarkReadyForDeletion() {
    AKBSystem::Lock();
    g_readyForDeletion.insert(this);
    AKBSystem::Unlock();
}