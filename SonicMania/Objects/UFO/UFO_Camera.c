// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_Camera Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUFO_Camera *UFO_Camera;

void UFO_Camera_Update(void) {}

void UFO_Camera_LateUpdate(void)
{
    RSDK_THIS(UFO_Camera);

    StateMachine_Run(self->state);

    UFO_Camera_HandleCamPos();
}

void UFO_Camera_StaticUpdate(void) {}

void UFO_Camera_Draw(void) {}

void UFO_Camera_Create(void *data)
{
    RSDK_THIS(UFO_Camera);

    self->active = ACTIVE_NORMAL;
    self->state  = UFO_Camera_State_Normal;
    self->height = 0x300000;
    self->radius = 0x2800;

    foreach_all(UFO_Player, player)
    {
        player->camera = self;
        self->target   = (Entity *)player;
    }
}

void UFO_Camera_StageLoad(void)
{
    RSDK.ResetEntitySlot(SLOT_UFO_CAMERA, UFO_Camera->classID, NULL);

    UFO_Camera->isSS7 = RSDK.CheckSceneFolder("UFO7");
}

void UFO_Camera_HandleCamPos(void)
{
    RSDK_THIS(UFO_Camera);

    int32 cos = MAX(RSDK.Cos1024(-self->angleX) << 12, 0x3C0000);

    int32 angle  = self->angle - self->prevAngle;
    int32 angle2 = angle - 0x400;
    if (self->angle <= 0x200)
        angle2 = angle + 0x400;

    if (abs(angle) >= abs(angle2))
        ScreenInfo->position.x -= 2 * angle2;
    else
        ScreenInfo->position.x -= 2 * angle;

    int32 offset           = ((RSDK.Sin1024(-self->angleX) << 12) << 8) / cos;
    ScreenInfo->position.y = offset - ScreenInfo->center.y + 512;
    self->prevAngle        = self->angle;

    self->clipY = CLAMP(ScreenInfo->center.y - offset + 8, -0x40, ScreenInfo->size.y);
}

void UFO_Camera_State_Normal(void)
{
    RSDK_THIS(UFO_Camera);

    EntityUFO_Player *target = (EntityUFO_Player *)self->target;

    if (target) {
        int32 negAngle = (-0x200 - target->angle) & 0x3FF;
        int32 rot      = negAngle - self->angle;

        if (abs(rot) >= abs(rot - 0x400)) {
            if (abs(rot - 0x400) < abs(rot + 0x400))
                self->angle += (rot - 0x400) >> 3;
            else
                self->angle += (rot + 0x400) >> 3;
        }
        else {
            if (abs(rot) < abs(rot + 0x400))
                self->angle += rot >> 3;
            else
                self->angle += (rot + 0x400) >> 3;
        }

        self->angle &= 0x3FF;
        if (target->state == UFO_Player_State_Springboard) {
            self->angleX = -(target->height >> 18);
            int32 rad    = self->radius * RSDK.Cos1024(self->angleX) >> 10;

            self->position.x = target->position.x - rad * RSDK.Sin1024(self->angle);
            self->position.y = target->position.y - rad * RSDK.Cos1024(self->angle);

            self->height = (target->height >> 1) - (RSDK.Sin1024(self->angleX) << 14) + 0x400000;
        }
        else {
            self->angleX     = 0;
            self->position.x = target->position.x - self->radius * RSDK.Sin1024(self->angle);
            self->position.y = target->position.y - self->radius * RSDK.Cos1024(self->angle);
            self->height     = (target->height >> 2) + 0x400000;
        }
    }

    RSDK.MatrixTranslateXYZ(&self->matWorld, -self->position.x, -self->height, -self->position.y, true);
    RSDK.MatrixRotateXYZ(&UFO_Camera->matView, self->angleX, self->angle, 0);

    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &self->matWorld, &UFO_Camera->matView);
    RSDK.MatrixScaleXYZ(&self->matWorld, -0x100, 0x100, 0x100);

    RSDK.MatrixMultiply(&UFO_Camera->matView, &UFO_Camera->matView, &self->matWorld);

    if (UFO_Camera->isSS7) {
        RSDK.MatrixRotateXYZ(&UFO_Camera->matTemp, self->angleX + 4 * UFO_Setup->timer, self->angle, 0);
        RSDK.MatrixMultiply(&UFO_Camera->matTemp, &UFO_Camera->matTemp, &self->matWorld);
    }

    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &UFO_Camera->matWorld, &self->matWorld);
}

void UFO_Camera_State_CourseOut(void)
{
    RSDK_THIS(UFO_Camera);

    self->height += 0x20000;

    if (self->angleX > -0x100)
        self->angleX -= 4;

    self->angle += 2;

    RSDK.MatrixTranslateXYZ(&self->matWorld, -self->position.x, -self->height, -self->position.y, true);
    RSDK.MatrixRotateXYZ(&UFO_Camera->matView, self->angleX, self->angle, 0);

    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &self->matWorld, &UFO_Camera->matView);
    RSDK.MatrixScaleXYZ(&self->matWorld, -0x100, 0x100, 0x100);

    RSDK.MatrixMultiply(&UFO_Camera->matView, &UFO_Camera->matView, &self->matWorld);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &UFO_Camera->matWorld, &self->matWorld);
}

void UFO_Camera_State_UFOCaught(void)
{
    RSDK_THIS(UFO_Camera);

    self->height += 0x80000;
    if (self->angleX > -0x100)
        self->angleX -= 8;

    self->position.x += RSDK.Sin1024(self->angle) << 8;
    self->position.y += RSDK.Cos1024(self->angle) << 8;

    RSDK.MatrixTranslateXYZ(&self->matWorld, -self->position.x, -self->height, -self->position.y, true);
    RSDK.MatrixRotateXYZ(&UFO_Camera->matView, self->angleX, self->angle, 0);

    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &self->matWorld, &UFO_Camera->matView);
    RSDK.MatrixScaleXYZ(&self->matWorld, -0x100, 0x100, 0x100);

    RSDK.MatrixMultiply(&UFO_Camera->matView, &UFO_Camera->matView, &self->matWorld);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &UFO_Camera->matWorld, &self->matWorld);
}

#if GAME_INCLUDE_EDITOR
void UFO_Camera_EditorDraw(void) {}

void UFO_Camera_EditorLoad(void) {}
#endif

void UFO_Camera_Serialize(void) {}
