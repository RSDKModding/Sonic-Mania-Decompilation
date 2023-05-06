// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_ItemBox Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUFO_ItemBox *UFO_ItemBox;

void UFO_ItemBox_Update(void)
{
    RSDK_THIS(UFO_ItemBox);
    StateMachine_Run(self->state);
}

void UFO_ItemBox_LateUpdate(void)
{
    RSDK_THIS(UFO_ItemBox);

    if (self->state == UFO_ItemBox_State_HasContents) {
        self->visible = true;

        int32 x = self->position.x >> 8;
        int32 y = self->height >> 8;
        int32 z = self->position.y >> 8;

        Matrix *mat = &UFO_Camera->matWorld;

        self->worldX = mat->values[0][3] + (y * mat->values[0][1] >> 8) + (z * mat->values[0][2] >> 8) + (x * mat->values[0][0] >> 8);
        self->worldY = mat->values[1][3] + (y * mat->values[1][1] >> 8) + (z * mat->values[1][2] >> 8) + (x * mat->values[1][0] >> 8);
        self->zdepth = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);

        if (self->zdepth >= 0x2000) {
            int32 depth = (int32)((mat->values[0][3] << 8) + (y * mat->values[0][1] & 0xFFFFFF00) + (z * mat->values[0][2] & 0xFFFFFF00)
                                  + (x * mat->values[0][0] & 0xFFFFFF00))
                          / self->zdepth;

            self->visible = abs(depth) < 0x100;
        }
        else {
            self->visible = false;
        }
    }
}

void UFO_ItemBox_StaticUpdate(void) {}

void UFO_ItemBox_Draw(void)
{
    RSDK_THIS(UFO_ItemBox);

    if (self->state == UFO_ItemBox_State_HasContents) {
        RSDK_GET_ENTITY(SLOT_UFO_CAMERA, UFO_Camera);

        RSDK.Prepare3DScene(UFO_ItemBox->sceneIndex);

        RSDK.MatrixTranslateXYZ(&self->matTransform, self->position.x, self->height, self->position.y, true);
        RSDK.MatrixRotateY(&self->matNormal, 8 * UFO_Setup->timer);

        RSDK.MatrixMultiply(&self->matWorld, &self->matNormal, &self->matTransform);
        RSDK.MatrixMultiply(&self->matWorld, &self->matWorld, &UFO_Camera->matWorld);

        RSDK.MatrixRotateXYZ(&self->matNormal, 0, 8 * UFO_Setup->timer, 4 * UFO_Setup->timer);

        RSDK.AddModelTo3DScene(UFO_ItemBox->meshFrames, UFO_ItemBox->sceneIndex, S3D_WIREFRAME_SHADED_SCREEN, &self->matWorld, &self->matNormal,
                               0xFFFF00);

        RSDK.Draw3DScene(UFO_ItemBox->sceneIndex);

        self->drawPos.x = (ScreenInfo->center.x + (self->worldX << 8) / self->zdepth) << 16;
        self->drawPos.y = (ScreenInfo->center.y - (self->worldY << 8) / self->zdepth) << 16;
        self->scale.x   = 0x2000000 / self->zdepth;
        self->scale.y   = 0x2000000 / self->zdepth;
    }

    RSDK.DrawSprite(&self->contentsAnimator, &self->drawPos, true);
}

void UFO_ItemBox_Create(void *data)
{
    RSDK_THIS(UFO_ItemBox);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_SCALE;
        self->drawGroup     = 4;
        self->active        = ACTIVE_RBOUNDS;
        self->updateRange.x = 0x400;
        self->updateRange.y = 0x400;

        self->contentsTimer = 256;
        self->sfxTimer      = 10;
        if (!self->height)
            self->height = 12;

        self->height <<= 16;
        self->state = UFO_ItemBox_State_HasContents;

        RSDK.SetSpriteAnimation(UFO_ItemBox->aniFrames, 0, &self->contentsAnimator, true, self->type);
    }
}

void UFO_ItemBox_StageLoad(void)
{
    UFO_ItemBox->aniFrames = RSDK.LoadSpriteAnimation("SpecialUFO/Items.bin", SCOPE_STAGE);

    UFO_ItemBox->meshFrames = RSDK.LoadMesh("Special/ItemBox.bin", SCOPE_STAGE);
    UFO_ItemBox->sceneIndex = RSDK.Create3DScene("View:Items", 1024, SCOPE_STAGE);

    UFO_ItemBox->breakCount = -1;

    RSDK.SetDiffuseColor(UFO_ItemBox->sceneIndex, 0xA0, 0xA0, 0x00);
    RSDK.SetDiffuseIntensity(UFO_ItemBox->sceneIndex, 8, 8, 0);
    RSDK.SetSpecularIntensity(UFO_ItemBox->sceneIndex, 14, 14, 0);

    UFO_ItemBox->sfxDestroy      = RSDK.GetSfx("Global/Destroy.wav");
    UFO_ItemBox->sfxRockemSockem = RSDK.GetSfx("Stage/RockemSockem.wav");
    UFO_ItemBox->sfxBumper       = RSDK.GetSfx("Stage/Bumper.wav");
}

void UFO_ItemBox_State_HasContents(void)
{
    RSDK_THIS(UFO_ItemBox);

    foreach_active(UFO_Player, player)
    {
        if (self->timer) {
            self->timer--;
        }
        else if (player->stateInput) {
            if (player->state != UFO_Player_State_CourseOut) {
                int32 rx = (self->position.x - player->position.x) >> 16;
                int32 ry = (self->height - player->height - 0xA0000) >> 16;
                int32 rz = (self->position.y - player->position.y) >> 16;

                if (rx * rx + ry * ry + rz * rz < UFO_Player->maxSpeed >> 8) {
                    player->gravityStrength = 0x60000;
                    player->onGround        = false;
                    player->state           = UFO_Player_State_Jump;

                    RSDK.SetModelAnimation(UFO_Player->jumpModel, &player->animator, 128, 0, true, 0);

#if MANIA_USE_PLUS
                    if (self->type >= UFO_ITEMBOX_BUMPER) {
                        RSDK.PlaySfx(UFO_ItemBox->sfxBumper, false, 255);
                        self->timer = 16;
                    }
                    else {
#endif
                        RSDK.PlaySfx(UFO_ItemBox->sfxDestroy, false, 255);
                        self->drawGroup = 12;
                        self->active    = 2;
                        self->state     = UFO_ItemBox_State_ShowContents;
#if MANIA_USE_PLUS
                    }
#endif
                    ++UFO_ItemBox->breakCount;
                }
            }
        }
    }
}

void UFO_ItemBox_State_ShowContents(void)
{
    RSDK_THIS(UFO_ItemBox);

    self->drawPos.x += ((ScreenInfo->center.x << 16) - self->drawPos.x) >> 3;
    self->drawPos.y += (((ScreenInfo->center.x - 4) << 16) - self->drawPos.y) >> 3;

    int32 scale   = self->scale.x + ((0x200 - self->scale.x) >> 3);
    self->scale.x = scale;
    self->scale.y = scale;

    if (self->contentsTimer > 0)
        self->contentsTimer -= 8;

    if (++self->timer > 15) {
        self->visible = (self->timer >> 2) & 1;

        switch (self->type) {
            case UFO_ITEMBOX_RING:
                if (!(self->timer & 3)) {
                    if (self->sfxTimer > 0) {
                        self->sfxTimer--;
                        UFO_Ring_PlayRingSfx();
                    }
                }
                break;

            case UFO_ITEMBOX_SPHERE:
                if (!(self->timer & 1) && self->sfxTimer > 0) {
                    ++UFO_Setup->machPoints;
                    UFO_HUD_CheckLevelUp();
                    UFO_Setup_PlaySphereSfx();
                }
                break;

            case UFO_ITEMBOX_LVLUP:
                if (self->timer == 16)
                    UFO_HUD_LevelUpMach();
                break;
        }
    }

    if (self->timer == 80) {
        if (UFO_ItemBox->breakCount > -1)
            UFO_ItemBox->breakCount--;

        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void UFO_ItemBox_EditorDraw(void)
{
    RSDK_THIS(UFO_ItemBox);

    RSDK.SetSpriteAnimation(UFO_ItemBox->aniFrames, 0, &self->contentsAnimator, true, self->type);

    RSDK.DrawSprite(&self->contentsAnimator, NULL, false);
}

void UFO_ItemBox_EditorLoad(void)
{
    UFO_ItemBox->aniFrames = RSDK.LoadSpriteAnimation("SpecialUFO/Items.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UFO_ItemBox, type);
    RSDK_ENUM_VAR("Rings", UFO_ITEMBOX_RING);
    RSDK_ENUM_VAR("Spheres", UFO_ITEMBOX_SPHERE);
    RSDK_ENUM_VAR("Level Up", UFO_ITEMBOX_LVLUP);
#if MANIA_USE_PLUS
    RSDK_ENUM_VAR("Bumper", UFO_ITEMBOX_BUMPER);
#endif
}
#endif

void UFO_ItemBox_Serialize(void)
{
    RSDK_EDITABLE_VAR(UFO_ItemBox, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(UFO_ItemBox, VAR_ENUM, height);
}
