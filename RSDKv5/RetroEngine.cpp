#include "Retroengine.hpp"

#if RETRO_PLATFORM == RETRO_WIN
#include "Windows.h"

HMODULE hLibModule = NULL;

typedef void(__cdecl *linkPtr)(GameInfo*);
#endif

int *gameOptionsPtr = NULL;
RetroEngine engine = RetroEngine();

bool processEvents()
{
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
            //case SDL_CONTROLLERDEVICEADDED: controllerInit(SDL_NumJoysticks() - 1); break;
            //case SDL_CONTROLLERDEVICEREMOVED: controllerClose(SDL_NumJoysticks() - 1); break;
            case SDL_WINDOWEVENT_CLOSE:
                if (engine.window) {
                    SDL_DestroyWindow(engine.window);
                    engine.window = NULL;
                }
                return false;
#endif

#ifdef RETRO_USING_MOUSE
            case SDL_MOUSEMOTION:
#if RETRO_USING_SDL2
                if (SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE)) <= 0) { // Touch always takes priority over mouse
#endif                                                                                     //! RETRO_USING_SDL2
                    SDL_GetMouseState(&touchX[0], &touchY[0]);
                    touchX[0] /= engine.windowScale;
                    touchY[0] /= engine.windowScale;
                    touches = 1;
#if RETRO_USING_SDL2
                }
#endif //! RETRO_USING_SDL2
                break;
            case SDL_MOUSEBUTTONDOWN:
#if RETRO_USING_SDL2
                if (SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE)) <= 0) { // Touch always takes priority over mouse
#endif                                                                                     //! RETRO_USING_SDL2
                    switch (engine.sdlEvents.button.button) {
                        case SDL_BUTTON_LEFT: touchDown[0] = 1; break;
                    }
                    touches = 1;
#if RETRO_USING_SDL2
                }
#endif //! RETRO_USING_SDL2
                break;
            case SDL_MOUSEBUTTONUP:
#if RETRO_USING_SDL2
                if (SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE)) <= 0) { // Touch always takes priority over mouse
#endif                                                                                     //! RETRO_USING_SDL2
                    switch (engine.sdlEvents.button.button) {
                        case SDL_BUTTON_LEFT: touchDown[0] = 0; break;
                    }
                    touches = 1;
#if RETRO_USING_SDL2
                }
#endif //! RETRO_USING_SDL2
                break;
#endif

#if defined(RETRO_USING_TOUCH) && defined(RETRO_USING_SDL2)
            case SDL_FINGERMOTION:
                touches = SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE));
                for (int i = 0; i < touches; i++) {
                    touchDown[i]       = true;
                    SDL_Finger *finger = SDL_GetTouchFinger(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE), i);
                    touchX[i]          = (finger->x * SCREEN_XSIZE * engine.windowScale) / engine.windowScale;

                    touchY[i] = (finger->y * SCREEN_YSIZE * engine.windowScale) / engine.windowScale;
                }
                break;
            case SDL_FINGERDOWN:
                touches = SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE));
                for (int i = 0; i < touches; i++) {
                    touchDown[i]       = true;
                    SDL_Finger *finger = SDL_GetTouchFinger(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE), i);
                    touchX[i]          = (finger->x * SCREEN_XSIZE * engine.windowScale) / engine.windowScale;

                    touchY[i] = (finger->y * SCREEN_YSIZE * engine.windowScale) / engine.windowScale;
                }
                break;
            case SDL_FINGERUP: touches = SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE)); break;
#endif //! RETRO_USING_SDL2

            case SDL_KEYDOWN:
                switch (engine.sdlEvents.key.keysym.sym) {
                    default: break;
                    case SDLK_ESCAPE:
                        if (engine.devMenu)
                            sceneInfo.state = ENGINESTATE_DEVMENU;
                        break;
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
                            SDL_SetWindowSize(engine.window, SCREEN_XSIZE * engine.windowScale, SCREEN_YSIZE * engine.windowScale);
                            SDL_SetWindowPosition(engine.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                            SDL_RestoreWindow(engine.window);
#endif
                        }
                        break;
                    case SDLK_F1:
                        if (engine.devMenu) {
                            activeSceneList   = 0;
                            sceneListPosition = 0;
                            sceneInfo.state = ENGINESTATE_LOAD;
                        }
                        break;
                    case SDLK_F2:
                        if (engine.devMenu) {
                            sceneListPosition--;
                            if (sceneListPosition < 0) {
                                activeSceneList--;

                                if (activeSceneList < 0) {
                                    activeSceneList = sceneListCount - 1;
                                }
                                sceneListPosition = sceneLists[activeSceneList].sceneCount - 1;
                            }
                            sceneInfo.state = ENGINESTATE_LOAD;
                        }
                        break;
                    case SDLK_F3:
                        if (engine.devMenu) {
                            sceneListPosition++;
                            if (sceneListPosition >= sceneLists[activeSceneList].sceneCount) {
                                activeSceneList++;

                                sceneListPosition = 0;
                                if (activeSceneList >= sceneListCount) {
                                    activeSceneList = 0;
                                }
                            }
                            sceneInfo.state       = ENGINESTATE_LOAD;
                        }
                        break;
                    case SDLK_F10:
                        if (engine.devMenu)
                            engine.showPaletteOverlay ^= 1;
                        break;
                    case SDLK_BACKSPACE:
                        if (engine.devMenu)
                            engine.gameSpeed = engine.fastForwardSpeed;
                        break;
#if RETRO_PLATFORM == RETRO_OSX
                    case SDLK_F6:
                        if (engine.masterPaused)
                            engine.frameStep = true;
                        break;
                    case SDLK_F7:
                        if (engine.devMenu)
                            engine.masterPaused ^= 1;
                        break;
#else
                    case SDLK_F11:
                        if (engine.masterPaused)
                            engine.frameStep = true;
                        break;
                    case SDLK_F12:
                        if (engine.devMenu)
                            engine.masterPaused ^= 1;
                        break;
#endif
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

void initRetroEngine() { 
    InitialiseUserStorage();

    CheckDataFile("Data.rsdk");
    //if (CheckDataFile("Data.rsdk"))
    //    DevMenuEnabled = Engine_GetSettingsBool(0) != 0;
    //else
    //    DevMenuEnabled = 1;

    startGameObjects(); 

    engine.running = true;
    if (!InitRenderDevice()) {
        engine.running = false;
        return;
    }
    if (!InitAudioDevice()) {
        engine.running = false;
        return;
    }
}
void runRetroEngine() {
    uint frameStart, frameEnd = SDL_GetTicks();
    float frameDelta = 0.0f;

    while (engine.running) {
        frameStart = SDL_GetTicks();
        frameDelta = frameStart - frameEnd;

        if (frameDelta < 1000.0f / (float)60)
            SDL_Delay(1000.0f / (float)60 - frameDelta);

        frameEnd = SDL_GetTicks();

        engine.running = processEvents();
        for (int f = 0; f < engine.gameSpeed && (!engine.masterPaused || engine.frameStep); ++f) {
            switch (sceneInfo.state) {
                default: break;
                case ENGINESTATE_LOAD:
                    LoadScene();
                    LoadSceneFile();
                    InitObjects();
                    for (int v = 0; v < 0x10 && v < DEBUGVAL_MAX; ++v) {
                        DebugValueInfo *val = &debugValues[debugValCnt++];
                        strncpy(val->name, drawGroupNames[v], 0x10);
                        val->field_14   = 0;
                        val->value      = &drawLayers[v].visible;
                        val->valByteCnt = 4;
                        val->unknown1   = 0;
                        val->unknown2   = 1;

                        MEM_ZERO(drawLayers[v]);
                        drawLayers[v].visible = true;
                    }
                    ProcessInput();
                    ProcessObjects();
                    break;
                case ENGINESTATE_REGULAR:
                    ProcessInput();
                    ProcessSceneTimer();
                    ProcessObjects();
                    ProcessParallaxAutoScroll();
                    for (int i = 1; i < engine.fastForwardSpeed; ++i) {
                        if (sceneInfo.state != ENGINESTATE_REGULAR)
                            break;
                        ProcessSceneTimer();
                        ProcessObjects();
                        ProcessParallaxAutoScroll();
                    }
                    ProcessObjectDrawLists();
                    break;
                case ENGINESTATE_PAUSED:
                    ProcessInput();
                    ProcessPausedObjects();
                    for (int i = 1; i < engine.fastForwardSpeed; ++i) {
                        if (sceneInfo.state != ENGINESTATE_PAUSED)
                            break;
                        ProcessPausedObjects();
                    }
                    ProcessObjectDrawLists();
                    break;
                case ENGINESTATE_FROZEN:
                    ProcessInput();
                    ProcessFrozenObjects();
                    for (int i = 1; i < engine.fastForwardSpeed; ++i) {
                        if (sceneInfo.state != ENGINESTATE_PAUSED)
                            break;
                        ProcessFrozenObjects();
                    }
                    ProcessObjectDrawLists();
                    break;
                case ENGINESTATE_LOAD_STEPOVER:
                    LoadScene();
                    LoadSceneFile();
                    InitObjects();
                    for (int v = 0; v < 0x10 && v < DEBUGVAL_MAX; ++v) {
                        DebugValueInfo *val = &debugValues[debugValCnt++];
                        strncpy(val->name, drawGroupNames[v], 0x10);
                        val->field_14   = 0;
                        val->value      = &drawLayers[v].visible;
                        val->valByteCnt = 4;
                        val->unknown1   = 0;
                        val->unknown2   = 1;

                        MEM_ZERO(drawLayers[v]);
                        drawLayers[v].visible = true;
                    }
                    ProcessInput();
                    ProcessObjects();
                    sceneInfo.state = ENGINESTATE_REGULAR_STEPOVER;
                    break;
                case ENGINESTATE_REGULAR_STEPOVER:
                    ProcessInput();
                    if (engine.frameStep) {
                        ProcessSceneTimer();
                        ProcessObjects();
                        ProcessParallaxAutoScroll();
                        ProcessObjectDrawLists();
                        engine.frameStep = false;
                    }
                    break;
                case ENGINESTATE_PAUSED_STEPOVER:
                    ProcessInput();
                    if (engine.frameStep) {
                        ProcessPausedObjects();
                        ProcessObjectDrawLists();
                        engine.frameStep = false;
                    }
                    break;
                case ENGINESTATE_FROZEN_STEPOVER:
                    ProcessInput();
                    if (engine.frameStep) {
                        ProcessFrozenObjects();
                        ProcessObjectDrawLists();
                        engine.frameStep = false;
                    }
                    break;
                case ENGINESTATE_DEVMENU:
                    ProcessInput();
                    //activeScreen = screens;
                    if (devMenu.state)
                        devMenu.state();
                    break;
                case ENGINESTATE_VIDEOPLAYBACK: break;
                case ENGINESTATE_SHOWPNG: break;
                case ENGINESTATE_ERRORMSG: break;
                case ENGINESTATE_ERRORMSG_FATAL: break;
            }
        }
    }

    ReleaseAudioDevice();
    ReleaseRenderDevice();
    //writeSettings();
    ReleaseUserStorage();

#if RETRO_USING_SDL1 || RETRO_USING_SDL2
    SDL_Quit();
#endif

    //Shutdown
#if RETRO_PLATFORM == RETRO_WIN
    if (hLibModule) {
        FreeLibrary(hLibModule);
        hLibModule = NULL;
    }
#endif
}

void parseArguments(int argc, char *argv[])
{
    for (int a = 0; a < argc; ++a) {

        if (StrComp(argv[a], "stage=")) {
            char buf[0x40];

            int b = 0;
            int c = 7;
            while (argv[a][c] && argv[a][c] != ';') {
                buf[b++] = argv[a][c++];
            }
            buf[b] = 0;
        }
        if (StrComp(argv[a], "scene=")) {
            char buf[0x40];

            int b = 0;
            int c = 7;
            while (argv[a][c] && argv[a][c] != ';') {
                buf[b++] = argv[a][c++];
            }
            buf[b] = 0;
        }
        if (StrComp(argv[a], "filter=")) {
            char buf[0x10];

            int b = 0;
            int c = 7;
            while (argv[a][c] && argv[a][c] != ';') {
                buf[b++] = argv[a][c++];
            }
            buf[b]           = 0;
            sceneInfo.filter = atoi(buf);
        }

        if (StrComp(argv[a], "console=true")) {
            engine.printConsole = true;
            engine.devMenu      = true;
        }
    }
}

void startGameObjects() {
    memset(&objectList, 0, OBJECT_COUNT * sizeof(ObjectInfo));
    sceneInfo.classCount     = 0;
    sceneInfo.activeCategory = 0;
    sceneInfo.listPos        = 0;
    sceneInfo.state          = 0;
    sceneInfo.inEditor       = 0;
    sceneInfo.debugMode      = engine.devMenu;
    devMenu.state = DevMenu_MainMenu;
    InitScriptSystem();
    LoadGameConfig();
}

void LoadGameConfig()
{
    FileInfo info;
    MEM_ZERO(info);

    if (LoadFile(&info, "Data/Game/GameConfig.bin")) {
        char buffer[0x100];
        uint sig = ReadInt32(&info);

        if (sig != 0x474643) {
            CloseFile(&info);
            return;
        }

        ReadString(&info, engine.gameName);
        if (!useDataFile) {
            StrAdd(engine.gameName, " (Using Data Folder)");
        }
        ReadString(&info, engine.gameSubName);
        ReadString(&info, engine.gameVersion);

        sceneInfo.activeCategory = ReadInt8(&info);
        sceneInfo.listPos        = ReadInt16(&info);

        byte objCnt = ReadInt8(&info);
        for (int i = 0; i < objCnt; ++i) {
            ReadString(&info, hashBuffer);

            uint hash[4];
            GenerateHash(hash, StrLength(hashBuffer));

            if (objectCount > 0) {
                int objID                          = 0;
                globalObjectIDs[globalObjectCount] = 0;
                do {
                    if (hash[0] == objectList[objID].hash[0] && hash[1] == objectList[objID].hash[1] && hash[2] == objectList[objID].hash[2]
                        && hash[3] == objectList[objID].hash[3]) {
                        globalObjectIDs[globalObjectCount++] = objID;
                    }
                    ++objID;
                } while (objID < objectCount);
            }
        }

        for (int i = 0; i < PALETTE_COUNT; ++i) {
            ushort activeRows = ReadInt16(&info);
            for (int r = 0; r < 0x10; ++r) {
                if ((activeRows >> r & 1)) {
                    for (int c = 0; c < 0x10; ++c) {
                        byte red        = ReadInt8(&info);
                        byte green      = ReadInt8(&info);
                        byte blue       = ReadInt8(&info);
                        fullPalette[i][(r << 4) + c] = bIndexes[buffer[2]] | gIndexes[buffer[1]] | rIndexes[buffer[0]];
                    }
                }
                else {
                    for (int c = 0; c < 0x10; ++c) {
                        fullPalette[i][(r << 4) + c] = bIndexes[0x00] | gIndexes[0x00] | rIndexes[0x00];
                    }
                }
            }
        }

        byte sfxCnt = ReadInt8(&info);
        for (int i = 0; i < sfxCnt; ++i) {
            ReadString(&info, buffer);
            byte maxConcurrentPlays = ReadInt8(&info);
            //LoadWAV(buffer, maxConcurrentPlays, SCOPE_GLOBAL);
        }

        sceneCount = ReadInt16(&info);
        sceneInfo.categoryCount = ReadInt8(&info);

        int sceneID = 0;
        for (int i = 0; i < sceneInfo.categoryCount; ++i) {
            ReadString(&info, hashBuffer);
            GenerateHash(sceneLists[i].hash, StrLength(hashBuffer));

            sceneLists[i].sceneOffset = sceneID;
            sceneLists[i].sceneCount = ReadInt8(&info);
            for (int s = 0; s < sceneLists[i].sceneCount; ++s) {
                ReadString(&info, hashBuffer);
                GenerateHash(sceneListEntries[sceneID + s].nameHash, StrLength(hashBuffer));

                ReadString(&info, sceneListEntries[sceneID + s].folder);
                ReadString(&info, sceneListEntries[sceneID + s].sceneID);

                sceneListEntries[sceneID + s].modeFilter = ReadInt8(&info);
            }
            sceneID += sceneLists[i].sceneCount;
        }

        byte cfmCount = ReadInt8(&info);
        for (int i = 0; i < cfmCount && gameOptionsPtr; ++i) {
            int offset = ReadInt32(&info);
            int count  = ReadInt32(&info);
            for (int v = 0; v < count; ++v) {
                gameOptionsPtr[offset + v] = ReadInt32(&info);
            }
        }

        CloseFile(&info);
    }
}

void InitScriptSystem()
{
    setupFunctions();

    CreateObject((Object**)&DefaultObject, ":DefaultObject:", sizeof(EntityDefaultObject), sizeof(ObjectDefaultObject), DefaultObject_Update, NULL, NULL, NULL,
                 DefaultObject_Create, NULL, NULL, NULL, NULL);
    CreateObject((Object **)&DevOutput, ":DevOutput:", sizeof(EntityDevOutput), sizeof(ObjectDevOutput), DevOutput_Update, NULL, NULL, DevOutput_Draw,
                 DevOutput_Create, NULL, NULL, NULL, NULL);
    globalObjectIDs[0] = 0;
    globalObjectIDs[1] = 1;

    globalObjectCount = 2;

    GameInfo info;

    info.functionPtrs      = functionTable;
    info.userdataPtrs      = userDataTable;
    info.gameName          = engine.gameName;
    info.currentSKU        = &curSKU;
    info.sceneInfo         = &sceneInfo;
    info.activeDPad        = NULL; //(GameInput *)&Key_Up;
    info.activeAnalogStick = NULL; //(GameInput *)&Engine_AnalogStickP1;
    info.unknown1          = NULL; //&Engine_AnalogStickP2;
    info.unknown2          = NULL; //&Engine_AnalogStickP3;
    info.unknown3          = NULL; //&Engine_AnalogStickP4;
    info.mousePos          = NULL; //&Engine_MousePos;
    info.inputCount        = NULL; //(int *)&Engine_InputCount;
    info.screenInfo        = screens;

    if (!engine.useExternalCode) {
        //return linkGameLogic(&info);
    }

#if RETRO_PLATFORM == RETRO_WIN
    if (!hLibModule)
        hLibModule = LoadLibraryA("Game");

    if (hLibModule) {
        linkPtr linkGameLogic = (linkPtr)GetProcAddress(hLibModule, "LinkGameLogicDLL");
        if (linkGameLogic)
            linkGameLogic(&info);
    }
#endif
}