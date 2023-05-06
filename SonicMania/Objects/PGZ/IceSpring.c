// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: IceSpring Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectIceSpring *IceSpring;

void IceSpring_Update(void)
{
    RSDK_THIS(IceSpring);

    // this was unironically fun to decompile
    // so many label jumps
    // - RMG
    self->activePlayers = 0;

    if (self->state) {
        if (!self->animator.speed) {
            // Bug Details:
            // due to this foreach loop using playerID as a variable instead of player->playerID there's a bug where if you have P2 hit the spring
            // while P1 is in debug mode the animator will be copied into P1's slot instead of P2's one Fix: use player->playerID or playerID =
            // RSDK.GetEntitySlot(player) instead of a manual playerID variable
            int32 playerID = 0;

            foreach_active(Player, player)
            {
                if (!self->planeFilter || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                    int32 storeX = player->position.x;
                    int32 storeY = player->position.y;

                    int32 collided = false;
                    if (player->state == Ice_PlayerState_Frozen)
                        collided = MathHelpers_CheckBoxCollision(self, &self->hitbox, player, &Ice->hitboxPlayerBlockOuter);
                    else
                        collided = MathHelpers_CheckBoxCollision(self, &self->hitbox, player, Player_GetHitbox(player));

                    if (collided) {
                        int32 type       = self->type;
                        bool32 canSpring = false;
                        switch (self->type) {
                            default:
                            case ICESPRING_DIAGONAL: break;

                            case ICESPRING_VERTICAL: canSpring = (self->direction & FLIP_Y) ? collided == C_BOTTOM : collided == C_TOP; break;

                            case ICESPRING_HORIZONTAL: canSpring = (self->direction & FLIP_X) ? collided == C_LEFT : collided == C_RIGHT; break;
                        }

                        if (!canSpring && type == ICESPRING_DIAGONAL)
                            canSpring |= (player->onGround || player->velocity.y >= 0 || abs(player->velocity.x) > -player->velocity.y);

                        if (canSpring) {
                            self->activePlayers |= 1 << playerID;
                            memcpy(&IceSpring->animators[playerID], &player->animator, sizeof(Animator));
                        }

                        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield);
                        if (player->state != Ice_PlayerState_Frozen && shield->shieldAnimator.animationID != SHIELDANI_FIREATTACK
#if MANIA_USE_PLUS
                            && player->state != Player_State_MightyHammerDrop
#endif
                        ) {
                            if (player->shield == SHIELD_FIRE && player->invincibleTimer <= 0 && !self->shatterTimer) {
                                self->shatterTimer = 15;
                                Ice_ShatterGenerator(6, 8, 8, 0, 0, false);
                            }
                        }
                        else if (!canSpring
#if MANIA_USE_PLUS
                                 || player->state == Player_State_MightyHammerDrop
#endif
                        ) {
                            switch (collided) {
                                case C_TOP: IceSpring_Shatter(0, 0); break;
                                case C_LEFT:
                                case C_RIGHT: IceSpring_Shatter(player->velocity.x, 0); break;
                                case C_BOTTOM: IceSpring_Shatter(0, player->velocity.y); break;
                                default: break;
                            }

                            foreach_return;
                        }
                    }

                    player->position.x = storeX;
                    player->position.y = storeY;
                }

                ++playerID;
            }
        }

        StateMachine_Run(self->state);

        for (int32 i = 0; i < Player->playerCount; ++i) {
            if ((1 << i) & self->activePlayers) {
                EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
                if (IceSpring->animators[i].animationID == ANI_JUMP)
                    memcpy(&player->animator, &IceSpring->animators[i], sizeof(Animator));

                player->applyJumpCap = false;
                if (player->sidekick && self->activePlayers == 0b10) {
                    RSDK.SetSpriteAnimation(IceSpring->aniFrames, self->type, &self->animator, true, 0);
                    self->animator.speed = 0;
                    self->activePlayers  = 0;
                }
            }
        }

        RSDK.ProcessAnimation(&self->animator);
        if (self->animator.frameID == 7)
            IceSpring_Shatter(0, 0);
    }

    if (self->shatterTimer > 0) {
        if (self->animator.frameID <= 0) {
            --self->shatterTimer;
            if (self->shatterTimer <= 0)
                IceSpring_Shatter(0, 0);
        }
        else
            self->shatterTimer = 0;
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
            self->type     = (VOID_TO_INT(data) >> 0) & 0xFF;
            self->flipFlag = (VOID_TO_INT(data) >> 8) & 0xFF;
        }

        RSDK.SetSpriteAnimation(IceSpring->aniFrames, self->type, &self->animator, true, 0);
        self->active         = ACTIVE_BOUNDS;
        self->animator.speed = 0;
        self->updateRange.x  = 0x600000;
        self->updateRange.y  = 0x600000;
        self->visible        = true;

        if (self->planeFilter && ((uint8)(self->planeFilter - 1) & 2))
            self->drawGroup = Zone->objectDrawGroup[1];
        else
            self->drawGroup = Zone->objectDrawGroup[0];

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
    IceSpring->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/IceSpring.bin", SCOPE_STAGE);

    IceSpring->sfxBounce  = RSDK.GetSfx("Global/Spring.wav");
    IceSpring->sfxShatter = RSDK.GetSfx("Stage/WindowShatter.wav");
}

void IceSpring_Shatter(int32 velX, int32 velY)
{
    RSDK_THIS(IceSpring);

    RSDK.PlaySfx(IceSpring->sfxShatter, false, 255);
    Ice_ShatterGenerator(16, 8, 8, velX, velY, false);

    if (self->type < ICESPRING_DIAGONAL)
        destroyEntity(self);
    else
        self->animator.frameID = 7;
}

#if GAME_INCLUDE_EDITOR
void IceSpring_EditorDraw(void)
{
    RSDK_THIS(IceSpring);

    RSDK.SetSpriteAnimation(IceSpring->aniFrames, self->type % 3, &self->animator, true, 0);
    self->direction = self->flipFlag & 3;

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
    RSDK_ENUM_VAR("None", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("AL", PLANEFILTER_AL);
    RSDK_ENUM_VAR("BL", PLANEFILTER_BL);
    RSDK_ENUM_VAR("AH", PLANEFILTER_AH);
    RSDK_ENUM_VAR("BH", PLANEFILTER_BH);
}
#endif

void IceSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(IceSpring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(IceSpring, VAR_ENUM, flipFlag);
    RSDK_EDITABLE_VAR(IceSpring, VAR_BOOL, onGround);
    RSDK_EDITABLE_VAR(IceSpring, VAR_UINT8, planeFilter);
}
