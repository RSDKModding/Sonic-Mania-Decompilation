#include "SonicMania.h"

ObjectUFO_Sphere *UFO_Sphere;

void UFO_Sphere_Update(void)
{
    RSDK_THIS(UFO_Sphere);
    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animator);
}

void UFO_Sphere_LateUpdate(void)
{
    RSDK_THIS(UFO_Sphere);
    int x = entity->position.x >> 8;
    int y = entity->height >> 8;
    int z = entity->position.y >> 8;

    Matrix *mat = &UFO_Camera->matWorld;

    entity->worldPos.x = mat->values[0][3] + (y * mat->values[0][1] >> 8) + (z * mat->values[0][2] >> 8) + (x * mat->values[0][0] >> 8);
    entity->worldPos.y = mat->values[1][3] + (y * mat->values[1][1] >> 8) + (z * mat->values[1][2] >> 8) + (x * mat->values[1][0] >> 8);
    entity->depth      = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);
}

void UFO_Sphere_StaticUpdate(void) {}

void UFO_Sphere_Draw(void)
{
    RSDK_THIS(UFO_Sphere);
    if (entity->drawOrder == 4) {
        if (entity->depth < 0x100)
            return;

        entity->direction = entity->animator.frameID > 8;
        entity->drawPos.x = (RSDK_screens->centerX + (entity->worldPos.x << 8) / entity->depth) << 16;
        entity->drawPos.y = (RSDK_screens->centerY - (entity->worldPos.y << 8) / entity->depth) << 16;
        entity->scale.x   = entity->dword9C / entity->depth;
        entity->scale.y   = entity->dword9C / entity->depth;
    }
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, true);
}

void UFO_Sphere_Create(void *data)
{
    RSDK_THIS(UFO_Sphere);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawFX        = FX_SCALE | FX_FLIP;
        entity->drawOrder     = 4;
        entity->active        = ACTIVE_RBOUNDS;
        entity->updateRange.x = 0x400;
        entity->updateRange.y = 0x400;
        entity->dword9C       = 0x1000000;
        if (!entity->height)
            entity->height = UFO_Sphere->defaultHeights[entity->type];
        entity->height <<= 0x10;

        switch (entity->behavior) {
            default:
            case 0: //static
                entity->state = UFO_Sphere_CheckPlayerCollision; 
                break;
            case 1:
                entity->velocity.y = 0x20000;
                entity->state      = UFO_Sphere_State_Bouncing;
                break;
            case 2:
                entity->amplitude.x >>= 8;
                entity->amplitude.y >>= 8;
                entity->dword84 = entity->position.x;
                entity->dword88 = entity->position.y;
                entity->dword80 = entity->height;
                entity->state   = UFO_Sphere_State_Moving;

                Matrix matrix;
                RSDK.MatrixRotateX(&matrix, entity->matAngle.x);
                RSDK.MatrixRotateY(&entity->matrix, entity->matAngle.y);
                RSDK.MatrixMultiply(&entity->matrix, &matrix, &entity->matrix);
                break;
        }
        RSDK.SetSpriteAnimation(UFO_Sphere->spriteIndex, entity->type, &entity->animator, true, 0);
    }
}

void UFO_Sphere_StageLoad(void)
{
    UFO_Sphere->spriteIndex    = RSDK.LoadSpriteAnimation("SpecialUFO/Spheres.bin", SCOPE_STAGE);
    UFO_Sphere->sfx_MachSpeed  = RSDK.GetSFX("Special/MachSpeed.wav");
    UFO_Sphere->sfx_LedgeBreak = RSDK.GetSFX("Stage/LedgeBreak.wav");
}

void UFO_Sphere_CheckPlayerCollision(void)
{
    RSDK_THIS(UFO_Sphere);

    switch (entity->type) {
        case 0: {
            foreach_active(UFO_Player, player)
            {
                int ry     = (entity->height - player->height - 0xA0000) >> 16;
                int rx     = (entity->position.y - player->position.y) >> 16;
                int radius = rx * rx + ry * ry;
                int pr     = UFO_Player->maxSpeed >> 9;
                if (((entity->position.x - player->position.x) >> 16) * ((entity->position.x - player->position.x) >> 16) + radius < pr) {
                    entity->drawOrder = 12;
                    entity->state     = UFO_Sphere_Unknown3;
                    UFO_Setup_PlaySphereSFX();
                }
            }

            if (UFO_Setup->machLevel == 2) {
                entity->animator.animationSpeed = 1;
            }
            break;
        }
        case 1: 
            //unused (red)
            break;
        case 2: {
            foreach_active(UFO_Player, player)
            {
                if (player->state != UFO_Player_Unknown8 && player->state != UFO_Player_HandleTilt && player->state != UFO_Player_Unknown10
                    && player->state != UFO_Player_Unknown11) {
                    int ry = (entity->height - player->height - 0xA0000) >> 16;
                    int rx = (entity->position.y - player->position.y) >> 16;
                    if (((entity->position.x - player->position.x) >> 16) * ((entity->position.x - player->position.x) >> 16) + rx * rx + ry * ry
                        < 384) {
                        RSDK.SetModelAnimation(UFO_Player->tumbleModel, &player->playerData, 80, 0, false, 0);
                        player->state = UFO_Player_Unknown8;
                        if (UFO_Setup->rings > 0)
                            RSDK.PlaySFX(UFO_Player->sfx_LoseRings, 0, 255);
                        UFO_Ring_LoseRings(player);
                        RSDK.SetSpriteAnimation(UFO_Sphere->spriteIndex, 4, &entity->animator, true, 0);
                        entity->dword9C = 0x1800000;
                        entity->state   = UFO_Sphere_Unknown5;
                        RSDK.PlaySFX(UFO_Sphere->sfx_LedgeBreak, 0, 255);
                    }
                }
            }
            break;
        }
        case 3: {
            foreach_active(UFO_Player, player)
            {
                if (player->state != UFO_Player_Unknown8 && player->state != UFO_Player_HandleTilt && player->state != UFO_Player_Unknown10
                    && player->state != UFO_Player_Unknown11) {
                    int ry = (entity->height - player->height - 0xA0000) >> 16;
                    int rx = (entity->position.y - player->position.y) >> 16;
                    if (((entity->position.x - player->position.x) >> 16) * ((entity->position.x - player->position.x) >> 16) + rx * rx + ry * ry
                        < 384) {
                        RSDK.SetModelAnimation(UFO_Player->tumbleModel, &player->playerData, 80, 0, false, 0);
                        player->state = UFO_Player_Unknown8;
                        if (UFO_Setup->rings > 0)
                            RSDK.PlaySFX(UFO_Player->sfx_LoseRings, 0, 255);
                        UFO_Ring_LoseRings(player);
                    }
                }
            }
            break;
        }
    }
}

void UFO_Sphere_State_Bouncing(void)
{
    RSDK_THIS(UFO_Sphere);
    entity->velocity.y -= 0x3800;
    entity->height += entity->velocity.y;
    if (entity->height < 0xC0000) {
        entity->velocity.y = 0x20000;
        entity->height     = 0xC0000;
    }
    UFO_Sphere_CheckPlayerCollision();
}

void UFO_Sphere_State_Moving(void)
{
    RSDK_THIS(UFO_Sphere);
    Matrix *mat = &entity->matrix;

    int amp            = entity->amplitude.y * RSDK.Sin256(entity->angle + (UFO_Setup->timer << entity->speed));
    entity->position.x = entity->amplitude.x * RSDK.Cos256(entity->angle + (UFO_Setup->timer << entity->speed));
    entity->position.y = entity->amplitude.y * amp;
    entity->height     = 0;
    amp >>= 8;

    entity->position.x = mat->values[0][3] + amp * mat->values[0][2] + mat->values[0][0] * (entity->position.x >> 8);
    entity->height     = mat->values[1][3] + amp * mat->values[1][2] + mat->values[1][0] * (entity->position.x >> 8);
    entity->position.y = mat->values[2][3] + amp * mat->values[2][2] + mat->values[2][0] * (entity->position.x >> 8) + mat->values[2][1] * (entity->height >> 8);

    entity->position.x += entity->dword84;
    entity->position.y += entity->dword88;
    entity->height += entity->dword80;
    UFO_Sphere_CheckPlayerCollision();
}

void UFO_Sphere_Unknown3(void)
{
    RSDK_THIS(UFO_Sphere);

    entity->drawPos.x += ((((RSDK_screens->centerX - 38) << 16) - entity->drawPos.x) >> 3);
    entity->drawPos.y += ((0x1C0000 - entity->drawPos.y) >> 3);
    entity->scale.x = entity->scale.x + ((128 - entity->scale.x) >> 3);
    entity->scale.y = entity->scale.x + ((128 - entity->scale.x) >> 3);
    if (entity->drawPos.y < 0x200000) {
        ++UFO_Setup->machPoints;
        UFO_HUD_CheckLevelUp();
        entity->timer   = UFO_Setup->timer;
        entity->visible = false;
        entity->state   = UFO_Sphere_Unknown4;
    }
}

void UFO_Sphere_Unknown4(void)
{
    RSDK_THIS(UFO_Sphere);

    if (abs(entity->timer - UFO_Setup->timer) > 240) {
        entity->visible   = true;
        entity->drawOrder = 4;

        switch (entity->behavior) {
            default:
            case 0:
            case 3: entity->state = UFO_Sphere_CheckPlayerCollision; break;
            case 1: entity->state = UFO_Sphere_State_Bouncing; break;
            case 2: entity->state = UFO_Sphere_State_Moving; break;
        }
    }
}

void UFO_Sphere_Unknown5(void)
{
    RSDK_THIS(UFO_Sphere);

    if (entity->animator.frameID == entity->animator.frameCount - 1)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
}

void UFO_Sphere_EditorDraw(void) {}

void UFO_Sphere_EditorLoad(void) {}

void UFO_Sphere_Serialize(void)
{
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_ENUM, behavior);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_ENUM, height);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_VECTOR2, matAngle);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_VECTOR2, amplitude);
}
