// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DebugMode Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDebugMode *DebugMode;

void DebugMode_Update(void)
{
    RSDK_THIS(DebugMode);

    API_SetAchievementsEnabled(false);

    if (Zone)
        Zone->stageFinishCallback = StateMachine_None;

    bool32 moved = false;

#if MANIA_USE_PLUS
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
#else
    if (ControllerInfo[CONT_P1].keyUp.down || (AnalogStickInfoL[CONT_P1].vDeltaL > 0.3)) {
        self->position.y -= self->velocity.y;
        moved = true;
    }
    else if (ControllerInfo[CONT_P1].keyDown.down || (AnalogStickInfoL[CONT_P1].vDeltaL < -0.3)) {
        self->position.y += self->velocity.y;
        moved = true;
    }

    if (ControllerInfo[CONT_P1].keyLeft.down || (AnalogStickInfoL[CONT_P1].hDeltaL < -0.3)) {
        self->position.x -= self->velocity.y;
        moved = true;
    }
    else if (ControllerInfo[CONT_P1].keyRight.down || (AnalogStickInfoL[CONT_P1].hDeltaL > 0.3)) {
        self->position.x += self->velocity.y;
        moved = true;
    }
#endif

    if (!moved) {
        self->velocity.y = 0;
    }
    else {
        self->velocity.y += 0xC00;
        if (self->velocity.y > TO_FIXED(16))
            self->velocity.y = TO_FIXED(16);
    }

#if GAME_VERSION != VER_100
    bool32 keySpawn = ControllerInfo[CONT_P1].keyY.press;
    bool32 keyBack  = ControllerInfo[CONT_P1].keyX.press;
#else
    bool32 keySpawn = ControllerInfo[CONT_P1].keyX.press;
    bool32 keyBack  = ControllerInfo[CONT_P1].keyY.press;
#endif

    if (ControllerInfo[CONT_P1].keyA.press) {
        ++DebugMode->itemID;
        DebugMode->itemID %= DebugMode->itemCount;
        DebugMode->itemType = 0;
    }
    else if (keyBack) {
        // Do this so we can access the player variables again
        RSDK_THIS(Player);
        self->classID        = Player->classID;
        self->groundVel      = 0;
        self->velocity.x     = 0;
        self->velocity.y     = 0;
        self->state          = Player_State_Air;
        self->collisionPlane = 0;
        self->tileCollisions = TILECOLLISION_DOWN;
        self->interaction    = true;
        self->visible        = true;
        self->drawGroup      = Zone->playerDrawGroup[0];

        SceneInfo->timeEnabled = true;
        if (SceneInfo->minutes == 9 && SceneInfo->seconds == 59 && SceneInfo->milliseconds == 99) {
            SceneInfo->minutes      = 0;
            SceneInfo->seconds      = 0;
            SceneInfo->milliseconds = 0;
        }

        DebugMode->debugActive = false;
    }
    else if (ControllerInfo[CONT_P1].keyA.down) {
        if (ControllerInfo[CONT_P1].keyC.press || keySpawn) {
            if (--DebugMode->itemID < 0)
                DebugMode->itemID = DebugMode->itemCount - 1;

            DebugMode->itemType = 0;
        }

        if (ControllerInfo[CONT_P1].keyB.press) {
            DebugMode->itemType--;

            if (DebugMode->itemType >= DebugMode->itemTypeCount)
                DebugMode->itemType = DebugMode->itemTypeCount - 1;
        }
    }
    else if (ControllerInfo[CONT_P1].keyC.press || keySpawn) {
        StateMachine_Run(DebugMode->spawn[DebugMode->itemID]);
    }
    else if (ControllerInfo[CONT_P1].keyB.press) {
        if (DebugMode->itemType >= DebugMode->itemTypeCount - 1)
            DebugMode->itemType = 0;
        else
            DebugMode->itemType++;
    }
}

void DebugMode_LateUpdate(void) {}

void DebugMode_StaticUpdate(void) {}

void DebugMode_Draw(void) { StateMachine_Run(DebugMode->draw[DebugMode->itemID]); }

void DebugMode_Create(void *data)
{
    RSDK_THIS(DebugMode);

    self->active  = ACTIVE_NORMAL;
    self->visible = true;
}

void DebugMode_StageLoad(void)
{
    DebugMode->itemID      = 0;
    DebugMode->itemCount   = 0;
    DebugMode->debugActive = false;

    for (int32 i = 0; i < DEBUGMODE_OBJECT_COUNT; ++i) {
        DebugMode->classIDs[i] = TYPE_BLANK;
        DebugMode->draw[i]     = StateMachine_None;
        DebugMode->spawn[i]    = DebugMode_NullState;
    }
}

void DebugMode_NullState(void) {}

void DebugMode_AddObject(uint16 id, void (*draw)(void), void (*spawn)(void))
{
    if (DebugMode->itemCount < DEBUGMODE_OBJECT_COUNT) {
        DebugMode->classIDs[DebugMode->itemCount] = id;
        DebugMode->draw[DebugMode->itemCount]     = draw;
        DebugMode->spawn[DebugMode->itemCount]    = spawn;
        DebugMode->itemCount++;
    }
}

#if GAME_INCLUDE_EDITOR
void DebugMode_EditorDraw(void) {}

void DebugMode_EditorLoad(void) {}
#endif

void DebugMode_Serialize(void) {}
