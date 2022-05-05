#include "RSDK/Core/RetroEngine.hpp"

#if RETRO_USING_DIRECTX9
#include <D3Dcompiler.h>
#endif

DrawList drawLayers[DRAWLAYER_COUNT];

uint16 blendLookupTable[0x20 * 0x100];
uint16 subtractLookupTable[0x20 * 0x100];

GFXSurface gfxSurface[SURFACE_MAX];

float dpi         = 1;
int32 cameraCount = 0;
ScreenInfo screens[SCREEN_MAX];
CameraInfo cameras[CAMERA_MAX];
ScreenInfo *currentScreen = NULL;

RenderVertex vertexBuffer[!RETRO_REV02 ? 24 : 60];

int32 shaderCount = 0;
ShaderEntry shaderList[SHADER_MAX];

int RenderDevice::isRunning          = true;
int RenderDevice::windowRefreshDelay = 0;

#if RETRO_REV02
uint8 RenderDevice::startVertex_2P[] = { 18, 24 };
uint8 RenderDevice::startVertex_3P[] = { 30, 36, 12 };
#endif

float2 RenderDevice::pixelSize   = { 424, 240 };
float2 RenderDevice::textureSize = { 512.0, 256.0 };
float2 RenderDevice::viewSize    = { 0, 0 };

int RenderDevice::displayWidth[16];
int RenderDevice::displayHeight[16];
int RenderDevice::displayCount = 0;
WindowInfo RenderDevice::displayInfo;

int RenderDevice::lastShaderID = -1;

#if RETRO_USING_DIRECTX9

HWND RenderDevice::windowHandle;

HDEVNOTIFY RenderDevice::deviceNotif = 0;
PAINTSTRUCT RenderDevice::Paint;

IDirect3D9 *RenderDevice::dx9Context;
IDirect3DDevice9 *RenderDevice::dx9Device;
UINT RenderDevice::dxAdapter;
IDirect3DVertexDeclaration9 *RenderDevice::dx9VertexDeclare;
IDirect3DVertexBuffer9 *RenderDevice::dx9VertexBuffer;
IDirect3DTexture9 *RenderDevice::screenTextures[4];
IDirect3DTexture9 *RenderDevice::imageTexture;
D3DVIEWPORT9 RenderDevice::dx9ViewPort;

int RenderDevice::adapterCount = 0;
RECT RenderDevice::monitorDisplayRect;
GUID RenderDevice::deviceIdentifier;

bool RenderDevice::useFrequency = false;

LARGE_INTEGER RenderDevice::performanceCount;
LARGE_INTEGER RenderDevice::frequency;
LARGE_INTEGER RenderDevice::initialFrequency;
LARGE_INTEGER RenderDevice::curFrequency;

// WinMain args
HINSTANCE RenderDevice::hInstance;
HINSTANCE RenderDevice::hPrevInstance;
INT RenderDevice::nShowCmd;
#endif

#if RETRO_USING_SDL2
SDL_Window *RenderDevice::window     = nullptr;
SDL_Renderer *RenderDevice::renderer = nullptr;
SDL_Texture *RenderDevice::screenTexture[SCREEN_MAX];

SDL_Texture *RenderDevice::imageTexture = nullptr;

uint32 RenderDevice::displayModeIndex = 0;
int32 RenderDevice::displayModeCount  = 0;

unsigned long long RenderDevice::targetFreq = 0;
unsigned long long RenderDevice::curTicks   = 0;
unsigned long long RenderDevice::prevTicks  = 0;

// clang-format off
SDL_Color vertexColorBuffer[60] = {
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
};
// clang-format on
#endif

char drawGroupNames[0x10][0x10] = {
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

#define NORMALIZE(val, minVal, maxVal) ((float)(val) - (float)(minVal)) / ((float)(maxVal) - (float)(minVal))

bool RenderDevice::Init()
{
#if RETRO_USING_DIRECTX9
    // shit workaround since windows is BEGGING me to use wide strs
    std::string str    = RSDK::gameVerInfo.gameName;
    std::wstring stemp = std::wstring(str.begin(), str.end());
    LPCWSTR gameTitle  = stemp.c_str();

    HMODULE handle = GetModuleHandle(NULL);

    WNDCLASS wndClass;
    wndClass.style         = CS_VREDRAW | CS_HREDRAW;
    wndClass.lpfnWndProc   = WindowEventCallback;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 4;
    wndClass.hInstance     = hInstance;
    wndClass.hIcon         = LoadIcon(handle, MAKEINTRESOURCE(101));
    wndClass.hCursor       = LoadCursor(0, IDI_APPLICATION);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName  = 0;
    wndClass.lpszClassName = gameTitle;
    if (!RegisterClass(&wndClass))
        return false;

    ShowCursor(false);

    tagRECT winRect;
    if (RSDK::gameSettings.windowed) {
        winRect.left   = (GetSystemMetrics(0) - RSDK::gameSettings.windowWidth) / 2;
        winRect.right  = RSDK::gameSettings.windowWidth + winRect.left;
        winRect.top    = (GetSystemMetrics(1) - RSDK::gameSettings.windowHeight) / 2;
        winRect.bottom = RSDK::gameSettings.windowHeight + winRect.top;
    }
    else if (RSDK::gameSettings.fsWidth <= 0 || RSDK::gameSettings.fsHeight <= 0) {
        winRect.left   = 0;
        winRect.right  = GetSystemMetrics(0);
        winRect.top    = 0;
        winRect.bottom = GetSystemMetrics(1);
    }
    else {
        winRect.left   = (GetSystemMetrics(0) - RSDK::gameSettings.fsWidth) / 2;
        winRect.right  = RSDK::gameSettings.fsWidth + winRect.left;
        winRect.top    = (GetSystemMetrics(1) - RSDK::gameSettings.fsHeight) / 2;
        winRect.bottom = RSDK::gameSettings.fsHeight + winRect.top;
    }

    if (RSDK::gameSettings.bordered && RSDK::gameSettings.windowed) {
        AdjustWindowRect(&winRect, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_GROUP, 0);
        windowHandle = CreateWindowEx(0, gameTitle, gameTitle, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_GROUP, winRect.left, winRect.top,
                                      winRect.right - winRect.left, winRect.bottom - winRect.top, 0, 0, hInstance, 0);
    }
    else {
        AdjustWindowRect(&winRect, WS_POPUP, 0);
        windowHandle = CreateWindowEx(0, gameTitle, gameTitle, WS_POPUP, winRect.left, winRect.top, winRect.right - winRect.left,
                                      winRect.bottom - winRect.top, 0, 0, hInstance, 0);
    }

    PrintLog(PRINT_NORMAL, "w: %d h: %d windowed: %d\n", winRect.right - winRect.left, winRect.bottom - winRect.top, RSDK::gameSettings.windowed);

    if (!windowHandle)
        return false;

    ShowWindow(windowHandle, nShowCmd);
    UpdateWindow(windowHandle);
#endif

#if RETRO_USING_SDL2
    const char *gameTitle = RSDK::gameVerInfo.gameName;

    SDL_Init(SDL_INIT_EVERYTHING);

    byte flags = 0;

#if RETRO_PLATFORM == RETRO_ANDROID || RETRO_PLATFORM == RETRO_SWITCH
    RSDK::gameSettings.windowed = false;
    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);
    float hdp = 0, vdp = 0;

    bool landscape = dm.h < dm.w;
    int h          = landscape ? dm.w : dm.h;
    int w          = landscape ? dm.h : dm.w;

    RSDK::gameSettings.windowWidth = pixWidth = ((float)SCREEN_YSIZE * h / w);
#endif

#if RETRO_PLATFORM == RETRO_SWITCH
    RSDK::gameSettings.windowWidth  = 1920;
    RSDK::gameSettings.windowHeight = 1080;
    flags |= SDL_WINDOW_FULLSCREEN;
#endif

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, RSDK::gameSettings.vsync ? "1" : "0");

    window = SDL_CreateWindow(gameTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RSDK::gameSettings.windowWidth,
                              RSDK::gameSettings.windowHeight, SDL_WINDOW_ALLOW_HIGHDPI | flags);

    if (!window) {
        PrintLog(PRINT_NORMAL, "ERROR: failed to create window!");
        return false;
    }

    if (!RSDK::gameSettings.windowed) {
        SDL_RestoreWindow(window);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_ShowCursor(SDL_FALSE);
    }

    if (!RSDK::gameSettings.bordered) {
        SDL_RestoreWindow(window);
        SDL_SetWindowBordered(window, SDL_FALSE);
    }

    PrintLog(PRINT_NORMAL, "w: %d h: %d windowed: %d\n", RSDK::gameSettings.windowWidth, RSDK::gameSettings.windowHeight,
             RSDK::gameSettings.windowed);
#endif

    if (!SetupRendering() || !InitAudioDevice())
        return false;

    InitInputDevices();
    return true;
}

void RenderDevice::CopyFrameBuffer()
{
#if RETRO_USING_DIRECTX9
    dx9Device->SetTexture(0, NULL);

    for (int s = 0; s < RSDK::gameSettings.screenCount; ++s) {
        D3DLOCKED_RECT rect;

        if (screenTextures[s]->LockRect(0, &rect, NULL, D3DLOCK_DISCARD) == 0) {
            WORD *pixels           = (WORD *)rect.pBits;
            uint16 *frameBufferPtr = screens[s].frameBuffer;

            int screenPitch = screens[s].pitch;
            int pitch       = (rect.Pitch >> 1) - screenPitch;

            for (int32 y = 0; y < 240; ++y) {
                int32 pixelCount = screenPitch >> 4;
                for (int32 x = 0; x < pixelCount; ++x) {
                    pixels[0]  = frameBufferPtr[0];
                    pixels[1]  = frameBufferPtr[1];
                    pixels[2]  = frameBufferPtr[2];
                    pixels[3]  = frameBufferPtr[3];
                    pixels[4]  = frameBufferPtr[4];
                    pixels[5]  = frameBufferPtr[5];
                    pixels[6]  = frameBufferPtr[6];
                    pixels[7]  = frameBufferPtr[7];
                    pixels[8]  = frameBufferPtr[8];
                    pixels[9]  = frameBufferPtr[9];
                    pixels[10] = frameBufferPtr[10];
                    pixels[11] = frameBufferPtr[11];
                    pixels[12] = frameBufferPtr[12];
                    pixels[13] = frameBufferPtr[13];
                    pixels[14] = frameBufferPtr[14];
                    pixels[15] = frameBufferPtr[15];

                    frameBufferPtr += 16;
                    pixels += 16;
                }

                pixels += pitch;
            }

            screenTextures[s]->UnlockRect(0);
        }
    }
#endif

#if RETRO_USING_SDL2
    int32 pitch    = 0;
    uint16 *pixels = NULL;

    for (int32 s = 0; s < RSDK::gameSettings.screenCount; ++s) {
        SDL_LockTexture(screenTexture[s], NULL, (void **)&pixels, &pitch);

        uint16 *frameBufferPtr = screens[s].frameBuffer;
        for (int32 y = 0; y < SCREEN_YSIZE; ++y) {
            memcpy(pixels, frameBufferPtr, screens[s].size.x * sizeof(uint16));
            frameBufferPtr += screens[s].pitch;
            pixels += pitch / sizeof(uint16);
        }

        SDL_UnlockTexture(screenTexture[s]);
    }
#endif
}

void RenderDevice::FlipScreen()
{
    if (RSDK::gameSettings.dimTimer < RSDK::gameSettings.dimLimit) {
        if (RSDK::gameSettings.dimPercent < 1.0) {
            RSDK::gameSettings.dimPercent += 0.05;
            if (RSDK::gameSettings.dimPercent > 1.0)
                RSDK::gameSettings.dimPercent = 1.0;
        }
    }
    else if (RSDK::gameSettings.dimPercent > 0.25) {
        RSDK::gameSettings.dimPercent *= 0.9;
    }

    if (windowRefreshDelay > 0) {
        windowRefreshDelay--;
        if (!windowRefreshDelay)
            UpdateGameWindow();
        return;
    }

#if RETRO_USING_DIRECTX9
    dx9Device->SetViewport(&displayInfo.viewport);
    dx9Device->Clear(0, 0, 1, 0xFF000000, 0x3F800000, 0);
    dx9Device->SetViewport(&dx9ViewPort);

    if (dx9Device->BeginScene() >= 0) {
        // reload shader if needed
        if (lastShaderID != RSDK::gameSettings.shaderID) {
            lastShaderID = RSDK::gameSettings.shaderID;

            if (shaderList[RSDK::gameSettings.shaderID].linear)
                dx9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            else
                dx9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
            dx9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

            if (RSDK::gameSettings.shaderSupport) {
                dx9Device->SetVertexShader(shaderList[RSDK::gameSettings.shaderID].vertexShaderObject);
                dx9Device->SetPixelShader(shaderList[RSDK::gameSettings.shaderID].pixelShaderObject);
                dx9Device->SetVertexDeclaration(dx9VertexDeclare);
                dx9Device->SetStreamSource(0, dx9VertexBuffer, 0, 24);
            }
            else {
                dx9Device->SetStreamSource(0, dx9VertexBuffer, 0, 24);
                dx9Device->SetFVF(D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ);
            }
        }

        if (RSDK::gameSettings.shaderSupport) {
            float2 dimAmount = { 0, 0 };
            dimAmount.x      = RSDK::gameSettings.dimMax * RSDK::gameSettings.dimPercent;

            dx9Device->SetPixelShaderConstantF(0, &pixelSize.x, 1);   // pixelSize
            dx9Device->SetPixelShaderConstantF(1, &textureSize.x, 1); // textureSize
            dx9Device->SetPixelShaderConstantF(2, &viewSize.x, 1);    // viewSize
            dx9Device->SetPixelShaderConstantF(3, &dimAmount.x, 1);   // screenDim
        }

        int32 startVert = 0;
        switch (RSDK::gameSettings.screenCount) {
            default:
            case 0:
#if RETRO_REV02
                startVert = 54;
#else
                startVert = 18;
#endif
                dx9Device->SetTexture(0, imageTexture);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVert, 2);
                dx9Device->EndScene();
                break;

            case 1:
                dx9Device->SetTexture(0, screenTextures[0]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
                break;

            case 2:
#if RETRO_REV02
                startVert = startVertex_2P[0];
#else
                startVert = 6;
#endif
                dx9Device->SetTexture(0, screenTextures[0]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVert, 2);

#if RETRO_REV02
                startVert = startVertex_2P[1];
#else
                startVert = 12;
#endif
                dx9Device->SetTexture(0, screenTextures[1]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVert, 2);
                break;

#if RETRO_REV02
            case 3:
                dx9Device->SetTexture(0, screenTextures[0]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex_3P[0], 2);

                dx9Device->SetTexture(0, screenTextures[1]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex_3P[1], 2);

                dx9Device->SetTexture(0, screenTextures[2]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex_3P[2], 2);
                break;

            case 4:
                dx9Device->SetTexture(0, screenTextures[0]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 30, 2);

                dx9Device->SetTexture(0, screenTextures[1]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 36, 2);

                dx9Device->SetTexture(0, screenTextures[2]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 42, 2);

                dx9Device->SetTexture(0, screenTextures[3]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 48, 2);
                break;
#endif
        }

        dx9Device->EndScene();
    }

    if (dx9Device->Present(NULL, NULL, NULL, NULL) < 0)
        windowRefreshDelay = 8;
#endif

#if RETRO_USING_SDL2
    float dimAmount = RSDK::gameSettings.dimMax * RSDK::gameSettings.dimPercent;

    // Clear the screen. This is needed to keep the
    // pillarboxes in fullscreen from displaying garbage data.
    SDL_RenderClear(renderer);

    int32 startVert = 0;
    switch (RSDK::gameSettings.screenCount) {
        default:
        case 0:
#if RETRO_REV02
            startVert = 54;
#else
            startVert = 18;
#endif
            SDL_RenderGeometryRaw(renderer, imageTexture, &vertexBuffer[startVert].pos.x, sizeof(RenderVertex), &vertexColorBuffer[startVert],
                                  sizeof(SDL_Color), &vertexBuffer[startVert].tex.x, sizeof(RenderVertex), 6, NULL, 0, 0);
            break;

        case 1:
            startVert = 0;
            SDL_RenderGeometryRaw(renderer, screenTexture[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex), &vertexColorBuffer[startVert],
                                  sizeof(SDL_Color), &vertexBuffer[startVert].tex.x, sizeof(RenderVertex), 6, NULL, 0, 0);
            break;

        case 2:
#if RETRO_REV02
            startVert = startVertex_2P[0];
#else
            startVert = 6;
#endif
            SDL_RenderGeometryRaw(renderer, screenTexture[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex), &vertexColorBuffer[startVert],
                                  sizeof(SDL_Color), &vertexBuffer[startVert].tex.x, sizeof(RenderVertex), 6, NULL, 0, 0);

#if RETRO_REV02
            startVert = startVertex_2P[1];
#else
            startVert = 12;
#endif
            SDL_RenderGeometryRaw(renderer, screenTexture[1], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex), &vertexColorBuffer[startVert],
                                  sizeof(SDL_Color), &vertexBuffer[startVert].tex.x, sizeof(RenderVertex), 6, NULL, 0, 0);
            break;

#if RETRO_REV02
        case 3:
            startVert = startVertex_3P[0];
            SDL_RenderGeometryRaw(renderer, screenTexture[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex), &vertexColorBuffer[startVert],
                                  sizeof(SDL_Color), &vertexBuffer[startVert].tex.x, sizeof(RenderVertex), 6, NULL, 0, 0);

            startVert = startVertex_3P[1];
            SDL_RenderGeometryRaw(renderer, screenTexture[1], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex), &vertexColorBuffer[startVert],
                                  sizeof(SDL_Color), &vertexBuffer[startVert].tex.x, sizeof(RenderVertex), 6, NULL, 0, 0);

            startVert = startVertex_3P[2];
            SDL_RenderGeometryRaw(renderer, screenTexture[2], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex), &vertexColorBuffer[startVert],
                                  sizeof(SDL_Color), &vertexBuffer[startVert].tex.x, sizeof(RenderVertex), 6, NULL, 0, 0);
            break;

        case 4:
            startVert = 30;
            SDL_RenderGeometryRaw(renderer, screenTexture[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex), &vertexColorBuffer[startVert],
                                  sizeof(SDL_Color), &vertexBuffer[startVert].tex.x, sizeof(RenderVertex), 6, NULL, 0, 0);

            startVert = 36;
            SDL_RenderGeometryRaw(renderer, screenTexture[1], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex), &vertexColorBuffer[startVert],
                                  sizeof(SDL_Color), &vertexBuffer[startVert].tex.x, sizeof(RenderVertex), 6, NULL, 0, 0);

            startVert = 42;
            SDL_RenderGeometryRaw(renderer, screenTexture[2], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex), &vertexColorBuffer[startVert],
                                  sizeof(SDL_Color), &vertexBuffer[startVert].tex.x, sizeof(RenderVertex), 6, NULL, 0, 0);

            startVert = 48;
            SDL_RenderGeometryRaw(renderer, screenTexture[3], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex), &vertexColorBuffer[startVert],
                                  sizeof(SDL_Color), &vertexBuffer[startVert].tex.x, sizeof(RenderVertex), 6, NULL, 0, 0);
            break;
#endif
    }

    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF - (dimAmount * 0xFF));
    if (dimAmount < 1.0)
        SDL_RenderFillRect(renderer, NULL);
    // no change here
    SDL_RenderPresent(renderer);
#endif
}

void RenderDevice::Release(bool isRefresh)
{
#if RETRO_USING_DIRECTX9
    if (RSDK::gameSettings.shaderSupport) {
        for (int32 i = 0; i < shaderCount; ++i) {
            if (shaderList[i].vertexShaderObject)
                shaderList[i].vertexShaderObject->Release();
            shaderList[i].vertexShaderObject = NULL;

            if (shaderList[i].pixelShaderObject)
                shaderList[i].pixelShaderObject->Release();
            shaderList[i].pixelShaderObject = NULL;
        }

        shaderCount = 0;
    }

    if (imageTexture) {
        imageTexture->Release();
        imageTexture = NULL;
    }

    for (int32 i = 0; i < SCREEN_MAX; ++i) {
        if (screenTextures[i])
            screenTextures[i]->Release();

        screenTextures[i] = NULL;
    }

    if (displayInfo.displays)
        free(displayInfo.displays);
    displayInfo.displays = NULL;

    if (dx9VertexBuffer) {
        dx9VertexBuffer->Release();
        dx9VertexBuffer = NULL;
    }

    if (isRefresh && dx9VertexDeclare) {
        dx9VertexDeclare->Release();
        dx9VertexDeclare = NULL;
    }

    if (dx9Device) {
        dx9Device->Release();
        dx9Device = NULL;
    }

    if (!isRefresh && dx9Context) {
        dx9Context->Release();
        dx9Context = NULL;
    }
#endif

#if RETRO_USING_SDL2
    for (int32 s = 0; s < SCREEN_MAX; ++s) {
        if (screenTexture[s])
            SDL_DestroyTexture(screenTexture[s]);
        screenTexture[s] = NULL;
    }

    if (imageTexture)
        SDL_DestroyTexture(imageTexture);
    imageTexture = NULL;

    if (!isRefresh && renderer)
        SDL_DestroyRenderer(renderer);

    if (!isRefresh && window)
        SDL_DestroyWindow(window);

    if (!isRefresh)
        SDL_Quit();
#endif

    if (!isRefresh) {
        if (scanlines)
            free(scanlines);
        scanlines = NULL;
    }
}

void RenderDevice::RefreshWindow()
{
    RSDK::gameSettings.windowState = WINDOWSTATE_UNINITIALIZED;

    RenderDevice::Release(true);

#if RETRO_USING_DIRECTX9
    ShowWindow(RenderDevice::windowHandle, false);

    if (RSDK::gameSettings.windowed && RSDK::gameSettings.bordered)
        SetWindowLong(RenderDevice::windowHandle, GWL_STYLE, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_GROUP);
    else
        SetWindowLong(RenderDevice::windowHandle, GWL_STYLE, WS_POPUP);

    Sleep(250); // zzzz.. mimimimi..

    GetDisplays();

    tagRECT rect;
    if (RSDK::gameSettings.windowed || !RSDK::gameSettings.exclusiveFS) {
        tagRECT winRect;
        GetClientRect(RenderDevice::windowHandle, &winRect);

        tagPOINT topLeft, bottomRight;
        topLeft.x     = winRect.left;
        topLeft.y     = winRect.top;
        bottomRight.x = winRect.right;
        bottomRight.y = winRect.bottom;

        ClientToScreen(RenderDevice::windowHandle, &topLeft);
        ClientToScreen(RenderDevice::windowHandle, &bottomRight);

        if (RSDK::gameSettings.windowed) {
            D3DDISPLAYMODE displayMode;
            dx9Context->GetAdapterDisplayMode(dxAdapter, &displayMode);

            if (RSDK::gameSettings.windowWidth >= displayMode.Width || RSDK::gameSettings.windowHeight >= displayMode.Height) {
                RSDK::gameSettings.windowWidth  = (displayMode.Height / 480 * RSDK::gameSettings.pixWidth);
                RSDK::gameSettings.windowHeight = displayMode.Height / 480 * RSDK::gameSettings.pixHeight;
            }

            rect.left   = (bottomRight.x + topLeft.x) / 2 - RSDK::gameSettings.windowWidth / 2;
            rect.top    = (bottomRight.y + topLeft.y) / 2 - RSDK::gameSettings.windowHeight / 2;
            rect.right  = (bottomRight.x + topLeft.x) / 2 + RSDK ::gameSettings.windowWidth / 2;
            rect.bottom = (bottomRight.y + topLeft.y) / 2 + RSDK::gameSettings.windowHeight / 2;

            if (RSDK::gameSettings.bordered)
                AdjustWindowRect(&rect, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_GROUP, false);
            else
                AdjustWindowRect(&rect, WS_POPUP, false);

            if (rect.left < monitorDisplayRect.left || rect.right > monitorDisplayRect.right || rect.top < monitorDisplayRect.top
                || rect.bottom > monitorDisplayRect.bottom) {
                rect.left   = (monitorDisplayRect.right + monitorDisplayRect.left) / 2 - RSDK::gameSettings.windowWidth / 2;
                rect.top    = (monitorDisplayRect.top + monitorDisplayRect.bottom) / 2 - RSDK::gameSettings.windowHeight / 2;
                rect.right  = (monitorDisplayRect.right + monitorDisplayRect.left) / 2 + RSDK::gameSettings.windowWidth / 2;
                rect.bottom = (monitorDisplayRect.top + monitorDisplayRect.bottom) / 2 + RSDK::gameSettings.windowHeight / 2;

                if (RSDK::gameSettings.bordered)
                    AdjustWindowRect(&rect, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_GROUP, false);
                else
                    AdjustWindowRect(&rect, WS_POPUP, false);
            }
        }
        else {
            rect = monitorDisplayRect;
            AdjustWindowRect(&monitorDisplayRect, 0x80000000, 0);
        }

        SetWindowPos(RenderDevice::windowHandle, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0x40);
    }

    ShowWindow(RenderDevice::windowHandle, SW_SHOW);
    RedrawWindow(RenderDevice::windowHandle, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
#endif

#if RETRO_USING_SDL2
    SDL_HideWindow(RenderDevice::window);

    if (RSDK::gameSettings.windowed && RSDK::gameSettings.bordered)
        SDL_SetWindowBordered(window, SDL_TRUE);
    else
        SDL_SetWindowBordered(window, SDL_FALSE);

    GetDisplays();

    SDL_Rect winRect;
    winRect.x = SDL_WINDOWPOS_CENTERED;
    winRect.y = SDL_WINDOWPOS_CENTERED;
    if (RSDK::gameSettings.windowed || !RSDK::gameSettings.exclusiveFS) {
        int32 currentWindowDisplay = SDL_GetWindowDisplayIndex(window);
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(currentWindowDisplay, &displayMode);

        if (RSDK::gameSettings.windowed) {
            if (RSDK::gameSettings.windowWidth >= displayMode.w || RSDK::gameSettings.windowHeight >= displayMode.h) {
                RSDK::gameSettings.windowWidth  = (displayMode.h / 480 * RSDK::gameSettings.pixWidth);
                RSDK::gameSettings.windowHeight = displayMode.h / 480 * RSDK::gameSettings.pixHeight;
            }

            winRect.w = RSDK::gameSettings.windowWidth;
            winRect.h = RSDK::gameSettings.windowHeight;
            SDL_SetWindowFullscreen(window, SDL_FALSE);
            SDL_ShowCursor(SDL_FALSE);
        }
        else {
            winRect.w = displayMode.w;
            winRect.h = displayMode.h;
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            SDL_ShowCursor(SDL_TRUE);
        }

        SDL_SetWindowSize(RenderDevice::window, winRect.w, winRect.h);
        SDL_SetWindowPosition(RenderDevice::window, winRect.x, winRect.y);
    }

    SDL_ShowWindow(RenderDevice::window);
#endif

    if (!InitGraphicsAPI() || !InitShaders())
        return;

    RSDK::gameSettings.windowState = WINDOWSTATE_ACTIVE;
}

void RenderDevice::InitFPSCap()
{
#if RETRO_USING_DIRECTX9
    if (QueryPerformanceFrequency(&frequency)) {
        useFrequency              = true;
        initialFrequency.QuadPart = frequency.QuadPart / RSDK::gameSettings.refreshRate;
        QueryPerformanceCounter(&performanceCount);
    }
    else {
        useFrequency              = false;
        performanceCount.QuadPart = timeGetTime();
    }
#endif

#if RETRO_USING_SDL2
    targetFreq = SDL_GetPerformanceFrequency() / RSDK::gameSettings.refreshRate;
    curTicks   = 0;
    prevTicks  = 0;
#endif
}
bool RenderDevice::CheckFPSCap()
{
#if RETRO_USING_DIRECTX9
    if (useFrequency)
        QueryPerformanceCounter(&curFrequency);
    else
        curFrequency.QuadPart = timeGetTime();

    if (curFrequency.QuadPart > performanceCount.QuadPart)
        return true;
#endif

#if RETRO_USING_SDL2
    curTicks = SDL_GetPerformanceCounter();
    if (curTicks >= prevTicks + targetFreq)
        return true;
#endif

    return false;
}
void RenderDevice::UpdateFPSCap()
{
#if RETRO_USING_DIRECTX9
    performanceCount.QuadPart = curFrequency.QuadPart + initialFrequency.LowPart;
#endif

#if RETRO_USING_SDL2
    prevTicks = curTicks;
#endif
}

void RenderDevice::InitVertexBuffer()
{
// clang-format off
#if RETRO_REV02
RenderVertex vertBuffer[60] = {
    // 1 Screen (0)
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    
    // 2 Screens - Bordered (Top Screen) (6)
    { { -0.5,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { { -0.5,  0.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  0.5,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -0.5,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  0.5,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  0.5,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    
    // 2 Screens - Bordered (Bottom Screen) (12)
    { { -0.5,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { { -0.5, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  0.5, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -0.5,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  0.5,  0.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  0.5, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // 2 Screens - Stretched (Top Screen)  (18)
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // 2 Screens - Stretched (Bottom Screen) (24)
    { { -1.0, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // 4 Screens (Top-Left) (30)
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  0.0,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { {  0.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // 4 Screens (Top-Right) (36)
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { {  0.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // 4 Screens (Bottom-Left) (42)
    { { -1.0, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  0.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  0.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // 4 Screens (Bottom-Right) (48)
    { {  0.0, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // Image/Video (54)
    { { -1.0, -1.0,  1.0,  1.0 }, {  0.0,  1.0 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  1.0,  1.0 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0,  1.0 }, {  1.0,  0.0 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  1.0,  1.0 } }
};
#else
RenderVertex vertexList[24] =
{
  // 1 Screen (0)
  { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
  { { -1.0, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
  { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
  { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
  { {  1.0,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
  { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },

  // 2 Screens - Stretched (Top Screen) (6)
  { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
  { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
  { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
  { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
  { {  1.0,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
  { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
  
  // 2 Screens - Stretched (Bottom Screen) (12)
  { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
  { { -1.0, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
  { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
  { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
  { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.0 } },
  { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
  
    // Image/Video (18)
  { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
  { { -1.0, -1.0,  1.0,  1.0 }, {  0.0,  1.0 } },
  { {  1.0, -1.0,  1.0,  1.0 }, {  1.0,  1.0 } },
  { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
  { {  1.0,  1.0,  1.0,  1.0 }, {  1.0,  0.0 } },
  { {  1.0, -1.0,  1.0,  1.0 }, {  1.0,  1.0 } }
};
#endif
    // clang-format on

#if RETRO_USING_SDL2
    for (int v = 0; v < (!RETRO_REV02 ? 24 : 60); ++v) {
        RenderVertex *vertex = &vertBuffer[v];
        vertex->pos.x        = NORMALIZE(vertex->pos.x, -1.0, 1.0) * RSDK::gameSettings.pixWidth;
        vertex->pos.y        = (1.0 - NORMALIZE(vertex->pos.y, -1.0, 1.0)) * SCREEN_YSIZE;

        if (vertex->tex.x)
            vertex->tex.x = screens[0].size.x * (1.0 / textureSize.x);

        if (vertex->tex.y)
            vertex->tex.y = screens[0].size.y * (1.0 / textureSize.y);
    }

    memcpy(vertexBuffer, vertBuffer, sizeof(vertBuffer));
#endif

#if RETRO_USING_DIRECTX9
    float x = 0.5 / (float)viewSize.x;
    float y = 0.5 / (float)viewSize.y;

    for (int v = 0; v < (!RETRO_REV02 ? 24 : 60); ++v) {
        RenderVertex *vertex = &vertBuffer[v];
        vertex->pos.x        = vertex->pos.x - x;
        vertex->pos.y        = vertex->pos.y + y;

        if (vertex->tex.x)
            vertex->tex.x = screens[0].size.x * (1.0 / textureSize.x);

        if (vertex->tex.y)
            vertex->tex.y = screens[0].size.y * (1.0 / textureSize.y);
    }

    RenderVertex *vertBufferPtr;
    if (dx9VertexBuffer->Lock(0, 0, (void **)&vertBufferPtr, 0) >= 0) {
        memcpy(vertBufferPtr, vertBuffer, sizeof(vertBuffer));
        dx9VertexBuffer->Unlock();
    }
#endif
}

bool RenderDevice::InitGraphicsAPI()
{
    RSDK::gameSettings.shaderSupport = false;

#if RETRO_USING_DIRECTX9
    D3DCAPS9 pCaps;
    if (dx9Context->GetDeviceCaps(0, D3DDEVTYPE_HAL, &pCaps) >= 0 && (pCaps.PixelShaderVersion & 0xFF00) >= 0x300)
        RSDK::gameSettings.shaderSupport = true;

    viewSize.x = 0;
    viewSize.y = 0;

    D3DPRESENT_PARAMETERS presentParams;
    memset(&presentParams, 0, sizeof(presentParams));
    if (RSDK::gameSettings.windowed || !RSDK::gameSettings.exclusiveFS) {
        presentParams.BackBufferFormat     = D3DFMT_UNKNOWN;
        presentParams.BackBufferCount      = 1;
        presentParams.SwapEffect           = D3DSWAPEFFECT_DISCARD;
        presentParams.PresentationInterval = 0;
        presentParams.hDeviceWindow        = windowHandle;
        presentParams.Windowed             = true;
        if (RSDK::gameSettings.windowed) {
            viewSize.x = RSDK::gameSettings.windowWidth;
            viewSize.y = RSDK::gameSettings.windowHeight;
        }
        else {
            viewSize.x = displayWidth[dxAdapter];
            viewSize.y = displayHeight[dxAdapter];
        }
    }
    else {
        int32 bufferWidth  = RSDK::gameSettings.fsWidth;
        int32 bufferHeight = RSDK::gameSettings.fsWidth;
        if (RSDK::gameSettings.fsWidth <= 0 || RSDK::gameSettings.fsHeight <= 0) {
            bufferWidth  = displayWidth[dxAdapter];
            bufferHeight = displayHeight[dxAdapter];
        }

        presentParams.BackBufferWidth            = bufferWidth;
        presentParams.BackBufferHeight           = bufferHeight;
        presentParams.BackBufferCount            = (RSDK::gameSettings.tripleBuffered == 1) + 1;
        presentParams.BackBufferFormat           = D3DFMT_X8R8G8B8;
        presentParams.PresentationInterval       = RSDK::gameSettings.vsync ? 1 : 0x80000000;
        presentParams.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
        presentParams.FullScreen_RefreshRateInHz = RSDK::gameSettings.refreshRate;
        presentParams.hDeviceWindow              = windowHandle;
        presentParams.Windowed                   = 0;
        viewSize.x                               = bufferWidth;
        viewSize.y                               = bufferHeight;
    }

    int32 adapterStatus = dx9Context->CreateDevice(dxAdapter, D3DDEVTYPE_HAL, windowHandle, 0x20, &presentParams, &dx9Device);
    if (RSDK::gameSettings.shaderSupport) {
        if (adapterStatus < 0)
            return false;

        D3DVERTEXELEMENT9 elements[4];

        elements[0].Type       = D3DDECLTYPE_FLOAT3;
        elements[0].Method     = 0;
        elements[0].Stream     = 0;
        elements[0].Offset     = 0;
        elements[0].Usage      = D3DDECLUSAGE_POSITION;
        elements[0].UsageIndex = 0;

        elements[1].Type       = D3DDECLTYPE_D3DCOLOR;
        elements[1].Method     = 0;
        elements[1].Stream     = 0;
        elements[1].Offset     = 12;
        elements[1].Usage      = D3DDECLUSAGE_COLOR;
        elements[1].UsageIndex = 0;

        elements[2].Type       = D3DDECLTYPE_FLOAT2;
        elements[2].Method     = 0;
        elements[2].Stream     = 0;
        elements[2].Offset     = 16;
        elements[2].Usage      = D3DDECLUSAGE_TEXCOORD;
        elements[2].UsageIndex = 0;

        elements[3].Type       = D3DDECLTYPE_UNUSED;
        elements[3].Method     = 0;
        elements[3].Stream     = 0xFF;
        elements[3].Offset     = 0;
        elements[3].Usage      = D3DDECLUSAGE_POSITION;
        elements[3].UsageIndex = 0;

        if (dx9Device->CreateVertexDeclaration(elements, &dx9VertexDeclare) < 0)
            return false;

        if (dx9Device->CreateVertexBuffer(sizeof(vertexBuffer), 0, 0, D3DPOOL_DEFAULT, &dx9VertexBuffer, NULL) < 0)
            return false;
    }
    else {
        if (adapterStatus < 0
            || dx9Device->CreateVertexBuffer(sizeof(vertexBuffer), 0, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ, D3DPOOL_DEFAULT, &dx9VertexBuffer,
                                             NULL)
                   < 0)
            return false;
    }

    int32 maxPixHeight = 0;
    for (int32 s = 0; s < SCREEN_MAX; ++s) {
        if (RSDK::gameSettings.pixHeight > maxPixHeight)
            maxPixHeight = RSDK::gameSettings.pixHeight;

        screens[s].size.y = RSDK::gameSettings.pixHeight;

        float viewAspect  = viewSize.x / viewSize.y;
        int32 screenWidth = (int)((viewAspect * RSDK::gameSettings.pixHeight) + 3) & 0xFFFFFFFC;
        if (screenWidth < RSDK::gameSettings.pixWidth)
            screenWidth = RSDK::gameSettings.pixWidth;

        // if (screenWidth > 424)
        //     screenWidth = 424;

        memset(&screens[s].frameBuffer, 0, sizeof(screens[s].frameBuffer));
        SetScreenSize(s, screenWidth, screens[s].size.y);
    }

    pixelSize.x     = screens[0].size.x;
    pixelSize.y     = screens[0].size.y;
    float pixAspect = pixelSize.x / pixelSize.y;

    dx9Device->GetViewport(&displayInfo.viewport);
    dx9ViewPort = displayInfo.viewport;

    if ((viewSize.x / viewSize.y) <= ((pixelSize.x / pixelSize.y) + 0.1)) {
        if ((pixAspect - 0.1) > (viewSize.x / viewSize.y)) {
            viewSize.y         = (pixelSize.y / pixelSize.x) * viewSize.x;
            dx9ViewPort.Y      = (displayInfo.viewport.Height >> 1) - (viewSize.y * 0.5);
            dx9ViewPort.Height = viewSize.y;

            dx9Device->SetViewport(&dx9ViewPort);
        }
    }
    else {
        viewSize.x        = pixAspect * viewSize.y;
        dx9ViewPort.X     = (displayInfo.viewport.Width >> 1) - ((pixAspect * viewSize.y) * 0.5);
        dx9ViewPort.Width = (pixAspect * viewSize.y);

        dx9Device->SetViewport(&dx9ViewPort);
    }

    if (maxPixHeight <= 256) {
        textureSize.x = 512.0;
        textureSize.y = 256.0;
    }
    else {
        textureSize.x = 1024.0;
        textureSize.y = 512.0;
    }

    for (int32 s = 0; s < SCREEN_MAX; ++s) {
        if (dx9Device->CreateTexture(textureSize.x, textureSize.y, 1, D3DUSAGE_DYNAMIC, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &screenTextures[s], NULL)
            != 0)
            return false;
    }

    if (dx9Device->CreateTexture(1024, 512, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &imageTexture, 0) != 0)
        return false;

    lastShaderID = -1;
    InitVertexBuffer();
    RSDK::gameSettings.viewportX = dx9ViewPort.X;
    RSDK::gameSettings.viewportY = dx9ViewPort.Y;
    RSDK::gameSettings.viewportW = 1.0 / viewSize.x;
    RSDK::gameSettings.viewportH = 1.0 / viewSize.y;
#endif

#if RETRO_USING_SDL2
    viewSize.x = 0;
    viewSize.y = 0;

    if (RSDK::gameSettings.windowed || !RSDK::gameSettings.exclusiveFS) {
        if (RSDK::gameSettings.windowed) {
            viewSize.x = RSDK::gameSettings.windowWidth;
            viewSize.y = RSDK::gameSettings.windowHeight;
        }
        else {
            viewSize.x = displayWidth[displayModeIndex];
            viewSize.y = displayHeight[displayModeIndex];
        }
    }
    else {
        int32 bufferWidth  = RSDK::gameSettings.fsWidth;
        int32 bufferHeight = RSDK::gameSettings.fsWidth;
        if (RSDK::gameSettings.fsWidth <= 0 || RSDK::gameSettings.fsHeight <= 0) {
            bufferWidth  = displayWidth[displayModeIndex];
            bufferHeight = displayHeight[displayModeIndex];
        }

        viewSize.x = bufferWidth;
        viewSize.y = bufferHeight;
    }

    SDL_SetWindowSize(RenderDevice::window, viewSize.x, viewSize.y);
    SDL_SetWindowPosition(RenderDevice::window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    int32 maxPixHeight = 0;
    for (int32 s = 0; s < 4; ++s) {
        if (RSDK::gameSettings.pixHeight > maxPixHeight)
            maxPixHeight = RSDK::gameSettings.pixHeight;

        screens[s].size.y = RSDK::gameSettings.pixHeight;

        float viewAspect  = viewSize.x / viewSize.y;
        int32 screenWidth = (int)((viewAspect * RSDK::gameSettings.pixHeight) + 3) & 0xFFFFFFFC;
        if (screenWidth < RSDK::gameSettings.pixWidth)
            screenWidth = RSDK::gameSettings.pixWidth;

        // if (screenWidth > 424)
        //     screenWidth = 424;

        memset(&screens[s].frameBuffer, 0, sizeof(screens[s].frameBuffer));
        SetScreenSize(s, screenWidth, screens[s].size.y);
    }

    pixelSize.x     = screens[0].size.x;
    pixelSize.y     = screens[0].size.y;
    float pixAspect = pixelSize.x / pixelSize.y;

    SDL_RenderSetLogicalSize(renderer, RSDK::gameSettings.pixWidth, SCREEN_YSIZE);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (maxPixHeight <= 256) {
        textureSize.x = 512.0;
        textureSize.y = 256.0;
    }
    else {
        textureSize.x = 1024.0;
        textureSize.y = 512.0;
    }

    for (int32 s = 0; s < SCREEN_MAX; ++s) {
        screenTexture[s] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, textureSize.x, textureSize.y);

        if (!screenTexture[s]) {
            PrintLog(PRINT_NORMAL, "ERROR: failed to create screen buffer!\nerror msg: %s", SDL_GetError());
            return 0;
        }
    }

    imageTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 1024, 512);
    if (!imageTexture)
        return false;

    lastShaderID = -1;
    InitVertexBuffer();
    RSDK::gameSettings.viewportX = 0;
    RSDK::gameSettings.viewportY = 0;
    RSDK::gameSettings.viewportW = 1.0 / viewSize.x;
    RSDK::gameSettings.viewportH = 1.0 / viewSize.y;
#endif

    return true;
}

void RenderDevice::LoadShader(const char *fileName, bool linear)
{
    char buffer[0x100];
    FileInfo info;

    for (int i = 0; i < shaderCount; ++i) {
        if (strcmp(shaderList[i].name, fileName) == 0)
            return;
    }

    if (shaderCount == SHADER_MAX)
        return;

    ShaderEntry *shader = &shaderList[shaderCount];
    shader->linear      = linear;
    sprintf(shader->name, "%s", fileName);

    const char *shaderFolder    = "Dummy"; // nothing should ever be in "Data/Shaders/Dummy" so it works out to never load anything
    const char *vertexShaderExt = "txt";
    const char *pixelShaderExt  = "txt";

    const char *bytecodeFolder    = "CSO-Dummy"; // nothing should ever be in "Data/Shaders/CSO-Dummy" so it works out to never load anything
    const char *vertexBytecodeExt = "bin";
    const char *pixelBytecodeExt  = "bin";

#if RETRO_USING_DIRECTX9
    shaderFolder    = "DX9"; // windows
    vertexShaderExt = "vs";
    pixelShaderExt  = "fs";

    bytecodeFolder    = "CSO-DX9"; // windows
    vertexBytecodeExt = "vso";
    pixelBytecodeExt  = "fso";
#endif

#if RETRO_USING_DIRECTX11
    shaderFolder    = "DX11"; // xbox one
    vertexShaderExt = "vs";
    pixelShaderExt  = "fs";

    bytecodeFolder    = "CSO-DX11"; // xbox one
    vertexBytecodeExt = "vso";
    pixelBytecodeExt  = "fso";
#endif

#if !RETRO_USE_ORIGINAL_CODE
    sprintf(buffer, "Data/Shaders/%s/%s.%s", shaderFolder, fileName, vertexShaderExt);
    InitFileInfo(&info);
    if (LoadFile(&info, buffer, FMODE_RB)) {
        byte *fileData = NULL;
        RSDK::AllocateStorage(info.fileSize, (void **)&fileData, RSDK::DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        CloseFile(&info);

#if RETRO_USING_DIRECTX9
        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
        if (engine.devMenu)
            flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL0;

        ID3DBlob *shaderBlob = nullptr;
        ID3DBlob *errorBlob  = nullptr;
        HRESULT result       = D3DCompile(fileData, info.fileSize, fileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_3_0", flags, 0,
                                    &shaderBlob, &errorBlob);

        if (FAILED(result)) {
            if (errorBlob) {
                PrintLog(PRINT_NORMAL, "ERROR COMPILING VERTEX SHADER: %s", (char *)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }

            if (shaderBlob)
                shaderBlob->Release();

            fileData = NULL;
            return;
        }
        else {
            if (dx9Device->CreateVertexShader((DWORD *)shaderBlob->GetBufferPointer(), &shader->vertexShaderObject) < 0) {
                if (shader->vertexShaderObject) {
                    shader->vertexShaderObject->Release();
                    shader->vertexShaderObject = NULL;
                }

                fileData = NULL;
                return;
            }
        }
#endif

        fileData = NULL;
    }
    else {
#endif

        sprintf(buffer, "Data/Shaders/%s/%s.%s", bytecodeFolder, fileName, vertexBytecodeExt);
        InitFileInfo(&info);
        if (LoadFile(&info, buffer, FMODE_RB)) {
            byte *fileData = NULL;
            RSDK::AllocateStorage(info.fileSize, (void **)&fileData, RSDK::DATASET_TMP, false);
            ReadBytes(&info, fileData, info.fileSize);
            CloseFile(&info);

#if RETRO_USING_DIRECTX9
            if (dx9Device->CreateVertexShader((DWORD *)fileData, &shader->vertexShaderObject) < 0) {
                if (shader->vertexShaderObject) {
                    shader->vertexShaderObject->Release();
                    shader->vertexShaderObject = NULL;
                }

                fileData = NULL;
                return;
            }
#endif

            fileData = NULL;
        }

#if !RETRO_USE_ORIGINAL_CODE
    }
#endif

#if !RETRO_USE_ORIGINAL_CODE
    sprintf(buffer, "Data/Shaders/%s/%s.%s", shaderFolder, fileName, pixelShaderExt);
    InitFileInfo(&info);
    if (LoadFile(&info, buffer, FMODE_RB)) {
        byte *fileData = NULL;
        RSDK::AllocateStorage(info.fileSize, (void **)&fileData, RSDK::DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        CloseFile(&info);

#if RETRO_USING_DIRECTX9
        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
        if (engine.devMenu)
            flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL0;

        ID3DBlob *shaderBlob = nullptr;
        ID3DBlob *errorBlob  = nullptr;
        HRESULT result       = D3DCompile(fileData, info.fileSize, fileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_3_0", flags, 0,
                                    &shaderBlob, &errorBlob);

        if (FAILED(result)) {
            if (errorBlob) {
                PrintLog(PRINT_NORMAL, "ERROR COMPILING PIXEL SHADER: %s", (char *)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }

            if (shaderBlob)
                shaderBlob->Release();
        }
        else {
            if (dx9Device->CreatePixelShader((DWORD *)shaderBlob->GetBufferPointer(), &shader->pixelShaderObject) < 0) {
                if (shader->vertexShaderObject) {
                    shader->vertexShaderObject->Release();
                    shader->vertexShaderObject = NULL;
                }

                fileData = NULL;
                return;
            }
        }
#endif

        fileData = NULL;
    }
    else {
#endif
        sprintf(buffer, "Data/Shaders/%s/%s.%s", bytecodeFolder, fileName, pixelBytecodeExt);
        InitFileInfo(&info);
        if (LoadFile(&info, buffer, FMODE_RB)) {
            byte *fileData = NULL;
            RSDK::AllocateStorage(info.fileSize, (void **)&fileData, RSDK::DATASET_TMP, false);
            ReadBytes(&info, fileData, info.fileSize);
            CloseFile(&info);

#if RETRO_USING_DIRECTX9
            if (dx9Device->CreatePixelShader((DWORD *)fileData, &shader->pixelShaderObject) < 0) {
                if (shader->pixelShaderObject) {
                    shader->pixelShaderObject->Release();
                    shader->pixelShaderObject = NULL;
                }

                fileData = NULL;
                return;
            }
#endif

            fileData = NULL;
        }

#if !RETRO_USE_ORIGINAL_CODE
    }
#endif

    shaderCount++;
}

bool RenderDevice::InitShaders()
{
#if RETRO_USING_DIRECTX9
    dx9Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    dx9Device->SetRenderState(D3DRS_LIGHTING, false);
    dx9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    dx9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    dx9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    dx9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    dx9Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    dx9Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
#endif

    int32 maxShaders = 0;
    if (RSDK::gameSettings.shaderSupport) {
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
        for (int s = 0; s < SHADER_MAX; ++s) shaderList[s].linear = true;

        shaderList[0].linear = RSDK::gameSettings.windowed ? false : shaderList[0].linear;
        maxShaders           = 1;
        shaderCount          = 1;
    }

    RSDK::gameSettings.shaderID = RSDK::gameSettings.shaderID >= maxShaders ? 0 : RSDK::gameSettings.shaderID;

#if RETRO_USING_DIRECTX9
    if (shaderList[RSDK::gameSettings.shaderID].linear || RSDK::gameSettings.screenCount > 1) {
        dx9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        dx9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    }
    else {
        dx9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        dx9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    }

    dx9Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
#endif

    return true;
}

bool RenderDevice::SetupRendering()
{
#if RETRO_USING_DIRECTX9
    dx9Context = Direct3DCreate9(D3D_SDK_VERSION);
    if (!dx9Context)
        return false;

    memset(&deviceIdentifier, 0, sizeof(deviceIdentifier));
#endif

#if RETRO_USING_SDL2
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        PrintLog(PRINT_NORMAL, "ERROR: failed to create renderer!");
        return false;
    }
#endif

    RenderDevice::GetDisplays();

    if (!InitGraphicsAPI() || !InitShaders())
        return false;

    int size  = RSDK::gameSettings.pixWidth >= SCREEN_YSIZE ? RSDK::gameSettings.pixWidth : SCREEN_YSIZE;
    scanlines = (ScanlineInfo *)malloc(size * sizeof(ScanlineInfo));
    memset(scanlines, 0, size * sizeof(ScanlineInfo));

    RSDK::gameSettings.windowState = WINDOWSTATE_ACTIVE;
    RSDK::gameSettings.dimMax      = 1.0;
    RSDK::gameSettings.dimPercent  = 1.0;

    return true;
}

void RenderDevice::GetDisplays()
{
#if RETRO_USING_DIRECTX9
    adapterCount = dx9Context->GetAdapterCount();

    UINT prevAdapter = dxAdapter;

    HMONITOR windowMonitor = MonitorFromWindow(windowHandle, 1);
    dxAdapter              = 0;
    for (int a = 0; a < adapterCount; ++a) {
        D3DDISPLAYMODE displayMode;

        HMONITOR monitor = dx9Context->GetAdapterMonitor(a);
        dx9Context->GetAdapterDisplayMode(a, &displayMode);
        displayWidth[a]  = displayMode.Width;
        displayHeight[a] = displayMode.Height;

        if (windowMonitor == monitor) {
            tagMONITORINFO lpmi;
            memset(&lpmi, 0, sizeof(lpmi));
            lpmi.cbSize = 40;

            GetMonitorInfo(windowMonitor, &lpmi);
            dxAdapter          = a;
            monitorDisplayRect = lpmi.rcMonitor;
        }
    }

    D3DADAPTER_IDENTIFIER9 adapterIdentifier;
    memset(&adapterIdentifier, 0, sizeof(adapterIdentifier));
    dx9Context->GetAdapterIdentifier(dxAdapter, 0, &adapterIdentifier);

    unsigned long *curIdentifier = &deviceIdentifier.Data1;
    unsigned long *newIdentifier = &adapterIdentifier.DeviceIdentifier.Data1;

    int remain = 3;
    while (*curIdentifier == *newIdentifier) {
        curIdentifier++;
        newIdentifier++;

        remain--;
        if (remain <= 0) {
            if (prevAdapter == dxAdapter) // no change
                return;
            else // change, reload info
                break;
        }
    }

    deviceIdentifier = adapterIdentifier.DeviceIdentifier;

    displayCount = dx9Context->GetAdapterModeCount(dxAdapter, D3DFMT_X8R8G8B8);
    if (displayInfo.displays)
        free(displayInfo.displays);

    displayInfo.displays        = (D3DDISPLAYMODE *)malloc(sizeof(D3DDISPLAYMODE) * displayCount);
    int newDisplayCount         = 0;
    bool foundFullScreenDisplay = false;

    for (int d = 0; d < displayCount; ++d) {
        dx9Context->EnumAdapterModes(dxAdapter, D3DFMT_X8R8G8B8, d, &displayInfo.displays[newDisplayCount]);

        int refreshRate = displayInfo.displays[newDisplayCount].RefreshRate;
        if (refreshRate >= 59 && (refreshRate <= 60 || refreshRate >= 120) && displayInfo.displays[newDisplayCount].Height >= (SCREEN_YSIZE * 2)) {
            if (d && refreshRate == 60 && displayInfo.displays[newDisplayCount - 1].RefreshRate == 59)
                --newDisplayCount;

            if (RSDK::gameSettings.fsWidth == displayInfo.displays[newDisplayCount].Width
                && RSDK::gameSettings.fsHeight == displayInfo.displays[newDisplayCount].Height)
                foundFullScreenDisplay = true;

            ++newDisplayCount;
        }
    }

    displayCount = newDisplayCount;
    if (!foundFullScreenDisplay) {
        RSDK::gameSettings.fsWidth     = 0;
        RSDK::gameSettings.fsHeight    = 0;
        RSDK::gameSettings.refreshRate = 60; // 0;
    }
#endif

#if RETRO_USING_SDL2
    int currentWindowDisplay = SDL_GetWindowDisplayIndex(window);

    int dispCount = SDL_GetNumVideoDisplays();

    uint32 prevDisplayMode = displayModeIndex;

    SDL_DisplayMode currentDisplay;
    SDL_GetCurrentDisplayMode(currentWindowDisplay, &currentDisplay);

    displayModeIndex = 0;
    for (int a = 0; a < dispCount; ++a) {
        SDL_DisplayMode displayMode;

        SDL_GetCurrentDisplayMode(currentWindowDisplay, &displayMode);
        displayWidth[a]  = displayMode.w;
        displayHeight[a] = displayMode.h;

        if (memcmp(&currentDisplay, &displayMode, sizeof(displayMode)) == 0) {
            displayModeIndex = a;
        }
    }

    displayCount = SDL_GetNumDisplayModes(currentWindowDisplay);
    if (displayInfo.displays)
        free(displayInfo.displays);

    displayInfo.displays        = (SDL_DisplayMode *)malloc(sizeof(SDL_DisplayMode) * displayCount);
    int newDisplayCount         = 0;
    bool foundFullScreenDisplay = false;

    for (int d = 0; d < displayCount; ++d) {
        SDL_GetDisplayMode(currentWindowDisplay, d, &displayInfo.displays[newDisplayCount]);

        int refreshRate = displayInfo.displays[newDisplayCount].refresh_rate;
        if (refreshRate >= 59 && (refreshRate <= 60 || refreshRate >= 120) && displayInfo.displays[newDisplayCount].h >= (SCREEN_YSIZE * 2)) {
            if (d && refreshRate == 60 && displayInfo.displays[newDisplayCount - 1].refresh_rate == 59)
                --newDisplayCount;

            if (RSDK::gameSettings.fsWidth == displayInfo.displays[newDisplayCount].w
                && RSDK::gameSettings.fsHeight == displayInfo.displays[newDisplayCount].h)
                foundFullScreenDisplay = true;

            ++newDisplayCount;
        }
    }

    displayCount = newDisplayCount;
    if (!foundFullScreenDisplay) {
        RSDK::gameSettings.fsWidth     = 0;
        RSDK::gameSettings.fsHeight    = 0;
        RSDK::gameSettings.refreshRate = 60; // 0;
    }
#endif
}

#if RETRO_USING_DIRECTX9
void RenderDevice::ProcessEvent(MSG Msg)
{
    bool handledMsg = false;

    switch (Msg.message) {
        case WM_QUIT: isRunning = false; break;

        // called when holding "ALT" down
        case WM_SYSKEYDOWN: {
            WPARAM activeButtons = Msg.wParam;
            switch (Msg.wParam) {
                case VK_SHIFT: activeButtons = MapVirtualKey(((Msg.lParam >> 8) & 0xFF), MAPVK_VSC_TO_VK_EX); break;

                case VK_CONTROL: activeButtons = VK_LCONTROL + ((Msg.lParam >> 8) & 1); break;

                case VK_MENU: // ALT key
                    activeButtons = VK_LMENU + ((Msg.lParam >> 8) & 1);
                    break;
            }

            switch (Msg.wParam) {
                default: UpdateKeyState(activeButtons); break;

                case VK_RETURN: // alt + enter
                    if (GetAsyncKeyState(VK_MENU)) {
                        RSDK::gameSettings.windowed ^= 1;
                        UpdateGameWindow();
                        RSDK::settingsChanged = false;
                    }
                    break;

                case VK_F4: // alt + f4
                    if (GetAsyncKeyState(VK_MENU))
                        isRunning = false;
                    break;
            }

            handledMsg = true;
            break;
        }

        // regular keydown
        case WM_KEYDOWN: {
            WPARAM activeButtons = Msg.wParam;
            switch (Msg.wParam) {
                case VK_SHIFT: activeButtons = MapVirtualKey(((Msg.lParam >> 8) & 0xFF), MAPVK_VSC_TO_VK_EX); break;

                case VK_CONTROL: activeButtons = VK_LCONTROL + ((Msg.lParam >> 8) & 1); break;

                case VK_MENU: // ALT key
                    activeButtons = VK_LMENU + ((Msg.lParam >> 8) & 1);
                    break;
            }

            // handledMsg = true;
            switch (Msg.wParam) {
                default:
                    UpdateKeyState(activeButtons);
                    handledMsg = false;
                    break;

                case VK_BACK:
                    if (engine.devMenu) {
                        engine.gameSpeed = engine.fastForwardSpeed;
                        handledMsg       = true;
                    }
                    break;

                case VK_ESCAPE:
                    if (engine.devMenu) {
                        if (sceneInfo.state == ENGINESTATE_DEVMENU) {
                            sceneInfo.state                = devMenu.stateStore;
                            RSDK::gameSettings.screenCount = sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK ? 0 : RSDK::gameSettings.screenCount;
                            ResumeSound();
                        }
                        else {
                            devMenu.stateStore             = sceneInfo.state;
                            devMenu.state                  = DevMenu_MainMenu;
                            devMenu.option                 = 0;
                            devMenu.scroll                 = 0;
                            devMenu.timer                  = 0;
                            RSDK::gameSettings.screenCount = sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK ? 1 : RSDK::gameSettings.screenCount;
                            sceneInfo.state                = ENGINESTATE_DEVMENU;
                            PauseSound();
                        }
                    }
                    else {
                        UpdateKeyState(activeButtons);
                        handledMsg = false;
                    }
                    break;

#if !RETRO_USE_ORIGINAL_CODE
                case VK_F1:
                    sceneInfo.listPos--;
                    if (sceneInfo.listPos < sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetStart) {
                        sceneInfo.activeCategory--;
                        if (sceneInfo.activeCategory >= sceneInfo.categoryCount) {
                            sceneInfo.activeCategory = sceneInfo.categoryCount - 1;
                        }
                        sceneInfo.listPos = sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd;
                    }

                    InitSceneLoad();
                    break;

                case VK_F2:
                    sceneInfo.listPos++;
                    if (sceneInfo.listPos > sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd) {
                        sceneInfo.activeCategory++;
                        if (sceneInfo.activeCategory >= sceneInfo.categoryCount) {
                            sceneInfo.activeCategory = 0;
                        }
                        sceneInfo.listPos = sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetStart;
                    }

                    InitSceneLoad();
                    break;
#endif

                case VK_F3:
                    RSDK::gameSettings.shaderID = (RSDK::gameSettings.shaderID + 1) % (shaderCount - 4);
                    handledMsg                  = true;
                    break;

#if !RETRO_USE_ORIGINAL_CODE
                case VK_F5:
                    // Quick-Reload
                    InitSceneLoad();
                    break;

                case VK_F6:
                    if (engine.devMenu && RSDK::gameSettings.screenCount > 1)
                        RSDK::gameSettings.screenCount--;
                    break;

                case VK_F7:
                    if (engine.devMenu && RSDK::gameSettings.screenCount < SCREEN_MAX)
                        RSDK::gameSettings.screenCount++;
                    break;

                case VK_F9:
                    if (engine.devMenu)
                        showHitboxes ^= 1;
                    break;

                case VK_F10:
                    if (engine.devMenu)
                        engine.showPaletteOverlay ^= 1;
                    break;
#endif

                case VK_INSERT:
                case VK_F11:
                    if (engine.devMenu) {
                        engine.frameStep = true;
                        handledMsg       = true;
                    }
                    break;

                case VK_PAUSE:
                case VK_F12:
                    if (engine.devMenu) {
                        if (sceneInfo.state != ENGINESTATE_NULL)
                            sceneInfo.state ^= ENGINESTATE_STEPOVER;
                    }
                    break;
            }
            break;
        }

        case WM_KEYUP:
        case WM_SYSKEYUP: {
            WPARAM activeButtons = Msg.wParam;
            switch (Msg.wParam) {
                case VK_SHIFT: activeButtons = MapVirtualKey(((Msg.lParam >> 8) & 0xFF), MAPVK_VSC_TO_VK_EX); break;

                case VK_CONTROL: activeButtons = VK_LCONTROL + ((Msg.lParam >> 8) & 1); break;

                case VK_MENU: // ALT key
                    activeButtons = VK_LMENU + ((Msg.lParam >> 8) & 1);
                    break;
            }

            switch (Msg.wParam) {
                default: ClearKeyState(activeButtons); break;

                case VK_BACK:
                    engine.gameSpeed = 1;
                    handledMsg       = true;
                    break;
            }

            break;
        }

        case WM_LBUTTONDOWN:
            touchMouseData.down[0] = 1;
            touchMouseData.count   = 1;
            handledMsg             = true;
            break;

        case WM_LBUTTONUP:
            touchMouseData.down[0] = 0;
            touchMouseData.count   = 0;
            handledMsg             = true;
            break;

        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN: handledMsg = true; break;
    }

    if (!handledMsg)
        DispatchMessage(&Msg);
}
#endif

#if RETRO_USING_SDL2
void RenderDevice::ProcessEvent(SDL_Event event)
{
    switch (event.type) {
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_MAXIMIZED: {
                    SDL_RestoreWindow(window);
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    SDL_ShowCursor(SDL_FALSE);
                    RSDK::gameSettings.windowed = false;
                    break;
                }

                case SDL_WINDOWEVENT_CLOSE: isRunning = false; break;

                case SDL_WINDOWEVENT_FOCUS_GAINED:
#if RETRO_REV02
                    RSDK::SKU::userCore->focusState = 0;
#endif
                    break;

                case SDL_WINDOWEVENT_FOCUS_LOST:
#if RETRO_REV02
                    RSDK::SKU::userCore->focusState = 1;
#endif
                    break;
            }
            break;

        case SDL_CONTROLLERDEVICEADDED: {
            uint32 id;
            char buffer[0x20];
            sprintf(buffer, "%s%d", "SDLDevice", event.cdevice.which);
            GenerateCRC(&id, buffer);

            InitSDL2InputDevice(id, event.cdevice.which);
            break;
        }

        case SDL_CONTROLLERDEVICEREMOVED: {
            uint32 id;
            char buffer[0x20];
            sprintf(buffer, "%s%d", "SDLDevice", event.cdevice.which);
            GenerateCRC(&id, buffer);

            RemoveInputDevice(InputDeviceFromID(id));
            break;
        }

        case SDL_APP_WILLENTERFOREGROUND:
#if RETRO_REV02
            RSDK::SKU::userCore->focusState = 0;
#endif
            break;

        case SDL_APP_WILLENTERBACKGROUND:
#if RETRO_REV02
            RSDK::SKU::userCore->focusState = 1;
#endif
            break;

        case SDL_APP_TERMINATING: isRunning = false; break;

        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: touchMouseData.down[0] = true; touchMouseData.count = 1;
#if !RETRO_REV02
                    if (buttonDownCount > 0)
                        buttonDownCount--;
#endif
                    break;

                case SDL_BUTTON_RIGHT:
#if !RETRO_REV02
                    specialKeyStates[3] = true;
                    buttonDownCount++;
#endif
                    break;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: touchMouseData.down[0] = false; touchMouseData.count = 0;
#if !RETRO_REV02
                    if (buttonDownCount > 0)
                        buttonDownCount--;
#endif
                    break;

                case SDL_BUTTON_RIGHT:
#if !RETRO_REV02
                    specialKeyStates[3] = false;
                    buttonDownCount--;
#endif
                    break;
            }
            break;

        case SDL_FINGERMOTION:
        case SDL_FINGERDOWN:
        case SDL_FINGERUP: {
            int32 count          = SDL_GetNumTouchFingers(event.tfinger.touchId);
            touchMouseData.count = 0;
            for (int32 i = 0; i < count; i++) {
                SDL_Finger *finger = SDL_GetTouchFinger(event.tfinger.touchId, i);
                if (finger) {
                    touchMouseData.down[touchMouseData.count] = true;
                    touchMouseData.x[touchMouseData.count]    = finger->x;
                    touchMouseData.y[touchMouseData.count]    = finger->y;
                    touchMouseData.count++;
                }
            }
            break;
        }

        case SDL_KEYDOWN:
#if !RETRO_REV02
            ++buttonDownCount;
#endif
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_RETURN:
                    if (event.key.keysym.mod == KMOD_LALT) {
                        RSDK::gameSettings.windowed ^= 1;
                        UpdateGameWindow();
                        RSDK::settingsChanged = false;
                        break;
                    }
                // [fallthrough]

                default: UpdateKeyState(event.key.keysym.scancode); break;

                case SDL_SCANCODE_ESCAPE:
                    if (engine.devMenu) {
                        if (sceneInfo.state == ENGINESTATE_DEVMENU) {
                            sceneInfo.state = devMenu.stateStore;
                            if (devMenu.stateStore == ENGINESTATE_VIDEOPLAYBACK)
                                RSDK::gameSettings.screenCount = 0;

                            ResumeSound();
                        }
                        else {
                            devMenu.stateStore = sceneInfo.state;
                            if (sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK)
                                RSDK::gameSettings.screenCount = 1;

                            devMenu.state   = DevMenu_MainMenu;
                            devMenu.option  = 0;
                            devMenu.scroll  = 0;
                            devMenu.timer   = 0;
                            sceneInfo.state = ENGINESTATE_DEVMENU;
                            PauseSound();
                        }
                    }
                    else {
                        UpdateKeyState(event.key.keysym.scancode);
                    }
#if !RETRO_REV02
                    specialKeyStates[0] = true;
#endif
                    break;

#if !RETRO_REV02
                case SDL_SCANCODE_RETURN: specialKeyStates[1] = true; break;
#endif

#if !RETRO_USE_ORIGINAL_CODE
                case SDL_SCANCODE_F1:
                    sceneInfo.listPos--;
                    if (sceneInfo.listPos < sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetStart) {
                        sceneInfo.activeCategory--;
                        if (sceneInfo.activeCategory >= sceneInfo.categoryCount) {
                            sceneInfo.activeCategory = sceneInfo.categoryCount - 1;
                        }
                        sceneInfo.listPos = sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd;
                    }

                    InitSceneLoad();
                    break;

                case SDL_SCANCODE_F2:
                    sceneInfo.listPos++;
                    if (sceneInfo.listPos > sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd) {
                        sceneInfo.activeCategory++;
                        if (sceneInfo.activeCategory >= sceneInfo.categoryCount) {
                            sceneInfo.activeCategory = 0;
                        }
                        sceneInfo.listPos = sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetStart;
                    }

                    InitSceneLoad();
                    break;
#endif

                case SDL_SCANCODE_F3: RSDK::gameSettings.shaderID = (RSDK::gameSettings.shaderID + 1) % (shaderCount - 4); break;

#if !RETRO_USE_ORIGINAL_CODE
                case SDL_SCANCODE_F5:
                    // Quick-Reload
                    InitSceneLoad();
                    break;

                case SDL_SCANCODE_F6:
                    if (engine.devMenu && RSDK::gameSettings.screenCount > 1)
                        RSDK::gameSettings.screenCount--;
                    break;

                case SDL_SCANCODE_F7:
                    if (engine.devMenu && RSDK::gameSettings.screenCount < SCREEN_MAX)
                        RSDK::gameSettings.screenCount++;
                    break;

                case SDL_SCANCODE_F9:
                    if (engine.devMenu)
                        showHitboxes ^= 1;
                    break;

                case SDL_SCANCODE_F10:
                    if (engine.devMenu)
                        engine.showPaletteOverlay ^= 1;
                    break;
#endif
                case SDL_SCANCODE_BACKSPACE:
                    if (engine.devMenu)
                        engine.gameSpeed = engine.fastForwardSpeed;
                    break;

                case SDL_SCANCODE_F11:
                case SDL_SCANCODE_INSERT:
                    if ((sceneInfo.state & ENGINESTATE_STEPOVER) == ENGINESTATE_STEPOVER)
                        engine.frameStep = true;
                    break;

                case SDL_SCANCODE_F12:
                case SDL_SCANCODE_PAUSE:
                    if (engine.devMenu) {
                        sceneInfo.state ^= ENGINESTATE_STEPOVER;
                    }
                    break;
            }
            break;

        case SDL_KEYUP:
#if !RETRO_REV02
            --buttonDownCount;
#endif
            switch (event.key.keysym.scancode) {
                default: ClearKeyState(event.key.keysym.scancode); break;
#if !RETRO_REV02
                case SDL_SCANCODE_ESCAPE: specialKeyStates[0] = false; break;
                case SDL_SCANCODE_RETURN: specialKeyStates[1] = false; break;
#endif
                case SDL_SCANCODE_BACKSPACE: engine.gameSpeed = 1; break;
            }
            break;

        case SDL_QUIT: isRunning = false; break;
    }
}
#endif

bool RenderDevice::ProcessEvents()
{
#if RETRO_USING_DIRECTX9
    MSG Msg;
    while (PeekMessage(&Msg, NULL, 0, 0, true)) {
        RenderDevice::ProcessEvent(Msg);

        if (!isRunning)
            return false;
    }
#endif

#if RETRO_USING_SDL1 || RETRO_USING_SDL2
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent)) {
        RenderDevice::ProcessEvent(sdlEvent);

        if (!isRunning)
            return false;
    }
#endif

    return true;
}

#if RETRO_USING_DIRECTX9
LRESULT CALLBACK RenderDevice::WindowEventCallback(HWND hRecipient, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg) {
        case WM_CREATE: {
            if (deviceNotif)
                return 0;

            int filter  = 32;
            deviceNotif = RegisterDeviceNotification(hRecipient, &filter, 0);
            break;
        }

        case WM_DESTROY:
            if (deviceNotif) {
                UnregisterDeviceNotification(deviceNotif);
                deviceNotif = 0;
            }

            isRunning = false;
            break;

        case WM_MOVE:
        case WM_SIZE: break;

        case WM_ACTIVATE:
            if (wParam) {
                if (!RSDK::gameSettings.windowState)
                    return 0;

                // if (byte_66BB18 == 1)
                // {
                //      byte_97BB18 = 0;
                //      audioContext_sourceVoice->Start(audioContext_sourceVoice, 0, 0);
                // }

                GetDisplays();
                RSDK::gameSettings.windowState = WINDOWSTATE_ACTIVE;
            }
            else {
                touchMouseData.down[0] = 0;
                touchMouseData.count   = 0;
                if (!RSDK::gameSettings.windowState)
                    return 0;

                // if (!byte_66BB18)
                // {
                //      byte_97BB18 = 1;
                //      audioContext_sourceVoice->Stop(audioContext_sourceVoice, 0, 0);
                // }

                RSDK::gameSettings.windowState = WINDOWSTATE_INACTIVE;
            }
            break;

        case WM_PAINT:
            BeginPaint(hRecipient, &Paint);
            EndPaint(hRecipient, &Paint);
            break;

        case WM_DEVICECHANGE: {
            unsigned int dbch_sizes[] = { 1771351300u, 298882031u, 2684406947u, 2519802569u };
            DEV_BROADCAST_HDR *param  = (DEV_BROADCAST_HDR *)lParam;

            if ((wParam == DBT_DEVICEARRIVAL || wParam == DBT_DEVICEREMOVECOMPLETE) && param && param->dbch_devicetype == 5) {
                DEV_BROADCAST_HDR *device = param + 1;
                int remain                = 3;
                unsigned int *size        = dbch_sizes;
                while (device->dbch_size == *size) {
                    ++device;
                    ++size;
                    remain--;
                    if (remain <= 0) {
                        // audioEnabled = 30;
                        break;
                    }
                }
            }

            UpdateXInputDevices();
            InitHIDAPI();
            InitXInputAPI();
            break;
        }

        case WM_INPUT: UpdateRawInputButtonState((HRAWINPUT)lParam); break;

        case WM_SYSCOMMAND: {
            int32 param = wParam & 0xFFF0;
            if (param == SC_MINIMIZE) {
                touchMouseData.down[0] = 0;
                touchMouseData.count   = 0;
                if (RSDK::gameSettings.windowState) {
                    PauseSound();
                    RSDK::gameSettings.windowState = WINDOWSTATE_INACTIVE;
                }
            }
            else if (param == SC_MAXIMIZE && RSDK::gameSettings.windowState != WINDOWSTATE_UNINITIALIZED) {
                ResumeSound();
                RSDK::gameSettings.windowState = WINDOWSTATE_ACTIVE;
            }

            return DefWindowProc(hRecipient, WM_SYSCOMMAND, wParam, lParam);
        }

        case WM_MENUSELECT:
        case WM_ENTERSIZEMOVE:
            touchMouseData.down[0] = 0;
            touchMouseData.count   = 0;
            break;

        case WM_EXITSIZEMOVE: GetDisplays(); break;

        default: return DefWindowProc(hRecipient, Msg, wParam, lParam);
    }

    return 0;
}
#endif

void UpdateGameWindow()
{
    RenderDevice::RefreshWindow();
}

void SetImageTexture(int width, int height, byte *imagePixels)
{
#if RETRO_USING_SDL2
    // if (RenderDevice::imageTexture)
    //     SDL_DestroyTexture(RenderDevice::imageTexture);
    //
    // int32 format        = imagePixels ? SDL_PIXELFORMAT_ARGB8888 : SDL_PIXELFORMAT_YV12;
    // RenderDevice::imageTexture = SDL_CreateTexture(RenderDevice::renderer, format, SDL_TEXTUREACCESS_STREAMING, width, height);
    //
    // if (imagePixels) {
    //     int pitch = 0;
    //     byte *pixels;
    //     SDL_LockTexture(RenderDevice::imageTexture, NULL, (void **)&pixels, &pitch);
    //     memcpy(pixels, imagePixels, pitch * height);
    //     SDL_UnlockTexture(RenderDevice::imageTexture);
    // }
#endif
}

void GenerateBlendLookupTable()
{
    for (int y = 0; y < 0x100; y++) {
        for (int x = 0; x < 0x20; x++) {
            blendLookupTable[x + (y * 0x20)]    = y * x >> 8;
            subtractLookupTable[x + (y * 0x20)] = y * (0x1F - x) >> 8;
        }
    }

#if !RETRO_REV02
    for (int i = 0; i < 0x10000; ++i) {
        int32 tintValue    = (((uint32)i & 0x1F) + ((i >> 6) & 0x1F) + (((uint16)i >> 11) & 0x1F)) / 3 + 6;
        tintLookupTable[i] = 0x841 * minVal(0x1F, tintValue);
    }
#endif

    for (int c = 0; c < 0x100; ++c) {
        rgb32To16_R[c] = (c & 0xFFF8) << 8;
        rgb32To16_G[c] = (c & 0xFFFC) << 3;
        rgb32To16_B[c] = c >> 3;
    }
}

void InitSystemSurfaces()
{
    GEN_HASH("TileBuffer", gfxSurface[0].hash);
    gfxSurface[0].scope    = SCOPE_GLOBAL;
    gfxSurface[0].width    = TILE_SIZE;
    gfxSurface[0].height   = 0x40000;
    gfxSurface[0].lineSize = 4; // 16
    gfxSurface[0].dataPtr  = tilesetGFXData;

#if RETRO_REV02
    GEN_HASH("EngineText", gfxSurface[1].hash);
    gfxSurface[1].scope    = SCOPE_GLOBAL;
    gfxSurface[1].width    = 8;
    gfxSurface[1].height   = 128 * 8;
    gfxSurface[1].lineSize = 3; // 8
    gfxSurface[1].dataPtr  = engineTextBuffer;
#endif
}

void GetDisplayInfo(int *displayID, int *width, int *height, int *refreshRate, char *text)
{
    if (!displayID)
        return;

    uint id    = *displayID;
    int dispID = 0;

    if (*displayID == -2) {
        if (RSDK::gameSettings.fsWidth && RSDK::gameSettings.fsHeight) {
            int d = 0;
#if RETRO_USING_DIRECTX9
            for (; d < RenderDevice::displayCount; ++d) {
                if (RenderDevice::displayInfo.displays[d].Width == RSDK::gameSettings.fsWidth
                    && RenderDevice::displayInfo.displays[d].Height == RSDK::gameSettings.fsHeight
                    && RenderDevice::displayInfo.displays[d].RefreshRate == RSDK::gameSettings.refreshRate) {
                    break;
                }
            }
#endif

#if RETRO_USING_SDL2
            for (; d < RenderDevice::displayCount; ++d) {
                if (RenderDevice::displayInfo.displays[d].w == RSDK::gameSettings.fsWidth
                    && RenderDevice::displayInfo.displays[d].h == RSDK::gameSettings.fsHeight
                    && RenderDevice::displayInfo.displays[d].refresh_rate == RSDK::gameSettings.refreshRate) {
                    break;
                }
            }
#endif
            dispID = d + 1;
        }
        else {
            dispID = 0;
        }
    }
    else {
        dispID = RenderDevice::displayCount - 1;
        if (id < RenderDevice::displayCount) {
            if (id == RenderDevice::displayCount)
                dispID = 0;
            else
                dispID = *displayID;
        }
    }

    *displayID = dispID;
    if (dispID) {
        int d = dispID - 1;

#if RETRO_USING_DIRECTX9
        if (width)
            *width = RenderDevice::displayInfo.displays[d].Width;

        if (height)
            *height = RenderDevice::displayInfo.displays[d].Height;

        if (refreshRate)
            *refreshRate = RenderDevice::displayInfo.displays[d].RefreshRate;

        if (text) {
            sprintf(text, "%ix%i @%iHz", RenderDevice::displayInfo.displays[d].Width, RenderDevice::displayInfo.displays[d].Height,
                    RenderDevice::displayInfo.displays[d].RefreshRate);
        }
#endif

#if RETRO_USING_SDL2
        if (width)
            *width = RenderDevice::displayInfo.displays[d].w;

        if (height)
            *height = RenderDevice::displayInfo.displays[d].h;

        if (refreshRate)
            *refreshRate = RenderDevice::displayInfo.displays[d].refresh_rate;

        if (text) {
            sprintf(text, "%ix%i @%iHz", RenderDevice::displayInfo.displays[d].w, RenderDevice::displayInfo.displays[d].h,
                    RenderDevice::displayInfo.displays[d].refresh_rate);
        }
#endif
    }
    else {
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

void GetWindowSize(int *width, int *height)
{
#if RETRO_USING_DIRECTX9
    D3DDISPLAYMODE display;
    RenderDevice::dx9Context->GetAdapterDisplayMode(RenderDevice::dxAdapter, &display);

    if (width)
        *width = display.Width;

    if (height)
        *height = display.Height;
#endif

#if RETRO_USING_SDL2
    if (!RSDK::gameSettings.windowed) {
        SDL_GetRendererOutputSize(RenderDevice::renderer, width, height);
    }
    else {
        int currentWindowDisplay = SDL_GetWindowDisplayIndex(RenderDevice::window);

        SDL_DisplayMode display;
        SDL_GetCurrentDisplayMode(currentWindowDisplay, &display);

        if (width)
            *width = display.w;

        if (height)
            *height = display.h;
    }
#endif
}

void SwapDrawListEntries(uint8 layer, uint16 entitySlotA, uint16 entitySlotB, int32 count)
{
    if (layer < DRAWLAYER_COUNT) {
        DrawList *list = &drawLayers[layer];
        if (count < 0 || count > list->entityCount)
            count = list->entityCount;

        if (count) {
            int32 slotA = -1;
            int32 slotB = -1;
            if (count > 0) {
                for (int i = 0; i < count; ++i) {
                    if (list->entries[i] == entitySlotA)
                        slotA = i;
                    if (list->entries[i] == entitySlotB)
                        slotB = i;
                }

                if (slotA > -1 && slotB > -1 && slotA < slotB) {
                    int temp             = list->entries[slotB];
                    list->entries[slotB] = list->entries[slotA];
                    list->entries[slotA] = temp;
                }
            }
        }
    }
}

void FillScreen(uint color, int alphaR, int alphaG, int alphaB)
{
    alphaR = clampVal(alphaR, 0x00, 0xFF);
    alphaG = clampVal(alphaG, 0x00, 0xFF);
    alphaB = clampVal(alphaB, 0x00, 0xFF);

    if (alphaR + alphaG + alphaB) {
        validDraw        = true;
        ushort clrBlendR = blendLookupTable[0x20 * alphaR + rgb32To16_B[(color >> 0x10) & 0xFF]];
        ushort clrBlendG = blendLookupTable[0x20 * alphaG + rgb32To16_B[(color >> 0x08) & 0xFF]];
        ushort clrBlendB = blendLookupTable[0x20 * alphaB + rgb32To16_B[(color >> 0x00) & 0xFF]];

        ushort *fbBlendR = &blendLookupTable[0x20 * (0xFF - alphaR)];
        ushort *fbBlendG = &blendLookupTable[0x20 * (0xFF - alphaG)];
        ushort *fbBlendB = &blendLookupTable[0x20 * (0xFF - alphaB)];

        int32 cnt = currentScreen->size.y * currentScreen->pitch;
        for (int id = 0; cnt > 0; --cnt, ++id) {
            ushort px = currentScreen->frameBuffer[id];

            int32 R = fbBlendR[(px & 0xF800) >> 11] + clrBlendR;
            int32 G = fbBlendG[(px & 0x7E0) >> 6] + clrBlendG;
            int32 B = fbBlendB[px & 0x1F] + clrBlendB;

            currentScreen->frameBuffer[id] = (B) | (G << 6) | (R << 11);
        }
    }
}

void DrawLine(int x1, int y1, int x2, int y2, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative)
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

    int drawY1 = y1;
    int drawX1 = x1;
    int drawY2 = y2;
    int drawX2 = x2;

    if (!screenRelative) {
        drawX1 = (x1 >> 16) - currentScreen->position.x;
        drawY1 = (y1 >> 16) - currentScreen->position.y;
        drawX2 = (x2 >> 16) - currentScreen->position.x;
        drawY2 = (y2 >> 16) - currentScreen->position.y;
    }

    int flags1 = 0;
    if (drawX1 >= currentScreen->clipBound_X2)
        flags1 = 2;
    else if (drawX1 < currentScreen->clipBound_X1)
        flags1 = 1;

    if (drawY1 >= currentScreen->clipBound_Y2)
        flags1 |= 8;
    else if (drawY1 < currentScreen->clipBound_Y1)
        flags1 |= 4;

    int flags2 = 0;
    if (drawX2 >= currentScreen->clipBound_X2)
        flags2 = 2;
    else if (drawX2 < currentScreen->clipBound_X1)
        flags2 = 1;

    if (drawY2 >= currentScreen->clipBound_Y2)
        flags2 |= 8;
    else if (drawY2 < currentScreen->clipBound_Y1)
        flags2 |= 4;

    int id = 0;
    while (flags1 || flags2) {
        if (flags1 & flags2)
            return;
        ++id;

        int curFlags = flags2;
        if (flags1)
            curFlags = flags1;

        int x = 0;
        int y = 0;
        if (curFlags & 8) {
            int div = (drawY2 - drawY1);
            if (!div)
                div = 1;
            x = drawX1 + ((drawX2 - drawX1) * (((currentScreen->clipBound_Y2 - drawY1) << 8) / div) >> 8);
            y = currentScreen->clipBound_Y2;
        }
        else if (curFlags & 4) {
            int div = (drawY2 - drawY1);
            if (!div)
                div = 1;
            x = drawX1 + ((drawX2 - drawX1) * (((currentScreen->clipBound_Y1 - drawY1) << 8) / div) >> 8);
            y = currentScreen->clipBound_Y1;
        }
        else if (curFlags & 2) {
            int div = (drawX2 - drawX1);
            if (!div)
                div = 1;
            x = currentScreen->clipBound_X2;
            y = drawY1 + ((drawY2 - drawY1) * (((currentScreen->clipBound_X2 - drawX1) << 8) / div) >> 8);
        }
        else if (curFlags & 1) {
            int div = (drawX2 - drawX1);
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

    int sizeX = abs(drawX2 - drawX1);
    int sizeY = abs(drawY2 - drawY1);
    int max   = sizeY;
    int hSize = sizeX >> 2;
    if (sizeX <= sizeY)
        hSize = -sizeY >> 2;

    if (drawX2 < drawX1) {
        int v  = drawX1;
        drawX1 = drawX2;
        drawX2 = v;

        v      = drawY1;
        drawY1 = drawY2;
        drawY2 = v;
    }

    ushort color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];
    ushort *frameBufferPtr = &currentScreen->frameBuffer[drawX1 + drawY1 * currentScreen->pitch];

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
                ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
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
                ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
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
                ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
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
                ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
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
void DrawRectangle(int x, int y, int width, int height, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative)
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

    int pitch              = currentScreen->pitch - width;
    validDraw              = true;
    ushort *frameBufferPtr = &currentScreen->frameBuffer[x + (y * currentScreen->pitch)];
    ushort color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];

    switch (inkEffect) {
        case INK_NONE: {
            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    *frameBufferPtr = color16;
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_BLEND: {
            int h = height;
            while (h--) {
                int w = width;
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

            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    setPixelAlpha(color16, *frameBufferPtr, alpha);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_ADD: {
            ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
            int h                 = height;
            while (h--) {
                int w = width;
                while (w--) {
                    setPixelAdditive(color16, *frameBufferPtr);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_SUB: {
            ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
            int h                 = height;
            while (h--) {
                int w = width;
                while (w--) {
                    setPixelSubtractive(color16, *frameBufferPtr);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_TINT: {
            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_MASKED: {
            int h = height;
            while (h--) {
                int w = width;
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
            int h = height;
            while (h--) {
                int w = width;
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
void DrawCircle(int x, int y, int radius, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative)
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

        int yRadiusBottom = y + radius;
        int bottom        = currentScreen->clipBound_Y1;
        int yRadiusTop    = y - radius;
        int top = top = y - radius;
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
            for (int i = top; i < bottom; ++i) {
                scanEdgeBuffer[i].start = 0x7FFF;
                scanEdgeBuffer[i].end   = -1;
            }

            int r                    = 3 - 2 * radius;
            int xRad                 = x - radius;
            int curY                 = y;
            int curX                 = x;
            int startY               = yRadiusTop + 1;
            ScanEdge *scanEdgeTop    = &scanEdgeBuffer[yRadiusTop];
            ScanEdge *scanEdgeBottom = &scanEdgeBuffer[yRadiusBottom];
            ScanEdge *scanEdge       = &scanEdgeBuffer[y];
            int dif                  = x - y;

            for (int i = 0; i <= radius; ++i) {
                int scanX = i + curX;
                if (yRadiusBottom >= top && yRadiusBottom <= bottom && scanX > scanEdgeBottom->end)
                    scanEdgeBottom->end = scanX;
                if (startY >= top && startY <= bottom && scanX > scanEdgeTop->end)
                    scanEdgeTop->end = scanX;

                int scanY = i + y;
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
            ushort *frameBufferPtr = &currentScreen->frameBuffer[top * currentScreen->pitch];
            ushort color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];

            switch (inkEffect) {
                default: break;
                case INK_NONE:
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
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
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
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
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
                                setPixelAlpha(color16, frameBufferPtr[edge->start + x], alpha);
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;

                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
                                setPixelAdditive(color16, frameBufferPtr[edge->start + x]);
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;
                }

                case INK_SUB: {
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
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
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
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
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
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
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
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
void DrawCircleOutline(int x, int y, int innerRadius, int outerRadius, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative)
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
        int top    = y - outerRadius;
        int left   = x - outerRadius;
        int right  = x + outerRadius;
        int bottom = y + outerRadius;

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
            int ir2                = innerRadius * innerRadius;
            int or2                = outerRadius * outerRadius;
            validDraw              = true;
            ushort *frameBufferPtr = &currentScreen->frameBuffer[left + top * currentScreen->pitch];
            ushort color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];
            int pitch              = (left + currentScreen->pitch - right);

            switch (inkEffect) {
                default: break;
                case INK_NONE:
                    if (top < bottom) {
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
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
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
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

                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
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
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    if (top < bottom) {
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
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
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    if (top < bottom) {
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
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
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
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
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
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
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
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

void DrawFace(Vector2 *vertices, int vertCount, int r, int g, int b, int alpha, InkEffects inkEffect)
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

    int top    = 0x7FFFFFFF;
    int bottom = -0x10000;
    for (int v = 0; v < vertCount; ++v) {
        if (vertices[v].y < top)
            top = vertices[v].y;
        if (vertices[v].y > bottom)
            bottom = vertices[v].y;
    }

    int topScreen    = top >> 16;
    int bottomScreen = bottom >> 16;

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
        for (int s = topScreen; s <= bottomScreen; ++s) {
            edge->start = 0x7FFF;
            edge->end   = -1;
            ++edge;
        }

        for (int v = 0; v < vertCount - 1; ++v) {
            ProcessScanEdge(vertices[v + 0].x, vertices[v + 0].y, vertices[v + 1].x, vertices[v + 1].y);
        }
        ProcessScanEdge(vertices[0].x, vertices[0].y, vertices[vertCount - 1].x, vertices[vertCount - 1].y);

        ushort *frameBufferPtr = &currentScreen->frameBuffer[topScreen * currentScreen->pitch];
        ushort color16         = rgb32To16_B[b] | rgb32To16_G[g] | rgb32To16_R[r];

        edge = &scanEdgeBuffer[topScreen];
        switch (inkEffect) {
            default: break;

            case INK_NONE:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        frameBufferPtr[edge->start + x] = color16;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_BLEND:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        setPixelBlend(color16, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_ALPHA: {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        setPixelAlpha(color16, frameBufferPtr[edge->start + x], alpha);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_ADD: {
                ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        setPixelAdditive(color16, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_SUB: {
                ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        setPixelSubtractive(color16, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_TINT:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        frameBufferPtr[edge->start + x] = tintLookupTable[frameBufferPtr[edge->start + x]];
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_MASKED:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        if (frameBufferPtr[edge->start + x] == maskColor)
                            frameBufferPtr[edge->start + x] = color16;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_UNMASKED:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
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
void DrawBlendedFace(Vector2 *vertices, uint *colors, int vertCount, int alpha, InkEffects inkEffect)
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

    int top    = 0x7FFFFFFF;
    int bottom = -0x10000;
    for (int v = 0; v < vertCount; ++v) {
        if (vertices[v].y < top)
            top = vertices[v].y;
        if (vertices[v].y > bottom)
            bottom = vertices[v].y;
    }

    int topScreen    = top >> 16;
    int bottomScreen = bottom >> 16;

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
        for (int s = topScreen; s <= bottomScreen; ++s) {
            edge->start = 0x7FFF;
            edge->end   = -1;
            ++edge;
        }

        for (int v = 0; v < vertCount - 1; ++v) {
            ProcessScanEdgeClr(colors[v + 0], colors[v + 1], vertices[v + 0].x, vertices[v + 0].y, vertices[v + 1].x, vertices[v + 1].y);
        }
        ProcessScanEdgeClr(colors[vertCount - 1], colors[0], vertices[vertCount - 1].x, vertices[vertCount - 1].y, vertices[0].x, vertices[0].y);

        ushort *frameBufferPtr = &currentScreen->frameBuffer[topScreen * currentScreen->pitch];

        edge = &scanEdgeBuffer[topScreen];
        switch (inkEffect) {
            default: break;
            case INK_NONE:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

                    if (edge->start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }
                    else if (edge->start < currentScreen->clipBound_X1) {
                        int dif = (currentScreen->clipBound_X1 - edge->start);
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

                    for (int x = 0; x < count; ++x) {
                        ushort color16                  = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
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
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

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

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelBlend(color, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_ALPHA: {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

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

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelAlpha(color, frameBufferPtr[edge->start + x], alpha);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_ADD: {
                ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

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

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelAdditive(color, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_SUB: {
                ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

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

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelSubtractive(color, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_TINT:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

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

                    for (int x = 0; x < count; ++x) {
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
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

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

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        if (frameBufferPtr[edge->start + x] == maskColor)
                            frameBufferPtr[edge->start + x] = color;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_UNMASKED:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

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

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
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

void DrawSprite(RSDK::Animator *animator, Vector2 *position, bool32 screenRelative)
{
    if (animator && animator->frames) {
        RSDK::SpriteFrame *frame = &animator->frames[animator->frameID];
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

        int rotation = sceneInfo.entity->rotation;
        int drawFX   = sceneInfo.entity->drawFX;
        if (sceneInfo.entity->drawFX & FX_ROTATE) {
            switch (animator->rotationFlag) {
                case RSDK::ROTFLAG_NONE:
                    rotation = 0;
                    if ((sceneInfo.entity->drawFX & FX_ROTATE) != FX_NONE)
                        drawFX ^= FX_ROTATE;
                    break;

                case RSDK::ROTFLAG_FULL:
                    rotation = sceneInfo.entity->rotation & 0x1FF;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case RSDK::ROTFLAG_45DEG: // 0x00, 0x40, 0x80, 0xC0, 0x100, 0x140, 0x180, 0x1C0
                    rotation = (sceneInfo.entity->rotation + 0x20) & 0x1C0;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case RSDK::ROTFLAG_90DEG: // 0x00, 0x80, 0x100, 0x180
                    rotation = (sceneInfo.entity->rotation + 0x40) & 0x180;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case RSDK::ROTFLAG_180DEG: // 0x00, 0x100
                    rotation = (sceneInfo.entity->rotation + 0x80) & 0x100;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case RSDK::ROTFLAG_STATICFRAMES:
                    if (sceneInfo.entity->rotation >= 0x100) {
                        rotation = 0x08 - ((0x214 - sceneInfo.entity->rotation) >> 6);
                    }
                    else {
                        rotation = (sceneInfo.entity->rotation + 20) >> 6;
                    }

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
                                  (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_FLIP:
                switch (sceneInfo.entity->direction) {
                    case FLIP_NONE:
                        DrawSpriteFlipped(pos.x + frame->pivotX, pos.y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                          FLIP_NONE, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;

                    case FLIP_X:
                        DrawSpriteFlipped(pos.x - frame->width - frame->pivotX, pos.y + frame->pivotY, frame->width, frame->height, frame->sprX,
                                          frame->sprY, FLIP_X, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;

                    case FLIP_Y:
                        DrawSpriteFlipped(pos.x + frame->pivotX, pos.y - frame->height - frame->pivotY, frame->width, frame->height, frame->sprX,
                                          frame->sprY, FLIP_Y, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;

                    case FLIP_XY:
                        DrawSpriteFlipped(pos.x - frame->width - frame->pivotX, pos.y - frame->height - frame->pivotY, frame->width, frame->height,
                                          frame->sprX, frame->sprY, FLIP_XY, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                                          frame->sheetID);
                        break;

                    default: break;
                }
                break;
            case FX_ROTATE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, 0x200, 0x200,
                                   FLIP_NONE, rotation, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_ROTATE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, 0x200, 0x200,
                                   FlipFlags(sceneInfo.entity->direction & FLIP_X), rotation, (InkEffects)sceneInfo.entity->inkEffect,
                                   sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_SCALE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE, 0, (InkEffects)sceneInfo.entity->inkEffect,
                                   sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_SCALE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FlipFlags(sceneInfo.entity->direction & FLIP_X), 0,
                                   (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_SCALE | FX_ROTATE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE, rotation, (InkEffects)sceneInfo.entity->inkEffect,
                                   sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_SCALE | FX_ROTATE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FlipFlags(sceneInfo.entity->direction & FLIP_X), rotation,
                                   (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            default: break;
        }
    }
}
void DrawSpriteFlipped(int x, int y, int width, int height, int sprX, int sprY, FlipFlags direction, InkEffects inkEffect, int alpha, int sheetID)
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
    int widthFlip  = width;
    int heightFlip = height;

    if (width + x > currentScreen->clipBound_X2)
        width = currentScreen->clipBound_X2 - x;

    if (x < currentScreen->clipBound_X1) {
        int val = x - currentScreen->clipBound_X1;
        sprX -= val;
        width += val;
        widthFlip += 2 * val;
        x = currentScreen->clipBound_X1;
    }

    if (height + y > currentScreen->clipBound_Y2)
        height = currentScreen->clipBound_Y2 - y;
    if (y < currentScreen->clipBound_Y1) {
        int val = y - currentScreen->clipBound_Y1;
        sprY -= val;
        height += val;
        heightFlip += 2 * val;
        y = currentScreen->clipBound_Y1;
    }

    if (width <= 0 || height <= 0)
        return;

    GFXSurface *surface    = &gfxSurface[sheetID];
    validDraw              = true;
    int pitch              = currentScreen->pitch - width;
    int gfxPitch           = 0;
    byte *lineBuffer       = NULL;
    byte *gfxData          = NULL;
    ushort *frameBufferPtr = NULL;
    switch (direction) {
        default: break;

        case FLIP_NONE:
            gfxPitch       = surface->width - width;
            lineBuffer     = &gfxLineBuffer[y];
            gfxData        = &surface->dataPtr[sprX + surface->width * sprY];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
            gfxData        = &surface->dataPtr[widthFlip - 1 + sprX + surface->width * sprY];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
            gfxData        = &surface->dataPtr[sprX + surface->width * (sprY + heightFlip - 1)];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
            gfxData        = &surface->dataPtr[widthFlip - 1 + sprX + surface->width * (sprY + heightFlip - 1)];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
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
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
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
void DrawSpriteRotozoom(int x, int y, int pivotX, int pivotY, int width, int height, int sprX, int sprY, int scaleX, int scaleY, FlipFlags direction,
                        short rotation, InkEffects inkEffect, signed int alpha, int sheetID)
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

    int angle = 0x200 - (rotation & 0x1FF);
    if (!(rotation & 0x1FF))
        angle = rotation & 0x1FF;

    int sine        = sinVal512[angle];
    int cosine      = cosVal512[angle];
    int fullScaleXS = scaleX * sine >> 9;
    int fullScaleXC = scaleX * cosine >> 9;
    int fullScaleYS = scaleY * sine >> 9;
    int fullScaleYC = scaleY * cosine >> 9;

    int posX[4];
    int posY[4];
    int sprXPos = (sprX - pivotX) << 16;
    int sprYPos = (sprY - pivotY) << 16;

    int xMax     = 0;
    int scaledX1 = 0;
    int scaledX2 = 0;
    int scaledY1 = 0;
    int scaledY2 = 0;
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

    int scaledXMaxS = fullScaleXS * xMax;
    int scaledXMaxC = fullScaleXC * xMax;
    int scaledYMaxC = fullScaleYC * (pivotY + 2 + height);
    int scaledYMaxS = fullScaleYS * (pivotY + 2 + height);
    posX[1]         = x + ((scaledXMaxC + scaledY1) >> 9);
    posY[1]         = y + ((scaledY2 - scaledXMaxS) >> 9);
    posX[2]         = x + ((scaledYMaxS + scaledX2) >> 9);
    posY[2]         = y + ((scaledYMaxC - scaledX1) >> 9);
    posX[3]         = x + ((scaledXMaxC + scaledYMaxS) >> 9);
    posY[3]         = y + ((scaledYMaxC - scaledXMaxS) >> 9);

    int left = currentScreen->pitch;
    for (int i = 0; i < 4; ++i) {
        if (posX[i] < left)
            left = posX[i];
    }
    if (left < currentScreen->clipBound_X1)
        left = currentScreen->clipBound_X1;

    int right = 0;
    for (int i = 0; i < 4; ++i) {
        if (posX[i] > right)
            right = posX[i];
    }
    if (right > currentScreen->clipBound_X2)
        right = currentScreen->clipBound_X2;

    int top = currentScreen->size.y;
    for (int i = 0; i < 4; ++i) {
        if (posY[i] < top)
            top = posY[i];
    }
    if (top < currentScreen->clipBound_Y1)
        top = currentScreen->clipBound_Y1;

    int bottom = 0;
    for (int i = 0; i < 4; ++i) {
        if (posY[i] > bottom)
            bottom = posY[i];
    }
    if (bottom > currentScreen->clipBound_Y2)
        bottom = currentScreen->clipBound_Y2;

    int xSize = right - left;
    int ySize = bottom - top;
    if (xSize >= 1 && ySize >= 1) {
        GFXSurface *surface = &gfxSurface[sheetID];

        int fullX              = (sprX + width) << 16;
        int fullY              = (sprY + height) << 16;
        validDraw              = true;
        int fullScaleX         = (float)((512.0 / (float)scaleX) * 512.0);
        int fullScaleY         = (float)((512.0 / (float)scaleY) * 512.0);
        int deltaXLen          = fullScaleX * sine >> 2;
        int deltaX             = fullScaleX * cosine >> 2;
        int pitch              = currentScreen->pitch - xSize;
        int deltaYLen          = fullScaleY * cosine >> 2;
        int deltaY             = fullScaleY * sine >> 2;
        int lineSize           = surface->lineSize;
        byte *lineBuffer       = &gfxLineBuffer[top];
        int xLen               = left - x;
        int yLen               = top - y;
        byte *gfxData          = surface->dataPtr;
        ushort *frameBufferPtr = &currentScreen->frameBuffer[left + (top * currentScreen->pitch)];
        int fullSprY           = (sprY << 16) - 1;
        int fullSprX           = (sprX << 16) - 1;

        int drawX = 0, drawY = 0;
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
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
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
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
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

                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index) {
                                ushort color = palettePtr[index];
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
                ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index) {
                                ushort color = palettePtr[index];
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
                ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index) {
                                ushort color = palettePtr[index];
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
                for (int y = 0; y < ySize; ++y) {
                    int drawXPos = drawX;
                    int drawYPos = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
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
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
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
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
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

void DrawDeformedSprite(ushort sheetID, InkEffects inkEffect, int alpha)
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
    byte *gfxDataPtr          = surface->dataPtr;
    int clipY1                = currentScreen->clipBound_Y1;
    ScanlineInfo *scanlinePtr = &scanlines[clipY1];
    ushort *frameBufferPtr    = &currentScreen->frameBuffer[clipY1 * currentScreen->pitch];
    byte *lineBuffer          = &gfxLineBuffer[clipY1];
    int width                 = surface->width - 1;
    int height                = surface->height - 1;
    int lineSize              = surface->lineSize;

    switch (inkEffect) {
        case INK_NONE:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
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
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
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
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort color = palettePtr[palIndex];
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
            ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort color = palettePtr[palIndex];
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
            ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort color = palettePtr[palIndex];
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
                int lx = scanlinePtr->position.x;
                int ly = scanlinePtr->position.y;
                int dx = scanlinePtr->deform.x;
                int dy = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
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
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
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
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
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

void DrawTile(ushort *tiles, int32 countX, int32 countY, Vector2 *position, Vector2 *offset, bool32 screenRelative)
{
    if (tiles) {
        if (!position)
            position = &sceneInfo.entity->position;

        int x = position->x >> 0x10;
        int y = position->y >> 0x10;
        if (!screenRelative) {
            x -= currentScreen->position.x;
            y -= currentScreen->position.y;
        }

        switch (sceneInfo.entity->drawFX) {
            case FX_NONE:
            case FX_FLIP: {
                int drawX = 0;
                int drawY = 0;
                if (offset) {
                    drawX = x - (offset->x >> 17);
                    drawY = y - (offset->y >> 17);
                }
                else {
                    drawX = x - 8 * countX;
                    drawY = y - 8 * countY;
                }

                for (int ty = 0; ty < countY; ++ty) {
                    for (int tx = 0; tx < countX; ++tx) {
                        ushort tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            DrawSpriteFlipped(drawX + (tx * TILE_SIZE), drawY + (ty * TILE_SIZE), TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (tile & 0xFFF),
                                              FLIP_NONE, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                        }
                    }
                }
                break;
            }

            case FX_ROTATE: // Flip
            case FX_ROTATE | FX_FLIP: {
                int pivotX = 0;
                int pivotY = 0;
                if (offset) {
                    pivotX = -(offset->x >> 17);
                    pivotY = -(offset->y >> 17);
                }
                else {
                    pivotX = -(8 * countX);
                    pivotY = -(8 * countY);
                }

                for (int ty = 0; ty < countY; ++ty) {
                    for (int tx = 0; tx < countX; ++tx) {
                        ushort tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            switch ((tile >> 10) & 3) {
                                case FLIP_NONE:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_NONE, sceneInfo.entity->rotation,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_X:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_X, sceneInfo.entity->rotation,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_Y:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_X, sceneInfo.entity->rotation + 0x100,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_XY:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_NONE, sceneInfo.entity->rotation + 0x100,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;
                            }
                        }
                    }
                }
                break;
            }

            case FX_SCALE: // Scale
            case FX_SCALE | FX_FLIP: {
                int pivotX = 0;
                int pivotY = 0;
                if (offset) {
                    pivotX = -(offset->x >> 17);
                    pivotY = -(offset->y >> 17);
                }
                else {
                    pivotX = -(8 * countX);
                    pivotY = -(8 * countY);
                }

                for (int ty = 0; ty < countY; ++ty) {
                    for (int tx = 0; tx < countX; ++tx) {
                        ushort tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            switch ((tile >> 10) & 3) {
                                case FLIP_NONE:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X, 0,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_X:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X, 0,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_Y:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       0x100, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_XY:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE,
                                                       0x100, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;
                            }
                        }
                    }
                }
                break;
            }

            case FX_SCALE | FX_ROTATE: // Flip + Scale + Rotation
            case FX_SCALE | FX_ROTATE | FX_FLIP: {
                int pivotX = 0;
                int pivotY = 0;
                if (offset) {
                    pivotX = -(offset->x >> 17);
                    pivotY = -(offset->y >> 17);
                }
                else {
                    pivotX = -(8 * countX);
                    pivotY = -(8 * countY);
                }

                for (int ty = 0; ty < countY; ++ty) {
                    for (int tx = 0; tx < countX; ++tx) {
                        ushort tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            switch ((tile >> 10) & 3) {
                                case FLIP_NONE:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       sceneInfo.entity->rotation, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                                                       0);
                                    break;

                                case FLIP_X:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       sceneInfo.entity->rotation, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                                                       0);
                                    break;

                                case FLIP_Y:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       sceneInfo.entity->rotation + 0x100, (InkEffects)sceneInfo.entity->inkEffect,
                                                       sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_XY:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE,
                                                       sceneInfo.entity->rotation + 0x100, (InkEffects)sceneInfo.entity->inkEffect,
                                                       sceneInfo.entity->alpha, 0);
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
void DrawAniTile(ushort sheetID, ushort tileIndex, ushort srcX, ushort srcY, ushort width, ushort height)
{

    if (sheetID < SURFACE_MAX && tileIndex < TILE_COUNT) {
        GFXSurface *surface = &gfxSurface[sheetID];

        // FLIP_NONE
        byte *tileGFXData = &tilesetGFXData[tileIndex << 8];
        int cnt           = 0;
        for (int fy = 0; fy < height; fy += TILE_SIZE) {
            byte *gfxData = &surface->dataPtr[((fy + srcY) << surface->lineSize) + srcX];
            cnt += ((width - 1) / TILE_SIZE) + 1;
            for (int fx = 0; fx < width; fx += TILE_SIZE) {
                byte *gfxDataPtr = &gfxData[fx];
                for (int ty = 0; ty < TILE_SIZE; ++ty) {
                    for (int tx = 0; tx < TILE_SIZE; ++tx) {
                        *tileGFXData++ = *gfxDataPtr++;
                    }
                    gfxDataPtr += surface->width - TILE_SIZE;
                }
            }
        }

        // FLIP_X
        byte *srcTileGFXData = &tilesetGFXData[tileIndex << 8];
        if (cnt * TILE_SIZE > 0) {
            tileGFXData = &tilesetGFXData[(tileIndex << 8) + (FLIP_X * TILESET_SIZE) + (TILE_SIZE - 1)];

            for (int i = 0; i < cnt * TILE_SIZE; ++i) {
                for (int p = 0; p < TILE_SIZE; ++p) {
                    *tileGFXData-- = *srcTileGFXData++;
                }
                tileGFXData += (TILE_SIZE * 2);
            }
        }

        // FLIP_Y
        srcTileGFXData = &tilesetGFXData[tileIndex << 8];
        if (cnt * TILE_SIZE > 0) {
            int index = tileIndex;
            for (int i = 0; i < cnt; ++i) {
                tileGFXData = &tilesetGFXData[(index << 8) + (FLIP_Y * TILESET_SIZE) + (TILE_DATASIZE - TILE_SIZE)];
                for (int y = 0; y < TILE_SIZE; ++y) {
                    for (int x = 0; x < TILE_SIZE; ++x) {
                        *tileGFXData++ = *srcTileGFXData++;
                    }
                    tileGFXData -= (TILE_SIZE * 2);
                }
                ++index;
            }
        }

        // FLIP_XY
        srcTileGFXData = &tilesetGFXData[(tileIndex << 8) + (FLIP_Y * TILESET_SIZE)];
        if (cnt * TILE_SIZE > 0) {
            tileGFXData = &tilesetGFXData[(tileIndex << 8) + (FLIP_XY * TILESET_SIZE) + (TILE_SIZE - 1)];

            for (int i = 0; i < cnt * TILE_SIZE; ++i) {
                for (int p = 0; p < TILE_SIZE; ++p) {
                    *tileGFXData-- = *srcTileGFXData++;
                }
                tileGFXData += (TILE_SIZE * 2);
            }
        }
    }
}

void DrawString(RSDK::Animator *animator, Vector2 *position, TextInfo *info, int32 startFrame, int32 endFrame, int32 align, int32 spacing,
                void *unused, Vector2 *charOffsets, bool32 screenRelative)
{
    if (animator && info && animator->frames) {
        if (!position)
            position = &sceneInfo.entity->position;

        Entity *entity = sceneInfo.entity;

        int32 x = position->x >> 0x10;
        int32 y = position->y >> 0x10;
        if (!screenRelative) {
            x -= currentScreen->position.x;
            y -= currentScreen->position.y;
        }

        startFrame = clampVal(startFrame, 0, info->length - 1);

        if (endFrame <= 0 || endFrame > info->length)
            endFrame = info->length;

        switch (align) {
            case ALIGN_LEFT:
                if (charOffsets) {
                    for (; startFrame < endFrame; ++startFrame) {
                        ushort curChar = info->text[startFrame];
                        if (curChar < animator->frameCount) {
                            RSDK::SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x + (charOffsets->x >> 0x10), y + frame->pivotY + (charOffsets->y >> 0x10), frame->width, frame->height,
                                              frame->sprX, frame->sprY, FLIP_NONE, (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
                            x += spacing + frame->width;
                            ++charOffsets;
                        }
                    }
                }
                else {
                    for (; startFrame < endFrame; ++startFrame) {
                        ushort curChar = info->text[startFrame];
                        if (curChar < animator->frameCount) {
                            RSDK::SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x, y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                              (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
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
                        ushort curChar = info->text[endFrame];
                        if (curChar < animator->frameCount) {
                            RSDK::SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x - frame->width + (charOffset->x >> 0x10), y + frame->pivotY + (charOffset->y >> 0x10), frame->width,
                                              frame->height, frame->sprX, frame->sprY, FLIP_NONE, (InkEffects)entity->inkEffect, entity->alpha,
                                              frame->sheetID);
                            x = (x - frame->width) - spacing;
                            --charOffset;
                        }
                    }
                }
                else {
                    for (; endFrame >= startFrame; --endFrame) {
                        ushort curChar = info->text[endFrame];
                        if (curChar < animator->frameCount) {
                            RSDK::SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x - frame->width, y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                              (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
                            x = (x - frame->width) - spacing;
                        }
                    }
                }
                break;
        }
    }
}
void DrawDevText(const char *text, int32 x, int32 y, int32 align, uint color)
{
    int32 length   = 0;
    ushort color16 = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];

    bool32 finished = false;
    while (!finished) {
        char cur = text[length];
        finished = true;

        int32 cnt = 0;
        if (cur != '\n') {
            while (cur) {
                cur = text[++length];
                cnt++;
                if (cur == '\n') {
                    finished = false;
                    break;
                }
            }
        }

        if (y >= 0 && y < currentScreen->size.y - 7) {
            int32 drawX = x;

            int32 alignX = 0;
            if (align == ALIGN_CENTER)
                alignX = 4 * cnt;
            else if (align == ALIGN_RIGHT)
                alignX = 8 * cnt;

            drawX = x - alignX;

            const char *textPtr = &text[length - cnt];
            while (cnt > 0) {
                if (drawX >= 0 && drawX < currentScreen->size.x - 7) {
                    ushort *frameBufferPtr = &currentScreen->frameBuffer[drawX + y * currentScreen->pitch];
                    char curChar           = *textPtr;
                    if ((curChar < '\t' || curChar > '\n') && curChar != ' ') {
                        byte h              = 8;
                        byte *engineTextPtr = &engineTextBuffer[0x40 * *textPtr];
                        do {
                            --h;
                            int32 w = 8;
                            do {
                                --w;
                                if (*engineTextPtr)
                                    *frameBufferPtr = color16;
                                ++engineTextPtr;
                                ++frameBufferPtr;
                            } while (w);
                            frameBufferPtr = &frameBufferPtr[currentScreen->pitch - 8];
                        } while (h);
                    }
                    ++textPtr;
                    --cnt;
                    drawX += 8;
                }
            }
        }

        y += 8;
        ++length;
    }
}
