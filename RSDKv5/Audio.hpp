#ifndef AUDIO_H
#define AUDIO_H

#define TRACK_COUNT (0x10)
#define SFX_COUNT   (0x100)
#if !RETRO_USE_ORIGINAL_CODE
#define CHANNEL_COUNT (0x10) // 4 in the original, 16 for convenience
#else
#define CHANNEL_COUNT (0x4)
#endif

#define MAX_VOLUME (100)

struct SFXInfo {
    uint hash[4];
    Sint16 *buffer;
    size_t length;
    byte maxConcurrentPlays;
    byte scope;
};

struct ChannelInfo {
    Sint16 *samplePtr;
    int pan;
    float volume;
    int speed;
    size_t sampleLength;
    int unknown1; 
    int unknown2; 
    int loop;
    int unknown3;
};


extern SFXInfo sfxList[SFX_COUNT];

bool32 InitAudioDevice();
void ReleaseAudioDevice();

#if RETRO_USING_SDL1 || RETRO_USING_SDL2
#if !RETRO_USE_ORIGINAL_CODE
// These functions did exist, but with different signatures
void ProcessMusicStream(Sint32 *stream, size_t bytes_wanted);
void ProcessAudioPlayback(void *data, Uint8 *stream, int len);
void ProcessAudioMixing(Sint32 *dst, const Sint16 *src, int len, int volume, sbyte pan);
#endif

#endif

void LoadSfx(char *filePath, byte plays, byte scope);
inline ushort GetSFX(const char* sfxName) {
    uint hash[4];
    GEN_HASH(sfxName, hash);

    for (int s = 0; s < SFX_COUNT; ++s) {
        if (HASH_MATCH(sfxList[s].hash, sfxName))
            return s;
    }

    return -1; 
}
void PlaySfx(int sfx, int loopPoint, int pan);
inline void StopSfx(int sfx)
{
    //for (int i = 0; i < CHANNEL_COUNT; ++i) {
    //    if (sfxChannels[i].sfxID == sfx) {
    //        MEM_ZERO(sfxChannels[i]);
    //        sfxChannels[i].sfxID = -1;
    //    }
    //}
}
void SetSfxAttributes(int sfx, int loopCount, sbyte pan);

#endif
