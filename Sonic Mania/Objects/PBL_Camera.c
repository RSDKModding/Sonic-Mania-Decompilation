#include "../SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Camera *PBL_Camera;

void PBL_Camera_Update(void)
{

}

void PBL_Camera_LateUpdate(void)
{
    RSDK_THIS(PBL_Camera);
    StateMachine_Run(entity->state);
    PBL_Camera_Unknown1();
    entity->pos.x = entity->position.x;
    entity->pos.y = entity->position.y;
    entity->pos.y -= 0x1000000;
}

void PBL_Camera_StaticUpdate(void)
{

}

void PBL_Camera_Draw(void)
{

}

void PBL_Camera_Create(void* data)
{
    RSDK_THIS(PBL_Camera);
    entity->active     = ACTIVE_NORMAL;
    entity->state           = PBL_Camera_Unknown2;
    entity->position.x = 0x4000000;
    entity->angle      = 0x200;
    entity->worldY          = 0x1000000;
    entity->rotationY       = -0x60;
    entity->position.y      = entity->dword68 + 0x7000000;
    entity->dword6C         = 0x1000000;
    entity->dword70         = 0x7000000;
    entity->dword74         = 0x1000000;
    entity->dword78         = 0x7000000;
}

void PBL_Camera_StageLoad(void)
{
    RSDK.ResetEntitySlot(SLOT_BSS_HUD, PBL_Camera->objectID, NULL);
    RSDK.ClearCameras();
    EntityPBL_Camera * entity = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
    RSDK.AddCamera(&entity->pos, 0x1000000, 0x1000000, true);
    PBL_Camera->flag = false;
}

void PBL_Camera_Unknown1(void)
{
    RSDK_THIS(PBL_Camera);
    int angle = RSDK.Cos1024(-entity->rotationY) << 12;
    if (angle < 0x3C0000)
        angle = 0x3C0000;
    if (!angle)
        angle = 1;

    int ang  = entity->angle - entity->field_7C;
    int ang2 = ang - 1024;
    if (entity->angle <= 512)
        ang2 = ang + 1024;
    ScreenInfo *screen = RSDK_screens;
    if (abs(ang) >= abs(ang2))
        screen->position.x -= 2 * ang2;
    else
        screen->position.x -= 2 * ang;
    int height         = ((RSDK.Sin1024(-entity->rotationY) << 12) << 8) / angle;
    screen->position.y = height - screen->centerY + 512;
    entity->field_7C   = entity->angle;
    entity->field_80   = screen->centerY - height + 8;
    if (entity->field_80 >= -64) {
        if (entity->field_80 > screen->height)
            entity->field_80 = screen->height;
    }
    else {
        entity->field_80 = -64;
    }
}

void PBL_Camera_Unknown2(void)
{
    RSDK_THIS(PBL_Camera);
    Entity* target = entity->targetPtr;
    if (target) {
        if (target->position.y < entity->position.y - 0x1500000) {
            int val = target->position.y - entity->position.y + 0x1500000;
            if (val < -0x100000)
                val - 0x100000;
            entity->position.y += val;
        }
        else if (target->position.y > entity->position.y - 0xF00000) {
            int val = target->position.y - entity->position.y + 0xF00000;
            if (val > 0x100000)
                val = 0x100000;
            entity->position.y += val;
        }

        if (entity->position.y < entity->dword6C + 0x2000000)
            entity->position.y = entity->dword6C + 0x2000000;
        if (entity->dword6C != entity->dword74) {
            int val = entity->dword74 - entity->dword6C;
            if (val > 0x100000) {
                val = 0x100000;
            }
            else if (val < -0x100000) {
                val = -0x100000;
            }
            entity->dword6C += val;
        }

        if (entity->position.y > entity->dword70 + 0x900000)
            entity->position.y = entity->dword70 + 0x900000;

        if (entity->dword70 != entity->dword78) {
            int val = entity->dword78 - entity->dword70;
            if (val > 0x100000) {
                val = 0x100000;
            }
            else if (val < -0x100000) {
                val = -0x100000;
            }
            entity->dword70 += val;
        }
    }
    RSDK.MatrixTranslateXYZ(&entity->matrix, -entity->position.x, -entity->worldY, -entity->position.y, true);
    RSDK.MatrixRotateXYZ(&PBL_Camera->matrix2, entity->rotationY, entity->angle, 0);
    RSDK.MatrixMultiply(&PBL_Camera->matrix1, &entity->matrix, &PBL_Camera->matrix2);
    RSDK.MatrixScaleXYZ(&entity->matrix, -256, 256, 256);
    RSDK.MatrixMultiply(&PBL_Camera->matrix2, &PBL_Camera->matrix2, &entity->matrix);
    RSDK.MatrixRotateXYZ(&PBL_Camera->matrix3, entity->rotationY + 8 * PBL_Setup->timer, entity->angle, 0);
    RSDK.MatrixMultiply(&PBL_Camera->matrix3, &PBL_Camera->matrix3, &entity->matrix);
    RSDK.MatrixMultiply(&PBL_Camera->matrix1, &PBL_Camera->matrix1, &entity->matrix);
}

void PBL_Camera_EditorDraw(void)
{

}

void PBL_Camera_EditorLoad(void)
{

}

void PBL_Camera_Serialize(void)
{

}
#endif
