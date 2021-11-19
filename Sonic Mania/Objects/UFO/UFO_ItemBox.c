#include "SonicMania.h"

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
        int32 x           = self->position.x >> 8;
        int32 y           = self->height >> 8;
        int32 z           = self->position.y >> 8;

        Matrix *mat = &UFO_Camera->matWorld;

        self->worldX = mat->values[0][3] + (y * mat->values[0][1] >> 8) + (z * mat->values[0][2] >> 8) + (x * mat->values[0][0] >> 8);
        self->worldY = mat->values[1][3] + (y * mat->values[1][1] >> 8) + (z * mat->values[1][2] >> 8) + (x * mat->values[1][0] >> 8);
        self->depth3D = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);

        if (self->depth3D >= 0x2000) {
            int32 depth = (int32)((mat->values[0][3] << 8) + (y * mat->values[0][1] & 0xFFFFFF00) + (z * mat->values[0][2] & 0xFFFFFF00)
                              + (x * mat->values[0][0] & 0xFFFFFF00))
                        / self->depth3D;
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
        RSDK.Prepare3DScene(UFO_ItemBox->sceneIndex);
        RSDK.GetEntityByID(SLOT_UFO_CAMERA);
        RSDK.MatrixTranslateXYZ(&self->matTransform, self->position.x, self->height, self->position.y, true);
        RSDK.MatrixRotateY(&self->matNormal, 8 * UFO_Setup->timer);
        RSDK.MatrixMultiply(&self->matWorld, &self->matNormal, &self->matTransform);
        RSDK.MatrixMultiply(&self->matWorld, &self->matWorld, &UFO_Camera->matWorld);
        RSDK.MatrixRotateXYZ(&self->matNormal, 0, 8 * UFO_Setup->timer, 4 * UFO_Setup->timer);
        RSDK.AddModelTo3DScene(UFO_ItemBox->meshFrames, UFO_ItemBox->sceneIndex, S3D_FLATCLR_SHADED_SCREEN_WIREFRAME, &self->matWorld, &self->matNormal,
                               0xFFFF00);
        RSDK.Draw3DScene(UFO_ItemBox->sceneIndex);

        self->drawPos.x = (ScreenInfo->centerX + (self->worldX << 8) / self->depth3D) << 16;
        self->drawPos.y = (ScreenInfo->centerY - (self->worldY << 8) / self->depth3D) << 16;
        self->scale.x   = 0x2000000 / self->depth3D;
        self->scale.y   = 0x2000000 / self->depth3D;
    }
    RSDK.DrawSprite(&self->itemAnimator, &self->drawPos, true);
}

void UFO_ItemBox_Create(void *data)
{
    RSDK_THIS(UFO_ItemBox);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_SCALE;
        self->drawOrder     = 4;
        self->active        = ACTIVE_RBOUNDS;
        self->updateRange.x = 0x400;
        self->updateRange.y = 0x400;
        self->timer2        = 256;
        self->sfxTimer      = 10;
        if (!self->height)
            self->height = 12;

        self->height <<= 16;
        self->state = UFO_ItemBox_State_HasContents;
        RSDK.SetSpriteAnimation(UFO_ItemBox->aniFrames, 0, &self->itemAnimator, true, self->type);
    }
}

void UFO_ItemBox_StageLoad(void)
{
    UFO_ItemBox->aniFrames  = RSDK.LoadSpriteAnimation("SpecialUFO/Items.bin", SCOPE_STAGE);
    UFO_ItemBox->meshFrames = RSDK.LoadMesh("Special/ItemBox.bin", SCOPE_STAGE);
    UFO_ItemBox->sceneIndex = RSDK.Create3DScene("View:Items", 1024, SCOPE_STAGE);
    UFO_ItemBox->breakCount = -1;
    RSDK.SetDiffuseColour(UFO_ItemBox->sceneIndex, 160, 160, 0);
    RSDK.SetDiffuseIntensity(UFO_ItemBox->sceneIndex, 8, 8, 0);
    RSDK.SetSpecularIntensity(UFO_ItemBox->sceneIndex, 14, 14, 0);
    UFO_ItemBox->sfxDestroy      = RSDK.GetSFX("Global/Destroy.wav");
    UFO_ItemBox->sfxRockemSockem = RSDK.GetSFX("Stage/RockemSockem.wav");
    UFO_ItemBox->sfxBumper       = RSDK.GetSFX("Stage/Bumper.wav");
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
                int32 pr = ((self->position.x - player->position.x) >> 16) * ((self->position.x - player->position.x) >> 16);

                int32 rx = (self->position.y - player->position.y) >> 16;
                int32 ry = (self->height - player->height - 0xA0000) >> 16;

                int32 spd = UFO_Player->maxSpeed >> 8;
                if (pr + (rx * rx + ry * ry) < spd) {
                    player->gravityStrength = 0x60000;
                    player->onGround        = false;
                    player->state           = UFO_Player_State_Jump;
                    RSDK.SetModelAnimation(UFO_Player->jumpModel, &player->animator, 128, 0, true, 0);
#if RETRO_USE_PLUS
                    if (self->type >= UFO_ITEMBOX_BUMPER) {
                        RSDK.PlaySfx(UFO_ItemBox->sfxBumper, false, 255);
                        self->timer = 16;
                    }
                    else {
#endif
                        RSDK.PlaySfx(UFO_ItemBox->sfxDestroy, false, 255);
                        self->drawOrder = 12;
                        self->active    = 2;
                        self->state     = UFO_ItemBox_State_ShowContents;
#if RETRO_USE_PLUS
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

    self->drawPos.x += ((ScreenInfo->centerX << 16) - self->drawPos.x) >> 3;
    self->drawPos.y += (((ScreenInfo->centerX - 4) << 16) - self->drawPos.y) >> 3;

    int32 scale       = self->scale.x + ((512 - self->scale.x) >> 3);
    self->scale.x = scale;
    self->scale.y = scale;

    if (self->timer2 > 0) {
        self->timer2 -= 8;
    }

    if (++self->timer > 15) {
        self->visible = (self->timer >> 2) & 1;

        switch (self->type) {
            case UFO_ITEMBOX_RING:
                if (!(self->timer & 3)) {
                    if (self->sfxTimer > 0) {
                        self->sfxTimer--;
                        UFO_Ring_PlayRingSFX();
                    }
                }
                break;
            case UFO_ITEMBOX_SPHERE:
                if (!(self->timer & 1) && self->sfxTimer > 0) {
                    ++UFO_Setup->machPoints;
                    UFO_HUD_CheckLevelUp();
                    UFO_Setup_PlaySphereSFX();
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

#if RETRO_INCLUDE_EDITOR
void UFO_ItemBox_EditorDraw(void) {}

void UFO_ItemBox_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(UFO_ItemBox, type);
    RSDK_ENUM_VAR("Rings", UFO_ITEMBOX_RING);
    RSDK_ENUM_VAR("Spheres", UFO_ITEMBOX_SPHERE);
    RSDK_ENUM_VAR("Level Up", UFO_ITEMBOX_LVLUP);
#if RETRO_USE_PLUS
    RSDK_ENUM_VAR("Bumper", UFO_ITEMBOX_BUMPER);
#endif
}
#endif

void UFO_ItemBox_Serialize(void)
{
    RSDK_EDITABLE_VAR(UFO_ItemBox, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(UFO_ItemBox, VAR_ENUM, height);
}
