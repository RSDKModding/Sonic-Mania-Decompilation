
#define LockAudioDevice()   ;
#define UnlockAudioDevice() ;

class AudioDevice : public AudioDeviceBase
{
public:
    static bool32 Init();
    static void Release();

    static void ProcessAudioMixing(void *stream, int32 length);

    static void FrameInit() {}

    inline static void HandleStreamLoad(ChannelInfo *channel, bool32 async) { LoadStream(channel); }

private:
    static uint8 contextInitialized;

    static void InitAudioChannels();
    static void InitMixBuffer() {}
};
