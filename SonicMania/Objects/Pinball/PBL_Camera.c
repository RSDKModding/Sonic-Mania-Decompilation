// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PBL_Camera Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectPBL_Camera *PBL_Camera;

void PBL_Camera_Update(void) {}

void PBL_Camera_LateUpdate(void)
{
    RSDK_THIS(PBL_Camera);

    StateMachine_Run(self->state);

    PBL_Camera_HandleScreenPos();

    self->targetPos = self->position;
    self->targetPos.y -= (0x100 << 16);
}

void PBL_Camera_StaticUpdate(void) {}

void PBL_Camera_Draw(void) {}

void PBL_Camera_Create(void *data)
{
    RSDK_THIS(PBL_Camera);

    self->active = ACTIVE_NORMAL;
    self->state  = PBL_Camera_State_Normal;

    self->angle     = 0x200;
    self->rotationY = -0x60;

    self->position.x = 0x400 << 16;
    self->worldY     = 0x100 << 16;
    self->position.y = self->cameraStartOffset + (0x700 << 16);

    self->curCamBoundaryT = 0x100 << 16;
    self->curCamBoundaryB = 0x700 << 16;
    self->newCamBoundaryT = 0x100 << 16;
    self->newCamBoundaryB = 0x700 << 16;
}

void PBL_Camera_StageLoad(void)
{
    RSDK.ResetEntitySlot(SLOT_PBL_CAMERA, PBL_Camera->classID, NULL);

    RSDK.ClearCameras();

    EntityPBL_Camera *entity = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
    RSDK.AddCamera(&entity->targetPos, 0x100 << 16, 0x100 << 16, true);

    PBL_Camera->useAltMatNormal = false;
}

void PBL_Camera_HandleScreenPos(void)
{
    RSDK_THIS(PBL_Camera);

    int32 angle = RSDK.Cos1024(-self->rotationY) << 12;
    if (angle < 0x3C0000)
        angle = 0x3C0000;

    int32 ang  = self->angle - self->prevAngle;
    int32 ang2 = ang - 0x400;
    if (self->angle <= 0x200)
        ang2 = ang + 0x400;

    if (abs(ang) >= abs(ang2))
        ScreenInfo->position.x -= 2 * ang2;
    else
        ScreenInfo->position.x -= 2 * ang;

    int32 height           = ((RSDK.Sin1024(-self->rotationY) << 12) << 8) / angle;
    ScreenInfo->position.y = height - ScreenInfo->center.y + 512;
    self->prevAngle        = self->angle;
    self->centerY          = CLAMP(ScreenInfo->center.y - height + 8, -64, ScreenInfo->size.y);
}

void PBL_Camera_State_Normal(void)
{
    RSDK_THIS(PBL_Camera);

    Entity *target = self->target;
    if (target) {
        if (target->position.y < self->position.y - 0x1500000) {
            self->position.y += MAX(target->position.y - self->position.y + 0x1500000, -0x100000);
        }
        else if (target->position.y > self->position.y - 0xF00000) {
            self->position.y += MIN(target->position.y - self->position.y + 0xF00000, 0x100000);
        }

        if (self->position.y < self->curCamBoundaryT + 0x2000000)
            self->position.y = self->curCamBoundaryT + 0x2000000;

        if (self->curCamBoundaryT != self->newCamBoundaryT)
            self->curCamBoundaryT += CLAMP(self->newCamBoundaryT - self->curCamBoundaryT, -0x100000, 0x100000);

        if (self->position.y > self->curCamBoundaryB + 0x900000)
            self->position.y = self->curCamBoundaryB + 0x900000;

        if (self->curCamBoundaryB != self->newCamBoundaryB)
            self->curCamBoundaryB += CLAMP(self->newCamBoundaryB - self->curCamBoundaryB, -0x100000, 0x100000);
    }

    RSDK.MatrixTranslateXYZ(&self->matTransform, -self->position.x, -self->worldY, -self->position.y, true);
    RSDK.MatrixRotateXYZ(&PBL_Camera->matNormalItem, self->rotationY, self->angle, 0);
    RSDK.MatrixMultiply(&PBL_Camera->matWorld, &self->matTransform, &PBL_Camera->matNormalItem);

    RSDK.MatrixScaleXYZ(&self->matTransform, -0x100, 0x100, 0x100);
    RSDK.MatrixMultiply(&PBL_Camera->matNormalItem, &PBL_Camera->matNormalItem, &self->matTransform);

    RSDK.MatrixRotateXYZ(&PBL_Camera->matNormal, self->rotationY + 8 * PBL_Setup->timer, self->angle, 0);
    RSDK.MatrixMultiply(&PBL_Camera->matNormal, &PBL_Camera->matNormal, &self->matTransform);
    RSDK.MatrixMultiply(&PBL_Camera->matWorld, &PBL_Camera->matWorld, &self->matTransform);
}

#if GAME_INCLUDE_EDITOR
void PBL_Camera_EditorDraw(void) {}

void PBL_Camera_EditorLoad(void) {}
#endif

void PBL_Camera_Serialize(void) {}
#endif
