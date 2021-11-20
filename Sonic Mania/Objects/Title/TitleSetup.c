#include "SonicMania.h"

ObjectTitleSetup *TitleSetup;

void TitleSetup_Update(void)
{
    RSDK_THIS(TitleSetup);
    StateMachine_Run(self->state);
    ScreenInfo->position.x = 0x100 - ScreenInfo->centerX;
}

void TitleSetup_LateUpdate(void) {}

void TitleSetup_StaticUpdate(void) {}

void TitleSetup_Draw(void)
{
    RSDK_THIS(TitleSetup);
    StateMachine_Run(self->stateDraw);
}

void TitleSetup_Create(void *data)
{
    RSDK_THIS(TitleSetup);
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(TitleSetup->aniFrames, 0, &self->animator, true, 0);
        self->active    = ACTIVE_ALWAYS;
        self->visible   = true;
        self->drawOrder = 12;
        self->drawFX    = FX_FLIP;
        self->state     = TitleSetup_Wait;
        self->stateDraw = TitleSetup_DrawState_FadeBlack;
        self->timer     = 1024;
        self->drawPos.x = 0x1000000;
        self->drawPos.y = 0x6C0000;
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
    if (ControllerInfo->keyUp.press || AnalogStickInfoL->keyUp.press) {
        keyState = 1;
    }
    if (ControllerInfo->keyDown.press || AnalogStickInfoL->keyDown.press) {
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
    if (ControllerInfo->keyA.press || ControllerInfo->keyB.press || ControllerInfo->keyStart.press) {
        RSDK.StopChannel(Music->channelID);
        return true;
    }
#if RETRO_USE_TOUCH_CONTROLS
    else if (TouchInfo->count) {
        RSDK.StopChannel(Music->channelID);
        return true;
    }
#endif
    return false;
}

void TitleSetup_Wait(void)
{
    RSDK_THIS(TitleSetup);
    if (self->timer <= -0x400) {
        self->timer     = 0;
        self->state     = TitleSetup_AnimateUntilFlash;
        self->stateDraw = TitleSetup_DrawState_DrawRing;
        Music_PlayTrack(TRACK_STAGE);
    }
    else {
        self->timer -= 16;
    }
}

void TitleSetup_AnimateUntilFlash(void)
{
    RSDK_THIS(TitleSetup);
    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == 31) {
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
        self->state = TitleSetup_Flash;
    }
}

void TitleSetup_Flash(void)
{
    RSDK_THIS(TitleSetup);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
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
        self->timer     = 0x300;
        self->state     = TitleSetup_WaitForSonic;
        self->stateDraw = TitleSetup_DrawState_Flash;
    }
}

void TitleSetup_WaitForSonic(void)
{
    RSDK_THIS(TitleSetup);
    TitleSetup_CheckCheatCode();
    if (self->timer <= 0) {
        self->stateDraw = StateMachine_None;
#if RETRO_USE_PLUS
        if (API.CheckDLC(DLC_PLUS))
            self->state = TitleSetup_SetupLogo_Plus;
        else
#endif
            self->state = TitleSetup_SetupLogo;
    }
    else {
        self->timer -= 16;
    }
}

void TitleSetup_SetupLogo(void)
{
    RSDK_THIS(TitleSetup);
#if RETRO_USE_PLUS
    if (self->timer < 120)
        TitleSetup_CheckCheatCode();
#endif
    if (++self->timer == 120) {
        foreach_all(TitleLogo, titleLogo)
        {
            if (titleLogo->type == TITLELOGO_PRESSSTART) {
                titleLogo->active      = ACTIVE_NORMAL;
                titleLogo->visible = true;
#if RETRO_USE_PLUS
                Entity *store          = SceneInfo->entity;
                SceneInfo->entity = (Entity *)titleLogo;
                TitleLogo_SetupPressStart();
                SceneInfo->entity = store;
#endif
            }
        }
        self->timer = 0;
        self->state = TitleSetup_WaitForEnter;
    }
}
#if RETRO_USE_PLUS
void TitleSetup_SetupLogo_Plus(void)
{
    RSDK_THIS(TitleSetup);
    if (self->timer < 120)
        TitleSetup_CheckCheatCode();
    if (++self->timer == 120) {
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
        self->timer = 0;
        self->state = TitleSetup_SetupLogo;
    }
}
#endif

void TitleSetup_WaitForEnter(void)
{
    RSDK_THIS(TitleSetup);
    bool32 anyButton = ControllerInfo->keyA.press || ControllerInfo->keyB.press || ControllerInfo->keyC.press || ControllerInfo->keyX.press
                     || ControllerInfo->keyY.press || ControllerInfo->keyZ.press || ControllerInfo->keyStart.press
                     || ControllerInfo->keySelect.press;
#if RETRO_USE_PLUS
    bool32 anyClick = (!TouchInfo->count && self->touched) || UnknownInfo->field_28;
#else
    bool32 anyClick = !TouchInfo->count && self->touched;
#endif
    self->touched = TouchInfo->count > 0;
    if (anyClick || anyButton) {
        RSDK.PlaySfx(TitleSetup->sfxMenuAccept, 0, 255);
        self->timer = 0;
        const char *nextScene = "Menu";
#if RETRO_GAMEVER == VER_100
        if (ControllerInfo->keyA.down && (ControllerInfo->keyX.down || ControllerInfo->keyC.down))
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
        self->state     = TitleSetup_FadeToMenu;
        self->stateDraw = TitleSetup_DrawState_FadeBlack;
    }
    else if (++self->timer == 800) {
        self->timer     = 0;
        self->state     = TitleSetup_FadeToVideo;
        self->stateDraw = TitleSetup_DrawState_FadeBlack;
    }
}

void TitleSetup_FadeToMenu(void)
{
    RSDK_THIS(TitleSetup);
    if (self->timer >= 1024) {
        RSDK.LoadScene();
    }
    else {
        self->timer += 8;
    }
}

void TitleSetup_FadeToVideo(void)
{
    RSDK_THIS(TitleSetup);
    if (self->timer >= 1024) {
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
        self->timer += 8;
    }
}

void TitleSetup_DrawState_FadeBlack(void)
{
    RSDK_THIS(TitleSetup);

    RSDK.FillScreen(0x000000, self->timer, self->timer - 128, self->timer - 256);
}

void TitleSetup_DrawState_DrawRing(void)
{
    RSDK_THIS(TitleSetup);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void TitleSetup_DrawState_Flash(void)
{
    RSDK_THIS(TitleSetup);

    RSDK.FillScreen(0xF0F0F0, self->timer, self->timer - 128, self->timer - 256);
}

#if RETRO_INCLUDE_EDITOR
void TitleSetup_EditorDraw(void) {}

void TitleSetup_EditorLoad(void) {}
#endif

void TitleSetup_Serialize(void) {}
