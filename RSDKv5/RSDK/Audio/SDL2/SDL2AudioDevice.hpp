
#define LockAudioDevice()   SDL_LockAudio()
#define UnlockAudioDevice() SDL_UnlockAudio()

struct SDL2AudioDevice : public AudioDeviceBase {
    static bool32 Init();
    static void Release();

    static void ClearStageSfx();

    static void ProcessAudioMixing(void *stream, int32 length);

    static void FrameInit();

    static void HandleStreamLoad(ChannelInfo *channel, bool32 async);

private:
    static void InitAudioChannels();
    static void InitMixBuffer();
};