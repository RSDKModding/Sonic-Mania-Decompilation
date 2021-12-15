// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_Springboard Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectUFO_Springboard *UFO_Springboard;

void UFO_Springboard_Update(void)
{
    RSDK_THIS(UFO_Springboard);
    foreach_active(UFO_Player, player)
    {
        if (player->bumperTimer <= 12 && player->stateInput) {
            int32 xDif = (player->position.x - self->position.x) >> 8;
            int32 yDif = (player->position.y - self->position.y) >> 8;
            int32 valX  = (yDif * RSDK.Sin256(self->angle >> 2)) + xDif * RSDK.Cos256(self->angle >> 2) + self->position.x;
            int32 valY = (self->position.y - xDif * RSDK.Sin256(self->angle >> 2)) + yDif * RSDK.Cos256(self->angle >> 2);

            RSDK.Sin256(self->angle >> 2);
            RSDK.Cos256(self->angle >> 2);
            RSDK.Cos256(self->angle >> 2);
            RSDK.Sin256(self->angle >> 2);
            if (abs(self->position.x - valX) < 0x180000) {
                if (abs(self->position.y - valY) < 0x180000) {
                    int32 val3 = 32 * ((valY - self->position.y + 0x180000) / 48);
                    if (player->height < val3) {
                        if (val3 - player->height <= 0xC0000) {
                            player->gravityStrength = 0xC0000;
                            player->onGround        = false;
                            player->state           = UFO_Player_State_Springboard;
                            RSDK.SetModelAnimation(UFO_Player->jumpModel, &player->animator, 128, 0, true, 0);
                            RSDK.PlaySfx(UFO_Player->sfxSpring, false, 255);
                        }
                        else {
                            player->bumperTimer = 16;
                            player->velocity.x  = -player->velocity.x;
                            player->velocity.y  = -player->velocity.y;
                            RSDK.PlaySfx(UFO_Player->sfxBumper, false, 255);
                        }
                    }
                }
            }
        }
    }

    if (RSDK.CheckOnScreen(self, 0)) {
        if (self->scale.x < 256)
            self->scale.x += 16;
        self->active = ACTIVE_NORMAL;
    }
    else {
        self->scale.x = 0;
        self->active  = ACTIVE_BOUNDS;
    }
}

void UFO_Springboard_LateUpdate(void)
{
    RSDK_THIS(UFO_Springboard);
    int32 z       = self->position.y;
    int32 y       = self->height;
    int32 x       = self->position.x;
    Matrix *mat = &UFO_Camera->matWorld;

    self->depth3D = mat->values[2][1] * (y >> 16) + mat->values[2][2] * (z >> 16) + mat->values[2][0] * (x >> 16) + mat->values[2][3];
    if (self->depth3D >= 0x4000) {
        int32 depth = (int32)((mat->values[0][3] << 8) + (mat->values[0][2] * (z >> 8) & 0xFFFFFF00) + (mat->values[0][0] * (x >> 8) & 0xFFFFFF00)
                     + (mat->values[0][1] * (self->height >> 8) & 0xFFFFFF00))
                    / self->depth3D;
        self->visible = abs(depth) < 256;
    }
}

void UFO_Springboard_StaticUpdate(void) {}

void UFO_Springboard_Draw(void)
{
    RSDK_THIS(UFO_Springboard);
    if (self->depth3D >= 0x4000) {
        RSDK.Prepare3DScene(UFO_Springboard->sceneIndex);
        RSDK.MatrixScaleXYZ(&self->matrix1, 0x100, self->scale.x, 0x100);
        RSDK.MatrixTranslateXYZ(&self->matrix1, self->position.x, self->height, self->position.y, 0);
        RSDK.MatrixRotateY(&self->matrix3, self->angle);
        RSDK.MatrixMultiply(&self->matrix2, &self->matrix3, &self->matrix1);
        RSDK.MatrixMultiply(&self->matrix2, &self->matrix2, &UFO_Camera->matWorld);
        RSDK.MatrixMultiply(&self->matrix3, &self->matrix3, &UFO_Camera->matView);
        RSDK.AddMeshFrameTo3DScene(UFO_Springboard->modelIndex, UFO_Springboard->sceneIndex, &self->animator, S3D_FLATCLR_SHADED_BLENDED_SCREEN,
                                   &self->matrix2, &self->matrix3, 0xFFFFFF);
        RSDK.Draw3DScene(UFO_Springboard->sceneIndex);
    }
}

void UFO_Springboard_Create(void *data)
{
    RSDK_THIS(UFO_Springboard);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = 4;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x5000000;
        self->updateRange.y = 0x5000000;
    }
}

void UFO_Springboard_StageLoad(void)
{
    UFO_Springboard->modelIndex = RSDK.LoadMesh("Special/Springboard.bin", SCOPE_STAGE);
    UFO_Springboard->sceneIndex = RSDK.Create3DScene("View:Special", 4096, SCOPE_STAGE);
}

#if RETRO_INCLUDE_EDITOR
void UFO_Springboard_EditorDraw(void) {}

void UFO_Springboard_EditorLoad(void) {}
#endif

void UFO_Springboard_Serialize(void)
{
    RSDK_EDITABLE_VAR(UFO_Springboard, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(UFO_Springboard, VAR_UINT8, rampCount);
}
