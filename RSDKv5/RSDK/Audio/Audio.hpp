#ifndef AUDIO_H
#define AUDIO_H

#define TRACK_COUNT (0x10)
#define SFX_COUNT   (0x100)
#if !RETRO_USE_ORIGINAL_CODE
#define CHANNEL_COUNT (0x10) // 4 in the original, 16 for convenience
#else
#define CHANNEL_COUNT (0x4)
#endif

#define MAX_VOLUME (1.0f)

#if RETRO_USING_SDL1 || RETRO_RENDERDEVICE_SDL2
#define LockAudioDevice()   SDL_LockAudio()
#define UnlockAudioDevice() SDL_UnlockAudio()
#else
#define LockAudioDevice()   ;
#define UnlockAudioDevice() ;
#endif

struct SFXInfo {
    uint32 hash[4];
    float *buffer;
    size_t length;
    int32 playCount;
    uint8 maxConcurrentPlays;
    uint8 scope;
    //I need this to fix sfx loopPoints
#if !RETRO_USE_ORIGINAL_CODE
    uint8 channelCount;
#endif
};

struct ChannelInfo {
    float *samplePtr;
    float pan;
    float volume;
    int32 speed;
    size_t sampleLength;
    int32 bufferPos;
    int32 playIndex;
    uint32 loop;
    int16 soundID;
    uint8 priority;
    uint8 state;
};

enum ChannelStates { CHANNEL_NONE, CHANNEL_SFX, CHANNEL_STREAMING, CHANNEL_STREAM_LOAD, CHANNEL_PAUSED = 0x40 };

extern SFXInfo sfxList[SFX_COUNT];
extern ChannelInfo channels[CHANNEL_COUNT];

extern bool32 audioEnabled;

bool32 InitAudioDevice();
void ReleaseAudioDevice();

#if RETRO_USING_SDL1 || RETRO_RENDERDEVICE_SDL2
#if !RETRO_USE_ORIGINAL_CODE
// These functions did exist, but with different signatures
void ProcessAudioPlayback(void *data, Uint8 *stream, int len);
void ProcessAudioMixing(float *dst, const float *src, int len, ChannelInfo *channel);
#endif

#endif

void LoadStream(ChannelInfo *channel);
int PlayStream(const char *filename, uint32 slot, int startPos, uint32 loopPoint, bool32 loadASync);

void LoadSfx(char *filePath, byte plays, byte scope);
bool32 LoadGlobalSfx();
inline uint16 GetSfx(const char *sfxName)
{
    uint hash[4];
    GEN_HASH(sfxName, hash);

    for (int s = 0; s < SFX_COUNT; ++s) {
        if (HASH_MATCH(sfxList[s].hash, hash))
            return s;
    }

    return -1;
}
int PlaySfx(uint16 sfx, uint32 loopPoint, uint32 priority);
inline void StopSfx(int sfx)
{
    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        if (channels[i].soundID == sfx) {
            MEM_ZERO(channels[i]);
            channels[i].soundID = -1;
            channels[i].state   = CHANNEL_NONE;
        }
    }
}
void SetChannelAttributes(uint8 channel, float volume, float panning, float speed);

inline void StopChannel(uint32 channel)
{
    if (channel < CHANNEL_COUNT) {
        if (channels[channel].state != CHANNEL_STREAM_LOAD)
            channels[channel].state = CHANNEL_NONE;
    }
}

inline void PauseChannel(uint32 channel)
{
    if (channel < CHANNEL_COUNT) {
        if (channels[channel].state != CHANNEL_STREAM_LOAD)
            channels[channel].state |= CHANNEL_PAUSED;
    }
}

inline void ResumeChannel(uint32 channel)
{
    if (channel < CHANNEL_COUNT) {
        if (channels[channel].state != CHANNEL_STREAM_LOAD)
            channels[channel].state &= ~CHANNEL_PAUSED;
    }
}

inline void PauseSound()
{
    for (int c = 0; c < CHANNEL_COUNT; ++c) 
        PauseChannel(c);
}

inline void ResumeSound()
{
    for (int c = 0; c < CHANNEL_COUNT; ++c) 
        ResumeChannel(c);
}

inline bool32 SfxPlaying(uint16 sfxID)
{
    for (int c = 0; c < CHANNEL_COUNT; ++c) {
        if (channels[c].state == CHANNEL_SFX && channels[c].soundID == sfxID)
            return true;
    }
    return false;
}

inline bool32 ChannelActive(uint32 channel)
{
    if (channel >= CHANNEL_COUNT)
        return false;
    else
        return (channels[channel].state & 0x3F) != CHANNEL_NONE;
}

uint GetChannelPos(uint32 channel);

#endif
