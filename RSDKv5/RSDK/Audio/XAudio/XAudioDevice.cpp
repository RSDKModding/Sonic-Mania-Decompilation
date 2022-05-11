
uint8 AudioDevice::contextInitialized;

IXAudio2 *AudioDevice::audioContext;
IXAudio2MasteringVoice *AudioDevice::masteringVoice;
IXAudio2SourceVoice *AudioDevice::sourceVoice;
AudioDeviceCallback AudioDevice::voiceCallback;
AudioEngineCallback AudioDevice::engineCallback;

RTL_CRITICAL_SECTION AudioDevice::criticalSection;

bool32 AudioDevice::Init()
{
    if (!contextInitialized) {
        contextInitialized = true;
        InitAudioChannels();

#if RETRO_AUDIODEVICE_XAUDIO
        CoInitializeEx(NULL, 0);
#endif
    }

    audioState = false;
    if (!audioContext) {
        if (InitContext() >= 0) {
            if (audioContext->CreateMasteringVoice(&masteringVoice, 0, 0, 0, NULL, NULL) >= 0) {
                WAVEFORMATEX format;
                format.cbSize          = 0;
                format.wBitsPerSample  = sizeof(SAMPLE_FORMAT) * 8;
                format.nChannels       = 2;
                format.nBlockAlign     = (format.nChannels * format.wBitsPerSample) / 8;
                format.wFormatTag      = WAVE_FORMAT_IEEE_FLOAT;
                format.nSamplesPerSec  = 44100;
                format.nAvgBytesPerSec = 352800;

                if (audioContext->CreateSourceVoice(&sourceVoice, &format, 0, 2.0, &voiceCallback, NULL, NULL) >= 0) {
                    sourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
                    InitMixBuffer();
                    audioState = true;
                }
                else {
                    if (RSDK::videoSettings.windowed) {
                        MessageBox(RenderDevice::windowHandle, L"Unable to create source voice. Sound will not be available during this session.",
                                   L"AudioDevice::Init", MB_OK);
                    }
                }
            }
            else {
                if (RSDK::videoSettings.windowed) {
                    MessageBox(RenderDevice::windowHandle, L"Unable to create mastering voice. Sound will not be available during this session.",
                               L"AudioDevice::Init", MB_OK);
                }

                if (audioContext) {
                    audioContext->Release();
                    audioContext = NULL;
                }
            }
        }
        else {
            if (RSDK::videoSettings.windowed) {
                MessageBox(RenderDevice::windowHandle,
                           L"Error Initializing XAudio2.\n"
                           "You may need to install the Jun 2010 DirectX update to have the XAudio Drivers.\n"
                           "Sound will not be available during this session.",
                           L"AudioDevice::Init", MB_OK);
            }
        }
    }

    return true;
}

void AudioDevice::Release()
{
    if (sourceVoice)
        sourceVoice->DestroyVoice();
    if (masteringVoice)
        masteringVoice->DestroyVoice();
    if (audioContext)
        audioContext->Release();

    CoUninitialize();
    DeleteCriticalSection(&AudioDevice::criticalSection);
}

void AudioDevice::ClearStageSfx()
{
    LockAudioDevice();

    for (int32 c = 0; c < CHANNEL_COUNT; ++c) {
        if (channels[c].state == CHANNEL_SFX || channels[c].state == (CHANNEL_SFX | CHANNEL_PAUSED)) {
            channels[c].soundID = -1;
            channels[c].state   = CHANNEL_NONE;
        }
    }

    // Unload stage SFX
    for (int32 s = 0; s < SFX_COUNT; ++s) {
        if (sfxList[s].scope >= SCOPE_STAGE) {
            MEM_ZERO(sfxList[s]);
            sfxList[s].scope = SCOPE_NONE;
        }
    }

    UnlockAudioDevice();
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
            case CHANNEL_NONE: break;

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
                if (c) {
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
                                channel->state   = CHANNEL_NONE;
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
                }
                else {
                    while (curStreamF < streamEndF && streamF < streamEndF) {
                        SAMPLE_FORMAT sample = (sfxBuffer[1] - *sfxBuffer) * speedMixAmounts[speedPercent >> 6] + *sfxBuffer;

                        speedPercent += channel->speed;
                        sfxBuffer += speedPercent >> 16;
                        channel->bufferPos += speedPercent >> 16;
                        speedPercent &= 0xFFFF;

                        curStreamF[0] = sample * panR;
                        curStreamF[1] = sample * panL;
                        curStreamF += 2;

                        if (channel->bufferPos >= channel->sampleLength) {
                            if (channel->loop == 0xFFFFFFFF) {
                                channel->state   = CHANNEL_NONE;
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
                }

                break;
            }

            case CHANNEL_STREAMING: {
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
                if (c) {
                    while (curStreamF < streamEndF && streamF < streamEndF) {
                        speedPercent += channel->speed;
                        int32 next = speedPercent >> 16;
                        speedPercent &= 0xFFFF;

                        curStreamF[0] += *streamBuffer;
                        curStreamF[1] += streamBuffer[next];
                        curStreamF += 2;

                        streamBuffer += next * 2;
                        channel->bufferPos += next * 2;

                        if (channel->bufferPos >= channel->sampleLength) {
                            channel->bufferPos -= channel->sampleLength;

                            streamBuffer = &channel->samplePtr[channel->bufferPos];

                            UpdateStreamBuffer(channel);
                        }
                    }
                }
                else {
                    while (curStreamF < streamEndF && streamF < streamEndF) {
                        speedPercent += channel->speed;
                        int32 next = speedPercent >> 16;
                        speedPercent &= 0xFFFF;

                        curStreamF[0] = *streamBuffer;
                        curStreamF[1] = streamBuffer[next];
                        curStreamF += 2;

                        streamBuffer += next * 2;
                        channel->bufferPos += next * 2;

                        if (channel->bufferPos >= channel->sampleLength) {
                            if (channel->loop == 0xFFFFFFFF) {
                                channel->state   = CHANNEL_NONE;
                                channel->soundID = -1;
                                break;
                            }
                            else {
                                channel->bufferPos -= channel->sampleLength;

                                streamBuffer = &channel->samplePtr[channel->bufferPos];

                                UpdateStreamBuffer(channel);
                            }
                        }
                    }
                }
                break;
            }

            case CHANNEL_STREAM_LOAD: break;
        }
    }

    UnlockAudioDevice();
}

void AudioDevice::FrameInit()
{
    if (audioState > 2 && --audioState == 2) {
        if (sourceVoice) {
            sourceVoice->DestroyVoice();
            sourceVoice = NULL;
        }

        if (masteringVoice) {
            masteringVoice->DestroyVoice();
            masteringVoice = NULL;
        }

        audioContext->StopEngine();

        Sleep(1000);

        if (audioContext)
            audioContext->Release();
        audioContext = NULL;

        Init();
    }
}

void AudioDevice::HandleStreamLoad(ChannelInfo *channel, bool32 async)
{
    if (async) {
        DWORD threadId;
        HANDLE thread = CreateThread(NULL, 0, LoadStreamASync, channel, 0, &threadId);
        CloseHandle(thread);
    }
    else {
        LoadStream(channel);
    }
}

HRESULT AudioDevice::InitContext()
{
    IXAudio2 *context = nullptr;

    HRESULT res = XAudio2Create(&context, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (res >= 0) {
        res = context->StartEngine();
        if (res < 0)
            context->Release();
        else
            audioContext = context;
    }

    return res;
}

void AudioDevice::InitAudioChannels()
{
    for (int32 i = 0; i < CHANNEL_COUNT; ++i) {
        channels[i].soundID = -1;
        channels[i].state   = CHANNEL_NONE;
    }

    for (int32 i = 0; i < 0x400; i += 2) {
        speedMixAmounts[i]     = (SAMPLE_FORMAT)i * (1.0f / 1024.0f);
        speedMixAmounts[i + 1] = (SAMPLE_FORMAT)(i + 1) * (1.0f / 1024.0f); // 0.00097656;
    }

    GEN_HASH("Stream Channel 0", sfxList[SFX_COUNT - 1].hash);
    sfxList[SFX_COUNT - 1].scope              = SCOPE_GLOBAL;
    sfxList[SFX_COUNT - 1].maxConcurrentPlays = 1;
    sfxList[SFX_COUNT - 1].length             = MIX_BUFFER_SIZE;
    RSDK::AllocateStorage(MIX_BUFFER_SIZE * sizeof(SAMPLE_FORMAT), (void **)&sfxList[SFX_COUNT - 1].buffer, RSDK::DATASET_MUS, false);

    InitializeCriticalSection(&AudioDevice::criticalSection);
    initializedAudioChannels = true;
}

void AudioDevice::InitMixBuffer()
{
    void *mixBuffer = AudioDevice::mixBuffer[AudioDevice::mixBufferID];

    XAUDIO2_BUFFER buffer;
    memset(&buffer, 0, sizeof(buffer));
    buffer.pAudioData = (BYTE *)mixBuffer;
    buffer.pContext   = 0;
    buffer.AudioBytes = MIX_BUFFER_SIZE * sizeof(SAMPLE_FORMAT);

    AudioDevice::mixBufferID = ((AudioDevice::mixBufferID) + 1) % 3;

    AudioDevice::sourceVoice->SubmitSourceBuffer(&buffer, NULL);
    AudioDevice::audioContext->RegisterForCallbacks(&engineCallback);
}

void AudioEngineCallback::OnCriticalError(HRESULT Error) { AudioDevice::audioState = 30; }

void AudioDeviceCallback::OnBufferEnd(void *pBufferContext)
{
    XAUDIO2_VOICE_STATE state;
    AudioDevice::sourceVoice->GetState(&state);

    if (state.BuffersQueued < 2) {
        void *mixBuffer = AudioDevice::mixBuffer[AudioDevice::mixBufferID];

        AudioDevice::ProcessAudioMixing(mixBuffer, MIX_BUFFER_SIZE);

        XAUDIO2_BUFFER buffer;
        memset(&buffer, 0, sizeof(buffer));
        buffer.pAudioData = (BYTE *)mixBuffer;
        buffer.pContext   = 0;
        buffer.AudioBytes = MIX_BUFFER_SIZE * sizeof(SAMPLE_FORMAT);

        AudioDevice::mixBufferID = ((AudioDevice::mixBufferID) + 1) % 3;

        AudioDevice::sourceVoice->SubmitSourceBuffer(&buffer, NULL);
    }
}