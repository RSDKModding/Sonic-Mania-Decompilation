// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Whirlpool Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectWhirlpool *Whirlpool;

void Whirlpool_Update(void)
{
    RSDK_THIS(Whirlpool);

    self->angle += self->angVel;
    if (self->angle < 0)
        self->angle += 0x400;
    self->angle %= 0x3FF;

    // LaundroMobile - whirlpool starting
    if (self->activePlayers == 0xFF) {
        if (self->alpha < 0x100)
            self->alpha += 2;
    }
    else if (self->activePlayers != 0xFE) {
        // LaundroMobile - whirlpool finishing
        if (self->activePlayers == 0xFD) {
            self->alpha -= 2;

            if (self->alpha <= 0)
                destroyEntity(self);
        }
        else {
            foreach_active(Player, player)
            {
                int32 playerID = RSDK.GetEntitySlot(player);

                if (!((1 << playerID) & self->activePlayers)) {
                    if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                        self->active = ACTIVE_NORMAL;
                        self->activePlayers |= (1 << playerID);
                        player->nextGroundState         = 0;
                        player->nextAirState            = 0;
                        player->velocity.x              = 0;
                        player->velocity.y              = 0;
                        player->groundVel               = 0;
                        player->onGround                = false;
                        player->tileCollisions          = TILECOLLISION_DOWN;
                        player->state                   = Player_State_Air;
                        self->playerAmplitude[playerID] = abs(player->position.x - self->position.x) >> 16;

                        self->playerAngle[playerID] = player->position.x > self->position.x ? 0x000 : 0x200;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, true, 0);
                    }
                }

                if (self->activePlayers & (1 << playerID)) {
                    if (player->state == Player_State_Air) {
                        self->playerAngle[playerID] += self->angVel;

                        if (self->playerAngle[playerID] < 0)
                            self->playerAngle[playerID] += 0x400;
                        self->playerAngle[playerID] %= 0x3FF;

                        int32 newX         = self->position.x + ((RSDK.Cos1024(self->playerAngle[playerID]) << 6) * self->playerAmplitude[playerID]);
                        int32 newY         = player->position.y + (self->yVel << 15);
                        player->velocity.x = newX - player->position.x;
                        player->velocity.y = newY - player->position.y;
                        player->drawGroup  = self->playerAngle[playerID] <= 0x200 ? Zone->playerDrawGroup[1] : Zone->playerDrawGroup[0];

                        if (!Player_CheckCollisionTouch(player, self, &self->hitbox) || player->animator.animationID != ANI_FAN) {
                            self->activePlayers &= ~(1 << playerID);
                            player->state = Player_State_Air;
                        }
                    }
                    else {
                        self->activePlayers &= ~(1 << playerID);
                    }
                }
            }
        }
    }
}

void Whirlpool_LateUpdate(void) {}

void Whirlpool_StaticUpdate(void)
{
    bool32 whirlpoolActive = false;
    foreach_active(Whirlpool, pool)
    {
        if (pool->activePlayers) {
            whirlpoolActive  = true;
            Whirlpool->timer = 30;
        }
    }

    if (!whirlpoolActive) {
        if (Whirlpool->timer <= 30) {
            if (Whirlpool->timer > 0)
                Whirlpool->timer--;
        }
        else
            Whirlpool->timer = 30;
    }

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID == PauseMenu->classID)
        Whirlpool->timer = 0;

    if (Whirlpool->playingWhirlpoolSfx)
        RSDK.SetChannelAttributes(Whirlpool->sfxChannel, MIN(Whirlpool->timer, 30) / 30.0, 0.0, 1.0);

    if (Whirlpool->timer > 0) {
        if (!Whirlpool->playingWhirlpoolSfx) {
            Whirlpool->sfxChannel          = RSDK.PlaySfx(Whirlpool->sfxWhirlpool, 56284, 255);
            Whirlpool->playingWhirlpoolSfx = true;
        }
    }
    else if (!Whirlpool->timer) {
        if (Whirlpool->playingWhirlpoolSfx) {
            RSDK.StopSfx(Whirlpool->sfxWhirlpool);
            Whirlpool->playingWhirlpoolSfx = false;
        }
    }
}

void Whirlpool_Draw(void) { WhirlPool_DrawSprites(); }

void Whirlpool_Create(void *data)
{
    RSDK_THIS(Whirlpool);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->inkEffect     = INK_ADD;
    self->visible       = true;
    self->updateRange.x = 0x800000 + self->size.x;
    self->updateRange.y = 0x800000 + self->size.y;
    self->drawFX        = FX_FLIP;
    self->alpha         = 0x100;
    if (data)
        self->size = *((Vector2 *)data);

    self->hitbox.left   = -(self->size.x >> 17);
    self->hitbox.top    = -(self->size.y >> 17);
    self->hitbox.right  = self->size.x >> 17;
    self->hitbox.bottom = self->size.y >> 17;

    if (SceneInfo->inEditor) {
        if (!self->size.x && !self->size.y) {
            self->size.x = 0x800000;
            self->size.y = 0x800000;
        }

        if (!self->yVel)
            self->yVel = 4;

        if (!self->angVel)
            self->angVel = 12;
    }
    else {
        Whirlpool_SetupBubbles();
        RSDK.SetSpriteAnimation(Whirlpool->aniFrames, 3, &self->animator, true, 0);
    }
}

void Whirlpool_StageLoad(void)
{
    Whirlpool->active = ACTIVE_ALWAYS;

    Whirlpool->aniFrames = RSDK.LoadSpriteAnimation("Global/Water.bin", SCOPE_STAGE);

    Whirlpool->sfxWhirlpool = RSDK.GetSfx("HCZ/Whirlpool.wav");
}

void WhirlPool_DrawSprites(void)
{
    RSDK_THIS(Whirlpool);
    Vector2 drawPos;

    drawPos      = self->position;
    int32 offset = (self->yVel * Zone->timer % (self->size.y >> 15)) << 15;

    for (int32 i = 0; i < 0x80; ++i) {
        int32 angle = self->angle + self->bubbleAngles[i];
        if (angle < 0)
            angle += 0x400;

        drawPos.x = self->position.x + (self->bubbleAngles[i] >> 16) * (RSDK.Cos1024(angle % 0x3FF) << 6);
        drawPos.y = self->position.y + offset % self->size.y + (2 * (i & 1) - 1) * (((i % 6) << 17) & 0xFFFC0000) - (self->size.y >> 1);

        RSDK.SetSpriteAnimation(Whirlpool->aniFrames, 3, &self->animator, true, (i & 3) + 3);
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        offset += self->size.y >> 7;
    }
}

void Whirlpool_SetupBubbles(void)
{
    RSDK_THIS(Whirlpool);

    for (int32 i = 0; i < 0x80; ++i) {
        // bubble angle proper
        int32 angle_high = RSDK.Rand(self->hitbox.left, self->hitbox.right);

        // Extra subpixel randomization
        int32 angle_low = RSDK.Rand(0, 0x400);

        self->bubbleAngles[i] = (angle_high << 16) | angle_low;
    }
}

#if GAME_INCLUDE_EDITOR
void Whirlpool_EditorDraw(void)
{
    RSDK_THIS(Whirlpool);

    Whirlpool_Create(NULL);

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);
}

void Whirlpool_EditorLoad(void) {}
#endif

void Whirlpool_Serialize(void)
{
    RSDK_EDITABLE_VAR(Whirlpool, VAR_ENUM, angVel);
    RSDK_EDITABLE_VAR(Whirlpool, VAR_ENUM, yVel);
    RSDK_EDITABLE_VAR(Whirlpool, VAR_VECTOR2, size);
}
