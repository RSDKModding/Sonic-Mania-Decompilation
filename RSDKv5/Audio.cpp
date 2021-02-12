#include "RetroEngine.hpp"

#if !RETRO_USE_ORIGINAL_CODE
//MusicPlaybackInfo musInfo;
#endif

SFXInfo sfxList[SFX_COUNT];

bool audioEnabled = false;

#if RETRO_USING_SDL1 || RETRO_USING_SDL2

#if RETRO_USING_SDL2
SDL_AudioDeviceID audioDevice;
#endif
SDL_AudioSpec audioDeviceFormat;

#define LOCK_AUDIO_DEVICE()   SDL_LockAudio();
#define UNLOCK_AUDIO_DEVICE() SDL_UnlockAudio();

#define AUDIO_FREQUENCY (44100)
#define AUDIO_FORMAT    (AUDIO_S16SYS) /**< Signed 16-bit samples */
#define AUDIO_SAMPLES   (0x800)
#define AUDIO_CHANNELS  (2)

#define ADJUST_VOLUME(s, v) (s = (s * v) / MAX_VOLUME)

#else
#define LOCK_AUDIO_DEVICE()   ;
#define UNLOCK_AUDIO_DEVICE() ;
#endif

#define MIX_BUFFER_SAMPLES (256)

bool InitAudioDevice()
{
    //StopAllSfx(); //"init"
#if !RETRO_USE_ORIGINAL_CODE
#if RETRO_USING_SDL1 || RETRO_USING_SDL2
    SDL_AudioSpec want;
    want.freq     = AUDIO_FREQUENCY;
    want.format   = AUDIO_FORMAT;
    want.samples  = AUDIO_SAMPLES;
    want.channels = AUDIO_CHANNELS;
    want.callback = ProcessAudioPlayback;

#if RETRO_USING_SDL2
    if ((audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, &audioDeviceFormat, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE)) > 0) {
        audioEnabled = true;
        SDL_PauseAudioDevice(audioDevice, 0);
    }
    else {
        printLog(SEVERITY_ERROR, "Unable to open audio device: %s", SDL_GetError());
        audioEnabled = false;
        return true; // no audio but game wont crash now
    }
#elif RETRO_USING_SDL1
    if (SDL_OpenAudio(&want, &audioDeviceFormat) == 0) {
        audioEnabled = true;
        SDL_PauseAudio(0);
    }
    else {
        printLog("Unable to open audio device: %s", SDL_GetError());
        audioEnabled = false;
        return true; // no audio but game wont crash now
    }
#endif // !RETRO_USING_SDL1
#endif
#endif
    //for (int i = 0; i < CHANNEL_COUNT; ++i) sfxChannels[i].sfxID = -1;

    return true;
}
void ReleaseAudioDevice() {}

void ProcessMusicStream(Sint32 *stream, size_t bytes_wanted) {}
void ProcessAudioPlayback(void *data, Uint8 *stream, int len) {}
void ProcessAudioMixing(Sint32 *dst, const Sint16 *src, int len, int volume, sbyte pan) {}

void LoadSfx(char *filePath, byte plays, byte scope)
{
    if (!audioEnabled)
        return;

    FileInfo info;
    MEM_ZERO(info);
    char fullPath[0x80];

    StrCopy(fullPath, "Data/SoundFX/");
    StrAdd(fullPath, filePath);

    uint hash[4];
    GEN_HASH(filePath, hash);

    ushort id = -1;
    for (id = 0; id < SFX_COUNT; ++id) {
        if (sfxList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SFX_COUNT)
        return;

    if (LoadFile(&info, fullPath)) {
#if !RETRO_USE_ORIGINAL_CODE
        byte type = fullPath[StrLength(fullPath) - 3];
        if (type == 'w') {
            byte *sfx = new byte[info.fileSize];
            ReadBytes(&info, sfx, info.fileSize);
            CloseFile(&info);

            SDL_RWops *src = SDL_RWFromMem(sfx, info.fileSize);
            if (src == NULL) {
                printLog(SEVERITY_ERROR, "Unable to open sfx: %s", fullPath);
            }
            else {
                SDL_AudioSpec wav_spec;
                uint wav_length;
                byte *wav_buffer;
                SDL_AudioSpec *wav = SDL_LoadWAV_RW(src, 0, &wav_spec, &wav_buffer, &wav_length);

                SDL_RWclose(src);
                delete[] sfx;
                if (wav == NULL) {
                    printLog(SEVERITY_ERROR, "Unable to read sfx: %s", fullPath);
                }
                else {
                    SDL_AudioCVT convert;
                    if (SDL_BuildAudioCVT(&convert, wav->format, wav->channels, wav->freq, audioDeviceFormat.format, audioDeviceFormat.channels,
                                          audioDeviceFormat.freq)
                        > 0) {
                        convert.buf = (byte *)malloc(wav_length * convert.len_mult);
                        convert.len = wav_length;
                        memcpy(convert.buf, wav_buffer, wav_length);
                        SDL_ConvertAudio(&convert);

                        LOCK_AUDIO_DEVICE()
                        GEN_HASH(filePath, sfxList[id].hash);
                        AllocateStorage(convert.len_cvt, (void **)&sfxList[id].buffer, DATASET_SFX, false);
                        memcpy(sfxList[id].buffer, convert.buf, convert.len_cvt);
                        sfxList[id].length      = convert.len_cvt / sizeof(Sint16);
                        sfxList[id].scope  = scope;
                        UNLOCK_AUDIO_DEVICE()
                        SDL_FreeWAV(wav_buffer);
                        free(convert.buf);
                    }
                    else {
                        LOCK_AUDIO_DEVICE()
                        GEN_HASH(filePath, sfxList[id].hash);
                        AllocateStorage(wav_length, (void **)&sfxList[id].buffer, DATASET_SFX, false);
                        memcpy(sfxList[id].buffer, wav_buffer, wav_length);
                        sfxList[id].length      = wav_length / sizeof(Sint16);
                        sfxList[id].scope  = scope;
                        UNLOCK_AUDIO_DEVICE()
                    }
                }
            }
        }
        else {
            // wtf lol
            CloseFile(&info);
            printLog(SEVERITY_ERROR, "Sfx format not supported!");
        }
#endif
    }
}
void PlaySfx(int sfx, int loopPoint, int pan) {}
void SetSfxAttributes(int sfx, int loopCount, sbyte pan) {}