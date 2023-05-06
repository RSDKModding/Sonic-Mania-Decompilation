// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Spring Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpring *Spring;

void Spring_Update(void)
{
    RSDK_THIS(Spring);

    if (self->sfxTimer > 0)
        self->sfxTimer--;

    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == 8)
        self->animator.speed = 0;
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
            int32 propertyVal = VOID_TO_INT(data);
            self->type        = (propertyVal >> 0) & 0xFF;
            self->flipFlag    = (propertyVal >> 8) & 0xFF;
        }

        RSDK.SetSpriteAnimation(Spring->aniFrames, self->type, &self->animator, true, 0);
        self->active         = ACTIVE_BOUNDS;
        self->animator.speed = 0;
        self->updateRange.x  = TO_FIXED(96);
        self->updateRange.y  = TO_FIXED(96);
        self->visible        = true;

        if (self->planeFilter && ((uint8)self->planeFilter - 1) & 2)
            self->drawGroup = Zone->objectDrawGroup[1];
        else
            self->drawGroup = Zone->objectDrawGroup[0];

        switch (self->type >> 1) {
            case 0: // vertical
                self->direction = self->flipFlag;
                if (self->type & 1)
                    self->velocity.y = TO_FIXED(16);
                else
                    self->velocity.y = TO_FIXED(10);

                if (!self->flipFlag)
                    self->velocity.y = -self->velocity.y;

                self->hitbox.left   = -16;
                self->hitbox.top    = -8;
                self->hitbox.right  = 16;
                self->hitbox.bottom = 8;

                self->state = Spring_State_Vertical;
                break;

            case 1: // horizontal
                self->direction = self->flipFlag;
                if (self->type & 1)
                    self->velocity.x = TO_FIXED(16);
                else
                    self->velocity.x = TO_FIXED(10);

                if (self->flipFlag)
                    self->velocity.x = -self->velocity.x;

                self->hitbox.left   = -8;
                self->hitbox.top    = -16;
                self->hitbox.right  = 8;
                self->hitbox.bottom = 16;

                self->state = Spring_State_Horizontal;
                break;

            case 2: // diagonal
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

                self->state = Spring_State_Diagonal;
                break;
        }
    }
}

void Spring_StageLoad(void)
{
    Spring->aniFrames = RSDK.LoadSpriteAnimation("Global/Springs.bin", SCOPE_STAGE);

    Spring->sfxSpring = RSDK.GetSfx("Global/Spring.wav");
}

void Spring_State_Vertical(void)
{
    RSDK_THIS(Spring);

    if (self->direction == FLIP_NONE) {
        foreach_active(Player, player)
        {
            if (!self->planeFilter || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                int32 col = (self->type == SPRING_TUBESPRING && player->velocity.y < -0x50000)
                                ? Player_CheckCollisionPlatform(player, self, &self->hitbox)
                                : Player_CheckCollisionBox(player, self, &self->hitbox);

                if (col == C_TOP) {
                    int32 anim = player->animator.animationID;

                    player->animationReserve = ANI_WALK;
                    if (anim == ANI_WALK || (anim > ANI_AIR_WALK && anim <= ANI_DASH))
                        player->animationReserve = player->animator.animationID;

                    if (player->state != Ice_PlayerState_Frozen) {
                        if (player->state == Player_State_TubeAirRoll || player->state == Player_State_TubeRoll) {
                            player->state = Player_State_TubeAirRoll;
                        }
                        else {
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_TWIRL, &player->animator, true, 0);
                            player->state = Player_State_Air;
                        }
                    }

                    player->onGround       = false;
                    player->velocity.y     = self->velocity.y;
                    player->tileCollisions = TILECOLLISION_DOWN;

                    self->animator.speed   = 0x80;
                    self->animator.timer   = 0;
                    self->animator.frameID = 1;

                    if (self->sfxTimer == 0) {
                        RSDK.PlaySfx(Spring->sfxSpring, false, 255);
                        self->sfxTimer = 8;
                    }
                }
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (!self->planeFilter || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_BOTTOM) {
                    if (player->state != Ice_PlayerState_Frozen) {
                        if (player->state == Player_State_TubeAirRoll || player->state == Player_State_TubeRoll)
                            player->state = Player_State_TubeAirRoll;
                        else
                            player->state = Player_State_Air;
                    }

                    player->onGround       = false;
                    player->velocity.y     = self->velocity.y;
                    player->tileCollisions = TILECOLLISION_DOWN;

                    self->animator.speed   = 0x80;
                    self->animator.timer   = 0;
                    self->animator.frameID = 1;

                    if (!self->sfxTimer) {
                        RSDK.PlaySfx(Spring->sfxSpring, false, 255);
                        self->sfxTimer = 8;
                    }
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
            if (!self->planeFilter || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_RIGHT && (!self->onGround || player->onGround)) {
                    if (player->collisionMode == CMODE_ROOF) {
                        player->velocity.x = -self->velocity.x;
                        player->groundVel  = -self->velocity.x;
                    }
                    else {
                        player->velocity.x = self->velocity.x;
                        player->groundVel  = player->velocity.x;
                    }

                    if (player->state != Ice_PlayerState_Frozen) {
                        if (player->state != Player_State_Roll && player->state != Player_State_TubeAirRoll
                            && player->state != Player_State_TubeRoll) {
                            player->state = player->onGround ? Player_State_Ground : Player_State_Air;
                        }

                        int32 anim = player->animator.animationID;
                        if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                            player->animator.animationID = ANI_WALK;
                    }

                    player->controlLock    = 16;
                    player->skidding       = false;
                    player->pushing        = false;
                    player->direction      = FLIP_NONE;
                    player->tileCollisions = TILECOLLISION_DOWN;

                    self->animator.speed   = 0x80;
                    self->animator.timer   = 0;
                    self->animator.frameID = 1;

                    if (self->sfxTimer == 0) {
                        RSDK.PlaySfx(Spring->sfxSpring, false, 255);
                        self->sfxTimer = 8;
                    }
                }
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (!self->planeFilter || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_LEFT && (!self->onGround || player->onGround)) {
                    if (player->collisionMode == CMODE_ROOF) {
                        player->velocity.x = -self->velocity.x;
                        player->groundVel  = -self->velocity.x;
                    }
                    else {
                        player->velocity.x = self->velocity.x;
                        player->groundVel  = player->velocity.x;
                    }

                    if (player->state != Ice_PlayerState_Frozen) {
                        if (player->state != Player_State_Roll && player->state != Player_State_TubeAirRoll
                            && player->state != Player_State_TubeRoll) {
                            player->state = player->onGround ? Player_State_Ground : Player_State_Air;
                        }

                        int32 anim = player->animator.animationID;
                        if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                            player->animator.animationID = ANI_WALK;
                    }

                    player->controlLock    = 16;
                    player->skidding       = 0;
                    player->pushing        = false;
                    player->direction      = FLIP_X;
                    player->tileCollisions = TILECOLLISION_DOWN;

                    self->animator.speed   = 0x80;
                    self->animator.timer   = 0;
                    self->animator.frameID = 1;

                    if (self->sfxTimer == 0) {
                        RSDK.PlaySfx(Spring->sfxSpring, false, 255);
                        self->sfxTimer = 8;
                    }
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
                bool32 collided = false;
                if (player->onGround || player->velocity.y >= 0)
                    collided = true;
                else
                    collided = abs(player->velocity.x) > -player->velocity.y;

                if (collided) {
                    if (player->state != Ice_PlayerState_Frozen) {
                        if (player->state == Player_State_TubeAirRoll || player->state == Player_State_TubeRoll) {
                            player->state = Player_State_TubeAirRoll;
                        }
                        else {
                            player->state = Player_State_Air;

                            int32 anim = player->animator.animationID;
                            if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                                player->animator.animationID = ANI_WALK;
                        }
                    }

                    if (self->direction < FLIP_Y) {
                        if (player->state != Player_State_TubeAirRoll && player->state != Player_State_TubeRoll) {
                            int32 anim               = player->animator.animationID;
                            player->animationReserve = ANI_WALK;
                            if (anim == ANI_WALK || (anim > ANI_AIR_WALK && anim <= ANI_DASH))
                                player->animationReserve = player->animator.animationID;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_DIAGONAL, &player->animator, true, 0);
                        }
                    }

                    player->direction      = self->direction & 1;
                    player->onGround       = false;
                    player->velocity.x     = self->velocity.x;
                    player->velocity.y     = self->velocity.y;
                    player->tileCollisions = TILECOLLISION_DOWN;

                    self->animator.speed   = 0x80;
                    self->animator.timer   = 0;
                    self->animator.frameID = 1;

                    if (self->sfxTimer == 0) {
                        RSDK.PlaySfx(Spring->sfxSpring, false, 255);
                        self->sfxTimer = 8;
                    }
                }
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
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
    RSDK_ENUM_VAR("Vertical (Yellow)", SPRING_VERT_YELLOW);
    RSDK_ENUM_VAR("Vertical (Red)", SPRING_VERT_RED);
    RSDK_ENUM_VAR("Horizontal (Yellow)", SPRING_HORIZ_YELLOW);
    RSDK_ENUM_VAR("Horizontal (Red)", SPRING_HORIZ_RED);
    RSDK_ENUM_VAR("Diagonal (Yellow)", SPRING_DIAG_YELLOW);
    RSDK_ENUM_VAR("Diagonal (Red)", SPRING_DIAG_RED);

    RSDK_ACTIVE_VAR(Spring, flipFlag);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);

    RSDK_ACTIVE_VAR(Spring, planeFilter);
    RSDK_ENUM_VAR("None", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("AL", PLANEFILTER_AL);
    RSDK_ENUM_VAR("BL", PLANEFILTER_BL);
    RSDK_ENUM_VAR("AH", PLANEFILTER_AH);
    RSDK_ENUM_VAR("BH", PLANEFILTER_BH);
}
#endif

void Spring_Serialize(void)
{
    RSDK_EDITABLE_VAR(Spring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Spring, VAR_ENUM, flipFlag);
    RSDK_EDITABLE_VAR(Spring, VAR_BOOL, onGround);
    RSDK_EDITABLE_VAR(Spring, VAR_UINT8, planeFilter);
}
