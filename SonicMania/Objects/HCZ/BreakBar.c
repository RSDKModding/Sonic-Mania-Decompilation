// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BreakBar Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBreakBar *BreakBar;

void BreakBar_Update(void)
{
    RSDK_THIS(BreakBar);

    StateMachine_Run(self->state);
}

void BreakBar_LateUpdate(void) { BreakBar_CheckPlayerCollisions(); }

void BreakBar_StaticUpdate(void) {}

void BreakBar_Draw(void) { BreakBar_DrawSprites(); }

void BreakBar_Create(void *data)
{
    RSDK_THIS(BreakBar);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    self->hitbox.left   = 24;
    self->hitbox.top    = 24;
    self->hitbox.right  = 32;
    self->hitbox.bottom = 32;

    if (self->orientation == BREAKBAR_V) {
        self->hitbox.top    = -8 - ((8 * self->length) >> 1);
        self->hitbox.bottom = ((8 * self->length) >> 1) + 8;
    }
    else {
        self->hitbox.left  = -8 - ((8 * self->length) >> 1);
        self->hitbox.right = ((8 * self->length) >> 1) + 8;
    }

    self->state = BreakBar_State_Init;
}

void BreakBar_StageLoad(void)
{
    BreakBar->aniFrames = RSDK.LoadSpriteAnimation("HCZ/BreakBar.bin", SCOPE_STAGE);

    BreakBar->sfxBreak = RSDK.GetSfx("Stage/LedgeBreak.wav");
}

void BreakBar_DrawSprites(void)
{
    RSDK_THIS(BreakBar);

    Vector2 drawPos = self->position;

    if (self->orientation != BREAKBAR_V) {
        drawPos.x += -0x40000 - (((8 * self->length) >> 1) << 16);
        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 1, &self->animator, true, 0);
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        drawPos.x += 0x80000;
        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 1, &self->animator, true, 1);
        for (int32 i = 0; i < self->length; ++i) {
            RSDK.DrawSprite(&self->animator, &drawPos, false);
            drawPos.x += 0x80000;
        }

        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 1, &self->animator, true, 2);
    }
    else {
        drawPos.y += -0x40000 - (((8 * self->length) >> 1) << 16);
        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 0, &self->animator, true, 0);
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        drawPos.y += 0x80000;
        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 0, &self->animator, true, 1);
        for (int32 i = 0; i < self->length; ++i) {
            RSDK.DrawSprite(&self->animator, &drawPos, false);
            drawPos.y += 0x80000;
        }

        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 0, &self->animator, true, 2);
    }

    RSDK.DrawSprite(&self->animator, &drawPos, false);
}

void BreakBar_CheckPlayerCollisions(void)
{
    RSDK_THIS(BreakBar);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if (((1 << playerID) & self->activePlayersGrabbed) && !((1 << playerID) & self->activePlayersReleased)) {
            if (!Player_CheckValidState(player)) {
                self->activePlayersGrabbed &= ~(1 << playerID);
            }
            else {
                player->direction = FLIP_NONE;

                if (self->orientation != BREAKBAR_V) {
                    player->position.y = self->startPos.y;

                    if (player->velocity.y <= 0) {
                        player->position.y -= 0x140000;
                        player->rotation = 0x180;
                    }
                    else {
                        player->position.y += 0x140000;
                        player->rotation = 0x080;
                    }
                }
                else {
                    player->position.x = self->startPos.x;

                    if (player->velocity.x <= 0) {
                        player->position.x -= 0x140000;
                        player->rotation = 0x100;
                    }
                    else {
                        player->position.x += 0x140000;
                        player->rotation = 0x000;
                    }
                }
            }
        }
    }
}

void BreakBar_HandlePlayerInteractions(EntityPlayer *player)
{
    RSDK_THIS(BreakBar);

    if (!self->isBroken) {
        int32 spawnX = self->position.x;
        int32 spawnY = self->position.y;

        if (self->orientation != BREAKBAR_V)
            spawnX += -0x40000 - ((8 * self->length) >> 1 << 16);
        else
            spawnY += -0x40000 - ((8 * self->length) >> 1 << 16);

        if (self->length != 0xFFFE) {
            int32 len = (self->length + 2) << 19;
            for (int32 i = 0; i < self->length + 2; ++i) {
                int32 frame          = i == self->length + 1 ? 2 : (i != 0);
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, spawnX, spawnY);

                debris->drawGroup       = Zone->objectDrawGroup[0];
                debris->gravityStrength = player->underwater ? 0x2000 : 0x3800;
                debris->velocity.x      = player->velocity.x >> 3;
                debris->velocity.y      = player->velocity.y >> 3;

                if (self->orientation != BREAKBAR_V) {
                    RSDK.SetSpriteAnimation(BreakBar->aniFrames, 1, &debris->animator, true, frame);
                    debris->velocity.x = abs(debris->position.x - player->position.x) >> 6;

                    if (debris->position.x < player->position.x)
                        debris->velocity.x = -(abs(debris->position.x - player->position.x) >> 6);

                    if (len > abs(debris->position.x - player->position.x))
                        debris->velocity.y += (2 * (player->velocity.y > 0) - 1) * (MAX(len - abs(debris->position.x - player->position.x), 0) >> 5);

                    debris->velocity.y = (3 * ((RSDK.Rand(-12, 12) << 10) + debris->velocity.y)) >> 3;
                    spawnX += 0x80000;
                }
                else {
                    RSDK.SetSpriteAnimation(BreakBar->aniFrames, 0, &debris->animator, true, frame);

                    debris->velocity.y = abs(debris->position.y - player->position.y) >> 6;

                    if (debris->position.y < player->position.y)
                        debris->velocity.y = -debris->velocity.y;

                    if (len > abs(debris->position.y - player->position.y))
                        debris->velocity.x += (2 * (player->velocity.x > 0) - 1) * (MAX(len - abs(debris->position.y - player->position.y), 0) >> 5);

                    debris->velocity.x = (3 * ((RSDK.Rand(-12, 12) << 10) + debris->velocity.x)) >> 3;
                    spawnY += 0x80000;
                }
            }
        }

        RSDK.PlaySfx(BreakBar->sfxBreak, false, 255);
        self->isBroken = true;
    }
}

void BreakBar_State_Init(void)
{
    RSDK_THIS(BreakBar);

    self->activePlayersGrabbed  = 0;
    self->activePlayersReleased = 0;
    self->releaseTimer          = 0;
    self->state                 = BreakBar_State_Main;
}

void BreakBar_State_Main(void)
{
    RSDK_THIS(BreakBar);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((player->sidekick && Player->respawnTimer >= 239) || !Player_CheckValidState(player)) {
            self->activePlayersReleased &= ~(1 << playerID);
            self->activePlayersGrabbed &= ~(1 << playerID);
            self->playerTimers[playerID] = 8;
        }
        else {
            if (self->playerTimers[playerID]) {
                self->playerTimers[playerID]--;
            }
            else if (!((1 << playerID) & self->activePlayersGrabbed) && !((1 << playerID) & self->activePlayersReleased)) {
                self->direction = FLIP_NONE;

                if (player->velocity.x < 0)
                    self->direction = FLIP_X;

                if (player->velocity.y < 0)
                    self->direction |= FLIP_Y;

                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    self->activePlayersGrabbed |= 1 << playerID;
                    RSDK.PlaySfx(Player->sfxGrab, false, 255);
                }
            }

            if (((1 << playerID) & self->activePlayersGrabbed)) {
                if (!((1 << playerID) & self->activePlayersReleased)) {
                    if (player->jumpPress || self->releaseTimer >= 240 || self->isBroken) {
                        self->activePlayersReleased |= 1 << playerID;
                    }
                    else {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_CLING, &player->animator, false, 0);
                        player->onGround        = false;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                        player->state           = Player_State_Static;
                    }

                    if (!player->sidekick && globals->gameMode != MODE_COMPETITION)
                        ++self->releaseTimer;
                }
            }

            if (((1 << playerID) & self->activePlayersReleased)) {
                if (globals->gameMode == MODE_COMPETITION) {
                    self->activePlayersReleased &= ~(1 << playerID);
                    self->activePlayersGrabbed &= ~(1 << playerID);
                    self->playerTimers[playerID] = 8;
                }
                else if (!player->sidekick && !self->isBroken) {
                    BreakBar_HandlePlayerInteractions(player);
                }

                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
                player->state = Player_State_Static;
            }
        }
    }

    if (self->isBroken)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void BreakBar_EditorDraw(void) { BreakBar_DrawSprites(); }

void BreakBar_EditorLoad(void)
{
    BreakBar->aniFrames = RSDK.LoadSpriteAnimation("HCZ/BreakBar.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(BreakBar, orientation);
    RSDK_ENUM_VAR("Vertical", BREAKBAR_V);
    RSDK_ENUM_VAR("Horizontal", BREAKBAR_H);
}
#endif

void BreakBar_Serialize(void)
{
    RSDK_EDITABLE_VAR(BreakBar, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(BreakBar, VAR_UINT16, length);
}
