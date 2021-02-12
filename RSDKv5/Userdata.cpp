#include "RetroEngine.hpp"

bool settingsChanged = false;

int GetSettingsValue(int id)
{
    switch (id) {
        case 0: return !engine.isFullScreen; // windowed
        case 1: return !engine.borderless;   // bordered
        case 2: return engine.exclusiveFS;
        case 3: return engine.vsync;
        case 4: return engine.tripleBuffer;
        case 5: return engine.windowWidth;
        case 6: return engine.windowHeight;
        case 7: return engine.fsWidth;
        case 8: return engine.fsHeight;
        case 9: return engine.refreshRate;
        case 10: return engine.shaderSupport;
        case 11: return engine.screenShader;
        case 12: return engine.shaderUnknown;
        case 13: return engine.dimTimer;
        case 14: return engine.streamsEnabled;
        case 15: return (engine.streamVolume * 1024.0); break;
        case 16: return (engine.soundFXVolume * 1024.0); break;
        case 17: return engine.language; break;
        case 20: return settingsChanged; break;
        default: break;
    }
    return 0;
}

void SetSettingsValue(int id, int val)
{
    bool bVal = val;
    switch (id) {
        case 0:
            if (!engine.isFullScreen != bVal) {
                settingsChanged     = true;
                engine.isFullScreen = !bVal;
            }
            break;
        case 1:
            if (!engine.borderless != bVal) {
                settingsChanged   = true;
                engine.borderless = !bVal;
            }
            break;
        case 2:
            if (engine.exclusiveFS != bVal) {
                settingsChanged    = 1;
                engine.exclusiveFS = bVal;
            }
            break;
        case 3:
            if (engine.vsync != bVal) {
                settingsChanged = true;
                engine.vsync    = bVal;
            }
            break;
        case 4:
            if (engine.tripleBuffer != bVal) {
                settingsChanged     = true;
                engine.tripleBuffer = bVal;
            }
            break;
        case 5:
            if (engine.windowWidth != val) {
                engine.windowWidth = val;
                settingsChanged    = true;
            }
            break;
        case 6:
            if (engine.windowHeight != val) {
                engine.windowHeight = val;
                settingsChanged     = true;
            }
            break;
        case 7: engine.fsWidth = val; break;
        case 8: engine.fsHeight = val; break;
        case 9: engine.refreshRate = val; break;
        case 10: engine.shaderSupport = val; break;
        case 11:
            if (engine.screenShader != val) {
                engine.screenShader = val;
                settingsChanged      = true;
            }
            break;
        case 12: engine.shaderUnknown = val; break;
        case 13: engine.dimTimer = val; break;
        case 14:
            if (engine.streamsEnabled != bVal)
               settingsChanged = true;
            engine.streamsEnabled = bVal;
            break;
        case 15:
            if (engine.streamVolume != ((float)val * 0.0009765625)) {
                engine.streamVolume = val * 0.0009765625;
                settingsChanged     = true;
            }
            break;
        case 16:
            if (engine.soundFXVolume != ((float)val * 0.0009765625)) {
                engine.soundFXVolume = val * 0.0009765625;
                settingsChanged      = true;
            }
            break;
        case 17: engine.language = val; break;
        case 18:
            // windowSettings = WindowSettings;
            // gameSettings   = GameSettings;
            // shaderSettings = ShaderSettings;
            // screenSettings = ScreenSettings;
            // inputSettings  = InputSettings;
            break;
        case 19:
            settingsChanged = true;
            // WindowSettings = windowSettings;
            // GameSettings   = gameSettings;
            // ShaderSettings = shaderSettings;
            // ScreenSettings = screenSettings;
            // InputSettings  = inputSettings;
            break;
        case 20: settingsChanged = val; break;
        case 21: writeSettings(val); break;
        default: return;
    }
}

void readSettings()
{

    dictionary *ini = iniparser_load("Settings.ini");

    const char *result = "";

    engine.language = strtol(iniparser_getstring(ini, "Game:language", "0"), NULL, 0);

    result = iniparser_getstring(ini, "Game:dataFile", "Data.rsdk");
    if (CheckDataFile(result))
        engine.devMenu = iniparser_getboolean(ini, "Game:devMenu", false);
    else
        engine.devMenu = true;

    engine.startFullScreen = !iniparser_getboolean(ini, "Video:windowed", true);
    engine.borderless      = !iniparser_getboolean(ini, "Video:border", true);
    engine.exclusiveFS     = iniparser_getboolean(ini, "Video:exclusiveFS", false);
    engine.vsync           = iniparser_getboolean(ini, "Video:vsync", false);
    engine.tripleBuffer    = iniparser_getboolean(ini, "Video:tripleBuffering", false);

    result       = iniparser_getstring(ini, "Video:pixWidth", "424");
    SCREEN_XSIZE = strtol(result, NULL, 0);

    engine.windowWidth   = strtol(iniparser_getstring(ini, "Video:winWidth", "424"), NULL, 0);
    engine.windowHeight  = strtol(iniparser_getstring(ini, "Video:winHeight", "240"), NULL, 0);
    engine.fsWidth       = strtol(iniparser_getstring(ini, "Video:fsWidth", "0"), NULL, 0);
    engine.fsHeight      = strtol(iniparser_getstring(ini, "Video:fsHeight", "0"), NULL, 0);
    engine.refreshRate   = strtol(iniparser_getstring(ini, "Video:refreshRate", "60"), NULL, 0);
    engine.shaderSupport = iniparser_getboolean(ini, "Video:shaderSupport", true);
    engine.screenShader  = strtol(iniparser_getstring(ini, "Video:refreshRate", "0"), NULL, 0);

    engine.streamsEnabled = iniparser_getboolean(ini, "Audio:streamsEnabled", true);
    engine.streamVolume   = iniparser_getdouble(ini, "Audio:streamVolume", 1.0);
    engine.soundFXVolume  = iniparser_getdouble(ini, "Audio:sfxVolume", 1.0);

    int defKeyMaps[PLAYER_COUNT][12] = {
        { SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_A, SDL_SCANCODE_B, SDL_SCANCODE_UNKNOWN,
          SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_RETURN, SDL_SCANCODE_TAB },
        { SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_6, SDL_SCANCODE_J, SDL_SCANCODE_J, SDL_SCANCODE_UNKNOWN,
          SDL_SCANCODE_U, SDL_SCANCODE_I, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_LEFTBRACKET, SDL_SCANCODE_RIGHTBRACKET },
        { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
          SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN },
        { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
          SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN }
    };

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        char buffer[0x30];

        sprintf(buffer, "Keyboard Map %d:up", i);
        controller[i].keyUp.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][0]);
        sprintf(buffer, "Keyboard Map %d:dpwn", i);
        controller[i].keyDown.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][1]);
        sprintf(buffer, "Keyboard Map %d:left", i);
        controller[i].keyLeft.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][2]);
        sprintf(buffer, "Keyboard Map %d:right", i);
        controller[i].keyRight.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][3]);
        sprintf(buffer, "Keyboard Map %d:buttonA", i);
        controller[i].keyA.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][4]);
        sprintf(buffer, "Keyboard Map %d:buttonB", i);
        controller[i].keyB.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][5]);
        sprintf(buffer, "Keyboard Map %d:buttonC", i);
        controller[i].keyC.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][6]);
        sprintf(buffer, "Keyboard Map %d:buttonX", i);
        controller[i].keyX.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][7]);
        sprintf(buffer, "Keyboard Map %d:buttonY", i);
        controller[i].keyY.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][8]);
        sprintf(buffer, "Keyboard Map %d:buttonZ", i);
        controller[i].keyZ.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][9]);
        sprintf(buffer, "Keyboard Map %d:start", i);
        controller[i].keyStart.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][10]);
        sprintf(buffer, "Keyboard Map %d:select", i);
        controller[i].keySelect.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][11]);
    }
    iniparser_freedict(ini);
}
void writeSettings(bool writeToFile)
{

    if (settingsChanged || writeToFile) {
    }
}