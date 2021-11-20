#include "SonicMania.h"

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

    drawPos.x = (ScreenInfo->centerX - self->timer2) << 16;
    drawPos.y = 0x680000;
    RSDK.DrawSprite(&self->leftAnimator, &drawPos, true);

    drawPos.x = (ScreenInfo->centerX + self->timer2) << 16;
    RSDK.DrawSprite(&self->rightAnimator, &drawPos, true);

    if (self->shouldFade)
        RSDK.FillScreen(self->colour, self->timer, self->timer - 128, self->timer - 256);
}

void BSS_Message_Create(void *data)
{
    RSDK_THIS(BSS_Message);
    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawOrder = 15;

        switch (voidToInt(data)) {
            case BSS_MESSAGE_GETBS:
                self->shouldFade   = true;
                self->colour = 0xF0F0F0;
                self->timer  = 512;
                self->state  = BSS_Message_State_GetBS;
                RSDK.SetSpriteAnimation(BSS_Message->aniFrames, 2, &self->leftAnimator, true, 0);
                RSDK.SetSpriteAnimation(BSS_Message->aniFrames, 2, &self->rightAnimator, true, 1);
                break;
            case BSS_MESSAGE_PERFECT:
                self->timer2 = 320;
                self->state  = BSS_Message_State_Perfect;
                RSDK.SetSpriteAnimation(BSS_Message->aniFrames, 3, &self->leftAnimator, true, 0);
                RSDK.SetSpriteAnimation(BSS_Message->aniFrames, 3, &self->rightAnimator, true, 1);
                break;
            case BSS_MESSAGE_2_FINISHED:
                self->shouldFade   = true;
                self->colour = 0;
                self->state  = BSS_Message_State_Finished;
                break;
        }
    }
}

void BSS_Message_StageLoad(void)
{
    BSS_Message->aniFrames = RSDK.LoadSpriteAnimation("SpecialBS/HUD.bin", SCOPE_STAGE);
    RSDK.ResetEntitySlot(SLOT_BSS_MESSAGE, BSS_Message->objectID, intToVoid(BSS_MESSAGE_GETBS));
}

void BSS_Message_State_GetBS(void)
{
    RSDK_THIS(BSS_Message);
    if (self->timer == 512) {
        Music_SetMusicTrack("BlueSpheresSPD.ogg", TRACK_STAGE, 5309957);
        Music_PlayTrack(TRACK_STAGE);
    }

    if (self->timer <= 0) {
        self->timer = 0;
        self->shouldFade  = false;
        self->state = BSS_Message_State_GetBSWait;
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

    if (!setup->maxSpeed) {
        if (player->up) {
            setup->maxSpeed      = 16;
            setup->globeSpeed    = 16;
            setup->globeSpeedInc = 2;
            if (player->onGround)
                RSDK.SetSpriteAnimation(player->aniFrames, 1, &player->animator, 0, 0);
            self->state = BSS_Message_State_Idle;
        }
        if (!setup->globeTimer && setup->state == BSS_Setup_State_HandleStage) {
            if (player->left)
                setup->state = BSS_Setup_State_SpinLeft;
            if (player->right)
                setup->state = BSS_Setup_State_SpinRight;
        }
    }

    if (++self->timer >= 180) {
        self->timer        = 0;
        setup->maxSpeed      = 16;
        setup->globeSpeed    = 16;
        setup->globeSpeedInc = 2;
        if (player->onGround)
            RSDK.SetSpriteAnimation(player->aniFrames, 1, &player->animator, 0, 0);
        self->state = BSS_Message_State_Finish;
    }
}

void BSS_Message_State_Finished(void)
{
    RSDK_THIS(BSS_Message);
    if (self->timer >= 768) {
        self->state = BSS_Message_LoadPrevScene;
    }
    else {
        self->timer += 8;
    }
}

void BSS_Message_State_Perfect(void)
{
    RSDK_THIS(BSS_Message);
    self->timer2 -= 16;
    if (self->timer2 <= 0)
        self->state = BSS_Message_State_Idle;
}

void BSS_Message_State_Idle(void)
{
    RSDK_THIS(BSS_Message);
    if (++self->timer >= 180) {
        self->timer = 0;
        self->state = BSS_Message_State_Finish;
    }
}

void BSS_Message_State_Finish(void)
{
    RSDK_THIS(BSS_Message);
    self->timer2 += 16;
    if (self->timer2 > 320)
        destroyEntity(self);
}
void BSS_Message_TrackProgress_CB(int32 success)
{
    RSDK_THIS(BSS_Message);
    self->field_6C = 0;
    UIWaitSpinner_Wait2();
}

void BSS_Message_LoadPrevScene(void)
{
    RSDK_THIS(BSS_Message);

    if (self->colour) {
        self->colour -= 0x80808;
    }
    else {
        EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
        if (param->field_59 == 1) {
            RSDK.SetScene("Presentation", "Menu");
            RSDK.LoadScene();
        }
        else if (globals->gameMode < MODE_TIMEATTACK) {
            if (globals->saveSlotID == NO_SAVE_SLOT) {
                globals->blueSpheresInit = true;
                GameProgress_ShuffleBSSID();
                EntitySaveGame *saveRAM = SaveGame->saveRAM;
#if RETRO_USE_PLUS
                if (globals->gameMode == MODE_ENCORE)
                    RSDK.SetScene("Encore Mode", "");
                else
#endif
                    RSDK.SetScene("Mania Mode", "");
                SceneInfo->listPos = saveRAM->storedStageID;
                RSDK.LoadScene();
                self->state = StateMachine_None;
            }
            else {
                self->state    = BSS_Message_LoadGameState;
                self->field_6C = 1;
                UIWaitSpinner_Wait();
                GameProgress_TrackGameProgress(BSS_Message_TrackProgress_CB);
            }
        }
    }
}

void BSS_Message_LoadGameState(void)
{
    RSDK_THIS(BSS_Message);
    if (!self->field_6C) {
        globals->blueSpheresInit = true;
        GameProgress_ShuffleBSSID();
        EntitySaveGame *saveRAM = SaveGame->saveRAM;
#if RETRO_USE_PLUS
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

#if RETRO_INCLUDE_EDITOR
void BSS_Message_EditorDraw(void) {}

void BSS_Message_EditorLoad(void) {}
#endif

void BSS_Message_Serialize(void) {}
