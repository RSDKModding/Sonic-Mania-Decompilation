// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RotatingStair Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectRotatingStair *RotatingStair;

void RotatingStair_Update(void) { Platform_Update(); }

void RotatingStair_LateUpdate(void) {}

void RotatingStair_StaticUpdate(void) {}

void RotatingStair_Draw(void)
{
    RSDK_THIS(RotatingStair);

    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void RotatingStair_Create(void *data)
{
    RSDK_THIS(RotatingStair);

    self->frameID   = 2;
    self->collision = PLATFORM_C_SOLID;
    self->speed     = 3;

    if (self->direction)
        self->amplitude.x = -self->amplitude.x;

    if (self->mode & 1)
        self->amplitude.x = -self->amplitude.x;

    int32 typeStore = self->mode;
    self->mode      = (RotatingStairModes)PLATFORM_LINEAR;
    Platform_Create(NULL);
    self->mode = typeStore;

    if (self->mode >= 4) {
        self->duration = 120 * self->speed;
        self->state    = RotatingStair_State_Move_Intervals;
        self->interval = self->duration + 512;
    }
    else {
        self->state = RotatingStair_State_Move;
    }
}

void RotatingStair_StageLoad(void) { RotatingStair->unused = 0; }

void RotatingStair_State_Move(void)
{
    RSDK_THIS(RotatingStair);

    int32 timer = Zone->timer + self->oscOff;
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    int32 dir = 0;
    if (self->mode & 1)
        dir = self->mode - (((3 * timer) >> 9) & 3) - 2;
    else
        dir = ((3 * timer) >> 9) + self->mode;

    switch (dir & 3) {
        case FLIP_NONE: // right
            self->drawPos.x = self->centerPos.x + self->amplitude.x * RSDK.Cos1024(timer * self->speed);
            self->drawPos.y = self->centerPos.y + (self->amplitude.y << 10);
            break;

        case FLIP_X: // up
            self->drawPos.x = self->centerPos.x - (self->amplitude.x << 10);
            self->drawPos.y = self->centerPos.y - self->amplitude.y * RSDK.Cos1024(timer * self->speed);
            break;

        case FLIP_Y: // left
            self->drawPos.x = self->centerPos.x - self->amplitude.x * RSDK.Cos1024(timer * self->speed);
            self->drawPos.y = self->centerPos.y - (self->amplitude.y << 10);
            break;

        case FLIP_XY: // down
            self->drawPos.x = self->centerPos.x + (self->amplitude.x << 10);
            self->drawPos.y = self->centerPos.y + self->amplitude.y * RSDK.Cos1024(timer * self->speed);
            break;
    }

    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void RotatingStair_State_Move_Intervals(void)
{
    RSDK_THIS(RotatingStair);

    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    int32 angle = 0;
    if (self->speed * (Zone->timer + self->oscOff) % self->interval >= self->duration)
        angle = self->speed * (Zone->timer + self->oscOff) % self->interval - self->duration;

    int32 dir = 0;
    if (self->mode & 1)
        dir = self->mode - ((self->speed * (Zone->timer + self->oscOff) / self->interval) & 3) - 2;
    else
        dir = self->speed * (Zone->timer + self->oscOff) / self->interval + self->mode;

    switch (dir & 3) {
        case FLIP_NONE: // right
            self->drawPos.x = self->centerPos.x + self->amplitude.x * RSDK.Cos1024(angle);
            self->drawPos.y = self->centerPos.y + (self->amplitude.y << 10);
            break;

        case FLIP_X: // up
            self->drawPos.x = self->centerPos.x - (self->amplitude.x << 10);
            self->drawPos.y = self->centerPos.y - self->amplitude.y * RSDK.Cos1024(angle + 0x200);
            break;

        case FLIP_Y: // left
            self->drawPos.x = self->centerPos.x - self->amplitude.x * RSDK.Cos1024(angle);
            self->drawPos.y = self->centerPos.y - (self->amplitude.y << 10);
            break;

        case FLIP_XY: // down
            self->drawPos.x = self->centerPos.x + (self->amplitude.x << 10);
            self->drawPos.y = self->centerPos.y + self->amplitude.y * RSDK.Cos1024(angle + 0x200);
            break;
    }

    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

#if GAME_INCLUDE_EDITOR
void RotatingStair_EditorDraw(void)
{
    RSDK_THIS(RotatingStair);

    self->frameID   = 2;
    self->collision = PLATFORM_C_SOLID;
    self->speed     = 3;
    if (self->direction)
        self->amplitude.x = -self->amplitude.x;
    if (self->mode & 1)
        self->amplitude.x = -self->amplitude.x;

    int32 typeStore = self->mode;
    self->mode      = (RotatingStairModes)PLATFORM_LINEAR;
    Platform_Create(NULL);
    self->mode = typeStore;

    // self->drawPos = self->position;

    Vector2 amplitude = self->amplitude;

    if (self->direction)
        self->amplitude.x = -self->amplitude.x;
    if (self->mode & 1)
        self->amplitude.x = -self->amplitude.x;

    if (self->mode >= 4) {
        self->duration = 120 * self->speed;
        self->interval = self->duration + 512;
        RotatingStair_State_Move_Intervals();
    }
    else {
        RotatingStair_State_Move();
    }

    RotatingStair_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s + i);
            if (!child)
                continue;

            DrawHelpers_DrawArrow(self->drawPos.x, self->drawPos.y, child->position.x, child->position.y, 0xFFFF00, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }

    self->amplitude = amplitude;
}

void RotatingStair_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(RotatingStair, mode);
    RSDK_ENUM_VAR("Start Left", ROTATINGSTAIR_LEFT);
    RSDK_ENUM_VAR("Start Down", ROTATINGSTAIR_DOWN);
    RSDK_ENUM_VAR("Start Right", ROTATINGSTAIR_RIGHT);
    RSDK_ENUM_VAR("Start Up", ROTATINGSTAIR_UP);
    RSDK_ENUM_VAR("Start Left (Use Intervals)", ROTATINGSTAIR_LEFT_INTERVAL);
    RSDK_ENUM_VAR("Start Down (Use Intervals)", ROTATINGSTAIR_DOWN_INTERVAL);
    RSDK_ENUM_VAR("Start Right (Use Intervals)", ROTATINGSTAIR_RIGHT_INTERVAL);
    RSDK_ENUM_VAR("Start Up (Use Intervals)", ROTATINGSTAIR_UP_INTERVAL);

    RSDK_ACTIVE_VAR(RotatingStair, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flipped", FLIP_X);
}
#endif

void RotatingStair_Serialize(void)
{
    RSDK_EDITABLE_VAR(RotatingStair, VAR_ENUM, mode);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_UINT16, oscOff);
}
