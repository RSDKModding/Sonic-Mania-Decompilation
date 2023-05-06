// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PBL_TargetBumper Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectPBL_TargetBumper *PBL_TargetBumper;

void PBL_TargetBumper_Update(void)
{
    RSDK_THIS(PBL_TargetBumper);

    StateMachine_Run(self->state);
}

void PBL_TargetBumper_LateUpdate(void)
{
    RSDK_THIS(PBL_TargetBumper);

    int32 x = self->position.x;
    int32 y = self->height;
    int32 z = self->position.y;

    Matrix *m    = &PBL_Camera->matWorld;
    self->zdepth = m->values[2][1] * (y >> 16) + m->values[2][2] * (z >> 16) + m->values[2][0] * (x >> 16) + m->values[2][3];

    if (self->zdepth >= 0x4000) {
        int32 depth = ((m->values[0][3] << 8) + ((m->values[0][2] * (z >> 8)) & 0xFFFFFF00) + ((m->values[0][0] * (x >> 8)) & 0xFFFFFF00)
                       + ((m->values[0][1] * (self->height >> 8)) & 0xFFFFFF00));
        depth /= self->zdepth;
    }
}

void PBL_TargetBumper_StaticUpdate(void) {}

void PBL_TargetBumper_Draw(void)
{
    RSDK_THIS(PBL_TargetBumper);

    if (self->zdepth >= 0x4000) {
        uint32 color = RSDK.GetPaletteEntry(0, (self->color & 0xFF) - 80);
        RSDK.SetDiffuseColor(PBL_TargetBumper->sceneIndex, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);

        RSDK.Prepare3DScene(PBL_TargetBumper->sceneIndex);

        RSDK.MatrixScaleXYZ(&self->matTransform, self->scale.x, self->scale.y, 256);
        RSDK.MatrixTranslateXYZ(&self->matTransform, self->position.x, self->height, self->position.y, false);
        RSDK.MatrixRotateY(&self->matNormal, self->angle);

        RSDK.MatrixMultiply(&self->matWorld, &self->matNormal, &self->matTransform);
        RSDK.MatrixMultiply(&self->matWorld, &self->matWorld, &PBL_Camera->matWorld);
        RSDK.MatrixMultiply(&self->matNormal, &self->matNormal, &PBL_Camera->matNormal);

        RSDK.AddModelTo3DScene(PBL_TargetBumper->modelFrames, PBL_TargetBumper->sceneIndex, PBL_TargetBumper->drawType, &self->matWorld,
                               &self->matNormal, 0xFFFFFF);

        RSDK.Draw3DScene(PBL_TargetBumper->sceneIndex);
    }
}

void PBL_TargetBumper_Create(void *data)
{
    RSDK_THIS(PBL_TargetBumper);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = 4;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->scale.x       = 0x100;
        self->scale.y       = 0x100;
        self->state         = PBL_TargetBumper_State_Active;
        self->color         = 2 * (RSDK.GetEntitySlot(self) % 3);

        RSDK.SetModelAnimation(PBL_TargetBumper->modelFrames, &self->animator, 96, 0, true, 0);
    }
}

void PBL_TargetBumper_StageLoad(void)
{
    PBL_TargetBumper->modelFrames = RSDK.LoadMesh("Pinball/TargetBumper.bin", SCOPE_STAGE);

    PBL_TargetBumper->sceneIndex = RSDK.Create3DScene("View:TargetBumper", 256, SCOPE_STAGE);

    RSDK.SetDiffuseIntensity(PBL_TargetBumper->sceneIndex, 9, 9, 9);
    RSDK.SetSpecularIntensity(PBL_TargetBumper->sceneIndex, 15, 15, 15);

    PBL_TargetBumper->drawType = S3D_SOLIDCOLOR_SHADED_BLENDED_SCREEN;

    PBL_TargetBumper->hitbox.left   = -16;
    PBL_TargetBumper->hitbox.top    = -6;
    PBL_TargetBumper->hitbox.right  = 12;
    PBL_TargetBumper->hitbox.bottom = 6;

    PBL_TargetBumper->sfxTargetBumper = RSDK.GetSfx("Pinball/TargetBumper.wav");
    PBL_TargetBumper->sfxFlipper      = RSDK.GetSfx("Pinball/Flipper.wav");
}

void PBL_TargetBumper_HandlePlayerInteractions(void)
{
    RSDK_THIS(PBL_TargetBumper);

    if (self->scale.y >= 0x80) {
        int32 angle    = self->angle >> 2;
        int32 negAngle = -angle;

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
            switch (RSDK.CheckObjectCollisionBox(self, &PBL_TargetBumper->hitbox, player, &PBL_Player->outerBox, true)) {
                case C_NONE:
                    player->position.x = posX;
                    player->position.y = posY;
                    player->velocity.x = velStoreX;
                    player->velocity.y = velStoreY;
                    break;

                case C_TOP:
                case C_BOTTOM:
                    player->onGround = false;
                    if (abs(velY) < 0x40000)
                        velY <<= 1;

                    player->velocity.y = -velY;
                    Zone_RotateOnPivot(&player->position, &self->position, negAngle);
                    Zone_RotateOnPivot(&player->velocity, &originVel, negAngle);

                    self->state      = PBL_TargetBumper_State_Reced;
                    self->velocity.y = -8;
                    PBL_Setup_GiveScore(1000);

                    RSDK.PlaySfx(PBL_TargetBumper->sfxTargetBumper, false, 255);
                    break;

                case C_LEFT:
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

                        Zone_RotateOnPivot(&player->position, &self->position, negAngle);
                        Zone_RotateOnPivot(&player->velocity, &originVel, negAngle);
                        player->onGround = false;
                    }
                    break;

                case C_RIGHT:
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

                    Zone_RotateOnPivot(&player->position, &self->position, negAngle);
                    Zone_RotateOnPivot(&player->velocity, &originVel, negAngle);
                    player->onGround = false;
                    break;

                default: break;
            }
        }
    }
}

void PBL_TargetBumper_State_Active(void) { PBL_TargetBumper_HandlePlayerInteractions(); }

void PBL_TargetBumper_State_Reced(void)
{
    RSDK_THIS(PBL_TargetBumper);

    self->velocity.y += 2;
    self->scale.y -= self->velocity.y;

    if (self->scale.y <= 0) {
        self->velocity.y = 0;
        self->scale.y    = 0;
        self->state      = StateMachine_None;
    }
}

void PBL_TargetBumper_State_Rise(void)
{
    RSDK_THIS(PBL_TargetBumper);

    self->velocity.y -= 2;
    self->scale.y += self->velocity.y;

    if (self->velocity.y < 0 && self->scale.y <= 0x100) {
        self->active     = ACTIVE_BOUNDS;
        self->velocity.y = 0;
        self->scale.y    = 256;
        self->state      = PBL_TargetBumper_State_Active;
    }
}

#if GAME_INCLUDE_EDITOR
void PBL_TargetBumper_EditorDraw(void) {}

void PBL_TargetBumper_EditorLoad(void) {}
#endif

void PBL_TargetBumper_Serialize(void) { RSDK_EDITABLE_VAR(PBL_TargetBumper, VAR_ENUM, angle); }
#endif
