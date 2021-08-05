#include "SonicMania.h"

ObjectBSS_Message *BSS_Message;

void BSS_Message_Update(void)
{
    RSDK_THIS(BSS_Message);

    StateMachine_Run(entity->state);
}

void BSS_Message_LateUpdate(void) {}

void BSS_Message_StaticUpdate(void) {}

void BSS_Message_Draw(void)
{
    RSDK_THIS(BSS_Message);
    Vector2 drawPos;

    drawPos.x = (RSDK_screens->centerX - entity->timer2) << 16;
    drawPos.y = 0x680000;
    RSDK.DrawSprite(&entity->leftData, &drawPos, true);
    RSDK.DrawSprite(&entity->rightData, &drawPos, true);
    if (entity->flag)
        RSDK.FillScreen(entity->colour, entity->timer, entity->timer - 128, entity->timer - 256);
}

void BSS_Message_Create(void *data)
{
    RSDK_THIS(BSS_Message);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = 1;
        entity->drawOrder = 15;
        if (voidToInt(data) == 1) {
            entity->timer2 = 320;
            entity->state  = BSS_Message_State_Perfect;
            RSDK.SetSpriteAnimation(BSS_Message->spriteIndex, 3, &entity->leftData, true, 0);
            RSDK.SetSpriteAnimation(BSS_Message->spriteIndex, 3, &entity->rightData, true, 1);
        }
        else {
            entity->flag = true;
            if (voidToInt(data) == 2) {
                entity->colour = 0;
                entity->state  = BSS_Message_State_Unknown;
            }
            else {
                entity->colour = 0xF0F0F0;
                entity->timer  = 512;
                entity->state  = BSS_Message_State_GetBS;
                RSDK.SetSpriteAnimation(BSS_Message->spriteIndex, 2, &entity->leftData, true, 0);
                RSDK.SetSpriteAnimation(BSS_Message->spriteIndex, 2, &entity->rightData, true, 1);
            }
        }
    }
}

void BSS_Message_StageLoad(void)
{
    BSS_Message->spriteIndex = RSDK.LoadSpriteAnimation("SpecialBS/HUD.bin", SCOPE_STAGE);
    RSDK.ResetEntitySlot(SLOT_BSS_MESSAGE, BSS_Message->objectID, 0);
}

void BSS_Message_State_GetBS(void)
{
    RSDK_THIS(BSS_Message);
    if (entity->timer == 512) {
        Music_SetMusicTrack("BlueSpheresSPD.ogg", TRACK_STAGE, 5309957);
        Music_PlayTrack(TRACK_STAGE);
    }

    if (entity->timer <= 0) {
        entity->timer = 0;
        entity->flag  = 0;
        entity->state = BSS_Message_State_GetBSWait;
    }
    else {
        entity->timer -= 16;
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
                RSDK.SetSpriteAnimation(player->spriteIndex, 1, &player->playerAnimator, 0, 0);
            entity->state = BSS_Message_State_Idle;
        }
        if (!setup->globeTimer && setup->state == BSS_Setup_State_HandleStage) {
            if (player->left)
                setup->state = BSS_Setup_State_SpinLeft;
            if (player->right)
                setup->state = BSS_Setup_State_SpinRight;
        }
    }

    if (++entity->timer >= 180) {
        entity->timer        = 0;
        setup->maxSpeed      = 16;
        setup->globeSpeed    = 16;
        setup->globeSpeedInc = 2;
        if (player->onGround)
            RSDK.SetSpriteAnimation(player->spriteIndex, 1, &player->playerAnimator, 0, 0);
        entity->state = BSS_Message_State_Finish;
    }
}

void BSS_Message_State_Unknown(void)
{
    RSDK_THIS(BSS_Message);
    if (entity->timer >= 768) {
        entity->state = BSS_Message_LoadPrevScene;
    }
    else {
        entity->timer += 8;
    }
}

void BSS_Message_State_Perfect(void)
{
    RSDK_THIS(BSS_Message);
    entity->timer2 -= 16;
    if (entity->timer2 <= 0)
        entity->state = BSS_Message_State_Idle;
}

void BSS_Message_State_Idle(void)
{
    RSDK_THIS(BSS_Message);
    if (++entity->timer >= 180) {
        entity->timer = 0;
        entity->state = BSS_Message_State_Finish;
    }
}

void BSS_Message_State_Finish(void)
{
    RSDK_THIS(BSS_Message);
    entity->timer2 += 16;
    if (entity->timer2 > 320)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
}
void BSS_Message_TrackProgress_CB(int success)
{
    RSDK_THIS(BSS_Message);
    entity->field_6C = 0;
    UIWaitSpinner_Wait2();
}

void BSS_Message_LoadPrevScene(void)
{
    RSDK_THIS(BSS_Message);

    if (entity->colour) {
        entity->colour -= 0x80808;
    }
    else {
        EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
        if (param->field_59 == 1) {
            RSDK.LoadScene("Presentation", "Menu");
            RSDK.InitSceneLoad();
        }
        else if (globals->gameMode < MODE_TIMEATTACK) {
            if (globals->saveSlotID == NO_SAVE_SLOT) {
                globals->blueSpheresInit = true;
                SaveGame_ShuffleBSSID();
                EntitySaveGame *saveRAM = SaveGame->saveRAM;
#if RETRO_USE_PLUS
                if (globals->gameMode == MODE_ENCORE)
                    RSDK.LoadScene("Encore Mode", "");
                else
#endif
                    RSDK.LoadScene("Mania Mode", "");
                RSDK_sceneInfo->listPos = saveRAM->storedStageID;
                RSDK.InitSceneLoad();
                entity->state = StateMachine_None;
            }
            else {
                entity->state    = BSS_Message_LoadGameState;
                entity->field_6C = 1;
                UIWaitSpinner_Wait();
                SaveGame_TrackGameProgress(BSS_Message_TrackProgress_CB);
            }
        }
    }
}

void BSS_Message_LoadGameState(void)
{
    RSDK_THIS(BSS_Message);
    if (!entity->field_6C) {
        globals->blueSpheresInit = true;
        SaveGame_ShuffleBSSID();
        EntitySaveGame *saveRAM = SaveGame->saveRAM;
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            RSDK.LoadScene("Encore Mode", "");
        else
#endif
            RSDK.LoadScene("Mania Mode", "");
        RSDK_sceneInfo->listPos = saveRAM->storedStageID;
        RSDK.InitSceneLoad();
        entity->state = StateMachine_None;
    }
}

void BSS_Message_EditorDraw(void) {}

void BSS_Message_EditorLoad(void) {}

void BSS_Message_Serialize(void) {}
