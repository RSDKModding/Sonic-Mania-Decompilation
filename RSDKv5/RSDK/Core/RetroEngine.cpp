#include "RSDK/Core/RetroEngine.hpp"

linkPtr linkGameLogic = NULL;

#if RETRO_PLATFORM == RETRO_WIN
#include "Windows.h"

HMODULE hLibModule = NULL;
#endif

#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
#include <dlfcn.h>

void *link_handle = NULL;
#if RETRO_PLATFORM == RETRO_ANDROID
#include <jni.h>
#include <unistd.h>
#endif
#endif

int32 *globalVarsPtr = NULL;
RetroEngine engine   = RetroEngine();

bool32 processEvents()
{
#if RETRO_USING_SDL1 || RETRO_USING_SDL2
    while (SDL_PollEvent(&engine.sdlEvents)) {
        // Main Events
        switch (engine.sdlEvents.type) {
#if RETRO_USING_SDL2
            case SDL_WINDOWEVENT:
                switch (engine.sdlEvents.window.event) {
                    case SDL_WINDOWEVENT_MAXIMIZED: {
                        SDL_RestoreWindow(engine.window);
                        SDL_SetWindowFullscreen(engine.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        SDL_ShowCursor(SDL_FALSE);
                        engine.isWindowed = false;
                        break;
                    }
                    case SDL_WINDOWEVENT_CLOSE: return false;
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
            case SDL_CONTROLLERDEVICEADDED: controllerInit(engine.sdlEvents.cdevice.which); break;
            case SDL_CONTROLLERDEVICEREMOVED: controllerClose(engine.sdlEvents.cdevice.which); break;
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
            case SDL_APP_TERMINATING: return false;
#endif

#ifdef RETRO_USING_MOUSE
            case SDL_MOUSEBUTTONDOWN:
#if RETRO_USING_SDL2
                switch (engine.sdlEvents.button.button) {
                    case SDL_BUTTON_LEFT: touchMouseData.down[0] = true; touchMouseData.count = 1;
#if !RETRO_REV02
                        if (buttonDownCount > 0)
                            buttonDownCount--;
#endif
                        break;
#if !RETRO_REV02
                    case SDL_BUTTON_RIGHT:
                        specialKeyStates[3] = true;
                        buttonDownCount++;
                        break;
#endif
                }
#endif //! RETRO_USING_SDL2
                break;
            case SDL_MOUSEBUTTONUP:
#if RETRO_USING_SDL2
                switch (engine.sdlEvents.button.button) {
                    case SDL_BUTTON_LEFT: touchMouseData.down[0] = false; touchMouseData.count = 0;
#if !RETRO_REV02
                        if (buttonDownCount > 0)
                            buttonDownCount--;
#endif
                        break;
#if !RETRO_REV02
                    case SDL_BUTTON_RIGHT:
                        specialKeyStates[3] = false;
                        buttonDownCount--;
                        break;
#endif
                }
#endif //! RETRO_USING_SDL2
                break;
#endif

#if defined(RETRO_USING_TOUCH) && RETRO_USING_SDL2
            case SDL_FINGERMOTION:
            case SDL_FINGERDOWN:
            case SDL_FINGERUP: {
                int count            = SDL_GetNumTouchFingers(engine.sdlEvents.tfinger.touchId);
                touchMouseData.count = 0;
                for (int i = 0; i < count; i++) {
                    SDL_Finger *finger = SDL_GetTouchFinger(engine.sdlEvents.tfinger.touchId, i);
                    if (finger) {
                        touchMouseData.down[touchMouseData.count] = true;
                        touchMouseData.x[touchMouseData.count]    = finger->x;
                        touchMouseData.y[touchMouseData.count]    = finger->y;
                        touchMouseData.count++;
                    }
                }
                break;
            }
#endif //! RETRO_USING_SDL2

            case SDL_KEYDOWN:
#if !RETRO_REV02
                ++buttonDownCount;
#endif
                switch (engine.sdlEvents.key.keysym.sym) {
                    default: break;
                    case SDLK_ESCAPE:
                        if (engine.devMenu) {
                            if (sceneInfo.state == ENGINESTATE_DEVMENU) {
                                sceneInfo.state = devMenu.stateStore;
                                if (devMenu.stateStore == ENGINESTATE_VIDEOPLAYBACK)
                                    engine.screenCount = 0;
                                ResumeSound();
                            }
                            else {
                                devMenu.stateStore = sceneInfo.state;
                                if (sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK)
                                    engine.screenCount = 1;
                                devMenu.state   = DevMenu_MainMenu;
                                devMenu.option  = 0;
                                devMenu.scroll  = 0;
                                devMenu.timer   = 0;
                                sceneInfo.state = ENGINESTATE_DEVMENU;
                                PauseSound();
                            }
                        }
#if !RETRO_REV02
                        specialKeyStates[0] = true;
#endif
                        break;
#if !RETRO_REV02
                    case SDLK_RETURN: specialKeyStates[1] = true; break;
#endif
                    case SDLK_F1:
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
                    case SDLK_F2:
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
                    case SDLK_F3: engine.shaderID = (engine.shaderID + 1) % (shaderCount - 4); break;
                    case SDLK_F4:
                        engine.isWindowed ^= 1;
                        if (!engine.isWindowed) {
#if RETRO_USING_SDL2
                            SDL_RestoreWindow(engine.window);
                            SDL_SetWindowFullscreen(engine.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                            SDL_ShowCursor(SDL_FALSE);
#endif
                        }
                        else {
#if RETRO_USING_SDL2
                            SDL_SetWindowFullscreen(engine.window, false);
                            SDL_ShowCursor(SDL_TRUE);
                            SDL_SetWindowSize(engine.window, engine.windowWidth, engine.windowHeight);
                            SDL_SetWindowPosition(engine.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                            SDL_RestoreWindow(engine.window);
#endif
                        }
                        break;
#if !RETRO_USE_ORIGINAL_CODE
                    case SDLK_F5:
                        if (engine.devMenu && engine.screenCount > 1)
                            engine.screenCount--;
                        break;
                    case SDLK_F6:
                        if (engine.devMenu && engine.screenCount < SCREEN_MAX)
                            engine.screenCount++;
                        break;
                    case SDLK_F9:
                        if (engine.devMenu)
                            showHitboxes ^= 1;
                        break;
                    case SDLK_F10:
                        if (engine.devMenu)
                            engine.showPaletteOverlay ^= 1;
                        break;
#endif
                    case SDLK_BACKSPACE:
                        if (engine.devMenu)
                            engine.gameSpeed = engine.fastForwardSpeed;
                        break;
                    case SDLK_F11:
                    case SDLK_INSERT:
                        if ((sceneInfo.state & ENGINESTATE_STEPOVER) == ENGINESTATE_STEPOVER)
                            engine.frameStep = true;
                        break;
                    case SDLK_F12:
                    case SDLK_PAUSE:
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
                switch (engine.sdlEvents.key.keysym.sym) {
                    default: break;
#if !RETRO_REV02
                    case SDLK_ESCAPE: specialKeyStates[0] = false; break;
                    case SDLK_RETURN: specialKeyStates[1] = false; break;
#endif
                    case SDLK_BACKSPACE: engine.gameSpeed = 1; break;
                }
                break;
            case SDL_QUIT: return false;
        }
    }
#endif
    return true;
}

bool32 initRetroEngine()
{
    RSDK::InitStorage();

#if RETRO_PLATFORM == RETRO_OSX
    char buffer[0x100];
    sprintf(buffer, "%s/RSDKv5/", getResourcesPath());
    RSDK::SKU::SetUserFileCallbacks(buffer, NULL, NULL);
#elif RETRO_PLATFORM == RETRO_ANDROID
    char buffer[0x200];

    JNIEnv *env      = (JNIEnv *)SDL_AndroidGetJNIEnv();
    jobject activity = (jobject)SDL_AndroidGetActivity();
    jclass cls(env->GetObjectClass(activity));
    jmethodID method = env->GetMethodID(cls, "getBasePath", "()Ljava/lang/String;");
    auto ret         = env->CallObjectMethod(activity, method);

    strcpy(buffer, env->GetStringUTFChars((jstring)ret, NULL));

    RSDK::SKU::SetUserFileCallbacks(buffer, NULL, NULL);

    env->DeleteLocalRef(activity);
    env->DeleteLocalRef(cls);
#elif RETRO_PLATFORM == RETRO_LINUX
    RSDK::SKU::SetUserFileCallbacks("./", NULL, NULL);
#else
    RSDK::SKU::SetUserFileCallbacks("", NULL, NULL);
#endif

    RSDK::SKU::InitUserData();

    // By Default we use the dummy system so this'll never be false
    // its used in cases like steam where it gives the "Steam must be running to play this game" message and closes
#if RETRO_REV02
    if (RSDK::SKU::userCore->CheckAPIInitialized()) {
#endif
        RSDK::SKU::readSettings();
        startGameObjects();

        engine.running = true;
        if (!InitRenderDevice()) {
            engine.running = false;
            return false;
        }

        if (!InitAudioDevice()) {
            engine.running = false;
            return false;
        }

        InitInputDevice();

#if RETRO_USE_MOD_LOADER
        // we confirmed the game actually is valid, lets start some callbacks
        RSDK::RunModCallbacks(RSDK::MODCB_GAME_STARTUP, NULL);
#endif

#if RETRO_REV02
    }
    else {
        engine.running = false;
    }
#endif
    return true;
}
void runRetroEngine()
{
    unsigned long long targetFreq = SDL_GetPerformanceFrequency() / engine.refreshRate;
    unsigned long long curTicks   = 0;
    unsigned long long prevTicks  = 0;

    while (engine.running) {
#if !RETRO_USE_ORIGINAL_CODE
        if (!engine.vsync) {
            curTicks = SDL_GetPerformanceCounter();
            if (curTicks < prevTicks + targetFreq)
                continue;
            prevTicks = curTicks;
        }
#endif

        engine.running  = processEvents();
        foreachStackPtr = foreachStackList;
#if !RETRO_USE_ORIGINAL_CODE
        debugHitboxCount = 0;
#endif

#if RETRO_REV02
        RSDK::SKU::userCore->FrameInit();
#endif

#if RETRO_REV02
        if (!RSDK::SKU::userCore->CheckEnginePause()) {
            // Focus Checks
            if (RSDK::SKU::userCore->CheckFocusLost()) {
                if (!(engine.focusState & 1)) {
                    engine.focusState = 1;
                    PauseSound();
                }
            }
            else if (engine.focusState) {
                engine.focusState = 0;
                ResumeSound();
            }
#endif

            if (!(engine.focusState & 1)) {
#if RETRO_USE_MOD_LOADER
                if (sceneInfo.state != ENGINESTATE_DEVMENU && devMenu.modsChanged) {
                    devMenu.modsChanged = false;
                    RSDK::saveMods();
                    for (int c = 0; c < CHANNEL_COUNT; ++c) StopChannel(c);
#if RETRO_REV02
                    hardResetFlag = true;
#endif
                    SceneInfo pre = sceneInfo;
                    startGameObjects();
                    sceneInfo.classCount = pre.classCount;
                    if (pre.state == ENGINESTATE_LOAD) {
                        sceneInfo.activeCategory = pre.activeCategory;
                        sceneInfo.listPos        = pre.listPos;
                    }
#if RETRO_USING_SDL2
                    SDL_SetWindowTitle(engine.window, RSDK::gameVerInfo.gameName);
#endif
                    LoadGlobalSfx();

                    sceneInfo.state = ENGINESTATE_LOAD;
                }
#endif

                if (engine.devMenu)
                    ProcessDebugCommands();

                switch (sceneInfo.state) {
                    default: break;
                    case ENGINESTATE_LOAD:
                        if (!sceneInfo.listData) {
                            sceneInfo.state = ENGINESTATE_NULL;
                        }
                        else {
                            LoadScene();
                            LoadSceneFile();
                            InitObjects();
#if RETRO_REV02
                            RSDK::SKU::userCore->StageLoad();
                            for (int v = 0; v < DRAWLAYER_COUNT; ++v) {
                                SetDebugValue(drawGroupNames[v], &engine.drawLayerVisible[v], DTYPE_BOOL, false, true);
                            }
#endif
                            // dim after 5 mins
                            engine.dimLimit = (5 * 60) * engine.refreshRate;
                            ProcessInput();
                            ProcessObjects();
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                            RSDK::SKU::LoadAchievementAssets();
#endif
                        }
                        break;
                    case ENGINESTATE_REGULAR:
                        ProcessInput();
                        ProcessSceneTimer();
                        ProcessObjects();
                        ProcessParallaxAutoScroll();
                        for (int i = 1; i < engine.gameSpeed; ++i) {
                            if (sceneInfo.state != ENGINESTATE_REGULAR)
                                break;
                            ProcessSceneTimer();
                            ProcessObjects();
                            ProcessParallaxAutoScroll();
                        }
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                        RSDK::SKU::ProcessAchievements();
#endif
                        ProcessObjectDrawLists();
                        break;
                    case ENGINESTATE_PAUSED:
                        ProcessInput();
                        ProcessPausedObjects();
                        for (int i = 1; i < engine.gameSpeed; ++i) {
                            if (sceneInfo.state != ENGINESTATE_PAUSED)
                                break;
                            ProcessPausedObjects();
                        }
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                        RSDK::SKU::ProcessAchievements();
#endif
                        ProcessObjectDrawLists();
                        break;
                    case ENGINESTATE_FROZEN:
                        ProcessInput();
                        ProcessFrozenObjects();
                        for (int i = 1; i < engine.gameSpeed; ++i) {
                            if (sceneInfo.state != ENGINESTATE_FROZEN)
                                break;
                            ProcessFrozenObjects();
                        }
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                        RSDK::SKU::ProcessAchievements();
#endif
                        ProcessObjectDrawLists();
                        break;
                    case ENGINESTATE_LOAD | ENGINESTATE_STEPOVER:
                        LoadScene();
                        LoadSceneFile();
                        InitObjects();
#if RETRO_REV02
                        RSDK::SKU::userCore->StageLoad();
                        for (int v = 0; v < DRAWLAYER_COUNT && v < DEBUGVAL_MAX; ++v) {
                            DebugValueInfo *val = &debugValues[debugValCnt++];
                            strncpy(val->name, drawGroupNames[v], 0x10);
                            val->type       = 0;
                            val->value      = &engine.drawLayerVisible[v];
                            val->valByteCnt = 4;
                            val->min        = 0;
                            val->max        = 1;
                        }
#endif
                        ProcessInput();
                        ProcessObjects();
                        sceneInfo.state = ENGINESTATE_REGULAR | ENGINESTATE_STEPOVER;
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                        RSDK::SKU::LoadAchievementAssets();
#endif
                        break;
                    case ENGINESTATE_REGULAR | ENGINESTATE_STEPOVER:
                        ProcessInput();
                        if (engine.frameStep) {
                            ProcessSceneTimer();
                            ProcessObjects();
                            ProcessParallaxAutoScroll();
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                            RSDK::SKU::ProcessAchievements();
#endif
                            ProcessObjectDrawLists();
                            engine.frameStep = false;
                        }
                        break;
                    case ENGINESTATE_PAUSED | ENGINESTATE_STEPOVER:
                        ProcessInput();
                        if (engine.frameStep) {
                            ProcessPausedObjects();
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                            RSDK::SKU::ProcessAchievements();
#endif
                            ProcessObjectDrawLists();
                            engine.frameStep = false;
                        }
                        break;
                    case ENGINESTATE_FROZEN | ENGINESTATE_STEPOVER:
                        ProcessInput();
                        if (engine.frameStep) {
                            ProcessFrozenObjects();
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                            RSDK::SKU::ProcessAchievements();
#endif
                            ProcessObjectDrawLists();
                            engine.frameStep = false;
                        }
                        break;
                    case ENGINESTATE_DEVMENU:
                        ProcessInput();
                        currentScreen = &screens[0];
                        if (devMenu.state)
                            devMenu.state();
                        break;
                    case ENGINESTATE_VIDEOPLAYBACK:
                        ProcessInput();
                        if (ProcessVideo() == 1)
                            sceneInfo.state = engine.prevEngineMode;
                        break;
                    case ENGINESTATE_SHOWPNG:
                        ProcessInput();
                        if (engine.imageDelta <= 0.0 || engine.dimMax >= 1.0) {
                            if (engine.displayTime <= 0.0) {
                                engine.dimMax += engine.imageDelta;
                                if (engine.dimMax <= 0.0) {
                                    engine.shaderID    = engine.prevShaderID;
                                    engine.screenCount = 1;
                                    sceneInfo.state    = engine.prevEngineMode;
                                    engine.dimMax      = 1.0;
                                }
                            }
                            else {
                                engine.displayTime -= 0.01666666666666667;
                                if (engine.skipCallback) {
                                    if (engine.skipCallback())
                                        engine.displayTime = 0.0;
                                }
                            }
                        }
                        else {
                            engine.dimMax += engine.imageDelta;
                            if (engine.dimMax >= 1.0) {
                                engine.imageDelta = -engine.imageDelta;
                                engine.dimMax     = 1.0;
                            }
                        }
                        break;
#if RETRO_REV02
                    case ENGINESTATE_ERRORMSG: {
                        ProcessInput();
                        if (controller[0].keyStart.down)
                            sceneInfo.state = engine.prevEngineMode;
                        currentScreen = &screens[0];
                        int yOff      = RSDK::DevOutput_GetStringYOffset(outputString);
                        DrawRectangle(0, currentScreen->center.y - (yOff >> 1), currentScreen->size.x, yOff, 128, 255, INK_NONE, true);
                        DrawDevText(outputString, 8, currentScreen->center.y - (yOff >> 1) + 8, 0, 0xF0F0F0);
                        break;
                    }
                    case ENGINESTATE_ERRORMSG_FATAL: {
                        ProcessInput();
                        currentScreen = &screens[0];
                        if (controller[0].keyStart.down)
                            engine.running = false;
                        int yOff = RSDK::DevOutput_GetStringYOffset(outputString);
                        DrawRectangle(0, currentScreen->center.y - (yOff >> 1), currentScreen->size.x, yOff, 0xF00000, 255, INK_NONE, true);
                        DrawDevText(outputString, 8, currentScreen->center.y - (yOff >> 1) + 8, 0, 0xF0F0F0);
                        break;
                    }
#endif
                }

#if !RETRO_USE_ORIGINAL_CODE
                for (int t = 0; t < touchMouseData.count; ++t) {
                    if (touchMouseData.down[t]) {
                        int tx = touchMouseData.x[t] * screens->size.x;
                        int ty = touchMouseData.y[t] * screens->size.y;

                        if (tx <= 32 && ty <= 32) {
                            if (engine.devMenu) {
                                if (sceneInfo.state != ENGINESTATE_DEVMENU) {
                                    devMenu.stateStore = sceneInfo.state;
                                    if (sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK)
                                        engine.screenCount = 1;
                                    devMenu.state   = DevMenu_MainMenu;
                                    devMenu.option  = 0;
                                    devMenu.scroll  = 0;
                                    devMenu.timer   = 0;
                                    sceneInfo.state = ENGINESTATE_DEVMENU;
                                    PauseSound();
                                }
                            }
                        }
                    }
                }
#endif
                // Uncomment this code to add the build number to dev menu
                // overrides the game subtitle, used in switch dev menu
                if (currentScreen && sceneInfo.state == ENGINESTATE_DEVMENU) {
                    // Switch 1.04 build # is 18403, 1.00 is 17051
                    // char buffer[0x40];
                    // sprintf(buffer, "Build #%d", 18403);
                    // DrawRectangle(currentScreen->center.x - 128, currentScreen->center.y - 48, 256, 8, 0x008000, 0xFF, INK_NONE, true);
                    // DrawDevText(buffer, currentScreen->center.x, currentScreen->center.y - 48, 1, 0xF0F0F0);
                }
            }

            FlipScreen();

#if RETRO_REV02
            if (!(engine.focusState & 1))
                RSDK::SKU::HandleUserStatuses();
        }
#endif
    }

    // Shutdown
    ReleaseAudioDevice();
    ReleaseRenderDevice();
    RSDK::SKU::writeSettings(false);
    RSDK::SKU::releaseUserData();
    RSDK::ReleaseStorage();
#if RETRO_USE_MOD_LOADER
    RSDK::unloadMods();
#endif

#if RETRO_USING_SDL1 || RETRO_USING_SDL2
    SDL_Quit();
#endif

#if RETRO_PLATFORM == RETRO_WIN
    if (hLibModule) {
        FreeLibrary(hLibModule);
        hLibModule = NULL;
    }
#endif

#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_LINUX
    if (link_handle)
        dlclose(link_handle);
#endif
}

void parseArguments(int32 argc, char *argv[])
{
    memset(currentSceneFolder, 0, sizeof(currentSceneFolder));
    memset(currentSceneID, 0, sizeof(currentSceneID));
#if RETRO_REV02
    sceneInfo.filter = 0;
#endif

    for (int a = 0; a < argc; ++a) {
        const char *find = "";

        find = strstr(argv[a], "stage=");
        if (find) {
            int b = 0;
            int c = 6;
            while (find[c] && find[c] != ';') currentSceneFolder[b++] = find[c++];
            currentSceneFolder[b] = 0;
        }

        find = strstr(argv[a], "scene=");
        if (find) {
            int b = 0;
            int c = 6;
            while (find[c] && find[c] != ';') currentSceneID[b++] = find[c++];
            currentSceneID[b] = 0;
        }

#if RETRO_REV02
        find = strstr(argv[a], "filter=");
        if (find) {
            char buf[0x10];

            int b = 0;
            int c = 7;
            while (argv[a][c] && find[c] != ';') buf[b++] = find[c++];
            buf[b]           = 0;
            sceneInfo.filter = atoi(buf);
        }
#endif

        find = strstr(argv[a], "console=true");
        if (find) {
            engine.printConsole = true;
            engine.devMenu      = true;
#if RETRO_PLATFORM == RETRO_WIN
            AllocConsole();
            freopen_s((FILE **)stdin, "CONIN$", "w", stdin);
            freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
            freopen_s((FILE **)stderr, "CONOUT$", "w", stderr);
#endif
        }
    }
}

void startGameObjects()
{
    memset(&objectList, 0, OBJECT_COUNT * sizeof(ObjectInfo));
    sceneInfo.classCount     = 0;
    sceneInfo.activeCategory = 0;
    sceneInfo.listPos        = 0;
    sceneInfo.state          = 0;
    sceneInfo.inEditor       = 0;
    sceneInfo.debugMode      = engine.devMenu;
    devMenu.state            = DevMenu_MainMenu;
    for (int l = 0; l < DRAWLAYER_COUNT; ++l) engine.drawLayerVisible[l] = true;
    RSDK::setupFunctions();
    InitScriptSystem();
    LoadGameConfig();
}

#if RETRO_USE_MOD_LOADER
const tinyxml2::XMLElement *firstXMLChildElement(tinyxml2::XMLDocument *doc, const tinyxml2::XMLElement *elementPtr, const char *name)
{
    if (doc) {
        if (!elementPtr)
            return doc->FirstChildElement(name);
        else
            return elementPtr->FirstChildElement(name);
    }
    return NULL;
}

const tinyxml2::XMLElement *nextXMLSiblingElement(tinyxml2::XMLDocument *doc, const tinyxml2::XMLElement *elementPtr, const char *name)
{
    if (doc) {
        if (!elementPtr)
            return doc->NextSiblingElement(name);
        else
            return elementPtr->NextSiblingElement(name);
    }
    return NULL;
}

const tinyxml2::XMLAttribute *findXMLAttribute(const tinyxml2::XMLElement *elementPtr, const char *name) { return elementPtr->FindAttribute(name); }
const char *getXMLAttributeName(const tinyxml2::XMLAttribute *attributePtr) { return attributePtr->Name(); }
int32 getXMLAttributeValueInt(const tinyxml2::XMLAttribute *attributePtr) { return attributePtr->IntValue(); }
bool32 getXMLAttributeValueBool(const tinyxml2::XMLAttribute *attributePtr) { return attributePtr->BoolValue(); }
const char *getXMLAttributeValueString(const tinyxml2::XMLAttribute *attributePtr) { return attributePtr->Value(); }

void LoadXMLObjects()
{
    FileInfo info;

    for (int m = 0; m < (int)RSDK::modList.size(); ++m) {
        if (!RSDK::modList[m].active)
            continue;

        RSDK::setActiveMod(m);
        InitFileInfo(&info);
        if (LoadFile(&info, "Data/Game/Game.xml", FMODE_RB)) {
            tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument;

            char *xmlData = new char[info.fileSize + 1];
            ReadBytes(&info, xmlData, info.fileSize);
            xmlData[info.fileSize] = 0;
            CloseFile(&info);

            bool success = doc->Parse(xmlData) == tinyxml2::XML_SUCCESS;

            if (success) {
                const tinyxml2::XMLElement *gameElement    = firstXMLChildElement(doc, nullptr, "game");
                const tinyxml2::XMLElement *objectsElement = firstXMLChildElement(doc, gameElement, "objects");
                if (objectsElement) {
                    const tinyxml2::XMLElement *objElement = firstXMLChildElement(doc, objectsElement, "object");
                    if (objElement) {
                        do {
                            const tinyxml2::XMLAttribute *nameAttr = findXMLAttribute(objElement, "name");
                            const char *objName                    = "unknownObject";
                            if (nameAttr)
                                objName = getXMLAttributeValueString(nameAttr);

                            RETRO_HASH(hash);
                            GEN_HASH(objName, hash);
                            globalObjectIDs[globalObjectCount] = 0;
                            for (int objID = 0; objID < objectCount; ++objID) {
                                if (HASH_MATCH(hash, objectList[objID].hash)) {
                                    globalObjectIDs[globalObjectCount] = objID;
                                    globalObjectCount++;
                                }
                            }

                        } while ((objElement = nextXMLSiblingElement(doc, objElement, "object")));
                    }
                }
            }
            else {
                PrintLog(PRINT_NORMAL, "Failed to parse Game.xml File!");
            }

            delete[] xmlData;
            delete doc;
        }
    }
    RSDK::setActiveMod(-1);
}
void LoadXMLSoundFX()
{
    FileInfo info;
    for (int m = 0; m < (int)RSDK::modList.size(); ++m) {
        if (!RSDK::modList[m].active)
            continue;

        RSDK::setActiveMod(m);
        InitFileInfo(&info);
        if (LoadFile(&info, "Data/Game/Game.xml", FMODE_RB)) {
            tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument;

            char *xmlData = new char[info.fileSize + 1];
            ReadBytes(&info, xmlData, info.fileSize);
            xmlData[info.fileSize] = 0;
            CloseFile(&info);

            bool success = doc->Parse(xmlData) == tinyxml2::XML_SUCCESS;

            if (success) {
                const tinyxml2::XMLElement *gameElement   = firstXMLChildElement(doc, nullptr, "game");
                const tinyxml2::XMLElement *soundsElement = firstXMLChildElement(doc, gameElement, "sounds");
                if (soundsElement) {
                    const tinyxml2::XMLElement *sfxElement = firstXMLChildElement(doc, soundsElement, "soundfx");
                    if (sfxElement) {
                        do {
                            const tinyxml2::XMLAttribute *valAttr = findXMLAttribute(sfxElement, "path");
                            const char *sfxPath                   = "unknownSFX.wav";
                            if (valAttr)
                                sfxPath = getXMLAttributeValueString(valAttr);

                            const tinyxml2::XMLAttribute *playsAttr = findXMLAttribute(sfxElement, "maxConcurrentPlays");
                            int maxConcurrentPlays                  = 0;
                            if (playsAttr)
                                maxConcurrentPlays = getXMLAttributeValueInt(playsAttr);

                            LoadSfx((char *)sfxPath, maxConcurrentPlays, SCOPE_GLOBAL);

                        } while ((sfxElement = nextXMLSiblingElement(doc, sfxElement, "soundfx")));
                    }
                }
            }
            else {
                PrintLog(PRINT_NORMAL, "Failed to parse Game.xml File!");
            }

            delete[] xmlData;
            delete doc;
        }
    }
    RSDK::setActiveMod(-1);
}
int32 LoadXMLStages(int32 mode, int32 gcListCount, int32 gcStageCount)
{
    FileInfo info;
    int listCount  = 0;
    int stageCount = 0;

    for (int m = 0; m < (int)RSDK::modList.size(); ++m) {
        if (!RSDK::modList[m].active)
            continue;

        RSDK::setActiveMod(m);
        InitFileInfo(&info);
        if (LoadFile(&info, "Data/Game/Game.xml", FMODE_RB)) {
            tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument;

            char *xmlData = new char[info.fileSize + 1];
            ReadBytes(&info, xmlData, info.fileSize);
            xmlData[info.fileSize] = 0;
            CloseFile(&info);

            bool success = doc->Parse(xmlData) == tinyxml2::XML_SUCCESS;

            if (success) {
                const tinyxml2::XMLElement *gameElement = firstXMLChildElement(doc, nullptr, "game");
                const tinyxml2::XMLElement *listElement = firstXMLChildElement(doc, gameElement, "category");
                if (listElement) {
                    do {
                        int listID                             = gcListCount++;
                        const tinyxml2::XMLElement *stgElement = firstXMLChildElement(doc, listElement, "stage");

                        SceneListInfo *list = &sceneInfo.listCategory[listID];
                        if (!mode) {
                            const tinyxml2::XMLAttribute *nameAttr = findXMLAttribute(listElement, "name");
                            const char *lstName                    = "unknown list";
                            if (nameAttr)
                                lstName = getXMLAttributeValueString(nameAttr);

                            sprintf(list->name, "%s", lstName);
                            GEN_HASH(list->name, list->hash);

                            list->sceneOffsetStart = gcStageCount;
                            list->sceneOffsetEnd   = gcStageCount;
                            list->sceneCount       = 0;
                        }

                        if (stgElement) {
                            do {
                                if (!mode) {
                                    const tinyxml2::XMLAttribute *nameAttr = findXMLAttribute(stgElement, "name");
                                    const char *stgName                    = "unknownStage";
                                    if (nameAttr)
                                        stgName = getXMLAttributeValueString(nameAttr);

                                    const tinyxml2::XMLAttribute *folderAttr = findXMLAttribute(stgElement, "folder");
                                    const char *stgFolder                    = "unknownStageFolder";
                                    if (nameAttr)
                                        stgFolder = getXMLAttributeValueString(folderAttr);

                                    const tinyxml2::XMLAttribute *idAttr = findXMLAttribute(stgElement, "id");
                                    const char *stgID                    = "unknownStageID";
                                    if (idAttr)
                                        stgID = getXMLAttributeValueString(idAttr);

#if RETRO_REV02
                                    const tinyxml2::XMLAttribute *filterAttr = findXMLAttribute(stgElement, "highlight");
                                    int stgFilter                            = 0;
                                    if (stgFilter)
                                        stgFilter = getXMLAttributeValueInt(filterAttr);
#endif

                                    SceneListEntry *scene = &sceneInfo.listData[gcStageCount];

                                    sprintf(scene->name, "%s", stgName);
                                    GEN_HASH(scene->name, scene->hash);
                                    sprintf(scene->folder, "%s", stgFolder);
                                    sprintf(scene->id, "%s", stgID);

#if RETRO_REV02
                                    scene->filter = stgFilter;
                                    if (scene->filter == 0x00)
                                        scene->filter = 0xFF;
#endif

                                    gcStageCount++;
                                    sceneInfo.listCategory[listID].sceneCount++;
                                }
                                ++stageCount;
                            } while ((stgElement = nextXMLSiblingElement(doc, stgElement, "stage")));
                        }

                        if (!mode) {
                            list->sceneOffsetEnd += list->sceneCount;
                            sceneInfo.categoryCount++;
                        }

                        ++listCount;
                    } while ((listElement = nextXMLSiblingElement(doc, listElement, "category")));
                }
            }
            else {
                PrintLog(PRINT_NORMAL, "Failed to parse Game.xml File!");
            }

            delete[] xmlData;
            delete doc;
        }
    }
    RSDK::setActiveMod(-1);

    if (mode == 1)
        return listCount;
    else
        return stageCount;
}
#endif

void LoadGameConfig()
{
    FileInfo info;
    InitFileInfo(&info);
    int gcListCount  = 0;
    int gcSceneCount = 0;

    if (LoadFile(&info, "Data/Game/GameConfig.bin", FMODE_RB)) {
        char buffer[0x100];
        uint sig = ReadInt32(&info, false);

        if (sig != 0x474643) {
            CloseFile(&info);
            return;
        }

        ReadString(&info, RSDK::gameVerInfo.gameName);
        if (!useDataFile)
            sprintf(RSDK::gameVerInfo.gameName, "%s (Data Folder)", RSDK::gameVerInfo.gameName);
        ReadString(&info, RSDK::gameVerInfo.gameSubName);
        ReadString(&info, RSDK::gameVerInfo.gameVersion);

        sceneInfo.activeCategory = ReadInt8(&info);
        int startScene           = ReadInt16(&info);

        byte objCnt       = ReadInt8(&info);
        globalObjectCount = TYPE_DEFAULTCOUNT;
        for (int i = 0; i < objCnt; ++i) {
            ReadString(&info, textBuffer);

            RETRO_HASH(hash);
            GEN_HASH(textBuffer, hash);

            if (objectCount > 0) {
                globalObjectIDs[globalObjectCount] = 0;
                for (int objID = 0; objID < objectCount; ++objID) {
                    if (HASH_MATCH(hash, objectList[objID].hash)) {
                        globalObjectIDs[globalObjectCount] = objID;
                        globalObjectCount++;
                    }
                }
            }
        }

        for (int i = 0; i < PALETTE_COUNT; ++i) {
            activeGlobalRows[i] = ReadInt16(&info);
            for (int r = 0; r < 0x10; ++r) {
                if ((activeGlobalRows[i] >> r & 1)) {
                    for (int c = 0; c < 0x10; ++c) {
                        byte red                       = ReadInt8(&info);
                        byte green                     = ReadInt8(&info);
                        byte blue                      = ReadInt8(&info);
                        globalPalette[i][(r << 4) + c] = rgb32To16_B[blue] | rgb32To16_G[green] | rgb32To16_R[red];
                    }
                }
                else {
                    for (int c = 0; c < 0x10; ++c) globalPalette[i][(r << 4) + c] = 0;
                }
            }
        }

        byte sfxCnt = ReadInt8(&info);
        for (int i = 0; i < sfxCnt; ++i) {
            ReadString(&info, buffer);
            ReadInt8(&info);
        }

        ushort totalSceneCount = ReadInt16(&info);

#if RETRO_USE_MOD_LOADER
        gcSceneCount = totalSceneCount;
        totalSceneCount += LoadXMLStages(2, 0, 0);
#endif
        if (!totalSceneCount)
            totalSceneCount = 1;

        if (strlen(currentSceneFolder) && strlen(currentSceneID)) {
            RSDK::AllocateStorage(sizeof(SceneListEntry) * (totalSceneCount + 1), (void **)&sceneInfo.listData, RSDK::DATASET_STG, false);
            SceneListEntry *scene = &sceneInfo.listData[totalSceneCount];
            strcpy(scene->name, "_RSDK_SCENE");
            strcpy(scene->folder, currentSceneFolder);
            strcpy(scene->id, currentSceneID);
#if RETRO_REV02
            scene->filter = sceneInfo.filter;
#endif
            GEN_HASH(scene->name, scene->hash);

            // Override existing values
            sceneInfo.activeCategory = 0;
            startScene               = totalSceneCount;
            currentSceneFolder[0]    = 0;
            currentSceneID[0]        = 0;
        }
        else {
            RSDK::AllocateStorage(sizeof(SceneListEntry) * totalSceneCount, (void **)&sceneInfo.listData, RSDK::DATASET_STG, false);
        }

        sceneInfo.categoryCount = ReadInt8(&info);
        sceneInfo.listPos       = 0;

        int categoryCount = sceneInfo.categoryCount;
#if RETRO_USE_MOD_LOADER
        gcListCount = categoryCount;
        categoryCount += LoadXMLStages(1, 0, 0);
#endif
        if (!categoryCount)
            categoryCount = 1;

        RSDK::AllocateStorage(sizeof(SceneListInfo) * categoryCount, (void **)&sceneInfo.listCategory, RSDK::DATASET_STG, false);
        sceneInfo.listPos = 0;

        int sceneID = 0;
        for (int i = 0; i < sceneInfo.categoryCount; ++i) {
            SceneListInfo *category = &sceneInfo.listCategory[i];
            ReadString(&info, category->name);
            GEN_HASH(category->name, category->hash);

            category->sceneOffsetStart = sceneID;
            category->sceneCount       = ReadInt8(&info);
            for (int s = 0; s < category->sceneCount; ++s) {
                SceneListEntry *scene = &sceneInfo.listData[sceneID + s];
                ReadString(&info, scene->name);
                GEN_HASH(scene->name, scene->hash);

                ReadString(&info, scene->folder);
                ReadString(&info, scene->id);

#if RETRO_REV02
                scene->filter = ReadInt8(&info);
                if (scene->filter == 0x00)
                    scene->filter = 0xFF;
#endif
            }
            category->sceneOffsetEnd = category->sceneOffsetStart + category->sceneCount;
            sceneID += category->sceneCount;
        }

        byte varCount = ReadInt8(&info);
        for (int i = 0; i < varCount && globalVarsPtr; ++i) {
            int offset = ReadInt32(&info, false);
            int count  = ReadInt32(&info, false);
            for (int v = 0; v < count; ++v) {
                globalVarsPtr[offset + v] = ReadInt32(&info, false);
            }
        }

        CloseFile(&info);
#if RETRO_USE_MOD_LOADER
        LoadXMLObjects();
        LoadXMLStages(0, gcListCount, gcSceneCount);
#endif

        sceneInfo.listPos = sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetStart + startScene;
    }
}

void InitScriptSystem()
{
#if RETRO_USE_MOD_LOADER
    objectCount = 0;
    memset(globalObjectIDs, 0, sizeof(int32) * OBJECT_COUNT);
    memset(objectEntityList, 0, sizeof(EntityBase) * ENTITY_COUNT);
    editableVarCount = 0;
    foreachStackPtr  = foreachStackList;
    RSDK::currentMod = NULL;
#endif
    RegisterObject((Object **)&RSDK::DefaultObject, ":DefaultObject:", sizeof(RSDK::EntityDefaultObject), sizeof(RSDK::ObjectDefaultObject),
                   RSDK::DefaultObject_Update, RSDK::DefaultObject_LateUpdate, RSDK::DefaultObject_StaticUpdate, RSDK::DefaultObject_Draw,
                   RSDK::DefaultObject_Create, RSDK::DefaultObject_StageLoad, RSDK::DefaultObject_EditorDraw, RSDK::DefaultObject_EditorLoad,
                   RSDK::DefaultObject_Serialize);
#if RETRO_REV02
    RegisterObject((Object **)&RSDK::DevOutput, ":DevOutput:", sizeof(RSDK::EntityDevOutput), sizeof(RSDK::ObjectDevOutput), RSDK::DevOutput_Update,
                   RSDK::DevOutput_LateUpdate, RSDK::DevOutput_StaticUpdate, RSDK::DevOutput_Draw, RSDK::DevOutput_Create, RSDK::DevOutput_StageLoad,
                   RSDK::DevOutput_EditorDraw, RSDK::DevOutput_EditorLoad, RSDK::DevOutput_Serialize);
#endif
    globalObjectIDs[0] = TYPE_DEFAULTOBJECT;
#if RETRO_REV02
    globalObjectIDs[1] = TYPE_DEVOUTPUT;
#endif

    globalObjectCount = TYPE_DEFAULTCOUNT;

    RSDK::GameInfo info;

    info.functionPtrs = RSDK::RSDKFunctionTable;
#if RETRO_REV02
    info.APIPtrs    = RSDK::APIFunctionTable;
    info.currentSKU = &RSDK::SKU::curSKU;
#endif
    info.engineInfo = &RSDK::gameVerInfo;
    info.sceneInfo  = &sceneInfo;
    info.controller = controller;
    info.stickL     = stickL;
#if RETRO_REV02
    info.stickR   = stickR;
    info.triggerL = triggerL;
    info.triggerR = triggerR;
#endif
    info.touchMouse = &touchMouseData;
#if RETRO_REV02
    info.unknown = &RSDK::SKU::unknownInfo;
#endif
    info.screenInfo = screens;
#if RETRO_USE_MOD_LOADER
    info.modPtrs = RSDK::modFunctionTable;
#endif
    bool32 linked = false;

    if (engine.useExternalCode) {
#if RETRO_PLATFORM == RETRO_WIN
        if (!hLibModule)
            hLibModule = LoadLibraryA(gameLogicName);

        if (hLibModule) {
            linkPtr linkGameLogic = (linkPtr)GetProcAddress(hLibModule, "LinkGameLogicDLL");
            if (linkGameLogic) {
                linkGameLogic(&info);
                linked = true;
            }
        }
#endif
#if RETRO_PLATFORM == RETRO_OSX
        char buffer[0x100];
        sprintf(buffer, "%s%s.dylib", RSDK::SKU::userFileDir, gameLogicName);
        if (!link_handle)
            link_handle = dlopen(buffer, RTLD_LOCAL | RTLD_LAZY);

        if (link_handle) {
            linkPtr linkGameLogic = (linkPtr)dlsym(link_handle, "LinkGameLogicDLL");
            if (linkGameLogic) {
                linkGameLogic(&info);
                linked = true;
            }
        }
#endif
#if RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
        char buffer[0x100];
        sprintf(buffer, "%s%s.so", RSDK::SKU::userFileDir, gameLogicName);
        if (!link_handle)
            link_handle = dlopen(buffer, RTLD_LOCAL | RTLD_LAZY);
        
        if (link_handle) {
            linkPtr linkGameLogic = (linkPtr)dlsym(link_handle, "LinkGameLogicDLL");
            if (linkGameLogic) {
                linkGameLogic(&info);
                linked = true;
            }
        }
#endif

        if (!linked) {
            PrintLog(PRINT_POPUP, "Failed to link game logic!");
        }
    }
    else {
        linkGameLogic(&info);
    }

#if RETRO_USE_MOD_LOADER
    for (int32 m = 0; m < RSDK::modList.size(); ++m) {
        if (!RSDK::modList[m].active || RSDK::modList[m].language)
            continue;
        RSDK::currentMod = &RSDK::modList[m];
        for (RSDK::modLinkSTD ptr : RSDK::modList[m].linkModLogic) {
            if (!ptr(&info, RSDK::modList[m].id.c_str())) {
                RSDK::modList[m].active = false;
                PrintLog(PRINT_ERROR, "[MOD] Failed to link logic for mod %s!", RSDK::modList[m].id.c_str());
            }
        }
    }
    RSDK::currentMod = NULL;
    RSDK::sortMods();
#endif
}

void ProcessDebugCommands()
{
    // This block of code here isn't original, but without it this function overrides the keyboard ones, which is really annoying!
    int id            = ControllerIDForInputID(1);
    uint8 gamepadType = GetControllerType(id) >> 8;
    if (gamepadType != DEVICE_TYPE_CONTROLLER || id == CONT_UNASSIGNED || id == CONT_AUTOASSIGN)
        return;

    if (controller[CONT_P1].keySelect.press) {
        if (sceneInfo.state == ENGINESTATE_DEVMENU) {
            sceneInfo.state = devMenu.stateStore;
            if (devMenu.stateStore == ENGINESTATE_VIDEOPLAYBACK)
                engine.screenCount = 0;
            ResumeSound();
        }
        else {
            devMenu.stateStore = sceneInfo.state;
            if (sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK)
                engine.screenCount = 1;
            devMenu.state   = DevMenu_MainMenu;
            devMenu.option  = 0;
            devMenu.scroll  = 0;
            devMenu.timer   = 0;
            sceneInfo.state = ENGINESTATE_DEVMENU;
            PauseSound();
        }
    }

    bool32 framePaused = (sceneInfo.state >> 2) & 1;

    if (triggerL[CONT_P1].keyBumper.down) {
        if (triggerL[CONT_P1].keyTrigger.down) {
            if (!framePaused)
                sceneInfo.state ^= ENGINESTATE_STEPOVER;
        }
        else {
            if (framePaused)
                sceneInfo.state ^= ENGINESTATE_STEPOVER;
        }

        framePaused = (sceneInfo.state >> 2) & 1;
        if (framePaused) {
            if (triggerR[CONT_P1].keyBumper.press)
                engine.frameStep = true;
        }
        else {
            if (triggerR[CONT_P1].keyTrigger.down)
                engine.gameSpeed = 8;
            else
                engine.gameSpeed = 1;
        }
    }
    else {
        if (engine.gameSpeed == 8)
            engine.gameSpeed = 1;

        if (framePaused)
            sceneInfo.state ^= ENGINESTATE_STEPOVER;
    }
}
