// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_Springboard Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUFO_Springboard *UFO_Springboard;

void UFO_Springboard_Update(void)
{
    RSDK_THIS(UFO_Springboard);
    foreach_active(UFO_Player, player)
    {
        if (player->bumperTimer <= 12 && player->stateInput) {
            Vector2 pivotPos = player->position;
            Zone_RotateOnPivot(&pivotPos, &self->position, self->angle >> 2);

            if (abs(self->position.x - pivotPos.x) < 0x180000 && abs(self->position.y - pivotPos.y) < 0x180000) {
                int32 height = 32 * ((pivotPos.y - self->position.y + 0x180000) / 48);
                if (player->height < height) {
                    if (height - player->height <= 0xC0000) {
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

    if (RSDK.CheckOnScreen(self, 0)) {
        if (self->scale.x < 0x100)
            self->scale.x += 0x10;

        self->active = ACTIVE_NORMAL;
    }
    else {
        self->scale.x = 0x000;
        self->active  = ACTIVE_BOUNDS;
    }
}

void UFO_Springboard_LateUpdate(void)
{
    RSDK_THIS(UFO_Springboard);

    int32 x = self->position.x;
    int32 y = self->height;
    int32 z = self->position.y;

    Matrix *m = &UFO_Camera->matWorld;

    self->zdepth = m->values[2][1] * (y >> 16) + m->values[2][2] * (z >> 16) + m->values[2][0] * (x >> 16) + m->values[2][3];
    if (self->zdepth >= 0x4000) {
        int32 depth = (int32)((m->values[0][3] << 8) + (m->values[0][2] * (z >> 8) & 0xFFFFFF00) + (m->values[0][0] * (x >> 8) & 0xFFFFFF00)
                              + (m->values[0][1] * (self->height >> 8) & 0xFFFFFF00))
                      / self->zdepth;

        self->visible = abs(depth) < 0x100;
    }
}

void UFO_Springboard_StaticUpdate(void) {}

void UFO_Springboard_Draw(void)
{
    RSDK_THIS(UFO_Springboard);

    if (self->zdepth >= 0x4000) {
        RSDK.Prepare3DScene(UFO_Springboard->sceneIndex);

        RSDK.MatrixScaleXYZ(&self->matTemp, 0x100, self->scale.x, 0x100);
        RSDK.MatrixTranslateXYZ(&self->matTemp, self->position.x, self->height, self->position.y, 0);

        RSDK.MatrixRotateY(&self->matNormal, self->angle);
        RSDK.MatrixMultiply(&self->matWorld, &self->matNormal, &self->matTemp);
        RSDK.MatrixMultiply(&self->matWorld, &self->matWorld, &UFO_Camera->matWorld);
        RSDK.MatrixMultiply(&self->matNormal, &self->matNormal, &UFO_Camera->matView);

        RSDK.AddMeshFrameTo3DScene(UFO_Springboard->modelIndex, UFO_Springboard->sceneIndex, &self->animator, S3D_SOLIDCOLOR_SHADED_BLENDED_SCREEN,
                                   &self->matWorld, &self->matNormal, 0xFFFFFF);

        RSDK.Draw3DScene(UFO_Springboard->sceneIndex);
    }
}

void UFO_Springboard_Create(void *data)
{
    RSDK_THIS(UFO_Springboard);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = 4;
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

#if GAME_INCLUDE_EDITOR
void UFO_Springboard_EditorDraw(void) {}

void UFO_Springboard_EditorLoad(void) {}
#endif

void UFO_Springboard_Serialize(void)
{
    RSDK_EDITABLE_VAR(UFO_Springboard, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(UFO_Springboard, VAR_UINT8, rampCount);
}
