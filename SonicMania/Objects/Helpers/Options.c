// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Options Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectOptions *Options;

void Options_Update(void) {}

void Options_LateUpdate(void) {}

void Options_StaticUpdate(void) {}

void Options_Draw(void) {}

void Options_Create(void *data) {}

void Options_StageLoad(void)
{
#if !MANIA_USE_PLUS
    Options->changed = false;
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        Options_Reload();
    }
    else {
        OptionsRAM *options = Options_GetOptionsRAM();

        options->vSync           = false;
        options->windowed        = false;
        options->windowBorder    = false;
        options->tripleBuffering = false;
    }
#endif
}

OptionsRAM *Options_GetOptionsRAM(void) { return (OptionsRAM *)globals->optionsRAM; }

void Options_Reload(void)
{
    OptionsRAM *options = Options_GetOptionsRAM();

    options->overrideLanguage = true;

    options->screenShader    = RSDK.GetVideoSetting(VIDEOSETTING_SHADERID) % 4;
    options->volMusic        = RSDK.GetVideoSetting(VIDEOSETTING_STREAM_VOL);
    options->volSfx          = RSDK.GetVideoSetting(VIDEOSETTING_SFX_VOL);
    options->language        = RSDK.GetVideoSetting(VIDEOSETTING_LANGUAGE);
    options->vSync           = RSDK.GetVideoSetting(VIDEOSETTING_VSYNC);
    options->windowBorder    = RSDK.GetVideoSetting(VIDEOSETTING_BORDERED);
    options->windowed        = RSDK.GetVideoSetting(VIDEOSETTING_WINDOWED);
    options->tripleBuffering = RSDK.GetVideoSetting(VIDEOSETTING_TRIPLEBUFFERED);

    Options_GetWinSize();

    LogHelpers_Print("optionsPtr->screenShader = %d", options->screenShader);
    LogHelpers_Print("optionsPtr->volMusic = %d", options->volMusic);
    LogHelpers_Print("optionsPtr->volSfx = %d", options->volSfx);
    LogHelpers_Print("optionsPtr->language = %d", options->language);
    LogHelpers_Print("optionsPtr->overrideLanguage = %d", options->overrideLanguage);
    LogHelpers_Print("optionsPtr->vsync = %d", options->vSync);
    LogHelpers_Print("optionsPtr->tripleBuffering = %d", options->tripleBuffering);
    LogHelpers_Print("optionsPtr->windowBorder = %d", options->windowBorder);
    LogHelpers_Print("optionsPtr->windowed = %d", options->windowed);
    LogHelpers_Print("optionsPtr->windowSize = %d", options->windowSize);
}

void Options_GetWinSize(void)
{
    OptionsRAM *options = Options_GetOptionsRAM();

    bool32 windowed = RSDK.GetVideoSetting(VIDEOSETTING_WINDOWED);
    if (!windowed) {
        options->windowSize = 4;
    }
    else {
        int32 width = RSDK.GetVideoSetting(VIDEOSETTING_WINDOW_WIDTH);

        if (width <= WIDE_SCR_XSIZE)
            options->windowSize = 0;
        else if (width <= WIDE_SCR_XSIZE * 2)
            options->windowSize = 1;
        else if (width <= WIDE_SCR_XSIZE * 3)
            options->windowSize = 2;
        else
            options->windowSize = 3;
    }
}

void Options_LoadCallback(bool32 success)
{
    if (success) {
        Localization->loaded = false;

        Localization_LoadStrings();
        UIWidgets_ApplyLanguage();
        UIHeading_LoadSprites();
    }
}

void Options_LoadFile(void (*callback)(bool32 success))
{
    if (sku_platform != PLATFORM_PC && sku_platform != PLATFORM_DEV) {
        if (globals->optionsLoaded != STATUS_CONTINUE) {
            if (globals->optionsLoaded == STATUS_OK) {
                if (callback)
                    callback(true);
            }
            else {
                globals->optionsLoaded = STATUS_CONTINUE;
                Options->loadEntityPtr = SceneInfo->entity;
                Options->loadCallback  = callback;
                API_LoadUserFile("Options.bin", globals->optionsRAM, sizeof(globals->optionsRAM), Options_LoadOptionsCallback);
            }
        }
    }
    else {
        globals->optionsLoaded = STATUS_OK;
        if (callback)
            callback(true);
    }
}

#if MANIA_USE_PLUS
void Options_SaveFile(void (*callback)(bool32 success))
#else
void Options_SaveFile(void (*callback)(void))
#endif
{
    if (Options->changed) {
        if (sku_platform != PLATFORM_PC && sku_platform != PLATFORM_DEV) {
            if (globals->optionsLoaded == STATUS_OK) {
                Options->saveEntityPtr = SceneInfo->entity;
                Options->saveCallback  = callback;

#if MANIA_USE_PLUS
                API_SaveUserFile("Options.bin", globals->optionsRAM, sizeof(globals->optionsRAM), Options_SaveOptionsCallback, false);
#else
                API_SaveUserFile("Options.bin", globals->optionsRAM, sizeof(globals->optionsRAM), Options_SaveOptionsCallback);
#endif
            }
            else {
                Options->changed = false;
#if MANIA_USE_PLUS
                if (callback)
                    callback(false);
#else
                if (callback)
                    callback();
#endif
            }

            return;
        }
        else {
#if MANIA_USE_PLUS
            RSDK.SetVideoSetting(VIDEOSETTING_CHANGED, true);
#else
            APICallback_SaveSettingsINI();
#endif
            Options_Reload();
        }
    }

#if MANIA_USE_PLUS
    if (callback)
        callback(true);
#else
    if (callback)
        callback();
#endif
}

void Options_SetLanguage(int32 language)
{
    OptionsRAM *options = Options_GetOptionsRAM();

    if (language >= 0) {
        options->language         = language;
        options->overrideLanguage = true;
    }
    else {
        options->language         = -1;
        options->overrideLanguage = false;
    }

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV)
        RSDK.SetVideoSetting(VIDEOSETTING_LANGUAGE, language);

    Options->changed = true;
}

void Options_LoadValuesFromSettings(OptionsRAM *options)
{
    Localization->language = options->overrideLanguage ? options->language : sku_language;

    if (!options->overrideShader)
        options->screenShader = RSDK.GetVideoSetting(VIDEOSETTING_SHADERID) % 4;

    if (!options->overrideMusicVol)
        options->volMusic = RSDK.GetVideoSetting(VIDEOSETTING_STREAM_VOL);

    if (!options->overrideSfxVol)
        options->volSfx = RSDK.GetVideoSetting(VIDEOSETTING_SFX_VOL);
}

void Options_LoadOptionsCallback(int32 status)
{
    OptionsRAM *options = Options_GetOptionsRAM();
    bool32 success      = false;

#if MANIA_USE_PLUS
    if (status == STATUS_OK || status == STATUS_NOTFOUND) {
#else
    if (status) {
#endif
        success                = true;
        globals->optionsLoaded = STATUS_OK;

        LogHelpers_Print("dataPtr.language = %d", options->language);
        LogHelpers_Print("dataPtr.overrideLanguage = %d", options->overrideLanguage);
        Options_LoadValuesFromSettings(Options_GetOptionsRAM());

        RSDK.SetVideoSetting(VIDEOSETTING_SHADERID, options->screenShader);
        RSDK.SetVideoSetting(VIDEOSETTING_STREAM_VOL, options->volMusic);
        RSDK.SetVideoSetting(VIDEOSETTING_SFX_VOL, options->volSfx);
    }
    else {
        success                = false;
        globals->optionsLoaded = STATUS_ERROR;
    }

    if (Options->loadCallback) {
        Entity *entStore = SceneInfo->entity;
        if (Options->loadEntityPtr)
            SceneInfo->entity = Options->loadEntityPtr;
        Options->loadCallback(success);
        SceneInfo->entity = entStore;

        Options->loadCallback  = NULL;
        Options->loadEntityPtr = NULL;
    }
}

void Options_SaveOptionsCallback(int32 status)
{
    Options->changed = false;
    if (Options->saveCallback) {
        Entity *entStore = SceneInfo->entity;
        if (Options->saveEntityPtr)
            SceneInfo->entity = Options->saveEntityPtr;

#if MANIA_USE_PLUS
        Options->saveCallback(status == STATUS_OK);
#else
        Options->saveCallback();
#endif
        SceneInfo->entity = entStore;

        Options->saveCallback  = NULL;
        Options->saveEntityPtr = NULL;
    }
}

#if RETRO_INCLUDE_EDITOR
void Options_EditorDraw(void) {}

void Options_EditorLoad(void) {}
#endif

void Options_Serialize(void) {}
