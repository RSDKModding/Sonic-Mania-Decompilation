#include "SonicMania.h"

ObjectIceSpring *IceSpring;

void IceSpring_Update(void)
{
    // this was unironically fun to decompile
    // so many label jumps
    RSDK_THIS(IceSpring);
    self->playerBits = 0;
    if (self->state) {
        if (!self->animator.speed) {
            int32 i = 0;
            foreach_active(Player, player)
            {
                if (self->planeFilter && player->collisionPlane != ((uint8)(self->planeFilter - 1) & 1))
                    continue;

                int32 storeX = player->position.x;
                int32 storeY = player->position.y;

                int32 collided = 0;
                if (player->state == Ice_State_FrozenPlayer)
                    collided = RSDK.CheckObjectCollisionBox(self, &self->hitbox, player, &Ice->hitboxPlayerBlockOuter, 0);
                else
                    collided = RSDK.CheckObjectCollisionBox(self, &self->hitbox, player, Player_GetHitbox(player), 0);
                if (!collided) {
                    ++i;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    continue;
                }

                int32 type          = self->type;
                bool32 collideCheck = false;
                switch (self->type) {
                    case ICESPRING_VERTICAL:
                        if (self->direction & FLIP_Y)
                            collideCheck = collided == C_BOTTOM;
                        else
                            collideCheck = collided == C_TOP;
                        break;
                    case ICESPRING_HORIZONTAL:
                        if (self->direction & FLIP_X)
                            collideCheck = collided == C_LEFT;
                        else
                            collideCheck = collided == C_RIGHT;
                        break;
                }

                if (!collideCheck && type == ICESPRING_DIAGONAL) {
                    collideCheck |= (player->onGround || player->velocity.y >= 0 || abs(player->velocity.x) > -player->velocity.y);
                }
                if (collideCheck) {
                    self->playerBits |= 1 << i;
                    memcpy(&IceSpring->animators[i], &player->animator, sizeof(Animator));
                }

                EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
                if (player->state != Ice_State_FrozenPlayer && shield->animator.animationID != 2
#if RETRO_USE_PLUS
                    && player->state != Player_State_MightyHammerDrop
#endif
                ) {
                    if (player->shield == SHIELD_FIRE && player->invincibleTimer <= 0 && !self->timer) {
                        self->timer = 15;
                        Ice_ShatterGenerator(8, 8, 6, 0, 0, false);
                    }
                    ++i;
                }
                else if (!collideCheck
#if RETRO_USE_PLUS
                         || player->state == Player_State_MightyHammerDrop
#endif
                ) {
                    switch (collided) {
                        case C_TOP: IceSpring_Shatter(0, 0); foreach_break;
                        case C_LEFT:
                        case C_RIGHT: IceSpring_Shatter(player->velocity.x, 0); foreach_break;
                        case C_BOTTOM: IceSpring_Shatter(0, player->velocity.y); foreach_break;
                        // RMG: this is NOT my own bug, this is in mania
                        // future RDC here, this is just a compiler thing, this code will NEVER be reached
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
                    memcpy(&player->animator, &IceSpring->animators[i], sizeof(Animator));
                }
                player->jumpAbility = 0;
                if (player->sidekick && self->playerBits == 0b10) {
                    RSDK.SetSpriteAnimation(IceSpring->aniFrames, self->type, &self->animator, true, 0);
                    self->animator.speed = 0;
                    self->playerBits     = 0;
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
            self->type     = (voidToInt(data) >> 0) & 0xFF;
            self->flipFlag = (voidToInt(data) >> 8) & 0xFF;
        }
        RSDK.SetSpriteAnimation(IceSpring->aniFrames, self->type, &self->animator, true, 0);
        self->active         = ACTIVE_BOUNDS;
        self->animator.speed = 0;
        self->updateRange.x  = 0x600000;
        self->updateRange.y  = 0x600000;
        self->visible        = true;
        if (self->planeFilter && ((uint8)(self->planeFilter - 1) & 2))
            self->drawOrder = Zone->drawOrderHigh;
        else
            self->drawOrder = Zone->drawOrderLow;

        switch (self->type) {
            case ICESPRING_VERTICAL:
                self->velocity.y = 0x80000 * (self->flipFlag ? 1 : -1);
                self->direction  = self->flipFlag;

                self->hitbox.left   = -16;
                self->hitbox.top    = -8;
                self->hitbox.right  = 16;
                self->hitbox.bottom = 8;

                self->state = Spring_State_Vertical;
                break;
            case ICESPRING_HORIZONTAL:
                self->velocity.x = 0x80000 * (self->flipFlag ? -1 : 1);
                self->direction  = self->flipFlag;

                self->hitbox.left   = -8;
                self->hitbox.top    = -16;
                self->hitbox.right  = 8;
                self->hitbox.bottom = 16;

                self->state = Spring_State_Horizontal;
                break;
            case ICESPRING_DIAGONAL:
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
    IceSpring->aniFrames  = RSDK.LoadSpriteAnimation("PSZ2/IceSpring.bin", SCOPE_STAGE);
    IceSpring->sfxBounce  = RSDK.GetSfx("Global/Spring.wav");
    IceSpring->sfxShatter = RSDK.GetSfx("Stage/WindowShatter.wav");
}

void IceSpring_Shatter(int32 velX, int32 velY)
{
    RSDK_THIS(IceSpring);
    RSDK.PlaySfx(IceSpring->sfxShatter, false, 255);
    Ice_ShatterGenerator(8, 8, 16, velX, velY, false);
    if (self->type < ICESPRING_DIAGONAL)
        destroyEntity(self);
    else
        self->animator.frameID = 7;
}

#if RETRO_INCLUDE_EDITOR
void IceSpring_EditorDraw(void)
{
    RSDK_THIS(IceSpring);
    RSDK.SetSpriteAnimation(IceSpring->aniFrames, self->type % 3, &self->animator, true, 0);
    self->direction = self->flipFlag;

    IceSpring_Draw();
}

void IceSpring_EditorLoad(void)
{
    IceSpring->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/IceSpring.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(IceSpring, type);
    RSDK_ENUM_VAR("Vertical", ICESPRING_VERTICAL);
    RSDK_ENUM_VAR("Horizontal", ICESPRING_HORIZONTAL);
    RSDK_ENUM_VAR("Diagonal", ICESPRING_DIAGONAL);

    RSDK_ACTIVE_VAR(IceSpring, flipFlag);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);

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
