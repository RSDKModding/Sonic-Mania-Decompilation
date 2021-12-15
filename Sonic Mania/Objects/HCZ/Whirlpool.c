// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Whirlpool Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectWhirlpool *Whirlpool;

void Whirlpool_Update(void)
{
    RSDK_THIS(Whirlpool);

    self->angle += self->angVel;
    if (self->angle < 0)
        self->angle += 1024;

    self->angle %= 0x3FF;
    if (self->activePlayers == 0xFF) {
        if (self->alpha < 256)
            self->alpha += 2;
    }
    else if (self->activePlayers != 0xFE) {
        if (self->activePlayers == 0xFD) {
            self->alpha -= 2;
            if (self->alpha <= 0)
                destroyEntity(self);
        }
        else {
            foreach_active(Player, player)
            {
                int32 playerID = RSDK.GetEntityID(player);
                if (!((1 << playerID) & self->activePlayers)) {
                    if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                        self->active = ACTIVE_NORMAL;
                        self->activePlayers |= (1 << playerID);
                        player->nextGroundState     = 0;
                        player->nextAirState        = 0;
                        player->velocity.x          = 0;
                        player->velocity.y          = 0;
                        player->groundVel           = 0;
                        player->onGround            = false;
                        player->tileCollisions      = true;
                        player->state               = Player_State_Air;
                        self->field_288[playerID] = abs(player->position.x - self->position.x) >> 16;

                        if (player->position.x > self->position.x)
                            self->field_278[playerID] = 0;
                        else
                            self->field_278[playerID] = 512;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, true, 0);
                    }
                }

                if ((self->activePlayers & (1 << playerID))) {
                    if (player->state == Player_State_Air) {
                        self->field_278[playerID] += self->angVel;

                        if (self->field_278[playerID] < 0)
                            self->field_278[playerID] += 0x400;
                        self->field_278[playerID] %= 0x3FF;
                        int32 prevX          = player->position.x;
                        int32 prevY          = player->position.y;
                        player->position.x = self->position.x;
                        player->position.x += (RSDK.Cos1024(self->field_278[playerID]) << 6) * self->field_288[playerID];
                        player->position.y += self->yVel << 15;
                        player->velocity.x = player->position.x - prevX;
                        player->velocity.y = player->position.y - prevY;
                        player->position.x = prevX;
                        player->position.y = prevY;
                        if (self->field_278[playerID] <= 512)
                            player->drawOrder = Zone->playerDrawHigh;
                        else
                            player->drawOrder = Zone->playerDrawLow;

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
    bool32 flag = false;
    foreach_active(Whirlpool, pool)
    {
        if (pool->activePlayers) {
            flag             = true;
            Whirlpool->timer = 30;
        }
    }

    if (!flag) {
        if (Whirlpool->timer <= 30) {
            if (Whirlpool->timer > 0)
                Whirlpool->timer--;
        }
        else {
            Whirlpool->timer = 30;
        }
    }

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID == PauseMenu->objectID)
        Whirlpool->timer = 0;

    if (Whirlpool->playingSFX) {
        RSDK.SetChannelAttributes(Whirlpool->sfxChannel, minVal(Whirlpool->timer, 30) / 30.0, 0.0, 1.0);
    }

    bool32 stopFlag = !Whirlpool->timer;
    if (Whirlpool->timer > 0) {
        if (!Whirlpool->playingSFX) {
            Whirlpool->sfxChannel = RSDK.PlaySfx(Whirlpool->sfxWhirlpool, 56284, 255);
            Whirlpool->playingSFX = true;
        }
    }
    else if (stopFlag) {
        if (Whirlpool->playingSFX) {
            RSDK.StopSfx(Whirlpool->sfxWhirlpool);
            Whirlpool->playingSFX = false;
        }
    }
}

void Whirlpool_Draw(void) { WhirlPool_DrawSprites(); }

void Whirlpool_Create(void *data)
{
    RSDK_THIS(Whirlpool);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->posUnknown2   = self->position;
    self->inkEffect     = INK_ADD;
    self->visible       = true;
    self->updateRange.x = 0x800000 + self->size.x;
    self->updateRange.y = 0x800000 + self->size.y;
    self->drawFX        = FX_FLIP;
    self->alpha         = 256;
    if (data)
        self->size = *(Vector2 *)data;

    self->hitbox.right  = self->size.x >> 17;
    self->hitbox.left   = -(self->size.x >> 17);
    self->hitbox.bottom = self->size.y >> 17;
    self->hitbox.top    = -(self->size.y >> 17);
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
    Whirlpool->active       = ACTIVE_ALWAYS;
    Whirlpool->aniFrames    = RSDK.LoadSpriteAnimation("Global/Water.bin", SCOPE_STAGE);
    Whirlpool->sfxWhirlpool = RSDK.GetSfx("HCZ/Whirlpool.wav");
}

void WhirlPool_DrawSprites(void)
{
    RSDK_THIS(Whirlpool);
    Vector2 drawPos;

    drawPos    = self->position;
    int32 offset = (self->yVel * Zone->timer % (self->size.y >> 15)) << 15;
    for (int32 i = 0; i < 0x80; ++i) {
        int32 angle = self->angle + self->field_78[i];
        if (angle < 0)
            angle += 0x400;
        drawPos.x = self->position.x;
        drawPos.x += (self->field_78[i] >> 16) * (RSDK.Cos1024(angle % 1023) << 6);
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
        int32 rand1           = RSDK.Rand(self->hitbox.left, self->hitbox.right);
        int32 rand2           = RSDK.Rand(0, 1024);
        self->field_78[i] = (rand1 << 16) | rand2;
    }
}

#if RETRO_INCLUDE_EDITOR
void Whirlpool_EditorDraw(void) {}

void Whirlpool_EditorLoad(void) {}
#endif

void Whirlpool_Serialize(void)
{
    RSDK_EDITABLE_VAR(Whirlpool, VAR_ENUM, angVel);
    RSDK_EDITABLE_VAR(Whirlpool, VAR_ENUM, yVel);
    RSDK_EDITABLE_VAR(Whirlpool, VAR_VECTOR2, size);
}
