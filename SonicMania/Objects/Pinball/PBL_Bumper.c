// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PBL_Bumper Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectPBL_Bumper *PBL_Bumper;

void PBL_Bumper_Update(void)
{
    RSDK_THIS(PBL_Bumper);

    StateMachine_Run(self->state);
}

void PBL_Bumper_LateUpdate(void)
{
    RSDK_THIS(PBL_Bumper);

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

void PBL_Bumper_StaticUpdate(void) {}

void PBL_Bumper_Draw(void)
{
    RSDK_THIS(PBL_Bumper);

    if (self->zdepth >= 0x4000) {
        RSDK.Prepare3DScene(PBL_Bumper->sceneIndex);

        RSDK.MatrixScaleXYZ(&self->matTransform, self->scale.x, self->scale.y, self->scale.x);
        RSDK.MatrixTranslateXYZ(&self->matTransform, self->position.x, self->height, self->position.y, false);
        RSDK.MatrixRotateY(&self->matNormal, self->angle);

        RSDK.MatrixMultiply(&self->matWorld, &self->matNormal, &self->matTransform);
        RSDK.MatrixMultiply(&self->matWorld, &self->matWorld, &PBL_Camera->matWorld);
        RSDK.MatrixMultiply(&self->matNormal, &self->matNormal, &PBL_Camera->matNormalItem);

        RSDK.AddModelTo3DScene(PBL_Bumper->modelFrames, PBL_Bumper->sceneIndex, S3D_SOLIDCOLOR_SHADED_BLENDED_SCREEN, &self->matWorld,
                               &self->matNormal, 0xFFFFFF);

        RSDK.Draw3DScene(PBL_Bumper->sceneIndex);
    }
}

void PBL_Bumper_Create(void *data)
{
    RSDK_THIS(PBL_Bumper);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = 4;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->angle         = 0x200;
        self->scale.x       = 0x100;
        self->scale.y       = 0x100;
        self->state         = PBL_Bumper_State_CheckBumps;
        RSDK.SetModelAnimation(PBL_Bumper->modelFrames, &self->animator, 96, 0, true, 0);
    }
}

void PBL_Bumper_StageLoad(void)
{
    PBL_Bumper->modelFrames = RSDK.LoadMesh("Pinball/Bumper.bin", SCOPE_STAGE);

    PBL_Bumper->sceneIndex = RSDK.Create3DScene("View:Pinball", 0x1000, SCOPE_STAGE);

    PBL_Bumper->hitbox.left   = -14;
    PBL_Bumper->hitbox.top    = -14;
    PBL_Bumper->hitbox.right  = 14;
    PBL_Bumper->hitbox.bottom = 14;

    PBL_Bumper->sfxBumper = RSDK.GetSfx("Stage/Bumper.wav");
}

void PBL_Bumper_HandlePlayerInteractions(void)
{
    RSDK_THIS(PBL_Bumper);

    foreach_active(PBL_Player, player)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &PBL_Bumper->hitbox, player, &PBL_Player->outerBox)) {
            if (self->state == PBL_Bumper_State_CheckBumps) {
                self->scaleFactor = 0;
                self->scaleVel    = 0x8000;
                self->active      = ACTIVE_NORMAL;
                self->state       = PBL_Bumper_State_Bumped;
                RSDK.PlaySfx(PBL_Bumper->sfxBumper, false, 255);
            }

            int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
            player->velocity.x >>= 1;
            player->velocity.y >>= 1;
            player->velocity.x += 0x700 * RSDK.Cos256(angle);
            player->velocity.y += 0x700 * RSDK.Sin256(angle);
            player->onGround = false;
            PBL_Setup_GiveScore(250);
        }
    }
}

void PBL_Bumper_State_CheckBumps(void) { PBL_Bumper_HandlePlayerInteractions(); }

void PBL_Bumper_State_Bumped(void)
{
    RSDK_THIS(PBL_Bumper);

    self->scaleVel += 0x1000;
    self->scaleFactor += self->scaleVel;

    if (self->scaleFactor > 0x8000) {
        self->scaleVel = -(self->scaleVel >> 1);
        if (++self->timer > 6) {
            self->timer = 0;
            self->state = PBL_Bumper_State_FinishedBump;
        }
    }

    self->scale.x = (self->scaleFactor >> 8) + 0x100;
    self->scale.y = (self->scaleFactor >> 8) + 0x100;
    PBL_Bumper_HandlePlayerInteractions();
}

void PBL_Bumper_State_FinishedBump(void)
{
    RSDK_THIS(PBL_Bumper);

    self->scaleFactor -= self->scaleFactor >> 2;
    if (self->scaleFactor < 0x100) {
        self->scaleFactor = 0;
        self->active      = ACTIVE_BOUNDS;
        self->state       = PBL_Bumper_State_CheckBumps;
    }

    self->scale.x = (self->scaleFactor >> 8) + 0x100;
    self->scale.y = (self->scaleFactor >> 8) + 0x100;

    PBL_Bumper_HandlePlayerInteractions();
}

#if GAME_INCLUDE_EDITOR
void PBL_Bumper_EditorDraw(void) {}

void PBL_Bumper_EditorLoad(void) {}
#endif

void PBL_Bumper_Serialize(void) {}
#endif
