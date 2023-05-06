// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DirectorChair Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDirectorChair *DirectorChair;

void DirectorChair_Update(void) { Platform_Update(); }

void DirectorChair_LateUpdate(void) {}

void DirectorChair_StaticUpdate(void) {}

void DirectorChair_Draw(void)
{
    RSDK_THIS(DirectorChair);

    Vector2 drawPos;
    int32 sin = RSDK.Sin512(self->rotation);

    drawPos                = self->centerPos;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
    RSDK.DrawSprite(&self->animator, &self->centerPos, false);

    drawPos.x += -0x10000 - (RSDK.Cos512(self->rotation) << 11);
    RSDK.DrawSprite(&self->scissorAnimator, &drawPos, false);

    self->direction = FLIP_X;
    self->rotation  = 0x100 - self->rotation;
    drawPos.y += sin << 12;
    RSDK.DrawSprite(&self->scissorAnimator, &drawPos, false);

    self->direction = FLIP_NONE;
    self->rotation  = 0x100 - self->rotation;

    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->scissorAnimator, &drawPos, false);

        self->direction = FLIP_X;
        self->rotation  = 0x100 - self->rotation;
        drawPos.y += sin << 12;
        RSDK.DrawSprite(&self->scissorAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        self->rotation  = 0x100 - self->rotation;
    }
}

void DirectorChair_Create(void *data)
{
    RSDK_THIS(DirectorChair);

    RSDK.SetSpriteAnimation(DirectorChair->aniFrames, 0, &self->scissorAnimator, true, 0);
    RSDK.SetSpriteAnimation(DirectorChair->aniFrames, 1, &self->animator, true, 0);

    if (!SceneInfo->inEditor) {
        self->centerPos     = self->position;
        self->drawPos       = self->position;
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawFX        = FX_ROTATE | FX_FLIP;
        self->updateRange.x = 0x800000;
        self->updateRange.y = ((RSDK.Sin512(96) + 0x8000) << 8) + (RSDK.Sin512(96) << 12) * (self->size + 1);
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->minRetract    = (self->size >> 2) - 16;
        self->rotation      = self->type == DIRECTORCHAIR_CLOSED ? self->minRetract : -96;
        self->angle         = self->rotation << 8;
        self->state         = DirectorChair_State_Idle;
        self->stateCollide  = DirectorChair_Collide_Chair;
        self->speed <<= 7;
        self->initExtendVel = MAX(32 - 2 * self->size, 1);
    }
}

void DirectorChair_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        DirectorChair->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/DirectorChair.bin", SCOPE_STAGE);

    DirectorChair->hitboxChair.top    = -18;
    DirectorChair->hitboxChair.left   = -18;
    DirectorChair->hitboxChair.right  = 18;
    DirectorChair->hitboxChair.bottom = -4;

    DirectorChair->hitboxL.top    = -35;
    DirectorChair->hitboxL.left   = -24;
    DirectorChair->hitboxL.right  = -16;
    DirectorChair->hitboxL.bottom = -3;

    DirectorChair->hitboxR.top    = -35;
    DirectorChair->hitboxR.left   = 16;
    DirectorChair->hitboxR.right  = 24;
    DirectorChair->hitboxR.bottom = -3;

    DirectorChair->extendCount  = 0;
    DirectorChair->retractCount = 0;

    DirectorChair->sfxUnravel = RSDK.GetSfx("Stage/Unravel.wav");
    DirectorChair->sfxExtend  = RSDK.GetSfx("Stage/Extend.wav");
    DirectorChair->sfxRetract = RSDK.GetSfx("Stage/Retract.wav");
}

void DirectorChair_Collide_Chair(void)
{
    RSDK_THIS(DirectorChair);

    int32 playerID     = 0;
    self->stoodPlayers = 0;

    foreach_active(Player, player)
    {
        bool32 prevOnGround = player->onGround;
        if (Player_CheckCollisionPlatform(player, self, &DirectorChair->hitboxChair)) {
#if MANIA_USE_PLUS
            if (self->state == DirectorChair_State_Idle && player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
#endif
            self->timer = 0;
            if (!prevOnGround) {
                self->stoodPlayers |= 1 << playerID;
                self->stood = true;
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }

        if (Player_CheckCollisionBox(player, self, &DirectorChair->hitboxL) == C_TOP) {
            self->timer = 0;
            if (!prevOnGround) {
                self->stoodPlayers |= 1 << playerID;
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }

        if (Player_CheckCollisionBox(player, self, &DirectorChair->hitboxR) == C_TOP) {
            self->timer = 0;
            if (!prevOnGround) {
                self->stoodPlayers |= 1 << playerID;
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }

        // Extend Hitbox
        Hitbox hitbox;
        hitbox.right  = (RSDK.Cos512(self->rotation) >> 5) + 8;
        hitbox.bottom = (self->centerPos.y - self->drawPos.y) >> 16;
        hitbox.left   = -hitbox.right;
        hitbox.top    = 0;
        Player_CheckCollisionBox(player, self, &hitbox);

        ++playerID;
    }
}

void DirectorChair_State_Idle(void)
{
    RSDK_THIS(DirectorChair);

    int32 sin       = RSDK.Sin512(self->rotation);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y + (sin << 8) + (sin << 12) * (self->size + 1);

    if (self->stood) {
        self->active    = ACTIVE_NORMAL;
        self->extendVel = 512;
        if (self->type == DIRECTORCHAIR_CLOSED) {
            RSDK.PlaySfx(DirectorChair->sfxUnravel, false, 0xFF);
            self->state = DirectorChair_State_StartExtend;
        }
        else {
            ++DirectorChair->retractCount;
            RSDK.PlaySfx(DirectorChair->sfxRetract, false, 0xFF);
            self->state = DirectorChair_State_Retract;
        }
    }
}

void DirectorChair_State_StartExtend(void)
{
    RSDK_THIS(DirectorChair);

    self->angle += self->extendVel;
    self->extendVel -= self->initExtendVel;
    self->rotation = self->angle >> 8;

    if (self->rotation >= 0) {
        self->rotation  = 0;
        self->angle     = 0;
        self->extendVel = -self->initExtendVel;
        ++DirectorChair->extendCount;
        RSDK.PlaySfx(DirectorChair->sfxExtend, false, 0xFF);
        self->state = DirectorChair_State_Extend;
    }

    int32 sin       = RSDK.Sin512(self->rotation);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y + (sin << 8) + (sin << 12) * (self->size + 1);
}

void DirectorChair_State_Extend(void)
{
    RSDK_THIS(DirectorChair);

    self->angle -= self->extendVel;
    if (self->extendVel != self->speed) {
        if (self->extendVel >= self->speed) {
            self->speed -= 32;
            if (self->extendVel < self->speed)
                self->extendVel = self->speed;
        }
        else {
            self->extendVel += 0x20;
            if (self->extendVel > self->speed)
                self->extendVel = self->speed;
        }
    }

    self->rotation = self->angle >> 8;
    if (self->rotation <= -96) {
        self->rotation = -96;

        if (!--DirectorChair->extendCount)
            RSDK.StopSfx(DirectorChair->sfxExtend);

        if (self->type == DIRECTORCHAIR_CLOSED) {
            self->state = DirectorChair_State_StartRetract;
        }
        else {
            self->active = ACTIVE_BOUNDS;
            self->state  = DirectorChair_State_Idle;
        }
    }

    int32 sin       = RSDK.Sin512(self->rotation);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y + (sin << 8) + (sin << 12) * (self->size + 1);
}

void DirectorChair_State_StartRetract(void)
{
    RSDK_THIS(DirectorChair);

    if (++self->timer >= 60) {
        self->timer     = 0;
        self->extendVel = 0;

        if (self->type == DIRECTORCHAIR_CLOSED) {
            ++DirectorChair->retractCount;
            RSDK.PlaySfx(DirectorChair->sfxRetract, false, 0xFF);
            self->state = DirectorChair_State_Retract;
        }
        else {
            ++DirectorChair->extendCount;
            RSDK.PlaySfx(DirectorChair->sfxExtend, false, 0xFF);
            self->state = DirectorChair_State_Extend;
        }
    }

    int32 sin       = RSDK.Sin512(self->rotation);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y + (sin << 8) + (sin << 12) * (self->size + 1);
}

void DirectorChair_State_Retract(void)
{
    RSDK_THIS(DirectorChair);

    self->angle += self->extendVel;

    if (self->extendVel != self->speed) {
        if (self->extendVel >= self->speed) {
            self->extendVel -= 32;
            if (self->extendVel < self->speed)
                self->extendVel = self->speed;
        }
        else {
            self->extendVel += 0x20;
            if (self->extendVel > self->speed)
                self->extendVel = self->speed;
        }
    }

    self->rotation = self->angle >> 8;
    if (self->rotation >= self->minRetract) {
        if (!--DirectorChair->retractCount)
            RSDK.StopSfx(DirectorChair->sfxRetract);

        self->rotation = self->minRetract;
        if (self->type == DIRECTORCHAIR_CLOSED) {
            self->active = ACTIVE_BOUNDS;
            self->state  = DirectorChair_State_Idle;
        }
        else {
            self->state = DirectorChair_State_StartRetract;
        }
    }

    int32 sin       = RSDK.Sin512(self->rotation);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y + (sin << 8) + (sin << 12) * (self->size + 1);
}

#if GAME_INCLUDE_EDITOR
void DirectorChair_EditorDraw(void)
{
    RSDK_THIS(DirectorChair);

    self->centerPos     = self->position;
    self->drawPos       = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = ((RSDK.Sin512(96) + 0x8000) << 8) + (RSDK.Sin512(96) << 12) * (self->size + 1);
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->minRetract    = (self->size >> 2) - 16;
    self->rotation      = self->type == DIRECTORCHAIR_CLOSED ? self->minRetract : -96;
    self->angle         = self->rotation << 8;
    self->initExtendVel = MAX(32 - 2 * self->size, 1);

    int32 sin       = RSDK.Sin512(self->rotation);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y + (sin << 8) + (sin << 12) * (self->size + 1);

    DirectorChair_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        self->rotation      = self->type != DIRECTORCHAIR_CLOSED ? self->minRetract : -96;
        self->angle         = self->rotation << 8;
        self->initExtendVel = MAX(32 - 2 * self->size, 1);

        self->drawPos.x = self->centerPos.x;
        self->drawPos.y = self->centerPos.y + (RSDK.Sin512(self->rotation) << 8) + (RSDK.Sin512(self->rotation) << 12) * (self->size + 1);

        self->inkEffect = INK_BLEND;
        DirectorChair_Draw();
        self->inkEffect = INK_NONE;

        RSDK_DRAWING_OVERLAY(false);
    }
}

void DirectorChair_EditorLoad(void)
{
    DirectorChair->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/DirectorChair.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(DirectorChair, type);
    RSDK_ENUM_VAR("Closed", DIRECTORCHAIR_CLOSED);
    RSDK_ENUM_VAR("Stretched Out", DIRECTORCHAIR_STRETCHED);
}
#endif

void DirectorChair_Serialize(void)
{
    RSDK_EDITABLE_VAR(DirectorChair, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(DirectorChair, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(DirectorChair, VAR_ENUM, size);
}
