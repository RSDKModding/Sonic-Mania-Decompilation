bool RenderDevice::Init()
{
    if (!SetupRendering() || !AudioDevice::Init())
        return false;

    InitInputDevices();
    return true;
}

bool RenderDevice::SetupRendering()
{

    GetDisplays();

    if (!InitGraphicsAPI() || !InitShaders())
        return false;

    int32 size  = videoSettings.pixWidth >= SCREEN_YSIZE ? videoSettings.pixWidth : SCREEN_YSIZE;
    scanlines = (ScanlineInfo *)malloc(size * sizeof(ScanlineInfo));
    memset(scanlines, 0, size * sizeof(ScanlineInfo));

    videoSettings.windowState = WINDOWSTATE_ACTIVE;
    videoSettings.dimMax      = 1.0;
    videoSettings.dimPercent  = 1.0;

    return true;
}

void RenderDevice::GetDisplays()
{
    displayCount = 0; // get bent
    if (!foundFullScreenDisplay) {
        videoSettings.fsWidth     = 0;
        videoSettings.fsHeight    = 0;
        videoSettings.refreshRate = 60; // 0;
    }
}

bool RenderDevice::InitGraphicsAPI()
{

    lastShaderID = -1;
    InitVertexBuffer();
    engine.inFocus          = 1;
    videoSettings.viewportX = 0;
    videoSettings.viewportY = 0;
    videoSettings.viewportW = 1.0 / viewSize.x;
    videoSettings.viewportH = 1.0 / viewSize.y;

    return true;
}

void RenderDevice::InitVertexBuffer()
{
    RenderVertex vertBuffer[sizeof(rsdkVertexBuffer) / sizeof(RenderVertex)];
    memcpy(vertBuffer, rsdkVertexBuffer, sizeof(rsdkVertexBuffer));
}

void RenderDevice::InitFPSCap() {}
bool RenderDevice::CheckFPSCap() { return false; }
void RenderDevice::UpdateFPSCap() {}

void RenderDevice::CopyFrameBuffer() {}

bool RenderDevice::ProcessEvents() {}

void RenderDevice::FlipScreen()
{
    if (lastShaderID != videoSettings.shaderID) {
        lastShaderID = videoSettings.shaderID;

        SetLinear(shaderList[videoSettings.shaderID].linear);

        glUseProgram(shaderList[videoSettings.shaderID].programID);
    }

    if (windowRefreshDelay > 0) {
        windowRefreshDelay--;
        if (!windowRefreshDelay)
            UpdateGameWindow();
        return;
    }
}

void RenderDevice::Release(bool32 isRefresh)
{

    for (int32 i = 0; i < shaderCount; ++i) {
    }
    shaderCount     = 0;
    userShaderCount = 0;

    glfwDestroyWindow(window);

    if (!isRefresh) {
        if (displayInfo.displays)
            free(displayInfo.displays);
        displayInfo.displays = NULL;

        if (scanlines)
            free(scanlines);
        scanlines = NULL;
    }
}

bool RenderDevice::InitShaders()
{
    videoSettings.shaderSupport = true;
    int32 maxShaders            = 0;

    if (videoSettings.shaderSupport) {
        LoadShader("None", false);
        LoadShader("Clean", true);
        LoadShader("CRT-Yeetron", true);
        LoadShader("CRT-Yee64", true);

#if RETRO_USE_MOD_LOADER
        // a place for mods to load custom shaders
        RunModCallbacks(MODCB_ONSHADERLOAD, NULL);
        userShaderCount = shaderCount;
#endif

        LoadShader("YUV-420", true);
        LoadShader("YUV-422", true);
        LoadShader("YUV-444", true);
        LoadShader("RGB-Image", true);
        maxShaders = shaderCount;
    }
    else {
        for (int32 s = 0; s < SHADER_MAX; ++s) shaderList[s].linear = true;

        shaderList[0].linear = videoSettings.windowed ? false : shaderList[0].linear;
        maxShaders           = 1;
        shaderCount          = 1;
    }

    videoSettings.shaderID = videoSettings.shaderID >= maxShaders ? 0 : videoSettings.shaderID;

    SetLinear(shaderList[videoSettings.shaderID].linear || videoSettings.screenCount > 1);

    return true;
}
void RenderDevice::LoadShader(const char *fileName, bool32 linear)
{
    char fullFilePath[0x100];
    FileInfo info;

    for (int32 i = 0; i < shaderCount; ++i) {
        if (strcmp(shaderList[i].name, fileName) == 0)
            return;
    }

    if (shaderCount == SHADER_MAX)
        return;

    ShaderEntry *shader = &shaderList[shaderCount];
    shader->linear      = linear;
    sprintf_s(shader->name, (int32)sizeof(shader->name), "%s", fileName);

    const char *shaderFolder    = "Dummy"; // nothing should ever be in "Data/Shaders/Dummy" so it works out to never load anything
    const char *vertexShaderExt = "txt";
    const char *pixelShaderExt  = "txt";

    const char *bytecodeFolder    = "CSO-Dummy"; // nothing should ever be in "Data/Shaders/CSO-Dummy" so it works out to never load anything
    const char *vertexBytecodeExt = "bin";
    const char *pixelBytecodeExt  = "bin";

    shaderFolder    = "GL3";
    vertexShaderExt = "vs";
    pixelShaderExt  = "fs";

    GLint success;
    char infoLog[0x1000];
    GLuint vert, frag;
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Shaders/%s/%s.%s", shaderFolder, fileName, vertexShaderExt);
    InitFileInfo(&info);
    if (LoadFile(&info, fullFilePath, FMODE_RB)) {
        uint8 *fileData = NULL;
        AllocateStorage(info.fileSize + 1, (void **)&fileData, DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        fileData[info.fileSize] = 0;
        CloseFile(&info);
    }
    else
        return;

    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Shaders/%s/%s.%s", shaderFolder, fileName, pixelShaderExt);
    InitFileInfo(&info);
    if (LoadFile(&info, fullFilePath, FMODE_RB)) {
        uint8 *fileData = NULL;
        AllocateStorage(info.fileSize + 1, (void **)&fileData, DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        fileData[info.fileSize] = 0;
        CloseFile(&info);
    }
    else
        return;

    shaderCount++;
};

void RenderDevice::RefreshWindow()
{
    videoSettings.windowState = WINDOWSTATE_UNINITIALIZED;

    Release(true);

    if (!InitGraphicsAPI() || !InitShaders())
        return;
}

void RenderDevice::GetWindowSize(int32 *width, int32 *height) {}

void RenderDevice::SetupImageTexture(int32 width, int32 height, uint8 *imagePixels) {}

void RenderDevice::SetupVideoTexture_YUV420(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
}

void RenderDevice::SetupVideoTexture_YUV422(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
}
void RenderDevice::SetupVideoTexture_YUV444(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
}