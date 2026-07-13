#pragma once

#include <cstdint>
#include <string>
#include <unordered_set>

class AKBTrack;

enum TrackType : int
{
    TrackType_BGM   = 0,
    TrackType_SE    = 1,
    TrackType_VOICE = 2
};

class AKBSystem
{
  public:
    static bool Init();
    static void Update();
    static void Quit();

    static void Lock();
    static void Unlock();

    static void SetMasterVolume(TrackType type, float volume);

    static int GetDeviceRate();
    static int GetDeviceChannels();
    static void PushRawPCM(const int16_t* samples, int frames);
    static void ClearRawPCM();
    static void SetRawPCMVolume(float volume);
    static int GetRawPCMAvailableFrames();

  private:
    friend class AKBTrack;
    static void RegisterTrack(AKBTrack* track);
    static void UnregisterTrack(AKBTrack* track);

  public:
    static std::unordered_set<AKBTrack*> shouldBeMonitored;
    static void PrintTracks();
};

class AKBTrack
{
  public:
    std::string path;

    TrackType type;

    AKBTrack(TrackType type);
    ~AKBTrack();

    bool Load(const std::string& path);
    void Unload();
    void MarkReadyForDeletion();

    void Play();
    void Reset();
    void Stop(int fadeFrames = 0);
    void Pause(int fadeFrames = 0);
    void TempStop(int fadeFrames = 0);
    void Resume();
    void SetTempo(int range);
    void SetPan(int pan);
    void SetPanRange(int panRange);
    void SetPitch(int pitch);
    void SetVolume(int volume);
    void SetVolume(float volume);
    void MoveVolume(int targetVolume, int frames);

    bool IsActive() const;
    bool IsPlaying() const;
    bool IsPaused() const;

    friend class AKBSystem;

    void* vorbis       = nullptr;
    uint8_t* fileData  = nullptr;
    int fileDataLen    = 0;
    uint32_t loopStart = 0;
    uint32_t loopEnd   = 0;
    bool loop          = true;
    int streamFramePos = 0;

    bool active = false;
    bool paused = false;

    int pan      = 0;
    int panRange = 127;
    int pitch    = 0;
    int tempo    = 1;

    float volume           = 1.0f;
    float volTarget        = 1.0f;
    float volStep          = 0.0f;
    int volFadeLeft        = 0;
    bool stopOnSilence     = false;
    bool pauseOnSilence    = false;
    bool tempStopOnSilence = false;
};