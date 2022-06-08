#include "RSDK/Core/RetroEngine.hpp"

#define STB_VORBIS_NO_STDIO
#include "stb_vorbis/stb_vorbis.c"

stb_vorbis *vorbisInfo = NULL;
stb_vorbis_alloc vorbisAlloc;

using namespace RSDK;

SFXInfo RSDK::sfxList[SFX_COUNT];
ChannelInfo RSDK::channels[CHANNEL_COUNT];

char streamFilePath[0x40];
uint8 *streamBuffer    = NULL;
int32 streamBufferSize = 0;
int32 streamStartPos   = 0;
int32 streamLoopPoint  = 0;

float speedMixAmounts[0x400];

#if RETRO_AUDIODEVICE_XAUDIO
#include "XAudio/XAudioDevice.cpp"
#elif RETRO_AUDIODEVICE_NX
#include "NX/NXAudioDevice.cpp"
#elif RETRO_AUDIODEVICE_SDL2
#include "SDL2/SDL2AudioDevice.cpp"
#elif RETRO_AUDIODEVICE_AAUDIO
#include "AAudio/AAudioDevice.cpp"
#endif

uint8 AudioDeviceBase::initializedAudioChannels = false;
uint8 AudioDeviceBase::audioState               = 0;
uint8 AudioDeviceBase::audioFocus               = 0;

int32 AudioDeviceBase::mixBufferID = 0;
float AudioDeviceBase::mixBuffer[3][MIX_BUFFER_SIZE];

void RSDK::UpdateStreamBuffer(ChannelInfo *channel)
{
    int32 bufferRemaining = 0x800;
    float *buffer         = channel->samplePtr;

    for (int32 s = 0; s < 0x800;) {
        int32 samples = stb_vorbis_get_samples_float_interleaved(vorbisInfo, 2, buffer, bufferRemaining) * 2;
        if (!samples) {
            if (channel->loop == 1 && stb_vorbis_seek_frame(vorbisInfo, streamLoopPoint)) {
                // we're looping & the seek was successful, get more samples
            }
            else {
                channel->state   = CHANNEL_IDLE;
                channel->soundID = -1;
                memset(buffer, 0, sizeof(float) * bufferRemaining);

                break;
            }
        }

        s += samples;
        buffer += samples;
        bufferRemaining = 0x800 - s;
    }

    for (int32 i = 0; i < 0x800; i += 4) {
        float *sampleBuffer = &channel->samplePtr[i];

        sampleBuffer[0] = sampleBuffer[0] * 0.5;
        sampleBuffer[1] = sampleBuffer[1] * 0.5;
        sampleBuffer[2] = sampleBuffer[2] * 0.5;
        sampleBuffer[3] = sampleBuffer[3] * 0.5;
    }
}

void RSDK::LoadStream(ChannelInfo *channel)
{
    if (channel->state != CHANNEL_LOADING_STREAM)
        return;

    if (vorbisInfo) {
        vorbis_deinit(vorbisInfo);
        if (!vorbisInfo->alloc.alloc_buffer)
            free(vorbisInfo);
    }

    FileInfo info;
    InitFileInfo(&info);

    if (LoadFile(&info, streamFilePath, FMODE_RB)) {
        streamBufferSize = info.fileSize;
        streamBuffer     = NULL;
        AllocateStorage(info.fileSize, (void **)&streamBuffer, DATASET_MUS, false);
        ReadBytes(&info, streamBuffer, streamBufferSize);
        CloseFile(&info);

        if (streamBufferSize > 0) {
            vorbisAlloc.alloc_buffer_length_in_bytes = 0x80000;
            AllocateStorage(0x80000, (void **)&vorbisAlloc, DATASET_MUS, false);

            vorbisInfo = stb_vorbis_open_memory(streamBuffer, streamBufferSize, NULL, &vorbisAlloc);
            if (vorbisInfo) {
                if (streamStartPos)
                    stb_vorbis_seek(vorbisInfo, streamStartPos);
                UpdateStreamBuffer(channel);

                channel->state = CHANNEL_STREAM;
            }
        }
    }

    if (channel->state == CHANNEL_LOADING_STREAM)
        channel->state = CHANNEL_IDLE;
}

int32 RSDK::PlayStream(const char *filename, uint32 slot, int32 startPos, uint32 loopPoint, bool32 loadASync)
{
    if (!engine.streamsEnabled)
        return -1;

    if (slot >= CHANNEL_COUNT) {
        for (int32 c = 0; c < CHANNEL_COUNT && slot >= CHANNEL_COUNT; ++c) {
            if (channels[c].soundID == -1 && channels[c].state != CHANNEL_LOADING_STREAM) {
                slot = c;
            }
        }

        // as a last resort, run through all channels
        // pick the channel closest to being finished
        if (slot >= CHANNEL_COUNT) {
            uint32 len = 0xFFFFFFFF;
            for (int32 c = 0; c < CHANNEL_COUNT; ++c) {
                if (channels[c].sampleLength < len && channels[c].state != CHANNEL_LOADING_STREAM) {
                    slot = c;
                    len  = (uint32)channels[c].sampleLength;
                }
            }
        }
    }

    if (slot >= CHANNEL_COUNT)
        return -1;

    ChannelInfo *channel = &channels[slot];

    LockAudioDevice();

    channel->soundID      = 0xFF;
    channel->loop         = loopPoint != 0;
    channel->priority     = 0xFF;
    channel->state        = CHANNEL_LOADING_STREAM;
    channel->pan          = 0.0;
    channel->volume       = 1.0;
    channel->sampleLength = sfxList[SFX_COUNT - 1].length;
    channel->samplePtr    = sfxList[SFX_COUNT - 1].buffer;
    channel->bufferPos    = 0;
    channel->speed        = 1 << 0x10;

    sprintf_s(streamFilePath, (int32)sizeof(streamFilePath), "Data/Music/%s", filename);
    streamStartPos  = startPos;
    streamLoopPoint = loopPoint;

    AudioDevice::HandleStreamLoad(channel, loadASync);

    UnlockAudioDevice();

    return slot;
}

#define WAV_SIG_HEADER 0x46464952 // RIFF
#define WAV_SIG_DATA   0x61746164 // data

void RSDK::ReadSfx(char *filename, uint8 id, uint8 plays, uint8 scope, uint32 *size, uint32 *format, uint16 *channels, uint32 *freq)
{
    FileInfo info;
    InitFileInfo(&info);

    if (LoadFile(&info, filename, FMODE_RB)) {
        uint32 signature = ReadInt32(&info, false);

        if (signature == WAV_SIG_HEADER) {
            ReadInt32(&info, false); // chunk size
            ReadInt32(&info, false); // WAVE
            ReadInt32(&info, false); // FMT
            ReadInt32(&info, false); // chunk size
            ReadInt16(&info);        // audio format
            *channels = ReadInt16(&info);
            *freq     = ReadInt32(&info, false);
            ReadInt32(&info, false); // bytes per sec
            ReadInt16(&info);        // block align
            *format = ReadInt16(&info);
            Seek_Set(&info, 34);

            uint16 sampleBits = ReadInt16(&info);
            int32 loop        = 0;
            while (true) {
                signature = ReadInt32(&info, false);
                if (signature == WAV_SIG_DATA)
                    break;

                loop += 4;
                if (loop >= 0x40) {
                    if (loop != 0x100) {
                        CloseFile(&info);
                        PrintLog(PRINT_ERROR, "Unable to read sfx: %s", filename);
                        return;
                    }
                    else {
                        break;
                    }
                }
            }

            uint32 length = ReadInt32(&info, false);
            *size         = length;
            if (sampleBits == 16)
                length >>= 1;

            AllocateStorage(sizeof(float) * length, (void **)&sfxList[id].buffer, DATASET_SFX, false);
            sfxList[id].length = length;

            float *buffer = (float *)sfxList[id].buffer;
            if (sampleBits == 8) {
                for (int32 s = 0; s < length; ++s) {
                    int32 sample = ReadInt8(&info);
                    *buffer++    = (sample - 128) * 0.0078125;
                }
            }
            else {
                for (int32 s = 0; s < length; ++s) {
                    int32 sample = ReadInt16(&info);
                    if (sample > 0x7FFF)
                        sample = (sample & 0x7FFF) - 0x8000;
                    *buffer++ = (sample * 0.000030518) * 0.75;
                }
            }
        }

        CloseFile(&info);
    }
    else {
        PrintLog(PRINT_ERROR, "Unable to open sfx: %s", filename);
    }
}

void RSDK::LoadSfx(char *filename, uint8 plays, uint8 scope)
{
    char fullFilePath[0x80];
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/SoundFX/%s", filename);

    RETRO_HASH_MD5(hash);
    GEN_HASH_MD5(filename, hash);

    uint16 id = -1;
    for (id = 0; id < SFX_COUNT; ++id) {
        if (sfxList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SFX_COUNT)
        return;

    uint8 type = fullFilePath[strlen(fullFilePath) - 3];
    if (type == 'w' || type == 'W') {
        GEN_HASH_MD5(filename, sfxList[id].hash);
        sfxList[id].scope              = scope;
        sfxList[id].maxConcurrentPlays = plays;

        uint16 channels = 0;
        uint32 freq     = 0;
        uint32 format   = 0;
        uint32 size     = 0;
        ReadSfx(fullFilePath, id, plays, scope, &size, &format, &channels, &freq);
    }
    else {
        // what the
        PrintLog(PRINT_ERROR, "Sfx format not supported!");
    }
}
int32 RSDK::PlaySfx(uint16 sfx, uint32 loopPoint, uint32 priority)
{
    if (sfx >= SFX_COUNT || !sfxList[sfx].scope)
        return -1;

    uint8 count = 0;
    for (int32 c = 0; c < CHANNEL_COUNT; ++c) {
        if (channels[c].soundID == sfx)
            ++count;
    }

    int8 slot = -1;
    // if we've hit the max, replace the oldest one
    if (count >= sfxList[sfx].maxConcurrentPlays) {
        int32 highestStackID = 0;
        for (int32 c = 0; c < CHANNEL_COUNT; ++c) {
            int32 stackID = sfxList[sfx].playCount - channels[c].playIndex;
            if (stackID > highestStackID && channels[c].soundID == sfx) {
                slot           = c;
                highestStackID = stackID;
            }
        }
    }

    // if we don't have a slot yet, try to pick any channel that's not currently playing
    for (int32 c = 0; c < CHANNEL_COUNT && slot < 0; ++c) {
        if (channels[c].soundID == -1 && channels[c].state != CHANNEL_LOADING_STREAM) {
            slot = c;
        }
    }

    // as a last resort, run through all channels
    // pick the channel closest to being finished AND with lower priority
    if (slot < 0) {
        uint32 len = 0xFFFFFFFF;
        for (int32 c = 0; c < CHANNEL_COUNT; ++c) {
            if (channels[c].sampleLength < len && priority > channels[c].priority && channels[c].state != CHANNEL_LOADING_STREAM) {
                slot = c;
                len  = (uint32)channels[c].sampleLength;
            }
        }
    }

    if (slot == -1)
        return -1;

    LockAudioDevice();

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
        channels[slot].loop = loopPoint - 1;
    channels[slot].priority  = priority;
    channels[slot].playIndex = sfxList[sfx].playCount++;

    UnlockAudioDevice();

    return slot;
}

void RSDK::SetChannelAttributes(uint8 channel, float volume, float panning, float speed)
{
    if (channel < CHANNEL_COUNT) {
        volume                   = fminf(4.0, volume);
        volume                   = fmaxf(0.0, volume);
        channels[channel].volume = volume;

        panning               = fminf(1.0, panning);
        panning               = fmaxf(-1.0f, panning);
        channels[channel].pan = panning;

        if (speed > 0.0)
            channels[channel].speed = (int32)(speed * 65536.0f);
        else if (speed == 1.0)
            channels[channel].speed = 0x10000;
    }
}

uint32 RSDK::GetChannelPos(uint32 channel)
{
    if (channel >= CHANNEL_COUNT)
        return 0;

    if (channels[channel].state == CHANNEL_SFX)
        return channels[channel].bufferPos;

    if (channels[channel].state == CHANNEL_STREAM) {
        if (!vorbisInfo->current_loc_valid || vorbisInfo->current_loc < 0)
            return 0;

        return vorbisInfo->current_loc;
    }

    return 0;
}

double RSDK::GetVideoStreamPos()
{
    if (channels[0].state == CHANNEL_STREAM && AudioDevice::audioState && AudioDevice::initializedAudioChannels && vorbisInfo->current_loc_valid) {
        return vorbisInfo->current_loc / (float)AUDIO_FREQUENCY;
    }

    return -1.0;
}

void RSDK::ClearStageSfx()
{
    LockAudioDevice();

    for (int32 c = 0; c < CHANNEL_COUNT; ++c) {
        if (channels[c].state == CHANNEL_SFX || channels[c].state == (CHANNEL_SFX | CHANNEL_PAUSED)) {
            channels[c].soundID = -1;
            channels[c].state   = CHANNEL_IDLE;
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

#if RETRO_USE_MOD_LOADER
void RSDK::ClearGlobalSfx()
{
    LockAudioDevice();

    for (int32 c = 0; c < CHANNEL_COUNT; ++c) {
        if (channels[c].state == CHANNEL_SFX || channels[c].state == (CHANNEL_SFX | CHANNEL_PAUSED)) {
            channels[c].soundID = -1;
            channels[c].state   = CHANNEL_IDLE;
        }
    }

    // Unload global SFX
    for (int32 s = 0; s < SFX_COUNT; ++s) {
        if (sfxList[s].scope == SCOPE_GLOBAL) {
            MEM_ZERO(sfxList[s]);
            sfxList[s].scope = SCOPE_NONE;
        }
    }

    UnlockAudioDevice();
}
#endif