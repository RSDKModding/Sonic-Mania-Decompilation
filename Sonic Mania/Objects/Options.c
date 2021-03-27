#include "../SonicMania.h"

#if RETRO_USE_PLUS
ObjectOptions *Options;

void Options_Reload()
{
    globals->optionsRAM[23] = RSDK.GetSettingsValue(SETTINGS_SHADERID) % 4;
    globals->optionsRAM[25] = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);
    globals->optionsRAM[27] = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);
    globals->optionsRAM[21] = RSDK.GetSettingsValue(SETTINGS_LANGUAGE);
    globals->optionsRAM[22] = true;
    globals->optionsRAM[29] = RSDK.GetSettingsValue(SETTINGS_VSYNC);
    globals->optionsRAM[31] = RSDK.GetSettingsValue(SETTINGS_BORDERED);
    globals->optionsRAM[32] = RSDK.GetSettingsValue(SETTINGS_WINDOWED);
    globals->optionsRAM[33] = RSDK.GetSettingsValue(SETTINGS_TRIPLEBUFFERED);
    Options_GetWinSize();
    Game_Print("optionsPtr->screenShader = %d", globals->optionsRAM[23]);
    Game_Print("optionsPtr->volMusic = %d", globals->optionsRAM[25]);
    Game_Print("optionsPtr->volSfx = %d", globals->optionsRAM[27]);
    Game_Print("optionsPtr->language = %d", globals->optionsRAM[21]);
    Game_Print("optionsPtr->overrideLanguage = %d", globals->optionsRAM[22]);
    Game_Print("optionsPtr->vsync = %d", globals->optionsRAM[29]);
    Game_Print("optionsPtr->tripleBuffering = %d", globals->optionsRAM[33]);
    Game_Print("optionsPtr->windowBorder = %d", globals->optionsRAM[31]);
    Game_Print("optionsPtr->windowed = %d", globals->optionsRAM[32]);
    Game_Print("optionsPtr->windowSize = %d", globals->optionsRAM[30]);
}

void Options_GetWinSize()
{
    int windowed = RSDK.GetSettingsValue(SETTINGS_WINDOWED);
    if (windowed == 2) {
        globals->optionsRAM[30] = 4;
    }
    else {
        int width = RSDK.GetSettingsValue(SETTINGS_WINDOW_WIDTH);
        if (width > 424) {
            if (width > 848) {
                globals->optionsRAM[30] = (width > 1272) + 2;
            }
            else {
                globals->optionsRAM[30] = 1;
            }
        }
        else {
            globals->optionsRAM[30] = 0;
        }
    }
}

void Options_LoadCallback(int success)
{
    if (success) {
        Localization->loaded = false;
        Localization_LoadStrings();
        UIWidgets_ApplyLanguage();
        UIHeading_LoadSprites();
    }
}

void Options_LoadOptionsBin()
{
    if (RSDK_sku->platform) {
        if (globals->optionsLoaded != 100) {
            if (globals->optionsLoaded == 200) {
                Localization->loaded = 0;
                Localization_LoadStrings();
                UIWidgets_ApplyLanguage();
                UIHeading_LoadSprites();
            }
            else {
                globals->optionsLoaded = 100;
                Options->loadEntityPtr = RSDK_sceneInfo->entity;
                Options->loadCallback  = Options_LoadCallback;
                User.LoadUserFile("Options.bin", globals->optionsRAM, 512, Options_LoadOptionsCallback);
            }
        }
    }
    else {
        globals->optionsLoaded = 200;
        Localization->loaded   = false;
        Localization_LoadStrings();
        UIWidgets_ApplyLanguage();
        UIHeading_LoadSprites();
    }
}

void Options_SaveOptionsBin(void (*callback)(int))
{
    if (Options->state) {
        if (RSDK_sku->platform && RSDK_sku->platform != PLATFORM_DEV) {
            if (globals->optionsLoaded == 200) {
                Options->saveEntityPtr = RSDK_sceneInfo->entity;
                Options->saveCallback  = callback;
                User.SaveUserFile("Options.bin", globals->optionsRAM, 512, Options_SaveOptionsCallback, 0);
            }
            else {
                Options->state = 0;
                if (callback)
                    callback(0);
            }
            return;
        }
        else {
            RSDK.SetSettingsValue(SETTINGS_SETTINGSCHANGED, 1);
            Options_Reload();
        }
    }
    if (callback)
        callback(1);
}

void Options_SetLanguage(int language)
{
    byte *optionsRAM = (byte *)&globals->optionsRAM[21];
    if (language >= 0) {
        optionsRAM[2]           = language;
        globals->optionsRAM[22] = 1;
    }
    else {
        optionsRAM[2]           = -1;
        globals->optionsRAM[22] = 0;
    }
    if (RSDK_sku->platform == PLATFORM_PC || RSDK_sku->platform == PLATFORM_DEV)
        RSDK.SetSettingsValue(SETTINGS_LANGUAGE, language);
    Options->state = 1;
}

void Options_Unknown1(int *optionsRAM)
{
    byte *langauagePtr = (byte *)&globals->optionsRAM[21];
    if (optionsRAM[22]) {
        Localization->language = langauagePtr[2];
    }
    else {
        langauagePtr[2] = RSDK_sku->language;
    }

    if (!optionsRAM[24]) {
        optionsRAM[23] = RSDK.GetSettingsValue(SETTINGS_SHADERID) % 4;
    }
    if (!optionsRAM[26]) {
        optionsRAM[25] = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);
    }
    if (!optionsRAM[28]) {
        optionsRAM[27] = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);
    }
}

void Options_LoadOptionsCallback(int statusCode)
{
    bool32 status = false;
    if (statusCode == 200 || statusCode == 404) {
        status                 = true;
        globals->optionsLoaded = 200;
        byte *optionsRAM       = (byte *)&globals->optionsRAM[21];
        Game_Print("dataPtr.language = %d", optionsRAM[2]);
        Game_Print("dataPtr.overrideLanguage = %d", globals->optionsRAM[22]);
        Options_Unknown1(globals->optionsRAM);
        RSDK.SetSettingsValue(SETTINGS_SHADERID, globals->optionsRAM[23]);
        RSDK.SetSettingsValue(SETTINGS_STREAM_VOL, globals->optionsRAM[25]);
        RSDK.SetSettingsValue(SETTINGS_SFX_VOL, globals->optionsRAM[27]);
    }
    else {
        status                 = false;
        globals->optionsLoaded = 500;
    }

    if (Options->loadCallback) {
        Entity *entStore = RSDK_sceneInfo->entity;
        if (Options->loadEntityPtr)
            RSDK_sceneInfo->entity = Options->loadEntityPtr;
        Options->loadCallback(status);
        RSDK_sceneInfo->entity = entStore;

        Options->loadCallback  = false;
        Options->loadEntityPtr = false;
    }
}

void Options_SaveOptionsCallback(int statusCode)
{
    Options->state = 0;
    if (Options->saveCallback) {
        Entity *entStore = RSDK_sceneInfo->entity;
        if (Options->saveEntityPtr)
            RSDK_sceneInfo->entity = Options->loadEntityPtr;
        // Options->saveCallback(status);
        RSDK_sceneInfo->entity = entStore;

        Options->saveCallback  = NULL;
        Options->saveEntityPtr = NULL;
    }
}

#endif