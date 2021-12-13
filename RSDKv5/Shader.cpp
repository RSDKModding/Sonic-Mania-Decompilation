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
        for (int i = 0; i < SHADER_MAX; ++i) {
            shaderList[i].linear = true;
        }

        if (!engine.isWindowed)
            shaderList[0].linear = true;
        else
            shaderList[0].linear = false;
    }

    if (engine.shaderID >= maxShaders)
        engine.shaderID = 0;

    return true;
}

void LoadShader(const char* fileName, bool32 linear) {
    char buffer[0x100];
    FileInfo info;

    for (int i = 0; i < shaderCount; ++i) {
        if (strcmp(shaderList[i].name, fileName) == 0) {
            return;
        }
    }

    if (shaderCount == SHADER_MAX)
        return;

    ShaderEntry *shader            = &shaderList[shaderCount];
    shader->linear        = linear;
    sprintf(shader->name, "%s", fileName);

    const char *csoPath = "Dummy"; //nothing will ever be in "Data/Shaders/Dummy" so it works out to never load anthing
    const char *ext_v   = "bin";
    const char *ext_f   = "bin";
#if RETRO_USING_DIRECTX9
    csoPath = "CSO-DX9"; //windows
    ext_v   = "vso";
    ext_f   = "fso";
#endif
#if RETRO_USING_DIRECTX11
    csoPath = "CSO-DX11"; //xbox one
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
        //if (dx9Device->lpVtbl->CreateVertexShader(dx9Device, (int *)fileData, &shader->vertexShaderObject) >= 0) {
        //}
        //else {
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
        AllocateStorage(info.fileSize, (void **)&fileData, DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        CloseFile(&info);

        //now compile it LOL
#if RETRO_USING_DIRECTX9
        //if (dx9Device->lpVtbl->CreatePixelShader(dx9Device, (int *)fileData, &shader->pixelShaderObject) >= 0) {
        //
        //}
        //else {
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