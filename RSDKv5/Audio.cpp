#include "RetroEngine.hpp"

#if !RETRO_USE_ORIGINAL_CODE
// MusicPlaybackInfo musInfo;
#endif

SFXInfo sfxList[SFX_COUNT];
ChannelInfo channels[CHANNEL_COUNT];

bool32 audioEnabled = false;

struct StreamInfo {
    char filename[0x100];
    byte *fileBuffer;
    int fileSize;
    float *buffer;
    int bufferSize;
    int startPos;
    int loopPoint;
    int bufferPos;

    bool32 loaded;
    OggVorbis_File vorbisFile;
    int vorbBitstream;
#if RETRO_USING_SDL2
    SDL_AudioStream *musicStream;
#endif
};

StreamInfo streamInfo;

float speedMixAmounts[0x400];

#if RETRO_USING_SDL1 || RETRO_USING_SDL2

#if RETRO_USING_SDL2
SDL_AudioDeviceID audioDevice;
#endif
SDL_AudioSpec audioDeviceFormat;

#define AUDIO_FREQUENCY (44100)
#define AUDIO_FORMAT    (AUDIO_F32SYS) /**< Floating-Point samples */
#define AUDIO_SAMPLES   (0x800)
#define AUDIO_CHANNELS  (2)

#define ADJUST_VOLUME(s, v) (s = (s * v) / MAX_VOLUME)

#else
#define LockAudioDevice()   ;
#define UnlockAudioDevice() ;
#endif

#define MIX_BUFFER_SAMPLES (256)

bool32 InitAudioDevice()
{
    // StopAllSfx(); //"init"
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
        printLog(PRINT_ERROR, "Unable to open audio device: %s", SDL_GetError());
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
    InitFileInfo(&info);

    if (LoadFile(&info, "Data/Game/GameConfig.bin", FMODE_RB)) {
        char buffer[0x100];
        uint sig = ReadInt32(&info, false);

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

    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        channels[i].soundID = -1;
        channels[i].state   = CHANNEL_NONE;
    }

    for (int i = 0; i < 0x400; i += 2) {
        speedMixAmounts[i]     = (float)i * (1.0f / 1024.0f);
        speedMixAmounts[i + 1] = (float)(i + 1) * (1.0f / 1024.0f); // 0.00097656;
    }

    GEN_HASH("Stream Channel 0", sfxList[SFX_COUNT - 1].hash);
    sfxList[SFX_COUNT - 1].scope              = SCOPE_GLOBAL;
    sfxList[SFX_COUNT - 1].maxConcurrentPlays = 1;
    sfxList[SFX_COUNT - 1].length             = 2048;
    AllocateStorage(0x2000, (void **)&sfxList[SFX_COUNT - 1].buffer, DATASET_MUS, false);

    return true;
}
void ReleaseAudioDevice()
{
    LockAudioDevice();

    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        MEM_ZERO(channels[i]);
        channels[i].soundID = -1;
        channels[i].state   = CHANNEL_NONE;
    }

    for (int i = 0; i < SFX_COUNT; ++i) {
        MEM_ZERO(sfxList[i]);
    }

    if (streamInfo.loaded) {
#if RETRO_USING_SDL2
        if (streamInfo.musicStream)
            SDL_FreeAudioStream(streamInfo.musicStream);
#endif

        ov_clear(&streamInfo.vorbisFile);
    }

    MEM_ZERO(streamInfo);

    UnlockAudioDevice();
}

size_t readVorbis(void *mem, size_t size, size_t nmemb, void *ptr)
{
    StreamInfo *info = (StreamInfo *)ptr;
    size_t bytes     = (int)(size * nmemb) * sizeof(byte);
    if (info->bufferPos + bytes >= info->fileSize)
        bytes = info->fileSize - info->bufferPos;

    memcpy(mem, &info->fileBuffer[info->bufferPos], (int)(size * nmemb) * sizeof(byte));
    info->bufferPos += bytes;
    return bytes;
}
int seekVorbis(void *ptr, ogg_int64_t offset, int whence)
{
    StreamInfo *info = (StreamInfo *)ptr;
    switch (whence) {
        case SEEK_SET: whence = 0; break;
        case SEEK_CUR: whence = info->bufferPos; break;
        case SEEK_END: whence = info->fileSize; break;
        default: break;
    }
    info->bufferPos = (int)(whence + offset);
    return info->bufferPos;
}
long tellVorbis(void *ptr)
{
    StreamInfo *info = (StreamInfo *)ptr;
    return info->bufferPos;
}
int closeVorbis(void *ptr) { return 1; }

void ProcessAudioPlayback(void *data, Uint8 *stream, int len)
{
    (void)data; // Unused

    if (!audioEnabled)
        return;

    float *output_buffer = (float *)stream;

    size_t samples_remaining = (size_t)len / sizeof(float);
    while (samples_remaining != 0) {
        float mix_buffer[MIX_BUFFER_SAMPLES];
        memset(mix_buffer, 0, sizeof(mix_buffer));

        const size_t samples_to_do = (samples_remaining < MIX_BUFFER_SAMPLES) ? samples_remaining : MIX_BUFFER_SAMPLES;

        // Mix Sounds
        for (byte i = 0; i < CHANNEL_COUNT; ++i) {
            ChannelInfo *channel = &channels[i];

            switch (channel->state) {
                default:
                case CHANNEL_NONE: break;
                case CHANNEL_SFX: {
                    if (channel->soundID < 0)
                        continue;

                    if (channel->samplePtr) {
                        float buffer[MIX_BUFFER_SAMPLES];
                        memset(buffer, 0, MIX_BUFFER_SAMPLES * sizeof(float));

                        size_t samples_done = 0;
                        int spdVal          = 0;
                        bool32 flag         = false;

                        while (samples_done != samples_to_do && !flag) {
                            size_t remain    = channel->sampleLength - channel->bufferPos;
                            size_t sampleLen = (remain < samples_to_do - samples_done) ? remain : samples_to_do - samples_done;

                            size_t outPos   = samples_done;
                            float *soundBuf = &channel->samplePtr[channel->bufferPos];
                            for (int s = 0; s < sampleLen && !flag; ++s) {
                                float mixAmount = speedMixAmounts[spdVal >> 6];
                                spdVal += channel->speed;
                                float bufVal = soundBuf[0] + ((soundBuf[1] - soundBuf[0]) * mixAmount);
                                soundBuf += spdVal >> 16;

                                buffer[outPos++] += bufVal;
                                channel->bufferPos += spdVal >> 16;

                                samples_done += 1;
                                spdVal &= 0xFFFF;

                                if (channel->bufferPos >= channel->sampleLength) {
                                    if (channel->loop == 1) {
                                        channel->bufferPos    = 0;
                                        channel->sampleLength = channel->sampleLength;
                                    }
                                    else if (channel->loop) {
                                        channel->bufferPos    = channel->loop;
                                        channel->sampleLength = channel->sampleLength;
                                    }
                                    else {
                                        MEM_ZEROP(channel);
                                        channel->soundID = -1;
                                        channel->state   = CHANNEL_NONE;
                                        flag             = true;
                                    }
                                }
                            }
                        }

#if RETRO_USING_SDL2
                        ProcessAudioMixing(mix_buffer, buffer, (int)samples_done, channel);
#endif
                    }
                    break;
                }
                case CHANNEL_STREAMING: {
                    if (channel->soundID < 0)
                        continue;

                    size_t samples_gotten = 0;
                    int spdVal            = 0;
                    for (int s = 0; s < samples_to_do; ++s) {
                        spdVal += channel->speed;
                        samples_gotten += spdVal >> 16;
                        spdVal &= 0xFFFF;
                    }

                    size_t bytes_wanted = samples_gotten * sizeof(float);

#if RETRO_USING_SDL2
                    while (SDL_AudioStreamAvailable(streamInfo.musicStream) < bytes_wanted) {
                        // We need more samples: get some
                        long bytes_read =
                            ov_read(&streamInfo.vorbisFile, (char *)streamInfo.buffer, sizeof(streamInfo.buffer), 0, 2, 1, &streamInfo.vorbBitstream);

                        if (bytes_read == 0) {
                            // We've reached the end of the file
                            if (channel->loop == 1) {
                                ov_pcm_seek(&streamInfo.vorbisFile, 0);
                                continue;
                            }
                            else if (channel->loop > 1) {
                                ov_pcm_seek(&streamInfo.vorbisFile, streamInfo.loopPoint);
                                continue;
                            }
                            else {
                                MEM_ZEROP(channel);
                                channel->soundID = -1;
                                channel->state   = CHANNEL_NONE;
                                break;
                            }
                        }

                        if (SDL_AudioStreamPut(streamInfo.musicStream, streamInfo.buffer, (int)bytes_read) == -1)
                            return;
                    }

                    // Now that we know there are enough samples, read them and mix them
                    int bytes_done = SDL_AudioStreamGet(streamInfo.musicStream, streamInfo.buffer, (int)bytes_wanted);
                    if (bytes_done == -1)
                        return;
                    if (bytes_done == 0)
                        continue;
#endif

                    float buffer[MIX_BUFFER_SAMPLES + 1];
                    memset(buffer, 0, sizeof(buffer));

                    float *soundBuf     = streamInfo.buffer;
                    spdVal              = 0;
                    size_t samples_done = 0;
                    for (int s = 0; s < samples_to_do;) {
                        float mixAmount = speedMixAmounts[spdVal >> 6];
                        spdVal += channel->speed;

                        if (s == samples_to_do - 1) {
                            float bufVal = soundBuf[0] + ((0.0f - soundBuf[0]) * mixAmount);
                            buffer[s++] += bufVal;
                        }
                        else {
                            float bufVal = soundBuf[0] + ((soundBuf[1] - soundBuf[0]) * mixAmount);
                            buffer[s++] += bufVal;
                        }

                        soundBuf += spdVal >> 16;
                        samples_done += 1;
                        spdVal &= 0xFFFF;
                    }

#if RETRO_USING_SDL2
                    ProcessAudioMixing(mix_buffer, buffer, (int)samples_done, channel);
#endif
                    break;
                }
                case CHANNEL_STREAM_LOAD: break;
            }
        }

        for (size_t i = 0; i < sizeof(mix_buffer) / sizeof(*mix_buffer); ++i) {
            // const float max_audioval = 1.0;
            // const float min_audioval  = 0.0;

            const float sample = mix_buffer[i];

            // if (sample > max_audioval)
            //    *output_buffer++ = max_audioval;
            // else if (sample < min_audioval)
            //    *output_buffer++ = min_audioval;
            // else
            *output_buffer++ = sample;
        }

        samples_remaining -= samples_to_do;
    }
}
void ProcessAudioMixing(float *dst, const float *src, int len, ChannelInfo *channel)
{
    if (!channel)
        return;

    float vol = engine.soundFXVolume;
    if (channel->state == CHANNEL_STREAMING)
        vol = engine.streamVolume;

    if (vol <= 0 || channel->volume <= 0)
        return;

    float panL = 1.0f;
    float panR = 1.0f;
    int i      = 0;

    float volume = channel->volume * vol;

    if (channel->pan >= 0) {
        panL = 1.0f - abs(channel->pan);
        panR = 1.0f;
    }
    else {
        panR = 1.0f - abs(channel->pan);
        panL = 1.0f;
    }

    while (len--) {
        float sample = *src++;
        ADJUST_VOLUME(sample, volume);

        if ((i % 2) != 0) {
            sample *= panR;
        }
        else {
            sample *= panL;
        }

        *dst++ += sample;
        i++;
    }
}

void LoadStream(ChannelInfo *channel)
{
    if (channel->state != CHANNEL_STREAM_LOAD)
        return;

    LockAudioDevice();

    if (streamInfo.loaded) {
#if RETRO_USING_SDL2
        if (streamInfo.musicStream)
            SDL_FreeAudioStream(streamInfo.musicStream);
#endif

        ov_clear(&streamInfo.vorbisFile);
    }

    FileInfo info;
    InitFileInfo(&info);

    if (LoadFile(&info, streamInfo.filename, FMODE_RB)) {
        streamInfo.fileSize   = info.fileSize;
        streamInfo.fileBuffer = NULL;
        AllocateStorage(info.fileSize, (void **)&streamInfo.fileBuffer, DATASET_MUS, false);
        ReadBytes(&info, streamInfo.fileBuffer, info.fileSize);
        CloseFile(&info);

        if (streamInfo.fileSize > 0) {
            streamInfo.bufferSize = 0x80000;
            streamInfo.buffer     = NULL;
            AllocateStorage(streamInfo.bufferSize, (void **)&streamInfo.buffer, DATASET_MUS, false);

            ov_callbacks callbacks;

            callbacks.read_func  = readVorbis;
            callbacks.seek_func  = seekVorbis;
            callbacks.tell_func  = tellVorbis;
            callbacks.close_func = closeVorbis;

            streamInfo.bufferPos = 0;
            int error            = ov_open_callbacks(&streamInfo, &streamInfo.vorbisFile, NULL, 0, callbacks);
            if (error != 0) {
            }

            streamInfo.vorbBitstream = -1;
            streamInfo.vorbisFile.vi = ov_info(&streamInfo.vorbisFile, -1);

#if RETRO_USING_SDL2
            streamInfo.musicStream = SDL_NewAudioStream(AUDIO_S16, streamInfo.vorbisFile.vi->channels, (int)streamInfo.vorbisFile.vi->rate,
                                                        audioDeviceFormat.format, audioDeviceFormat.channels, audioDeviceFormat.freq);
            if (!streamInfo.musicStream) {
                printLog(PRINT_NORMAL, "Failed to create stream: %s", SDL_GetError());
            }
#endif

#if RETRO_USING_SDL1
            musInfo.spec.format   = AUDIO_S16;
            musInfo.spec.channels = musInfo.vorbisFile.vi->channels;
            musInfo.spec.freq     = (int)musInfo.vorbisFile.vi->rate;
#endif

            if (streamInfo.startPos) {
                ov_pcm_seek(&streamInfo.vorbisFile, streamInfo.startPos);
            }
            streamInfo.startPos = 0;
            streamInfo.loaded   = true;

            // musicFileInfo = Unknown118(streamFileBuffer, streamFileSize);
            if (/*musicFileInfo*/ true) {
                // if (streamStartPos)
                //    Unknown114(v8, musicStartPos[0]);
                channel->state = CHANNEL_STREAMING;
            }
        }
    }
    UnlockAudioDevice();
}

int PlayStream(const char *filename, unsigned int slot, int startPos, unsigned int loopPoint, bool32 loadASync)
{
    if (!audioEnabled || !engine.streamsEnabled)
        return -1;

    if (slot >= CHANNEL_COUNT) {
    }

    ChannelInfo *channel  = &channels[slot];
    channel->soundID      = 0xFF;
    channel->loop         = loopPoint;
    channel->unknown3     = 0xFF;
    channel->state        = CHANNEL_STREAM_LOAD;
    channel->pan          = 0.0;
    channel->volume       = 1.0;
    channel->sampleLength = sfxList[SFX_COUNT - 1].length;
    channel->samplePtr    = sfxList[SFX_COUNT - 1].buffer;
    channel->bufferPos    = 0;
    channel->speed        = 1 << 0x10;

    sprintf(streamInfo.filename, "Data/Music/%s", filename);
    streamInfo.startPos  = startPos;
    streamInfo.loopPoint = loopPoint;

    if (loadASync) {
        SDL_CreateThread((SDL_ThreadFunction)LoadStream, "LoadStream", (void *)channel);
    }
    else {
        LoadStream(channel);
    }

    return slot;
}

void LoadSfx(char *filename, byte plays, byte scope)
{
    if (!audioEnabled)
        return;

    char fullPath[0x80];
    sprintf(fullPath, "Data/SoundFX/%s", filename);

    uint hash[4];
    GEN_HASH(filename, hash);

    ushort id = -1;
    for (id = 0; id < SFX_COUNT; ++id) {
        if (sfxList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SFX_COUNT)
        return;

    FileInfo info;
    InitFileInfo(&info);
    if (LoadFile(&info, fullPath, FMODE_RB)) {
#if !RETRO_USE_ORIGINAL_CODE
        byte type = fullPath[strlen(fullPath) - 3];
        if (type == 'w' || type == 'W') {
            byte *sfx = new byte[info.fileSize];
            ReadBytes(&info, sfx, info.fileSize);
            CloseFile(&info);

            SDL_RWops *src = SDL_RWFromMem(sfx, info.fileSize);
            if (src == NULL) {
                SDL_RWclose(src);
                delete[] sfx;
                printLog(PRINT_ERROR, "Unable to read sfx: %s", fullPath);
            }
            else {
                SDL_AudioSpec wav_spec;
                uint wav_length;
                byte *wav_buffer;
                SDL_AudioSpec *wav = SDL_LoadWAV_RW(src, 0, &wav_spec, &wav_buffer, &wav_length);

                SDL_RWclose(src);
                delete[] sfx;
                if (wav == NULL) {
                    printLog(PRINT_POPUP, "Unable to open sfx: '%s'\nError: %s\nTrying native method", fullPath, SDL_GetError());

                    // Try Again, this time with RSDK's loader
                    InitFileInfo(&info);
                    if (LoadFile(&info, fullPath, FMODE_RB)) {
                        uint signature = ReadInt32(&info, false);

                        if (signature == 'FFIR') {
                            ReadInt32(&info, false); // chunk size
                            ReadInt32(&info, false); // WAVE
                            ReadInt32(&info, false); // FMT
                            ReadInt32(&info, false); // chunk size
                            ReadInt16(&info);        // audio format
                            ushort channels = ReadInt16(&info);
                            uint freq       = ReadInt32(&info, false);
                            ReadInt32(&info, false); // bytes per sec
                            ReadInt16(&info);        // block align
                            uint format = 0x8000 | (ReadInt16(&info) & 0xFF);
                            Seek_Set(&info, 34);

                            ushort sampleBits = ReadInt16(&info);
                            int loop               = 0;
                            while (true) {
                                signature = ReadInt32(&info, false);
                                if (signature == 'atad')
                                    break;

                                loop += 4;
                                if (loop >= 0x40) {
                                    if (loop != 0x100) {
                                        CloseFile(&info);
                                        printLog(PRINT_ERROR, "Unable to read sfx: %s", fullPath);
                                        return;
                                    }
                                    else {
                                        break;
                                    }
                                }
                            }

                            uint length = ReadInt32(&info, false);
                            uint size   = length;
                            if (sampleBits == 16) {
                                length >>= 1;
                            }
                            AllocateStorage(sizeof(float) * length, (void **)&sfxList[id].buffer, DATASET_SFX, false);
                            sfxList[id].length = length;
                            
                            if (sampleBits == 8) {
                                sbyte *buffer = (sbyte*)sfxList[id].buffer;
                                for (int remaining = length; remaining; --remaining) {
                                    int sample = ReadInt8(&info);
                                    *buffer    = (sample - 128);
                                    buffer++;
                                }
                            }
                            else {
                                short *buffer = (short *)sfxList[id].buffer;
                                for (int remaining = length; remaining; --remaining) {
                                    int sample = ReadInt16(&info);
                                    if (sample > 0x7FFF)
                                        sample = (sample & 0x7FFF) - 0x8000;
                                    *buffer = sample;
                                    buffer++;
                                }
                            }

                            
                            SDL_AudioCVT convert;
                            if (SDL_BuildAudioCVT(&convert, format, channels, freq, audioDeviceFormat.format,
                                                  audioDeviceFormat.channels, audioDeviceFormat.freq)
                                > 0) {
                                convert.buf = (byte *)malloc(size * convert.len_mult);
                                convert.len = size;
                                memcpy(convert.buf, sfxList[id].buffer, size);
                                SDL_ConvertAudio(&convert);

                                LockAudioDevice();
                                GEN_HASH(filename, sfxList[id].hash);
                                sfxList[id].buffer = NULL;
                                AllocateStorage(convert.len_cvt, (void **)&sfxList[id].buffer, DATASET_SFX, false);
                                memcpy(sfxList[id].buffer, convert.buf, convert.len_cvt);
                                sfxList[id].length             = convert.len_cvt / sizeof(float);
                                sfxList[id].scope              = scope;
                                sfxList[id].maxConcurrentPlays = plays;
                                UnlockAudioDevice();
                                free(convert.buf);
                            }
                            else {
                                LockAudioDevice();
                                GEN_HASH(filename, sfxList[id].hash);
                                //AllocateStorage(wav_length, (void **)&sfxList[id].buffer, DATASET_SFX, false);
                                //memcpy(sfxList[id].buffer, wav_buffer, wav_length);
                                //sfxList[id].length             = wav_length / sizeof(float);
                                sfxList[id].scope              = scope;
                                sfxList[id].maxConcurrentPlays = plays;
                                UnlockAudioDevice();
                            }

                        }
                        CloseFile(&info);
                    }
                    else {
                        printLog(PRINT_ERROR, "Unable to open sfx: %s", fullPath);
                    }
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

                        LockAudioDevice();
                        GEN_HASH(filename, sfxList[id].hash);
                        AllocateStorage(convert.len_cvt, (void **)&sfxList[id].buffer, DATASET_SFX, false);
                        memcpy(sfxList[id].buffer, convert.buf, convert.len_cvt);
                        sfxList[id].length             = convert.len_cvt / sizeof(float);
                        sfxList[id].scope              = scope;
                        sfxList[id].maxConcurrentPlays = plays;
                        UnlockAudioDevice();
                        SDL_FreeWAV(wav_buffer);
                        free(convert.buf);
                    }
                    else {
                        LockAudioDevice();
                        GEN_HASH(filename, sfxList[id].hash);
                        AllocateStorage(wav_length, (void **)&sfxList[id].buffer, DATASET_SFX, false);
                        memcpy(sfxList[id].buffer, wav_buffer, wav_length);
                        sfxList[id].length             = wav_length / sizeof(float);
                        sfxList[id].scope              = scope;
                        sfxList[id].maxConcurrentPlays = plays;
                        UnlockAudioDevice();
                    }
                }
            }
        }
        else {
            // wtf lol
            CloseFile(&info);
            printLog(PRINT_ERROR, "Sfx format not supported!");
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
    // if we've hit the max, replace one
    if (count >= sfxList[sfx].maxConcurrentPlays) {
        for (int c = 0; c < CHANNEL_COUNT; ++c) {
            if (channels[c].soundID == sfx) {
                slot = c;
                break;
            }
        }
    }
    else {
        for (int c = 0; c < CHANNEL_COUNT && slot < 0; ++c) {
            if (channels[c].soundID == -1 && channels[c].state != CHANNEL_STREAM_LOAD) {
                slot = c;
            }
        }

        uint len = 0xFFFFFFFF;
        for (int c = 0; c < CHANNEL_COUNT && slot < 0; ++c) {
            if (channels[c].sampleLength < len && channels[c].unknown3 <= unknown) {
                if (channels[c].state != CHANNEL_STREAM_LOAD)
                    slot = c;
                if (channels[c].state != CHANNEL_STREAM_LOAD)
                    len = (uint)channels[c].sampleLength;
            }
        }
    }

    if (slot == -1)
        return -1;

    channels[slot].state        = CHANNEL_SFX;
    channels[slot].bufferPos    = 0;
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

void SetChannelAttributes(byte slot, float volume, float panning, float speed)
{
    if (slot < CHANNEL_COUNT) {
        volume                = fminf(4.0, volume);
        volume                = fmaxf(0.0, volume);
        channels[slot].volume = volume;

        panning            = fminf(1.0, panning);
        panning            = fmaxf(-1.0f, panning);
        channels[slot].pan = panning;

        if (speed > 0.0) {
            channels[slot].speed = (int)(speed * 65536.0f);
        }
        else if (speed == 1.0) {
            channels[slot].speed = 0x10000;
        }
    }
}

uint GetChannelPos(byte slot)
{
    if (slot >= CHANNEL_COUNT)
        return 0;
    if (channels[slot].state == CHANNEL_SFX)
        return channels[slot].bufferPos;
    if (channels[slot].state == CHANNEL_STREAMING) {
        return (uint)ov_pcm_tell(&streamInfo.vorbisFile);
    }
    return 0;
}
