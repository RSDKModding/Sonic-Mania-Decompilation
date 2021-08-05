#include "RetroEngine.hpp"

linkPtr linkGameLogic = NULL;

#if RETRO_PLATFORM == RETRO_WIN
#include "Windows.h"

HMODULE hLibModule = NULL;
#endif

#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
#include <dlfcn.h>

void* link_handle = NULL;
#endif

byte *gameOptionsPtr = NULL;
RetroEngine engine  = RetroEngine();

bool32 processEvents()
{
    touchMouseData.count = 0;
#if RETRO_USING_SDL2
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
                        engine.isFullScreen = true;
                        break;
                    }
                    case SDL_WINDOWEVENT_CLOSE: return false;
                }
                break;
            case SDL_CONTROLLERDEVICEADDED: controllerInit(engine.sdlEvents.cdevice.which); break;
            case SDL_CONTROLLERDEVICEREMOVED: controllerClose(engine.sdlEvents.cdevice.which); break;
            case SDL_WINDOWEVENT_CLOSE:
                if (engine.window) {
                    SDL_DestroyWindow(engine.window);
                    engine.window = NULL;
                }
                return false;
#endif

#ifdef RETRO_USING_MOUSE
            /*case SDL_MOUSEMOTION:

                break;*/
            case SDL_MOUSEBUTTONDOWN:
#if RETRO_USING_SDL2
                if (SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE)) <= 0) { // Touch always takes priority over mouse
#endif                                                                                     //! RETRO_USING_SDL2
                    switch (engine.sdlEvents.button.button) {
                        case SDL_BUTTON_LEFT:
                            touchMouseData.down[0] = true;
                            touchMouseData.count   = 1;
                            break;
                        //case SDL_BUTTON_MIDDLE: touchMouseData.down[0] = true; break;
                        //case SDL_BUTTON_RIGHT: touchMouseData.down[0] = true; break;
                    }
#if RETRO_USING_SDL2
                }
#endif //! RETRO_USING_SDL2
                break;
            case SDL_MOUSEBUTTONUP:
#if RETRO_USING_SDL2
                if (SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE)) <= 0) { // Touch always takes priority over mouse
#endif                                                                                     //! RETRO_USING_SDL2
                    switch (engine.sdlEvents.button.button) {
                        case SDL_BUTTON_LEFT:
                            touchMouseData.down[0] = false;
                            touchMouseData.count   = 0;
                            break;
                        //case SDL_BUTTON_MIDDLE: touchMouseData.down[0] = false; break;
                        //case SDL_BUTTON_RIGHT: touchMouseData.down[0] = false; break;
                    }
#if RETRO_USING_SDL2
                }
#endif //! RETRO_USING_SDL2
                break;
#endif

#if defined(RETRO_USING_TOUCH) && defined(RETRO_USING_SDL2)
            case SDL_FINGERMOTION:
                touchMouseData.count = SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE));
                for (int i = 0; i < touchMouseData.count; i++) {
                    SDL_Finger *finger     = SDL_GetTouchFinger(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE), i);
                    touchMouseData.down[i] = true;
                    touchMouseData.x[i]    = finger->x;
                    touchMouseData.y[i]    = finger->y;
                }
                break;
            case SDL_FINGERDOWN:
                touchMouseData.count = SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE));
                for (int i = 0; i < touchMouseData.count; i++) {
                    SDL_Finger *finger     = SDL_GetTouchFinger(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE), i);
                    touchMouseData.down[i] = true;
                    touchMouseData.x[i]    = finger->x;
                    touchMouseData.y[i]    = finger->y;
                }
                break;
            case SDL_FINGERUP: touchMouseData.count = SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE)); break;
#endif //! RETRO_USING_SDL2

            case SDL_KEYDOWN:
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
                                devMenu.state  = DevMenu_MainMenu;
                                devMenu.option = 0;
                                devMenu.scroll = 0;
                                devMenu.timer  = 0;
                                sceneInfo.state = ENGINESTATE_DEVMENU;
                                PauseSound();
                            }
                        }
                        break;
                    case SDLK_F1:
                        sceneInfo.listPos--;
                        if (sceneInfo.listPos >= sceneInfo.listCategory[sceneInfo.activeCategory].sceneCount) {
                            sceneInfo.activeCategory--;
                            if (sceneInfo.activeCategory >= sceneInfo.categoryCount) {
                                sceneInfo.activeCategory = sceneInfo.categoryCount - 1;
                            }
                            sceneInfo.listPos = sceneInfo.listCategory[sceneInfo.activeCategory].sceneCount - 1;
                        }
                        InitSceneLoad();
                        break;
                    case SDLK_F2:
                        sceneInfo.listPos++;
                        if (sceneInfo.listPos >= sceneInfo.listCategory[sceneInfo.activeCategory].sceneCount) {
                            sceneInfo.activeCategory++;
                            if (sceneInfo.activeCategory >= sceneInfo.categoryCount) {
                                sceneInfo.activeCategory = 0;
                            }
                            sceneInfo.listPos = 0;
                        }
                        InitSceneLoad();
                        break;
                    case SDLK_F3: engine.shaderID = (engine.shaderID + 1) % (shaderCount - 4); break;
                    case SDLK_F4:
                        engine.isFullScreen ^= 1;
                        if (engine.isFullScreen) {
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
                        if (engine.devMenu) {
                            sceneInfo.activeCategory = 0;
                            sceneInfo.listPos        = 0;
                            sceneInfo.state   = ENGINESTATE_LOAD;
                        }
                        break;
                    case SDLK_F6:
                        if (engine.devMenu) {
                            sceneInfo.listPos--;
                            SceneListInfo *list = &sceneInfo.listCategory[sceneInfo.activeCategory];
                            if (sceneInfo.listPos < 0) {
                                if (sceneInfo.activeCategory - 1 < 0) {
                                    sceneInfo.activeCategory = sceneInfo.categoryCount - 1;
                                }
                                else {
                                    sceneInfo.activeCategory--;
                                }
                                list = &sceneInfo.listCategory[sceneInfo.activeCategory];
                                sceneInfo.listPos = list->sceneCount - 1;
                            }
                            sceneInfo.state = ENGINESTATE_LOAD;
                        }
                        break;
                    case SDLK_F7:
                        if (engine.devMenu) {
                            sceneInfo.listPos++;
                            SceneListInfo *list = &sceneInfo.listCategory[sceneInfo.activeCategory];
                            if (sceneInfo.listPos - list->sceneOffsetStart >= list->sceneCount) {
                                sceneInfo.activeCategory++;
                                if (sceneInfo.activeCategory >= sceneInfo.categoryCount) {
                                    sceneInfo.activeCategory = 0;
                                }
                                list = &sceneInfo.listCategory[sceneInfo.activeCategory];
                                sceneInfo.listPos = 0;
                            }
                            sceneInfo.state = ENGINESTATE_LOAD;
                        }
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
                switch (engine.sdlEvents.key.keysym.sym) {
                    default: break;
                    case SDLK_BACKSPACE: engine.gameSpeed = 1; break;
                }
                break;
            case SDL_QUIT: return false;
        }
    }
#endif
    return true;
}

bool initRetroEngine()
{
    InitStorage();
    initUserData();
    
    SetUserFileCallbacks("", NULL, NULL);
#if RETRO_PLATFORM == RETRO_OSX
    char buffer[0xFF];
    sprintf(buffer, "%s/RSDKv5/", getResourcesPath());
    SetUserFileCallbacks(buffer, NULL, NULL);
#endif

    readSettings();
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

    return true;
}
void runRetroEngine()
{
    uint frameStart, frameEnd = SDL_GetTicks();
    int frameDelta = 0;

    while (engine.running) {
        frameStart = SDL_GetTicks();
        frameDelta = frameStart - frameEnd;

        if (frameDelta < 1000.0f / (float)engine.refreshRate)
            SDL_Delay((1000.0f / engine.refreshRate) - frameDelta);

        frameEnd = SDL_GetTicks();

        engine.running = processEvents();
        foreachStackPtr = foreachStackList;
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
                    userCore->SetupDebugValues();
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
                    // dim after 5 mins
                    engine.dimLimit = (5 * 60) * engine.refreshRate;
                    ProcessInput();
                    ProcessObjects();
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                    LoadAchievementAssets();
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
                ProcessAchievements();
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
                ProcessAchievements();
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
                ProcessAchievements();
#endif
                ProcessObjectDrawLists();
                break;
            case ENGINESTATE_LOAD | ENGINESTATE_STEPOVER:
                LoadScene();
                LoadSceneFile();
                InitObjects();
#if RETRO_REV02
                userCore->SetupDebugValues();
                for (int v = 0; v < DRAWLAYER_COUNT && v < DEBUGVAL_MAX; ++v) {
                    DebugValueInfo *val = &debugValues[debugValCnt++];
                    strncpy(val->name, drawGroupNames[v], 0x10);
                    val->type   = 0;
                    val->value      = &engine.drawLayerVisible[v];
                    val->valByteCnt = 4;
                    val->min   = 0;
                    val->max   = 1;
                }
#endif
                ProcessInput();
                ProcessObjects();
                sceneInfo.state = ENGINESTATE_REGULAR | ENGINESTATE_STEPOVER;
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                LoadAchievementAssets();
#endif
                break;
            case ENGINESTATE_REGULAR | ENGINESTATE_STEPOVER:
                ProcessInput();
                if (engine.frameStep) {
                    ProcessSceneTimer();
                    ProcessObjects();
                    ProcessParallaxAutoScroll();
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                    ProcessAchievements();
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
                    ProcessAchievements();
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
                    ProcessAchievements();
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
                if (ProcessVideo())
                    sceneInfo.state = engine.prevEngineMode;
                break;
            case ENGINESTATE_SHOWPNG: 
                if (engine.imageDelta <= 0.0 || engine.dimMax >= 1.0) {
                    if (engine.displayTime <= 0.0) {
                        engine.dimMax += engine.imageDelta;
                        if (engine.dimMax <= 0.0) {
                            engine.shaderID = engine.prevShaderID;
                            engine.screenCount  = 1;
                            sceneInfo.state     = engine.prevEngineMode;
                            engine.dimMax       = 1.0;
                        }
                    }
                    else {
                        engine.displayTime -= 0.0166666667f;
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
                int yOff      = DevOutput_GetStringYOffset(outputString);
                DrawRectangle(0, currentScreen->centerY - (yOff >> 1), currentScreen->width, yOff, 128, 255, INK_NONE, true);
                DrawDevText(8, outputString, currentScreen->centerY - (yOff >> 1) + 8, 0, 0xF0F0F0);
                break;
            }
            case ENGINESTATE_ERRORMSG_FATAL: {
                ProcessInput();
                currentScreen = &screens[0];
                if (controller[0].keyStart.down)
                    engine.running = false;
                int yOff = DevOutput_GetStringYOffset(outputString);
                DrawRectangle(0, currentScreen->centerY - (yOff >> 1), currentScreen->width, yOff, 0xF00000, 255, INK_NONE, true);
                DrawDevText(8, outputString, currentScreen->centerY - (yOff >> 1) + 8, 0, 0xF0F0F0);
                break;
            }
#endif
        }

        FlipScreen();
    }

    // Shutdown
    ReleaseAudioDevice();
    ReleaseRenderDevice();
    writeSettings(false);
    releaseUserData();
    ReleaseStorage();

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

void parseArguments(int argc, char *argv[])
{
    for (int a = 0; a < argc; ++a) {
        const char *find = "";

        find = strstr(argv[a], "stage=");
        if (find) {
            char buf[0x40];

            int b = 0;
            int c = 7;
            while (find[c] && find[c] != ';') {
                buf[b++] = find[c++];
            }
            buf[b] = 0;
        }

        find = strstr(argv[a], "scene=");
        if (find) {
            char buf[0x40];

            int b = 0;
            int c = 7;
            while (find[c] && find[c] != ';') {
                buf[b++] = find[c++];
            }
            buf[b] = 0;
        }

#if RETRO_REV02
        find = strstr(argv[a], "filter=");
        if (find) {
            char buf[0x10];

            int b = 0;
            int c = 7;
            while (argv[a][c] && find[c] != ';') {
                buf[b++] = find[c++];
            }
            buf[b]           = 0;
            sceneInfo.filter = atoi(buf);
        }
#endif

        find = strstr(argv[a], "console=true");
        if (find) {
            engine.printConsole = true;
            engine.devMenu      = true;
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
    setupFunctions();
    InitScriptSystem();
    LoadGameConfig();
}

void LoadGameConfig()
{
    FileInfo info;
    InitFileInfo(&info);

    if (LoadFile(&info, "Data/Game/GameConfig.bin", FMODE_RB)) {
        char buffer[0x100];
        uint sig = ReadInt32(&info, false);

        if (sig != 0x474643) {
            CloseFile(&info);
            return;
        }

        ReadString(&info, gameVerInfo.gameName);
        if (!useDataFile) {
            sprintf(gameVerInfo.gameName, "%s (Data Folder)", gameVerInfo.gameName);
        }
        ReadString(&info, gameVerInfo.gameSubName);
        ReadString(&info, gameVerInfo.gameVersion);

        sceneInfo.activeCategory = ReadInt8(&info);
        sceneInfo.listPos        = ReadInt16(&info);

        byte objCnt       = ReadInt8(&info);
        globalObjectCount = TYPE_DEFAULTCOUNT;
        for (int i = 0; i < objCnt; ++i) {
            ReadString(&info, hashBuffer);

            uint hash[4];
            GEN_HASH(hashBuffer, hash);

            if (objectCount > 0) {
                int objID                          = 0;
                globalObjectIDs[globalObjectCount] = 0;
                for (objID = 0; objID < objectCount; ++objID) {
                    if (HASH_MATCH(hash, objectList[objID].hash)) {
                        globalObjectIDs[globalObjectCount++] = objID;
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
                        globalPalette[i][(r << 4) + c] = bIndexes[blue] | gIndexes[green] | rIndexes[red];
                    }
                }
                else {
                    for (int c = 0; c < 0x10; ++c) {
                        globalPalette[i][(r << 4) + c] = 0;
                    }
                }
            }
        }

        byte sfxCnt = ReadInt8(&info);
        for (int i = 0; i < sfxCnt; ++i) {
            ReadString(&info, buffer);
            ReadInt8(&info);
        }

        ushort totalSceneCount = ReadInt16(&info);

        if (!totalSceneCount)
            totalSceneCount = 1;

        AllocateStorage(sizeof(SceneListEntry) * totalSceneCount, (void **)&sceneInfo.listData, DATASET_STG, false);

        sceneInfo.categoryCount = ReadInt8(&info);
        sceneInfo.listPos       = 0;

        int catSize = sceneInfo.categoryCount;
        if (!catSize)
            catSize = 1;
        AllocateStorage(sizeof(SceneListInfo) * catSize, (void **)&sceneInfo.listCategory, DATASET_STG, false);

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
                ReadString(&info, scene->sceneID);

#if RETRO_REV02
                scene->filter = ReadInt8(&info);
                if (scene->filter == 0x00)
                    scene->filter = 0xFF;
#endif
            }
            category->sceneOffsetEnd = category->sceneOffsetStart + category->sceneCount;
            sceneID += category->sceneCount;
        }

        byte cfmCount = ReadInt8(&info);
        for (int i = 0; i < cfmCount && gameOptionsPtr; ++i) {
            int offset = ReadInt32(&info, false);
            int count  = ReadInt32(&info, false);
            for (int v = 0; v < count; ++v) {
                ReadBytes(&info, &gameOptionsPtr[offset + v], sizeof(int));
            }
        }

        CloseFile(&info);
    }
}

void InitScriptSystem()
{
    RegisterObject((Object **)&DefaultObject, ":DefaultObject:", sizeof(EntityDefaultObject), sizeof(ObjectDefaultObject), DefaultObject_Update, NULL,
                 NULL, NULL, DefaultObject_Create, NULL, NULL, NULL, NULL);
#if RETRO_REV02
    RegisterObject((Object **)&DevOutput, ":DevOutput:", sizeof(EntityDevOutput), sizeof(ObjectDevOutput), DevOutput_Update, NULL, NULL, DevOutput_Draw,
                 DevOutput_Create, NULL, NULL, NULL, NULL);
#endif
    RegisterObject((Object **)&TestObject, ":TestObject:", sizeof(EntityTestObject), sizeof(ObjectTestObject), TestObject_Update,
                 TestObject_LateUpdate, TestObject_StaticUpdate, TestObject_Draw, TestObject_Create, TestObject_StageLoad, TestObject_EditorDraw,
                 TestObject_EditorLoad, TestObject_Serialize);
    globalObjectIDs[0] = 0;
#if RETRO_REV02
    globalObjectIDs[1] = 1;
    globalObjectIDs[2] = 2;
#else
    globalObjectIDs[1] = 1;
#endif

    globalObjectCount = TYPE_DEFAULTCOUNT;

    GameInfo info;

    info.functionPtrs = RSDKFunctionTable;
#if RETRO_REV02
    info.APIPtrs    = APIFunctionTable;
    info.currentSKU = &curSKU;
#endif
    info.engineInfo   = &gameVerInfo;
    info.sceneInfo    = &sceneInfo;
    info.controller   = controller;
    info.stickL       = stickL;
#if RETRO_REV02
    info.stickR       = stickR;
    info.triggerL     = triggerL;
    info.triggerR     = triggerR;
#endif
    info.touchMouse = &touchMouseData;
#if RETRO_REV02
    info.unknown = &unknownInfo;
#endif
    info.screenInfo   = screens;

    if (!engine.useExternalCode) {
        return linkGameLogic(&info);
    }

    bool32 linked = false;
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
    sprintf(buffer, "%s%s.dylib", userFileDir, gameLogicName);
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
#if RETRO_PLATFORM == RETRO_LINUX
    sprintf(gameLogicName, "%s.so", gameLogicName);
    if (!link_handle)
        link_handle = dlopen(gameLogicName, RTLD_LOCAL | RTLD_LAZY);

    if (link_handle) {
        linkPtr linkGameLogic = (linkPtr)dlsym(link_handle, "LinkGameLogicDLL");
        if (linkGameLogic) {
            linkGameLogic(&info);
            linked = true;
        }
    }
#endif
#if RETRO_PLATFORM == RETRO_ANDROID
    sprintf(gameLogicName, "/data/data/org.libsdl.app/files/Game.so", gameLogicName);
    //sprintf(gameLogicName, "%s", SDL_GetBasePath());
    printLog(PRINT_NORMAL, "Loading from %s", SDL_AndroidGetExternalStoragePath());
    if (!link_handle)
        link_handle = SDL_LoadObject(gameLogicName);
    printLog(PRINT_NORMAL, "Link handle? %s", SDL_GetError());

    if (link_handle) {
        linkPtr linkGameLogic = (linkPtr)dlsym(link_handle, "LinkGameLogicDLL");
        if (linkGameLogic) {
            linkGameLogic(&info);
            linked = true;
        }
    }
#endif

    if (!linked) {
        printLog(PRINT_POPUP, "Failed to link game logic!");
    }
}
