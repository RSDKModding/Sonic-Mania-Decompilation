// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Options Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

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
    if (GameInfo->platform == PLATFORM_DEV || GameInfo->platform == PLATFORM_PC) {
        Options_Reload();
    }
    else {
        EntityOptions *options   = (EntityOptions *)globals->optionsRAM;
        options->vSync           = false;
        options->windowed        = false;
        options->windowBorder    = false;
        options->tripleBuffering = false;
    }
#endif
}

void Options_Reload(void)
{
    EntityOptions *options    = (EntityOptions *)globals->optionsRAM;
    options->screenShader     = RSDK.GetSettingsValue(SETTINGS_SHADERID) % 4;
    options->volMusic         = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);
    options->volSfx           = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);
    options->language         = RSDK.GetSettingsValue(SETTINGS_LANGUAGE);
    options->overrideLanguage = true;
    options->vSync            = RSDK.GetSettingsValue(SETTINGS_VSYNC);
    options->windowBorder     = RSDK.GetSettingsValue(SETTINGS_BORDERED);
    options->windowed         = RSDK.GetSettingsValue(SETTINGS_WINDOWED);
    options->tripleBuffering  = RSDK.GetSettingsValue(SETTINGS_TRIPLEBUFFERED);
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
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;
    int32 windowed           = RSDK.GetSettingsValue(SETTINGS_WINDOWED);
    if (!windowed) {
        options->windowSize = 4;
    }
    else {
        int32 width = RSDK.GetSettingsValue(SETTINGS_WINDOW_WIDTH);
        if (width > WIDE_SCR_XSIZE) {
            if (width > (WIDE_SCR_XSIZE * 2)) {
                options->windowSize = (width > (WIDE_SCR_XSIZE * 3)) + 2;
            }
            else {
                options->windowSize = 1;
            }
        }
        else {
            options->windowSize = 0;
        }
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

void Options_LoadOptionsBin(void)
{
    if (sku_platform && sku_platform != PLATFORM_DEV) {
        if (globals->optionsLoaded != STATUS_CONTINUE) {
            if (globals->optionsLoaded == STATUS_OK) {
                Localization->loaded = 0;
                Localization_LoadStrings();
                UIWidgets_ApplyLanguage();
                UIHeading_LoadSprites();
            }
            else {
                globals->optionsLoaded = STATUS_CONTINUE;
                Options->loadEntityPtr = SceneInfo->entity;
                Options->loadCallback  = Options_LoadCallback;
                API_LoadUserFile("Options.bin", globals->optionsRAM, 0x200, Options_LoadOptionsCallback);
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

void Options_SaveOptionsBin(void (*callback)(bool32 success))
{
    if (Options->changed) {
        if (sku_platform && sku_platform != PLATFORM_DEV) {
            if (globals->optionsLoaded == STATUS_OK) {
                Options->saveEntityPtr = SceneInfo->entity;
                Options->saveCallback  = callback;
#if RETRO_USE_PLUS
                API_SaveUserFile("Options.bin", globals->optionsRAM, 0x200, Options_SaveOptionsCallback, false);
#else
                API_SaveUserFile("Options.bin", globals->optionsRAM, 0x200, Options_SaveOptionsCallback);
#endif
            }
            else {
                Options->changed = false;
                if (callback)
                    callback(false);
            }
            return;
        }
        else {
#if RETRO_USE_PLUS
            RSDK.SetSettingsValue(SETTINGS_CHANGED, true);
#else
            APICallback_SaveSettingsINI();
#endif
            Options_Reload();
        }
    }
    if (callback)
        callback(true);
}

void Options_SetLanguage(int32 language)
{
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;
    if (language >= 0) {
        options->language         = language;
        options->overrideLanguage = true;
    }
    else {
        options->language         = -1;
        options->overrideLanguage = false;
    }

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV)
        RSDK.SetSettingsValue(SETTINGS_LANGUAGE, language);
    Options->changed = true;
}

void Options_LoadValuesFromSettings(EntityOptions *options)
{
    if (options->overrideLanguage) {
        Localization->language = options->language;
    }
    else {
        options->language = sku_language;
    }

    if (!options->overrideShader) {
        options->screenShader = RSDK.GetSettingsValue(SETTINGS_SHADERID) % 4;
    }
    if (!options->overrideMusicVol) {
        options->volMusic = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);
    }
    if (!options->overrideSfxVol) {
        options->volSfx = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);
    }
}

void Options_LoadOptionsCallback(int32 statusCode)
{
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;
    bool32 success          = false;
    if (statusCode == STATUS_OK || statusCode == STATUS_NOTFOUND) {
        success                 = true;
        globals->optionsLoaded = STATUS_OK;
        LogHelpers_Print("dataPtr.language = %d", options->language);
        LogHelpers_Print("dataPtr.overrideLanguage = %d", options->overrideLanguage);
        Options_LoadValuesFromSettings((EntityOptions *)globals->optionsRAM);
        RSDK.SetSettingsValue(SETTINGS_SHADERID, options->screenShader);
        RSDK.SetSettingsValue(SETTINGS_STREAM_VOL, options->volMusic);
        RSDK.SetSettingsValue(SETTINGS_SFX_VOL, options->volSfx);
    }
    else {
        success                 = false;
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

void Options_SaveOptionsCallback(int32 statusCode)
{
    Options->changed = false;
    if (Options->saveCallback) {
        Entity *entStore = SceneInfo->entity;
        if (Options->saveEntityPtr)
            SceneInfo->entity = Options->saveEntityPtr;
        Options->saveCallback(statusCode == STATUS_OK);
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