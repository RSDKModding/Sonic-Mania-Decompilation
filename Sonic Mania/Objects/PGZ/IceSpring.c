#include "SonicMania.h"

ObjectIceSpring *IceSpring;

void IceSpring_Update(void)
{
    //this was unironically fun to decompile
    //so many label jumps
    RSDK_THIS(IceSpring);
    entity->playerBits = 0;
    if (entity->state) {
        if (!entity->animator.animationSpeed) {
            int32 i = 0;
            foreach_active(Player, player)
            {
                if (entity->planeFilter && player->collisionPlane != ((entity->planeFilter - 1) & 1))
                    continue;

                int32 storeX = player->position.x;
                int32 storeY = player->position.y;

                int32 collided = 0;
                if (player->state == Ice_State_FrozenPlayer)
                    collided = RSDK.CheckObjectCollisionBox(entity, &entity->hitbox, player, &Ice->hitbox2, 0);
                else
                    collided = RSDK.CheckObjectCollisionBox(entity, &entity->hitbox, player, Player_GetHitbox(player), 0);
                if (!collided) {
                    ++i;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    continue;
                }

                int32 type = entity->type;
                bool32 collideCheck = false;
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
                    memcpy(&IceSpring->animators[i], &player->playerAnimator, sizeof(Animator));
                }

                EntityShield *shield      = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
                if (player->state != Ice_State_FrozenPlayer && shield->animator.animationID != 2
#if RETRO_USE_PLUS
                    && player->state != Player_State_MightyHammerDrop
#endif
                    ) {
                    if (player->shield == 3 && player->invincibleTimer <= 0 && !entity->timer) {
                        entity->timer = 15;
                        Ice_ShatterGenerator(8, 8, 6, 0, 0, 0);
                    }
                    ++i;
                }
                else if (!collideCheck
#if RETRO_USE_PLUS
                    || player->state == Player_State_MightyHammerDrop
#endif
                    ) {
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
                player->position.x = storeX;
                player->position.y = storeY;
            }
        }
        StateMachine_Run(entity->state);
        for (int32 i = 0; i < Player->playerCount; ++i) {
            if ((1 << i) & entity->playerBits) {
                EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
                if (IceSpring->animators[i].animationID == ANI_JUMP) {
                    memcpy(&player->playerAnimator, &IceSpring->animators[i], sizeof(Animator));
                }
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
    if (!RSDK_sceneInfo->inEditor) {
        entity->type %= 3;
        if (data) {
            entity->type     = voidToInt(data);
            entity->flipFlag = voidToInt(data);
        }
        RSDK.SetSpriteAnimation(IceSpring->animID, entity->type, &entity->animator, true, 0);
        entity->active                  = 4;
        entity->animator.animationSpeed = 0;
        entity->updateRange.x           = 0x600000;
        entity->updateRange.y           = 0x600000;
        entity->visible                 = true;
        if (entity->planeFilter && ((entity->planeFilter - 1) & 2))
            entity->drawOrder = Zone->drawOrderHigh;
        else
            entity->drawOrder = Zone->drawOrderLow;
        
        switch (entity->type) {
            case 0:
                entity->velocity.y = 0x80000 * (entity->flipFlag ? 1 : -1);
                entity->direction  = entity->flipFlag;

                entity->hitbox.left   = -16;
                entity->hitbox.top    = -8;
                entity->hitbox.right  = 16;
                entity->hitbox.bottom = 8;

                entity->state = Spring_State_Vertical;
                break;
            case 1:
                entity->velocity.x = 0x80000 * (entity->flipFlag ? -1 : 1);
                entity->direction  = entity->flipFlag;

                entity->hitbox.left   = -8;
                entity->hitbox.top    = -16;
                entity->hitbox.right  = 8;
                entity->hitbox.bottom = 16;

                entity->state = Spring_State_Horizontal;
                break;
            case 2:
                entity->direction  = entity->flipFlag;
                entity->velocity.x = 0x54000 * ((entity->flipFlag & 1) ? -1 : 1);
                entity->velocity.y = 0x54000 * ((entity->flipFlag < 2) ? -1 : 1);

                entity->hitbox.left   = -12;
                entity->hitbox.top    = -12;
                entity->hitbox.right  = 12;
                entity->hitbox.bottom = 12;

                entity->state = Spring_State_Diagonal;
                break;
        }
    }
}

void IceSpring_StageLoad(void)
{
    IceSpring->animID     = RSDK.LoadSpriteAnimation("PSZ2/IceSpring.bin", SCOPE_STAGE);
    IceSpring->bounceSFX  = RSDK.GetSFX("Global/Spring.wav");
    IceSpring->shatterSFX = RSDK.GetSFX("Stage/WindowShatter.wav");
}

void IceSpring_Shatter(int32 velX, int32 velY)
{
    RSDK_THIS(IceSpring);
    RSDK.PlaySfx(IceSpring->shatterSFX, 0, 255);
    Ice_ShatterGenerator(8, 8, 16, velX, velY, 0);
    if (entity->type < 2)
        destroyEntity(entity);
    else entity->animator.frameID = 7;
}

#if RETRO_INCLUDE_EDITOR
void IceSpring_EditorLoad(void) { IceSpring_StageLoad(); }

void IceSpring_EditorDraw(void)
{
    RSDK_THIS(IceSpring);
    RSDK.SetSpriteAnimation(IceSpring->animID, entity->type, &entity->animator, true, 0);
    IceSpring_Draw();

    RSDK_ACTIVE_VAR(IceSpring, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
}
#endif

void IceSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(IceSpring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(IceSpring, VAR_ENUM, flipFlag);
    RSDK_EDITABLE_VAR(IceSpring, VAR_BOOL, onGround);
    RSDK_EDITABLE_VAR(IceSpring, VAR_UINT8, planeFilter);
}
