// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BSS_Message Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBSS_Message *BSS_Message;

void BSS_Message_Update(void)
{
    RSDK_THIS(BSS_Message);

    StateMachine_Run(self->state);
}

void BSS_Message_LateUpdate(void) {}

void BSS_Message_StaticUpdate(void) {}

void BSS_Message_Draw(void)
{
    RSDK_THIS(BSS_Message);
    Vector2 drawPos;

    drawPos.x = (ScreenInfo->center.x - self->messageFinishTimer) << 16;
    drawPos.y = TO_FIXED(104);
    RSDK.DrawSprite(&self->leftAnimator, &drawPos, true);

    drawPos.x = (ScreenInfo->center.x + self->messageFinishTimer) << 16;
    RSDK.DrawSprite(&self->rightAnimator, &drawPos, true);

    if (self->fadeEnabled)
        RSDK.FillScreen(self->color, self->timer, self->timer - 128, self->timer - 256);
}

void BSS_Message_Create(void *data)
{
    RSDK_THIS(BSS_Message);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawGroup = 15;

        switch (VOID_TO_INT(data)) {
            case BSS_MESSAGE_GETSPHERES:
                self->fadeEnabled = true;
                self->color       = 0xF0F0F0;
                self->timer       = 512;
                self->state       = BSS_Message_State_GetBS;

                RSDK.SetSpriteAnimation(BSS_Message->aniFrames, 2, &self->leftAnimator, true, 0);
                RSDK.SetSpriteAnimation(BSS_Message->aniFrames, 2, &self->rightAnimator, true, 1);
                break;
            case BSS_MESSAGE_PERFECT:
                self->messageFinishTimer = 320;
                self->state              = BSS_Message_State_Perfect;

                RSDK.SetSpriteAnimation(BSS_Message->aniFrames, 3, &self->leftAnimator, true, 0);
                RSDK.SetSpriteAnimation(BSS_Message->aniFrames, 3, &self->rightAnimator, true, 1);
                break;
            case BSS_MESSAGE_FINISHED:
                self->fadeEnabled = true;
                self->color       = 0x000000;
                self->state       = BSS_Message_State_Finished;
                break;
        }
    }
}

void BSS_Message_StageLoad(void)
{
    BSS_Message->aniFrames = RSDK.LoadSpriteAnimation("SpecialBS/HUD.bin", SCOPE_STAGE);

    RSDK.ResetEntitySlot(SLOT_BSS_MESSAGE, BSS_Message->classID, INT_TO_VOID(BSS_MESSAGE_GETSPHERES));
}

void BSS_Message_State_GetBS(void)
{
    RSDK_THIS(BSS_Message);

    if (self->timer == 512) {
        Music_SetMusicTrack("BlueSpheresSPD.ogg", TRACK_STAGE, 5309957);
        Music_PlayTrack(TRACK_STAGE);
    }

    if (self->timer <= 0) {
        self->timer       = 0;
        self->fadeEnabled = false;
        self->state       = BSS_Message_State_GetBSWait;
    }
    else {
        self->timer -= 16;
    }
}

void BSS_Message_State_GetBSWait(void)
{
    RSDK_THIS(BSS_Message);

    EntityBSS_Setup *setup   = RSDK_GET_ENTITY(SLOT_BSS_SETUP, BSS_Setup);
    EntityBSS_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);

    if (!setup->speedupLevel) {
        if (player->up) {
            setup->speedupLevel  = 16;
            setup->globeSpeed    = 16;
            setup->globeSpeedInc = 2;

            if (player->onGround)
                RSDK.SetSpriteAnimation(player->aniFrames, 1, &player->animator, false, 0);

            self->state = BSS_Message_State_WaitPerfect;
        }

        if (!setup->globeTimer && setup->state == BSS_Setup_State_GlobeMoveZ) {
            if (player->left)
                setup->state = BSS_Setup_State_GlobeTurnLeft;

            if (player->right)
                setup->state = BSS_Setup_State_GlobeTurnRight;
        }
    }

    if (++self->timer >= 180) {
        self->timer = 0;

        setup->speedupLevel  = 16;
        setup->globeSpeed    = 16;
        setup->globeSpeedInc = 2;

        if (player->onGround)
            RSDK.SetSpriteAnimation(player->aniFrames, 1, &player->animator, false, 0);

        self->state = BSS_Message_State_MsgFinished;
    }
}

void BSS_Message_State_Finished(void)
{
    RSDK_THIS(BSS_Message);

    if (self->timer >= 768)
        self->state = BSS_Message_State_SaveGameProgress;
    else
        self->timer += 8;
}

void BSS_Message_State_Perfect(void)
{
    RSDK_THIS(BSS_Message);

    self->messageFinishTimer -= 16;
    if (self->messageFinishTimer <= 0)
        self->state = BSS_Message_State_WaitPerfect;
}

void BSS_Message_State_WaitPerfect(void)
{
    RSDK_THIS(BSS_Message);

    if (++self->timer >= 180) {
        self->timer = 0;
        self->state = BSS_Message_State_MsgFinished;
    }
}

void BSS_Message_State_MsgFinished(void)
{
    RSDK_THIS(BSS_Message);

    self->messageFinishTimer += 16;
    if (self->messageFinishTimer > 320)
        destroyEntity(self);
}

#if MANIA_USE_PLUS
void BSS_Message_TrackProgressCB(bool32 success)
{
    RSDK_THIS(BSS_Message);

    self->saveInProgress = false;
    UIWaitSpinner_FinishWait();
}
#else
void BSS_Message_TrackProgressCB(void)
{
    RSDK_THIS(BSS_Message);

    self->saveInProgress = false;
    UIWaitSpinner_FinishWait();
}
#endif

void BSS_Message_State_SaveGameProgress(void)
{
    RSDK_THIS(BSS_Message);

    if (self->color) {
        self->color -= 0x80808;
    }
    else {
        EntityMenuParam *param = MenuParam_GetParam();

        if (param->bssSelection == BSS_SELECTION_EXTRAS) {
            RSDK.SetScene("Presentation", "Menu");
            RSDK.LoadScene();
        }
        else if (globals->gameMode < MODE_TIMEATTACK) {
            if (globals->saveSlotID == NO_SAVE_SLOT) {
                BSS_Message_State_LoadPrevScene();
            }
            else {
                self->state          = BSS_Message_State_LoadPrevScene;
                self->saveInProgress = true;

                UIWaitSpinner_StartWait();
                GameProgress_TrackGameProgress(BSS_Message_TrackProgressCB);
            }
        }
    }
}

void BSS_Message_State_LoadPrevScene(void)
{
    RSDK_THIS(BSS_Message);

    if (!self->saveInProgress) {
        globals->blueSpheresInit = true;
        GameProgress_ShuffleBSSID();

        SaveRAM *saveRAM = SaveGame_GetSaveRAM();
#if MANIA_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            RSDK.SetScene("Encore Mode", "");
        else
#endif
            RSDK.SetScene("Mania Mode", "");

        SceneInfo->listPos = saveRAM->storedStageID;
        RSDK.LoadScene();
        self->state = StateMachine_None;
    }
}

#if GAME_INCLUDE_EDITOR
void BSS_Message_EditorDraw(void) {}

void BSS_Message_EditorLoad(void) {}
#endif

void BSS_Message_Serialize(void) {}
