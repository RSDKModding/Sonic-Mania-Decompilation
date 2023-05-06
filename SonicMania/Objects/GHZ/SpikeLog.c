// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpikeLog Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
    if (!SceneInfo->inEditor)
        self->frame *= 4;
    self->drawGroup = Zone->objectDrawGroup[0];
    RSDK.SetSpriteAnimation(SpikeLog->aniFrames, 0, &self->animator, true, 0);
    self->state = SpikeLog_State_Main;
}

void SpikeLog_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        SpikeLog->aniFrames = RSDK.LoadSpriteAnimation("GHZ/SpikeLog.bin", SCOPE_STAGE);

    SpikeLog->hitboxSpikeLog.left   = -8;
    SpikeLog->hitboxSpikeLog.top    = -16;
    SpikeLog->hitboxSpikeLog.right  = 8;
    SpikeLog->hitboxSpikeLog.bottom = 0;

    SpikeLog->hitboxBurnLog.left   = -9;
    SpikeLog->hitboxBurnLog.top    = -16;
    SpikeLog->hitboxBurnLog.right  = 9;
    SpikeLog->hitboxBurnLog.bottom = 0;
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

            if (Player_CheckCollisionTouch(player, self, &SpikeLog->hitboxSpikeLog)) {
#if MANIA_USE_PLUS
                if (player->characterID == ID_MIGHTY) {
                    int32 anim = player->animator.animationID;
                    if (anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_HAMMERDROP) {
                        if (!player->invincibleTimer && player->blinkTimer <= 0) {
                            if (abs(player->velocity.x) < 0x10000 || !player->groundedStore) {
                                player->velocity.y       = -0x48000;
                                player->onGround         = false;
                                player->applyJumpCap     = false;
                                player->jumpAbilityState = 0;
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_UNSPIN, &player->animator, false, 0);
                                RSDK.PlaySfx(Player->sfxMightyUnspin, false, 255);
                            }

                            if (!player->uncurlTimer) {
                                player->uncurlTimer = 15;
                                RSDK.PlaySfx(Player->sfxPimPom, false, 255);
                            }
                        }
                    }
                    else if (player->animator.animationID != ANI_UNSPIN)
                        Player_Hurt(player, self);
                }
                else {
#endif
                    Player_Hurt(player, self);
#if MANIA_USE_PLUS
                }
#endif
            }
        }
        else if (Player_CheckCollisionTouch(player, self, &SpikeLog->hitboxSpikeLog)) {
            if (!SpikeLog->hasAchievement) {
                API_UnlockAchievement(&achievementList[ACH_GHZ]);

                SpikeLog->hasAchievement = true;
            }
            CREATE_ENTITY(BurningLog, INT_TO_VOID(16), self->position.x, self->position.y);
            RSDK.SetTile(Zone->fgLayer[0], self->position.x >> 20, self->position.y >> 20, -1);
            self->frame = 8;
            self->state = SpikeLog_State_Burn;
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
                && RSDK.CheckObjectCollisionTouchBox(other, &SpikeLog->hitboxBurnLog, self, &SpikeLog->hitboxBurnLog)) {
                CREATE_ENTITY(BurningLog, INT_TO_VOID(16), other->position.x, other->position.y);
                RSDK.SetTile(Zone->fgLayer[0], other->position.x >> 20, other->position.y >> 20, -1);
                other->frame  = 8;
                other->state  = SpikeLog_State_Burn;
                other->active = ACTIVE_NORMAL;
            }
        }
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void SpikeLog_EditorDraw(void)
{
    RSDK_THIS(SpikeLog);
    self->animator.frameID = self->frame * 4;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void SpikeLog_EditorLoad(void) { SpikeLog->aniFrames = RSDK.LoadSpriteAnimation("GHZ/SpikeLog.bin", SCOPE_STAGE); }
#endif

void SpikeLog_Serialize(void) { RSDK_EDITABLE_VAR(SpikeLog, VAR_UINT8, frame); }
