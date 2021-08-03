#include "RetroEngine.hpp"

FileInfo videoFile;

void LoadVideo(const char *filename, double a2, bool32 (*skipCallback)(void))
{
    if (sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK)
        return;

    printLog(PRINT_NORMAL, "Dummy LoadVideo(%s, %f, %p)", filename, a2, skipCallback);
    char buffer[0x80];
    sprintf(buffer, "Data/Videos/%s", filename);

    InitFileInfo(&videoFile);
    if (LoadFile(&videoFile, buffer, FMODE_RB)) {
    
        // TODO: yknow...

        if (true) {
            engine.displayTime = 0.0;
            /*engine.videoUnknown = 0.0;
            if (dword_66B80C == 1)
                engine.videoUnknown = a2;*/
            engine.prevShaderID   = engine.shaderID;
            engine.screenCount    = 0;
            engine.prevEngineMode = sceneInfo.state;
            // if (yuv_mode)
            //    gameSettings.shaderID = (yuv_mode != 2) + SHADER_YUV_422;
            // else
            //    gameSettings.shaderID = SHADER_YUV_420; // YUV-420
            engine.skipCallback = NULL;
            ProcessVideo();
            engine.skipCallback = skipCallback;
            settingsChanged     = false;
            sceneInfo.state     = ENGINESTATE_VIDEOPLAYBACK;

            CloseFile(&videoFile); //temp!!
        }
        else {
            CloseFile(&videoFile);
        }
    }
}

bool32 ProcessVideo() {
    if (engine.skipCallback) {
        if (engine.skipCallback()) {
            engine.shaderID    = engine.prevShaderID;
            sceneInfo.state    = engine.prevEngineMode;
            engine.screenCount = 1;
            return true;
        }
    }
    return false;
}