#include "SonicMania.h"

ObjectTitleSetup *TitleSetup;

void TitleSetup_Update(void)
{
    RSDK_THIS(TitleSetup);
    StateMachine_Run(entity->state);
    RSDK_screens->position.x = 0x100 - RSDK_screens->centerX;
}

void TitleSetup_LateUpdate(void) {}

void TitleSetup_StaticUpdate(void) {}

void TitleSetup_Draw(void)
{
    RSDK_THIS(TitleSetup);
    StateMachine_Run(entity->stateDraw);
}

void TitleSetup_Create(void *data)
{
    RSDK_THIS(TitleSetup);
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(TitleSetup->aniFrames, 0, &entity->animator, true, 0);
        entity->active    = ACTIVE_ALWAYS;
        entity->visible   = true;
        entity->drawOrder = 12;
        entity->drawFX    = FX_FLIP;
        entity->state     = TitleSetup_Wait;
        entity->stateDraw = TitleSetup_DrawState_FadeBlack;
        entity->timer     = 1024;
        entity->drawPos.x = 0x1000000;
        entity->drawPos.y = 0x6C0000;
    }
}

void TitleSetup_StageLoad(void)
{
    TextInfo presence;
    Localization_GetString(&presence, STR_RPC_TITLE);
    API_SetRichPresence(PRESENCE_TITLE, &presence);
#if RETRO_USE_PLUS
    API.SetUserStorageNoSave(false);
#else
    globals->noSave = false;
#endif
    globals->blueSpheresInit = false;
    TimeAttackData_ClearOptions();
#if RETRO_USE_PLUS
    API.ClearPrerollErrors();
    API.ClearUserStorageStatus();
#else
    APICallback_ClearPrerollErrors();
#endif
    globals->saveLoaded = false;
    memset(globals->saveRAM, 0, 0x10000);
    globals->optionsLoaded = 0;
    memset(globals->optionsRAM, 0, 0x200);
#if RETRO_USE_PLUS
    API.ClearUserDB(globals->replayTableID);
    API.ClearUserDB(globals->taTableID);
    globals->replayTableID     = -1;
    globals->replayTableLoaded = STATUS_NONE;
    globals->taTableID         = -1;
    globals->taTableLoaded     = STATUS_NONE;
#endif
    TitleSetup->aniFrames    = RSDK.LoadSpriteAnimation("Title/Electricity.bin", SCOPE_STAGE);
    TitleSetup->sfxMenuBleep  = RSDK.GetSFX("Global/MenuBleep.wav");
    TitleSetup->sfxMenuAccept = RSDK.GetSFX("Global/MenuAccept.wav");
    TitleSetup->sfxRing       = RSDK.GetSFX("Global/Ring.wav");
    RSDK.ResetEntitySlot(0, TitleSetup->objectID, NULL);
}

void TitleSetup_HandleCheatInputs(void)
{
    uint8 keyState = 0;
    if (RSDK_controller->keyUp.press || RSDK_stickL->keyUp.press) {
        keyState = 1;
    }
    if (RSDK_controller->keyDown.press || RSDK_stickL->keyDown.press) {
        keyState = 2;
    }

    if (keyState) {
        TitleSetup->cheatCode[0] = TitleSetup->cheatCode[1];
        TitleSetup->cheatCode[1] = TitleSetup->cheatCode[2];
        TitleSetup->cheatCode[2] = TitleSetup->cheatCode[3];
        TitleSetup->cheatCode[3] = TitleSetup->cheatCode[4];
        TitleSetup->cheatCode[4] = TitleSetup->cheatCode[5];
        TitleSetup->cheatCode[5] = TitleSetup->cheatCode[6];
        TitleSetup->cheatCode[6] = TitleSetup->cheatCode[7];
        TitleSetup->cheatCode[7] = keyState;
    }
}

void TitleSetup_CheckCheatCode(void)
{
    TitleSetup_HandleCheatInputs();
    if (TitleSetup->cheatCode[0] == 1 && TitleSetup->cheatCode[1] == 1 && TitleSetup->cheatCode[2] == 2 && TitleSetup->cheatCode[3] == 2
        && TitleSetup->cheatCode[4] == 1 && TitleSetup->cheatCode[5] == 1 && TitleSetup->cheatCode[6] == 1 && TitleSetup->cheatCode[7] == 1) {
#if RETRO_USE_PLUS
        if (!globals->superSecret) {
            RSDK.PlaySfx(TitleSetup->sfxRing, false, 255);
            globals->superSecret = true;
        }
#endif
    }
}
bool32 TitleSetup_IntroCallback(void)
{
    if (RSDK_controller->keyA.press || RSDK_controller->keyB.press || RSDK_controller->keyStart.press) {
        RSDK.StopChannel(Music->channelID);
        return true;
    }
#if RETRO_USE_TOUCH_CONTROLS
    else if (RSDK_touchMouse->count) {
        RSDK.StopChannel(Music->channelID);
        return true;
    }
#endif
    return false;
}

void TitleSetup_Wait(void)
{
    RSDK_THIS(TitleSetup);
    if (entity->timer <= -0x400) {
        entity->timer     = 0;
        entity->state     = TitleSetup_AnimateUntilFlash;
        entity->stateDraw = TitleSetup_DrawState_DrawRing;
        Music_PlayTrack(TRACK_STAGE);
    }
    else {
        entity->timer -= 16;
    }
}

void TitleSetup_AnimateUntilFlash(void)
{
    RSDK_THIS(TitleSetup);
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == 31) {
        foreach_all(TitleLogo, titleLogo)
        {
            if (titleLogo->type >= TITLELOGO_EMBLEM) {
                if (titleLogo->type <= TITLELOGO_RIBBON) {
                    titleLogo->active  = ACTIVE_NORMAL;
                    titleLogo->visible = true;
                }
                else if (titleLogo->type == TITLELOGO_POWERLED) {
                    destroyEntity(titleLogo);
                }
            }
        }
        entity->state = TitleSetup_Unknown6;
    }
}

void TitleSetup_Unknown6(void)
{
    RSDK_THIS(TitleSetup);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        foreach_all(TitleLogo, titleLogo)
        {
#if RETRO_USE_PLUS
            if (titleLogo->type == TITLELOGO_PLUS) {
                titleLogo->position.y -= 0x200000;
            }
            else if (titleLogo->type != TITLELOGO_PRESSSTART) {
#else
            if (titleLogo->type != TITLELOGO_PRESSSTART) {
#endif
                titleLogo->active  = ACTIVE_NORMAL;
                titleLogo->visible = true;
            }
            if (titleLogo->type == TITLELOGO_RIBBON) {
                titleLogo->flag = true;
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 2, &titleLogo->animator1, true, 0);
            }
        }

        foreach_all(TitleSonic, titleSonic)
        {
            titleSonic->active  = ACTIVE_NORMAL;
            titleSonic->visible = true;
        }

        TitleBG_SetupFX();
        entity->timer     = 0x300;
        entity->state     = TitleSetup_Unknown7;
        entity->stateDraw = TitleSetup_DrawState_Flash;
    }
}

void TitleSetup_Unknown7(void)
{
    RSDK_THIS(TitleSetup);
    TitleSetup_CheckCheatCode();
    if (entity->timer <= 0) {
        entity->stateDraw = StateMachine_None;
#if RETRO_USE_PLUS
        if (API.CheckDLC(DLC_PLUS))
            entity->state = TitleSetup_SetupLogo_Plus;
        else
#endif
            entity->state = TitleSetup_SetupLogo;
    }
    else {
        entity->timer -= 16;
    }
}

void TitleSetup_SetupLogo(void)
{
    RSDK_THIS(TitleSetup);
#if RETRO_USE_PLUS
    if (entity->timer < 120)
        TitleSetup_CheckCheatCode();
#endif
    if (++entity->timer == 120) {
        foreach_all(TitleLogo, titleLogo)
        {
            if (titleLogo->type == TITLELOGO_PRESSSTART) {
                titleLogo->active      = ACTIVE_NORMAL;
                titleLogo->visible = true;
#if RETRO_USE_PLUS
                Entity *store          = RSDK_sceneInfo->entity;
                RSDK_sceneInfo->entity = (Entity *)titleLogo;
                TitleLogo_SetupPressStart();
                RSDK_sceneInfo->entity = store;
#endif
            }
        }
        entity->timer = 0;
        entity->state = TitleSetup_Unknown10;
    }
}
#if RETRO_USE_PLUS
void TitleSetup_SetupLogo_Plus(void)
{
    RSDK_THIS(TitleSetup);
    if (entity->timer < 120)
        TitleSetup_CheckCheatCode();
    if (++entity->timer == 120) {
        foreach_all(TitleLogo, titleLogo)
        {
            switch (titleLogo->type) {
                case TITLELOGO_RIBBON:
                case TITLELOGO_GAMETITLE:
                    titleLogo->storeY     = titleLogo->position.y - 0x70000;
                    titleLogo->velocity.y = -0x30000;
                    titleLogo->timer      = 2;
                    titleLogo->state      = TitleLogo_State_HandleSetup;
                    RSDK.PlaySfx(TitleLogo->sfxPlus, false, 255);
                    break;
                case TITLELOGO_PRESSSTART: titleLogo->position.y += 0x80000; break;
                case TITLELOGO_PLUS:
                    titleLogo->active  = ACTIVE_NORMAL;
                    titleLogo->visible = true;
                    titleLogo->timer   = 2;
                    titleLogo->position.y -= 0x40000;
                    titleLogo->state = TitleLogo_State_HandleSetup;
                    break;
                default: break;
            }
        }

        CREATE_ENTITY(TitleEggman, NULL, 0, 0xC00000);
        entity->timer = 0;
        entity->state = TitleSetup_SetupLogo;
    }
}
#endif

void TitleSetup_Unknown10(void)
{
    RSDK_THIS(TitleSetup);
    bool32 skipped = RSDK_controller->keyA.press || RSDK_controller->keyB.press || RSDK_controller->keyC.press || RSDK_controller->keyX.press
                     || RSDK_controller->keyY.press || RSDK_controller->keyZ.press || RSDK_controller->keyStart.press
                     || RSDK_controller->keySelect.press;
#if RETRO_USE_PLUS
    bool32 skipped2 = (!RSDK_touchMouse->count && entity->touched) || RSDK_unknown->field_28;
#else
    bool32 skipped2 = !RSDK_touchMouse->count && entity->touched;
#endif
    entity->touched = RSDK_touchMouse->count > 0;
    if (skipped2 || skipped) {
        RSDK.PlaySfx(TitleSetup->sfxMenuAccept, 0, 255);
        entity->timer = 0;
        const char *nextScene = "Menu";
#if RETRO_GAMEVER == VER_100
        if (RSDK_controller->keyA.down && (RSDK_controller->keyX.down || RSDK_controller->keyC.down))
            nextScene = "Level Select";
#endif
        RSDK.SetScene("Presentation", nextScene);
#if RETRO_USE_PLUS
        int32 id = API_MostRecentActiveControllerID(0, 0, 5);
#else
        int32 id = API_MostRecentActiveControllerID(0);
#endif
        API_ResetControllerAssignments();
        API_AssignControllerID(1, id);
        RSDK.StopChannel(Music->channelID);
        entity->state     = TitleSetup_Unknown11;
        entity->stateDraw = TitleSetup_DrawState_FadeBlack;
    }
    else if (++entity->timer == 800) {
        entity->timer     = 0;
        entity->state     = TitleSetup_Unknown12;
        entity->stateDraw = TitleSetup_DrawState_FadeBlack;
    }
}

void TitleSetup_Unknown11(void)
{
    RSDK_THIS(TitleSetup);
    if (entity->timer >= 1024) {
        RSDK.LoadScene();
    }
    else {
        entity->timer += 8;
    }
}

void TitleSetup_Unknown12(void)
{
    RSDK_THIS(TitleSetup);
    if (entity->timer >= 1024) {
        RSDK.LoadScene();
        RSDK.StopChannel(Music->channelID);
        if (TitleSetup->altMusic) {
            RSDK.PlayStream("IntroTee.ogg", Music->channelID, 0, 0, false);
            RSDK.LoadVideo("Mania.ogv", 1.8, TitleSetup_IntroCallback);
            TitleSetup->altMusic = 0;
        }
        else {
            RSDK.PlayStream("IntroHP.ogg", Music->channelID, 0, 0, false);
            RSDK.LoadVideo("Mania.ogv", 0, TitleSetup_IntroCallback);
            TitleSetup->altMusic = true;
        }
    }
    else {
        entity->timer += 8;
    }
}

void TitleSetup_DrawState_FadeBlack(void)
{
    RSDK_THIS(TitleSetup);

    RSDK.FillScreen(0x000000, entity->timer, entity->timer - 128, entity->timer - 256);
}

void TitleSetup_DrawState_DrawRing(void)
{
    RSDK_THIS(TitleSetup);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);

    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void TitleSetup_DrawState_Flash(void)
{
    RSDK_THIS(TitleSetup);

    RSDK.FillScreen(0xF0F0F0, entity->timer, entity->timer - 128, entity->timer - 256);
}

#if RETRO_INCLUDE_EDITOR
void TitleSetup_EditorDraw(void) {}

void TitleSetup_EditorLoad(void) {}
#endif

void TitleSetup_Serialize(void) {}
