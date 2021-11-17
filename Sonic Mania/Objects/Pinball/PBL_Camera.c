#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Camera *PBL_Camera;

void PBL_Camera_Update(void) {}

void PBL_Camera_LateUpdate(void)
{
    RSDK_THIS(PBL_Camera);
    StateMachine_Run(entity->state);
    PBL_Camera_Unknown1();
    entity->targetPos = entity->position;
    entity->targetPos.y -= (0x100 << 16);
}

void PBL_Camera_StaticUpdate(void) {}

void PBL_Camera_Draw(void) {}

void PBL_Camera_Create(void *data)
{
    RSDK_THIS(PBL_Camera);
    entity->active     = ACTIVE_NORMAL;
    entity->state      = PBL_Camera_Unknown2;
    entity->position.x = 0x400 << 16;
    entity->angle      = 0x200;
    entity->worldY     = 0x100 << 16;
    entity->rotationY  = -0x60;
    entity->position.y = entity->dword68 + (0x700 << 16);
    entity->dword6C    = 0x100 << 16;
    entity->dword70    = 0x700 << 16;
    entity->dword74    = 0x100 << 16;
    entity->dword78    = 0x700 << 16;
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
    int32 angle = RSDK.Cos1024(-entity->rotationY) << 12;
    if (angle < 0x3C0000)
        angle = 0x3C0000;
    //if (!angle)
    //    angle = 1;

    int32 ang  = entity->angle - entity->field_7C;
    int32 ang2 = ang - 0x400;
    if (entity->angle <= 512)
        ang2 = ang + 0x400;

    if (abs(ang) >= abs(ang2))
        ScreenInfo->position.x -= 2 * ang2;
    else
        ScreenInfo->position.x -= 2 * ang;

    int32 height               = ((RSDK.Sin1024(-entity->rotationY) << 12) << 8) / angle;
    ScreenInfo->position.y = height - ScreenInfo->centerY + 512;
    entity->field_7C         = entity->angle;
    entity->field_80         = clampVal(ScreenInfo->centerY - height + 8, -64, ScreenInfo->height);
}

void PBL_Camera_Unknown2(void)
{
    RSDK_THIS(PBL_Camera);
    Entity *target = entity->targetPtr;
    if (target) {
        if (target->position.y < entity->position.y - 0x1500000) {
            entity->position.y += maxVal(target->position.y - entity->position.y + 0x1500000, -0x100000);
        }
        else if (target->position.y > entity->position.y - 0xF00000) {
            entity->position.y += minVal(target->position.y - entity->position.y + 0xF00000, 0x100000);
        }

        if (entity->position.y < entity->dword6C + 0x2000000)
            entity->position.y = entity->dword6C + 0x2000000;
        if (entity->dword6C != entity->dword74) {
            entity->dword6C += clampVal(entity->dword74 - entity->dword6C, -0x100000, 0x100000);
        }

        if (entity->position.y > entity->dword70 + 0x900000)
            entity->position.y = entity->dword70 + 0x900000;

        if (entity->dword70 != entity->dword78) {
            entity->dword70 += clampVal(entity->dword78 - entity->dword70, -0x100000, 0x100000);
        }
    }
    RSDK.MatrixTranslateXYZ(&entity->matrix, -entity->position.x, -entity->worldY, -entity->position.y, true);
    RSDK.MatrixRotateXYZ(&PBL_Camera->matrix2, entity->rotationY, entity->angle, 0);
    RSDK.MatrixMultiply(&PBL_Camera->matrix1, &entity->matrix, &PBL_Camera->matrix2);
    RSDK.MatrixScaleXYZ(&entity->matrix, -0x100, 0x100, 0x100);
    RSDK.MatrixMultiply(&PBL_Camera->matrix2, &PBL_Camera->matrix2, &entity->matrix);
    RSDK.MatrixRotateXYZ(&PBL_Camera->matrix3, entity->rotationY + 8 * PBL_Setup->timer, entity->angle, 0);
    RSDK.MatrixMultiply(&PBL_Camera->matrix3, &PBL_Camera->matrix3, &entity->matrix);
    RSDK.MatrixMultiply(&PBL_Camera->matrix1, &PBL_Camera->matrix1, &entity->matrix);
}

#if RETRO_INCLUDE_EDITOR
void PBL_Camera_EditorDraw(void) {}

void PBL_Camera_EditorLoad(void) {}
#endif

void PBL_Camera_Serialize(void) {}
#endif
