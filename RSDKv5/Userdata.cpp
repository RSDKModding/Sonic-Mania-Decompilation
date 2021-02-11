#include "RetroEngine.hpp"

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
    engine.tripleBuffer  = iniparser_getboolean(ini, "Video:tripleBuffering", false);

    result       = iniparser_getstring(ini, "Video:pixWidth", "424");
    SCREEN_XSIZE = strtol(result, NULL, 0);

    engine.windowWidth      = strtol(iniparser_getstring(ini, "Video:winWidth", "424"), NULL, 0);
    engine.windowHeight     = strtol(iniparser_getstring(ini, "Video:winHeight", "240"), NULL, 0);
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
void writeSettings() {}