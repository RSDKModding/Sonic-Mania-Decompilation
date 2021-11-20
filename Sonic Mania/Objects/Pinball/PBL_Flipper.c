#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Flipper *PBL_Flipper;

void PBL_Flipper_Update(void)
{
    RSDK_THIS(PBL_Flipper);

    if (self->direction) {
        self->buttonPress =
            TriggerInfoL[1].key1.press || ControllerInfo[1].keyA.press || ControllerInfo[1].keyC.press || ControllerInfo[1].keyX.press;
        self->buttonDown =
            TriggerInfoL[1].key1.down || ControllerInfo[1].keyA.down || ControllerInfo[1].keyC.down || ControllerInfo[1].keyX.down;
    }
    else {
        self->buttonPress =
            TriggerInfoR[1].key1.press || ControllerInfo[1].keyA.press || ControllerInfo[1].keyB.press || ControllerInfo[1].keyC.press;
        self->buttonDown =
            TriggerInfoR[1].key1.down || ControllerInfo[1].keyA.down || ControllerInfo[1].keyB.down || ControllerInfo[1].keyC.down;
    }

    StateMachine_Run(self->state);

    if (self->retractable) {
        foreach_active(PBL_Player, player)
        {
            if (player->position.y >= self->position.y - 0x100000 || self->minCraneID > PBL_Setup->sectorID) {
                if (player->position.y > self->position.y + 0x100000)
                    self->field_170 = false;
            }
            else {
                self->field_170 = true;
            }
        }

        if (self->field_170) {
            if (self->scale.y < 0x100)
                self->scale.y += 0x10;
        }
        else if (self->scale.y > 0) {
            self->scale.y -= 0x10;
        }
    }
}

void PBL_Flipper_LateUpdate(void)
{
    RSDK_THIS(PBL_Flipper);
    int32 z       = self->position.y;
    int32 y       = self->height;
    int32 x       = self->position.x;
    Matrix *mat = &PBL_Camera->matrix1;

    self->depth3D = mat->values[2][1] * (y >> 16) + mat->values[2][2] * (z >> 16) + mat->values[2][0] * (x >> 16) + mat->values[2][3];
    if (self->depth3D >= 0x4000) {
        int32 depth       = ((mat->values[0][3] << 8) + ((mat->values[0][2] * (z >> 8)) & 0xFFFFFF00) + ((mat->values[0][0] * (x >> 8)) & 0xFFFFFF00)
                     + ((mat->values[0][1] * (self->height >> 8)) & 0xFFFFFF00));
        depth /= self->depth3D;
        self->visible = abs(depth) < 256;
    }
}

void PBL_Flipper_StaticUpdate(void) {}

void PBL_Flipper_Draw(void)
{
    RSDK_THIS(PBL_Flipper);
    if (self->depth3D >= 0x4000 && self->minCraneID <= PBL_Setup->sectorID) {
        RSDK.Prepare3DScene(PBL_Flipper->sceneIndex);
        RSDK.MatrixScaleXYZ(&self->matrix1, self->scale.x, self->scale.y, 0x100);
        RSDK.MatrixTranslateXYZ(&self->matrix1, self->position.x, self->height, self->position.y, false);
        RSDK.MatrixRotateY(&self->matrix3, self->angle >> 8);
        RSDK.MatrixMultiply(&self->matrix2, &self->matrix3, &self->matrix1);
        RSDK.MatrixMultiply(&self->matrix2, &self->matrix2, &PBL_Camera->matrix1);
        if (PBL_Camera->flag)
            RSDK.MatrixMultiply(&self->matrix3, &self->matrix3, &PBL_Camera->matrix3);
        else
            RSDK.MatrixMultiply(&self->matrix3, &self->matrix3, &PBL_Camera->matrix2);
        RSDK.AddModelTo3DScene(PBL_Flipper->aniFrames, PBL_Flipper->sceneIndex, PBL_Flipper->drawType, &self->matrix2, &self->matrix3, 0xFFFFFF);
        RSDK.Draw3DScene(PBL_Flipper->sceneIndex);
    }
}

void PBL_Flipper_Create(void *data)
{
    RSDK_THIS(PBL_Flipper);
    if (!SceneInfo->inEditor) {
        self->visible = true;
        if (self->direction == FLIP_NONE)
            self->scale.x = 0x100;
        else
            self->scale.x = -0x100;
        self->drawOrder     = 4;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->angle         = -0x4000;
        self->scale.y       = 0x100;
        self->field_170     = true;
        self->state         = PBL_Flipper_Unknown2;
        RSDK.SetModelAnimation(PBL_Flipper->aniFrames, &self->animator, 96, 0, true, 0);
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
    if (self->scale.y >= 128) {
        int32 angle    = self->angle >> 10;
        int32 negAngle = 0;
        if (self->direction == FLIP_X) {
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
            int32 distanceX = (player->position.x - self->position.x) >> 8;
            int32 distanceY = (player->position.y - self->position.y) >> 8;
            int32 posX                   = player->position.x;
            int32 posY                   = player->position.y;
            int32 velStoreX              = player->velocity.x;
            int32 velStoreY              = player->velocity.y;

            player->position.x         = distanceY * RSDK.Sin256(angle) + distanceX * RSDK.Cos256(angle) + self->position.x;
            player->position.y         = distanceY * RSDK.Cos256(angle) - distanceX * RSDK.Sin256(angle) + self->position.y;
            player->velocity.x         = (player->velocity.y >> 8) * RSDK.Sin256(angle) + (player->velocity.x >> 8) * RSDK.Cos256(angle);
            player->velocity.y         = (player->velocity.y >> 8) * RSDK.Cos256(angle) - (player->velocity.x >> 8) * RSDK.Sin256(angle);

            int32 velX = player->velocity.x;
            int32 velY = player->velocity.y;

            int32 distance = 0;
            if (self->direction == FLIP_NONE)
                distance = self->position.x - player->position.x;
            else
                distance = player->position.x - self->position.x;
            int32 force = clampVal((distance + 0x80000) >> 13, 16, 256);

            switch (RSDK.CheckObjectCollisionBox(self, &PBL_Flipper->hitbox, player, &PBL_Player->outerBox, true)) {
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
                        Zone_RotateOnPivot(&player->position, &self->position, negAngle);
                    }
                    else {
                        if (velY > 0)
                            velY >>= 2;
                        if (velY > 0x80000)
                            velY = 0x80000;
                        player->onGround = false;
                        if (velY > 0x40000)
                            player->velocity.x >>= 2;
                        if (self->state == PBL_Flipper_Unknown3 && self->velocity.y == 0x2000) {
                            player->velocity.y -= (0xE0000 * force) >> 8;
                            player->onGround = false;
                            player->angle    = 0;
                        }
                        Zone_RotateOnPivot(&player->velocity, &pos2, negAngle);
                        Zone_RotateOnPivot(&player->position, &self->position, negAngle);
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

                        Zone_RotateOnPivot(&player->position, &self->position, negAngle);
                        Zone_RotateOnPivot(&player->velocity, &pos2, negAngle);
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

                    Zone_RotateOnPivot(&player->position, &self->position, negAngle);
                    Zone_RotateOnPivot(&player->velocity, &pos2, negAngle);
                    break;
                case 4:
                    player->velocity.y = -(velY >> 2);
                    Zone_RotateOnPivot(&player->position, &self->position, negAngle);
                    Zone_RotateOnPivot(&player->velocity, &pos2, negAngle);
                    break;
                default: break;
            }
        }
    }
}

void PBL_Flipper_Unknown2(void)
{
    RSDK_THIS(PBL_Flipper);
    if (self->buttonDown) {
        self->velocity.y = 0x2000;
        self->state      = PBL_Flipper_Unknown3;
        RSDK.PlaySfx(PBL_Flipper->sfxFlipper, false, 255);
    }
    PBL_Flipper_HandlePlayerInteractions();
}

void PBL_Flipper_Unknown3(void)
{
    RSDK_THIS(PBL_Flipper);

    self->angle += self->velocity.y;
    if (self->angle < 0x4000 || self->velocity.y < 0) {
        if (self->velocity.y < 0x2000)
            self->velocity.y += 0x400;
    }
    else {
        self->angle      = 0x4000;
        self->velocity.y = -(self->velocity.y >> 2);
        if (!self->buttonDown) {
            self->velocity.y = -0x2000;
            self->state      = PBL_Flipper_Unknown4;
        }
    }
    PBL_Flipper_HandlePlayerInteractions();
}

void PBL_Flipper_Unknown4(void)
{
    RSDK_THIS(PBL_Flipper);
    self->angle += self->velocity.y;
    if (self->angle > -0x4000 || self->velocity.y > 0) {
        if (self->velocity.y > -0x2000)
            self->velocity.y -= 0x400;
    }
    else {
        self->angle      = -0x4000;
        self->velocity.y = -(self->velocity.y >> 2);
        if (self->velocity.y < 0x800) {
            self->velocity.y = 0;
            self->state      = PBL_Flipper_Unknown2;
        }
    }
    PBL_Flipper_HandlePlayerInteractions();
}

#if RETRO_INCLUDE_EDITOR
void PBL_Flipper_EditorDraw(void) {}

void PBL_Flipper_EditorLoad(void) {}
#endif

void PBL_Flipper_Serialize(void)
{
    RSDK_EDITABLE_VAR(PBL_Flipper, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PBL_Flipper, VAR_UINT8, minCraneID);
    RSDK_EDITABLE_VAR(PBL_Flipper, VAR_BOOL, retractable);
}
#endif
