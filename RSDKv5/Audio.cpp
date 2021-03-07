#include "RetroEngine.hpp"

#if !RETRO_USE_ORIGINAL_CODE
//MusicPlaybackInfo musInfo;
#endif

SFXInfo sfxList[SFX_COUNT];
ChannelInfo channels[CHANNEL_COUNT];

bool32 audioEnabled = false;

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

bool32 InitAudioDevice()
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
        PrintLog("Unable to open audio device: %s", SDL_GetError());
        audioEnabled = false;
        return true; // no audio but game wont crash now
    }
#endif // !RETRO_USING_SDL1
#endif
#endif

    
    FileInfo info;
    MEM_ZERO(info);

    if (LoadFile(&info, "Data/Game/GameConfig.bin")) {
        char buffer[0x100];
        uint sig = ReadInt32(&info);

        if (sig != 0x474643) {
            CloseFile(&info);
            return true;
        }

        ReadString(&info, buffer);
        ReadString(&info, buffer);
        ReadString(&info, buffer);

        ReadInt8(&info);
        ReadInt16(&info);

        byte objCnt = ReadInt8(&info);
        for (int i = 0; i < objCnt; ++i) {
            ReadString(&info, buffer);
        }

        for (int i = 0; i < PALETTE_COUNT; ++i) {
            byte rows = ReadInt16(&info);
            for (int r = 0; r < 0x10; ++r) {
                if ((rows >> r & 1)) {
                    for (int c = 0; c < 0x10; ++c) {
                        ReadInt8(&info);
                        ReadInt8(&info);
                        ReadInt8(&info);
                    }
                }
            }
        }

        byte sfxCnt = ReadInt8(&info);
        for (int i = 0; i < sfxCnt; ++i) {
            ReadString(&info, buffer);
            byte maxConcurrentPlays = ReadInt8(&info);
            LoadSfx(buffer, maxConcurrentPlays, SCOPE_GLOBAL);
        }
        CloseFile(&info);
    }

    //for (int i = 0; i < CHANNEL_COUNT; ++i) channels[i].sfxID = -1;

    return true;
}
void ReleaseAudioDevice() {}

size_t readVorbis(void *mem, size_t size, size_t nmemb, void *ptr)
{
    FileInfo *info = (FileInfo *)ptr;
    return ReadBytes(info, mem, (int)(size * nmemb));
}
int seekVorbis(void *ptr, int64 offset, int whence)
{
    FileInfo *info = (FileInfo *)ptr;
    switch (whence) {
        case SEEK_SET: whence = 0; break;
        case SEEK_CUR: whence = info->readPos; break;
        case SEEK_END: whence = info->fileSize; break;
        default: break;
    }
    Seek_Set(info, (int)(whence + offset));
    return info->readPos;
}
long tellVorbis(void *ptr)
{
    FileInfo *info = (FileInfo *)ptr;
    return info->readPos;
}
int closeVorbis(void *ptr)
{
    CloseFile((FileInfo *)ptr);
    return 1;
}

void ProcessMusicStream(Sint32 *stream, size_t bytes_wanted)
{
    if (!engine.streamsEnabled)
        return;

    //int musVol = engine.streamVolume * MAX_VOLUME;
    /*if (!musInfo.loaded)
        return;
    switch (musicStatus) {
        case MUSIC_READY:
        case MUSIC_PLAYING: {
#if RETRO_USING_SDL2
            while (SDL_AudioStreamAvailable(musInfo.stream) < bytes_wanted) {
                // We need more samples: get some
                long bytes_read = ov_read(&musInfo.vorbisFile, (char *)musInfo.buffer, sizeof(musInfo.buffer), 0, 2, 1, &musInfo.vorbBitstream);

                if (bytes_read == 0) {
                    // We've reached the end of the file
                    if (musInfo.trackLoop) {
                        ov_pcm_seek(&musInfo.vorbisFile, musInfo.loopPoint);
                        continue;
                    }
                    else {
                        musicStatus = MUSIC_STOPPED;
                        break;
                    }
                }

                if (SDL_AudioStreamPut(musInfo.stream, musInfo.buffer, bytes_read) == -1)
                    return;
            }

            // Now that we know there are enough samples, read them and mix them
            int bytes_done = SDL_AudioStreamGet(musInfo.stream, musInfo.buffer, bytes_wanted);
            if (bytes_done == -1) {
                return;
            }
            if (bytes_done != 0)
                ProcessAudioMixing(stream, musInfo.buffer, bytes_done / sizeof(Sint16), musVol, 0);
#endif
            break;
        }
        case MUSIC_STOPPED:
        case MUSIC_PAUSED:
        case MUSIC_LOADING:
            // dont play
            break;
    }*/
}
void ProcessAudioPlayback(void *data, Uint8 *stream, int len) {

    (void)data; // Unused

    if (!audioEnabled)
        return;

    /*if (musicStatus == MUSIC_LOADING) {
        if (trackBuffer < 0 || trackBuffer >= TRACK_COUNT) {
            StopMusic();
            return;
        }

        TrackInfo *trackPtr = &musicTracks[trackBuffer];

        if (!trackPtr->fileName[0]) {
            StopMusic();
            return;
        }

        uint oldPos   = 0;
        uint oldTotal = 0;
        if (musInfo.loaded) {
            oldPos   = (uint)ov_pcm_tell(&musInfo.vorbisFile);
            oldTotal = (uint)ov_pcm_total(&musInfo.vorbisFile, -1);
            StopMusic();
        }

        if (LoadFile2(trackPtr->fileName, &musInfo.fileInfo)) {
            musInfo.trackLoop = trackPtr->trackLoop;
            musInfo.loopPoint = trackPtr->loopPoint;
            musInfo.loaded    = true;

            unsigned long long samples = 0;
            ov_callbacks callbacks;

            callbacks.read_func  = readVorbis;
            callbacks.seek_func  = seekVorbis;
            callbacks.tell_func  = tellVorbis;
            callbacks.close_func = closeVorbis;

            int error = ov_open_callbacks(&musInfo, &musInfo.vorbisFile, NULL, 0, callbacks);
            if (error != 0) {
            }

            musInfo.vorbBitstream = -1;
            musInfo.vorbisFile.vi = ov_info(&musInfo.vorbisFile, -1);

            samples = (unsigned long long)ov_pcm_total(&musInfo.vorbisFile, -1);

#if RETRO_USING_SDL2
            musInfo.stream = SDL_NewAudioStream(AUDIO_S16, musInfo.vorbisFile.vi->channels, musInfo.vorbisFile.vi->rate, audioDeviceFormat.format,
                                                audioDeviceFormat.channels, audioDeviceFormat.freq);
            if (!musInfo.stream) {
                PrintLog("Failed to create stream: %s", SDL_GetError());
            }
#endif

#if RETRO_USING_SDL1
            musInfo.spec.format   = AUDIO_S16;
            musInfo.spec.channels = musInfo.vorbisFile.vi->channels;
            musInfo.spec.freq     = (int)musInfo.vorbisFile.vi->rate;
#endif

            musInfo.buffer = new Sint16[MIX_BUFFER_SAMPLES];

            if (musicStartPos) {
                float newPos  = oldPos * ((float)musicRatio * 0.0001); // 8000 == 0.8 (ratio / 10,000)
                musicStartPos = fmod(newPos, samples);

                ov_pcm_seek(&musInfo.vorbisFile, musicStartPos);
            }
            musicStartPos = 0;

            musicStatus  = MUSIC_PLAYING;
            masterVolume = MAX_VOLUME;
            trackID      = trackBuffer;
            trackBuffer  = -1;
        }
    }*/

    Sint16 *output_buffer = (Sint16 *)stream;

    size_t samples_remaining = (size_t)len / sizeof(Sint16);
    while (samples_remaining != 0) {
        Sint32 mix_buffer[MIX_BUFFER_SAMPLES];
        memset(mix_buffer, 0, sizeof(mix_buffer));

        const size_t samples_to_do = (samples_remaining < MIX_BUFFER_SAMPLES) ? samples_remaining : MIX_BUFFER_SAMPLES;

        // Mix music
        ProcessMusicStream(mix_buffer, samples_to_do * sizeof(Sint16));

        int sfxVol = engine.soundFXVolume * MAX_VOLUME;
        // Mix SFX
        for (byte i = 0; i < CHANNEL_COUNT; ++i) {
            ChannelInfo *sfx = &channels[i];
            if (sfx == NULL)
                continue;

            if (sfx->soundID < 0 || sfx->soundID == 0xFF)
                continue;

            if (sfx->samplePtr) {
                Sint16 buffer[MIX_BUFFER_SAMPLES];

                size_t samples_done = 0;
                while (samples_done != samples_to_do) {
                    size_t sampleLen = (sfx->sampleLength < samples_to_do - samples_done) ? sfx->sampleLength : samples_to_do - samples_done;
                    memcpy(&buffer[samples_done], sfx->samplePtr, sampleLen * sizeof(Sint16));

                    samples_done += sampleLen;
                    sfx->samplePtr += sampleLen;
                    sfx->sampleLength -= sampleLen;

                    if (sfx->sampleLength == 0) {
                        if (sfx->loop == 1) {
                            sfx->samplePtr    = sfxList[sfx->loop].buffer;
                            sfx->sampleLength = sfxList[sfx->loop].length;
                        }
                        else if (sfx->loop) {
                            sfx->samplePtr    = sfxList[sfx->loop].buffer + sfx->loop;
                            sfx->sampleLength = sfxList[sfx->loop].length - sfx->loop;
                        }
                        else {
                            StopSfx(sfx->loop);
                            break;
                        }
                    }
                }

#if RETRO_USING_SDL2
                ProcessAudioMixing(mix_buffer, buffer, samples_done, sfxVol, sfx->pan);
#endif
            }
        }

        // Clamp mixed samples back to 16-bit and write them to the output buffer
        for (size_t i = 0; i < sizeof(mix_buffer) / sizeof(*mix_buffer); ++i) {
            const Sint16 max_audioval = ((1 << (16 - 1)) - 1);
            const Sint16 min_audioval = -(1 << (16 - 1));

            const Sint32 sample = mix_buffer[i];

            if (sample > max_audioval)
                *output_buffer++ = max_audioval;
            else if (sample < min_audioval)
                *output_buffer++ = min_audioval;
            else
                *output_buffer++ = sample;
        }

        samples_remaining -= samples_to_do;
    }
}
void ProcessAudioMixing(Sint32 *dst, const Sint16 *src, int len, int volume, sbyte pan)
{
    if (volume == 0)
        return;

    if (volume > MAX_VOLUME)
        volume = MAX_VOLUME;

    float panL = 0.0;
    float panR = 0.0;
    int i      = 0;

    if (pan < 0) {
        panR = 1.0f - abs(pan / 100.0f);
        panL = 1.0f;
    }
    else if (pan > 0) {
        panL = 1.0f - abs(pan / 100.0f);
        panR = 1.0f;
    }

    while (len--) {
        Sint32 sample = *src++;
        ADJUST_VOLUME(sample, volume);

        if (pan != 0) {
            if ((i % 2) != 0) {
                sample *= panR;
            }
            else {
                sample *= panL;
            }
        }

        *dst++ += sample;

        i++;
    }
}

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
int PlaySfx(ushort sfx, uint loopPoint, int unknown)
{
    if (sfx >= SFX_COUNT)
        return -1;
    if (!sfxList[sfx].scope)
        return -1;

    byte count = 0;
    for (int c = 0; c < CHANNEL_COUNT; ++c) {
        if (channels[c].soundID == sfx)
            ++count;
    }

    sbyte slot = -1;
    //if we've hit the max, replace one
    if (count >= sfxList[sfx].maxConcurrentPlays) {
        for (int c = 0; c < CHANNEL_COUNT; ++c) {
            if (channels[c].soundID == sfx) {
                slot = c;
                break;
            }
        }
    }
    else {
        bool32 flag = false;
        for (int c = 0; c < CHANNEL_COUNT && !flag; ++c) {
            if (channels[c].soundID == -1 && channels[c].state != 3) {
                slot = c;
            }
        }

        int len = -1;
        for (int c = 0; c < CHANNEL_COUNT && !flag; ++c) {
            if (channels[c].sampleLength < len && channels[c].state <= unknown && channels[c].state != 3) {
                slot = c;
            }
        }
    }

    channels[slot].state        = 1;
    channels[slot].unknown1     = 0;
    channels[slot].samplePtr    = sfxList[sfx].buffer;
    channels[slot].sampleLength = sfxList[sfx].length;
    channels[slot].volume       = 1.0;
    channels[slot].pan          = 0.0;
    channels[slot].speed        = 1 << 0x10;
    channels[slot].soundID      = sfx;
    if (loopPoint >= 2)
        channels[slot].loop = loopPoint;
    else
        channels[slot].loop = loopPoint != 0 ? 1 : 0;
    channels[slot].unknown3 = unknown;
    channels[slot].unknown2 = sfxList[sfx].unknown;
    return slot;
}

void SetSoundAttributes(sbyte a1, byte slot, float volume, float panning, float speed)
{
    if (slot < CHANNEL_COUNT) {
        float vol = 0.0;
        if (volume >= 0.0)
            vol = fminf(4.0, volume);
        float pan = -1.0f;
        if (panning >= -1.0)
            pan = fminf(1.0, panning);
        channels[slot].volume    = vol;
        //v9              = speed < 1.0;
        //v13             = 0;
        //v10             = speed == 1.0;
        //v11             = 0;
        //v12             = 0;
        channels[slot].pan = pan;
        //BYTE1(result)   = a1;
        // channels[slot].unknown3 = a1; // Maybe ??
        if (speed > 0.0) {
            channels[slot].speed = (int)(speed * 65536.0f);
        }
        else if (speed == 1.0) {
            channels[slot].speed = 0x10000;
        }
    }
}
