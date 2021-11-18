#include "SonicMania.h"

ObjectIceSpring *IceSpring;

void IceSpring_Update(void)
{
    //this was unironically fun to decompile
    //so many label jumps
    RSDK_THIS(IceSpring);
    self->playerBits = 0;
    if (self->state) {
        if (!self->animator.animationSpeed) {
            int32 i = 0;
            foreach_active(Player, player)
            {
                if (self->planeFilter && player->collisionPlane != ((self->planeFilter - 1) & 1))
                    continue;

                int32 storeX = player->position.x;
                int32 storeY = player->position.y;

                int32 collided = 0;
                if (player->state == Ice_State_FrozenPlayer)
                    collided = RSDK.CheckObjectCollisionBox(self, &self->hitbox, player, &Ice->hitbox2, 0);
                else
                    collided = RSDK.CheckObjectCollisionBox(self, &self->hitbox, player, Player_GetHitbox(player), 0);
                if (!collided) {
                    ++i;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    continue;
                }

                int32 type = self->type;
                bool32 collideCheck = false;
                if (!type) {
                    if ((self->direction & 2) != 0)
                        collideCheck = collided == 4;
                    else
                        collideCheck = collided == 1;
                }
                if (!--type) {
                    if ((self->direction & 1) != 0)
                        collideCheck = collided == 2;
                    else
                        collideCheck = collided == 3;
                }

                if (!collideCheck && type == 1) {
                    collideCheck |= (player->onGround || player->velocity.y >= 0 || abs(player->velocity.x) > -player->velocity.y);
                }
                if (collideCheck) {
                    self->playerBits |= 1 << i;
                    memcpy(&IceSpring->animators[i], &player->playerAnimator, sizeof(Animator));
                }

                EntityShield *shield      = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
                if (player->state != Ice_State_FrozenPlayer && shield->animator.animationID != 2
#if RETRO_USE_PLUS
                    && player->state != Player_State_MightyHammerDrop
#endif
                    ) {
                    if (player->shield == 3 && player->invincibleTimer <= 0 && !self->timer) {
                        self->timer = 15;
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
        StateMachine_Run(self->state);
        for (int32 i = 0; i < Player->playerCount; ++i) {
            if ((1 << i) & self->playerBits) {
                EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
                if (IceSpring->animators[i].animationID == ANI_JUMP) {
                    memcpy(&player->playerAnimator, &IceSpring->animators[i], sizeof(Animator));
                }
                player->jumpAbility    = 0;
                if (player->sidekick && self->playerBits == 0b10) {
                    RSDK.SetSpriteAnimation(IceSpring->animID, self->type, &self->animator, true, 0);
                    self->animator.animationSpeed = 0;
                    self->playerBits              = 0;
                }
            }
        }
        RSDK.ProcessAnimation(&self->animator);
        if (self->animator.frameID == 7)
            IceSpring_Shatter(0, 0);
    }
    if (self->timer > 0) {
        if (self->animator.frameID <= 0) {
            --self->timer;
            if (self->timer <= 0)
                IceSpring_Shatter(0, 0);
        }
        else
            self->timer = 0;
    }
}

void IceSpring_LateUpdate(void) {}

void IceSpring_StaticUpdate(void) {}

void IceSpring_Draw(void)
{
    RSDK_THIS(IceSpring);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void IceSpring_Create(void *data)
{
    RSDK_THIS(IceSpring);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->type %= 3;
        if (data) {
            self->type     = voidToInt(data);
            self->flipFlag = voidToInt(data);
        }
        RSDK.SetSpriteAnimation(IceSpring->animID, self->type, &self->animator, true, 0);
        self->active                  = 4;
        self->animator.animationSpeed = 0;
        self->updateRange.x           = 0x600000;
        self->updateRange.y           = 0x600000;
        self->visible                 = true;
        if (self->planeFilter && ((self->planeFilter - 1) & 2))
            self->drawOrder = Zone->drawOrderHigh;
        else
            self->drawOrder = Zone->drawOrderLow;
        
        switch (self->type) {
            case 0:
                self->velocity.y = 0x80000 * (self->flipFlag ? 1 : -1);
                self->direction  = self->flipFlag;

                self->hitbox.left   = -16;
                self->hitbox.top    = -8;
                self->hitbox.right  = 16;
                self->hitbox.bottom = 8;

                self->state = Spring_State_Vertical;
                break;
            case 1:
                self->velocity.x = 0x80000 * (self->flipFlag ? -1 : 1);
                self->direction  = self->flipFlag;

                self->hitbox.left   = -8;
                self->hitbox.top    = -16;
                self->hitbox.right  = 8;
                self->hitbox.bottom = 16;

                self->state = Spring_State_Horizontal;
                break;
            case 2:
                self->direction  = self->flipFlag;
                self->velocity.x = 0x54000 * ((self->flipFlag & 1) ? -1 : 1);
                self->velocity.y = 0x54000 * ((self->flipFlag < 2) ? -1 : 1);

                self->hitbox.left   = -12;
                self->hitbox.top    = -12;
                self->hitbox.right  = 12;
                self->hitbox.bottom = 12;

                self->state = Spring_State_Diagonal;
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
    if (self->type < 2)
        destroyEntity(self);
    else self->animator.frameID = 7;
}

#if RETRO_INCLUDE_EDITOR
void IceSpring_EditorLoad(void) { IceSpring_StageLoad(); }

void IceSpring_EditorDraw(void)
{
    RSDK_THIS(IceSpring);
    RSDK.SetSpriteAnimation(IceSpring->animID, self->type, &self->animator, true, 0);
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
