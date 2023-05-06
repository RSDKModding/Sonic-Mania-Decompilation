// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PBL_Flipper Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectPBL_Flipper *PBL_Flipper;

void PBL_Flipper_Update(void)
{
    RSDK_THIS(PBL_Flipper);

    if (self->direction) {
        self->buttonPress = TriggerInfoL[CONT_P1].keyBumper.press || ControllerInfo[CONT_P1].keyA.press || ControllerInfo[CONT_P1].keyX.press
                            || ControllerInfo[CONT_P1].keyC.press;

        self->buttonDown = TriggerInfoL[CONT_P1].keyBumper.down || ControllerInfo[CONT_P1].keyA.down || ControllerInfo[CONT_P1].keyX.down
                           || ControllerInfo[CONT_P1].keyC.down;
    }
    else {
        self->buttonPress = TriggerInfoR[CONT_P1].keyBumper.press || ControllerInfo[CONT_P1].keyA.press || ControllerInfo[CONT_P1].keyB.press
                            || ControllerInfo[CONT_P1].keyC.press;

        self->buttonDown = TriggerInfoR[CONT_P1].keyBumper.down || ControllerInfo[CONT_P1].keyA.down || ControllerInfo[CONT_P1].keyB.down
                           || ControllerInfo[CONT_P1].keyC.down;
    }

    StateMachine_Run(self->state);

    if (self->retractable) {
        foreach_active(PBL_Player, player)
        {
            if (player->position.y >= self->position.y - 0x100000 || self->minCraneID > PBL_Setup->sectorID) {
                if (player->position.y > self->position.y + 0x100000)
                    self->flipperActive = false;
            }
            else {
                self->flipperActive = true;
            }
        }

        if (self->flipperActive) {
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

    int32 z = self->position.y;
    int32 y = self->height;
    int32 x = self->position.x;

    Matrix *m    = &PBL_Camera->matWorld;
    self->zdepth = m->values[2][1] * (y >> 16) + m->values[2][2] * (z >> 16) + m->values[2][0] * (x >> 16) + m->values[2][3];

    if (self->zdepth >= 0x4000) {
        int32 depth = ((m->values[0][3] << 8) + ((m->values[0][2] * (z >> 8)) & 0xFFFFFF00) + ((m->values[0][0] * (x >> 8)) & 0xFFFFFF00)
                       + ((m->values[0][1] * (self->height >> 8)) & 0xFFFFFF00));
        depth /= self->zdepth;
        self->visible = abs(depth) < 0x100;
    }
}

void PBL_Flipper_StaticUpdate(void) {}

void PBL_Flipper_Draw(void)
{
    RSDK_THIS(PBL_Flipper);

    if (self->zdepth >= 0x4000 && self->minCraneID <= PBL_Setup->sectorID) {
        RSDK.Prepare3DScene(PBL_Flipper->sceneIndex);

        RSDK.MatrixScaleXYZ(&self->matTransform, self->scale.x, self->scale.y, 0x100);
        RSDK.MatrixTranslateXYZ(&self->matTransform, self->position.x, self->height, self->position.y, false);
        RSDK.MatrixRotateY(&self->matNormal, self->angle >> 8);

        RSDK.MatrixMultiply(&self->matWorld, &self->matNormal, &self->matTransform);
        RSDK.MatrixMultiply(&self->matWorld, &self->matWorld, &PBL_Camera->matWorld);

        if (PBL_Camera->useAltMatNormal)
            RSDK.MatrixMultiply(&self->matNormal, &self->matNormal, &PBL_Camera->matNormal);
        else
            RSDK.MatrixMultiply(&self->matNormal, &self->matNormal, &PBL_Camera->matNormalItem);

        RSDK.AddModelTo3DScene(PBL_Flipper->modelFrames, PBL_Flipper->sceneIndex, PBL_Flipper->drawType, &self->matWorld, &self->matNormal, 0xFFFFFF);

        RSDK.Draw3DScene(PBL_Flipper->sceneIndex);
    }
}

void PBL_Flipper_Create(void *data)
{
    RSDK_THIS(PBL_Flipper);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->scale.x       = self->direction == FLIP_NONE ? 0x100 : -0x100;
        self->drawGroup     = 4;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->angle         = -0x4000;
        self->scale.y       = 0x100;
        self->flipperActive = true;
        self->state         = PBL_Flipper_State_AwaitFlip;

        RSDK.SetModelAnimation(PBL_Flipper->modelFrames, &self->animator, 96, 0, true, 0);
    }
}

void PBL_Flipper_StageLoad(void)
{
    PBL_Flipper->modelFrames = RSDK.LoadMesh("Pinball/Flipper.bin", SCOPE_STAGE);

    PBL_Flipper->sceneIndex = RSDK.Create3DScene("View:Pinball", 0x1000, SCOPE_STAGE);

    PBL_Flipper->drawType = S3D_SOLIDCOLOR_SHADED_BLENDED_SCREEN;

    PBL_Flipper->hitbox.left   = -52;
    PBL_Flipper->hitbox.top    = -8;
    PBL_Flipper->hitbox.right  = 12;
    PBL_Flipper->hitbox.bottom = 8;

    PBL_Flipper->hitboxUnused.left   = -52;
    PBL_Flipper->hitboxUnused.top    = -24;
    PBL_Flipper->hitboxUnused.right  = 12;
    PBL_Flipper->hitboxUnused.bottom = 8;

    PBL_Flipper->sfxFlipperStage = RSDK.GetSfx("Stage/Flipper.wav");
    PBL_Flipper->sfxFlipper      = RSDK.GetSfx("Pinball/Flipper.wav");
}

void PBL_Flipper_HandlePlayerInteractions(void)
{
    RSDK_THIS(PBL_TargetBumper);

    if (self->scale.y >= 0x80) {
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

        Vector2 originVel = { 0, 0 };
        foreach_active(PBL_Player, player)
        {
            int32 posX      = player->position.x;
            int32 posY      = player->position.y;
            int32 velStoreX = player->velocity.x;
            int32 velStoreY = player->velocity.y;

            Zone_RotateOnPivot(&player->position, &self->position, angle);
            Zone_RotateOnPivot(&player->velocity, &originVel, angle);

            int32 velX = player->velocity.x;
            int32 velY = player->velocity.y;

            int32 distance = 0;
            if (self->direction == FLIP_NONE)
                distance = self->position.x - player->position.x;
            else
                distance = player->position.x - self->position.x;
            int32 force = CLAMP((distance + 0x80000) >> 13, 16, 256);

            switch (RSDK.CheckObjectCollisionBox(self, &PBL_Flipper->hitbox, player, &PBL_Player->outerBox, true)) {
                case C_NONE:
                    player->position.x = posX;
                    player->position.y = posY;
                    player->velocity.x = velStoreX;
                    player->velocity.y = velStoreY;
                    break;

                case C_TOP:
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

                        if (self->state == PBL_Flipper_State_RiseFlipper && self->velocity.y == 0x2000) {
                            player->velocity.y -= (0xE0000 * force) >> 8;
                            player->onGround = false;
                            player->angle    = 0;
                        }

                        Zone_RotateOnPivot(&player->position, &self->position, negAngle);
                        Zone_RotateOnPivot(&player->velocity, &originVel, negAngle);
                    }
                    break;

                case C_LEFT:
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
                        Zone_RotateOnPivot(&player->velocity, &originVel, negAngle);
                    }
                    break;

                case C_RIGHT:
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
                    Zone_RotateOnPivot(&player->velocity, &originVel, negAngle);
                    break;

                case C_BOTTOM:
                    player->velocity.y = -(velY >> 2);

                    Zone_RotateOnPivot(&player->position, &self->position, negAngle);
                    Zone_RotateOnPivot(&player->velocity, &originVel, negAngle);
                    break;

                default: break;
            }
        }
    }
}

void PBL_Flipper_State_AwaitFlip(void)
{
    RSDK_THIS(PBL_Flipper);

    if (self->buttonDown) {
        self->velocity.y = 0x2000;
        self->state      = PBL_Flipper_State_RiseFlipper;
        RSDK.PlaySfx(PBL_Flipper->sfxFlipper, false, 255);
    }

    PBL_Flipper_HandlePlayerInteractions();
}

void PBL_Flipper_State_RiseFlipper(void)
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
            self->state      = PBL_Flipper_State_LowerFlipper;
        }
    }

    PBL_Flipper_HandlePlayerInteractions();
}

void PBL_Flipper_State_LowerFlipper(void)
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
            self->state      = PBL_Flipper_State_AwaitFlip;
        }
    }

    PBL_Flipper_HandlePlayerInteractions();
}

#if GAME_INCLUDE_EDITOR
void PBL_Flipper_EditorDraw(void) {}

void PBL_Flipper_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(PBL_Flipper, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void PBL_Flipper_Serialize(void)
{
    RSDK_EDITABLE_VAR(PBL_Flipper, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PBL_Flipper, VAR_UINT8, minCraneID);
    RSDK_EDITABLE_VAR(PBL_Flipper, VAR_BOOL, retractable);
}
#endif
