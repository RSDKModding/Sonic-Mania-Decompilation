#include "SonicMania.h"

ObjectSpring *Spring;

void Spring_Update(void)
{
    RSDK_THIS(Spring);

    if (self->timer > 0)
        self->timer--;
    StateMachine_Run(self->state);
    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == 8)
        self->animator.animationSpeed = 0;
}

void Spring_LateUpdate(void) {}

void Spring_StaticUpdate(void) {}

void Spring_Draw(void)
{
    RSDK_THIS(Spring);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Spring_Create(void *data)
{
    RSDK_THIS(Spring);
    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->type %= 6;
        if (data) {
            int32 propertyVal  = voidToInt(data);
            self->type     = (propertyVal >> 0) & 0xFF;
            self->flipFlag = (propertyVal >> 8) & 0xFF;
        }
        RSDK.SetSpriteAnimation(Spring->aniFrames, self->type, &self->animator, true, 0);
        self->active                  = ACTIVE_BOUNDS;
        self->animator.animationSpeed = 0;
        self->updateRange.x           = 0x600000;
        self->updateRange.y           = 0x600000;
        self->visible                 = true;
        if (self->planeFilter && ((uint8)self->planeFilter - 1) & 2)
            self->drawOrder = Zone->drawOrderHigh;
        else
            self->drawOrder = Zone->drawOrderLow;

        switch (self->type >> 1) {
            case 0:
                self->direction = self->flipFlag;
                if (self->type & 1)
                    self->velocity.y = 0x100000;
                else
                    self->velocity.y = 0xA0000;
                if (!self->flipFlag)
                    self->velocity.y = -self->velocity.y;
                self->hitbox.left   = -16;
                self->hitbox.top    = -8;
                self->hitbox.right  = 16;
                self->hitbox.bottom = 8;
                self->state         = Spring_State_Vertical;
                break;
            case 1:
                self->direction = self->flipFlag;
                if (self->type & 1)
                    self->velocity.x = 0x100000;
                else
                    self->velocity.x = 0xA0000;
                if (self->flipFlag)
                    self->velocity.x = -self->velocity.x;
                self->hitbox.left   = -8;
                self->hitbox.top    = -16;
                self->hitbox.right  = 8;
                self->hitbox.bottom = 16;
                self->state         = Spring_State_Horizontal;
                break;
            case 2:
                self->direction = self->flipFlag;
                if (self->type & 1) {
                    self->velocity.x = 0xB4000;
                    self->velocity.y = 0xB4000;
                }
                else {
                    self->velocity.x = 0x74000;
                    self->velocity.y = 0x74000;
                }
                if (self->flipFlag < FLIP_Y)
                    self->velocity.y = -self->velocity.y;
                if (self->flipFlag & FLIP_X)
                    self->velocity.x = -self->velocity.x;
                self->hitbox.left   = -12;
                self->hitbox.top    = -12;
                self->hitbox.right  = 12;
                self->hitbox.bottom = 12;
                self->state         = Spring_State_Diagonal;
                break;
        }
    }
}

void Spring_StageLoad(void)
{
    Spring->aniFrames = RSDK.LoadSpriteAnimation("Global/Springs.bin", SCOPE_STAGE);
    Spring->sfxSpring  = RSDK.GetSFX("Global/Spring.wav");
}

void Spring_State_Vertical(void)
{
    RSDK_THIS(Spring);
    if (self->direction == FLIP_NONE) {
        foreach_active(Player, player)
        {
            if (!self->planeFilter || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                int32 col = (self->type == 0xFF && player->velocity.y < -0x50000) ? Player_CheckCollisionPlatform(player, self, &self->hitbox)
                                                                                  : Player_CheckCollisionBox(player, self, &self->hitbox);
                if (col == 1) {
                    int32 anim = player->playerAnimator.animationID;
                    if (anim == ANI_WALK || (anim > ANI_AIRWALK && anim <= ANI_DASH))
                        player->storedAnim = player->playerAnimator.animationID;
                    else
                        player->storedAnim = ANI_WALK;

                    if (player->state != Ice_State_FrozenPlayer) {
                        if (player->state == Player_State_ForceRoll_Air || player->state == Player_State_ForceRoll_Ground) {
                            player->state = Player_State_ForceRoll_Air;
                        }
                        else {
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
                            player->state = Player_State_Air;
                        }
                    }
                    player->onGround                = false;
                    player->velocity.y              = self->velocity.y;
                    player->tileCollisions          = true;
                    self->animator.animationSpeed = 0x80;
                    self->animator.animationTimer = 0;
                    self->animator.frameID        = 1;
                    if (self->timer == 0) {
                        RSDK.PlaySfx(Spring->sfxSpring, 0, 255);
                        self->timer = 8;
                    }
                }
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            if ((!self->planeFilter || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
                && Player_CheckCollisionBox(player, self, &self->hitbox) == 4) {
                if (player->state != Ice_State_FrozenPlayer) {
                    if (player->state == Player_State_ForceRoll_Air || player->state == Player_State_ForceRoll_Ground)
                        player->state = Player_State_ForceRoll_Air;
                    else
                        player->state = Player_State_Air;
                }
                player->onGround                = false;
                player->velocity.y              = self->velocity.y;
                player->tileCollisions          = true;
                self->animator.animationSpeed = 0x80;
                self->animator.animationTimer = 0;
                self->animator.frameID        = 1;
                if (!self->timer) {
                    RSDK.PlaySfx(Spring->sfxSpring, 0, 255);
                    self->timer = 8;
                }
            }
        }
    }
}
void Spring_State_Horizontal(void)
{
    RSDK_THIS(Spring);
    if (self->direction == FLIP_NONE) {
        foreach_active(Player, player)
        {
            if ((!self->planeFilter || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
                && Player_CheckCollisionBox(player, self, &self->hitbox) == 3 && (!self->onGround || player->onGround)) {
                if (player->collisionMode == CMODE_ROOF) {
                    player->velocity.x = -self->velocity.x;
                    player->groundVel  = -self->velocity.x;
                }
                else {
                    player->velocity.x = self->velocity.x;
                    player->groundVel  = player->velocity.x;
                }

                if (player->state != Ice_State_FrozenPlayer) {
                    if (player->state != Player_State_Roll && player->state != Player_State_ForceRoll_Air && player->state != Player_State_ForceRoll_Ground) {
                        if (player->onGround)
                            player->state = Player_State_Ground;
                        else
                            player->state = Player_State_Air;
                    }
                    int32 anim = player->playerAnimator.animationID;
                    if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                        player->playerAnimator.animationID = ANI_WALK;
                }
                player->controlLock             = 16;
                player->skidding                = 0;
                player->pushing                 = false;
                player->direction               = FLIP_NONE;
                player->tileCollisions          = true;
                self->animator.animationSpeed = 0x80;
                self->animator.animationTimer = 0;
                self->animator.frameID        = 1;
                if (self->timer == 0) {
                    RSDK.PlaySfx(Spring->sfxSpring, 0, 255);
                    self->timer = 8;
                }
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            if ((!self->planeFilter || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
                && Player_CheckCollisionBox(player, self, &self->hitbox) == 2 && (!self->onGround || player->onGround)) {
                if (player->collisionMode == CMODE_ROOF) {
                    player->velocity.x = -self->velocity.x;
                    player->groundVel  = -self->velocity.x;
                }
                else {
                    player->velocity.x = self->velocity.x;
                    player->groundVel  = player->velocity.x;
                }

                if (player->state != Ice_State_FrozenPlayer) {
                    if (player->state != Player_State_Roll && player->state != Player_State_ForceRoll_Air && player->state != Player_State_ForceRoll_Ground) {
                        if (player->onGround)
                            player->state = Player_State_Ground;
                        else
                            player->state = Player_State_Air;
                    }
                    int32 anim = player->playerAnimator.animationID;
                    if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                        player->playerAnimator.animationID = ANI_WALK;
                }
                player->controlLock             = 16;
                player->skidding                = 0;
                player->pushing                 = false;
                player->direction               = FLIP_X;
                player->tileCollisions          = true;
                self->animator.animationSpeed = 0x80;
                self->animator.animationTimer = 0;
                self->animator.frameID        = 1;
                if (self->timer == 0) {
                    RSDK.PlaySfx(Spring->sfxSpring, 0, 255);
                    self->timer = 8;
                }
            }
        }
    }
}
void Spring_State_Diagonal(void)
{
    RSDK_THIS(Spring);
    foreach_active(Player, player)
    {
        if ((!self->planeFilter || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                bool32 flag = false;
                if (player->onGround) {
                    flag = true;
                }
                else {
                    int32 y = player->velocity.y;
                    if (y >= 0) {
                        flag = true;
                    }
                    else {
                        int32 x = player->velocity.x;
                        if (x < 0)
                            x = -x;
                        flag = x > -y;
                    }
                }
                if (flag) {
                    if (player->state != Ice_State_FrozenPlayer) {
                        if (player->state == Player_State_ForceRoll_Air || player->state == Player_State_ForceRoll_Ground) {
                            player->state = Player_State_ForceRoll_Air;
                        }
                        else {
                            player->state = Player_State_Air;
                            int32 anim      = player->playerAnimator.animationID;
                            if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                                player->playerAnimator.animationID = ANI_WALK;
                        }
                    }
                    if (self->direction < FLIP_Y) {
                        if (player->state != Player_State_ForceRoll_Air && player->state != Player_State_ForceRoll_Ground) {
                            int32 anim = player->playerAnimator.animationID;
                            if (anim == ANI_WALK || (anim > ANI_AIRWALK && anim <= ANI_DASH))
                                player->storedAnim = player->playerAnimator.animationID;
                            else
                                player->storedAnim = ANI_WALK;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGDIAGONAL, &player->playerAnimator, true, 0);
                        }
                    }
                    player->direction               = self->direction & 1;
                    player->onGround                = false;
                    player->velocity.x              = self->velocity.x;
                    player->velocity.y              = self->velocity.y;
                    player->tileCollisions          = true;
                    self->animator.animationSpeed = 0x80;
                    self->animator.animationTimer = 0;
                    self->animator.frameID        = 1;
                    if (self->timer == 0) {
                        RSDK.PlaySfx(Spring->sfxSpring, 0, 255);
                        self->timer = 8;
                    }
                }
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Spring_EditorDraw(void)
{
    RSDK_THIS(Spring);
    RSDK.SetSpriteAnimation(Spring->aniFrames, self->type, &self->animator, true, 0);
    self->direction = self->flipFlag;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Spring_EditorLoad(void)
{
    Spring->aniFrames = RSDK.LoadSpriteAnimation("Global/Springs.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Spring, type);
    RSDK_ENUM_VAR("Vertical (Red)", SPRING_VERT_RED);
    RSDK_ENUM_VAR("Vertical (Yellow)", SPRING_VERT_YELLOW);
    RSDK_ENUM_VAR("Horizontal (Red)", SPRING_HORIZ_RED);
    RSDK_ENUM_VAR("Horizontal (Yellow)", SPRING_HORIZ_YELLOW);
    RSDK_ENUM_VAR("Diagonal (Red)", SPRING_DIAG_RED);
    RSDK_ENUM_VAR("Diagonal (Yellow)", SPRING_DIAG_YELLOW);

    RSDK_ACTIVE_VAR(Spring, flipFlag);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);

    RSDK_ACTIVE_VAR(Spring, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
}
#endif

void Spring_Serialize(void)
{
    RSDK_EDITABLE_VAR(Spring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Spring, VAR_ENUM, flipFlag);
    RSDK_EDITABLE_VAR(Spring, VAR_BOOL, onGround);
    RSDK_EDITABLE_VAR(Spring, VAR_UINT8, planeFilter);
}
