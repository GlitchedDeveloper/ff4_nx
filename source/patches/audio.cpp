#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <vector>

#include "../audio/akb.h"
#include "../babil.h"
#include "../patches.h"
// #include "../so_util.h"
#include "../util.h"

namespace patches::audio {

int CoreAudioOutInit(int, int, uintptr_t callback) {
    debugPrintf("CoreAudioOutInit\n");
    return -1;
}

int CoreAudioOutKick() {
    debugPrintf("CoreAudioOutKick\n");
    return -1;
}

int CoreAudioOutQueueBuffer(void* buffer, size_t size) {
    debugPrintf("CoreAudioOutQueueBuffer buffer: %p size: %d\n", buffer, size);
    return -1;
}

void CoreAudioOutRelease() {
    debugPrintf("CoreAudioOutRelease\n");
}

int CoreAudioOutResume() {
    debugPrintf("CoreAudioOutResume\n");
    return -1;
}

int CoreAudioOutSuspend() {
    debugPrintf("CoreAudioOutSuspend\n");
    return -1;
}

namespace ds::snd {
    namespace BGMHandle {
        struct BGMHandle
        {
            AKBTrack* m_NNSSndHandle;
            bool m_pausing;
            int m_seqNo;
            static bool m_BGMEnabler;
        };

        std::vector<BGMHandle*> g_activeBGMHandles;
        bool BGMHandle::m_BGMEnabler = true;
        int m_LastPlayedID           = -1;
        std::unordered_map<BGMHandle*, int> m_IDs;

        void Ctor(BGMHandle* self) {
            debugPrintf("BGMHandle::BGMHandle %p\n", self);
            self->m_NNSSndHandle = new AKBTrack(TrackType_BGM);
            self->m_pausing      = false;
            self->m_seqNo        = -1;
        }
        void Dtor(BGMHandle* self) {
            debugPrintf("BGMHandle::~BGMHandle %p\n", self);
            auto it = std::find(g_activeBGMHandles.begin(), g_activeBGMHandles.end(), self);
            if (it != g_activeBGMHandles.end()) {
                g_activeBGMHandles.erase(it);
            }
            if (self->m_NNSSndHandle != nullptr) {
                delete self->m_NNSSndHandle;
                self->m_NNSSndHandle = nullptr;
            }
            m_IDs.erase(self);
        }
        bool IsPlaying(BGMHandle* self) {
            debugPrintf("BGMHandle::IsPlaying %p\n", self);
            if (self->m_NNSSndHandle == nullptr)
                return false;
            return self->m_NNSSndHandle->IsPlaying();
        }
        void Stop(BGMHandle* self, int fadeFrame) {
            debugPrintf("BGMHandle::Stop %p fadeFrame: %d\n", self, fadeFrame);
            self->m_NNSSndHandle->Stop(fadeFrame);

            auto it = std::find(g_activeBGMHandles.begin(), g_activeBGMHandles.end(), self);
            if (it != g_activeBGMHandles.end()) {
                g_activeBGMHandles.erase(it);
            }
        }
        void Pause(BGMHandle* self, bool pause) {
            debugPrintf("BGMHandle::Pause %p %s\n", self, pause ? "true" : "false");
            AKBTrack& track = *self->m_NNSSndHandle;
            self->m_pausing = pause;
            if (pause) {
                track.Pause();

                auto it = std::find(g_activeBGMHandles.begin(), g_activeBGMHandles.end(), self);
                if (it != g_activeBGMHandles.end()) {
                    g_activeBGMHandles.erase(it);
                }
            } else {
                track.Resume();

                std::vector<BGMHandle*> activeInstances = g_activeBGMHandles;
                for (BGMHandle* other : activeInstances) {
                    if (other != self) {
                        Pause(other, true);
                    }
                }

                if (std::find(g_activeBGMHandles.begin(), g_activeBGMHandles.end(), self) == g_activeBGMHandles.end()) {
                    g_activeBGMHandles.push_back(self);
                }

                if (m_LastPlayedID != m_IDs[self]) {
                    self->m_NNSSndHandle->Reset();
                    m_LastPlayedID = m_IDs[self];
                }
            }
        }
        void PlayV(BGMHandle* self, int* args) {
            debugPrintf("BGMHandle::Play %p playerNo: %d\n", self, args[0]);
            if (!self->m_BGMEnabler) {
                debugPrintf("m_BGMEnabler disabled\n");
                return;
            }
            if (self->m_NNSSndHandle == nullptr) {
                debugPrintf("m_NNSSndHandle uninitialized\n");
                return;
            }

            int& id         = args[0];
            AKBTrack& track = *self->m_NNSSndHandle;

            debugPrintf("Started Playing: %p\n", self->m_NNSSndHandle);

            std::vector<BGMHandle*> activeInstances = g_activeBGMHandles;
            for (BGMHandle* other : activeInstances) {
                if (other != self) {
                    Pause(other, true);
                }
            }
            if (std::find(g_activeBGMHandles.begin(), g_activeBGMHandles.end(), self) == g_activeBGMHandles.end()) {
                g_activeBGMHandles.push_back(self);
            }

            if (m_LastPlayedID != id) {
                self->m_NNSSndHandle->Reset();
                m_LastPlayedID = id;
            }

            m_IDs[self] = id;

            char path[256];
            snprintf(path, sizeof(path), "BGM%02d.akb", id);
            track.Load(path);
            track.SetVolume(1.0f);
            track.Play();
        }
        void Play(BGMHandle* self, int playerNo, int bankNo, int playerPrio, int seqNo) {
            debugPrintf("BGMHandle::Play %p playerNo: %d bankNo: %d playerPrio: %d seqNo: %d\n", self, playerNo, bankNo, playerPrio, seqNo);
            if (!self->m_BGMEnabler) {
                debugPrintf("m_BGMEnabler disabled\n");
                return;
            }
            if (self->m_NNSSndHandle == nullptr) {
                debugPrintf("m_NNSSndHandle uninitialized\n");
                return;
            }

            int args[4]   = { playerNo, bankNo, playerPrio, seqNo };
            self->m_seqNo = seqNo;
            PlayV(self, args);
        }
        void SetVolume(BGMHandle* self, int volume) {
            debugPrintf("BGMHandle::SetVolume %p %.0f%%\n", self, (float)volume / 1.270f);
            self->m_NNSSndHandle->SetVolume(volume);
        }
        void MoveVolume(BGMHandle* self, int targetVolume, int frames) {
            debugPrintf("BGMHandle::MoveVolume %p targetVolume: %d frames: %d\n", self, targetVolume, frames);
            self->m_NNSSndHandle->MoveVolume(targetVolume, frames);
        }
        bool isPausing(BGMHandle* self) {
            debugPrintf("BGMHandle::isPausing %p\n", self);
            return self->m_pausing;
        }
        int seqNo(BGMHandle* self) {
            debugPrintf("BGMHandle::seqNo %p\n", self);
            if (self->m_NNSSndHandle == nullptr) {
                debugPrintf("m_NNSSndHandle uninitialized\n");
                return -1;
            }
            return self->m_seqNo;
        }
        void setBGMEnable(bool enable) {
            debugPrintf("BGMHandle::setBGMEnable %s\n", enable ? "true" : "false");
            BGMHandle::m_BGMEnabler = enable;
        }
        bool getBGMEnable() {
            debugPrintf("BGMHandle::getBGMEnable\n");
            return BGMHandle::m_BGMEnabler;
        }
        void forceReleaseSequence(BGMHandle* self) {
            debugPrintf("BGMHandle::forceReleaseSequence %p\n", self);
        }
        void setTempo(BGMHandle* self, int ratio) {
            debugPrintf("BGMHandle::setTempo %p ratio: %d\n", self, ratio);
            self->m_NNSSndHandle->SetTempo(ratio);
        }
    }

    namespace SEHandle {
        struct SEHandle
        {
            AKBTrack* m_NNSSndHandle;
        };

        std::vector<SEHandle*> g_activeSEHandles;
        std::unordered_map<AKBTrack*, std::pair<int, int>> g_sePlayInfo;

        void Ctor(SEHandle* self) {
            debugPrintf("SEHandle::Ctor %p\n", self);
            self->m_NNSSndHandle = new AKBTrack(TrackType_SE);
        }

        void Dtor(SEHandle* self) {
            debugPrintf("SEHandle::Dtor %p\n", self);
            auto it = std::find(g_activeSEHandles.begin(), g_activeSEHandles.end(), self);
            if (it != g_activeSEHandles.end()) {
                g_activeSEHandles.erase(it);
            }
            if (self->m_NNSSndHandle) {
                self->m_NNSSndHandle->MarkReadyForDeletion();
                self->m_NNSSndHandle = nullptr;
            }
        }

        bool IsPlaying(SEHandle* self) {
            debugPrintf("SEHandle::IsPlaying %p\n", self);
            if (self->m_NNSSndHandle == nullptr)
                return false;
            return self->m_NNSSndHandle->IsPlaying();
        }

        void Play(SEHandle* self, int seqArcNo, int index, int volume, int pan) {
            debugPrintf("SEHandle::Play %p seqArcNo: %d index: %d volume: %d seqNo: %d\n", self, seqArcNo, index, volume, pan);

            if (self->m_NNSSndHandle == nullptr)
                return;

            debugPrintf("Started Playing: %p\n", self->m_NNSSndHandle);
            g_sePlayInfo[self->m_NNSSndHandle] = std::make_pair(seqArcNo, index);
            // if (seqArcNo == 0 && index == 22) {
            //     AKBSystem::shouldBeMonitored.insert(self->m_NNSSndHandle);
            //     debugPrintf("Started Monitoring: %p\n", self->m_NNSSndHandle);
            // }

            char path[256];
            snprintf(path, sizeof(path), "SE%03d_%02d.akb", seqArcNo, index);
            self->m_NNSSndHandle->Load(path);
            self->m_NNSSndHandle->SetVolume(volume);
            self->m_NNSSndHandle->SetPan(pan);
            self->m_NNSSndHandle->Play();

            if (std::find(g_activeSEHandles.begin(), g_activeSEHandles.end(), self) == g_activeSEHandles.end()) {
                g_activeSEHandles.push_back(self);
            }
        }

        void PlayFadeIn(SEHandle* self, int seqArcNo, int index, int fadeFrames, int volume, int pan) {
            debugPrintf("SEHandle::PlayFadeIn %p seqArcNo: %d index: %d fadeFrames: %d volume: %d seqNo: %d\n", self, seqArcNo, index, fadeFrames, volume, pan);
            if (self->m_NNSSndHandle == nullptr)
                return;

            g_sePlayInfo[self->m_NNSSndHandle] = std::make_pair(seqArcNo, index);

            char path[256];
            snprintf(path, sizeof(path), "SE%03d_%02d.akb", seqArcNo, index);
            self->m_NNSSndHandle->Load(path);
            self->m_NNSSndHandle->SetVolume(0);
            self->m_NNSSndHandle->SetPan(pan);
            self->m_NNSSndHandle->Play();
            self->m_NNSSndHandle->MoveVolume(volume, fadeFrames);

            if (std::find(g_activeSEHandles.begin(), g_activeSEHandles.end(), self) == g_activeSEHandles.end()) {
                g_activeSEHandles.push_back(self);
            }
        }

        void setPan(SEHandle* self, uint16_t trackBitMask, int pan) {
            debugPrintf("SEHandle::setPan %p trackBitMask: %d pan: %d\n", self, trackBitMask, pan);
            self->m_NNSSndHandle->SetPan(pan);
        }

        void setPanRange(SEHandle* self, uint16_t trackBitMask, int panRange) {
            debugPrintf("SEHandle::setPanRange %p trackBitMask: %d panRange: %d\n", self, trackBitMask, panRange);
            self->m_NNSSndHandle->SetPanRange(panRange);
        }

        void setPitch(SEHandle* self, uint16_t trackBitMask, int pitch) {
            debugPrintf("SEHandle::setPitch %p trackBitMask: %d pitch: %d\n", self, trackBitMask, pitch);
            self->m_NNSSndHandle->SetPitch(pitch);
        }

        void setTempo(SEHandle* self, int ratio) {
            debugPrintf("SEHandle::setTempo %p ratio: %d\n", self, ratio);
            self->m_NNSSndHandle->SetTempo(ratio);
        }

        void setVolume(SEHandle* self, int volume) {
            debugPrintf("SEHandle::setVolume %p volume: %d\n", self, volume);
            if (self->m_NNSSndHandle != nullptr) {
                self->m_NNSSndHandle->SetVolume(volume);
            }
        }

        void MoveVolume(SEHandle* self, int targetVolume, int frames) {
            debugPrintf("SEHandle::MoveVolume %p targetVolume: %d frames: %d\n", self, targetVolume, frames);
            if (self->m_NNSSndHandle != nullptr) {
                self->m_NNSSndHandle->MoveVolume(targetVolume, frames);
            }
        }

        void stop(SEHandle* self, int fadeFrame) {
            debugPrintf("SEHandle::stop %p fadeFrame: %d\n", self, fadeFrame);
            g_sePlayInfo.erase(self->m_NNSSndHandle);
            if (self->m_NNSSndHandle != nullptr) {
                self->m_NNSSndHandle->Stop(fadeFrame);
            }
            auto it = std::find(g_activeSEHandles.begin(), g_activeSEHandles.end(), self);
            if (it != g_activeSEHandles.end()) {
                g_activeSEHandles.erase(it);
            }
        }
    }

    void dssndUnloadSE1() {
        debugPrintf("dssndUnloadSE\n");
        if (SEHandle::g_activeSEHandles.empty())
            return;
        auto* handle = SEHandle::g_activeSEHandles.back();
        if (handle->m_NNSSndHandle != nullptr) {
            SEHandle::g_sePlayInfo.erase(handle->m_NNSSndHandle);
            handle->m_NNSSndHandle->Stop(0);
            // handle->m_NNSSndHandle->MarkReadyForDeletion();
            // handle->m_NNSSndHandle = nullptr;
        }
        SEHandle::g_activeSEHandles.pop_back();
    }
    void dssndUnloadSE2(s32 stacks) {
        debugPrintf("dssndUnloadSE(%d)\n", stacks);
        if (stacks <= 0)
            stacks = 1;
        int count = std::min((size_t)stacks, SEHandle::g_activeSEHandles.size());
        for (int i = 0; i < count; i++) {
            if (SEHandle::g_activeSEHandles.empty())
                break;
            auto* handle = SEHandle::g_activeSEHandles.back();
            if (handle->m_NNSSndHandle != nullptr) {
                SEHandle::g_sePlayInfo.erase(handle->m_NNSSndHandle);
                handle->m_NNSSndHandle->Stop(0);
                // handle->m_NNSSndHandle->MarkReadyForDeletion();
                // handle->m_NNSSndHandle = nullptr;
            }
            SEHandle::g_activeSEHandles.pop_back();
        }
    }
    void dssndUnloadSEAll() {
        debugPrintf("dssndUnloadSEAll\n");
        while (!SEHandle::g_activeSEHandles.empty()) {
            auto* handle = SEHandle::g_activeSEHandles.back();
            if (handle->m_NNSSndHandle != nullptr) {
                SEHandle::g_sePlayInfo.erase(handle->m_NNSSndHandle);
                handle->m_NNSSndHandle->Stop(0);
                // handle->m_NNSSndHandle->MarkReadyForDeletion();
                // handle->m_NNSSndHandle = nullptr;
            }
            SEHandle::g_activeSEHandles.pop_back();
        }
    }

    void dssndUnloadBGM1() {
        debugPrintf("dssndUnloadBGM\n");
        if (BGMHandle::g_activeBGMHandles.empty())
            return;
        auto* handle = BGMHandle::g_activeBGMHandles.back();
        if (handle->m_NNSSndHandle != nullptr) {
            handle->m_NNSSndHandle->Stop(0);
        }
        BGMHandle::g_activeBGMHandles.pop_back();
    }
    void dssndUnloadBGM2(s32 stacks) {
        debugPrintf("dssndUnloadBGM(%d)\n", stacks);
        if (stacks <= 0)
            stacks = 1;
        int count = std::min((size_t)stacks, BGMHandle::g_activeBGMHandles.size());
        for (int i = 0; i < count; i++) {
            if (BGMHandle::g_activeBGMHandles.empty())
                break;
            auto* handle = BGMHandle::g_activeBGMHandles.back();
            if (handle->m_NNSSndHandle != nullptr) {
                handle->m_NNSSndHandle->Stop(0);
            }
            BGMHandle::g_activeBGMHandles.pop_back();
        }
    }
    void dssndUnloadBGMAll() {
        debugPrintf("dssndUnloadBGMAll\n");
        while (!BGMHandle::g_activeBGMHandles.empty()) {
            auto* handle = BGMHandle::g_activeBGMHandles.back();
            if (handle->m_NNSSndHandle != nullptr) {
                handle->m_NNSSndHandle->Stop(0);
            }
            BGMHandle::g_activeBGMHandles.pop_back();
        }
    }
}

namespace egs {
    AKBTrack* m_tracks[2];
    bool m_isDoubleVoice;
    void GilbartInitialize(bool, bool isDoubleVoice) {
        debugPrintf("GilbartInitialize isDoubleVoice: %s\n", isDoubleVoice ? "true" : "false");
        m_tracks[0]     = new AKBTrack(TrackType_VOICE);
        m_tracks[1]     = new AKBTrack(TrackType_VOICE);
        m_isDoubleVoice = isDoubleVoice;
    }
    void GilbartTerminate() {
        debugPrintf("GilbartTerminate\n");
        delete m_tracks[0];
        delete m_tracks[1];
    }
    bool GilbartIsPlaying(int index) {
        debugPrintf("GilbartIsPlaying index: %d\n", index);
        return m_tracks[index]->IsPlaying();
    }
    bool GilbartIsEndOfPlayVoice(int index) {
        debugPrintf("GilbartIsEndOfPlayVoice index: %d\n", index);
        return !m_tracks[index]->IsActive();
    }
    void GilbartSetVolume(int index, float volume) {
        debugPrintf("GilbartSetVolume index: %d, volume: %.0f%%\n", index, volume * 100);
        m_tracks[index]->SetVolume(volume);
    }
    void GilbartPauseVoice(int index) {
        m_tracks[index]->Pause();
    }
    void GilbartPlayVoice(const char* file, int index) {
        debugPrintf("GilbartPlayVoice file: %s index: %d\n", file, index);
        m_tracks[index]->Load(file);
        m_tracks[index]->SetVolume(1.0f);
        m_tracks[index]->Play();
    }
    void GilbartStopVoice(int index) {
        debugPrintf("GilbartStopVoice index: %d\n", index);
        m_tracks[index]->Stop();
    }
    int GilbartStatus(int index) {
        debugPrintf("GilbartStatus index: %d\n", index);
        if (m_tracks[index]->IsPlaying())
            return 2;
        else
            return 1;
    }
    void GilbartProcess(void*) {
        debugPrintf("GilbartProcess\n");
    }
}

void NNS_SndUpdate() { }

void NNS_SndPlayerSetPlayerVolume(int playerNo, int volume) {
    debugPrintf("NNS_SndPlayerSetPlayerVolume playerNo: %d volume: %.0f%%\n", playerNo, std::clamp(volume, 0, 127) / 1.27f);
    AKBSystem::SetMasterVolume(static_cast<TrackType>(playerNo), std::clamp(volume, 0, 127) / 127.0f);
}

void NNS_SndPlayerStopSeqBySeqArcIdx(s32 seqArcNo, s32 index, s32 frames) {
    debugPrintf("NNS_SndPlayerStopSeqBySeqArcIdx seqArcNo: %d index: %d frames: %d\n", seqArcNo, index, frames);
    std::vector<AKBTrack*> toStop;
    for (const auto& kv : ds::snd::SEHandle::g_sePlayInfo) {
        if (kv.second.first == seqArcNo && kv.second.second == index) {
            toStop.push_back(kv.first);
        }
    }
    for (AKBTrack* track : toStop) {
        if (track != nullptr) {
            track->Stop(frames);
            track->MarkReadyForDeletion();
        }
        ds::snd::SEHandle::g_sePlayInfo.erase(track);

        ds::snd::SEHandle::SEHandle* owner = nullptr;
        for (auto* h : ds::snd::SEHandle::g_activeSEHandles) {
            if (h->m_NNSSndHandle == track) {
                owner = h;
                break;
            }
        }
        if (owner) {
            auto hit = std::find(ds::snd::SEHandle::g_activeSEHandles.begin(),
                ds::snd::SEHandle::g_activeSEHandles.end(), owner);
            if (hit != ds::snd::SEHandle::g_activeSEHandles.end())
                ds::snd::SEHandle::g_activeSEHandles.erase(hit);
            owner->m_NNSSndHandle = nullptr;
        }
    }
}

void patch() {
    HOOK_FUNCTION(CoreAudioOutInit);
    HOOK_FUNCTION(CoreAudioOutKick);
    HOOK_FUNCTION(CoreAudioOutQueueBuffer);
    HOOK_FUNCTION(CoreAudioOutRelease);
    HOOK_FUNCTION(CoreAudioOutResume);
    HOOK_FUNCTION(CoreAudioOutSuspend);
    HOOK_FUNCTION(NNS_SndUpdate);
    HOOK_FUNCTION(NNS_SndPlayerSetPlayerVolume);
    HOOK_FUNCTION(NNS_SndPlayerStopSeqBySeqArcIdx);

    HOOK_FUNCTION(ds::snd::BGMHandle::Ctor);
    HOOK_FUNCTION(ds::snd::BGMHandle::Dtor);
    HOOK_FUNCTION(ds::snd::BGMHandle::IsPlaying);
    HOOK_FUNCTION(ds::snd::BGMHandle::PlayV);
    HOOK_FUNCTION(ds::snd::BGMHandle::Play);
    HOOK_FUNCTION(ds::snd::BGMHandle::SetVolume);
    HOOK_FUNCTION(ds::snd::BGMHandle::MoveVolume);
    HOOK_FUNCTION(ds::snd::BGMHandle::Stop);
    HOOK_FUNCTION(ds::snd::BGMHandle::seqNo);
    HOOK_FUNCTION(ds::snd::BGMHandle::Pause);
    HOOK_FUNCTION(ds::snd::BGMHandle::setBGMEnable);
    HOOK_FUNCTION(ds::snd::BGMHandle::getBGMEnable);
    HOOK_FUNCTION(ds::snd::BGMHandle::forceReleaseSequence);
    HOOK_FUNCTION(ds::snd::BGMHandle::setTempo);

    HOOK_FUNCTION(ds::snd::SEHandle::Ctor);
    HOOK_FUNCTION(ds::snd::SEHandle::Dtor);
    HOOK_FUNCTION(ds::snd::SEHandle::IsPlaying);
    HOOK_FUNCTION(ds::snd::SEHandle::Play);
    HOOK_FUNCTION(ds::snd::SEHandle::PlayFadeIn);
    HOOK_FUNCTION(ds::snd::SEHandle::setPan);
    HOOK_FUNCTION(ds::snd::SEHandle::setPanRange);
    HOOK_FUNCTION(ds::snd::SEHandle::setPitch);
    HOOK_FUNCTION(ds::snd::SEHandle::setTempo);
    HOOK_FUNCTION(ds::snd::SEHandle::setVolume);
    HOOK_FUNCTION(ds::snd::SEHandle::MoveVolume);
    HOOK_FUNCTION(ds::snd::SEHandle::stop);

    HOOK_FUNCTION(egs::GilbartInitialize);
    HOOK_FUNCTION(egs::GilbartTerminate);
    HOOK_FUNCTION(egs::GilbartIsPlaying);
    HOOK_FUNCTION(egs::GilbartIsEndOfPlayVoice);
    HOOK_FUNCTION(egs::GilbartSetVolume);
    HOOK_FUNCTION(egs::GilbartPauseVoice);
    HOOK_FUNCTION(egs::GilbartPlayVoice);
    HOOK_FUNCTION(egs::GilbartStopVoice);
    HOOK_FUNCTION(egs::GilbartStatus);
    HOOK_FUNCTION(egs::GilbartProcess);

    HOOK_FUNCTION(ds::snd::dssndUnloadSE1);
    HOOK_FUNCTION(ds::snd::dssndUnloadSE2);
    HOOK_FUNCTION(ds::snd::dssndUnloadSEAll);

    // HOOK_FUNCTION(ds::snd::dssndUnloadBGM1);
    // HOOK_FUNCTION(ds::snd::dssndUnloadBGM2);
    // HOOK_FUNCTION(ds::snd::dssndUnloadBGMAll);
}
}