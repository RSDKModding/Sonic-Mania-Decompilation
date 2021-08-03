#include "SonicMania.h"

ObjectMenuSetup *MenuSetup;

void MenuSetup_Update(void)
{
    RSDK_THIS(MenuSetup);
    StateMachine_Run(entity->state);
    if (entity->timer >= entity->timeOut) {
        StateMachine_Run(entity->timedState);
        destroyEntity(entity);
    }
    else {
        entity->timer++;
    }
}

void MenuSetup_LateUpdate(void) {}

void MenuSetup_StaticUpdate(void)
{
    if (!MenuSetup->dword18) {
        MenuSetup->fxFade->speedOut = 0;
        TextInfo tag;
        INIT_TEXTINFO(tag);
        foreach_all(UIControl, control)
        {
            if (!RSDK.StringCompare(&tag, &control->tag, false)) {
                RSDK.PrependText(&tag, "Main Menu");
            }
        }
        if (!ManiaModeMenu_InitUserdata()) {
            control->selectionDisabled = true;
            return;
        }
        else {
            control->selectionDisabled = false;
            MenuSetup->dword18         = 1;
            TextInfo info;
            Localization_GetString(&info, STR_RPC_MENU);
#if RETRO_USE_PLUS
            API.SetRichPresence(1, &info);
#else
            APICallback_SetRichPresence(1, &info);
#endif
        }
    }
    if (!MenuSetup->initialized) {
        ManiaModeMenu_Initialize();
        MenuSetup->initialized = true;
    }
    if (!MenuSetup->dword8) {
        ManiaModeMenu_Unknown7();
        MenuSetup->dword8 = 1;
        ManiaModeMenu_SetBGColours();
        if (!globals->suppressAutoMusic) {
            ManiaModeMenu_ChangeMenuTrack();
        }
        globals->suppressAutoMusic = false;
    }
    MenuSetup->fxFade->speedOut = 12;
#if RETRO_USE_PLUS
    DialogRunner_GetUserAuthStatus();
#else
    APICallback_GetUserAuthStatus();
#endif
}

void MenuSetup_Draw(void)
{
    RSDK_THIS(MenuSetup);
    RSDK.FillScreen(entity->fadeColour, entity->fadeTimer, entity->fadeTimer - 128, entity->fadeTimer - 256);
}

void MenuSetup_Create(void *data)
{
    RSDK_THIS(MenuSetup);
    entity->active    = ACTIVE_ALWAYS;
    entity->visible   = true;
    entity->drawOrder = 14;
}

void MenuSetup_StageLoad(void)
{
    const char *tag = (const char *)(&((char *)&globals->menuParam)[90]);
    LogHelpers_Print("Menu recall ctrl: %s", tag);
    MenuSetup->dword8      = 0;
    MenuSetup->initialized = 0;
    MenuSetup->dword10     = 0;
    MenuSetup->gameLoaded  = 0;
    MenuSetup->dword18     = 0;
    MenuSetup->fxFade      = 0;
    if (!globals->suppressAutoMusic) {
        RSDK.StopChannel(Music->channelID);
        Music->activeTrack = -1;
    }
    if (!RSDK_sceneInfo->inEditor) {
#if RETRO_USE_PLUS
        switch (RSDK_sku->platform) {
#else
        switch (RSDK_info->platform) {
#endif
            case PLATFORM_PC: LogHelpers_Print("PC SKU"); break;
            case PLATFORM_PS4: LogHelpers_Print("PS4 SKU"); break;
            case PLATFORM_XB1: LogHelpers_Print("XB1 SKU"); break;
            case PLATFORM_SWITCH: LogHelpers_Print("NX SKU"); break;
            case PLATFORM_DEV: LogHelpers_Print("DEV SKU"); break;
            default:
#if RETRO_USE_PLUS
                LogHelpers_Print("INVALID PLATFORM: %d", RSDK_sku->platform);
#else
                LogHelpers_Print("INVALID PLATFORM: %d", RSDK_info->platform);
#endif
                break;
        }

#if RETRO_USE_PLUS
        switch (RSDK_sku->region) {
#else
        switch (RSDK_info->region) {
#endif
            case REGION_US: LogHelpers_Print("US REGION"); break;
            case REGION_JP: LogHelpers_Print("JP REGION"); break;
            case REGION_EU: LogHelpers_Print("EU REGION"); break;
            default:
#if RETRO_USE_PLUS
                LogHelpers_Print("INVALID REGION: %d", RSDK_sku->region);
#else
                LogHelpers_Print("INVALID REGION: %d", RSDK_info->region);
#endif
                break;
        }
    }

    memset(globals->noSaveSlot, 0, 0x400);
    globals->continues      = 0;
    globals->recallEntities = false;
    RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
    foreach_all(FXFade, fade) { MenuSetup->fxFade = fade; }
}

void MenuSetup_StartTransition(void (*callback)(void), int time)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->field_68        = 5;
    menuSetup->timeOut         = time;
    menuSetup->state           = ManiaModeMenu_Unknown13;
    menuSetup->timedState      = callback;
}

//START PRE-PLUS AREA
#if !RETRO_USE_PLUS

#endif

void MenuSetup_EditorDraw(void) {}

void MenuSetup_EditorLoad(void) {}

void MenuSetup_Serialize(void) {}
