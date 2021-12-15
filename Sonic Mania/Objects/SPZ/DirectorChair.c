// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DirectorChair Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectDirectorChair *DirectorChair;

void DirectorChair_Update(void) { Platform_Update(); }

void DirectorChair_LateUpdate(void) {}

void DirectorChair_StaticUpdate(void) {}

void DirectorChair_Draw(void)
{
    RSDK_THIS(DirectorChair);
    Vector2 drawPos;

    int sin = RSDK.Sin512(self->rotation);

    drawPos                  = self->centerPos;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
    RSDK.DrawSprite(&self->animator, &self->centerPos, false);

    drawPos.x += -0x10000 - (RSDK.Cos512(self->rotation) << 11);
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    self->direction = FLIP_X;
    self->rotation  = 0x100 - self->rotation;
    drawPos.y += sin << 12;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    self->direction = FLIP_NONE;
    self->rotation  = 0x100 - self->rotation;

    for (int i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->direction = FLIP_X;
        self->rotation  = 0x100 - self->rotation;
        drawPos.y += sin << 12;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->direction = FLIP_NONE;
        self->rotation  = 0x100 - self->rotation;
    }
}

void DirectorChair_Create(void *data)
{
    RSDK_THIS(DirectorChair);

    RSDK.SetSpriteAnimation(DirectorChair->aniFrames, 0, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(DirectorChair->aniFrames, 1, &self->animator, true, 0);
    if (!SceneInfo->inEditor) {
        self->centerPos     = self->position;
        self->drawPos.x     = self->position.x;
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawFX        = FX_ROTATE | FX_FLIP;
        self->drawPos.y     = self->position.y;
        self->updateRange.x = 0x800000;
        self->updateRange.y = ((RSDK.Sin512(96) + 0x8000) << 8) + (RSDK.Sin512(96) << 12) * (self->size + 1);
        self->drawOrder     = Zone->drawOrderLow;
        self->field_E8      = (self->size >> 2) - 16;
        if (!self->type)
            self->rotation = self->field_E8;
        else
            self->rotation = -96;
        self->angle        = self->rotation << 8;
        self->state        = DirectorChair_Unknown2;
        self->stateCollide = DirectorChair_StateCollide_Chair;
        self->speed <<= 7;
        self->field_EC = maxVal(32 - 2 * self->size, 1);
    }
}

void DirectorChair_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        DirectorChair->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/DirectorChair.bin", SCOPE_STAGE);
    DirectorChair->hitbox1.top    = -18;
    DirectorChair->hitbox1.left   = -18;
    DirectorChair->hitbox1.right  = 18;
    DirectorChair->hitbox1.bottom = -4;
    DirectorChair->hitbox2.top    = -35;
    DirectorChair->hitbox2.left   = -24;
    DirectorChair->hitbox2.right  = -16;
    DirectorChair->hitbox2.bottom = -3;
    DirectorChair->hitbox3.top    = -35;
    DirectorChair->hitbox3.left   = 16;
    DirectorChair->hitbox3.right  = 24;
    DirectorChair->hitbox3.bottom = -3;
    DirectorChair->field_3        = 0;
    DirectorChair->field_4        = 0;
    DirectorChair->sfxUnravel     = RSDK.GetSfx("Stage/Unravel.wav");
    DirectorChair->sfxExtend      = RSDK.GetSfx("Stage/Extend.wav");
    DirectorChair->sfxRetract     = RSDK.GetSfx("Stage/Retract.wav");
}

void DirectorChair_StateCollide_Chair(void)
{
    RSDK_THIS(DirectorChair);

    int playerID         = 0;
    self->stoodPlayers = 0;
    foreach_active(Player, player)
    {
        bool32 prevOnGround = player->onGround;
        if (Player_CheckCollisionPlatform(player, self, &DirectorChair->hitbox1)) {
#if RETRO_USE_PLUS
            if (self->state == DirectorChair_Unknown2 && player->state == Player_State_MightyHammerDrop)
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
        if (Player_CheckCollisionBox(player, self, &DirectorChair->hitbox2) == C_TOP) {
            self->timer = 0;
            if (!prevOnGround) {
                self->stoodPlayers |= 1 << playerID;
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }
        if (Player_CheckCollisionBox(player, self, &DirectorChair->hitbox3) == C_TOP) {
            self->timer = 0;
            if (!prevOnGround) {
                self->stoodPlayers |= 1 << playerID;
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }

        Hitbox hitbox;
        hitbox.top    = 0;
        hitbox.bottom = (self->centerPos.y - self->drawPos.y) >> 16;
        hitbox.right  = (RSDK.Cos512(self->rotation) >> 5) + 8;
        hitbox.left   = -hitbox.right;
        Player_CheckCollisionBox(player, self, &hitbox);
        ++playerID;
    }
}

void DirectorChair_Unknown2(void)
{
    RSDK_THIS(DirectorChair);

    int sin           = RSDK.Sin512(self->rotation);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y + (sin << 8) + (sin << 12) * (self->size + 1);

    if (self->stood) {
        self->active   = ACTIVE_NORMAL;
        self->field_E4 = 512;
        if (!self->type) {
            RSDK.PlaySfx(DirectorChair->sfxUnravel, false, 255);
            self->state = DirectorChair_Unknown3;
        }
        else {
            ++DirectorChair->field_4;
            RSDK.PlaySfx(DirectorChair->sfxRetract, false, 255);
            self->state = DirectorChair_Unknown6;
        }
    }
}

void DirectorChair_Unknown3(void)
{
    RSDK_THIS(DirectorChair);

    self->angle += self->field_E4;
    self->field_E4 -= self->field_EC;
    self->rotation = self->angle >> 8;
    if (self->rotation >= 0) {
        self->rotation = 0;
        self->angle    = 0;
        self->field_E4 = -self->field_EC;
        ++DirectorChair->field_3;
        RSDK.PlaySfx(DirectorChair->sfxExtend, false, 255);
        self->state = DirectorChair_Unknown4;
    }

    int sin           = RSDK.Sin512(self->rotation);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y + (sin << 8) + (sin << 12) * (self->size + 1);
}

void DirectorChair_Unknown4(void)
{
    RSDK_THIS(DirectorChair);

    self->angle -= self->field_E4;
    if (self->field_E4 != self->speed) {
        if (self->field_E4 >= self->speed) {
            self->speed -= 32;
            if (self->field_E4 < self->speed)
                self->field_E4 = self->speed;
        }
        else {
            self->field_E4 += 0x20;
            if (self->field_E4 > self->speed)
                self->field_E4 = self->speed;
        }
    }

    self->rotation = self->angle >> 8;
    if (self->rotation <= -96) {
        self->rotation = -96;
        if (!--DirectorChair->field_3)
            RSDK.StopSfx(DirectorChair->sfxExtend);
        if (self->type) {
            self->active = ACTIVE_BOUNDS;
            self->state  = DirectorChair_Unknown2;
        }
        else {
            self->state = DirectorChair_Unknown5;
        }
    }

    int sin           = RSDK.Sin512(self->rotation);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y + (sin << 8) + (sin << 12) * (self->size + 1);
}

void DirectorChair_Unknown5(void)
{
    RSDK_THIS(DirectorChair);

    if (++self->timer >= 60) {
        self->timer = 0;
        self->field_E4      = 0;
        if (!self->type) {
            ++DirectorChair->field_4;
            RSDK.PlaySfx(DirectorChair->sfxRetract, false, 255);
            self->state = DirectorChair_Unknown6;
        }
        else {
            ++DirectorChair->field_3;
            RSDK.PlaySfx(DirectorChair->sfxExtend, false, 255);
            self->state = DirectorChair_Unknown4;
        }
    }

    int sin           = RSDK.Sin512(self->rotation);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y + (sin << 8) + (sin << 12) * (self->size + 1);
}

void DirectorChair_Unknown6(void)
{
    RSDK_THIS(DirectorChair);

    self->angle += self->field_E4;

    if (self->field_E4 != self->speed) {
        if (self->field_E4 >= self->speed) {
            self->field_E4 -= 32;
            if (self->field_E4 < self->speed)
                self->field_E4 = self->speed;
        }
        else {
            self->field_E4 += 0x20;
            if (self->field_E4 > self->speed)
                self->field_E4 = self->speed;
        }
    }

    self->rotation = self->angle >> 8;
    if (self->rotation >= self->field_E8) {
        if (!--DirectorChair->field_4)
            RSDK.StopSfx(DirectorChair->sfxRetract);
        self->rotation = self->field_E8;
        if (!self->type) {
            self->active = ACTIVE_BOUNDS;
            self->state  = DirectorChair_Unknown2;
        }
        else {
            self->state = DirectorChair_Unknown5;
        }
    }

    int sin           = RSDK.Sin512(self->rotation);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y + (sin << 8) + (sin << 12) * (self->size + 1);
}

#if RETRO_INCLUDE_EDITOR
void DirectorChair_EditorDraw(void)
{
    RSDK_THIS(DirectorChair);
    self->centerPos     = self->position;
    self->drawPos.x     = self->position.x;
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->drawPos.y     = self->position.y;
    self->updateRange.x = 0x800000;
    self->updateRange.y = ((RSDK.Sin512(96) + 0x8000) << 8) + (RSDK.Sin512(96) << 12) * (self->size + 1);
    self->drawOrder     = Zone->drawOrderLow;
    self->field_E8      = (self->size >> 2) - 16;
    if (!self->type)
        self->rotation = self->field_E8;
    else
        self->rotation = -96;
    self->angle = self->rotation << 8;
    self->field_EC = maxVal(32 - 2 * self->size, 1);

    DirectorChair_Draw();
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
