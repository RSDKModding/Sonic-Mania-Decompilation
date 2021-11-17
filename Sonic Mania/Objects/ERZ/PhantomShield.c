#include "SonicMania.h"

ObjectPhantomShield *PhantomShield;

void PhantomShield_Update(void)
{
    RSDK_THIS(PhantomShield);

    if (entity->parent) {
        entity->position.x = entity->parent->position.x;
        entity->position.y = entity->parent->position.y;
    }

    if (entity->blendAmount > 0)
        entity->blendAmount -= 0x20;

    StateMachine_Run(entity->state);
}

void PhantomShield_LateUpdate(void) {}

void PhantomShield_StaticUpdate(void) {}

void PhantomShield_Draw(void)
{
    RSDK_THIS(PhantomShield);

    if (entity->blendAmount > 0)
        RSDK.SetLimitedFade(0, 1, 3, entity->blendAmount, 136, 143);
    entity->direction = RSDK.GetFrameID(&entity->animator) != 98 ? 0 : 2;
    RSDK.DrawSprite(&entity->animator, 0, false);
    if (entity->blendAmount > 0)
        RSDK.CopyPalette(1, 136, 0, 136, 8);
}

void PhantomShield_Create(void *data)
{
    RSDK_THIS(PhantomShield);

    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->parent        = (Entity *)data;
        entity->active        = ACTIVE_NORMAL;
        entity->drawFX        = FX_FLIP;
        entity->inkEffect     = INK_ADD;
        entity->alpha         = 0x100;
        entity->scale.x       = 0x200;
        entity->scale.y       = 0x200;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->state         = PhantomShield_Unknown1;
        RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 0, &entity->animator, true, 0);
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

void PhantomShield_Unknown1(void)
{
    RSDK_THIS(PhantomShield);

    RSDK.ProcessAnimation(&entity->animator);
    if (!entity->shieldActive) {
        entity->shieldActive = true;
        RSDK.PlaySfx(PhantomEgg->sfxShield, false, 255);
    }

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 1, &entity->animator, true, 0);
        entity->shieldActive = false;
        entity->state        = PhantomShield_Unknown2;
    }
}

void PhantomShield_Unknown2(void)
{
    RSDK_THIS(PhantomShield);

    RSDK.ProcessAnimation(&entity->animator);

    foreach_active(Player, player)
    {
        if (entity->playerTimer[player->playerID])
            entity->playerTimer[player->playerID]--;

        if (Player_CheckCollisionTouch(player, entity, &PhantomShield->hitbox)) {
            if (Player_CheckAttacking(player, entity)) {
                entity->blendAmount = 256;
                if (!entity->playerTimer[player->playerID]) {
                    RSDK.PlaySfx(PhantomEgg->sfxRepel, false, 255);
                    entity->playerTimer[player->playerID] = 16;
                }

                int angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                int velX  = 0x500 * RSDK.Cos256(angle);
                int velY  = 0x500 * RSDK.Sin256(angle);

                if (player->state == Player_State_FlyCarried)
                    RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

                int anim = player->playerAnimator.animationID;
                if (anim != ANI_FLY && anim != ANI_FLYLIFTTIRED) {
                    if (player->state != Player_State_TailsFlight) {
                        if (player->state != Player_State_DropDash)
                            player->state = Player_State_Air;
                        if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                            player->playerAnimator.animationID = ANI_WALK;
                    }
                }
                if (player->characterID == ID_KNUCKLES && player->playerAnimator.animationID == ANI_FLY) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FLYTIRED, &player->playerAnimator, false, 0);
                    player->state = Player_State_KnuxGlideDrop;
                }
                player->velocity.x     = velX;
                player->groundVel      = velX;
                player->velocity.y     = velY;
                player->jumpAbility    = 0;
                player->onGround       = false;
                player->tileCollisions = true;
            }
            else {
                Player_CheckHit(player, entity);
            }
        }
    }
}

void PhantomShield_Unknown3(void)
{
    RSDK_THIS(PhantomShield);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void PhantomShield_EditorDraw(void) {}

void PhantomShield_EditorLoad(void) {}
#endif

void PhantomShield_Serialize(void) {}
