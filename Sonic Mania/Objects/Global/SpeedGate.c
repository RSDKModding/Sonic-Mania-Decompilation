#include "SonicMania.h"

ObjectSpeedGate *SpeedGate;

void SpeedGate_Update(void)
{
    RSDK_THIS(SpeedGate);
    foreach_active(Player, player)
    {
        int32 vel = 0;
        if (player->onGround)
            vel = player->groundVel;
        else
            vel = player->velocity.x;
        int32 spd = vel >> 15;
        if (spd) {
            if (Player_CheckCollisionTouch(player, self, &SpeedGate->hitbox)) {
                int32 s = spd - 2;
                if (spd >= 0)
                    s = spd + 2;
                if (abs(s) > abs(self->field_70))
                    self->field_70 = s;
                self->field_74 = 0;
            }
        }
    }

    self->angle += self->field_70;
    if (self->field_70 <= 0) {
        if (self->angle <= -0x200) {
            ++self->field_74;
            self->angle += 512;

            self->field_70 += 4;
            if (self->field_70 > -2)
                self->field_70 = -2;

            if (self->field_70 == -2) {
                self->field_70 = 0;
                self->angle    = 0;
            }
        }
    }
    else {
        if (self->angle >= 0x200) {
            ++self->field_74;
            self->angle -= 0x200;
            self->field_70 -= 4;
            if (self->field_70 < 2)
                self->field_70 = 2;

            if (self->field_70 == 2) {
                self->field_70 = 0;
                self->angle    = 0;
            }
        }
    }

    StateMachine_Run(self->state);
}

void SpeedGate_LateUpdate(void) {}

void SpeedGate_StaticUpdate(void) {}

void SpeedGate_Draw(void)
{
    RSDK_THIS(SpeedGate);
    Vector2 drawPos;

    self->rotation = self->timer2;
    self->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&self->topData, NULL, false);
    RSDK.DrawSprite(&self->baseData, NULL, false);

    drawPos.y                 = self->position.y;
    self->drawFX = FX_SCALE;

    self->scale.x          = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                 = self->position.x + 0x30000;
    self->finsData.frameID = 1;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);

    self->scale.x          = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                = self->position.x - 0x30000;
    self->finsData.frameID = 0;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);
    drawPos.x                = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finsData.frameID = 1;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);

    self->scale.x          = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                = 0xB40 * RSDK.Cos512(self->angle & 0x7F) + self->position.x;
    self->finsData.frameID = 2;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);

    self->scale.x          = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finsData.frameID = 0;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);

    self->scale.x          = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                = self->position.x - 0xB40 * RSDK.Sin512(self->angle & 0x7F);
    self->finsData.frameID = 2;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);

    self->rotation = self->timer3;
    self->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&self->timerData, &self->posUnknown, false);

    self->drawFX = FX_NONE;
    if (self->state == SpeedGate_State_Unknown1)
        RSDK.DrawSprite(&self->bubbleData, NULL, false);
}

void SpeedGate_Create(void *data)
{
    RSDK_THIS(SpeedGate);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 0, &self->baseData, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 1, &self->topData, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 2, &self->timerData, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 5, &self->bubbleData, true, 0);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->playerDrawHigh;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->scale.y       = 0x200;
        if (self->timer < 1) {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->timerData, true, 0);
            RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 4, &self->finsData, true, 0);
            self->posUnknown.x = self->position.x;
            self->posUnknown.y = self->position.y - 0x200000;
        }
        else {
            RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 3, &self->finsData, true, 0);
            self->posUnknown.x = self->position.x;
            self->state        = SpeedGate_State_Unknown1;
            self->posUnknown.y = self->position.y - 0x200000;
        }
    }
}

void SpeedGate_StageLoad(void)
{
    SpeedGate->aniFrames   = RSDK.LoadSpriteAnimation("Global/SpeedGate.bin", SCOPE_STAGE);
    SpeedGate->sfxStarPost  = RSDK.GetSFX("Global/StarPost.wav");
    SpeedGate->hitbox.left   = -8;
    SpeedGate->hitbox.top    = -44;
    SpeedGate->hitbox.right  = 8;
    SpeedGate->hitbox.bottom = 20;
}

void SpeedGate_State_Unknown1(void)
{
    RSDK_THIS(SpeedGate);

    int32 playerBit = 1;
    foreach_active(Player, player)
    {
        if (!(playerBit & self->activePlayers)) {
            if (Player_CheckCollisionTouch(player, self, &SpeedGate->hitbox)) {
                self->state     = SpeedGate_State_Unknown2;
                self->active    = ACTIVE_BOUNDS;
                self->playerPtr = player;
                self->field_80  = self->position.x - player->position.x;
                self->field_84  = self->position.y - player->position.y;

                self->timer2 = 0;
                if (self->field_70 < 0)
                    self->timer2 = 0x200;
                RSDK.PlaySfx(SpeedGate->sfxStarPost, 0, 255);
                foreach_return;
            }
        }
        playerBit <<= 1;
    }
}

void SpeedGate_State_Unknown2(void)
{
    RSDK_THIS(SpeedGate);

    if (self->timer2 > 0x100) {
        self->timer2 -= 8;
    }
    else if (self->timer2 < 0x100) {
        self->timer2 += 8;
    }

    if (self->timer3 < 256)
        self->timer3 += 0x10;

    if (self->timer <= 0) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->timerData, true, 0);
        self->active = 4;
        self->state  = 0;
    }
    else {
        self->timer--;
        EntityPlayer *player = self->playerPtr;
        if (self->playerPtr) {
            int32 vel = 0x100000;
            if (self->playerPtr->direction == FLIP_X)
                vel = -0x100000;

            self->velocity.x += +((int32)(vel - self->field_80) >> 6) - (self->velocity.x >> 4);
            self->velocity.y += ((int32)(-0x200000 - self->field_84) >> 6) - (self->velocity.y >> 4);

            self->field_80 += self->velocity.x;
            self->field_84 += self->velocity.y;
            self->posUnknown.x = self->field_80;
            self->posUnknown.y = self->field_84;
            self->posUnknown.x += player->position.x;
            self->posUnknown.y += player->position.y;
            EntitySpeedGate *thisEntity = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, SpeedGate);
            if (Player_CheckCollisionTouch(player, &thisEntity, &SpeedGate->hitbox)) {
                RSDK.SetSpriteAnimation(0xFFFF, 0, &self->timerData, true, 0);
                self->state            = 0;
                self->angle            = 0;
                self->active           = ACTIVE_BOUNDS;
                thisEntity->velocity.x   = player->velocity.x;
                thisEntity->velocity.y   = -0x40000;
                thisEntity->posUnknown.x = self->posUnknown.x;
                thisEntity->posUnknown.y = self->posUnknown.y;
                thisEntity->timer3       = self->timer3;
                thisEntity->active       = ACTIVE_NORMAL;
                thisEntity->state        = SpeedGate_State_Unknown3;
                RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 2, &thisEntity->timerData, true, 0);
                thisEntity->timer2 = 0;
                if (thisEntity->field_70 < 0)
                    thisEntity->timer2 = 0x200;
                RSDK.PlaySfx(SpeedGate->sfxStarPost, 0, 255);
            }
        }
    }
}

void SpeedGate_State_Unknown3(void)
{
    RSDK_THIS(SpeedGate);
    int32 y = self->position.y - 0x200000;

    self->velocity.x += ((self->position.x - self->posUnknown.x) >> 5) - (self->velocity.x >> 3);
    self->velocity.y += ((y - self->posUnknown.y) >> 5) - (self->velocity.y >> 3);
    self->posUnknown.x += self->velocity.x;
    self->posUnknown.y += self->velocity.y;

    if (self->timer2 > 0x100) {
        self->timer2 -= 8;
    }
    else if (self->timer2 < 0x100) {
        self->timer2 += 8;
    }

    if (self->timer3 < 512) {
        self->timer3 += 16;
    }
    if (++self->timer == 48) {
        self->posUnknown.x = self->position.x;
        self->timer        = 0;
        self->posUnknown.y = y;
        self->active       = ACTIVE_BOUNDS;
        self->state        = StateMachine_None;
    }
}

#if RETRO_INCLUDE_EDITOR
void SpeedGate_EditorDraw(void)
{
    RSDK_THIS(SpeedGate);
    Vector2 drawPos;

    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 0, &self->baseData, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 1, &self->topData, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 2, &self->timerData, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 5, &self->bubbleData, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 3, &self->finsData, true, 0);

    self->rotation = self->timer2;
    self->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&self->topData, NULL, false);
    RSDK.DrawSprite(&self->baseData, NULL, false);

    drawPos.y      = self->position.y;
    self->drawFX = FX_SCALE;

    self->scale.x          = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                = self->position.x + 0x30000;
    self->finsData.frameID = 1;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);

    self->scale.x          = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                = self->position.x - 0x30000;
    self->finsData.frameID = 0;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);
    drawPos.x                = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finsData.frameID = 1;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);

    self->scale.x          = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                = 0xB40 * RSDK.Cos512(self->angle & 0x7F) + self->position.x;
    self->finsData.frameID = 2;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);

    self->scale.x          = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finsData.frameID = 0;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);

    self->scale.x          = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                = self->position.x - 0xB40 * RSDK.Sin512(self->angle & 0x7F);
    self->finsData.frameID = 2;
    RSDK.DrawSprite(&self->finsData, &drawPos, false);

    self->rotation = self->timer3;
    self->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&self->timerData, &self->posUnknown, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->bubbleData, NULL, false);
}

void SpeedGate_EditorLoad(void) { SpeedGate->aniFrames = RSDK.LoadSpriteAnimation("Global/SpeedGate.bin", SCOPE_STAGE); }
#endif

void SpeedGate_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpeedGate, VAR_ENUM, id);
    RSDK_EDITABLE_VAR(SpeedGate, VAR_ENUM, timer);
    RSDK_EDITABLE_VAR(SpeedGate, VAR_ENUM, prize);
    RSDK_EDITABLE_VAR(SpeedGate, VAR_UINT8, direction);
}
