#include "../SonicMania.h"

ObjectSpikeLog *SpikeLog;

void SpikeLog_Update(void)
{
    RSDK_THIS(SpikeLog);
    StateMachine_Run(entity->state);
}

void SpikeLog_LateUpdate(void) {}

void SpikeLog_StaticUpdate(void) { SpikeLog->timer = Zone->timer / 3 & 0x1F; }

void SpikeLog_Draw(void)
{
    RSDK_THIS(SpikeLog);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void SpikeLog_Create(void *data)
{
    RSDK_THIS(SpikeLog);
    entity->drawFX |= FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    entity->frame *= 4;
    entity->drawOrder = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(SpikeLog->spriteIndex, 0, &entity->animator, true, 0);
    entity->state = SpikeLog_State_Main;
}

void SpikeLog_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        SpikeLog->spriteIndex = RSDK.LoadSpriteAnimation("GHZ/SpikeLog.bin", SCOPE_STAGE);
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
    entity->animator.frameID = (entity->frame + SpikeLog->timer) & 0x1F;
    foreach_active(Player, player)
    {
        if (player->shield != SHIELD_FIRE || player->invincibleTimer) {
            if ((entity->animator.frameID & 0xFFFFFFFC) != 8)
                continue;
            if (Player_CheckCollisionTouch(player, entity, &SpikeLog->hitbox)) {
#if RETRO_USE_PLUS
                if (player->characterID == ID_MIGHTY) {
                    int anim = player->playerAnimator.animationID;
                    if (anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH) {
                        if (!player->invincibleTimer && player->blinkTimer <= 0) {
                            if (abs(player->velocity.x) < 0x10000 || !player->groundedStore) {
                                player->velocity.y       = -0x48000;
                                player->onGround         = 0;
                                player->jumpAbility      = 0;
                                player->jumpAbilityTimer = 0;
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLY, &player->playerAnimator, 0, 0);
                                RSDK.PlaySFX(Player->sfx_MightyUnspin, 0, 255);
                            }
                            if (!player->uncurlTimer) {
                                player->uncurlTimer = 15;
                                RSDK.PlaySFX(Player->sfx_PimPom, 0, 255);
                            }
                        }
                    }
                    else if (player->playerAnimator.animationID != ANI_FLY)
                        Player_CheckHit(player, entity);
                }
                else {
#endif
                    Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
                }
#endif
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, entity, &SpikeLog->hitbox)) {
                if (!SpikeLog->hasAchievement) {
#if RETRO_USE_PLUS
                    API.UnlockAchievement("ACH_GHZ");
#else
                    APICallback_UnlockAchievement("ACH_GHZ");
#endif
                    SpikeLog->hasAchievement = true;
                }
                RSDK.CreateEntity(BurningLog->objectID, (void *)0x10, entity->position.x, entity->position.y);
                RSDK.SetTileInfo(Zone->fgLow, entity->position.x >> 20, entity->position.y >> 20, 0xFFFF);
                entity->frame = 8;
                entity->state = SpikeLog_State_Burn;
            }
        }
    }
}

void SpikeLog_State_Burn(void)
{
    RSDK_THIS(SpikeLog);
    if (!--entity->frame) {
        foreach_all(SpikeLog, other)
        {
            if (other->state == SpikeLog_State_Main
                && RSDK.CheckObjectCollisionTouchBox(other, &SpikeLog->burnHitbox, entity, &SpikeLog->burnHitbox)) {
                RSDK.CreateEntity(BurningLog->objectID, (void *)0x10, other->position.x, other->position.y);
                RSDK.SetTileInfo(Zone->fgLow, other->position.x >> 20, other->position.y >> 20, 0xFFFF);
                other->frame  = 8;
                other->state  = SpikeLog_State_Burn;
                other->active = ACTIVE_NORMAL;
            }
        }
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
}

void SpikeLog_EditorDraw(void) {}

void SpikeLog_EditorLoad(void) {}

void SpikeLog_Serialize(void) { RSDK_EDITABLE_VAR(SpikeLog, VAR_UINT8, frame); }
