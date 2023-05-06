// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SSZSpikeBall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSSZSpikeBall *SSZSpikeBall;

void SSZSpikeBall_Update(void)
{
    RSDK_THIS(SSZSpikeBall);

    StateMachine_Run(self->state);

    foreach_active(Player, player)
    {
        if (self->type < SSZSPIKEBALL_MOVEBALL_UP)
            Player_CheckCollisionBox(player, self, &SSZSpikeBall->hitboxBase[self->type]);

        Vector2 posStore = self->position;
        self->position   = self->spikeBallPos;
        if (Player_CheckCollisionTouch(player, self, &SSZSpikeBall->hitboxSpikeBall)) {
            self->position = posStore;
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x400, 2, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
        else {
            self->position = posStore;
        }
    }
}

void SSZSpikeBall_LateUpdate(void) {}

void SSZSpikeBall_StaticUpdate(void) {}

void SSZSpikeBall_Draw(void)
{
    RSDK_THIS(SSZSpikeBall);

    RSDK.DrawSprite(&self->spikeBallAnimator, &self->spikeBallPos, false);
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);
}

void SSZSpikeBall_Create(void *data)
{
    RSDK_THIS(SSZSpikeBall);

    if (!self->speed) {
        self->speed = 12;
        self->dist  = 64;
    }

    if (!self->interval)
        self->interval = 120;

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 0, &self->baseAnimator, true, 0);
        RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 1, &self->spikeBallAnimator, true, 0);

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x600000;
        self->updateRange.y = 0x600000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->speed <<= 15;
        self->dist           = (self->dist << 16) / self->speed;
        self->spikeBallPos.x = self->position.x;
        self->spikeBallPos.y = self->position.y;

        switch (self->type) {
            case SSZSPIKEBALL_LAUNCHER_UP:
            case SSZSPIKEBALL_LAUNCHER_LEFT:
                self->spikeBallPos.x = -0x200000;
                self->spikeBallPos.y = -0x200000;
                self->direction      = FLIP_NONE;
                break;

            case SSZSPIKEBALL_LAUNCHER_DOWN:
            case SSZSPIKEBALL_LAUNCHER_RIGHT:
                self->spikeBallPos.x = -0x200000;
                self->spikeBallPos.y = -0x200000;
                self->direction      = FLIP_X;
                break;

            case SSZSPIKEBALL_MOVEBALL_UP: self->velocity.y = -self->speed; break;
            case SSZSPIKEBALL_MOVEBALL_DOWN: self->velocity.y = self->speed; break;
            case SSZSPIKEBALL_MOVEBALL_LEFT: self->velocity.x = -self->speed; break;
            case SSZSPIKEBALL_MOVEBALL_RIGHT: self->velocity.x = self->speed; break;
            default: break;
        }

        self->state                = SSZSpikeBall_State_AwaitInterval;
        self->baseAnimator.frameID = MIN(self->type, 4);
    }
}

void SSZSpikeBall_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeBall.bin", SCOPE_STAGE);
    else
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SpikeBall.bin", SCOPE_STAGE);

    SSZSpikeBall->hitboxSpikeBall.left   = -12;
    SSZSpikeBall->hitboxSpikeBall.top    = -12;
    SSZSpikeBall->hitboxSpikeBall.right  = 12;
    SSZSpikeBall->hitboxSpikeBall.bottom = 12;

    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_UP].left   = -16;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_UP].top    = -12;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_UP].right  = 16;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_UP].bottom = 0;

    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_DOWN].left   = -16;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_DOWN].top    = 0;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_DOWN].right  = 16;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_DOWN].bottom = 12;

    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_LEFT].left   = -12;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_LEFT].top    = -16;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_LEFT].right  = 0;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_LEFT].bottom = 16;

    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_RIGHT].left   = -12;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_RIGHT].top    = -16;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_RIGHT].right  = 0;
    SSZSpikeBall->hitboxBase[SSZSPIKEBALL_LAUNCHER_RIGHT].bottom = 16;

    SSZSpikeBall->sfxPon = RSDK.GetSfx("Stage/Pon.wav");
}

void SSZSpikeBall_State_AwaitInterval(void)
{
    RSDK_THIS(SSZSpikeBall);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        self->spikeBallPos.x = self->position.x;
        self->spikeBallPos.y = self->position.y;
        self->active         = ACTIVE_NORMAL;
        self->timer          = 0;

        switch (self->type) {
            case SSZSPIKEBALL_LAUNCHER_UP:
                self->state = SSZSpikeBall_State_V;
                self->spikeBallPos.y += 0x40000;
                self->velocity.y = -self->speed;
                break;

            case SSZSPIKEBALL_LAUNCHER_DOWN:
                self->spikeBallPos.y -= 0x40000;
                self->state      = SSZSpikeBall_State_V;
                self->velocity.y = self->speed;
                break;

            case SSZSPIKEBALL_LAUNCHER_LEFT:
                self->state = SSZSpikeBall_State_H;
                self->spikeBallPos.x += 0x30000;
                self->velocity.x = -self->speed;
                break;

            case SSZSPIKEBALL_LAUNCHER_RIGHT:
                self->spikeBallPos.x -= 0x30000;
                self->state      = SSZSpikeBall_State_H;
                self->velocity.x = self->speed;
                break;

            case SSZSPIKEBALL_MOVEBALL_UP:
            case SSZSPIKEBALL_MOVEBALL_DOWN:
            case SSZSPIKEBALL_MOVEBALL_LEFT:
            case SSZSPIKEBALL_MOVEBALL_RIGHT:
                self->timer = self->dist;
                self->state = SSZSpikeBall_State_Ball_MoveOut;
                break;

            default: break;
        }
    }
}

void SSZSpikeBall_State_H(void)
{
    RSDK_THIS(SSZSpikeBall);

    if (++self->timer >= 0x20) {
        if (self->timer == 48) {
            RSDK.PlaySfx(SSZSpikeBall->sfxPon, false, 255);
            self->state = SSZSpikeBall_State_H_Launch;
        }
    }
    else if (self->direction)
        self->spikeBallPos.x += 0x8000;
    else
        self->spikeBallPos.x -= 0x8000;
}

void SSZSpikeBall_State_V(void)
{
    RSDK_THIS(SSZSpikeBall);

    if (++self->timer >= 0x20) {
        if (self->timer == 48) {
            RSDK.PlaySfx(SSZSpikeBall->sfxPon, false, 255);
            self->state = SSZSpikeBall_State_V_Launch;
        }
    }
    else if (self->direction)
        self->spikeBallPos.y += 0x8000;
    else
        self->spikeBallPos.y -= 0x8000;
}

void SSZSpikeBall_State_H_Launch(void)
{
    RSDK_THIS(SSZSpikeBall);

    self->spikeBallPos.x += self->velocity.x;

    if (self->direction) {
        self->velocity.x -= 0x3800;
        if (self->spikeBallPos.x < self->position.x) {
            self->state          = SSZSpikeBall_State_AwaitInterval;
            self->active         = ACTIVE_BOUNDS;
            self->spikeBallPos.x = -0x200000;
        }
    }
    else {
        self->velocity.x += 0x3800;
        if (self->spikeBallPos.x > self->position.x) {
            self->state          = SSZSpikeBall_State_AwaitInterval;
            self->active         = ACTIVE_BOUNDS;
            self->spikeBallPos.x = -0x200000;
        }
    }
}

void SSZSpikeBall_State_V_Launch(void)
{
    RSDK_THIS(SSZSpikeBall);

    self->spikeBallPos.y += self->velocity.y;

    if (self->direction) {
        self->velocity.y -= 0x3800;
        if (self->spikeBallPos.y < self->position.y) {
            self->state          = SSZSpikeBall_State_AwaitInterval;
            self->active         = ACTIVE_BOUNDS;
            self->spikeBallPos.y = -0x200000;
        }
    }
    else {
        self->velocity.y += 0x3800;
        if (self->spikeBallPos.y > self->position.y) {
            self->state          = SSZSpikeBall_State_AwaitInterval;
            self->active         = ACTIVE_BOUNDS;
            self->spikeBallPos.y = -0x200000;
        }
    }
}

void SSZSpikeBall_State_Ball_MoveOut(void)
{
    RSDK_THIS(SSZSpikeBall);

    self->spikeBallPos.x += self->velocity.x;
    self->spikeBallPos.y += self->velocity.y;

    if (!--self->timer) {
        self->timer = (self->interval >> 1) - self->dist;
        self->state = SSZSpikeBall_State_Ball_Stopped;
    }
}

void SSZSpikeBall_State_Ball_Stopped(void)
{
    RSDK_THIS(SSZSpikeBall);

    if (!--self->timer) {
        self->timer = self->dist;
        self->state = SSZSpikeBall_State_Ball_MoveBack;
    }
}

void SSZSpikeBall_State_Ball_MoveBack(void)
{
    RSDK_THIS(SSZSpikeBall);

    self->spikeBallPos.x -= self->velocity.x;
    self->spikeBallPos.y -= self->velocity.y;

    if (!--self->timer) {
        self->active = ACTIVE_BOUNDS;
        self->state  = SSZSpikeBall_State_AwaitInterval;
    }
}

#if GAME_INCLUDE_EDITOR
void SSZSpikeBall_EditorDraw(void)
{
    RSDK_THIS(SSZSpikeBall);

    RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 0, &self->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 1, &self->spikeBallAnimator, true, 0);

    self->spikeBallPos.x = self->position.x;
    self->spikeBallPos.y = self->position.y;

    switch (self->type) {
        case SSZSPIKEBALL_LAUNCHER_UP:
        case SSZSPIKEBALL_LAUNCHER_LEFT: self->direction = FLIP_NONE; break;

        case SSZSPIKEBALL_LAUNCHER_DOWN:
        case SSZSPIKEBALL_LAUNCHER_RIGHT: self->direction = FLIP_X; break;
        default: break;
    }

    self->baseAnimator.frameID = MIN(self->type, 4);

    SSZSpikeBall_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        int32 speed = self->speed;
        int32 dist  = self->dist;

        self->speed <<= 15;
        self->dist = (self->dist << 16) / self->speed;

        int32 distance = self->dist * self->speed;

        switch (self->type) {
            case SSZSPIKEBALL_LAUNCHER_UP:
                self->spikeBallPos.y += 0x40000;
                self->spikeBallPos.y += 0x8000 * 48;

                self->velocity.y = -self->speed;
                while (self->velocity.y < 0) {
                    self->spikeBallPos.y += self->velocity.y;

                    self->velocity.y += 0x3800;
                }

                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->spikeBallPos.x, self->spikeBallPos.y, 0xFFFF00, INK_NONE, 0xFF);

                self->inkEffect = INK_BLEND;
                RSDK.DrawSprite(&self->spikeBallAnimator, &self->spikeBallPos, false);
                self->inkEffect = INK_NONE;
                break;

            case SSZSPIKEBALL_LAUNCHER_DOWN:
                self->spikeBallPos.y -= 0x40000;
                self->spikeBallPos.y -= 0x8000 * 48;

                self->velocity.y = self->speed;
                while (self->velocity.y > 0) {
                    self->spikeBallPos.y += self->velocity.y;

                    self->velocity.y -= 0x3800;
                }

                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->spikeBallPos.x, self->spikeBallPos.y, 0xFFFF00, INK_NONE, 0xFF);

                self->inkEffect = INK_BLEND;
                RSDK.DrawSprite(&self->spikeBallAnimator, &self->spikeBallPos, false);
                self->inkEffect = INK_NONE;
                break;

            case SSZSPIKEBALL_LAUNCHER_LEFT:
                self->spikeBallPos.x += 0x40000;
                self->spikeBallPos.x += 0x8000 * 48;

                self->velocity.x = -self->speed;
                while (self->velocity.x < 0) {
                    self->spikeBallPos.x += self->velocity.x;

                    self->velocity.x += 0x3800;
                }

                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->spikeBallPos.x, self->spikeBallPos.y, 0xFFFF00, INK_NONE, 0xFF);

                self->inkEffect = INK_BLEND;
                RSDK.DrawSprite(&self->spikeBallAnimator, &self->spikeBallPos, false);
                self->inkEffect = INK_NONE;
                break;

            case SSZSPIKEBALL_LAUNCHER_RIGHT:
                self->spikeBallPos.x -= 0x40000;
                self->spikeBallPos.x -= 0x8000 * 48;

                self->velocity.x = self->speed;
                while (self->velocity.x > 0) {
                    self->spikeBallPos.x += self->velocity.x;

                    self->velocity.x -= 0x3800;
                }

                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->spikeBallPos.x, self->spikeBallPos.y, 0xFFFF00, INK_NONE, 0xFF);

                self->inkEffect = INK_BLEND;
                RSDK.DrawSprite(&self->spikeBallAnimator, &self->spikeBallPos, false);
                self->inkEffect = INK_NONE;
                break;

            case SSZSPIKEBALL_MOVEBALL_UP:
                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x, self->position.y - distance, 0xFFFF00, INK_NONE, 0xFF);

                self->spikeBallPos.y -= distance;
                self->inkEffect = INK_BLEND;
                RSDK.DrawSprite(&self->spikeBallAnimator, &self->spikeBallPos, false);
                self->inkEffect = INK_NONE;
                break;

            case SSZSPIKEBALL_MOVEBALL_DOWN:
                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x, self->position.y + distance, 0xFFFF00, INK_NONE, 0xFF);

                self->spikeBallPos.y += distance;
                self->inkEffect = INK_BLEND;
                RSDK.DrawSprite(&self->spikeBallAnimator, &self->spikeBallPos, false);
                self->inkEffect = INK_NONE;
                break;

            case SSZSPIKEBALL_MOVEBALL_LEFT:
                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x - distance, self->position.y, 0xFFFF00, INK_NONE, 0xFF);

                self->spikeBallPos.x -= distance;
                self->inkEffect = INK_BLEND;
                RSDK.DrawSprite(&self->spikeBallAnimator, &self->spikeBallPos, false);
                self->inkEffect = INK_NONE;
                break;

            case SSZSPIKEBALL_MOVEBALL_RIGHT:
                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x + distance, self->position.y, 0xFFFF00, INK_NONE, 0xFF);

                self->spikeBallPos.x += distance;
                self->inkEffect = INK_BLEND;
                RSDK.DrawSprite(&self->spikeBallAnimator, &self->spikeBallPos, false);
                self->inkEffect = INK_NONE;
                break;
        }

        self->speed = speed;
        self->dist  = dist;

        RSDK_DRAWING_OVERLAY(false);
    }
}

void SSZSpikeBall_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeBall.bin", SCOPE_STAGE);
    else
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SpikeBall.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SSZSpikeBall, type);
    RSDK_ENUM_VAR("Launcher (Up)", SSZSPIKEBALL_LAUNCHER_UP);
    RSDK_ENUM_VAR("Launcher (Down)", SSZSPIKEBALL_LAUNCHER_DOWN);
    RSDK_ENUM_VAR("Launcher (Left)", SSZSPIKEBALL_LAUNCHER_LEFT);
    RSDK_ENUM_VAR("Launcher (Right)", SSZSPIKEBALL_LAUNCHER_RIGHT);
    RSDK_ENUM_VAR("Moving Ball (Up)", SSZSPIKEBALL_MOVEBALL_UP);
    RSDK_ENUM_VAR("Moving Ball (Down)", SSZSPIKEBALL_MOVEBALL_DOWN);
    RSDK_ENUM_VAR("Moving Ball (Left)", SSZSPIKEBALL_MOVEBALL_LEFT);
    RSDK_ENUM_VAR("Moving Ball (Right)", SSZSPIKEBALL_MOVEBALL_RIGHT);

    RSDK_ACTIVE_VAR(SSZSpikeBall, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flipped", FLIP_X);
}
#endif

void SSZSpikeBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_ENUM, dist);
}
