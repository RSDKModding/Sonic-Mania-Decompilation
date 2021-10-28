#include "SonicMania.h"

ObjectDebugMode *DebugMode;

void DebugMode_Update(void)
{
    RSDK_THIS(DebugMode);

    bool32 flag = false;
    // Disables Achievements
#if RETRO_USE_PLUS
    API.SetAchievementStatus(false);
#else
    if (!APICallback->achievementsDisabled)
        APICallback->achievementsDisabled = true;
#endif
    //if (Zone)
    //    Zone->stageFinishCallback = NULL;
    if (RSDK_controller[CONT_P1].keyUp.down || (RSDK_stickL[CONT_P1].vDelta > 0.3)) {
        entity->position.y -= entity->velocity.y;
        flag = true;
    }
    else if (RSDK_controller[CONT_P1].keyDown.down || (RSDK_stickL[CONT_P1].vDelta < -0.3)) {
        entity->position.y += entity->velocity.y;
        flag = true;
    }

    if (RSDK_controller[CONT_P1].keyLeft.down || (RSDK_stickL[CONT_P1].hDelta < -0.3)) {
        entity->position.x -= entity->velocity.y;
        flag = true;
    }
    else if (RSDK_controller[CONT_P1].keyRight.down || (RSDK_stickL[CONT_P1].hDelta > 0.3)) {
        entity->position.x += entity->velocity.y;
        flag = true;
    }

    if (!flag) {
        entity->velocity.y = 0;
    }
    else {
        entity->velocity.y += 0xC00;
        if (entity->velocity.y > 0x100000)
            entity->velocity.y = 0x100000;
    }

#if RETRO_GAMEVER != VER_100
    bool32 keyBack = RSDK_controller[CONT_P1].keyY.press;
    bool32 keySpawn = RSDK_controller[CONT_P1].keyX.press;
#else
    bool32 spawnKey = RSDK_controller[CONT_P1].keyX.press;
    bool32 keySpawn = RSDK_controller[CONT_P1].keyY.press;
#endif

    if (RSDK_controller[CONT_P1].keyA.press) {
        ++DebugMode->objID;
        DebugMode->objID %= DebugMode->itemCount;
        DebugMode->itemSubType = 0;
    }
    else if (keySpawn) {
        EntityPlayer *player        = (EntityPlayer *)RSDK_sceneInfo->entity;
        player->objectID            = Player->objectID;
        player->groundVel           = 0;
        player->velocity.x          = 0;
        player->velocity.y          = 0;
        player->state               = Player_State_Air;
        player->collisionPlane      = 0;
        player->tileCollisions      = true;
        player->interaction         = true;
        player->visible             = true;
        player->drawOrder           = Zone->playerDrawLow;
        RSDK_sceneInfo->timeEnabled = true;
        if (RSDK_sceneInfo->minutes == 9 && RSDK_sceneInfo->seconds == 59 && RSDK_sceneInfo->milliseconds == 99) {
            RSDK_sceneInfo->minutes      = 0;
            RSDK_sceneInfo->seconds      = 0;
            RSDK_sceneInfo->milliseconds = 0;
        }
        DebugMode->debugActive = false;
    }
    else if (RSDK_controller[CONT_P1].keyA.down) {
        if (RSDK_controller[CONT_P1].keyC.press || keyBack) {
            --DebugMode->objID;
            if (DebugMode->objID < 0) {
                DebugMode->objID = DebugMode->itemCount - 1;
            }
            DebugMode->itemSubType = 0;
        }
        if (RSDK_controller[CONT_P1].keyB.press) {
            DebugMode->itemSubType--;
            if (DebugMode->itemSubType >= DebugMode->subtypeCount)
                DebugMode->itemSubType = DebugMode->subtypeCount - 1;
        }
    }
    else if (RSDK_controller[CONT_P1].keyC.press || keyBack) {
        StateMachine_Run(DebugMode->spawn[DebugMode->objID]);
    }
    else if (RSDK_controller[CONT_P1].keyB.press) {
        if (DebugMode->itemSubType >= DebugMode->subtypeCount - 1)
            DebugMode->itemSubType = 0;
        else
            DebugMode->itemSubType++;
    }
}

void DebugMode_LateUpdate(void) {}

void DebugMode_StaticUpdate(void) {}

void DebugMode_Draw(void)
{
    StateMachine_Run(DebugMode->draw[DebugMode->objID]);
}

void DebugMode_Create(void *data)
{
    RSDK_THIS(DebugMode);
    entity->active  = ACTIVE_NORMAL;
    entity->visible = true;
}

void DebugMode_StageLoad(void)
{
    DebugMode->objID       = 0;
    DebugMode->itemCount   = 0;
    DebugMode->debugActive = false;

    for (int32 i = 0; i < DebugMode_ObjectLimit; ++i) {
        DebugMode->objectIDs[i] = TYPE_BLANK;
        DebugMode->draw[i]      = StateMachine_None;
        DebugMode->spawn[i]     = DebugMode_NullState;
    }
}

void DebugMode_NullState(void) {}
void DebugMode_AddObject(uint16 id, void (*draw)(void), void (*spawn)(void))
{
    if (DebugMode->itemCount < DebugMode_ObjectLimit) {
        DebugMode->objectIDs[DebugMode->itemCount] = id;
        DebugMode->draw[DebugMode->itemCount]      = draw;
        DebugMode->spawn[DebugMode->itemCount]     = spawn;
        DebugMode->itemCount++;
    }
}

#if RETRO_INCLUDE_EDITOR
void DebugMode_EditorDraw(void) {}

void DebugMode_EditorLoad(void) {}
#endif

void DebugMode_Serialize(void) {}
