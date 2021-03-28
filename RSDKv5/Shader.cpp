#include "RetroEngine.hpp"

int shaderCount = 0;
ShaderEntry shaderList[SHADER_MAX];

bool InitShaders()
{
    //dword_83C71C = (int)Direct3DCreate9(0x20u);
    //if (!dword_83C71C)
    //    return false;

    //Engine_Unknown185();
    //if (!Engine_Unknown186())
    //    return false;
    if (!ReloadShaders())
        return false;
    engine.windowActive = true;
    engine.dimMax       = 1.0;
    engine.dimPercent   = 1.0;

    return true;
}

bool ReloadShaders() {
    int maxShaders = 1;
    if (engine.shaderSupport) {
        LoadShader("None", 0);
        LoadShader("Clean", 1);
        LoadShader("CRT-Yeetron", 1);
        LoadShader("CRT-Yee64", 1);
        LoadShader("YUV-420", 1);
        LoadShader("YUV-422", 1);
        LoadShader("YUV-444", 1);
        LoadShader("RGB-Image", 1);
        maxShaders = shaderCount;
    }
    else {
        for (int i = 0; i < SHADER_MAX; ++i) {
            shaderList[i].scope = 1;
        }

        if (!engine.isFullScreen)
            shaderList[0].scope = 1;
        else
            shaderList[0].scope = 0;
    }

    if (engine.shaderID >= maxShaders)
        engine.shaderID = 0;

    return true;
}

void LoadShader(const char* fileName, byte scope) {
    char buffer[0x100];
    FileInfo info;

    for (int i = 0; i < shaderCount; ++i) {
        if (strcmp(shaderList[i].name, fileName) == 0) {
            return;
        }
    }

    if (shaderCount == SHADER_MAX)
        return;

    shaderList[shaderCount].scope = scope;
    sprintf(shaderList[shaderCount].name, "%s", fileName);

    sprintf(buffer, "Data/Shaders/CSO-DX9/%s.vso", fileName);
    MEM_ZERO(info);
    if (LoadFile(&info, buffer)) {
        byte *fileData = NULL;
        AllocateStorage(info.fileSize, (void **)&fileData, DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        CloseFile(&info);

        // now compile it LOL

        fileData = NULL;
    }

    sprintf(buffer, "Data/Shaders/CSO-DX9/%s.fso", fileName);
    MEM_ZERO(info);
    if (LoadFile(&info, buffer)) {
        byte *fileData = NULL;
        AllocateStorage(info.fileSize, (void **)&fileData, DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        CloseFile(&info);

        //now compile it LOL

        fileData = NULL;
    }

    ++shaderCount;
}