
uint8 AudioDevice::contextInitialized;

SDL_AudioDeviceID AudioDevice::device;
SDL_AudioSpec AudioDevice::deviceSpec;

bool32 AudioDevice::Init()
{
    SDL_InitSubSystem(SDL_INIT_AUDIO);
    if (!contextInitialized) {
        contextInitialized = true;
        InitAudioChannels();
    }

    SDL_AudioSpec want;
    want.freq     = AUDIO_FREQUENCY;
    want.format   = AUDIO_F32SYS;
    want.samples  = MIX_BUFFER_SIZE / AUDIO_CHANNELS;
    want.channels = AUDIO_CHANNELS;
    want.callback = AudioCallback;

    audioState = false;
    if ((device = SDL_OpenAudioDevice(nullptr, 0, &want, &deviceSpec, 0)) > 0) {
        SDL_PauseAudioDevice(device, SDL_FALSE);
        audioState = true;
    }
    else {
        PrintLog(PRINT_NORMAL, "ERROR: Unable to open audio device!");
        PrintLog(PRINT_NORMAL, "ERROR: %s", SDL_GetError());
    }

    return true;
}

void AudioDevice::Release()
{
    LockAudioDevice();

    if (vorbisInfo) {
        vorbis_deinit(vorbisInfo);
        if (!vorbisInfo->alloc.alloc_buffer)
            free(vorbisInfo);
    }
    vorbisInfo = NULL;

    UnlockAudioDevice();

    SDL_CloseAudioDevice(AudioDevice::device);
    SDL_Quit();
}

void AudioDevice::ProcessAudioMixing(void *stream, int32 length)
{
    SAMPLE_FORMAT *streamF    = (SAMPLE_FORMAT *)stream;
    SAMPLE_FORMAT *streamEndF = ((SAMPLE_FORMAT *)stream) + length;

    memset(stream, 0, length * sizeof(SAMPLE_FORMAT));

    LockAudioDevice();

    for (int32 c = 0; c < CHANNEL_COUNT; ++c) {
        ChannelInfo *channel = &channels[c];

        switch (channel->state) {
            default:
            case CHANNEL_IDLE: break;

            case CHANNEL_SFX: {
                SAMPLE_FORMAT *sfxBuffer = &channel->samplePtr[channel->bufferPos];

                float volL = channel->volume, volR = channel->volume;
                if (channel->pan < 0.0)
                    volL = (1.0 + channel->pan) * channel->volume;
                else
                    volR = (1.0 - channel->pan) * channel->volume;

                float panL = volL * engine.soundFXVolume;
                float panR = volR * engine.soundFXVolume;

                uint32 speedPercent       = 0;
                SAMPLE_FORMAT *curStreamF = streamF;
                while (curStreamF < streamEndF && streamF < streamEndF) {
                    SAMPLE_FORMAT sample = (sfxBuffer[1] - *sfxBuffer) * speedMixAmounts[speedPercent >> 6] + *sfxBuffer;

                    speedPercent += channel->speed;
                    sfxBuffer += speedPercent >> 16;
                    channel->bufferPos += speedPercent >> 16;
                    speedPercent &= 0xFFFF;

                    curStreamF[0] += sample * panR;
                    curStreamF[1] += sample * panL;
                    curStreamF += 2;

                    if (channel->bufferPos >= channel->sampleLength) {
                        if (channel->loop == 0xFFFFFFFF) {
                            channel->state   = CHANNEL_IDLE;
                            channel->soundID = -1;
                            break;
                        }
                        else {
                            channel->bufferPos -= channel->sampleLength;
                            channel->bufferPos += channel->loop;

                            sfxBuffer = &channel->samplePtr[channel->bufferPos];
                        }
                    }
                }

                break;
            }

            case CHANNEL_STREAM: {
                SAMPLE_FORMAT *streamBuffer = &channel->samplePtr[channel->bufferPos];

                float volL = channel->volume, volR = channel->volume;
                if (channel->pan < 0.0)
                    volL = (1.0 + channel->pan) * channel->volume;
                else
                    volR = (1.0 - channel->pan) * channel->volume;

                float panL = volL * engine.streamVolume;
                float panR = volR * engine.streamVolume;

                uint32 speedPercent       = 0;
                SAMPLE_FORMAT *curStreamF = streamF;
                while (curStreamF < streamEndF && streamF < streamEndF) {
                    speedPercent += channel->speed;
                    int32 next = speedPercent >> 16;
                    speedPercent &= 0xFFFF;

                    curStreamF[0] += panR * *streamBuffer;
                    curStreamF[1] += panL * streamBuffer[next];
                    curStreamF += 2;

                    streamBuffer += next * 2;
                    channel->bufferPos += next * 2;

                    if (channel->bufferPos >= channel->sampleLength) {
                        channel->bufferPos -= channel->sampleLength;

                        streamBuffer = &channel->samplePtr[channel->bufferPos];

                        UpdateStreamBuffer(channel);
                    }
                }
                break;
            }

            case CHANNEL_LOADING_STREAM: break;
        }
    }

    UnlockAudioDevice();
}

void AudioDevice::InitAudioChannels()
{
    for (int32 i = 0; i < CHANNEL_COUNT; ++i) {
        channels[i].soundID = -1;
        channels[i].state   = CHANNEL_IDLE;
    }

    for (int32 i = 0; i < 0x400; i += 2) {
        speedMixAmounts[i]     = (i + 0) * (1.0f / 1024.0f);
        speedMixAmounts[i + 1] = (i + 1) * (1.0f / 1024.0f);
    }

    GEN_HASH_MD5("Stream Channel 0", sfxList[SFX_COUNT - 1].hash);
    sfxList[SFX_COUNT - 1].scope              = SCOPE_GLOBAL;
    sfxList[SFX_COUNT - 1].maxConcurrentPlays = 1;
    sfxList[SFX_COUNT - 1].length             = MIX_BUFFER_SIZE;
    AllocateStorage(MIX_BUFFER_SIZE * sizeof(SAMPLE_FORMAT), (void **)&sfxList[SFX_COUNT - 1].buffer, DATASET_MUS, false);

    initializedAudioChannels = true;
}

void AudioDevice::AudioCallback(void *data, uint8 *stream, int32 len)
{
    (void)data; // Unused

    AudioDevice::ProcessAudioMixing(stream, len / sizeof(SAMPLE_FORMAT));
}
