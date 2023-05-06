// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_Sphere Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
    self->zdepth     = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);
}

void UFO_Sphere_StaticUpdate(void) {}

void UFO_Sphere_Draw(void)
{
    RSDK_THIS(UFO_Sphere);

    if (self->drawGroup == 4 && self->zdepth >= 0x100) {
        self->direction = self->animator.frameID > 8;
        self->drawPos.x = (ScreenInfo->center.x + (self->worldPos.x << 8) / self->zdepth) << 16;
        self->drawPos.y = (ScreenInfo->center.y - (self->worldPos.y << 8) / self->zdepth) << 16;
        self->scale.x   = self->scaleFactor / self->zdepth;
        self->scale.y   = self->scaleFactor / self->zdepth;
    }

    RSDK.DrawSprite(&self->animator, &self->drawPos, true);
}

void UFO_Sphere_Create(void *data)
{
    RSDK_THIS(UFO_Sphere);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_SCALE | FX_FLIP;
        self->drawGroup     = 4;
        self->active        = ACTIVE_RBOUNDS;
        self->updateRange.x = 0x400;
        self->updateRange.y = 0x400;

        self->scaleFactor = 0x1000000;

        if (!self->height)
            self->height = UFO_Sphere->defaultHeights[self->type];
        self->height <<= 16;

        switch (self->behavior) {
            default:
            case UFO_SPHERE_B_NONE: // static
                self->state = UFO_Sphere_State_Fixed;
                break;

            case UFO_SPHERE_B_BOUNCE:
                self->velocity.y = 0x20000;
                self->state      = UFO_Sphere_State_Bouncing;
                break;

            case UFO_SPHERE_B_MOVE:
                self->amplitude.x >>= 8;
                self->amplitude.y >>= 8;
                self->startPos.x  = self->position.x;
                self->startPos.y  = self->position.y;
                self->startHeight = self->height;
                self->state       = UFO_Sphere_State_Moving;

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
    UFO_Sphere->aniFrames = RSDK.LoadSpriteAnimation("SpecialUFO/Spheres.bin", SCOPE_STAGE);

    UFO_Sphere->sfxMachSpeed  = RSDK.GetSfx("Special/MachSpeed.wav");
    UFO_Sphere->sfxLedgeBreak = RSDK.GetSfx("Stage/LedgeBreak.wav");
}

void UFO_Sphere_State_Fixed(void)
{
    RSDK_THIS(UFO_Sphere);

    switch (self->type) {
        case UFO_SPHERE_BLUE: {
            foreach_active(UFO_Player, player)
            {
                int32 rx = (self->position.x - player->position.x) >> 16;
                int32 ry = (self->height - player->height - 0xA0000) >> 16;
                int32 rz = (self->position.y - player->position.y) >> 16;

                if (rx * rx + ry * ry + rz * rz < UFO_Player->maxSpeed >> 9) {
                    self->drawGroup = 12;

                    self->state = UFO_Sphere_State_Collected;
                    UFO_Setup_PlaySphereSfx();
                }
            }

            if (UFO_Setup->machLevel == 2)
                self->animator.speed = 1;
            break;
        }

        case UFO_SPHERE_RED:
            // unused
            break;

        case UFO_SPHERE_MINE: {
            foreach_active(UFO_Player, player)
            {
                if (player->state != UFO_Player_State_Trip && player->state != UFO_Player_State_Springboard
                    && player->state != UFO_Player_State_UFOCaught_Charge && player->state != UFO_Player_State_UFOCaught_Released) {
                    int32 rx = (self->position.x - player->position.x) >> 16;
                    int32 ry = (self->height - player->height - 0xA0000) >> 16;
                    int32 rz = (self->position.y - player->position.y) >> 16;

                    if (rx * rx + ry * ry + rz * rz < 0x180) {
                        RSDK.SetModelAnimation(UFO_Player->tumbleModel, &player->animator, 80, 0, false, 0);

                        player->state = UFO_Player_State_Trip;

                        if (UFO_Setup->rings > 0)
                            RSDK.PlaySfx(UFO_Player->sfxLoseRings, false, 255);

                        UFO_Ring_LoseRings(player);

                        RSDK.SetSpriteAnimation(UFO_Sphere->aniFrames, 4, &self->animator, true, 0);
                        self->scaleFactor = 0x1800000;
                        self->state       = UFO_Sphere_State_AnimateAndDestroy;
                        RSDK.PlaySfx(UFO_Sphere->sfxLedgeBreak, false, 255);
                    }
                }
            }
            break;
        }
        case UFO_SPHERE_SPIKES: {
            foreach_active(UFO_Player, player)
            {
                if (player->state != UFO_Player_State_Trip && player->state != UFO_Player_State_Springboard
                    && player->state != UFO_Player_State_UFOCaught_Charge && player->state != UFO_Player_State_UFOCaught_Released) {
                    int32 rx = (self->position.x - player->position.x) >> 16;
                    int32 ry = (self->height - player->height - 0xA0000) >> 16;
                    int32 rz = (self->position.y - player->position.y) >> 16;

                    if (rx * rx + ry * ry + rz * rz < 0x180) {
                        RSDK.SetModelAnimation(UFO_Player->tumbleModel, &player->animator, 80, 0, false, 0);

                        player->state = UFO_Player_State_Trip;

                        if (UFO_Setup->rings > 0)
                            RSDK.PlaySfx(UFO_Player->sfxLoseRings, false, 255);

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

    UFO_Sphere_State_Fixed();
}

void UFO_Sphere_State_Moving(void)
{
    RSDK_THIS(UFO_Sphere);

    Matrix *m = &self->matrix;

    self->position.x = self->amplitude.x * RSDK.Cos256(self->angle + (UFO_Setup->timer << self->speed));
    self->position.y = self->amplitude.y * RSDK.Sin256(self->angle + (UFO_Setup->timer << self->speed));
    self->height     = 0;

    self->position.x = m->values[0][3] + (self->position.y >> 8) * m->values[0][2] + m->values[0][0] * (self->position.x >> 8);
    self->height     = m->values[1][3] + (self->position.y >> 8) * m->values[1][2] + m->values[1][0] * (self->position.x >> 8);
    self->position.y = m->values[2][3] + (self->position.y >> 8) * m->values[2][2] + m->values[2][0] * (self->position.x >> 8)
                       + m->values[2][1] * (self->height >> 8);

    self->position.x += self->startPos.x;
    self->position.y += self->startPos.y;
    self->height += self->startHeight;

    UFO_Sphere_State_Fixed();
}

void UFO_Sphere_State_Collected(void)
{
    RSDK_THIS(UFO_Sphere);

    self->drawPos.x += ((((ScreenInfo->center.x - 38) << 16) - self->drawPos.x) >> 3);
    self->drawPos.y += ((0x1C0000 - self->drawPos.y) >> 3);

    self->scale.x = self->scale.x + ((0x80 - self->scale.x) >> 3);
    self->scale.y = self->scale.x + ((0x80 - self->scale.x) >> 3);

    if (self->drawPos.y < 0x200000) {
        ++UFO_Setup->machPoints;
        UFO_HUD_CheckLevelUp();

        self->timer   = UFO_Setup->timer;
        self->visible = false;
        self->state   = UFO_Sphere_State_HandleRespawn;
    }
}

void UFO_Sphere_State_HandleRespawn(void)
{
    RSDK_THIS(UFO_Sphere);

    if (abs(self->timer - UFO_Setup->timer) > 240) {
        self->visible   = true;
        self->drawGroup = 4;

        switch (self->behavior) {
            default:
            case UFO_SPHERE_B_NONE: self->state = UFO_Sphere_State_Fixed; break;

            case UFO_SPHERE_B_BOUNCE: self->state = UFO_Sphere_State_Bouncing; break;

            case UFO_SPHERE_B_MOVE: self->state = UFO_Sphere_State_Moving; break;
        }
    }
}

void UFO_Sphere_State_AnimateAndDestroy(void)
{
    RSDK_THIS(UFO_Sphere);

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void UFO_Sphere_EditorDraw(void)
{
    RSDK_THIS(UFO_Sphere);

    RSDK.SetSpriteAnimation(UFO_Sphere->aniFrames, 5, &self->animator, true, self->type);

    RSDK.DrawSprite(&self->animator, NULL, false);

    if (showGizmos() && self->behavior == UFO_SPHERE_B_MOVE) {
        Vector2 amplitude;

        amplitude.x      = self->amplitude.x >> 8;
        amplitude.y      = self->amplitude.y >> 8;
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;

        self->inkEffect = INK_BLEND;

        Vector2 start, end;

        // start pos
        self->position.x = amplitude.x * RSDK.Cos256(self->angle) + self->startPos.x;
        self->position.y = amplitude.y * RSDK.Sin256(self->angle) + self->startPos.y;
        RSDK.DrawSprite(&self->animator, NULL, false);

        // right max
        self->position.x = amplitude.x * RSDK.Cos256(0x00) + self->startPos.x;
        self->position.y = amplitude.y * RSDK.Sin256(0x00) + self->startPos.y;
        start            = self->position;
        RSDK.DrawSprite(&self->animator, NULL, false);

        // left max
        self->position.x = amplitude.x * RSDK.Cos256(0x80) + self->startPos.x;
        self->position.y = amplitude.y * RSDK.Sin256(0x80) + self->startPos.y;
        end              = self->position;
        RSDK.DrawSprite(&self->animator, NULL, false);

        DrawHelpers_DrawArrow(start.x, start.y, end.x, end.y, 0x00FF00, INK_NONE, 0xFF);
        DrawHelpers_DrawArrow(end.x, end.y, start.x, start.y, 0x00FF00, INK_NONE, 0xFF);

        self->inkEffect = INK_NONE;
    }
}

void UFO_Sphere_EditorLoad(void)
{
    UFO_Sphere->aniFrames = RSDK.LoadSpriteAnimation("SpecialUFO/Spheres.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UFO_Sphere, type);
    RSDK_ENUM_VAR("Blue", UFO_SPHERE_BLUE);
    RSDK_ENUM_VAR("Red (Unused)", UFO_SPHERE_RED);
    RSDK_ENUM_VAR("Mine", UFO_SPHERE_MINE);
    RSDK_ENUM_VAR("Spikes", UFO_SPHERE_SPIKES);

    RSDK_ACTIVE_VAR(UFO_Sphere, behavior);
    RSDK_ENUM_VAR("None", UFO_SPHERE_B_NONE);
    RSDK_ENUM_VAR("Bouncing", UFO_SPHERE_B_BOUNCE);
    RSDK_ENUM_VAR("Moving", UFO_SPHERE_B_MOVE);
}
#endif

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
