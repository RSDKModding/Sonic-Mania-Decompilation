#include "SonicMania.h"

ObjectOptions *Options;

void Options_Update(void) {}
void Options_LateUpdate(void) {}
void Options_StaticUpdate(void) {}
void Options_Draw(void) {}
void Options_Create(void *data) {}
void Options_StageLoad(void)
{
#if !RETRO_USE_PLUS
    Options->state = 0;
    if (RSDK_info->platform == PLATFORM_DEV || RSDK_info->platform == PLATFORM_PC) {
        Options_Reload();
    }
    else {
        globals->optionsRAM[29] = 0;
        globals->optionsRAM[32] = 0;
        globals->optionsRAM[31] = 0;
        globals->optionsRAM[33] = 0;
    }
#endif
}
void Options_EditorDraw(void) {}
void Options_EditorLoad(void) {}
void Options_Serialize(void) {}

void Options_Reload(void)
{
    globals->optionsRAM[23] = RSDK.GetSettingsValue(SETTINGS_SHADERID) % 4;
    globals->optionsRAM[25] = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);
    globals->optionsRAM[27] = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);
    globals->optionsRAM[21] = RSDK.GetSettingsValue(SETTINGS_LANGUAGE) << 16;
    globals->optionsRAM[22] = true;
    globals->optionsRAM[29] = RSDK.GetSettingsValue(SETTINGS_VSYNC);
    globals->optionsRAM[31] = RSDK.GetSettingsValue(SETTINGS_BORDERED);
    globals->optionsRAM[32] = RSDK.GetSettingsValue(SETTINGS_WINDOWED);
    globals->optionsRAM[33] = RSDK.GetSettingsValue(SETTINGS_TRIPLEBUFFERED);
    Options_GetWinSize();
    LogHelpers_Print("optionsPtr->screenShader = %d", globals->optionsRAM[23]);
    LogHelpers_Print("optionsPtr->volMusic = %d", globals->optionsRAM[25]);
    LogHelpers_Print("optionsPtr->volSfx = %d", globals->optionsRAM[27]);
    LogHelpers_Print("optionsPtr->language = %d", (globals->optionsRAM[21] >> 16) & 0xFF);
    LogHelpers_Print("optionsPtr->overrideLanguage = %d", globals->optionsRAM[22]);
    LogHelpers_Print("optionsPtr->vsync = %d", globals->optionsRAM[29]);
    LogHelpers_Print("optionsPtr->tripleBuffering = %d", globals->optionsRAM[33]);
    LogHelpers_Print("optionsPtr->windowBorder = %d", globals->optionsRAM[31]);
    LogHelpers_Print("optionsPtr->windowed = %d", globals->optionsRAM[32]);
    LogHelpers_Print("optionsPtr->windowSize = %d", globals->optionsRAM[30] & 0xFF);
}

void Options_GetWinSize(void)
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

void Options_LoadOptionsBin(void)
{
#if RETRO_USE_PLUS
    if (RSDK_sku->platform && RSDK_sku->platform != PLATFORM_DEV) {
#else
    if (RSDK_info->platform && RSDK_info->platform != PLATFORM_DEV) {
#endif
        if (globals->optionsLoaded != STATUS_CONTINUE) {
            if (globals->optionsLoaded == STATUS_OK) {
                Localization->loaded = 0;
                Localization_LoadStrings();
                UIWidgets_ApplyLanguage();
                UIHeading_LoadSprites();
            }
            else {
                globals->optionsLoaded = STATUS_CONTINUE;
                Options->loadEntityPtr = RSDK_sceneInfo->entity;
                Options->loadCallback  = Options_LoadCallback;
#if RETRO_USE_PLUS
                API.LoadUserFile("Options.bin", globals->optionsRAM, 0x200, Options_LoadOptionsCallback);
#else
                APICallback_LoadUserFile(globals->optionsRAM, "Options.bin", 0x200, Options_LoadOptionsCallback);
#endif
            }
        }
    }
    else {
        globals->optionsLoaded = STATUS_OK;
        Localization->loaded   = false;
        Localization_LoadStrings();
        UIWidgets_ApplyLanguage();
        UIHeading_LoadSprites();
    }
}

void Options_SaveOptionsBin(void (*callback)(int))
{
    if (Options->state) {
#if RETRO_USE_PLUS
        if (RSDK_sku->platform && RSDK_sku->platform != PLATFORM_DEV) {
#else
        if (RSDK_info->platform && RSDK_info->platform != PLATFORM_DEV) {
#endif
            if (globals->optionsLoaded == STATUS_OK) {
                Options->saveEntityPtr = RSDK_sceneInfo->entity;
                Options->saveCallback  = callback;
#if RETRO_USE_PLUS
                API.SaveUserFile("Options.bin", globals->optionsRAM, 0x200, Options_SaveOptionsCallback, 0);
#else
                APICallback_SaveUserFile(globals->optionsRAM, "Options.bin", 0x200, Options_SaveOptionsCallback);
#endif
            }
            else {
                Options->state = 0;
                if (callback)
                    callback(false);
            }
            return;
        }
        else {
            RSDK.SetSettingsValue(SETTINGS_CHANGED, true);
            Options_Reload();
        }
    }
    if (callback)
        callback(true);
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
#if RETRO_USE_PLUS
    if (RSDK_sku->platform == PLATFORM_PC || RSDK_sku->platform == PLATFORM_DEV)
#else
    if (RSDK_info->platform == PLATFORM_PC || RSDK_info->platform == PLATFORM_DEV)
#endif
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
#if RETRO_USE_PLUS
        langauagePtr[2] = RSDK_sku->language;
#else
        langauagePtr[2] = RSDK_info->language;
#endif
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
    if (statusCode == STATUS_OK || statusCode == STATUS_NOTFOUND) {
        status                 = true;
        globals->optionsLoaded = STATUS_OK;
        byte *optionsRAM       = (byte *)&globals->optionsRAM[21];
        LogHelpers_Print("dataPtr.language = %d", optionsRAM[2]);
        LogHelpers_Print("dataPtr.overrideLanguage = %d", globals->optionsRAM[22]);
        Options_Unknown1(globals->optionsRAM);
        RSDK.SetSettingsValue(SETTINGS_SHADERID, globals->optionsRAM[23]);
        RSDK.SetSettingsValue(SETTINGS_STREAM_VOL, globals->optionsRAM[25]);
        RSDK.SetSettingsValue(SETTINGS_SFX_VOL, globals->optionsRAM[27]);
    }
    else {
        status                 = false;
        globals->optionsLoaded = STATUS_ERROR;
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
            RSDK_sceneInfo->entity = Options->saveEntityPtr;
        Options->saveCallback(statusCode);
        RSDK_sceneInfo->entity = entStore;

        Options->saveCallback  = NULL;
        Options->saveEntityPtr = NULL;
    }
}
