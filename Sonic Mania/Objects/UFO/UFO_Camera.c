#include "SonicMania.h"

ObjectUFO_Camera *UFO_Camera;

void UFO_Camera_Update(void) {}

void UFO_Camera_LateUpdate(void)
{
    RSDK_THIS(UFO_Camera);
    StateMachine_Run(entity->state);
    UFO_Camera_Unknown1();
}

void UFO_Camera_StaticUpdate(void) {}

void UFO_Camera_Draw(void) {}

void UFO_Camera_Create(void *data)
{
    RSDK_THIS(UFO_Camera);
    entity->active   = ACTIVE_NORMAL;
    entity->state    = UFO_Camera_Unknown2;
    entity->height   = 0x300000;
    entity->field_64 = 0x2800;

    foreach_all(UFO_Player, player)
    {
        player->camera = entity;
        entity->target = (Entity *)player;
    }
}

void UFO_Camera_StageLoad(void)
{
    RSDK.ResetEntitySlot(SLOT_UFO_CAMERA, UFO_Camera->objectID, 0);
    UFO_Camera->isSS7 = RSDK.CheckStageFolder("UFO7");
}

void UFO_Camera_Unknown1(void)
{
    RSDK_THIS(UFO_Camera);
    int32 val = RSDK.Cos1024(-entity->angleX) << 12;
    if (val < 0x3C0000)
        val = 0x3C0000;

    if (!val)
        val = 1;
    int32 div  = !val ? 1 : val;
    int32 val2 = entity->angle - entity->dword6C;
    int32 val3 = val2 - 0x400;
    if (entity->angle <= 0x200)
        val3 = val2 + 0x400;

    if (abs(val2) >= abs(val3))
        ScreenInfo->position.x -= 2 * val3;
    else
        ScreenInfo->position.x -= 2 * val2;

    int32 offset               = ((RSDK.Sin1024(-entity->angleX) << 12) << 8) / div;
    ScreenInfo->position.y = offset - ScreenInfo->centerY + 512;
    entity->dword6C          = entity->angle;

    entity->clipY = clampVal(ScreenInfo->centerY - offset + 8, -0x40, ScreenInfo->height);
}

void UFO_Camera_Unknown2(void)
{
    RSDK_THIS(UFO_Camera);
    EntityUFO_Player *target = (EntityUFO_Player *)entity->target;
    if (target) {
        int32 angle2   = (-0x200 - target->angle) & 0x3FF;
        int32 angleDif = angle2 - entity->angle;

        int32 val1, val2, val3;
        if (angleDif >= 0)
            val1 = angle2 - entity->angle;
        else
            val1 = entity->angle - angle2;

        if (angleDif + 0x400 >= 0)
            val2 = angleDif + 0x400;
        else
            val2 = entity->angle - angle2 - 0x400;

        if (angleDif - 0x400 >= 0)
            val3 = angleDif - 0x400;
        else
            val3 = entity->angle - angle2 + 0x400;
        if (val1 >= val3) {
            if (val3 < val2) {
                entity->angle = entity->angle + ((angleDif - 0x400) >> 3);
            }
            else {
                entity->angle = entity->angle + ((angleDif + 0x400) >> 3);
            }
        }
        else if (val1 < val2) {
            entity->angle = entity->angle + (angleDif >> 3);
        }
        else {
            entity->angle = entity->angle + ((angleDif + 0x400) >> 3);
        }

        entity->angle &= 0x3FF;
        if (target->state == UFO_Player_HandleTilt) {
            entity->angleX = -(target->height >> 18);
            int32 val         = entity->field_64 * RSDK.Cos1024(entity->angleX) >> 10;

            entity->position.x = target->position.x - val * RSDK.Sin1024(entity->angle);
            entity->position.y = target->position.y - val * RSDK.Cos1024(entity->angle);

            entity->height = (target->height >> 1) - (RSDK.Sin1024(entity->angleX) << 14) + 0x400000;
        }
        else {
            entity->angleX    = 0;
            entity->position.x = target->position.x - entity->field_64 * RSDK.Sin1024(entity->angle);
            entity->position.y = target->position.y - entity->field_64 * RSDK.Cos1024(entity->angle);
            entity->height     = (target->height >> 2) + 0x400000;
        }
    }

    RSDK.MatrixTranslateXYZ(&entity->matWorld, -entity->position.x, -entity->height, -entity->position.y, true);
    RSDK.MatrixRotateXYZ(&UFO_Camera->matView, entity->angleX, entity->angle, 0);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &entity->matWorld, &UFO_Camera->matView);
    RSDK.MatrixScaleXYZ(&entity->matWorld, -0x100, 0x100, 0x100);
    RSDK.MatrixMultiply(&UFO_Camera->matView, &UFO_Camera->matView, &entity->matWorld);
    if (UFO_Camera->isSS7) {
        RSDK.MatrixRotateXYZ(&UFO_Camera->matTemp, entity->angleX + 4 * UFO_Setup->timer, entity->angle, 0);
        RSDK.MatrixMultiply(&UFO_Camera->matTemp, &UFO_Camera->matTemp, &entity->matWorld);
    }
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &UFO_Camera->matWorld, &entity->matWorld);
}

void UFO_Camera_Unknown3(void)
{
    RSDK_THIS(UFO_Camera);
    entity->height += 0x20000;

    if (entity->angleX > -0x100)
        entity->angleX -= 4;

    entity->angle += 2;
    RSDK.MatrixTranslateXYZ(&entity->matWorld, -entity->position.x, -entity->height, -entity->position.y, true);
    RSDK.MatrixRotateXYZ(&UFO_Camera->matView, entity->angleX, entity->angle, 0);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &entity->matWorld, &UFO_Camera->matView);
    RSDK.MatrixScaleXYZ(&entity->matWorld, -0x100, 0x100, 0x100);
    RSDK.MatrixMultiply(&UFO_Camera->matView, &UFO_Camera->matView, &entity->matWorld);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &UFO_Camera->matWorld, &entity->matWorld);
}

void UFO_Camera_Unknown4(void)
{
    RSDK_THIS(UFO_Camera);
    entity->height += 0x80000;
    if (entity->angleX > -0x100)
        entity->angleX -= 8;

    entity->position.x += RSDK.Sin1024(entity->angle) << 8;
    entity->position.y += RSDK.Cos1024(entity->angle) << 8;
    RSDK.MatrixTranslateXYZ(&entity->matWorld, -entity->position.x, -entity->height, -entity->position.y, true);
    RSDK.MatrixRotateXYZ(&UFO_Camera->matView, entity->angleX, entity->angle, 0);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &entity->matWorld, &UFO_Camera->matView);
    RSDK.MatrixScaleXYZ(&entity->matWorld, -0x100, 0x100, 0x100);
    RSDK.MatrixMultiply(&UFO_Camera->matView, &UFO_Camera->matView, &entity->matWorld);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &UFO_Camera->matWorld, &entity->matWorld);
}

#if RETRO_INCLUDE_EDITOR
void UFO_Camera_EditorDraw(void) {}

void UFO_Camera_EditorLoad(void) {}
#endif

void UFO_Camera_Serialize(void) {}
