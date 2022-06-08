#define LockAudioDevice()   
#define UnlockAudioDevice() 

struct AudioDevice : public AudioDeviceBase {
    static bool32 Init();
    static void Release();

    static void ProcessAudioMixing(void *stream, int32 length);

    static void FrameInit();

    static void HandleStreamLoad(ChannelInfo *channel, bool32 async);


private:
    static void InitAudioChannels();
    static void InitMixBuffer();
};