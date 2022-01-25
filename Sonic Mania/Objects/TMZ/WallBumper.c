// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: WallBumper Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectWallBumper *WallBumper;

void WallBumper_Update(void) { WallBumper_HandleInteractions(); }

void WallBumper_LateUpdate(void) {}

void WallBumper_StaticUpdate(void) {}

void WallBumper_Draw(void)
{
    RSDK_THIS(WallBumper);
    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    if (self->type) {
        self->direction ^= self->reverse;
        drawPos.x -= self->size << 20;
        for (int32 i = 0; i < self->size; ++i) {
            RSDK.DrawSprite(&self->animator, &drawPos, false);
            drawPos.x += 0x200000;
        }
        self->direction ^= self->reverse;
    }
    else {
        self->direction ^= FLIP_Y * self->reverse;
        drawPos.y -= self->size << 20;
        for (int32 i = 0; i < self->size; ++i) {
            RSDK.DrawSprite(&self->animator, &drawPos, false);
            drawPos.y += 0x200000;
        }
        self->direction ^= FLIP_Y * self->reverse;
    }
}

void WallBumper_Create(void *data)
{
    RSDK_THIS(WallBumper);
    self->drawFX        = FX_FLIP;
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    if (!self->type) {
        self->updateRange.y = (self->size + 4) << 20;
        self->hitbox.left   = 0;
        self->hitbox.top    = -16 - (16 * self->size);
        self->hitbox.right  = 8;
        self->hitbox.bottom = (16 * self->size) - 16;
    }
    else {
        self->direction *= FLIP_Y;
        self->updateRange.x = (self->size + 4) << 20;
        self->hitbox.left   = -16 - (16 * self->size);
        self->hitbox.top    = 0;
        self->hitbox.right  = (16 * self->size) - 16;
        self->hitbox.bottom = 8;
    }
    RSDK.SetSpriteAnimation(WallBumper->aniFrames, self->type, &self->animator, true, 0);
}

void WallBumper_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        WallBumper->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/WallBumper.bin", SCOPE_STAGE);
    WallBumper->sfxBouncer = RSDK.GetSfx("TMZ1/Bouncer.wav");

    DEBUGMODE_ADD_OBJ(WallBumper);
}

void WallBumper_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(WallBumper, NULL, self->position.x, self->position.y);
}

void WallBumper_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(WallBumper->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void WallBumper_HandleInteractions(void)
{
    RSDK_THIS(WallBumper);

    foreach_active(Player, player)
    {
        if (player->animator.animationID != ANI_HURT && Player_CheckBadnikTouch(player, self, &self->hitbox)) {
            if (self->type) {
                if (self->direction) {
                    if (player->velocity.y < 0)
                        continue;
                    player->velocity.y = -0x80000;
                }
                else {
                    if (player->velocity.y > 0)
                        continue;
                    player->velocity.y = 0x80000;
                }
                if (self->reverse) {
                    player->velocity.x = -0x80000;
                    player->groundVel  = -0x80000;
                }
                else {
                    player->velocity.x = 0x80000;
                    player->groundVel  = 0x80000;
                }
            }
            else {
                if (self->direction) {
                    if (player->velocity.x < 0)
                        continue;
                    player->velocity.x = -0x80000;
                    player->groundVel  = -0x80000;
                }
                else {
                    if (player->velocity.x > 0)
                        continue;
                    player->velocity.x = 0x80000;
                    player->groundVel  = 0x80000;
                }

                if (!self->reverse)
                    player->velocity.y = -0x80000;
                else
                    player->velocity.y = 0x80000;
            }
            RSDK.PlaySfx(WallBumper->sfxBouncer, false, 255);
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, false, 0);

            if (player->state == Player_State_FlyCarried) {
                RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;
            }
            player->jumpAbility    = 0;
            player->onGround       = false;
            player->tileCollisions = true;
            player->onGround       = false;
            player->state          = Player_State_Air;
            if (self->destructible) {
                destroyEntity(self);
                foreach_break;
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void WallBumper_EditorDraw(void)
{
    RSDK_THIS(WallBumper);

    int dir = self->direction;

    if (!self->type) {
        self->updateRange.y = (self->size + 4) << 20;
    }
    else {
        self->direction *= FLIP_Y;
        self->updateRange.x = (self->size + 4) << 20;
    }
    RSDK.SetSpriteAnimation(WallBumper->aniFrames, self->type, &self->animator, true, 0);

    WallBumper_Draw();

    self->direction = dir;
}

void WallBumper_EditorLoad(void) { WallBumper->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/WallBumper.bin", SCOPE_STAGE); }
#endif

void WallBumper_Serialize(void)
{
    RSDK_EDITABLE_VAR(WallBumper, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(WallBumper, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(WallBumper, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(WallBumper, VAR_BOOL, reverse);
    RSDK_EDITABLE_VAR(WallBumper, VAR_BOOL, destructible);
}
