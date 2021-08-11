#include "SonicMania.h"

ObjectIceSpring *IceSpring;

void IceSpring_Update(void)
{
    //this was unironically fun to decompile
    //so many label jumps
    RSDK_THIS(IceSpring);
    if (entity->state) {
        if (!entity->animator.animationSpeed) {
            int i = 0;
            foreach_active(Player, player)
            {
                if (entity->planeFilter && player->collisionPlane != ((entity->planeFilter - 1) & 1))
                    continue;

                int collided = 0;
                if (player->state == Ice_State_FrozenPlayer)
                    collided = RSDK.CheckObjectCollisionBox(entity, &entity->hitbox, player, &Ice->hitbox2, 0);
                else
                    collided = RSDK.CheckObjectCollisionBox(entity, &entity->hitbox, player, Player_GetHitbox(player), 0);
                if (!collided) {
                    ++i;
                    continue;
                }

                int type = entity->type;
                bool32 collideCheck;
                if (!type) {
                    if ((entity->direction & 2) != 0)
                        collideCheck = collided == 4;
                    else
                        collideCheck = collided == 1;
                }
                if (!--type) {
                    if ((entity->direction & 1) != 0)
                        collideCheck = collided == 2;
                    else
                        collideCheck = collided == 3;
                }

                if (!collideCheck && type == 1) {
                    collideCheck |= (player->onGround || player->velocity.y >= 0 || abs(player->velocity.x) > -player->velocity.y);
                }
                if (collideCheck) {
                    entity->playerBits |= 1 << i;
                    IceSpring->animators[i] = player->playerAnimator;
                }

                EntityShield *shield      = RSDK.GetEntityByID(Player->playerCount + RSDK.GetEntityID(player));
                StateMachine(playerState) = player->state;
                if (playerState != Ice_State_FrozenPlayer && shield->animator.animationID != 2 && playerState != Player_State_MightyHammerDrop) {
                    if (player->shield == 3 && player->invincibleTimer <= 0 && !entity->timer) {
                        entity->timer = 15;
                        Ice_ShatterGenerator(8, 8, 6, 0, 0, 0);
                    }
                    ++i;
                    continue;
                }
                if (!collideCheck || playerState == Player_State_MightyHammerDrop) {
                    switch (collided) {
                        case 1: IceSpring_Shatter(0, 0); foreach_break;
                        case 2:
                        case 3: IceSpring_Shatter(player->velocity.x, 0); foreach_break;
                        case 4: IceSpring_Shatter(0, player->velocity.y); foreach_break;
                        // this is NOT my own bug, this is in mania
                        default: IceSpring_Shatter(player->position.x, player->position.y); foreach_break;
                    }
                    return;
                }
            }
        }
        entity->state(); // skipping using StateMachine_Run cause we already check
        for (int i = 0; i < Player->playerCount; ++i) {
            if (((1 << i) & entity->playerBits) != 0) {
                EntityPlayer *player   = (EntityPlayer *)RSDK.GetEntityByID(i);
                IceSpring              = IceSpring;
                player->playerAnimator = IceSpring->animators[i];
                player->jumpAbility    = 0;
                if (player->sidekick && entity->playerBits == 0b10) {
                    RSDK.SetSpriteAnimation(IceSpring->animID, entity->type, &entity->animator, true, 0);
                    entity->animator.animationSpeed = 0;
                    entity->playerBits              = 0;
                }
            }
        }
        RSDK.ProcessAnimation(&entity->animator);
        if (entity->animator.frameID == 7)
            IceSpring_Shatter(0, 0);
    }
    if (entity->timer > 0) {
        if (entity->animator.frameID <= 0) {
            --entity->timer;
            if (entity->timer <= 0)
                IceSpring_Shatter(0, 0);
        }
        else
            entity->timer = 0;
    }
}

void IceSpring_LateUpdate(void) {}

void IceSpring_StaticUpdate(void) {}

void IceSpring_Draw(void)
{
    RSDK_THIS(IceSpring);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void IceSpring_Create(void *data)
{
    RSDK_THIS(IceSpring);
    entity->drawFX = FX_FLIP;
    if (RSDK_sceneInfo->inEditor)
        return;

    entity->type %= 3;
    if (data) {
        entity->type     = voidToInt(data);
        entity->flipFlag = (byte)voidToInt(data);
    }
    RSDK.SetSpriteAnimation(IceSpring->animID, entity->type, &entity->animator, true, 0);
    entity->active                  = 4;
    entity->animator.animationSpeed = 0;
    entity->updateRange.x           = 0x600000;
    entity->updateRange.y           = 0x600000;
    entity->visible                 = 1;
    if (entity->planeFilter && ((entity->planeFilter - 1) & 2))
        entity->drawOrder = Zone->drawOrderHigh;
    else
        entity->drawOrder = Zone->drawOrderLow;
    int type = entity->type;
    if (type) {
        if (--type) {
            if (type == 1) {
                entity->direction  = entity->flipFlag;
                entity->velocity.x = 0x54000 * ((entity->flipFlag & 1) ? -1 : 1);
                entity->velocity.y = 0x54000 * ((entity->flipFlag < 2u) ? -1 : 1);

                entity->hitbox.left   = -12;
                entity->hitbox.top    = -12;
                entity->hitbox.right  = 12;
                entity->hitbox.bottom = 12;

                entity->state = Spring_State_Diagonal;
            }
        }
        else {
            entity->velocity.x = 0x80000 * (entity->flipFlag ? -1 : 1);
            entity->direction  = entity->flipFlag;

            entity->hitbox.left   = -8;
            entity->hitbox.top    = -16;
            entity->hitbox.right  = 8;
            entity->hitbox.bottom = 16;

            entity->state = Spring_State_Horizontal;
        }
    }
    else {
        entity->velocity.y = 0x80000 * (entity->flipFlag ? 1 : -1);
        entity->direction  = entity->flipFlag;

        entity->hitbox.left   = -16;
        entity->hitbox.top    = -8;
        entity->hitbox.right  = 16;
        entity->hitbox.bottom = 8;

        entity->state = Spring_State_Vertical;
    }
}

void IceSpring_StageLoad(void)
{
    IceSpring->animID     = RSDK.LoadSpriteAnimation("PSZ2/IceSpring.bin", SCOPE_STAGE);
    IceSpring->bounceSFX  = RSDK.GetSFX("Global/Spring.wav");
    IceSpring->shatterSFX = RSDK.GetSFX("Stage/WindowShatter.wav");
}

void IceSpring_EditorLoad(void) { IceSpring_StageLoad(); }

void IceSpring_EditorDraw(void)
{
    RSDK_THIS(IceSpring);
    RSDK.SetSpriteAnimation(IceSpring->animID, entity->type, &entity->animator, true, 0);
    IceSpring_Draw();
}

void IceSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(IceSpring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(IceSpring, VAR_ENUM, flipFlag);
    RSDK_EDITABLE_VAR(IceSpring, VAR_BOOL, onGround);
    RSDK_EDITABLE_VAR(IceSpring, VAR_UINT8, planeFilter);
}

void IceSpring_Shatter(int velX, int velY)
{
    RSDK_THIS(IceSpring);
    RSDK.PlaySFX(IceSpring->shatterSFX, 0, 255);
    Ice_ShatterGenerator(8, 8, 16, velX, velY, 0);
    if (entity->type < 2)
        destroyEntity(entity);
    else entity->animator.frameID = 7;
}