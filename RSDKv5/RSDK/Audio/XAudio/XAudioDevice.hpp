
#define LockAudioDevice()   EnterCriticalSection(&AudioDevice::criticalSection)
#define UnlockAudioDevice() LeaveCriticalSection(&AudioDevice::criticalSection)

// DX9
class AudioDeviceCallback : public IXAudio2VoiceCallback
{
    void WINAPI OnVoiceProcessingPassStart(UINT32 BytesRequired) {}
    void WINAPI OnVoiceProcessingPassEnd() {}
    void WINAPI OnStreamEnd() {}
    void WINAPI OnBufferStart(void *pBufferContext) {}
    void WINAPI OnBufferEnd(void *pBufferContext);
    void WINAPI OnLoopEnd(void *pBufferContext) {}
    void WINAPI OnVoiceError(void *pBufferContext, HRESULT Error) {}
};

class AudioEngineCallback : public IXAudio2EngineCallback
{
    void WINAPI OnProcessingPassStart() {}
    void WINAPI OnProcessingPassEnd() {}
    void WINAPI OnCriticalError(HRESULT Error);
};

struct AudioDevice : public AudioDeviceBase {
    static bool32 Init();
    static void Release();

    static void ProcessAudioMixing(void *stream, int32 length);

    static void FrameInit();

    static void HandleStreamLoad(ChannelInfo *channel, bool32 async);

    static uint8 contextInitialized;

    static IXAudio2 *audioContext;
    static IXAudio2MasteringVoice *masteringVoice;
    static IXAudio2SourceVoice *sourceVoice;
    static AudioDeviceCallback voiceCallback;
    static AudioEngineCallback engineCallback;

    static RTL_CRITICAL_SECTION criticalSection;

private:
    static void InitAudioChannels();
    static void InitMixBuffer();
    static HRESULT InitContext();

    DWORD static WINAPI LoadStreamASync(LPVOID channel)
    {
        LoadStream((ChannelInfo *)channel);
        return 0;
    }
};