#include "RSDK/Core/RetroEngine.hpp"

FileInfo videoFile;

void LoadVideo(const char *filename, double a2, bool32 (*skipCallback)(void))
{
    if (sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK)
        return;

    char fullFilePath[0x80];
    sprintf(fullFilePath, "Data/Video/%s", filename);

    InitFileInfo(&videoFile);
    if (LoadFile(&videoFile, fullFilePath, FMODE_RB)) {

        engine.displayTime = 0.0;
        /*engine.videoUnknown = 0.0;
        if (dword_66B80C == 1)
            engine.videoUnknown = a2;*/
        RenderDevice::lastShaderID     = RSDK::videoSettings.shaderID;
        RSDK::videoSettings.screenCount = 0;
        engine.prevEngineMode = sceneInfo.state;
        // if (yuv_mode)
        //    videoSettings.shaderID = (yuv_mode != 2) + SHADER_YUV_422;
        // else
        //    videoSettings.shaderID = SHADER_YUV_420; // YUV-420
        engine.skipCallback = NULL;
        ProcessVideo();
        engine.skipCallback = skipCallback;
        RSDK::changedVideoSettings = false;
        sceneInfo.state     = ENGINESTATE_VIDEOPLAYBACK;
    }
}

int32 ProcessVideo() {
    if (engine.skipCallback) {
        if (engine.skipCallback()) {
            return 1; // video finished
        }
    }

    return 0; // its not even initialised
}
