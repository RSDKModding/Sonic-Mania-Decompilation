// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TitleSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTitleSetup *TitleSetup;

void TitleSetup_Update(void)
{
    RSDK_THIS(TitleSetup);

    StateMachine_Run(self->state);

    ScreenInfo->position.x = 0x100 - ScreenInfo->center.x;
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
        self->drawGroup = 12;
        self->drawFX    = FX_FLIP;
        self->state     = TitleSetup_State_Wait;
        self->stateDraw = TitleSetup_Draw_FadeBlack;
        self->timer     = 1024;
        self->drawPos.x = 256 << 16;
        self->drawPos.y = 108 << 16;
    }
}

void TitleSetup_StageLoad(void)
{
    String presence;
    Localization_GetString(&presence, STR_RPC_TITLE);
    API_SetRichPresence(PRESENCE_TITLE, &presence);

    API_SetNoSave(false);

    globals->blueSpheresInit = false;
    TimeAttackData_Clear();

    API_ClearPrerollErrors();
#if MANIA_USE_PLUS
    API.ClearSaveStatus();
#endif

    globals->saveLoaded = false;
    memset(globals->saveRAM, 0, sizeof(globals->saveRAM));

    globals->optionsLoaded = false;
    memset(globals->optionsRAM, 0, sizeof(globals->optionsRAM));

#if MANIA_USE_PLUS
    API.ClearUserDB(globals->replayTableID);
    API.ClearUserDB(globals->taTableID);

    globals->replayTableID     = (uint16)-1;
    globals->replayTableLoaded = STATUS_NONE;
    globals->taTableID         = (uint16)-1;
    globals->taTableLoaded     = STATUS_NONE;
#endif

    TitleSetup->aniFrames = RSDK.LoadSpriteAnimation("Title/Electricity.bin", SCOPE_STAGE);

    TitleSetup->sfxMenuBleep  = RSDK.GetSfx("Global/MenuBleep.wav");
    TitleSetup->sfxMenuAccept = RSDK.GetSfx("Global/MenuAccept.wav");
    TitleSetup->sfxRing       = RSDK.GetSfx("Global/Ring.wav");

    RSDK.ResetEntitySlot(0, TitleSetup->classID, NULL);
}

#if MANIA_USE_PLUS
void TitleSetup_HandleCheatInputs(void)
{
    uint8 keyState = 0;
    if (ControllerInfo->keyUp.press || AnalogStickInfoL->keyUp.press)
        keyState = 1;
    if (ControllerInfo->keyDown.press || AnalogStickInfoL->keyDown.press)
        keyState = 2;

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
        if (!globals->superSecret) {
            RSDK.PlaySfx(TitleSetup->sfxRing, false, 255);
            globals->superSecret = true;
        }
    }
}
#endif

bool32 TitleSetup_VideoSkipCB(void)
{
    if (ControllerInfo->keyA.press || ControllerInfo->keyB.press || ControllerInfo->keyStart.press) {
        Music_Stop();
        return true;
    }

    return false;
}

void TitleSetup_State_Wait(void)
{
    RSDK_THIS(TitleSetup);

    if (self->timer <= -0x400) {
        self->timer     = 0;
        self->state     = TitleSetup_State_AnimateUntilFlash;
        self->stateDraw = TitleSetup_Draw_DrawRing;
        Music_PlayTrack(TRACK_STAGE);
    }
    else {
        self->timer -= 16;
    }
}

void TitleSetup_State_AnimateUntilFlash(void)
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

        self->state = TitleSetup_State_FlashIn;
    }
}

void TitleSetup_State_FlashIn(void)
{
    RSDK_THIS(TitleSetup);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        foreach_all(TitleLogo, titleLogo)
        {
#if MANIA_USE_PLUS
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
                titleLogo->showRibbonCenter = true;
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 2, &titleLogo->mainAnimator, true, 0);
            }
        }

        foreach_all(TitleSonic, titleSonic)
        {
            titleSonic->active  = ACTIVE_NORMAL;
            titleSonic->visible = true;
        }

        TitleBG_SetupFX();
        self->timer     = 0x300;
        self->state     = TitleSetup_State_WaitForSonic;
        self->stateDraw = TitleSetup_Draw_Flash;
    }
}

void TitleSetup_State_WaitForSonic(void)
{
    RSDK_THIS(TitleSetup);

#if MANIA_USE_PLUS
    TitleSetup_CheckCheatCode();
#endif

    if (self->timer <= 0) {
        self->stateDraw = StateMachine_None;

#if MANIA_USE_PLUS
        if (API.CheckDLC(DLC_PLUS))
            self->state = TitleSetup_State_SetupPlusLogo;
        else
#endif
            self->state = TitleSetup_State_SetupLogo;
    }
    else {
        self->timer -= 16;
    }
}

void TitleSetup_State_SetupLogo(void)
{
    RSDK_THIS(TitleSetup);

#if MANIA_USE_PLUS
    if (self->timer < 120)
        TitleSetup_CheckCheatCode();
#endif

    if (++self->timer == 120) {
        foreach_all(TitleLogo, titleLogo)
        {
            if (titleLogo->type == TITLELOGO_PRESSSTART) {
                titleLogo->active  = ACTIVE_NORMAL;
                titleLogo->visible = true;

#if MANIA_USE_PLUS
                Entity *store     = SceneInfo->entity;
                SceneInfo->entity = (Entity *)titleLogo;
                TitleLogo_SetupPressStart();
                SceneInfo->entity = store;
#endif
            }
        }

        self->timer = 0;
        self->state = TitleSetup_State_WaitForEnter;
    }
}
#if MANIA_USE_PLUS
void TitleSetup_State_SetupPlusLogo(void)
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
        self->state = TitleSetup_State_SetupLogo;
    }
}
#endif

void TitleSetup_State_WaitForEnter(void)
{
    RSDK_THIS(TitleSetup);

    bool32 anyButton = ControllerInfo->keyA.press || ControllerInfo->keyB.press || ControllerInfo->keyC.press || ControllerInfo->keyX.press
                       || ControllerInfo->keyY.press || ControllerInfo->keyZ.press || ControllerInfo->keyStart.press
                       || ControllerInfo->keySelect.press;

    bool32 anyClick = (!TouchInfo->count && self->touched) || Unknown_anyKeyPress;
    self->touched   = TouchInfo->count > 0;

    if (anyClick || anyButton) {
        RSDK.PlaySfx(TitleSetup->sfxMenuAccept, false, 0xFF);
        self->timer = 0;

        const char *nextScene = "Menu";
        // Switch 1.0 dev level select cheat (Skips all the funky API setup stuff the menu does, so it was known for causin a bunch of issues)
#if GAME_VERSION == VER_100
        if (ControllerInfo->keyA.down && (ControllerInfo->keyX.down || ControllerInfo->keyC.down))
            nextScene = "Level Select";
#endif
        RSDK.SetScene("Presentation", nextScene);

#if MANIA_USE_PLUS
        int32 id = API_GetFilteredInputDeviceID(false, false, 5);
#else
        int32 id = API_GetFilteredInputDeviceID(INPUT_NONE);
#endif
        API_ResetInputSlotAssignments();
        API_AssignInputSlotToDevice(CONT_P1, id);

        Music_Stop();
        self->state     = TitleSetup_State_FadeToMenu;
        self->stateDraw = TitleSetup_Draw_FadeBlack;
    }
    else if (++self->timer == 800) {
        self->timer     = 0;
        self->state     = TitleSetup_State_FadeToVideo;
        self->stateDraw = TitleSetup_Draw_FadeBlack;
    }
}

void TitleSetup_State_FadeToMenu(void)
{
    RSDK_THIS(TitleSetup);

    if (self->timer >= 1024)
        RSDK.LoadScene();
    else
        self->timer += 8;
}

void TitleSetup_State_FadeToVideo(void)
{
    RSDK_THIS(TitleSetup);

    if (self->timer >= 1024) {
        RSDK.LoadScene();
        Music_Stop();

        if (TitleSetup->useAltIntroMusic) {
            RSDK.PlayStream("IntroTee.ogg", Music->channelID, 0, 0, false);
            RSDK.LoadVideo("Mania.ogv", 1.8, TitleSetup_VideoSkipCB);
            TitleSetup->useAltIntroMusic = false;
        }
        else {
            RSDK.PlayStream("IntroHP.ogg", Music->channelID, 0, 0, false);
            RSDK.LoadVideo("Mania.ogv", 0, TitleSetup_VideoSkipCB);
            TitleSetup->useAltIntroMusic = true;
        }
    }
    else {
        self->timer += 8;
    }
}

void TitleSetup_Draw_FadeBlack(void)
{
    RSDK_THIS(TitleSetup);

    RSDK.FillScreen(0x000000, self->timer, self->timer - 128, self->timer - 256);
}

void TitleSetup_Draw_DrawRing(void)
{
    RSDK_THIS(TitleSetup);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void TitleSetup_Draw_Flash(void)
{
    RSDK_THIS(TitleSetup);

    RSDK.FillScreen(0xF0F0F0, self->timer, self->timer - 128, self->timer - 256);
}

#if GAME_INCLUDE_EDITOR
void TitleSetup_EditorDraw(void) {}

void TitleSetup_EditorLoad(void) {}
#endif

void TitleSetup_Serialize(void) {}
