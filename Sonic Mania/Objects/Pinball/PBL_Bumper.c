#include "SonicMania.h"

#if RETRO_USE_PLUS
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

    Matrix *mat   = &PBL_Camera->matrix1;
    self->depth3D = mat->values[2][1] * (y >> 16) + mat->values[2][2] * (z >> 16) + mat->values[2][0] * (x >> 16) + mat->values[2][3];
    if (self->depth3D >= 0x4000) {
        int32 depth = ((mat->values[0][3] << 8) + ((mat->values[0][2] * (z >> 8)) & 0xFFFFFF00) + ((mat->values[0][0] * (x >> 8)) & 0xFFFFFF00)
                     + ((mat->values[0][1] * (self->height >> 8)) & 0xFFFFFF00));
        depth /= self->depth3D;
    }
}

void PBL_Bumper_StaticUpdate(void) {}

void PBL_Bumper_Draw(void)
{
    RSDK_THIS(PBL_Bumper);
    if (self->depth3D >= 0x4000) {
        RSDK.Prepare3DScene(PBL_Bumper->sceneIndex);
        RSDK.MatrixScaleXYZ(&self->matrix2, self->scale.x, self->scale.y, self->scale.x);
        RSDK.MatrixTranslateXYZ(&self->matrix2, self->position.x, self->height, self->position.y, false);
        RSDK.MatrixRotateY(&self->matrix3, self->angle);
        RSDK.MatrixMultiply(&self->matrix1, &self->matrix3, &self->matrix2);
        RSDK.MatrixMultiply(&self->matrix1, &self->matrix1, &PBL_Camera->matrix1);
        RSDK.MatrixMultiply(&self->matrix3, &self->matrix3, &PBL_Camera->matrix2);
        RSDK.AddModelTo3DScene(PBL_Bumper->modelFrames, PBL_Bumper->sceneIndex, S3D_FLATCLR_SHADED_BLENDED_SCREEN, &self->matrix1, &self->matrix3,
                               0xFFFFFF);
        RSDK.Draw3DScene(PBL_Bumper->sceneIndex);
    }
}

void PBL_Bumper_Create(void *data)
{
    RSDK_THIS(PBL_Bumper);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = 4;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->angle         = 512;
        self->scale.x       = 0x100;
        self->scale.y       = 0x100;
        self->state         = PBL_Bumper_Unknown2;
        RSDK.SetModelAnimation(PBL_Bumper->modelFrames, &self->animator, 96, 0, true, 0);
    }
}

void PBL_Bumper_StageLoad(void)
{
    PBL_Bumper->modelFrames   = RSDK.LoadMesh("Pinball/Bumper.bin", SCOPE_STAGE);
    PBL_Bumper->sceneIndex    = RSDK.Create3DScene("View:Pinball", 0x1000, SCOPE_STAGE);
    PBL_Bumper->hitbox.left   = -14;
    PBL_Bumper->hitbox.top    = -14;
    PBL_Bumper->hitbox.right  = 14;
    PBL_Bumper->hitbox.bottom = 14;
    PBL_Bumper->sfxBumper     = RSDK.GetSfx("Stage/Bumper.wav");
}

void PBL_Bumper_HandlePlayerInteractions(void)
{
    RSDK_THIS(PBL_Bumper);

    foreach_active(PBL_Player, player)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &PBL_Bumper->hitbox, player, &PBL_Player->outerBox)) {
            if (self->state == PBL_Bumper_Unknown2) {
                self->field_60 = 0;
                self->field_64 = 0x8000;
                self->active   = ACTIVE_NORMAL;
                self->state    = PBL_Bumper_Unknown3;
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

void PBL_Bumper_Unknown2(void) { PBL_Bumper_HandlePlayerInteractions(); }

void PBL_Bumper_Unknown3(void)
{
    RSDK_THIS(PBL_Bumper);
    self->field_64 += 0x1000;
    self->field_60 += self->field_64;
    if (self->field_60 > 0x8000) {
        self->field_64 = -(self->field_64 >> 1);
        if (++self->field_68 > 6) {
            self->field_68 = 0;
            self->state    = PBL_Bumper_Unknown4;
        }
    }
    self->scale.x = (self->field_60 >> 8) + 256;
    self->scale.y = (self->field_60 >> 8) + 256;
    PBL_Bumper_HandlePlayerInteractions();
}

void PBL_Bumper_Unknown4(void)
{
    RSDK_THIS(PBL_Bumper);
    self->field_60 -= (self->field_60 >> 2);
    if (self->field_60 < 256) {
        self->field_60 = 0;
        self->active   = ACTIVE_BOUNDS;
        self->state    = PBL_Bumper_Unknown2;
    }
    self->scale.x = (self->field_60 >> 8) + 256;
    self->scale.y = (self->field_60 >> 8) + 256;
    PBL_Bumper_HandlePlayerInteractions();
}

#if RETRO_INCLUDE_EDITOR
void PBL_Bumper_EditorDraw(void) {}

void PBL_Bumper_EditorLoad(void) {}
#endif

void PBL_Bumper_Serialize(void) {}
#endif
