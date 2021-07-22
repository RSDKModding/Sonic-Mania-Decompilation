#include "SonicMania.h"

ObjectUFO_Springboard *UFO_Springboard;

void UFO_Springboard_Update(void)
{
    RSDK_THIS(UFO_Springboard);
    foreach_active(UFO_Player, player)
    {
        if (player->bumperTimer <= 12 && player->stateInput) {
            int xDif = (player->position.x - entity->position.x) >> 8;
            int yDif = (player->position.y - entity->position.y) >> 8;
            int valX  = (yDif * RSDK.Sin256(entity->angle >> 2)) + xDif * RSDK.Cos256(entity->angle >> 2) + entity->position.x;
            int valY = (entity->position.y - xDif * RSDK.Sin256(entity->angle >> 2)) + yDif * RSDK.Cos256(entity->angle >> 2);

            RSDK.Sin256(entity->angle >> 2);
            RSDK.Cos256(entity->angle >> 2);
            RSDK.Cos256(entity->angle >> 2);
            RSDK.Sin256(entity->angle >> 2);
            if (abs(entity->position.x - valX) < 0x180000) {
                if (abs(entity->position.y - valY) < 0x180000) {
                    int val3 = 32 * ((valY - entity->position.y + 0x180000) / 48);
                    if (player->height < val3) {
                        if (val3 - player->height <= 0xC0000) {
                            player->gravityStrength = 0xC0000;
                            player->onGround        = 0;
                            player->state           = UFO_Player_HandleTilt;
                            RSDK.SetModelAnimation(UFO_Player->jumpModel, &player->playerData, 128, 0, true, 0);
                            RSDK.PlaySFX(UFO_Player->sfx_Spring, 0, 255);
                        }
                        else {
                            player->bumperTimer = 16;
                            player->velocity.x  = -player->velocity.x;
                            player->velocity.y  = -player->velocity.y;
                            RSDK.PlaySFX(UFO_Player->sfx_Bumper, 0, 255);
                        }
                    }
                }
            }
        }
    }

    if (RSDK.CheckOnScreen(entity, 0)) {
        if (entity->scale.x < 256)
            entity->scale.x += 16;
        entity->active = ACTIVE_NORMAL;
    }
    else {
        entity->scale.x = 0;
        entity->active  = ACTIVE_BOUNDS;
    }
}

void UFO_Springboard_LateUpdate(void)
{
    RSDK_THIS(UFO_Springboard);
    int z       = entity->position.y;
    int y       = entity->height;
    int x       = entity->position.x;
    Matrix *mat = &UFO_Camera->matWorld;

    entity->depth = mat->values[2][1] * (y >> 16) + mat->values[2][2] * (z >> 16) + mat->values[2][0] * (x >> 16) + mat->values[2][3];
    if (entity->depth >= 0x4000) {
        int depth = (int)((mat->values[0][3] << 8) + (mat->values[0][2] * (z >> 8) & 0xFFFFFF00) + (mat->values[0][0] * (x >> 8) & 0xFFFFFF00)
                     + (mat->values[0][1] * (entity->height >> 8) & 0xFFFFFF00))
                    / entity->depth;
        entity->visible = abs(depth) < 256;
    }
}

void UFO_Springboard_StaticUpdate(void) {}

void UFO_Springboard_Draw(void)
{
    RSDK_THIS(UFO_Springboard);
    if (entity->depth >= 0x4000) {
        RSDK.Prepare3DScene(UFO_Springboard->sceneIndex);
        RSDK.MatrixScaleXYZ(&entity->matrix1, 0x100, entity->scale.x, 0x100);
        RSDK.MatrixTranslateXYZ(&entity->matrix1, entity->position.x, entity->height, entity->position.y, 0);
        RSDK.MatrixRotateY(&entity->matrix3, entity->angle);
        RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix3, &entity->matrix1);
        RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix2, &UFO_Camera->matWorld);
        RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &UFO_Camera->matView);
        RSDK.AddMeshFrameTo3DScene(UFO_Springboard->modelIndex, UFO_Springboard->sceneIndex, &entity->animator, S3D_FLATCLR_SHADED_BLENDED_SCREEN,
                                   &entity->matrix2, &entity->matrix3, 0xFFFFFF);
        RSDK.Draw3DScene(UFO_Springboard->sceneIndex);
    }
}

void UFO_Springboard_Create(void *data)
{
    RSDK_THIS(UFO_Springboard);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = 4;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x5000000;
        entity->updateRange.y = 0x5000000;
    }
}

void UFO_Springboard_StageLoad(void)
{
    UFO_Springboard->modelIndex = RSDK.LoadMesh("Special/Springboard.bin", SCOPE_STAGE);
    UFO_Springboard->sceneIndex = RSDK.Create3DScene("View:Special", 4096, SCOPE_STAGE);
}

void UFO_Springboard_EditorDraw(void) {}

void UFO_Springboard_EditorLoad(void) {}

void UFO_Springboard_Serialize(void)
{
    RSDK_EDITABLE_VAR(UFO_Springboard, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(UFO_Springboard, VAR_UINT8, rampCount);
}
