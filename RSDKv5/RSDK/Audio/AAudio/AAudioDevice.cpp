bool32 AudioDevice::Init()
{
    return true;
}

void AudioDevice::Release() {};
void AudioDevice::FrameInit() {};
void AudioDevice::HandleStreamLoad(ChannelInfo *channel, bool32 async) {};