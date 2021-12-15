// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DebugMode Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectDebugMode *DebugMode;

void DebugMode_Update(void)
{
    RSDK_THIS(DebugMode);

    // Disables Achievements
#if RETRO_USE_PLUS
    API.SetAchievementsEnabled(false);
#else
    if (!APICallback->achievementsDisabled)
        APICallback->achievementsDisabled = true;
#endif

    //if (Zone)
    //    Zone->stageFinishCallback = NULL;

    bool32 moved = false;
    if (ControllerInfo[CONT_P1].keyUp.down || (AnalogStickInfoL[CONT_P1].vDelta > 0.3)) {
        self->position.y -= self->velocity.y;
        moved = true;
    }
    else if (ControllerInfo[CONT_P1].keyDown.down || (AnalogStickInfoL[CONT_P1].vDelta < -0.3)) {
        self->position.y += self->velocity.y;
        moved = true;
    }

    if (ControllerInfo[CONT_P1].keyLeft.down || (AnalogStickInfoL[CONT_P1].hDelta < -0.3)) {
        self->position.x -= self->velocity.y;
        moved = true;
    }
    else if (ControllerInfo[CONT_P1].keyRight.down || (AnalogStickInfoL[CONT_P1].hDelta > 0.3)) {
        self->position.x += self->velocity.y;
        moved = true;
    }

    if (!moved) {
        self->velocity.y = 0;
    }
    else {
        self->velocity.y += 0xC00;
        if (self->velocity.y > 0x100000)
            self->velocity.y = 0x100000;
    }

#if RETRO_GAMEVER != VER_100
    bool32 keyBack = ControllerInfo[CONT_P1].keyY.press;
    bool32 keySpawn = ControllerInfo[CONT_P1].keyX.press;
#else
    bool32 keyBack  = ControllerInfo[CONT_P1].keyX.press;
    bool32 keySpawn = ControllerInfo[CONT_P1].keyY.press;
#endif

    if (ControllerInfo[CONT_P1].keyA.press) {
        ++DebugMode->objID;
        DebugMode->objID %= DebugMode->itemCount;
        DebugMode->itemSubType = 0;
    }
    else if (keySpawn) {
        EntityPlayer *player        = (EntityPlayer *)SceneInfo->entity;
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
        SceneInfo->timeEnabled      = true;
        if (SceneInfo->minutes == 9 && SceneInfo->seconds == 59 && SceneInfo->milliseconds == 99) {
            SceneInfo->minutes      = 0;
            SceneInfo->seconds      = 0;
            SceneInfo->milliseconds = 0;
        }
        DebugMode->debugActive = false;
    }
    else if (ControllerInfo[CONT_P1].keyA.down) {
        if (ControllerInfo[CONT_P1].keyC.press || keyBack) {
            --DebugMode->objID;
            if (DebugMode->objID < 0) {
                DebugMode->objID = DebugMode->itemCount - 1;
            }
            DebugMode->itemSubType = 0;
        }
        if (ControllerInfo[CONT_P1].keyB.press) {
            DebugMode->itemSubType--;
            if (DebugMode->itemSubType >= DebugMode->subtypeCount)
                DebugMode->itemSubType = DebugMode->subtypeCount - 1;
        }
    }
    else if (ControllerInfo[CONT_P1].keyC.press || keyBack) {
        StateMachine_Run(DebugMode->spawn[DebugMode->objID]);
    }
    else if (ControllerInfo[CONT_P1].keyB.press) {
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
    self->active  = ACTIVE_NORMAL;
    self->visible = true;
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
