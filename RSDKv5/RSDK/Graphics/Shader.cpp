#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

#define RSDK_DIRECTX_SDKVER 32

int32 shaderCount = 0;
ShaderEntry shaderList[SHADER_MAX];

bool InitShaders()
{
    // dx9Context = Direct3DCreate9(RSDK_DIRECTX_SDKVER);
    // if (!dx9Context)
    //    return false;

    // Engine_GetDisplays();
    // if (!Engine_InitRenderDevices())
    //    return false;

    if (!ReloadShaders())
        return false;

    engine.windowActive = true;
    engine.dimMax       = 1.0;
    engine.dimPercent   = 1.0;

    return true;
}

bool ReloadShaders()
{
    int32 maxShaders = 1;
    if (engine.shaderSupport) {
        LoadShader("None", false);
        LoadShader("Clean", true);
        LoadShader("CRT-Yeetron", true);
        LoadShader("CRT-Yee64", true);
        LoadShader("YUV-420", true);
        LoadShader("YUV-422", true);
        LoadShader("YUV-444", true);
        LoadShader("RGB-Image", true);
        maxShaders = shaderCount;
    }
    else {
        for (int32 i = 0; i < SHADER_MAX; ++i) shaderList[i].linear = true;

        shaderList[0].linear = !engine.isWindowed ? true : false;
    }

    if (engine.shaderID >= maxShaders)
        engine.shaderID = 0;

    return true;
}

void LoadShader(const char *fileName, bool32 linear)
{
    char buffer[0x100];
    FileInfo info;

    for (int32 i = 0; i < shaderCount; ++i) {
        if (strcmp(shaderList[i].name, fileName) == 0)
            return;
    }

    if (shaderCount == SHADER_MAX)
        return;

    ShaderEntry *shader = &shaderList[shaderCount];
    shader->linear      = linear;
    sprintf(shader->name, "%s", fileName);

    const char *csoPath = "Dummy"; // nothing should ever be in "Data/Shaders/Dummy" so it works out to never load anything
    const char *ext_v   = "bin";
    const char *ext_f   = "bin";

#if RETRO_USING_DIRECTX9
    csoPath = "CSO-DX9"; // windows
    ext_v   = "vso";
    ext_f   = "fso";
#endif

#if RETRO_USING_DIRECTX11
    csoPath = "CSO-DX11"; // xbox one
    ext_v   = "vso";
    ext_f   = "fso";
#endif

    sprintf(buffer, "Data/Shaders/%s/%s.%s", csoPath, fileName, ext_v);
    InitFileInfo(&info);
    if (LoadFile(&info, buffer, FMODE_RB)) {
        byte *fileData = NULL;
        AllocateStorage(info.fileSize, (void **)&fileData, DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        CloseFile(&info);

        // now compile it LOL
#if RETRO_USING_DIRECTX9
        // if (dx9Device->lpVtbl->CreateVertexShader(dx9Device, (int32 *)fileData, &shader->vertexShaderObject) >= 0) {
        //}
        // else {
        //    fileData = NULL;
        //    return;
        //}
#endif
#if RETRO_USING_DIRECTX11

#endif

        fileData = NULL;
    }

    sprintf(buffer, "Data/Shaders/%s/%s.%s", csoPath, fileName, ext_f);
    InitFileInfo(&info);
    if (LoadFile(&info, buffer, FMODE_RB)) {
        byte *fileData = NULL;
        RSDK::AllocateStorage(info.fileSize, (void **)&fileData, RSDK::DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        CloseFile(&info);

        // now compile it LOL
#if RETRO_USING_DIRECTX9
        // if (dx9Device->lpVtbl->CreatePixelShader(dx9Device, (int32 *)fileData, &shader->pixelShaderObject) >= 0) {
        //
        //}
        // else {
        //    fileData = NULL;
        //    return;
        //}
#endif
#if RETRO_USING_DIRECTX11

#endif

        fileData = NULL;
    }

    shaderCount++;
}