#include "RetroEngine.hpp"

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

int *globalVarsPtr = NULL;
RetroEngine engine = RetroEngine();

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
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        // TODO: I know v5 does stuff here
                        break;
                }
                break;
            case SDL_CONTROLLERDEVICEADDED: controllerInit(engine.sdlEvents.cdevice.which); break;
            case SDL_CONTROLLERDEVICEREMOVED: controllerClose(engine.sdlEvents.cdevice.which); break;
            case SDL_APP_WILLENTERBACKGROUND:
                // TODO: I know v5 does stuff here
                break;
            case SDL_APP_TERMINATING: return false;
#endif

#ifdef RETRO_USING_MOUSE
            case SDL_MOUSEBUTTONDOWN:
#if RETRO_USING_SDL2
                switch (engine.sdlEvents.button.button) {
                    case SDL_BUTTON_LEFT:
                        touchMouseData.down[0] = true;
                        touchMouseData.count   = 1;
                        break;
                }
#endif //! RETRO_USING_SDL2
                break;
            case SDL_MOUSEBUTTONUP:
#if RETRO_USING_SDL2
                switch (engine.sdlEvents.button.button) {
                    case SDL_BUTTON_LEFT:
                        touchMouseData.down[0] = false;
                        touchMouseData.count   = 0;
                        break;
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
                        break;
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
#if RETRO_PLATFORM == RETRO_ANDROID
    sleep(1); // wait to initialize the engine
#endif

    InitStorage();

#if RETRO_PLATFORM == RETRO_OSX
    char buffer[0x100];
    sprintf(buffer, "%s/RSDKv5/", getResourcesPath());
    SetUserFileCallbacks(buffer, NULL, NULL);
#elif RETRO_PLATFORM == RETRO_ANDROID
    char buffer[0x200];

    JNIEnv *env      = (JNIEnv *)SDL_AndroidGetJNIEnv();
    jobject activity = (jobject)SDL_AndroidGetActivity();
    jclass cls(env->GetObjectClass(activity));
    jmethodID method = env->GetMethodID(cls, "getBasePath", "()Ljava/lang/String;");
    auto ret         = env->CallObjectMethod(activity, method);

    strcpy(buffer, env->GetStringUTFChars((jstring)ret, NULL));

    SetUserFileCallbacks(buffer, NULL, NULL);

    env->DeleteLocalRef(activity);
    env->DeleteLocalRef(cls);
#else
    SetUserFileCallbacks("", NULL, NULL);
#endif

    initUserData();

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

#if RETRO_USE_MOD_LOADER
    // we confirmed the game actually is valid, lets start some callbacks
    RunModCallbacks(MODCB_GAME_STARTUP, NULL);
#endif

    return true;
}
void runRetroEngine()
{
    const Uint64 frequency = SDL_GetPerformanceFrequency();
    Uint64 frameStart = SDL_GetPerformanceCounter(), frameEnd = SDL_GetPerformanceCounter();
    float frameDelta = 0.0f;

    while (engine.running) {
#if !RETRO_USE_ORIGINAL_CODE
        frameStart = SDL_GetPerformanceCounter();
        frameDelta = frameStart - frameEnd;
        if (frameDelta < frequency / (float)engine.refreshRate) {
            continue;
        }
        frameEnd = SDL_GetPerformanceCounter();
#endif

#if RETRO_USE_MOD_LOADER
        if (sceneInfo.state != ENGINESTATE_DEVMENU && devMenu.modsChanged) {
            devMenu.modsChanged = false;
            saveMods();
            for (int c = 0; c < CHANNEL_COUNT; ++c) {
                StopChannel(c);
            }
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
            SDL_SetWindowTitle(engine.window, gameVerInfo.gameName);
#endif
            LoadGlobalSFX();

            sceneInfo.state = ENGINESTATE_LOAD;
        }
#endif

        engine.running  = processEvents();
        foreachStackPtr = foreachStackList;
#if !RETRO_USE_ORIGINAL_CODE
        debugHitboxCount = 0;
#endif
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
                    for (int v = 0; v < DRAWLAYER_COUNT; ++v) {
                        SetDebugValue(drawGroupNames[v], &engine.drawLayerVisible[v], DTYPE_BOOL, false, true);
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

#if !RETRO_USE_ORIGINAL_CODE
        for (int t = 0; t < touchMouseData.count; ++t) {
            if (touchMouseData.down[t]) {
                int tx = touchMouseData.x[t] * screens->width;
                int ty = touchMouseData.y[t] * screens->height;

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

        FlipScreen();
    }

    // Shutdown
    ReleaseAudioDevice();
    ReleaseRenderDevice();
    writeSettings(false);
    releaseUserData();
    ReleaseStorage();
    unloadMods();

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
int getXMLAttributeValueInt(const tinyxml2::XMLAttribute *attributePtr) { return attributePtr->IntValue(); }
bool getXMLAttributeValueBool(const tinyxml2::XMLAttribute *attributePtr) { return attributePtr->BoolValue(); }
const char *getXMLAttributeValueString(const tinyxml2::XMLAttribute *attributePtr) { return attributePtr->Value(); }

void LoadXMLObjects()
{
    FileInfo info;

    for (int m = 0; m < (int)modList.size(); ++m) {
        if (!modList[m].active)
            continue;

        setActiveMod(m);
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
                printLog(PRINT_NORMAL, "Failed to parse Game.xml File!");
            }

            delete[] xmlData;
            delete doc;
        }
    }
    setActiveMod(-1);
}
void LoadXMLSoundFX()
{
    FileInfo info;
    FileInfo infoStore;
    for (int m = 0; m < (int)modList.size(); ++m) {
        if (!modList[m].active)
            continue;

        setActiveMod(m);
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
                            int maxConcurrentPlays                         = 0;
                            if (playsAttr)
                                maxConcurrentPlays = getXMLAttributeValueInt(playsAttr);

                            LoadSfx((char *)sfxPath, maxConcurrentPlays, SCOPE_GLOBAL);

                        } while ((sfxElement = nextXMLSiblingElement(doc, sfxElement, "soundfx")));
                    }
                }
            }
            else {
                printLog(PRINT_NORMAL, "Failed to parse Game.xml File!");
            }

            delete[] xmlData;
            delete doc;

        }
    }
    setActiveMod(-1);
}
int LoadXMLStages(int mode, int gcListCount, int gcStageCount)
{
    FileInfo info;
    int listCount = 0;
    int stageCount = 0;


    for (int m = 0; m < (int)modList.size(); ++m) {
        if (!modList[m].active)
            continue;

        setActiveMod(m);
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

                            sprintf(list->name, lstName);
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

                                    sprintf(scene->name, stgName);
                                    GEN_HASH(scene->name, scene->hash);
                                    sprintf(scene->folder, stgFolder);
                                    sprintf(scene->sceneID, stgID);

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
                printLog(PRINT_NORMAL, "Failed to parse Game.xml File!");
            }

            delete[] xmlData;
            delete doc;
        }
    }
    setActiveMod(-1);

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
    int gcListCount = 0;
    int gcSceneCount = 0;

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

            RETRO_HASH(hash);
            GEN_HASH(hashBuffer, hash);

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
                        globalPalette[i][(r << 4) + c] = bIndexes[blue] | gIndexes[green] | rIndexes[red];
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

        AllocateStorage(sizeof(SceneListEntry) * totalSceneCount, (void **)&sceneInfo.listData, DATASET_STG, false);

        sceneInfo.categoryCount = ReadInt8(&info);
        sceneInfo.listPos       = 0;

        int categoryCount = sceneInfo.categoryCount;
#if RETRO_USE_MOD_LOADER
        gcListCount = categoryCount;
        categoryCount += LoadXMLStages(1, 0, 0);
#endif
        if (!categoryCount)
            categoryCount = 1;

        AllocateStorage(sizeof(SceneListInfo) * categoryCount, (void **)&sceneInfo.listCategory, DATASET_STG, false);

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
    }
}

void InitScriptSystem()
{
#if RETRO_USE_MOD_LOADER
    objectCount = 0;
    memset(globalObjectIDs, 0, sizeof(int) * OBJECT_COUNT);
    memset(objectEntityList, 0, sizeof(EntityBase) * ENTITY_COUNT);
    editableVarCount = 0;
    foreachStackPtr  = foreachStackList;
    currentMod = NULL;
#endif
    RegisterObject((Object **)&DefaultObject, ":DefaultObject:", sizeof(EntityDefaultObject), sizeof(ObjectDefaultObject), DefaultObject_Update,
                   DefaultObject_LateUpdate, DefaultObject_StaticUpdate, DefaultObject_Draw, DefaultObject_Create, DefaultObject_StageLoad,
                   DefaultObject_EditorDraw, DefaultObject_EditorLoad, DefaultObject_Serialize);
#if RETRO_REV02
    RegisterObject((Object **)&DevOutput, ":DevOutput:", sizeof(EntityDevOutput), sizeof(ObjectDevOutput), DevOutput_Update, DevOutput_LateUpdate,
                   DevOutput_StaticUpdate, DevOutput_Draw, DevOutput_Create, DevOutput_StageLoad, DevOutput_EditorDraw, DevOutput_EditorLoad,
                   DevOutput_Serialize);
#endif
    globalObjectIDs[0] = 0;

#if RETRO_REV02
    globalObjectIDs[1] = 1;
#endif

    globalObjectCount = TYPE_DEFAULTCOUNT;

    GameInfo info;

    info.functionPtrs = RSDKFunctionTable;
#if RETRO_REV02
    info.APIPtrs    = APIFunctionTable;
    info.currentSKU = &curSKU;
#endif
    info.engineInfo = &gameVerInfo;
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
    info.unknown = &unknownInfo;
#endif
    info.screenInfo = screens;
#if RETRO_USE_MOD_LOADER
    info.modPtrs = modFunctionTable;
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
#if RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
        sprintf(gameLogicName, "%s%s.so", userFileDir, gameLogicName);
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

        if (!linked) {
            printLog(PRINT_POPUP, "Failed to link game logic!");
        }
    }
    else {
        linkGameLogic(&info);
    }

#if RETRO_USE_MOD_LOADER
    for (int m = 0; m < modList.size(); ++m) {
        if (!modList[m].active || modList[m].language)
            continue;
        currentMod = &modList[m];
        for (modLinkSTD ptr : modList[m].linkModLogic) {
            if (!ptr(&info, modList[m].folder.c_str())) {
                modList[m].active = false;
                printLog(PRINT_ERROR, "[MOD] Failed to link logic for mod %s!", modList[m].folder.c_str());
            }
        }
    }
    currentMod = NULL;
    sortMods();
#endif
}