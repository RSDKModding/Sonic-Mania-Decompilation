#include "SonicMania.h"

ObjectUFO_Sphere *UFO_Sphere;

void UFO_Sphere_Update(void)
{
    RSDK_THIS(UFO_Sphere);
    StateMachine_Run(self->state);
    RSDK.ProcessAnimation(&self->animator);
}

void UFO_Sphere_LateUpdate(void)
{
    RSDK_THIS(UFO_Sphere);
    int32 x = self->position.x >> 8;
    int32 y = self->height >> 8;
    int32 z = self->position.y >> 8;

    Matrix *mat = &UFO_Camera->matWorld;

    self->worldPos.x = mat->values[0][3] + (y * mat->values[0][1] >> 8) + (z * mat->values[0][2] >> 8) + (x * mat->values[0][0] >> 8);
    self->worldPos.y = mat->values[1][3] + (y * mat->values[1][1] >> 8) + (z * mat->values[1][2] >> 8) + (x * mat->values[1][0] >> 8);
    self->depth3D    = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);
}

void UFO_Sphere_StaticUpdate(void) {}

void UFO_Sphere_Draw(void)
{
    RSDK_THIS(UFO_Sphere);
    if (self->drawOrder == 4) {
        if (self->depth3D >= 0x100) {
            self->direction = self->animator.frameID > 8;
            self->drawPos.x = (ScreenInfo->centerX + (self->worldPos.x << 8) / self->depth3D) << 16;
            self->drawPos.y = (ScreenInfo->centerY - (self->worldPos.y << 8) / self->depth3D) << 16;
            self->scale.x   = self->dword9C / self->depth3D;
            self->scale.y   = self->dword9C / self->depth3D;
        }
    }
    RSDK.DrawSprite(&self->animator, &self->drawPos, true);
}

void UFO_Sphere_Create(void *data)
{
    RSDK_THIS(UFO_Sphere);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_SCALE | FX_FLIP;
        self->drawOrder     = 4;
        self->active        = ACTIVE_RBOUNDS;
        self->updateRange.x = 0x400;
        self->updateRange.y = 0x400;
        self->dword9C       = 0x1000000;
        if (!self->height)
            self->height = UFO_Sphere->defaultHeights[self->type];
        self->height <<= 0x10;

        switch (self->behavior) {
            default:
            case 0: //static
                self->state = UFO_Sphere_CheckPlayerCollision; 
                break;
            case 1:
                self->velocity.y = 0x20000;
                self->state      = UFO_Sphere_State_Bouncing;
                break;
            case 2:
                self->amplitude.x >>= 8;
                self->amplitude.y >>= 8;
                self->dword84 = self->position.x;
                self->dword88 = self->position.y;
                self->dword80 = self->height;
                self->state   = UFO_Sphere_State_Moving;

                Matrix matrix;
                RSDK.MatrixRotateX(&matrix, self->matAngle.x);
                RSDK.MatrixRotateY(&self->matrix, self->matAngle.y);
                RSDK.MatrixMultiply(&self->matrix, &matrix, &self->matrix);
                break;
        }
        RSDK.SetSpriteAnimation(UFO_Sphere->aniFrames, self->type, &self->animator, true, 0);
    }
}

void UFO_Sphere_StageLoad(void)
{
    UFO_Sphere->aniFrames    = RSDK.LoadSpriteAnimation("SpecialUFO/Spheres.bin", SCOPE_STAGE);
    UFO_Sphere->sfxMachSpeed  = RSDK.GetSFX("Special/MachSpeed.wav");
    UFO_Sphere->sfxLedgeBreak = RSDK.GetSFX("Stage/LedgeBreak.wav");
}

void UFO_Sphere_CheckPlayerCollision(void)
{
    RSDK_THIS(UFO_Sphere);

    switch (self->type) {
        case 0: {
            foreach_active(UFO_Player, player)
            {
                int32 ry     = (self->height - player->height - 0xA0000) >> 16;
                int32 rx     = (self->position.y - player->position.y) >> 16;
                int32 radius = rx * rx + ry * ry;
                int32 pr     = UFO_Player->maxSpeed >> 9;
                if (((self->position.x - player->position.x) >> 16) * ((self->position.x - player->position.x) >> 16) + radius < pr) {
                    self->drawOrder = 12;
                    self->state     = UFO_Sphere_Unknown3;
                    UFO_Setup_PlaySphereSFX();
                }
            }

            if (UFO_Setup->machLevel == 2) {
                self->animator.animationSpeed = 1;
            }
            break;
        }
        case 1: 
            //unused (red)
            break;
        case 2: {
            foreach_active(UFO_Player, player)
            {
                if (player->state != UFO_Player_Unknown8 && player->state != UFO_Player_HandleTilt && player->state != UFO_Player_Unknown10
                    && player->state != UFO_Player_Unknown11) {
                    int32 ry = (self->height - player->height - 0xA0000) >> 16;
                    int32 rx = (self->position.y - player->position.y) >> 16;
                    if (((self->position.x - player->position.x) >> 16) * ((self->position.x - player->position.x) >> 16) + rx * rx + ry * ry
                        < 384) {
                        RSDK.SetModelAnimation(UFO_Player->tumbleModel, &player->animator, 80, 0, false, 0);
                        player->state = UFO_Player_Unknown8;
                        if (UFO_Setup->rings > 0)
                            RSDK.PlaySfx(UFO_Player->sfxLoseRings, 0, 255);
                        UFO_Ring_LoseRings(player);
                        RSDK.SetSpriteAnimation(UFO_Sphere->aniFrames, 4, &self->animator, true, 0);
                        self->dword9C = 0x1800000;
                        self->state   = UFO_Sphere_Unknown5;
                        RSDK.PlaySfx(UFO_Sphere->sfxLedgeBreak, 0, 255);
                    }
                }
            }
            break;
        }
        case 3: {
            foreach_active(UFO_Player, player)
            {
                if (player->state != UFO_Player_Unknown8 && player->state != UFO_Player_HandleTilt && player->state != UFO_Player_Unknown10
                    && player->state != UFO_Player_Unknown11) {
                    int32 ry = (self->height - player->height - 0xA0000) >> 16;
                    int32 rx = (self->position.y - player->position.y) >> 16;
                    if (((self->position.x - player->position.x) >> 16) * ((self->position.x - player->position.x) >> 16) + rx * rx + ry * ry
                        < 384) {
                        RSDK.SetModelAnimation(UFO_Player->tumbleModel, &player->animator, 80, 0, false, 0);
                        player->state = UFO_Player_Unknown8;
                        if (UFO_Setup->rings > 0)
                            RSDK.PlaySfx(UFO_Player->sfxLoseRings, 0, 255);
                        UFO_Ring_LoseRings(player);
                    }
                }
            }
            break;
        }
    }
}

void UFO_Sphere_State_Bouncing(void)
{
    RSDK_THIS(UFO_Sphere);
    self->velocity.y -= 0x3800;
    self->height += self->velocity.y;
    if (self->height < 0xC0000) {
        self->velocity.y = 0x20000;
        self->height     = 0xC0000;
    }
    UFO_Sphere_CheckPlayerCollision();
}

void UFO_Sphere_State_Moving(void)
{
    RSDK_THIS(UFO_Sphere);
    Matrix *mat = &self->matrix;

    int32 amp            = self->amplitude.y * RSDK.Sin256(self->angle + (UFO_Setup->timer << self->speed));
    self->position.x = self->amplitude.x * RSDK.Cos256(self->angle + (UFO_Setup->timer << self->speed));
    self->position.y = self->amplitude.y * amp;
    self->height     = 0;
    amp >>= 8;

    self->position.x = mat->values[0][3] + amp * mat->values[0][2] + mat->values[0][0] * (self->position.x >> 8);
    self->height     = mat->values[1][3] + amp * mat->values[1][2] + mat->values[1][0] * (self->position.x >> 8);
    self->position.y = mat->values[2][3] + amp * mat->values[2][2] + mat->values[2][0] * (self->position.x >> 8) + mat->values[2][1] * (self->height >> 8);

    self->position.x += self->dword84;
    self->position.y += self->dword88;
    self->height += self->dword80;
    UFO_Sphere_CheckPlayerCollision();
}

void UFO_Sphere_Unknown3(void)
{
    RSDK_THIS(UFO_Sphere);

    self->drawPos.x += ((((ScreenInfo->centerX - 38) << 16) - self->drawPos.x) >> 3);
    self->drawPos.y += ((0x1C0000 - self->drawPos.y) >> 3);
    self->scale.x = self->scale.x + ((128 - self->scale.x) >> 3);
    self->scale.y = self->scale.x + ((128 - self->scale.x) >> 3);
    if (self->drawPos.y < 0x200000) {
        ++UFO_Setup->machPoints;
        UFO_HUD_CheckLevelUp();
        self->timer   = UFO_Setup->timer;
        self->visible = false;
        self->state   = UFO_Sphere_Unknown4;
    }
}

void UFO_Sphere_Unknown4(void)
{
    RSDK_THIS(UFO_Sphere);

    if (abs(self->timer - UFO_Setup->timer) > 240) {
        self->visible   = true;
        self->drawOrder = 4;

        switch (self->behavior) {
            default:
            case 0:
            case 3: self->state = UFO_Sphere_CheckPlayerCollision; break;
            case 1: self->state = UFO_Sphere_State_Bouncing; break;
            case 2: self->state = UFO_Sphere_State_Moving; break;
        }
    }
}

void UFO_Sphere_Unknown5(void)
{
    RSDK_THIS(UFO_Sphere);

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

void UFO_Sphere_EditorDraw(void)
{
    RSDK_THIS(UFO_Sphere);
    RSDK.SetSpriteAnimation(UFO_Sphere->aniFrames, self->type, &self->animator, true, 0);

    RSDK.DrawSprite(&self->animator, &self->drawPos, true);
}

void UFO_Sphere_EditorLoad(void) { UFO_Sphere->aniFrames = RSDK.LoadSpriteAnimation("SpecialUFO/Spheres.bin", SCOPE_STAGE); }

void UFO_Sphere_Serialize(void)
{
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_ENUM, behavior);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_ENUM, height);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_VECTOR2, matAngle);
    RSDK_EDITABLE_VAR(UFO_Sphere, VAR_VECTOR2, amplitude);
}
