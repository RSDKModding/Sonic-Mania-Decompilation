#include "SonicMania.h"

ObjectSpikeLog *SpikeLog;

void SpikeLog_Update(void)
{
    RSDK_THIS(SpikeLog);
    StateMachine_Run(self->state);
}

void SpikeLog_LateUpdate(void) {}

void SpikeLog_StaticUpdate(void) { SpikeLog->timer = Zone->timer / 3 & 0x1F; }

void SpikeLog_Draw(void)
{
    RSDK_THIS(SpikeLog);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void SpikeLog_Create(void *data)
{
    RSDK_THIS(SpikeLog);
    self->drawFX |= FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    self->frame *= 4;
    self->drawOrder = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(SpikeLog->aniFrames, 0, &self->animator, true, 0);
    self->state = SpikeLog_State_Main;
}

void SpikeLog_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        SpikeLog->aniFrames = RSDK.LoadSpriteAnimation("GHZ/SpikeLog.bin", SCOPE_STAGE);
    SpikeLog->hitbox.left       = -8;
    SpikeLog->hitbox.top        = -16;
    SpikeLog->hitbox.right      = 8;
    SpikeLog->hitbox.bottom     = 0;
    SpikeLog->burnHitbox.left   = -9;
    SpikeLog->burnHitbox.top    = -16;
    SpikeLog->burnHitbox.right  = 9;
    SpikeLog->burnHitbox.bottom = 0;
}

void SpikeLog_State_Main(void)
{
    RSDK_THIS(SpikeLog);
    self->animator.frameID = (self->frame + SpikeLog->timer) & 0x1F;
    foreach_active(Player, player)
    {
        if (player->shield != SHIELD_FIRE || player->invincibleTimer) {
            if ((self->animator.frameID & 0xFFFFFFFC) != 8)
                continue;
            if (Player_CheckCollisionTouch(player, self, &SpikeLog->hitbox)) {
#if RETRO_USE_PLUS
                if (player->characterID == ID_MIGHTY) {
                    int32 anim = player->playerAnimator.animationID;
                    if (anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH) {
                        if (!player->invincibleTimer && player->blinkTimer <= 0) {
                            if (abs(player->velocity.x) < 0x10000 || !player->groundedStore) {
                                player->velocity.y       = -0x48000;
                                player->onGround         = 0;
                                player->jumpAbility      = 0;
                                player->jumpAbilityTimer = 0;
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FLY, &player->playerAnimator, 0, 0);
                                RSDK.PlaySfx(Player->sfxMightyUnspin, 0, 255);
                            }
                            if (!player->uncurlTimer) {
                                player->uncurlTimer = 15;
                                RSDK.PlaySfx(Player->sfxPimPom, 0, 255);
                            }
                        }
                    }
                    else if (player->playerAnimator.animationID != ANI_FLY)
                        Player_CheckHit(player, self);
                }
                else {
#endif
                    Player_CheckHit(player, self);
#if RETRO_USE_PLUS
                }
#endif
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &SpikeLog->hitbox)) {
                if (!SpikeLog->hasAchievement) {
                    API_UnlockAchievement("ACH_GHZ");
                    SpikeLog->hasAchievement = true;
                }
                CREATE_ENTITY(BurningLog, intToVoid(0x10), self->position.x, self->position.y);
                RSDK.SetTileInfo(Zone->fgLow, self->position.x >> 20, self->position.y >> 20, 0xFFFF);
                self->frame = 8;
                self->state = SpikeLog_State_Burn;
            }
        }
    }
}

void SpikeLog_State_Burn(void)
{
    RSDK_THIS(SpikeLog);
    if (!--self->frame) {
        foreach_all(SpikeLog, other)
        {
            if (other->state == SpikeLog_State_Main
                && RSDK.CheckObjectCollisionTouchBox(other, &SpikeLog->burnHitbox, self, &SpikeLog->burnHitbox)) {
                CREATE_ENTITY(BurningLog, intToVoid(0x10), other->position.x, other->position.y);
                RSDK.SetTileInfo(Zone->fgLow, other->position.x >> 20, other->position.y >> 20, 0xFFFF);
                other->frame  = 8;
                other->state  = SpikeLog_State_Burn;
                other->active = ACTIVE_NORMAL;
            }
        }
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void SpikeLog_EditorDraw(void)
{
    RSDK_THIS(SpikeLog);
    self->animator.frameID = self->frame;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void SpikeLog_EditorLoad(void) { SpikeLog->aniFrames = RSDK.LoadSpriteAnimation("GHZ/SpikeLog.bin", SCOPE_STAGE); }
#endif

void SpikeLog_Serialize(void) { RSDK_EDITABLE_VAR(SpikeLog, VAR_UINT8, frame); }
