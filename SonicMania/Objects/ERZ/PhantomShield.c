// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PhantomShield Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPhantomShield *PhantomShield;

void PhantomShield_Update(void)
{
    RSDK_THIS(PhantomShield);

    if (self->parent) {
        self->position.x = self->parent->position.x;
        self->position.y = self->parent->position.y;
    }

    if (self->blendAmount > 0)
        self->blendAmount -= 0x20;

    StateMachine_Run(self->state);
}

void PhantomShield_LateUpdate(void) {}

void PhantomShield_StaticUpdate(void) {}

void PhantomShield_Draw(void)
{
    RSDK_THIS(PhantomShield);

    if (self->blendAmount > 0)
        RSDK.SetLimitedFade(0, 1, 3, self->blendAmount, 136, 143);

    self->direction = RSDK.GetFrameID(&self->animator) == 'b' ? FLIP_Y : FLIP_NONE;
    RSDK.DrawSprite(&self->animator, NULL, false);

    if (self->blendAmount > 0)
        RSDK.CopyPalette(1, 136, 0, 136, 8);
}

void PhantomShield_Create(void *data)
{
    RSDK_THIS(PhantomShield);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->parent        = (Entity *)data;
        self->active        = ACTIVE_NORMAL;
        self->drawFX        = FX_FLIP;
        self->inkEffect     = INK_ADD;
        self->alpha         = 0x100;
        self->scale.x       = 0x200;
        self->scale.y       = 0x200;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->state         = PhantomShield_State_Appear;
        RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 0, &self->animator, true, 0);
    }
}

void PhantomShield_StageLoad(void)
{
    PhantomShield->aniFrames = RSDK.LoadSpriteAnimation("Phantom/EggShield.bin", SCOPE_STAGE);

    PhantomShield->hitbox.left   = -32;
    PhantomShield->hitbox.top    = -32;
    PhantomShield->hitbox.right  = 32;
    PhantomShield->hitbox.bottom = 32;
}

void PhantomShield_State_Appear(void)
{
    RSDK_THIS(PhantomShield);

    RSDK.ProcessAnimation(&self->animator);
    if (!self->shieldActive) {
        self->shieldActive = true;
        RSDK.PlaySfx(PhantomEgg->sfxShield, false, 255);
    }

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 1, &self->animator, true, 0);
        self->shieldActive = false;
        self->state        = PhantomShield_State_Active;
    }
}

void PhantomShield_State_Active(void)
{
    RSDK_THIS(PhantomShield);

    RSDK.ProcessAnimation(&self->animator);

    foreach_active(Player, player)
    {
        if (self->playerTimer[player->playerID])
            self->playerTimer[player->playerID]--;

        if (Player_CheckCollisionTouch(player, self, &PhantomShield->hitbox)) {
            if (Player_CheckAttacking(player, self)) {
                self->blendAmount = 256;
                if (!self->playerTimer[player->playerID]) {
                    RSDK.PlaySfx(PhantomEgg->sfxRepel, false, 255);
                    self->playerTimer[player->playerID] = 16;
                }

                int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
                int32 velX  = 0x500 * RSDK.Cos256(angle);
                int32 velY  = 0x500 * RSDK.Sin256(angle);

                if (player->state == Player_State_FlyCarried)
                    RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

                int32 anim = player->animator.animationID;
                if (anim != ANI_FLY && anim != ANI_FLY_LIFT_TIRED) {
                    if (player->state != Player_State_TailsFlight) {
                        if (player->state != Player_State_DropDash)
                            player->state = Player_State_Air;
                        if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                            player->animator.animationID = ANI_WALK;
                    }
                }

                if (player->characterID == ID_KNUCKLES && player->animator.animationID == ANI_GLIDE) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_GLIDE_DROP, &player->animator, false, 0);
                    player->state = Player_State_KnuxGlideDrop;
                }

                player->groundVel      = velX;
                player->velocity.x     = velX;
                player->velocity.y     = velY;
                player->applyJumpCap   = false;
                player->onGround       = false;
                player->tileCollisions = TILECOLLISION_DOWN;
            }
            else {
                Player_Hurt(player, self);
            }
        }
    }
}

void PhantomShield_State_Disappear(void)
{
    RSDK_THIS(PhantomShield);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void PhantomShield_EditorDraw(void)
{
    RSDK_THIS(PhantomShield);
    RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 1, &self->animator, true, 0);

    self->direction = RSDK.GetFrameID(&self->animator) == 'b' ? FLIP_Y : FLIP_NONE;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PhantomShield_EditorLoad(void) { PhantomShield->aniFrames = RSDK.LoadSpriteAnimation("Phantom/EggShield.bin", SCOPE_STAGE); }
#endif

void PhantomShield_Serialize(void) {}
