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
        EntityOptions *options   = (EntityOptions *)globals->optionsRAM;
        options->vSync           = false;
        options->windowed        = false;
        options->windowBorder    = false;
        options->tripleBuffering = false;
    }
#endif
}
void Options_EditorDraw(void) {}
void Options_EditorLoad(void) {}
void Options_Serialize(void) {}

void Options_Reload(void)
{
    EntityOptions *options    = (EntityOptions *)globals->optionsRAM;
    options->screenShader     = RSDK.GetSettingsValue(SETTINGS_SHADERID) % 4;
    options->volMusic         = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);
    options->volSfx           = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);
    options->language         = RSDK.GetSettingsValue(SETTINGS_LANGUAGE) << 16;
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
    int windowed           = RSDK.GetSettingsValue(SETTINGS_WINDOWED);
    if (!windowed) {
        options->windowSize = 4;
    }
    else {
        int width = RSDK.GetSettingsValue(SETTINGS_WINDOW_WIDTH);
        if (width > 424) {
            if (width > 848) {
                options->windowSize = (width > 1272) + 2;
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
                Options->loadEntityPtr = RSDK_sceneInfo->entity;
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

void Options_SaveOptionsBin(void (*callback)(int))
{
    if (Options->state) {
        if (sku_platform && sku_platform != PLATFORM_DEV) {
            if (globals->optionsLoaded == STATUS_OK) {
                Options->saveEntityPtr = RSDK_sceneInfo->entity;
                Options->saveCallback  = callback;
#if RETRO_USE_PLUS
                API_SaveUserFile("Options.bin", globals->optionsRAM, 0x200, Options_SaveOptionsCallback, false);
#else
                API_SaveUserFile("Options.bin", globals->optionsRAM, 0x200, Options_SaveOptionsCallback);
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

void Options_SetLanguage(int language)
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
    Options->state = 1;
}

void Options_Unknown1(EntityOptions *options)
{
    if (options->overrideLanguage) {
        Localization->language = options->language;
    }
    else {
        options->language = sku_language;
    }

    if (!options->field_60) {
        options->screenShader = RSDK.GetSettingsValue(SETTINGS_SHADERID) % 4;
    }
    if (!options->field_68) {
        options->volMusic = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);
    }
    if (!options->field_70) {
        options->volSfx = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);
    }
}

void Options_LoadOptionsCallback(int statusCode)
{
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;
    bool32 status          = false;
    if (statusCode == STATUS_OK || statusCode == STATUS_NOTFOUND) {
        status                 = true;
        globals->optionsLoaded = STATUS_OK;
        LogHelpers_Print("dataPtr.language = %d", options->language);
        LogHelpers_Print("dataPtr.overrideLanguage = %d", options->overrideLanguage);
        Options_Unknown1((EntityOptions *)globals->optionsRAM);
        RSDK.SetSettingsValue(SETTINGS_SHADERID, options->screenShader);
        RSDK.SetSettingsValue(SETTINGS_STREAM_VOL, options->volMusic);
        RSDK.SetSettingsValue(SETTINGS_SFX_VOL, options->volSfx);
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
