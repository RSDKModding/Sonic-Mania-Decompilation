#ifndef AUDIO_H
#define AUDIO_H

#define TRACK_COUNT   (0x10)
#define SFX_COUNT     (0x100)
#define CHANNEL_COUNT (0x10)

#define MIX_BUFFER_SIZE (0x800)
#define SAMPLE_FORMAT   float

struct SFXInfo {
    uint32 hash[4];
    float *buffer;
    size_t length;
    int32 playCount;
    uint8 maxConcurrentPlays;
    uint8 scope;
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

struct AudioDeviceBase {
    static bool32 Init();
    static void Release();

    static void ClearStageSfx();

    static void ProcessAudioMixing(void *stream, int32 length);

    static void FrameInit();

    static void HandleStreamLoad(ChannelInfo *channel, bool32 async);

    static uint8 initializedAudioChannels;
    static uint8 audioState;

    static int32 mixBufferID;
    static float mixBuffer[3][MIX_BUFFER_SIZE];

private:
    static void InitAudioChannels();
    static void InitMixBuffer();
};

void UpdateStreamBuffer(ChannelInfo *channel);
void LoadStream(ChannelInfo *channel);
int32 PlayStream(const char *filename, uint32 slot, int32 startPos, uint32 loopPoint, bool32 loadASync);

void ReadSfx(char *filename, uint8 id, uint8 plays, uint8 scope, uint32 *size, uint32 *format, uint16 *channels, uint32 *freq);
void LoadSfx(char *filePath, uint8 plays, uint8 scope);

inline uint16 GetSfx(const char *sfxName)
{
    uint32 hash[4];
    GEN_HASH(sfxName, hash);

    for (int32 s = 0; s < SFX_COUNT; ++s) {
        if (HASH_MATCH(sfxList[s].hash, hash))
            return s;
    }

    return -1;
}
int32 PlaySfx(uint16 sfx, uint32 loopPoint, uint32 priority);
inline void StopSfx(int32 sfx)
{
    for (int32 i = 0; i < CHANNEL_COUNT; ++i) {
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
    for (int32 c = 0; c < CHANNEL_COUNT; ++c) PauseChannel(c);
}

inline void ResumeSound()
{
    for (int32 c = 0; c < CHANNEL_COUNT; ++c) ResumeChannel(c);
}

inline bool32 SfxPlaying(uint16 sfxID)
{
    for (int32 c = 0; c < CHANNEL_COUNT; ++c) {
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

uint32 GetChannelPos(uint32 channel);

#if RETRO_AUDIODEVICE_XAUDIO
#include "XAudio/XAudioDevice.hpp"
#elif RETRO_AUDIODEVICE_SDL2
#include "SDL2/SDL2RenderDevice.hpp"
#endif

#endif
