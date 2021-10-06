#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_TargetBumper *PBL_TargetBumper;

void PBL_TargetBumper_Update(void)
{
    RSDK_THIS(PBL_TargetBumper);
    StateMachine_Run(entity->state);
}

void PBL_TargetBumper_LateUpdate(void)
{
    RSDK_THIS(PBL_TargetBumper);
    int x = entity->position.x;
    int y = entity->height;
    int z = entity->position.y;

    Matrix *mat = &PBL_Camera->matrix1;
    entity->depth3D = mat->values[2][1] * (y >> 16) + mat->values[2][2] * (z >> 16) + mat->values[2][0] * (x >> 16) + mat->values[2][3];
    if (entity->depth3D >= 0x4000) {
        int depth = ((mat->values[0][3] << 8) + ((mat->values[0][2] * (z >> 8)) & 0xFFFFFF00) + ((mat->values[0][0] * (x >> 8)) & 0xFFFFFF00)
                     + ((mat->values[0][1] * (entity->height >> 8)) & 0xFFFFFF00));
        depth /= entity->depth3D;
    }
}

void PBL_TargetBumper_StaticUpdate(void) {}

void PBL_TargetBumper_Draw(void)
{
    RSDK_THIS(PBL_TargetBumper);
    if (entity->depth3D >= 0x4000) {
        uint colour = RSDK.GetPaletteEntry(0, (entity->field_60 & 0xFF) - 80);
        RSDK.SetDiffuseColour(PBL_TargetBumper->sceneIndex, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, colour & 0xFF);
        RSDK.Prepare3DScene(PBL_TargetBumper->sceneIndex);
        RSDK.MatrixScaleXYZ(&entity->matrix2, entity->scale.x, entity->scale.y, 256);
        RSDK.MatrixTranslateXYZ(&entity->matrix2, entity->position.x, entity->height, entity->position.y, false);
        RSDK.MatrixRotateY(&entity->matrix3, entity->angle);
        RSDK.MatrixMultiply(&entity->matrix1, &entity->matrix3, &entity->matrix2);
        RSDK.MatrixMultiply(&entity->matrix1, &entity->matrix1, &PBL_Camera->matrix1);
        RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &PBL_Camera->matrix3);
        RSDK.AddModelTo3DScene(PBL_TargetBumper->modelFrames, PBL_TargetBumper->sceneIndex, PBL_TargetBumper->drawType, &entity->matrix1,
                               &entity->matrix3, 0xFFFFFF);
        RSDK.Draw3DScene(PBL_TargetBumper->sceneIndex);
    }
}

void PBL_TargetBumper_Create(void *data)
{
    RSDK_THIS(PBL_TargetBumper);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = 4;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->scale.x       = 0x100;
        entity->scale.y       = 0x100;
        entity->state         = PBL_TargetBumper_Unknown2;
        entity->field_60      = 2 * (RSDK.GetEntityID(entity) % 3);
        RSDK.SetModelAnimation(PBL_TargetBumper->modelFrames, &entity->animator, 96, 0, true, 0);
    }
}

void PBL_TargetBumper_StageLoad(void)
{
    PBL_TargetBumper->modelFrames = RSDK.LoadMesh("Pinball/TargetBumper.bin", SCOPE_STAGE);
    PBL_TargetBumper->sceneIndex  = RSDK.Create3DScene("View:TargetBumper", 256u, SCOPE_STAGE);
    RSDK.SetDiffuseIntensity(PBL_TargetBumper->sceneIndex, 9, 9, 9);
    RSDK.SetSpecularIntensity(PBL_TargetBumper->sceneIndex, 15, 15, 15);
    PBL_TargetBumper->drawType        = S3D_FLATCLR_SHADED_BLENDED_SCREEN;
    PBL_TargetBumper->hitbox.left     = -16;
    PBL_TargetBumper->hitbox.top      = -6;
    PBL_TargetBumper->hitbox.right    = 12;
    PBL_TargetBumper->hitbox.bottom   = 6;
    PBL_TargetBumper->sfxTargetBumper = RSDK.GetSFX("Pinball/TargetBumper.wav");
    PBL_TargetBumper->sfxFlipper      = RSDK.GetSFX("Pinball/Flipper.wav");
}

void PBL_TargetBumper_HandlePlayerInteractions(void)
{
    RSDK_THIS(PBL_TargetBumper);
    if (entity->scale.y >= 128) {
        int angle    = entity->angle >> 2;
        int negAngle = -angle;

        Vector2 pos2;
        pos2.x = 0;
        pos2.y = 0;
        foreach_active(PBL_Player, player)
        {
            int distanceX = (player->position.x - entity->position.x) >> 8;
            int distanceY = (player->position.y - entity->position.y) >> 8;
            int posX      = player->position.x;
            int posY      = player->position.y;
            int velStoreX = player->velocity.x;
            int velStoreY = player->velocity.y;

            player->position.x = distanceY * RSDK.Sin256(angle) + distanceX * RSDK.Cos256(angle) + entity->position.x;
            player->position.y = distanceY * RSDK.Cos256(angle) - distanceX * RSDK.Sin256(angle) + entity->position.y;
            player->velocity.x = (player->velocity.y >> 8) * RSDK.Sin256(angle) + (player->velocity.x >> 8) * RSDK.Cos256(angle);
            player->velocity.y = (player->velocity.y >> 8) * RSDK.Cos256(angle) - (player->velocity.x >> 8) * RSDK.Sin256(angle);

            int velX = player->velocity.x;
            int velY = player->velocity.y;
            switch (RSDK.CheckObjectCollisionBox(entity, &PBL_TargetBumper->hitbox, player, &PBL_Player->outerBox, true)) {
                case 0:
                    player->position.x = posX;
                    player->position.y = posY;
                    player->velocity.x = velStoreX;
                    player->velocity.y = velStoreY;
                    break;
                case 1:
                case 4:
                    player->onGround = false;
                    if (abs(velY) < 0x40000)
                        velY *= 2;
                    player->velocity.y = -velY;
                    Zone_Unknown3(&entity->position, &player->position, negAngle);
                    Zone_Unknown3(&pos2, &player->velocity, negAngle);
                    entity->state      = PBL_TargetBumper_Unknown3;
                    entity->velocity.y = -8;
                    PBL_Setup_GiveScore(1000);
                    RSDK.PlaySfx(PBL_TargetBumper->sfxTargetBumper, false, 255);
                    break;
                case 2:
                    if (velX < 0) {
                        player->velocity.x = velX;
                    }
                    else {
                        player->velocity.x = -(velX >> 1);
                        if (player->velocity.x <= -0x10000) {
                            if (player->velocity.x < -0x80000)
                                player->velocity.x = -0x80000;
                            player->velocity.y -= 0x20000;
                        }
                        else {
                            player->velocity.x = -0x10000;
                            player->velocity.y -= 0x20000;
                        }

                        Zone_Unknown3(&entity->position, &player->position, negAngle);
                        Zone_Unknown3(&pos2, &player->velocity, negAngle);
                        player->onGround = false;
                    }
                    break;
                case 3:
                    if (velX > 0) {
                        player->velocity.x = velX;
                    }
                    else {
                        player->velocity.x = -(velX >> 1);
                        if (player->velocity.x >= 0x10000) {
                            if (player->velocity.x > 0x80000)
                                player->velocity.x = 0x80000;
                            player->velocity.y -= 0x20000;
                        }
                        else {
                            player->velocity.x = 0x10000;
                            player->velocity.y -= 0x20000;
                        }
                    }

                    Zone_Unknown3(&entity->position, &player->position, negAngle);
                    Zone_Unknown3(&pos2, &player->velocity, negAngle);
                    player->onGround = false;
                    break;
                default: break;
            }
        }
    }
}

void PBL_TargetBumper_Unknown2(void) { PBL_TargetBumper_HandlePlayerInteractions(); }

void PBL_TargetBumper_Unknown3(void)
{
    RSDK_THIS(PBL_TargetBumper);
    entity->velocity.y += 2;
    entity->scale.y -= entity->velocity.y;
    if (entity->scale.y <= 0) {
        entity->velocity.y = 0;
        entity->scale.y    = 0;
        entity->state      = StateMachine_None;
    }
}

void PBL_TargetBumper_Unknown4(void)
{
    RSDK_THIS(PBL_TargetBumper);
    entity->velocity.y -= 2;
    entity->scale.y += entity->velocity.y;
    if (entity->velocity.y < 0 && entity->scale.y <= 0x100) {
        entity->active     = ACTIVE_BOUNDS;
        entity->velocity.y = 0;
        entity->scale.y    = 256;
        entity->state      = PBL_TargetBumper_Unknown2;
    }
}

void PBL_TargetBumper_EditorDraw(void) {}

void PBL_TargetBumper_EditorLoad(void) {}

void PBL_TargetBumper_Serialize(void) { RSDK_EDITABLE_VAR(PBL_TargetBumper, VAR_ENUM, angle); }
#endif
