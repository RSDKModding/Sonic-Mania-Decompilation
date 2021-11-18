#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Camera *PBL_Camera;

void PBL_Camera_Update(void) {}

void PBL_Camera_LateUpdate(void)
{
    RSDK_THIS(PBL_Camera);
    StateMachine_Run(self->state);
    PBL_Camera_Unknown1();
    self->targetPos = self->position;
    self->targetPos.y -= (0x100 << 16);
}

void PBL_Camera_StaticUpdate(void) {}

void PBL_Camera_Draw(void) {}

void PBL_Camera_Create(void *data)
{
    RSDK_THIS(PBL_Camera);
    self->active     = ACTIVE_NORMAL;
    self->state      = PBL_Camera_Unknown2;
    self->position.x = 0x400 << 16;
    self->angle      = 0x200;
    self->worldY     = 0x100 << 16;
    self->rotationY  = -0x60;
    self->position.y = self->dword68 + (0x700 << 16);
    self->dword6C    = 0x100 << 16;
    self->dword70    = 0x700 << 16;
    self->dword74    = 0x100 << 16;
    self->dword78    = 0x700 << 16;
}

void PBL_Camera_StageLoad(void)
{
    RSDK.ResetEntitySlot(SLOT_PBL_CAMERA, PBL_Camera->objectID, NULL);
    RSDK.ClearCameras();
    EntityPBL_Camera *entity = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
    RSDK.AddCamera(&entity->targetPos, 0x100 << 16, 0x100 << 16, true);
    PBL_Camera->flag = false;
}

void PBL_Camera_Unknown1(void)
{
    RSDK_THIS(PBL_Camera);
    int32 angle = RSDK.Cos1024(-self->rotationY) << 12;
    if (angle < 0x3C0000)
        angle = 0x3C0000;

    int32 ang  = self->angle - self->field_7C;
    int32 ang2 = ang - 0x400;
    if (self->angle <= 512)
        ang2 = ang + 0x400;

    if (abs(ang) >= abs(ang2))
        ScreenInfo->position.x -= 2 * ang2;
    else
        ScreenInfo->position.x -= 2 * ang;

    int32 height               = ((RSDK.Sin1024(-self->rotationY) << 12) << 8) / angle;
    ScreenInfo->position.y = height - ScreenInfo->centerY + 512;
    self->field_7C         = self->angle;
    self->field_80         = clampVal(ScreenInfo->centerY - height + 8, -64, ScreenInfo->height);
}

void PBL_Camera_Unknown2(void)
{
    RSDK_THIS(PBL_Camera);
    Entity *target = self->targetPtr;
    if (target) {
        if (target->position.y < self->position.y - 0x1500000) {
            self->position.y += maxVal(target->position.y - self->position.y + 0x1500000, -0x100000);
        }
        else if (target->position.y > self->position.y - 0xF00000) {
            self->position.y += minVal(target->position.y - self->position.y + 0xF00000, 0x100000);
        }

        if (self->position.y < self->dword6C + 0x2000000)
            self->position.y = self->dword6C + 0x2000000;
        if (self->dword6C != self->dword74) {
            self->dword6C += clampVal(self->dword74 - self->dword6C, -0x100000, 0x100000);
        }

        if (self->position.y > self->dword70 + 0x900000)
            self->position.y = self->dword70 + 0x900000;

        if (self->dword70 != self->dword78) {
            self->dword70 += clampVal(self->dword78 - self->dword70, -0x100000, 0x100000);
        }
    }
    RSDK.MatrixTranslateXYZ(&self->matrix, -self->position.x, -self->worldY, -self->position.y, true);
    RSDK.MatrixRotateXYZ(&PBL_Camera->matrix2, self->rotationY, self->angle, 0);
    RSDK.MatrixMultiply(&PBL_Camera->matrix1, &self->matrix, &PBL_Camera->matrix2);
    RSDK.MatrixScaleXYZ(&self->matrix, -0x100, 0x100, 0x100);
    RSDK.MatrixMultiply(&PBL_Camera->matrix2, &PBL_Camera->matrix2, &self->matrix);
    RSDK.MatrixRotateXYZ(&PBL_Camera->matrix3, self->rotationY + 8 * PBL_Setup->timer, self->angle, 0);
    RSDK.MatrixMultiply(&PBL_Camera->matrix3, &PBL_Camera->matrix3, &self->matrix);
    RSDK.MatrixMultiply(&PBL_Camera->matrix1, &PBL_Camera->matrix1, &self->matrix);
}

#if RETRO_INCLUDE_EDITOR
void PBL_Camera_EditorDraw(void) {}

void PBL_Camera_EditorLoad(void) {}
#endif

void PBL_Camera_Serialize(void) {}
#endif
