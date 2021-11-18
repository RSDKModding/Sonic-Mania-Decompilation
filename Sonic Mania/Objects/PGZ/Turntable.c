#include "SonicMania.h"

ObjectTurntable *Turntable;

void Turntable_Update(void)
{
    RSDK_THIS(Turntable);
    self->angle += self->angleVel;
    if (self->angle < 0)
        self->angle += 0x400;
    self->angle %= 0x3FF;
    foreach_active(Player, player)
    {
        int32 pID = RSDK.GetEntityID(player);
        if (!((1 << pID) & self->activePlayers)) {
            if (Player_CheckCollisionBox(player, self, &self->hitbox) == 1) {
#if RETRO_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop || player->state == Player_State_BubbleBounce)
                    continue;
#else
                if (player->state == Player_State_BubbleBounce)
                    continue;
#endif
                self->active          = ACTIVE_NORMAL;
                self->activePlayers   = self->activePlayers | (1 << pID);
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->groundVel       = 0;
                player->onGround        = 1;
                player->state           = Player_State_None;

                int32 dist = abs(player->position.x - self->position.x) >> 0x10;

                if (dist >= 0x10) {
                    if (player->position.x <= self->position.x) {
                        self->field_78[pID] = 512;
                        self->field_98[pID] = 12;
                    }
                    else {
                        self->field_78[pID] = 0;
                        self->field_98[pID] = 0;
                    }
                }
                else {
                    int32 distY = 16 * (0x10000 - dist / 16);
                    int32 angY  = -16 * (0x10000 - dist / 16);
                    if (player->drawOrder != Zone->playerDrawLow)
                        angY = distY;
                    int32 distX = player->position.x - self->position.x;
                    int32 angX  = -distX;
                    if (player->position.x >= self->position.x)
                        angX = distX;
                    dist                  = 0x10;
                    self->field_78[pID] = 4 * RSDK.ATan2(angX, angY);
                    self->field_98[pID] = 0;
                }
                self->field_88[pID] = dist;
                RSDK.SetSpriteAnimation(player->aniFrames, 34, &player->playerAnimator, true, self->field_98[pID]);
                Hitbox *hitbox     = Player_GetHitbox(player);
                player->position.y = self->position.y - (hitbox->bottom << 16) - (self->size.y >> 1);
            }
        }
        if ((1 << pID) & self->activePlayers) {
            player->velocity.x    = 0;
            player->velocity.y    = 0;
            player->groundVel     = 0;
            self->field_78[pID] = (self->angleVel + self->field_78[pID]) & 0x3FF;
            player->position.x    = self->position.x;
            player->position.x += (RSDK.Cos1024(self->field_78[pID]) << 6) * self->field_88[pID];

            int32 frame = 0;
            if (player->direction)
                frame = 24 - self->field_78[pID] / 42;
            else
                frame = self->field_78[pID] / 42 % 24;

            if (self->field_78[pID] < 512)
                player->drawOrder = Zone->playerDrawHigh;
            else
                player->drawOrder = Zone->playerDrawLow;
            player->playerAnimator.frameID = (self->field_98[pID] + frame) % -24;
            if (player->jumpPress) {
                Player_StartJump(player);
            }
            else if (player->playerAnimator.animationID == ANI_TWISTER && player->state == Player_State_None) {
                continue;
            }
            self->activePlayers &= ~(1 << pID);
        }
    }
}

void Turntable_LateUpdate(void) {}

void Turntable_StaticUpdate(void) {}

void Turntable_Draw(void)
{
    RSDK_THIS(Turntable);
    RSDK.SetSpriteAnimation(Turntable->aniFrames, self->type, &self->animator, true, 13 - (self->angle >> 1) / 9 % 14);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Turntable_Create(void *data)
{
    RSDK_THIS(Turntable);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->origin.x      = self->position.x;
    self->origin.y      = self->position.y;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (!self->angleVel)
        self->angleVel = 6;
    Turntable_SetupSize();
    self->hitbox.right  = (self->size.x >> 17);
    self->hitbox.left   = -(self->size.x >> 17);
    self->hitbox.bottom = (self->size.y >> 17);
    self->hitbox.top    = -(self->size.y >> 17);
    RSDK.SetSpriteAnimation(Turntable->aniFrames, 0, &self->animator, true, 6);
}

void Turntable_StageLoad(void) { Turntable->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Turntable.bin", SCOPE_STAGE); }

void Turntable_SetupSize(void)
{
    RSDK_THIS(Turntable);
    RSDK.SetSpriteAnimation(Turntable->aniFrames, self->type, &self->animator, true, 0);

    switch (self->type) {
        case 0:
            self->size.x = 0x6A0000;
            self->size.y = 0x200000;
            break;
        case 1:
            self->size.x = 0xD40000;
            self->size.y = 0x300000;
            break;
    }
}

#if RETRO_INCLUDE_EDITOR
void Turntable_EditorDraw(void) {}

void Turntable_EditorLoad(void) {}
#endif

void Turntable_Serialize(void)
{
    RSDK_EDITABLE_VAR(Turntable, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Turntable, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Turntable, VAR_INT32, angleVel);
}
