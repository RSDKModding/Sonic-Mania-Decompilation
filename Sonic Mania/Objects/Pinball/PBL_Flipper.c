#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Flipper *PBL_Flipper;

void PBL_Flipper_Update(void)
{
    RSDK_THIS(PBL_Flipper);

    if (entity->direction) {
        entity->buttonPress =
            RSDK_triggerL[1].key1.press || RSDK_controller[1].keyA.press || RSDK_controller[1].keyC.press || RSDK_controller[1].keyX.press;
        entity->buttonDown =
            RSDK_triggerL[1].key1.down || RSDK_controller[1].keyA.down || RSDK_controller[1].keyC.down || RSDK_controller[1].keyX.down;
    }
    else {
        entity->buttonPress =
            RSDK_triggerR[1].key1.press || RSDK_controller[1].keyA.press || RSDK_controller[1].keyB.press || RSDK_controller[1].keyC.press;
        entity->buttonDown =
            RSDK_triggerR[1].key1.down || RSDK_controller[1].keyA.down || RSDK_controller[1].keyB.down || RSDK_controller[1].keyC.down;
    }

    StateMachine_Run(entity->state);

    if (entity->retractable) {
        foreach_active(PBL_Player, player)
        {
            if (player->position.y >= entity->position.y - 0x100000 || entity->minCraneID > PBL_Setup->sectorID) {
                if (player->position.y > entity->position.y + 0x100000)
                    entity->field_170 = false;
            }
            else {
                entity->field_170 = true;
            }
        }

        if (entity->field_170) {
            if (entity->scale.y < 0x100)
                entity->scale.y += 0x10;
        }
        else if (entity->scale.y > 0) {
            entity->scale.y -= 0x10;
        }
    }
}

void PBL_Flipper_LateUpdate(void)
{
    RSDK_THIS(PBL_Flipper);
    int z       = entity->position.y;
    int y       = entity->height;
    int x       = entity->position.x;
    Matrix *mat = &PBL_Camera->matrix1;

    entity->depth3D = mat->values[2][1] * (y >> 16) + mat->values[2][2] * (z >> 16) + mat->values[2][0] * (x >> 16) + mat->values[2][3];
    if (entity->depth3D >= 0x4000) {
        int depth       = ((mat->values[0][3] << 8) + ((mat->values[0][2] * (z >> 8)) & 0xFFFFFF00) + ((mat->values[0][0] * (x >> 8)) & 0xFFFFFF00)
                     + ((mat->values[0][1] * (entity->height >> 8)) & 0xFFFFFF00));
        depth /= entity->depth3D;
        entity->visible = abs(depth) < 256;
    }
}

void PBL_Flipper_StaticUpdate(void) {}

void PBL_Flipper_Draw(void)
{
    RSDK_THIS(PBL_Flipper);
    if (entity->depth3D >= 0x4000 && entity->minCraneID <= PBL_Setup->sectorID) {
        RSDK.Prepare3DScene(PBL_Flipper->sceneIndex);
        RSDK.MatrixScaleXYZ(&entity->matrix1, entity->scale.x, entity->scale.y, 0x100);
        RSDK.MatrixTranslateXYZ(&entity->matrix1, entity->position.x, entity->height, entity->position.y, false);
        RSDK.MatrixRotateY(&entity->matrix3, entity->angle >> 8);
        RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix3, &entity->matrix1);
        RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix2, &PBL_Camera->matrix1);
        if (PBL_Camera->flag)
            RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &PBL_Camera->matrix3);
        else
            RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &PBL_Camera->matrix2);
        RSDK.AddModelTo3DScene(PBL_Flipper->aniFrames, PBL_Flipper->sceneIndex, PBL_Flipper->drawType, &entity->matrix2, &entity->matrix3, 0xFFFFFF);
        RSDK.Draw3DScene(PBL_Flipper->sceneIndex);
    }
}

void PBL_Flipper_Create(void *data)
{
    RSDK_THIS(PBL_Flipper);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible = true;
        if (entity->direction == FLIP_NONE)
            entity->scale.x = 0x100;
        else
            entity->scale.x = -0x100;
        entity->drawOrder     = 4;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->angle         = -0x4000;
        entity->scale.y       = 0x100;
        entity->field_170     = true;
        entity->state         = PBL_Flipper_Unknown2;
        RSDK.SetModelAnimation(PBL_Flipper->aniFrames, &entity->animator, 96, 0, true, 0);
    }
}

void PBL_Flipper_StageLoad(void)
{
    PBL_Flipper->aniFrames       = RSDK.LoadMesh("Pinball/Flipper.bin", SCOPE_STAGE);
    PBL_Flipper->sceneIndex      = RSDK.Create3DScene("View:Pinball", 0x1000, SCOPE_STAGE);
    PBL_Flipper->drawType        = S3D_FLATCLR_SHADED_BLENDED_SCREEN;
    PBL_Flipper->hitbox.left     = -52;
    PBL_Flipper->hitbox.top      = -8;
    PBL_Flipper->hitbox.right    = 12;
    PBL_Flipper->hitbox.bottom   = 8;
    PBL_Flipper->hitbox2.left    = -52;
    PBL_Flipper->hitbox2.top     = -24;
    PBL_Flipper->hitbox2.right   = 12;
    PBL_Flipper->hitbox2.bottom  = 8;
    PBL_Flipper->sfxFlipperStage = RSDK.GetSFX("Stage/Flipper.wav");
    PBL_Flipper->sfxFlipper      = RSDK.GetSFX("Pinball/Flipper.wav");
}

void PBL_Flipper_HandlePlayerInteractions(void)
{
    RSDK_THIS(PBL_TargetBumper);
    if (entity->scale.y >= 128) {
        int angle    = entity->angle >> 10;
        int negAngle = 0;
        if (entity->direction == FLIP_X) {
            negAngle = angle;
            angle    = -angle;
        }
        else {
            negAngle = -angle;
            angle    = angle;
        }

        Vector2 pos2;
        pos2.x = 0;
        pos2.y = 0;
        foreach_active(PBL_Player, player)
        {
            int distanceX = (player->position.x - entity->position.x) >> 8;
            int distanceY = (player->position.y - entity->position.y) >> 8;
            int posX                   = player->position.x;
            int posY                   = player->position.y;
            int velStoreX              = player->velocity.x;
            int velStoreY              = player->velocity.y;

            player->position.x         = distanceY * RSDK.Sin256(angle) + distanceX * RSDK.Cos256(angle) + entity->position.x;
            player->position.y         = distanceY * RSDK.Cos256(angle) - distanceX * RSDK.Sin256(angle) + entity->position.y;
            player->velocity.x         = (player->velocity.y >> 8) * RSDK.Sin256(angle) + (player->velocity.x >> 8) * RSDK.Cos256(angle);
            player->velocity.y         = (player->velocity.y >> 8) * RSDK.Cos256(angle) - (player->velocity.x >> 8) * RSDK.Sin256(angle);

            int velX = player->velocity.x;
            int velY = player->velocity.y;

            int distance = 0;
            if (entity->direction == FLIP_NONE)
                distance = entity->position.x - player->position.x;
            else
                distance = player->position.x - entity->position.x;
            int force = clampVal((distance + 0x80000) >> 13, 16, 256);

            switch (RSDK.CheckObjectCollisionBox(entity, &PBL_Flipper->hitbox, player, &PBL_Player->outerBox, true)) {
                case 0:
                    player->position.x = posX;
                    player->position.y = posY;
                    player->velocity.x = velStoreX;
                    player->velocity.y = velStoreY;
                    break;
                case 1:
                    if (velY < 0) {
                        player->velocity.x = velStoreX;
                        player->velocity.y = velStoreY;
                        Zone_Unknown3(&entity->position, &player->position, negAngle);
                    }
                    else {
                        if (velY > 0)
                            velY >>= 2;
                        if (velY > 0x80000)
                            velY = 0x80000;
                        player->onGround = false;
                        if (velY > 0x40000)
                            player->velocity.x >>= 2;
                        if (entity->state == PBL_Flipper_Unknown3 && entity->velocity.y == 0x2000) {
                            player->velocity.y -= (0xE0000 * force) >> 8;
                            player->onGround = false;
                            player->angle    = 0;
                        }
                        Zone_Unknown3(&pos2, &player->velocity, negAngle);
                        Zone_Unknown3(&entity->position, &player->position, negAngle);
                    }
                    break;
                case 2:
                    if (velX < 0) {
                        player->velocity.x = velX;
                    }
                    else {
                        player->velocity.x = -(velX >> 2);
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
                    }
                    break;
                case 3:
                    if (velX > 0) {
                        player->velocity.x = velX;
                    }
                    else {
                        player->velocity.x = -(velX >> 2);
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
                    break;
                case 4:
                    player->velocity.y = -(velY >> 2);
                    Zone_Unknown3(&entity->position, &player->position, negAngle);
                    Zone_Unknown3(&pos2, &player->velocity, negAngle);
                    break;
                default: break;
            }
        }
    }
}

void PBL_Flipper_Unknown2(void)
{
    RSDK_THIS(PBL_Flipper);
    if (entity->buttonDown) {
        entity->velocity.y = 0x2000;
        entity->state      = PBL_Flipper_Unknown3;
        RSDK.PlaySfx(PBL_Flipper->sfxFlipper, false, 255);
    }
    PBL_Flipper_HandlePlayerInteractions();
}

void PBL_Flipper_Unknown3(void)
{
    RSDK_THIS(PBL_Flipper);

    entity->angle += entity->velocity.y;
    if (entity->angle < 0x4000 || entity->velocity.y < 0) {
        if (entity->velocity.y < 0x2000)
            entity->velocity.y += 0x400;
    }
    else {
        entity->angle      = 0x4000;
        entity->velocity.y = -(entity->velocity.y >> 2);
        if (!entity->buttonDown) {
            entity->velocity.y = -0x2000;
            entity->state      = PBL_Flipper_Unknown4;
        }
    }
    PBL_Flipper_HandlePlayerInteractions();
}

void PBL_Flipper_Unknown4(void)
{
    RSDK_THIS(PBL_Flipper);
    entity->angle += entity->velocity.y;
    if (entity->angle > -0x4000 || entity->velocity.y > 0) {
        if (entity->velocity.y > -0x2000)
            entity->velocity.y -= 0x400;
    }
    else {
        entity->angle      = -0x4000;
        entity->velocity.y = -(entity->velocity.y >> 2);
        if (entity->velocity.y < 0x800) {
            entity->velocity.y = 0;
            entity->state      = PBL_Flipper_Unknown2;
        }
    }
    PBL_Flipper_HandlePlayerInteractions();
}

void PBL_Flipper_EditorDraw(void) {}

void PBL_Flipper_EditorLoad(void) {}

void PBL_Flipper_Serialize(void)
{
    RSDK_EDITABLE_VAR(PBL_Flipper, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PBL_Flipper, VAR_UINT8, minCraneID);
    RSDK_EDITABLE_VAR(PBL_Flipper, VAR_BOOL, retractable);
}
#endif
