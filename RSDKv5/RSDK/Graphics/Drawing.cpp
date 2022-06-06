#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

// all render devices need to access the initial vertex buffer :skull:

// clang-format off
#if RETRO_REV02
const RenderVertex rsdkVertexBuffer[60] = {
    // 1 Screen (0)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 2 Screens - Bordered (Top Screen) (6)
    { { -0.5,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -0.5,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.5,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 2 Screens - Bordered (Bottom Screen) (12)
    { { -0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -0.5, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  0.5, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  0.5, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 2 Screens - Stretched (Top Screen)  (18)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 2 Screens - Stretched (Bottom Screen) (24)
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 4 Screens (Top-Left) (30)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 4 Screens (Top-Right) (36)
    { {  0.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { {  0.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 4 Screens (Bottom-Right) (48)
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  0.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  0.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 4 Screens (Bottom-Left) (42)
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // Image/Video (54)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  1.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  1.0 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  1.0 } }
};
#else
const RenderVertex rsdkVertexBuffer[24] =
{
    // 1 Screen (0)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },

  // 2 Screens - Stretched (Top Screen) (6)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
  
  // 2 Screens - Stretched (Bottom Screen) (12)
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
  
    // Image/Video (18)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  1.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  1.0 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  1.0 } }
};
#endif
// clang-format on

#if RETRO_RENDERDEVICE_DIRECTX9
#include "DX9/DX9RenderDevice.cpp"
#elif RETRO_RENDERDEVICE_DIRECTX11
#include "DX11/DX11RenderDevice.cpp"
#elif RETRO_RENDERDEVICE_NX
#include "NX/NXRenderDevice.cpp"
#elif RETRO_RENDERDEVICE_SDL2
#include "SDL2/SDL2RenderDevice.cpp"
#elif RETRO_RENDERDEVICE_GLFW
#include "GLFW/GLFWRenderDevice.cpp"
#elif RETRO_RENDERDEVICE_EGL
#include "EGL/EGLRenderDevice.cpp"
#endif

RenderDevice::WindowInfo RenderDevice::displayInfo;

DrawList RSDK::drawGroups[DRAWGROUP_COUNT];

uint16 RSDK::blendLookupTable[0x20 * 0x100];
uint16 RSDK::subtractLookupTable[0x20 * 0x100];

GFXSurface RSDK::gfxSurface[SURFACE_COUNT];

float RSDK::dpi         = 1;
int32 RSDK::cameraCount = 0;
ScreenInfo RSDK::screens[SCREEN_COUNT];
CameraInfo RSDK::cameras[CAMERA_COUNT];
ScreenInfo *RSDK::currentScreen = NULL;

int32 RSDK::shaderCount = 0;
ShaderEntry RSDK::shaderList[SHADER_COUNT];

bool32 RSDK::changedVideoSettings = false;
VideoSettings RSDK::videoSettings;
VideoSettings RSDK::videoSettingsBackup;

#if RETRO_USE_MOD_LOADER
int32 RSDK::userShaderCount = 0;
#endif

bool32 RenderDeviceBase::isRunning         = true;
int32 RenderDeviceBase::windowRefreshDelay = 0;

#if RETRO_REV02
uint8 RenderDeviceBase::startVertex_2P[] = { 18, 24 };
uint8 RenderDeviceBase::startVertex_3P[] = { 30, 36, 12 };
#endif

float2 RenderDeviceBase::pixelSize   = { DEFAULT_PIXWIDTH, SCREEN_YSIZE };
float2 RenderDeviceBase::textureSize = { 512.0, 256.0 };
float2 RenderDeviceBase::viewSize    = { 0, 0 };

int32 RenderDeviceBase::displayWidth[16];
int32 RenderDeviceBase::displayHeight[16];
int32 RenderDeviceBase::displayCount = 0;

int32 RenderDeviceBase::lastShaderID = -1;

char RSDK::drawGroupNames[0x10][0x10] = {
    "Draw Group 0", "Draw Group 1", "Draw Group 2",  "Draw Group 3",  "Draw Group 4",  "Draw Group 5",  "Draw Group 6",  "Draw Group 7",
    "Draw Group 8", "Draw Group 9", "Draw Group 10", "Draw Group 11", "Draw Group 12", "Draw Group 13", "Draw Group 14", "Draw Group 15",
};

#include "RSDK/Dev/DevFont.hpp"

// 50% alpha, but way faster
#define setPixelBlend(pixel, frameBufferClr) frameBufferClr = ((pixel >> 1) & 0x7BEF) + ((frameBufferClr >> 1) & 0x7BEF)

// Alpha blending
#define setPixelAlpha(pixel, frameBufferClr, alpha)                                                                                                  \
    int32 R = (fbufferBlend[(frameBufferClr & 0xF800) >> 11] + pixelBlend[(pixel & 0xF800) >> 11]) << 11;                                            \
    int32 G = (fbufferBlend[(frameBufferClr & 0x7E0) >> 6] + pixelBlend[(pixel & 0x7E0) >> 6]) << 6;                                                 \
    int32 B = fbufferBlend[frameBufferClr & 0x1F] + pixelBlend[pixel & 0x1F];                                                                        \
                                                                                                                                                     \
    frameBufferClr = R | G | B;

// Additive Blending
#define setPixelAdditive(pixel, frameBufferClr)                                                                                                      \
    int32 R = minVal((blendTablePtr[(pixel & 0xF800) >> 11] << 11) + (frameBufferClr & 0xF800), 0xF800);                                             \
    int32 G = minVal((blendTablePtr[(pixel & 0x7E0) >> 6] << 6) + (frameBufferClr & 0x7E0), 0x7E0);                                                  \
    int32 B = minVal(blendTablePtr[pixel & 0x1F] + (frameBufferClr & 0x1F), 0x1F);                                                                   \
                                                                                                                                                     \
    frameBufferClr = R | G | B;

// Subtractive Blending
#define setPixelSubtractive(pixel, frameBufferClr)                                                                                                   \
    int32 R = maxVal((frameBufferClr & 0xF800) - (subBlendTable[(pixel & 0xF800) >> 11] << 11), 0);                                                  \
    int32 G = maxVal((frameBufferClr & 0x7E0) - (subBlendTable[(pixel & 0x7E0) >> 6] << 6), 0);                                                      \
    int32 B = maxVal((frameBufferClr & 0x1F) - subBlendTable[pixel & 0x1F], 0);                                                                      \
                                                                                                                                                     \
    frameBufferClr = R | G | B;

// Only draw if framebuffer clr IS maskColor
#define setPixelMasked(pixel, frameBufferClr)                                                                                                        \
    if (frameBufferClr == maskColor)                                                                                                                 \
        frameBufferClr = pixel;

// Only draw if framebuffer clr is NOT maskColor
#define setPixelUnmasked(pixel, frameBufferClr)                                                                                                      \
    if (frameBufferClr != maskColor)                                                                                                                 \
        frameBufferClr = pixel;

void RSDK::RenderDeviceBase::ProcessDimming()
{
    if (videoSettings.dimTimer < videoSettings.dimLimit) {
        if (videoSettings.dimPercent < 1.0) {
            videoSettings.dimPercent += 0.05f;
            if (videoSettings.dimPercent > 1.0)
                videoSettings.dimPercent = 1.0;
        }
    }
    else if (videoSettings.dimPercent > 0.25) {
        videoSettings.dimPercent *= 0.9f;
    }
}

void RSDK::GenerateBlendLookupTable()
{
    for (int32 y = 0; y < 0x100; y++) {
        for (int32 x = 0; x < 0x20; x++) {
            blendLookupTable[x + (y * 0x20)]    = y * x >> 8;
            subtractLookupTable[x + (y * 0x20)] = y * (0x1F - x) >> 8;
        }
    }

#if !RETRO_REV02
    for (int32 i = 0; i < 0x10000; ++i) {
        int32 tintValue    = (((uint32)i & 0x1F) + ((i >> 6) & 0x1F) + (((uint16)i >> 11) & 0x1F)) / 3 + 6;
        tintLookupTable[i] = 0x841 * minVal(0x1F, tintValue);
    }
#endif

    for (int32 c = 0; c < 0x100; ++c) {
        rgb32To16_R[c] = (c & 0xFFF8) << 8;
        rgb32To16_G[c] = (c & 0xFFFC) << 3;
        rgb32To16_B[c] = c >> 3;
    }
}

void RSDK::InitSystemSurfaces()
{
    GEN_HASH_MD5("TileBuffer", gfxSurface[0].hash);
    gfxSurface[0].scope    = SCOPE_GLOBAL;
    gfxSurface[0].width    = TILE_SIZE;
    gfxSurface[0].height   = TILE_COUNT * TILE_SIZE;
    gfxSurface[0].lineSize = 4; // 16px
    gfxSurface[0].pixels   = tilesetPixels;

#if RETRO_REV02
    GEN_HASH_MD5("EngineText", gfxSurface[1].hash);
    gfxSurface[1].scope    = SCOPE_GLOBAL;
    gfxSurface[1].width    = 8;
    gfxSurface[1].height   = 128 * 8;
    gfxSurface[1].lineSize = 3; // 8px
    gfxSurface[1].pixels   = devTextStencil;
#endif
}

void RSDK::UpdateGameWindow() { RenderDevice::RefreshWindow(); }

void RSDK::GetDisplayInfo(int32 *displayID, int32 *width, int32 *height, int32 *refreshRate, char *text)
{
    if (!displayID)
        return;

    int32 prevDisplay = *displayID;
    int32 display     = 0;

    if (*displayID == -2) { // -2 == "get FS size display"
        if (videoSettings.fsWidth && videoSettings.fsHeight) {
            for (display = 0; display < RenderDevice::displayCount; ++display) {
#if RETRO_RENDERDEVICE_DIRECTX11
                int32 refresh = RenderDevice::displayInfo.displays[display].refresh_rate.Numerator
                                / RenderDevice::displayInfo.displays[display].refresh_rate.Denominator;
#else
                int32 refresh = RenderDevice::displayInfo.displays[display].refresh_rate;
#endif

                if (RenderDevice::displayInfo.displays[display].width == videoSettings.fsWidth
                    && RenderDevice::displayInfo.displays[display].height == videoSettings.fsHeight && refresh == videoSettings.refreshRate) {
                    break;
                }
            }

            display++;
        }
    }
    else {
        display = *displayID;
        if (prevDisplay < 0)
            display = RenderDevice::displayCount;
        else if (prevDisplay > RenderDevice::displayCount)
            display = 0;
    }

    *displayID = display;
    if (display) {
        int32 d = display - 1;

#if RETRO_RENDERDEVICE_DIRECTX11
        int32 refresh = RenderDevice::displayInfo.displays[d].refresh_rate.Numerator / RenderDevice::displayInfo.displays[d].refresh_rate.Denominator;
#else
        int32 refresh = RenderDevice::displayInfo.displays[d].refresh_rate;
#endif

        int32 displayWidth   = RenderDevice::displayInfo.displays[d].width;
        int32 displayHeight  = RenderDevice::displayInfo.displays[d].height;
        int32 displayRefresh = refresh;

        if (width)
            *width = displayWidth;

        if (height)
            *height = displayHeight;

        if (refreshRate)
            *refreshRate = displayRefresh;

        if (text)
            sprintf(text, "%ix%i @%iHz", displayWidth, displayHeight, displayRefresh);
    }
    else { // displayID 0 == "default fullscreen size"
        if (width)
            *width = 0;

        if (height)
            *height = 0;

        if (refreshRate)
            *refreshRate = 0;

        if (text)
            sprintf(text, "%s", "DEFAULT");
    }
}

void RSDK::GetWindowSize(int32 *width, int32 *height) { RenderDevice::GetWindowSize(width, height); }
int32 RSDK::GetVideoSetting(int32 id)
{
    switch (id) {
        case VIDEOSETTING_WINDOWED: return videoSettings.windowed;
        case VIDEOSETTING_BORDERED: return videoSettings.bordered;
        case VIDEOSETTING_EXCLUSIVEFS: return videoSettings.exclusiveFS;
        case VIDEOSETTING_VSYNC: return videoSettings.vsync;
        case VIDEOSETTING_TRIPLEBUFFERED: return videoSettings.tripleBuffered;
        case VIDEOSETTING_WINDOW_WIDTH: return videoSettings.windowWidth;
        case VIDEOSETTING_WINDOW_HEIGHT: return videoSettings.windowHeight;
        case VIDEOSETTING_FSWIDTH: return videoSettings.fsWidth;
        case VIDEOSETTING_FSHEIGHT: return videoSettings.fsHeight;
        case VIDEOSETTING_REFRESHRATE: return videoSettings.refreshRate;
        case VIDEOSETTING_SHADERSUPPORT: return videoSettings.shaderSupport;
        case VIDEOSETTING_SHADERID: return videoSettings.shaderID;
        case VIDEOSETTING_SCREENCOUNT: return videoSettings.screenCount;
#if RETRO_REV02
        case VIDEOSETTING_DIMTIMER: return videoSettings.dimTimer;
#endif
        case VIDEOSETTING_STREAMSENABLED: return engine.streamsEnabled;
        case VIDEOSETTING_STREAM_VOL: return (int32)(engine.streamVolume * 1024.0);
        case VIDEOSETTING_SFX_VOL: return (int32)(engine.soundFXVolume * 1024.0);
        case VIDEOSETTING_LANGUAGE:
#if RETRO_REV02
            return SKU::curSKU.language;
#else
            return gameVerInfo.language;
#endif
        case VIDEOSETTING_CHANGED: return changedVideoSettings;

        default: break;
    }

    return 0;
}

void RSDK::SetVideoSetting(int32 id, int32 value)
{
    bool32 boolVal = value;
    switch (id) {
        case VIDEOSETTING_WINDOWED:
            if (videoSettings.windowed != boolVal) {
                videoSettings.windowed = boolVal;
                changedVideoSettings   = true;
            }
            break;

        case VIDEOSETTING_BORDERED:
            if (videoSettings.bordered != boolVal) {
                videoSettings.bordered = boolVal;
                changedVideoSettings   = true;
            }
            break;

        case VIDEOSETTING_EXCLUSIVEFS:
            if (videoSettings.exclusiveFS != boolVal) {
                videoSettings.exclusiveFS = boolVal;
                changedVideoSettings      = true;
            }
            break;

        case VIDEOSETTING_VSYNC:
            if (videoSettings.vsync != boolVal) {
                videoSettings.vsync  = boolVal;
                changedVideoSettings = true;
            }
            break;

        case VIDEOSETTING_TRIPLEBUFFERED:
            if (videoSettings.tripleBuffered != boolVal) {
                videoSettings.tripleBuffered = boolVal;
                changedVideoSettings         = true;
            }
            break;

        case VIDEOSETTING_WINDOW_WIDTH:
            if (videoSettings.windowWidth != value) {
                videoSettings.windowWidth = value;
                changedVideoSettings      = true;
            }
            break;

        case VIDEOSETTING_WINDOW_HEIGHT:
            if (videoSettings.windowHeight != value) {
                videoSettings.windowHeight = value;
                changedVideoSettings       = true;
            }
            break;

        case VIDEOSETTING_FSWIDTH: videoSettings.fsWidth = value; break;
        case VIDEOSETTING_FSHEIGHT: videoSettings.fsHeight = value; break;
        case VIDEOSETTING_REFRESHRATE: videoSettings.refreshRate = value; break;
        case VIDEOSETTING_SHADERSUPPORT: videoSettings.shaderSupport = value; break;
        case VIDEOSETTING_SHADERID:
            if (videoSettings.shaderID != value) {
                videoSettings.shaderID = value;
                changedVideoSettings   = true;
            }
            break;

        case VIDEOSETTING_SCREENCOUNT: videoSettings.screenCount = value; break;
#if RETRO_REV02
        case VIDEOSETTING_DIMTIMER: videoSettings.dimLimit = value; break;
#endif
        case VIDEOSETTING_STREAMSENABLED:
            if (engine.streamsEnabled != boolVal)
                changedVideoSettings = true;

            engine.streamsEnabled = boolVal;
            break;

        case VIDEOSETTING_STREAM_VOL:
            if (engine.streamVolume != (value / 1024.0f)) {
                engine.streamVolume  = (float)value / 1024.0f;
                changedVideoSettings = true;
            }
            break;

        case VIDEOSETTING_SFX_VOL:
            if (engine.soundFXVolume != ((float)value / 1024.0f)) {
                engine.soundFXVolume = (float)value / 1024.0f;
                changedVideoSettings = true;
            }
            break;

        case VIDEOSETTING_LANGUAGE:
#if RETRO_REV02
            SKU::curSKU.language = value;
#else
            gameVerInfo.language = value;
#endif
            break;

        case VIDEOSETTING_STORE: memcpy(&videoSettingsBackup, &videoSettings, sizeof(videoSettings)); break;

        case VIDEOSETTING_RELOAD:
            changedVideoSettings = true;
            memcpy(&videoSettings, &videoSettingsBackup, sizeof(videoSettingsBackup));
            break;

        case VIDEOSETTING_CHANGED: changedVideoSettings = boolVal; break;

        case VIDEOSETTING_WRITE: SaveSettingsINI(value); break;

        default: break;
    }
}

void RSDK::SwapDrawListEntries(uint8 drawGroup, uint16 startSlotID, uint16 endSlotID, int32 count)
{
    if (drawGroup < DRAWGROUP_COUNT) {
        DrawList *list = &drawGroups[drawGroup];
        if (count < 0 || count > list->entityCount)
            count = list->entityCount;

        if (count) {
            int32 slotA = -1;
            int32 slotB = -1;
            if (count > 0) {
                for (int32 i = 0; i < count; ++i) {
                    if (list->entries[i] == startSlotID)
                        slotA = i;
                    if (list->entries[i] == endSlotID)
                        slotB = i;
                }

                if (slotA > -1 && slotB > -1 && slotA < slotB) {
                    int32 temp           = list->entries[slotB];
                    list->entries[slotB] = list->entries[slotA];
                    list->entries[slotA] = temp;
                }
            }
        }
    }
}

void RSDK::FillScreen(uint32 color, int32 alphaR, int32 alphaG, int32 alphaB)
{
    alphaR = clampVal(alphaR, 0x00, 0xFF);
    alphaG = clampVal(alphaG, 0x00, 0xFF);
    alphaB = clampVal(alphaB, 0x00, 0xFF);

    if (alphaR + alphaG + alphaB) {
        validDraw        = true;
        uint16 clrBlendR = blendLookupTable[0x20 * alphaR + rgb32To16_B[(color >> 0x10) & 0xFF]];
        uint16 clrBlendG = blendLookupTable[0x20 * alphaG + rgb32To16_B[(color >> 0x08) & 0xFF]];
        uint16 clrBlendB = blendLookupTable[0x20 * alphaB + rgb32To16_B[(color >> 0x00) & 0xFF]];

        uint16 *fbBlendR = &blendLookupTable[0x20 * (0xFF - alphaR)];
        uint16 *fbBlendG = &blendLookupTable[0x20 * (0xFF - alphaG)];
        uint16 *fbBlendB = &blendLookupTable[0x20 * (0xFF - alphaB)];

        int32 cnt = currentScreen->size.y * currentScreen->pitch;
        for (int32 id = 0; cnt > 0; --cnt, ++id) {
            uint16 px = currentScreen->frameBuffer[id];

            int32 R = fbBlendR[(px & 0xF800) >> 11] + clrBlendR;
            int32 G = fbBlendG[(px & 0x7E0) >> 6] + clrBlendG;
            int32 B = fbBlendB[px & 0x1F] + clrBlendB;

            currentScreen->frameBuffer[id] = (B) | (G << 6) | (R << 11);
        }
    }
}

void RSDK::DrawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, int32 alpha, int32 inkEffect, bool32 screenRelative)
{
    switch (inkEffect) {
        default: break;

        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    int32 drawY1 = y1;
    int32 drawX1 = x1;
    int32 drawY2 = y2;
    int32 drawX2 = x2;

    if (!screenRelative) {
        drawX1 = (x1 >> 16) - currentScreen->position.x;
        drawY1 = (y1 >> 16) - currentScreen->position.y;
        drawX2 = (x2 >> 16) - currentScreen->position.x;
        drawY2 = (y2 >> 16) - currentScreen->position.y;
    }

    int32 flags1 = 0;
    if (drawX1 >= currentScreen->clipBound_X2)
        flags1 = 2;
    else if (drawX1 < currentScreen->clipBound_X1)
        flags1 = 1;

    if (drawY1 >= currentScreen->clipBound_Y2)
        flags1 |= 8;
    else if (drawY1 < currentScreen->clipBound_Y1)
        flags1 |= 4;

    int32 flags2 = 0;
    if (drawX2 >= currentScreen->clipBound_X2)
        flags2 = 2;
    else if (drawX2 < currentScreen->clipBound_X1)
        flags2 = 1;

    if (drawY2 >= currentScreen->clipBound_Y2)
        flags2 |= 8;
    else if (drawY2 < currentScreen->clipBound_Y1)
        flags2 |= 4;

    int32 id = 0;
    while (flags1 || flags2) {
        if (flags1 & flags2)
            return;
        ++id;

        int32 curFlags = flags2;
        if (flags1)
            curFlags = flags1;

        int32 x = 0;
        int32 y = 0;
        if (curFlags & 8) {
            int32 div = (drawY2 - drawY1);
            if (!div)
                div = 1;
            x = drawX1 + ((drawX2 - drawX1) * (((currentScreen->clipBound_Y2 - drawY1) << 8) / div) >> 8);
            y = currentScreen->clipBound_Y2;
        }
        else if (curFlags & 4) {
            int32 div = (drawY2 - drawY1);
            if (!div)
                div = 1;
            x = drawX1 + ((drawX2 - drawX1) * (((currentScreen->clipBound_Y1 - drawY1) << 8) / div) >> 8);
            y = currentScreen->clipBound_Y1;
        }
        else if (curFlags & 2) {
            int32 div = (drawX2 - drawX1);
            if (!div)
                div = 1;
            x = currentScreen->clipBound_X2;
            y = drawY1 + ((drawY2 - drawY1) * (((currentScreen->clipBound_X2 - drawX1) << 8) / div) >> 8);
        }
        else if (curFlags & 1) {
            int32 div = (drawX2 - drawX1);
            if (!div)
                div = 1;
            x = currentScreen->clipBound_X1;
            y = drawY1 + ((drawY2 - drawY1) * (((currentScreen->clipBound_X1 - drawX1) << 8) / div) >> 8);
        }

        if (curFlags == flags1) {
            drawX1 = x;
            drawY1 = y;
            flags1 = 0;
            if (x > currentScreen->clipBound_X2) {
                flags1 = 2;
            }
            else if (x < currentScreen->clipBound_X1) {
                flags1 = 1;
            }

            if (y < currentScreen->clipBound_Y1) {
                flags1 |= 4;
            }
            else if (y > currentScreen->clipBound_Y2) {
                flags1 |= 8;
            }
        }
        else {
            drawX2 = x;
            drawY2 = y;
            flags2 = 0;
            if (x > currentScreen->clipBound_X2) {
                flags2 = 2;
            }
            else if (x < currentScreen->clipBound_X1) {
                flags2 = 1;
            }

            if (y < currentScreen->clipBound_Y1) {
                flags2 |= 4;
            }
            else if (y > currentScreen->clipBound_Y2) {
                flags2 |= 8;
            }
        }
    }

    if (drawX1 > currentScreen->clipBound_X2)
        drawX1 = currentScreen->clipBound_X2;
    else if (drawX1 < currentScreen->clipBound_X1)
        drawX1 = currentScreen->clipBound_X1;

    if (drawY1 > currentScreen->clipBound_Y2)
        drawY1 = currentScreen->clipBound_Y2;
    else if (drawY1 < currentScreen->clipBound_Y1)
        drawY1 = currentScreen->clipBound_Y1;

    if (drawX2 > currentScreen->clipBound_X2)
        drawX2 = currentScreen->clipBound_X2;
    else if (drawX2 < currentScreen->clipBound_X1)
        drawX2 = currentScreen->clipBound_X1;

    if (drawY2 > currentScreen->clipBound_Y2)
        drawY2 = currentScreen->clipBound_Y2;
    else if (drawY2 < currentScreen->clipBound_Y1)
        drawY2 = currentScreen->clipBound_Y1;

    int32 sizeX = abs(drawX2 - drawX1);
    int32 sizeY = abs(drawY2 - drawY1);
    int32 max   = sizeY;
    int32 hSize = sizeX >> 2;
    if (sizeX <= sizeY)
        hSize = -sizeY >> 2;

    if (drawX2 < drawX1) {
        int32 v = drawX1;
        drawX1  = drawX2;
        drawX2  = v;

        v      = drawY1;
        drawY1 = drawY2;
        drawY2 = v;
    }

    uint16 color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];
    uint16 *frameBufferPtr = &currentScreen->frameBuffer[drawX1 + drawY1 * currentScreen->pitch];

    switch (inkEffect) {
        case INK_NONE:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    *frameBufferPtr = color16;

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                while (true) {
                    *frameBufferPtr = color16;
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_BLEND:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    setPixelBlend(color16, *frameBufferPtr);

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                while (true) {
                    setPixelBlend(color16, *frameBufferPtr);
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_ALPHA:
            if (drawY1 > drawY2) {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    setPixelAlpha(color16, *frameBufferPtr, alpha);

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                while (true) {
                    setPixelAlpha(color16, *frameBufferPtr, alpha);
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_ADD:
            if (drawY1 > drawY2) {
                uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    setPixelAdditive(color16, *frameBufferPtr);

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];
                while (true) {
                    setPixelAdditive(color16, *frameBufferPtr);
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_SUB:
            if (drawY1 > drawY2) {
                uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    setPixelSubtractive(color16, *frameBufferPtr);

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
                while (true) {
                    setPixelSubtractive(color16, *frameBufferPtr);
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_TINT:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    *frameBufferPtr = tintLookupTable[*frameBufferPtr];

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                while (true) {
                    *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_MASKED:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    if (*frameBufferPtr == maskColor)
                        *frameBufferPtr = color16;

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                while (true) {
                    if (*frameBufferPtr == maskColor)
                        *frameBufferPtr = color16;
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_UNMASKED:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    if (*frameBufferPtr != maskColor)
                        *frameBufferPtr = color16;

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                while (true) {
                    if (*frameBufferPtr != maskColor)
                        *frameBufferPtr = color16;
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;
    }
}
void RSDK::DrawRectangle(int32 x, int32 y, int32 width, int32 height, uint32 color, int32 alpha, int32 inkEffect, bool32 screenRelative)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    if (!screenRelative) {
        x = (x >> 16) - currentScreen->position.x;
        y = (y >> 16) - currentScreen->position.y;
        width >>= 16;
        height >>= 16;
    }

    if (width + x > currentScreen->clipBound_X2)
        width = currentScreen->clipBound_X2 - x;
    if (x < currentScreen->clipBound_X1) {
        width += x - currentScreen->clipBound_X1;
        x = currentScreen->clipBound_X1;
    }

    if (height + y > currentScreen->clipBound_Y2)
        height = currentScreen->clipBound_Y2 - y;
    if (y < currentScreen->clipBound_Y1) {
        height += y - currentScreen->clipBound_Y1;
        y = currentScreen->clipBound_Y1;
    }

    if (width <= 0 || height <= 0)
        return;

    int32 pitch            = currentScreen->pitch - width;
    validDraw              = true;
    uint16 *frameBufferPtr = &currentScreen->frameBuffer[x + (y * currentScreen->pitch)];
    uint16 color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];

    switch (inkEffect) {
        case INK_NONE: {
            int32 h = height;
            while (h--) {
                int32 w = width;
                while (w--) {
                    *frameBufferPtr = color16;
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_BLEND: {
            int32 h = height;
            while (h--) {
                int32 w = width;
                while (w--) {
                    setPixelBlend(color16, *frameBufferPtr);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_ALPHA: {
            uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
            uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

            int32 h = height;
            while (h--) {
                int32 w = width;
                while (w--) {
                    setPixelAlpha(color16, *frameBufferPtr, alpha);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_ADD: {
            uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];
            int32 h               = height;
            while (h--) {
                int32 w = width;
                while (w--) {
                    setPixelAdditive(color16, *frameBufferPtr);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_SUB: {
            uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
            int32 h               = height;
            while (h--) {
                int32 w = width;
                while (w--) {
                    setPixelSubtractive(color16, *frameBufferPtr);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_TINT: {
            int32 h = height;
            while (h--) {
                int32 w = width;
                while (w--) {
                    *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_MASKED: {
            int32 h = height;
            while (h--) {
                int32 w = width;
                while (w--) {
                    if (*frameBufferPtr == maskColor)
                        *frameBufferPtr = color16;
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_UNMASKED: {
            int32 h = height;
            while (h--) {
                int32 w = width;
                while (w--) {
                    if (*frameBufferPtr != maskColor)
                        *frameBufferPtr = color16;
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }
    }
}
void RSDK::DrawCircle(int32 x, int32 y, int32 radius, uint32 color, int32 alpha, int32 inkEffect, bool32 screenRelative)
{
    if (radius > 0) {
        switch (inkEffect) {
            default: break;
            case INK_ALPHA:
                if (alpha > 0xFF) {
                    inkEffect = INK_NONE;
                }
                else if (alpha <= 0)
                    return;
                break;

            case INK_ADD:
            case INK_SUB:
                if (alpha > 0xFF) {
                    alpha = 0xFF;
                }
                else if (alpha <= 0)
                    return;
                break;

            case INK_TINT:
                if (!tintLookupTable)
                    return;
                break;
        }

        if (!screenRelative) {
            x = (x >> 16) - currentScreen->position.x;
            y = (y >> 16) - currentScreen->position.y;
        }

        int32 yRadiusBottom = y + radius;
        int32 bottom        = currentScreen->clipBound_Y1;
        int32 yRadiusTop    = y - radius;
        int32 top = top = y - radius;
        if (y - radius >= bottom) {
            top = y - radius;
            if (top > currentScreen->clipBound_Y2)
                top = currentScreen->clipBound_Y2;
        }
        else {
            top = currentScreen->clipBound_Y1;
        }

        if (yRadiusBottom >= bottom) {
            bottom = y + radius;
            bottom++;
            if (bottom > currentScreen->clipBound_Y2)
                bottom = currentScreen->clipBound_Y2;
        }

        if (top != bottom) {
            for (int32 i = top; i < bottom; ++i) {
                scanEdgeBuffer[i].start = 0x7FFF;
                scanEdgeBuffer[i].end   = -1;
            }

            int32 r                  = 3 - 2 * radius;
            int32 xRad               = x - radius;
            int32 curY               = y;
            int32 curX               = x;
            int32 startY             = yRadiusTop + 1;
            ScanEdge *scanEdgeTop    = &scanEdgeBuffer[yRadiusTop];
            ScanEdge *scanEdgeBottom = &scanEdgeBuffer[yRadiusBottom];
            ScanEdge *scanEdge       = &scanEdgeBuffer[y];
            int32 dif                = x - y;

            for (int32 i = 0; i <= radius; ++i) {
                int32 scanX = i + curX;
                if (yRadiusBottom >= top && yRadiusBottom <= bottom && scanX > scanEdgeBottom->end)
                    scanEdgeBottom->end = scanX;
                if (startY >= top && startY <= bottom && scanX > scanEdgeTop->end)
                    scanEdgeTop->end = scanX;

                int32 scanY = i + y;
                if (scanY >= top && scanY <= bottom) {
                    ScanEdge *edge = &scanEdgeBuffer[scanY];
                    if (yRadiusBottom + dif > edge->end)
                        edge->end = yRadiusBottom + dif;
                }

                if (curY >= top && curY <= bottom && yRadiusBottom + dif > scanEdge->end)
                    scanEdge->end = yRadiusBottom + dif;
                if (yRadiusBottom >= top && yRadiusBottom <= bottom && x < scanEdgeBottom->start)
                    scanEdgeBottom->start = x;
                if (startY >= top && startY <= bottom && x < scanEdgeTop->start)
                    scanEdgeTop->start = x;

                if (scanY >= top && scanY <= bottom) {
                    ScanEdge *edge = &scanEdgeBuffer[scanY];
                    if (xRad < edge->start)
                        edge->start = xRad;
                }

                if (curY >= top && curY <= bottom && xRad < scanEdge->start)
                    scanEdge->start = xRad;

                if (r >= 0) {
                    --yRadiusBottom;
                    --scanEdgeBottom;
                    ++startY;
                    r += 4 * (i - radius) + 10;
                    ++scanEdgeTop;
                    --radius;
                    ++xRad;
                }
                else {
                    r += 4 * i + 6;
                }
                --curY;
                --scanEdge;
                --x;
            }

            // validDraw              = true;
            uint16 *frameBufferPtr = &currentScreen->frameBuffer[top * currentScreen->pitch];
            uint16 color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];

            switch (inkEffect) {
                default: break;
                case INK_NONE:
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int32 yCnt     = bottom - top;
                        for (int32 y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int32 count = edge->end - edge->start;
                            for (int32 x = 0; x < count; ++x) {
                                frameBufferPtr[edge->start + x] = color16;
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;

                case INK_BLEND:
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int32 yCnt     = bottom - top;
                        for (int32 y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int32 count = edge->end - edge->start;
                            for (int32 x = 0; x < count; ++x) {
                                setPixelBlend(color16, frameBufferPtr[edge->start + x]);
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;

                case INK_ALPHA:
                    if (top <= bottom) {
                        uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                        uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int32 yCnt     = bottom - top;
                        for (int32 y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int32 count = edge->end - edge->start;
                            for (int32 x = 0; x < count; ++x) {
                                setPixelAlpha(color16, frameBufferPtr[edge->start + x], alpha);
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;

                case INK_ADD: {
                    uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int32 yCnt     = bottom - top;
                        for (int32 y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int32 count = edge->end - edge->start;
                            for (int32 x = 0; x < count; ++x) {
                                setPixelAdditive(color16, frameBufferPtr[edge->start + x]);
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;
                }

                case INK_SUB: {
                    uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int32 yCnt     = bottom - top;
                        for (int32 y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int32 count = edge->end - edge->start;
                            for (int32 x = 0; x < count; ++x) {
                                setPixelSubtractive(color16, frameBufferPtr[edge->start + x]);
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;
                }

                case INK_TINT:
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int32 yCnt     = bottom - top;
                        for (int32 y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int32 count = edge->end - edge->start;
                            for (int32 x = 0; x < count; ++x) {
                                frameBufferPtr[edge->start + x] = tintLookupTable[frameBufferPtr[edge->start + x]];
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;

                case INK_MASKED:
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int32 yCnt     = bottom - top;
                        for (int32 y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int32 count = edge->end - edge->start;
                            for (int32 x = 0; x < count; ++x) {
                                if (frameBufferPtr[edge->start + x] == maskColor)
                                    frameBufferPtr[edge->start + x] = color16;
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;

                case INK_UNMASKED:
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int32 yCnt     = bottom - top;
                        for (int32 y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int32 count = edge->end - edge->start;
                            for (int32 x = 0; x < count; ++x) {
                                if (frameBufferPtr[edge->start + x] != maskColor)
                                    frameBufferPtr[edge->start + x] = color16;
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;
            }
        }
    }
}
void RSDK::DrawCircleOutline(int32 x, int32 y, int32 innerRadius, int32 outerRadius, uint32 color, int32 alpha, int32 inkEffect,
                             bool32 screenRelative)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    if (!screenRelative) {
        x = (x >> 16) - currentScreen->position.x;
        y = (y >> 16) - currentScreen->position.y;
    }

    if (outerRadius >= 1 && innerRadius < outerRadius) {
        int32 top    = y - outerRadius;
        int32 left   = x - outerRadius;
        int32 right  = x + outerRadius;
        int32 bottom = y + outerRadius;

        if (left < currentScreen->clipBound_X1)
            left = currentScreen->clipBound_X1;
        if (left > currentScreen->clipBound_X2)
            left = currentScreen->clipBound_X2;

        if (right < currentScreen->clipBound_X1)
            right = currentScreen->clipBound_X1;
        if (right > currentScreen->clipBound_X2)
            right = currentScreen->clipBound_X2;

        if (top < currentScreen->clipBound_Y1)
            top = currentScreen->clipBound_Y1;
        if (top > currentScreen->clipBound_Y1)
            top = currentScreen->clipBound_Y1;

        if (bottom < currentScreen->clipBound_Y2)
            bottom = currentScreen->clipBound_Y2;
        if (bottom > currentScreen->clipBound_Y2)
            bottom = currentScreen->clipBound_Y2;

        if (left != right && top != bottom) {
            int32 ir2              = innerRadius * innerRadius;
            int32 or2              = outerRadius * outerRadius;
            validDraw              = true;
            uint16 *frameBufferPtr = &currentScreen->frameBuffer[left + top * currentScreen->pitch];
            uint16 color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];
            int32 pitch            = (left + currentScreen->pitch - right);

            switch (inkEffect) {
                default: break;
                case INK_NONE:
                    if (top < bottom) {
                        int32 yDif1 = top - y;
                        int32 yDif2 = bottom - top;
                        do {
                            int32 y2 = yDif1 * yDif1;
                            if (left < right) {
                                int32 xDif1 = left - x;
                                int32 xDif2 = right - left;
                                do {
                                    int32 r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2)
                                        *frameBufferPtr = color16;
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;

                case INK_BLEND:
                    if (top < bottom) {
                        int32 yDif1 = top - y;
                        int32 yDif2 = bottom - top;
                        do {
                            int32 y2 = yDif1 * yDif1;
                            if (left < right) {
                                int32 xDif1 = left - x;
                                int32 xDif2 = right - left;
                                do {
                                    int32 r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2)
                                        setPixelBlend(color16, *frameBufferPtr);
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;

                case INK_ALPHA:
                    if (top < bottom) {
                        uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                        uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                        int32 yDif1 = top - y;
                        int32 yDif2 = bottom - top;
                        do {
                            int32 y2 = yDif1 * yDif1;
                            if (left < right) {
                                int32 xDif1 = left - x;
                                int32 xDif2 = right - left;
                                do {
                                    int32 r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2) {
                                        setPixelAlpha(color16, *frameBufferPtr, alpha);
                                    }
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;

                case INK_ADD: {
                    uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    if (top < bottom) {
                        int32 yDif1 = top - y;
                        int32 yDif2 = bottom - top;
                        do {
                            int32 y2 = yDif1 * yDif1;
                            if (left < right) {
                                int32 xDif1 = left - x;
                                int32 xDif2 = right - left;
                                do {
                                    int32 r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2) {
                                        setPixelAdditive(color16, *frameBufferPtr);
                                    }
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
                }

                case INK_SUB: {
                    uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    if (top < bottom) {
                        int32 yDif1 = top - y;
                        int32 yDif2 = bottom - top;
                        do {
                            int32 y2 = yDif1 * yDif1;
                            if (left < right) {
                                int32 xDif1 = left - x;
                                int32 xDif2 = right - left;
                                do {
                                    int32 r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2) {
                                        setPixelSubtractive(color16, *frameBufferPtr);
                                    }
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
                }

                case INK_TINT:
                    if (top < bottom) {
                        int32 yDif1 = top - y;
                        int32 yDif2 = bottom - top;
                        do {
                            int32 y2 = yDif1 * yDif1;
                            if (left < right) {
                                int32 xDif1 = left - x;
                                int32 xDif2 = right - left;
                                do {
                                    int32 r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2)
                                        *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;

                case INK_MASKED:
                    if (top < bottom) {
                        int32 yDif1 = top - y;
                        int32 yDif2 = bottom - top;
                        do {
                            int32 y2 = yDif1 * yDif1;
                            if (left < right) {
                                int32 xDif1 = left - x;
                                int32 xDif2 = right - left;
                                do {
                                    int32 r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2 && *frameBufferPtr == maskColor)
                                        *frameBufferPtr = color16;
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;

                case INK_UNMASKED:
                    if (top < bottom) {
                        int32 yDif1 = top - y;
                        int32 yDif2 = bottom - top;
                        do {
                            int32 y2 = yDif1 * yDif1;
                            if (left < right) {
                                int32 xDif1 = left - x;
                                int32 xDif2 = right - left;
                                do {
                                    int32 r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2 && *frameBufferPtr != maskColor)
                                        *frameBufferPtr = color16;
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
            }
        }
    }
}

void RSDK::DrawFace(Vector2 *vertices, int32 vertCount, int32 r, int32 g, int32 b, int32 alpha, int32 inkEffect)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF)
                inkEffect = INK_NONE;
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF)
                alpha = 0xFF;
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    int32 top    = 0x7FFFFFFF;
    int32 bottom = -0x10000;
    for (int32 v = 0; v < vertCount; ++v) {
        if (vertices[v].y < top)
            top = vertices[v].y;
        if (vertices[v].y > bottom)
            bottom = vertices[v].y;
    }

    int32 topScreen    = top >> 16;
    int32 bottomScreen = bottom >> 16;

    if (topScreen < currentScreen->clipBound_Y1)
        topScreen = currentScreen->clipBound_Y1;
    if (topScreen > currentScreen->clipBound_Y2)
        topScreen = currentScreen->clipBound_Y2;

    if (bottomScreen < currentScreen->clipBound_Y1)
        bottomScreen = currentScreen->clipBound_Y1;
    if (bottomScreen > currentScreen->clipBound_Y2)
        bottomScreen = currentScreen->clipBound_Y2;

    if (topScreen != bottomScreen) {
        ScanEdge *edge = &scanEdgeBuffer[topScreen];
        for (int32 s = topScreen; s <= bottomScreen; ++s) {
            edge->start = 0x7FFF;
            edge->end   = -1;
            ++edge;
        }

        for (int32 v = 0; v < vertCount - 1; ++v) {
            ProcessScanEdge(vertices[v + 0].x, vertices[v + 0].y, vertices[v + 1].x, vertices[v + 1].y);
        }
        ProcessScanEdge(vertices[0].x, vertices[0].y, vertices[vertCount - 1].x, vertices[vertCount - 1].y);

        uint16 *frameBufferPtr = &currentScreen->frameBuffer[topScreen * currentScreen->pitch];
        uint16 color16         = rgb32To16_B[b] | rgb32To16_G[g] | rgb32To16_R[r];

        edge = &scanEdgeBuffer[topScreen];
        switch (inkEffect) {
            default: break;

            case INK_NONE:
                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int32 count = edge->end - edge->start;
                    for (int32 x = 0; x < count; ++x) {
                        frameBufferPtr[edge->start + x] = color16;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_BLEND:
                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int32 count = edge->end - edge->start;
                    for (int32 x = 0; x < count; ++x) {
                        setPixelBlend(color16, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_ALPHA: {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int32 count = edge->end - edge->start;
                    for (int32 x = 0; x < count; ++x) {
                        setPixelAlpha(color16, frameBufferPtr[edge->start + x], alpha);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_ADD: {
                uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];

                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int32 count = edge->end - edge->start;
                    for (int32 x = 0; x < count; ++x) {
                        setPixelAdditive(color16, frameBufferPtr[edge->start + x]);
                    }

                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_SUB: {
                uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int32 count = edge->end - edge->start;
                    for (int32 x = 0; x < count; ++x) {
                        setPixelSubtractive(color16, frameBufferPtr[edge->start + x]);
                    }

                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_TINT:
                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int32 count = edge->end - edge->start;
                    for (int32 x = 0; x < count; ++x) {
                        frameBufferPtr[edge->start + x] = tintLookupTable[frameBufferPtr[edge->start + x]];
                    }

                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_MASKED:
                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int32 count = edge->end - edge->start;
                    for (int32 x = 0; x < count; ++x) {
                        if (frameBufferPtr[edge->start + x] == maskColor)
                            frameBufferPtr[edge->start + x] = color16;
                    }

                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_UNMASKED:
                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int32 count = edge->end - edge->start;
                    for (int32 x = 0; x < count; ++x) {
                        if (frameBufferPtr[edge->start + x] != maskColor)
                            frameBufferPtr[edge->start + x] = color16;
                    }

                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
        }
    }
}
void RSDK::DrawBlendedFace(Vector2 *vertices, uint32 *colors, int32 vertCount, int32 alpha, int32 inkEffect)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF)
                inkEffect = INK_NONE;
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF)
                alpha = 0xFF;
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    int32 top    = 0x7FFFFFFF;
    int32 bottom = -0x10000;
    for (int32 v = 0; v < vertCount; ++v) {
        if (vertices[v].y < top)
            top = vertices[v].y;
        if (vertices[v].y > bottom)
            bottom = vertices[v].y;
    }

    int32 topScreen    = top >> 16;
    int32 bottomScreen = bottom >> 16;

    if (topScreen < currentScreen->clipBound_Y1)
        topScreen = currentScreen->clipBound_Y1;
    if (topScreen > currentScreen->clipBound_Y2)
        topScreen = currentScreen->clipBound_Y2;

    if (bottomScreen < currentScreen->clipBound_Y1)
        bottomScreen = currentScreen->clipBound_Y1;
    if (bottomScreen > currentScreen->clipBound_Y2)
        bottomScreen = currentScreen->clipBound_Y2;

    if (topScreen != bottomScreen) {
        ScanEdge *edge = &scanEdgeBuffer[topScreen];
        for (int32 s = topScreen; s <= bottomScreen; ++s) {
            edge->start = 0x7FFF;
            edge->end   = -1;
            ++edge;
        }

        for (int32 v = 0; v < vertCount - 1; ++v) {
            ProcessScanEdgeClr(colors[v + 0], colors[v + 1], vertices[v + 0].x, vertices[v + 0].y, vertices[v + 1].x, vertices[v + 1].y);
        }
        ProcessScanEdgeClr(colors[vertCount - 1], colors[0], vertices[vertCount - 1].x, vertices[vertCount - 1].y, vertices[0].x, vertices[0].y);

        uint16 *frameBufferPtr = &currentScreen->frameBuffer[topScreen * currentScreen->pitch];

        edge = &scanEdgeBuffer[topScreen];
        switch (inkEffect) {
            default: break;
            case INK_NONE:
                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    int32 count  = edge->end - edge->start;
                    int32 deltaR = 0;
                    int32 deltaG = 0;
                    int32 deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int32 startR = edge->startR;
                    int32 startG = edge->startG;
                    int32 startB = edge->startB;

                    if (edge->start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }
                    else if (edge->start < currentScreen->clipBound_X1) {
                        int32 dif = (currentScreen->clipBound_X1 - edge->start);
                        startR += deltaR * dif;
                        startG += deltaG * dif;
                        startB += deltaB * dif;
                        count -= dif;
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                        count     = currentScreen->clipBound_X1 - edge->start;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int32 x = 0; x < count; ++x) {
                        uint16 color16                  = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        frameBufferPtr[edge->start + x] = color16;
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_BLEND:
                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    int32 start  = edge->start;
                    int32 count  = edge->end - edge->start;
                    int32 deltaR = 0;
                    int32 deltaG = 0;
                    int32 deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int32 startR = edge->startR;
                    int32 startG = edge->startG;
                    int32 startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int32 x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        uint16 color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelBlend(color, frameBufferPtr[edge->start + x]);
                    }

                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_ALPHA: {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    int32 start  = edge->start;
                    int32 count  = edge->end - edge->start;
                    int32 deltaR = 0;
                    int32 deltaG = 0;
                    int32 deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int32 startR = edge->startR;
                    int32 startG = edge->startG;
                    int32 startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int32 x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        uint16 color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelAlpha(color, frameBufferPtr[edge->start + x], alpha);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_ADD: {
                uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];

                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    int32 start  = edge->start;
                    int32 count  = edge->end - edge->start;
                    int32 deltaR = 0;
                    int32 deltaG = 0;
                    int32 deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int32 startR = edge->startR;
                    int32 startG = edge->startG;
                    int32 startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int32 x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        uint16 color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelAdditive(color, frameBufferPtr[edge->start + x]);
                    }

                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_SUB: {
                uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];

                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    int32 start  = edge->start;
                    int32 count  = edge->end - edge->start;
                    int32 deltaR = 0;
                    int32 deltaG = 0;
                    int32 deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int32 startR = edge->startR;
                    int32 startG = edge->startG;
                    int32 startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int32 x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        uint16 color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelSubtractive(color, frameBufferPtr[edge->start + x]);
                    }

                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_TINT:
                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    int32 start  = edge->start;
                    int32 count  = edge->end - edge->start;
                    int32 deltaR = 0;
                    int32 deltaG = 0;
                    int32 deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int32 startR = edge->startR;
                    int32 startG = edge->startG;
                    int32 startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int32 x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        frameBufferPtr[edge->start + x] = tintLookupTable[frameBufferPtr[edge->start + x]];
                    }

                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_MASKED:
                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    int32 start  = edge->start;
                    int32 count  = edge->end - edge->start;
                    int32 deltaR = 0;
                    int32 deltaG = 0;
                    int32 deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int32 startR = edge->startR;
                    int32 startG = edge->startG;
                    int32 startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int32 x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        uint16 color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        if (frameBufferPtr[edge->start + x] == maskColor)
                            frameBufferPtr[edge->start + x] = color;
                    }

                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_UNMASKED:
                for (int32 s = topScreen; s <= bottomScreen; ++s) {
                    int32 start  = edge->start;
                    int32 count  = edge->end - edge->start;
                    int32 deltaR = 0;
                    int32 deltaG = 0;
                    int32 deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int32 startR = edge->startR;
                    int32 startG = edge->startG;
                    int32 startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int32 x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        uint16 color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        if (frameBufferPtr[edge->start + x] != maskColor)
                            frameBufferPtr[edge->start + x] = color;
                    }

                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
        }
    }
}

void RSDK::DrawSprite(Animator *animator, Vector2 *position, bool32 screenRelative)
{
    if (animator && animator->frames) {
        SpriteFrame *frame = &animator->frames[animator->frameID];
        Vector2 pos;
        if (!position)
            pos = sceneInfo.entity->position;
        else
            pos = *position;

        pos.x >>= 0x10;
        pos.y >>= 0x10;
        if (!screenRelative) {
            pos.x -= currentScreen->position.x;
            pos.y -= currentScreen->position.y;
        }

        int32 rotation = sceneInfo.entity->rotation;
        int32 drawFX   = sceneInfo.entity->drawFX;
        if (sceneInfo.entity->drawFX & FX_ROTATE) {
            switch (animator->rotationStyle) {
                case ROTSTYLE_NONE:
                    rotation = 0;
                    if ((sceneInfo.entity->drawFX & FX_ROTATE) != FX_NONE)
                        drawFX ^= FX_ROTATE;
                    break;

                case ROTSTYLE_FULL:
                    rotation = sceneInfo.entity->rotation & 0x1FF;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case ROTFLAG_45DEG: // 0x00, 0x40, 0x80, 0xC0, 0x100, 0x140, 0x180, 0x1C0
                    rotation = (sceneInfo.entity->rotation + 0x20) & 0x1C0;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case ROTSTYLE_90DEG: // 0x00, 0x80, 0x100, 0x180
                    rotation = (sceneInfo.entity->rotation + 0x40) & 0x180;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case ROTSTYLE_180DEG: // 0x00, 0x100
                    rotation = (sceneInfo.entity->rotation + 0x80) & 0x100;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case ROTSTYLE_STATICFRAMES:
                    if (sceneInfo.entity->rotation >= 0x100)
                        rotation = 0x08 - ((0x214 - sceneInfo.entity->rotation) >> 6);
                    else
                        rotation = (sceneInfo.entity->rotation + 20) >> 6;

                    switch (rotation) {
                        case 0: // 0 deg
                        case 8: // 360 deg
                            rotation = 0x00;
                            if ((sceneInfo.entity->drawFX & FX_SCALE) != FX_NONE)
                                drawFX ^= FX_ROTATE;
                            break;

                        case 1: // 45 deg
                            rotation = 0x80;
                            frame += animator->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x00;
                            break;

                        case 2: // 90 deg
                            rotation = 0x80;
                            break;

                        case 3: // 135 deg
                            rotation = 0x100;
                            frame += animator->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x80;
                            break;

                        case 4: // 180 deg
                            rotation = 0x100;
                            break;

                        case 5: // 225 deg
                            rotation = 0x180;
                            frame += animator->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x100;
                            break;

                        case 6: // 270 deg
                            rotation = 0x180;
                            break;

                        case 7: // 315 deg
                            rotation = 0x180;
                            frame += animator->frameCount;
                            if (!sceneInfo.entity->direction)
                                rotation = 0;
                            break;

                        default: break;
                    }
                    break;

                default: break;
            }
        }

        switch (drawFX) {
            case FX_NONE:
                DrawSpriteFlipped(pos.x + frame->pivotX, pos.y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                  sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_FLIP:
                switch (sceneInfo.entity->direction) {
                    case FLIP_NONE:
                        DrawSpriteFlipped(pos.x + frame->pivotX, pos.y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                          FLIP_NONE, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;

                    case FLIP_X:
                        DrawSpriteFlipped(pos.x - frame->width - frame->pivotX, pos.y + frame->pivotY, frame->width, frame->height, frame->sprX,
                                          frame->sprY, FLIP_X, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;

                    case FLIP_Y:
                        DrawSpriteFlipped(pos.x + frame->pivotX, pos.y - frame->height - frame->pivotY, frame->width, frame->height, frame->sprX,
                                          frame->sprY, FLIP_Y, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;

                    case FLIP_XY:
                        DrawSpriteFlipped(pos.x - frame->width - frame->pivotX, pos.y - frame->height - frame->pivotY, frame->width, frame->height,
                                          frame->sprX, frame->sprY, FLIP_XY, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;

                    default: break;
                }
                break;

            case FX_ROTATE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, 0x200, 0x200,
                                   FLIP_NONE, rotation, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_ROTATE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, 0x200, 0x200,
                                   sceneInfo.entity->direction & FLIP_X, rotation, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                                   frame->sheetID);
                break;

            case FX_SCALE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE, 0, sceneInfo.entity->inkEffect,
                                   sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_SCALE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, sceneInfo.entity->direction & FLIP_X, 0,
                                   sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_SCALE | FX_ROTATE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE, rotation, sceneInfo.entity->inkEffect,
                                   sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_SCALE | FX_ROTATE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, sceneInfo.entity->direction & FLIP_X, rotation,
                                   sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            default: break;
        }
    }
}
void RSDK::DrawSpriteFlipped(int32 x, int32 y, int32 width, int32 height, int32 sprX, int32 sprY, int32 direction, int32 inkEffect, int32 alpha,
                             int32 sheetID)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF)
                inkEffect = INK_NONE;
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF)
                alpha = 0xFF;
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }
    int32 widthFlip  = width;
    int32 heightFlip = height;

    if (width + x > currentScreen->clipBound_X2)
        width = currentScreen->clipBound_X2 - x;

    if (x < currentScreen->clipBound_X1) {
        int32 val = x - currentScreen->clipBound_X1;
        sprX -= val;
        width += val;
        widthFlip += 2 * val;
        x = currentScreen->clipBound_X1;
    }

    if (height + y > currentScreen->clipBound_Y2)
        height = currentScreen->clipBound_Y2 - y;

    if (y < currentScreen->clipBound_Y1) {
        int32 val = y - currentScreen->clipBound_Y1;
        sprY -= val;
        height += val;
        heightFlip += 2 * val;
        y = currentScreen->clipBound_Y1;
    }

    if (width <= 0 || height <= 0)
        return;

    GFXSurface *surface    = &gfxSurface[sheetID];
    validDraw              = true;
    int32 pitch            = currentScreen->pitch - width;
    int32 gfxPitch         = 0;
    uint8 *lineBuffer      = NULL;
    uint8 *gfxData         = NULL;
    uint16 *frameBufferPtr = NULL;

    switch (direction) {
        default: break;

        case FLIP_NONE:
            gfxPitch       = surface->width - width;
            lineBuffer     = &gfxLineBuffer[y];
            gfxData        = &surface->pixels[sprX + surface->width * sprY];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_BLEND:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                setPixelBlend(activePalette[*gfxData], *frameBufferPtr);
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_ALPHA: {
                    uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                    uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelAlpha(color, *frameBufferPtr, alpha);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_ADD: {
                    uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelAdditive(color, *frameBufferPtr);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_SUB: {
                    uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelSubtractive(color, *frameBufferPtr);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_TINT:
                    while (height--) {
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_MASKED:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr == maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_UNMASKED:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr != maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
            }
            break;

        case FLIP_X:
            gfxPitch       = width + surface->width;
            lineBuffer     = &gfxLineBuffer[y];
            gfxData        = &surface->pixels[widthFlip - 1 + sprX + surface->width * sprY];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_BLEND:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                setPixelBlend(activePalette[*gfxData], *frameBufferPtr);
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_ALPHA: {
                    uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                    uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelAlpha(color, *frameBufferPtr, alpha);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_ADD: {
                    uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelAdditive(color, *frameBufferPtr);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_SUB: {
                    uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelSubtractive(color, *frameBufferPtr);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_TINT:
                    while (height--) {
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_MASKED:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr == maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_UNMASKED:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr != maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
            }
            break;

        case FLIP_Y:
            gfxPitch       = width + surface->width;
            lineBuffer     = &gfxLineBuffer[y];
            gfxData        = &surface->pixels[sprX + surface->width * (sprY + heightFlip - 1)];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_BLEND:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                setPixelBlend(activePalette[*gfxData], *frameBufferPtr);
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_ALPHA: {
                    uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                    uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelAlpha(color, *frameBufferPtr, alpha);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_ADD: {
                    uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelAdditive(color, *frameBufferPtr);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_SUB: {
                    uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelSubtractive(color, *frameBufferPtr);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_TINT:
                    while (height--) {
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_MASKED:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr == maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_UNMASKED:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr != maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
            }
            break;

        case FLIP_XY:
            gfxPitch       = surface->width - width;
            lineBuffer     = &gfxLineBuffer[y];
            gfxData        = &surface->pixels[widthFlip - 1 + sprX + surface->width * (sprY + heightFlip - 1)];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_BLEND:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                setPixelBlend(activePalette[*gfxData], *frameBufferPtr);
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_ALPHA: {
                    uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                    uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelAlpha(color, *frameBufferPtr, alpha);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_ADD: {
                    uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelAdditive(color, *frameBufferPtr);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_SUB: {
                    uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                uint16 color = activePalette[*gfxData];
                                setPixelSubtractive(color, *frameBufferPtr);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_TINT:
                    while (height--) {
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_MASKED:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr == maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_UNMASKED:
                    while (height--) {
                        uint16 *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int32 w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr != maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
            }
            break;
    }
}
void RSDK::DrawSpriteRotozoom(int32 x, int32 y, int32 pivotX, int32 pivotY, int32 width, int32 height, int32 sprX, int32 sprY, int32 scaleX,
                              int32 scaleY, int32 direction, int16 rotation, int32 inkEffect, int32 alpha, int32 sheetID)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    int32 angle = 0x200 - (rotation & 0x1FF);
    if (!(rotation & 0x1FF))
        angle = rotation & 0x1FF;

    int32 sine        = sin512LookupTable[angle];
    int32 cosine      = cos512LookupTable[angle];
    int32 fullScaleXS = scaleX * sine >> 9;
    int32 fullScaleXC = scaleX * cosine >> 9;
    int32 fullScaleYS = scaleY * sine >> 9;
    int32 fullScaleYC = scaleY * cosine >> 9;

    int32 posX[4];
    int32 posY[4];
    int32 sprXPos = (sprX - pivotX) << 16;
    int32 sprYPos = (sprY - pivotY) << 16;

    int32 xMax     = 0;
    int32 scaledX1 = 0;
    int32 scaledX2 = 0;
    int32 scaledY1 = 0;
    int32 scaledY2 = 0;
    switch (direction) {
        default:
        case FLIP_NONE: {
            scaledX1 = fullScaleXS * (pivotX - 2);
            scaledX2 = fullScaleXC * (pivotX - 2);
            scaledY1 = fullScaleYS * (pivotY - 2);
            scaledY2 = fullScaleYC * (pivotY - 2);
            xMax     = pivotX + 2 + width;
            posX[0]  = x + ((scaledX2 + scaledY1) >> 9);
            posY[0]  = y + ((fullScaleYC * (pivotY - 2) - scaledX1) >> 9);
            break;
        }

        case FLIP_X: {
            scaledX1 = fullScaleXS * (2 - pivotX);
            scaledX2 = fullScaleXC * (2 - pivotX);
            scaledY1 = fullScaleYS * (pivotY - 2);
            scaledY2 = fullScaleYC * (pivotY - 2);
            xMax     = -2 - pivotX - width;
            posX[0]  = x + ((scaledX2 + scaledY1) >> 9);
            posY[0]  = y + ((fullScaleYC * (pivotY - 2) - scaledX1) >> 9);
            break;
        }

        case FLIP_Y:
        case FLIP_XY: break;
    }

    int32 scaledXMaxS = fullScaleXS * xMax;
    int32 scaledXMaxC = fullScaleXC * xMax;
    int32 scaledYMaxC = fullScaleYC * (pivotY + 2 + height);
    int32 scaledYMaxS = fullScaleYS * (pivotY + 2 + height);
    posX[1]           = x + ((scaledXMaxC + scaledY1) >> 9);
    posY[1]           = y + ((scaledY2 - scaledXMaxS) >> 9);
    posX[2]           = x + ((scaledYMaxS + scaledX2) >> 9);
    posY[2]           = y + ((scaledYMaxC - scaledX1) >> 9);
    posX[3]           = x + ((scaledXMaxC + scaledYMaxS) >> 9);
    posY[3]           = y + ((scaledYMaxC - scaledXMaxS) >> 9);

    int32 left = currentScreen->pitch;
    for (int32 i = 0; i < 4; ++i) {
        if (posX[i] < left)
            left = posX[i];
    }
    if (left < currentScreen->clipBound_X1)
        left = currentScreen->clipBound_X1;

    int32 right = 0;
    for (int32 i = 0; i < 4; ++i) {
        if (posX[i] > right)
            right = posX[i];
    }
    if (right > currentScreen->clipBound_X2)
        right = currentScreen->clipBound_X2;

    int32 top = currentScreen->size.y;
    for (int32 i = 0; i < 4; ++i) {
        if (posY[i] < top)
            top = posY[i];
    }
    if (top < currentScreen->clipBound_Y1)
        top = currentScreen->clipBound_Y1;

    int32 bottom = 0;
    for (int32 i = 0; i < 4; ++i) {
        if (posY[i] > bottom)
            bottom = posY[i];
    }
    if (bottom > currentScreen->clipBound_Y2)
        bottom = currentScreen->clipBound_Y2;

    int32 xSize = right - left;
    int32 ySize = bottom - top;
    if (xSize >= 1 && ySize >= 1) {
        GFXSurface *surface = &gfxSurface[sheetID];

        int32 fullX            = (sprX + width) << 16;
        int32 fullY            = (sprY + height) << 16;
        validDraw              = true;
        int32 fullScaleX       = (int32)((512.0 / (float)scaleX) * 512.0);
        int32 fullScaleY       = (int32)((512.0 / (float)scaleY) * 512.0);
        int32 deltaXLen        = fullScaleX * sine >> 2;
        int32 deltaX           = fullScaleX * cosine >> 2;
        int32 pitch            = currentScreen->pitch - xSize;
        int32 deltaYLen        = fullScaleY * cosine >> 2;
        int32 deltaY           = fullScaleY * sine >> 2;
        int32 lineSize         = surface->lineSize;
        uint8 *lineBuffer      = &gfxLineBuffer[top];
        int32 xLen             = left - x;
        int32 yLen             = top - y;
        uint8 *gfxData         = surface->pixels;
        uint16 *frameBufferPtr = &currentScreen->frameBuffer[left + (top * currentScreen->pitch)];
        int32 fullSprY         = (sprY << 16) - 1;
        int32 fullSprX         = (sprX << 16) - 1;

        int32 drawX = 0, drawY = 0;
        if (direction == FLIP_X) {
            drawX     = sprXPos + deltaXLen * yLen - deltaX * xLen - (fullScaleX >> 1);
            drawY     = sprYPos + deltaYLen * yLen + deltaY * xLen;
            deltaX    = -deltaX;
            deltaXLen = -deltaXLen;
        }
        else if (!direction) {
            drawX = sprXPos + deltaX * xLen - deltaXLen * yLen;
            drawY = sprYPos + deltaYLen * yLen + deltaY * xLen;
        }

        switch (inkEffect) {
            case INK_NONE:
                for (int32 y = 0; y < ySize; ++y) {
                    uint16 *palettePtr = fullPalette[*lineBuffer++];
                    int32 drawXPos     = drawX;
                    int32 drawYPos     = drawY;
                    for (int32 x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            uint8 index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index)
                                *frameBufferPtr = palettePtr[index];
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;

            case INK_BLEND:
                for (int32 y = 0; y < ySize; ++y) {
                    uint16 *palettePtr = fullPalette[*lineBuffer++];
                    int32 drawXPos     = drawX;
                    int32 drawYPos     = drawY;
                    for (int32 x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            uint8 index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index)
                                setPixelBlend(palettePtr[index], *frameBufferPtr);
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;

            case INK_ALPHA: {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                for (int32 y = 0; y < ySize; ++y) {
                    uint16 *palettePtr = fullPalette[*lineBuffer++];
                    int32 drawXPos     = drawX;
                    int32 drawYPos     = drawY;
                    for (int32 x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            uint8 index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index) {
                                uint16 color = palettePtr[index];
                                setPixelAlpha(color, *frameBufferPtr, alpha);
                            }
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;
            }

            case INK_ADD: {
                uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];
                for (int32 y = 0; y < ySize; ++y) {
                    uint16 *palettePtr = fullPalette[*lineBuffer++];
                    int32 drawXPos     = drawX;
                    int32 drawYPos     = drawY;
                    for (int32 x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            uint8 index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index) {
                                uint16 color = palettePtr[index];
                                setPixelAdditive(color, *frameBufferPtr);
                            }
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;
            }

            case INK_SUB: {
                uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
                for (int32 y = 0; y < ySize; ++y) {
                    uint16 *palettePtr = fullPalette[*lineBuffer++];
                    int32 drawXPos     = drawX;
                    int32 drawYPos     = drawY;
                    for (int32 x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            uint8 index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index) {
                                uint16 color = palettePtr[index];
                                setPixelSubtractive(color, *frameBufferPtr);
                            }
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;
            }

            case INK_TINT:
                for (int32 y = 0; y < ySize; ++y) {
                    int32 drawXPos = drawX;
                    int32 drawYPos = drawY;
                    for (int32 x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            uint8 index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index)
                                *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;

            case INK_MASKED:
                for (int32 y = 0; y < ySize; ++y) {
                    uint16 *palettePtr = fullPalette[*lineBuffer++];
                    int32 drawXPos     = drawX;
                    int32 drawYPos     = drawY;
                    for (int32 x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            uint8 index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index && *frameBufferPtr == maskColor)
                                *frameBufferPtr = palettePtr[index];
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;

            case INK_UNMASKED:
                for (int32 y = 0; y < ySize; ++y) {
                    uint16 *palettePtr = fullPalette[*lineBuffer++];
                    int32 drawXPos     = drawX;
                    int32 drawYPos     = drawY;
                    for (int32 x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            uint8 index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index && *frameBufferPtr != maskColor)
                                *frameBufferPtr = palettePtr[index];
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;
        }
    }
}

void RSDK::DrawDeformedSprite(uint16 sheetID, int32 inkEffect, int32 alpha)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    validDraw                 = true;
    GFXSurface *surface       = &gfxSurface[sheetID];
    uint8 *gfxDataPtr         = surface->pixels;
    int32 clipY1              = currentScreen->clipBound_Y1;
    ScanlineInfo *scanlinePtr = &scanlines[clipY1];
    uint16 *frameBufferPtr    = &currentScreen->frameBuffer[clipY1 * currentScreen->pitch];
    uint8 *lineBuffer         = &gfxLineBuffer[clipY1];
    int32 width               = surface->width - 1;
    int32 height              = surface->height - 1;
    int32 lineSize            = surface->lineSize;

    switch (inkEffect) {
        case INK_NONE:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                uint16 *palettePtr = fullPalette[*lineBuffer++];
                int32 lx           = scanlinePtr->position.x;
                int32 ly           = scanlinePtr->position.y;
                int32 dx           = scanlinePtr->deform.x;
                int32 dy           = scanlinePtr->deform.y;
                for (int32 i = 0; i < currentScreen->pitch; ++i) {
                    uint8 palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex)
                        *frameBufferPtr = palettePtr[palIndex];
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;

        case INK_BLEND:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                uint16 *palettePtr = fullPalette[*lineBuffer++];
                int32 lx           = scanlinePtr->position.x;
                int32 ly           = scanlinePtr->position.y;
                int32 dx           = scanlinePtr->deform.x;
                int32 dy           = scanlinePtr->deform.y;
                for (int32 i = 0; i < currentScreen->pitch; ++i) {
                    uint8 palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex)
                        setPixelBlend(palettePtr[palIndex], *frameBufferPtr);
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;

        case INK_ALPHA: {
            uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
            uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                uint16 *palettePtr = fullPalette[*lineBuffer++];
                int32 lx           = scanlinePtr->position.x;
                int32 ly           = scanlinePtr->position.y;
                int32 dx           = scanlinePtr->deform.x;
                int32 dy           = scanlinePtr->deform.y;
                for (int32 i = 0; i < currentScreen->pitch; ++i) {
                    uint8 palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        uint16 color = palettePtr[palIndex];
                        setPixelAlpha(color, *frameBufferPtr, alpha);
                    }
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        }

        case INK_ADD: {
            uint16 *blendTablePtr = &blendLookupTable[0x20 * alpha];
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                uint16 *palettePtr = fullPalette[*lineBuffer++];
                int32 lx           = scanlinePtr->position.x;
                int32 ly           = scanlinePtr->position.y;
                int32 dx           = scanlinePtr->deform.x;
                int32 dy           = scanlinePtr->deform.y;
                for (int32 i = 0; i < currentScreen->pitch; ++i) {
                    uint8 palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        uint16 color = palettePtr[palIndex];
                        setPixelAdditive(color, *frameBufferPtr);
                    }
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        }

        case INK_SUB: {
            uint16 *subBlendTable = &subtractLookupTable[0x20 * alpha];
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                uint16 *palettePtr = fullPalette[*lineBuffer++];
                int32 lx           = scanlinePtr->position.x;
                int32 ly           = scanlinePtr->position.y;
                int32 dx           = scanlinePtr->deform.x;
                int32 dy           = scanlinePtr->deform.y;
                for (int32 i = 0; i < currentScreen->pitch; ++i) {
                    uint8 palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        uint16 color = palettePtr[palIndex];
                        setPixelSubtractive(color, *frameBufferPtr);
                    }
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        }

        case INK_TINT:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                int32 lx = scanlinePtr->position.x;
                int32 ly = scanlinePtr->position.y;
                int32 dx = scanlinePtr->deform.x;
                int32 dy = scanlinePtr->deform.y;
                for (int32 i = 0; i < currentScreen->pitch; ++i) {
                    uint8 palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex)
                        *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;

        case INK_MASKED:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                uint16 *palettePtr = fullPalette[*lineBuffer++];
                int32 lx           = scanlinePtr->position.x;
                int32 ly           = scanlinePtr->position.y;
                int32 dx           = scanlinePtr->deform.x;
                int32 dy           = scanlinePtr->deform.y;
                for (int32 i = 0; i < currentScreen->pitch; ++i) {
                    uint8 palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex && *frameBufferPtr == maskColor)
                        *frameBufferPtr = palettePtr[palIndex];
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;

        case INK_UNMASKED:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                uint16 *palettePtr = fullPalette[*lineBuffer++];
                int32 lx           = scanlinePtr->position.x;
                int32 ly           = scanlinePtr->position.y;
                int32 dx           = scanlinePtr->deform.x;
                int32 dy           = scanlinePtr->deform.y;
                for (int32 i = 0; i < currentScreen->pitch; ++i) {
                    uint8 palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex && *frameBufferPtr != maskColor)
                        *frameBufferPtr = palettePtr[palIndex];
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
    }
}

void RSDK::DrawTile(uint16 *tiles, int32 countX, int32 countY, Vector2 *position, Vector2 *offset, bool32 screenRelative)
{
    if (tiles) {
        if (!position)
            position = &sceneInfo.entity->position;

        int32 x = position->x >> 0x10;
        int32 y = position->y >> 0x10;
        if (!screenRelative) {
            x -= currentScreen->position.x;
            y -= currentScreen->position.y;
        }

        switch (sceneInfo.entity->drawFX) {
            case FX_NONE:
            case FX_FLIP: {
                int32 drawX = 0;
                int32 drawY = 0;
                if (offset) {
                    drawX = x - (offset->x >> 17);
                    drawY = y - (offset->y >> 17);
                }
                else {
                    drawX = x - 8 * countX;
                    drawY = y - 8 * countY;
                }

                for (int32 ty = 0; ty < countY; ++ty) {
                    for (int32 tx = 0; tx < countX; ++tx) {
                        uint16 tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            DrawSpriteFlipped(drawX + (tx * TILE_SIZE), drawY + (ty * TILE_SIZE), TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (tile & 0xFFF),
                                              FLIP_NONE, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                        }
                    }
                }
                break;
            }

            case FX_ROTATE: // Flip
            case FX_ROTATE | FX_FLIP: {
                int32 pivotX = 0;
                int32 pivotY = 0;
                if (offset) {
                    pivotX = -(offset->x >> 17);
                    pivotY = -(offset->y >> 17);
                }
                else {
                    pivotX = -(8 * countX);
                    pivotY = -(8 * countY);
                }

                for (int32 ty = 0; ty < countY; ++ty) {
                    for (int32 tx = 0; tx < countX; ++tx) {
                        uint16 tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            switch ((tile >> 10) & 3) {
                                case FLIP_NONE:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_NONE, sceneInfo.entity->rotation,
                                                       sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_X:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_X, sceneInfo.entity->rotation,
                                                       sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_Y:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_X, sceneInfo.entity->rotation + 0x100,
                                                       sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_XY:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_NONE, sceneInfo.entity->rotation + 0x100,
                                                       sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;
                            }
                        }
                    }
                }
                break;
            }

            case FX_SCALE: // Scale
            case FX_SCALE | FX_FLIP: {
                int32 pivotX = 0;
                int32 pivotY = 0;
                if (offset) {
                    pivotX = -(offset->x >> 17);
                    pivotY = -(offset->y >> 17);
                }
                else {
                    pivotX = -(8 * countX);
                    pivotY = -(8 * countY);
                }

                for (int32 ty = 0; ty < countY; ++ty) {
                    for (int32 tx = 0; tx < countX; ++tx) {
                        uint16 tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            switch ((tile >> 10) & 3) {
                                case FLIP_NONE:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X, 0,
                                                       sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_X:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X, 0,
                                                       sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_Y:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       0x100, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_XY:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE,
                                                       0x100, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;
                            }
                        }
                    }
                }
                break;
            }

            case FX_SCALE | FX_ROTATE: // Flip + Scale + Rotation
            case FX_SCALE | FX_ROTATE | FX_FLIP: {
                int32 pivotX = 0;
                int32 pivotY = 0;
                if (offset) {
                    pivotX = -(offset->x >> 17);
                    pivotY = -(offset->y >> 17);
                }
                else {
                    pivotX = -(8 * countX);
                    pivotY = -(8 * countY);
                }

                for (int32 ty = 0; ty < countY; ++ty) {
                    for (int32 tx = 0; tx < countX; ++tx) {
                        uint16 tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            switch ((tile >> 10) & 3) {
                                case FLIP_NONE:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       sceneInfo.entity->rotation, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_X:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       sceneInfo.entity->rotation, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_Y:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       sceneInfo.entity->rotation + 0x100, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_XY:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE,
                                                       sceneInfo.entity->rotation + 0x100, sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}
void RSDK::DrawAniTile(uint16 sheetID, uint16 tileIndex, uint16 srcX, uint16 srcY, uint16 width, uint16 height)
{

    if (sheetID < SURFACE_COUNT && tileIndex < TILE_COUNT) {
        GFXSurface *surface = &gfxSurface[sheetID];

        // FLIP_NONE
        uint8 *tilePixels = &tilesetPixels[tileIndex << 8];
        int32 cnt         = 0;
        for (int32 fy = 0; fy < height; fy += TILE_SIZE) {
            uint8 *gfxData = &surface->pixels[((fy + srcY) << surface->lineSize) + srcX];
            cnt += ((width - 1) / TILE_SIZE) + 1;
            for (int32 fx = 0; fx < width; fx += TILE_SIZE) {
                uint8 *gfxDataPtr = &gfxData[fx];
                for (int32 ty = 0; ty < TILE_SIZE; ++ty) {
                    for (int32 tx = 0; tx < TILE_SIZE; ++tx) *tilePixels++ = *gfxDataPtr++;

                    gfxDataPtr += surface->width - TILE_SIZE;
                }
            }
        }

        // FLIP_X
        uint8 *srcTilePixels = &tilesetPixels[tileIndex << 8];
        if (cnt * TILE_SIZE > 0) {
            tilePixels = &tilesetPixels[(tileIndex << 8) + (FLIP_X * TILESET_SIZE) + (TILE_SIZE - 1)];

            for (int32 i = 0; i < cnt * TILE_SIZE; ++i) {
                for (int32 p = 0; p < TILE_SIZE; ++p) *tilePixels-- = *srcTilePixels++;

                tilePixels += (TILE_SIZE * 2);
            }
        }

        // FLIP_Y
        srcTilePixels = &tilesetPixels[tileIndex << 8];
        if (cnt * TILE_SIZE > 0) {
            int32 index = tileIndex;
            for (int32 i = 0; i < cnt; ++i) {
                tilePixels = &tilesetPixels[(index << 8) + (FLIP_Y * TILESET_SIZE) + (TILE_DATASIZE - TILE_SIZE)];
                for (int32 y = 0; y < TILE_SIZE; ++y) {
                    for (int32 x = 0; x < TILE_SIZE; ++x) *tilePixels++ = *srcTilePixels++;

                    tilePixels -= (TILE_SIZE * 2);
                }
                ++index;
            }
        }

        // FLIP_XY
        srcTilePixels = &tilesetPixels[(tileIndex << 8) + (FLIP_Y * TILESET_SIZE)];
        if (cnt * TILE_SIZE > 0) {
            tilePixels = &tilesetPixels[(tileIndex << 8) + (FLIP_XY * TILESET_SIZE) + (TILE_SIZE - 1)];

            for (int32 i = 0; i < cnt * TILE_SIZE; ++i) {
                for (int32 p = 0; p < TILE_SIZE; ++p) *tilePixels-- = *srcTilePixels++;

                tilePixels += (TILE_SIZE * 2);
            }
        }
    }
}

void RSDK::DrawString(Animator *animator, Vector2 *position, String *string, int32 startFrame, int32 endFrame, int32 align, int32 spacing,
                      void *unused, Vector2 *charOffsets, bool32 screenRelative)
{
    if (animator && string && animator->frames) {
        if (!position)
            position = &sceneInfo.entity->position;

        Entity *entity = sceneInfo.entity;

        int32 x = position->x >> 0x10;
        int32 y = position->y >> 0x10;
        if (!screenRelative) {
            x -= currentScreen->position.x;
            y -= currentScreen->position.y;
        }

        startFrame = clampVal(startFrame, 0, string->length - 1);

        if (endFrame <= 0 || endFrame > string->length)
            endFrame = string->length;

        switch (align) {
            case ALIGN_LEFT:
                if (charOffsets) {
                    for (; startFrame < endFrame; ++startFrame) {
                        uint16 curChar = string->chars[startFrame];
                        if (curChar < animator->frameCount) {
                            SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x + (charOffsets->x >> 0x10), y + frame->pivotY + (charOffsets->y >> 0x10), frame->width, frame->height,
                                              frame->sprX, frame->sprY, FLIP_NONE, entity->inkEffect, entity->alpha, frame->sheetID);
                            x += spacing + frame->width;
                            ++charOffsets;
                        }
                    }
                }
                else {
                    for (; startFrame < endFrame; ++startFrame) {
                        uint16 curChar = string->chars[startFrame];
                        if (curChar < animator->frameCount) {
                            SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x, y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                              entity->inkEffect, entity->alpha, frame->sheetID);
                            x += spacing + frame->width;
                        }
                    }
                }
                break;

            case ALIGN_RIGHT: break;

            case ALIGN_CENTER:
                --endFrame;
                if (charOffsets) {
                    for (Vector2 *charOffset = &charOffsets[endFrame]; endFrame >= startFrame; --endFrame) {
                        uint16 curChar = string->chars[endFrame];
                        if (curChar < animator->frameCount) {
                            SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x - frame->width + (charOffset->x >> 0x10), y + frame->pivotY + (charOffset->y >> 0x10), frame->width,
                                              frame->height, frame->sprX, frame->sprY, FLIP_NONE, entity->inkEffect, entity->alpha, frame->sheetID);
                            x = (x - frame->width) - spacing;
                            --charOffset;
                        }
                    }
                }
                else {
                    for (; endFrame >= startFrame; --endFrame) {
                        uint16 curChar = string->chars[endFrame];
                        if (curChar < animator->frameCount) {
                            SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x - frame->width, y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                              entity->inkEffect, entity->alpha, frame->sheetID);
                            x = (x - frame->width) - spacing;
                        }
                    }
                }
                break;
        }
    }
}
void RSDK::DrawDevString(const char *string, int32 x, int32 y, int32 align, uint32 color)
{
    uint16 color16 = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];

    int32 charOffset   = 0;
    bool32 linesRemain = true;
    while (linesRemain) {
        linesRemain = false;

        int32 lineSize = 0;
        char cur       = string[charOffset];
        if (cur != '\n') {
            while (cur) {
                cur = string[++charOffset];
                lineSize++;
                if (cur == '\n') {
                    linesRemain = true;
                    break;
                }
            }
        }

        if (y >= 0 && y < currentScreen->size.y - 7) {
            int32 offset = 0;
            switch (align) {
                default:
                case ALIGN_LEFT: offset = 0; break;

                case ALIGN_CENTER: offset = 4 * lineSize; break;

                case ALIGN_RIGHT: offset = 8 * lineSize; break;
            }
            int32 drawX = x - offset;

            const char *curChar = &string[charOffset++ - lineSize];

            for (int32 c = 0; c < lineSize; ++c) {
                if (drawX >= 0 && drawX < currentScreen->size.x - 7) {
                    uint16 *frameBufferPtr = &currentScreen->frameBuffer[drawX + y * currentScreen->pitch];

                    if ((*curChar < '\t' || *curChar > '\n') && *curChar != ' ') {
                        uint8 *textStencilPtr = &devTextStencil[0x40 * *curChar];

                        for (int32 h = 0; h < 8; ++h) {
                            for (int32 w = 0; w < 8; ++w) {
                                if (*textStencilPtr)
                                    *frameBufferPtr = color16;

                                ++textStencilPtr;
                                ++frameBufferPtr;
                            }

                            frameBufferPtr = &frameBufferPtr[currentScreen->pitch - 8];
                        }
                    }

                    ++curChar;
                    drawX += 8;
                }
            }
        }

        y += 8;
    }
}
