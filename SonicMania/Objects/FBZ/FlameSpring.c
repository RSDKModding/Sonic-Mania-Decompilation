// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FlameSpring Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFlameSpring *FlameSpring;

void FlameSpring_Update(void)
{
    RSDK_THIS(FlameSpring);

    StateMachine_Run(self->state);
}

void FlameSpring_LateUpdate(void) {}

void FlameSpring_StaticUpdate(void) {}

void FlameSpring_Draw(void)
{
    RSDK_THIS(FlameSpring);

    StateMachine_Run(self->stateDraw);
}

void FlameSpring_Create(void *data)
{
    RSDK_THIS(FlameSpring);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0] + 1;

        if (data) {
            RSDK.SetSpriteAnimation(FlameSpring->aniFrames, 2, &self->mainAnimator, true, 0);
            self->state     = FlameSpring_State_Flame;
            self->stateDraw = FlameSpring_Draw_Flame;
        }
        else {
            if (!(self->force & 1))
                self->velocity.y = -0xA0000;
            else
                self->velocity.y = -0x100000;
            RSDK.SetSpriteAnimation(FlameSpring->aniFrames, 0, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(FlameSpring->aniFrames, 1, &self->nozzleAnimator, true, 0);

            self->flamePosL.x = self->position.x - 0x300 * RSDK.Sin1024(0x100);
            self->flamePosL.y = self->position.y - 0x40000;
            self->flamePosR.x = 0x300 * RSDK.Sin1024(0x100) + self->position.x;
            self->flamePosR.y = self->position.y - 0x40000;

            if (self->type > FLAMESPRING_LEFT)
                self->mainAnimator.frameID = 2;

            self->type      = self->type % 3;
            self->state     = FlameSpring_State_Spring;
            self->stateDraw = FlameSpring_Draw_Spring;
        }
    }
}

void FlameSpring_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("FBZ"))
        FlameSpring->aniFrames = RSDK.LoadSpriteAnimation("FBZ/FlameSpring.bin", SCOPE_STAGE);

    FlameSpring->hitboxSpring.left   = -16;
    FlameSpring->hitboxSpring.top    = -8;
    FlameSpring->hitboxSpring.right  = 16;
    FlameSpring->hitboxSpring.bottom = 8;

    FlameSpring->hitboxFlame.left   = -4;
    FlameSpring->hitboxFlame.top    = -4;
    FlameSpring->hitboxFlame.right  = 4;
    FlameSpring->hitboxFlame.bottom = 4;

    FlameSpring->sfxSpring = RSDK.GetSfx("Global/Spring.wav");
    FlameSpring->sfxFlame  = RSDK.GetSfx("Stage/Flame2.wav");
}

void FlameSpring_State_Spring(void)
{
    RSDK_THIS(FlameSpring);

    self->spittingFire = false;
    foreach_active(Player, playerPtr)
    {
        if (Player_CheckCollisionBox(playerPtr, self, &FlameSpring->hitboxSpring) == C_TOP && playerPtr->velocity.y >= 0) {
            if (!self->mainAnimator.frameID) {
                self->timer                = 0;
                self->mainAnimator.frameID = 1;
            }

            if (self->mainAnimator.frameID <= 1)
                self->spittingFire = true;
        }
    }

    if (self->mainAnimator.frameID == 1) {
        if (self->spittingFire) {
            if (++self->timer >= 60) {
                foreach_active(Player, player)
                {
                    if (Player_CheckCollisionBox(player, self, &FlameSpring->hitboxSpring) == C_TOP) {
                        int32 anim = player->animator.animationID;
                        if (anim == ANI_WALK || (anim > ANI_AIR_WALK && anim <= ANI_DASH))
                            player->animationReserve = player->animator.animationID;
                        else
                            player->animationReserve = ANI_WALK;

                        if (self->playerAni)
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_TWIRL, &player->animator, true, 0);
                        else
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_DIAGONAL, &player->animator, true, 0);

                        player->state      = Player_State_Air;
                        player->onGround   = false;
                        player->velocity.y = self->velocity.y;
                        if (player->underwater)
                            player->velocity.y >>= 1;
                        RSDK.PlaySfx(FlameSpring->sfxSpring, false, 255);
                    }
                }
            }
        }
        else {
            self->mainAnimator.frameID = 0;
        }
    }
    else {
        if (!(Zone->timer & 7))
            RSDK.PlaySfx(FlameSpring->sfxFlame, false, 255);

        int32 timer = Zone->timer + self->offset;
        if (!self->type) {
            self->flamePosL.x = self->position.x - 0x300 * RSDK.Sin1024(((4 * timer) & 0x1FF) + 0x100);
            self->flamePosR.x = 0x300 * RSDK.Sin1024(((4 * timer) & 0x1FF) + 0x100) + self->position.x;
        }

        if (!(Zone->timer & 3)) {
            EntityFlameSpring *flame = NULL;
            switch (self->type) {
                case FLAMESPRING_BOTH:
                    flame             = CREATE_ENTITY(FlameSpring, INT_TO_VOID(true), self->flamePosL.x, self->flamePosL.y);
                    flame->drawGroup  = Zone->objectDrawGroup[0];
                    flame->velocity.x = -0x100 * RSDK.Sin1024(((4 * timer) & 0x1FF) + 0x100);

                    flame             = CREATE_ENTITY(FlameSpring, INT_TO_VOID(1), self->flamePosR.x, self->flamePosR.y);
                    flame->velocity.x = (RSDK.Sin1024(((4 * timer) & 0x1FF) + 256) << 8);
                    break;

                case FLAMESPRING_RIGHT:
                    flame            = CREATE_ENTITY(FlameSpring, INT_TO_VOID(true), self->flamePosR.x, self->flamePosR.y);
                    flame->drawGroup = Zone->objectDrawGroup[0];
                    if (0xC00 * RSDK.Sin1024((timer & 0x1FF) + 0x100) >= 0)
                        flame->velocity.x = (0x44000 - 0xC0 * RSDK.Sin1024((timer & 0x1FF) + 0x100));
                    else
                        flame->velocity.x = (0x44000 - -0xC0 * RSDK.Sin1024((timer & 0x1FF) + 0x100));
                    break;

                case FLAMESPRING_LEFT:
                    flame            = CREATE_ENTITY(FlameSpring, INT_TO_VOID(true), self->flamePosL.x, self->flamePosL.y);
                    flame->drawGroup = Zone->objectDrawGroup[0];
                    if (-0xC0 * RSDK.Sin1024((timer & 0x1FF) + 0x100) >= 0)
                        flame->velocity.x = (-0xC0 * RSDK.Sin1024((timer & 0x1FF) + 0x100) - 0x44000);
                    else
                        flame->velocity.x = (0xC0 * RSDK.Sin1024((timer & 0x1FF) + 0x100) - 0x44000);
                    break;
            }
        }
    }
}

void FlameSpring_State_Flame(void)
{
    RSDK_THIS(FlameSpring);

    self->position.x += self->velocity.x;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &FlameSpring->hitboxFlame)) {
            Player_ElementHurt(player, self, SHIELD_FIRE);
        }
    }

    RSDK.ProcessAnimation(&self->mainAnimator);
    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1)
        destroyEntity(self);
}

void FlameSpring_Draw_Spring(void)
{
    RSDK_THIS(FlameSpring);

    if (self->mainAnimator.frameID != 1 && self->type != FLAMESPRING_RIGHT) {
        self->nozzleAnimator.frameID = self->flamePosL.x > self->position.x;
        RSDK.DrawSprite(&self->nozzleAnimator, &self->flamePosL, false);
    }

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    if (self->mainAnimator.frameID != 1 && self->type != FLAMESPRING_LEFT) {
        self->nozzleAnimator.frameID = self->flamePosR.x > self->position.x;
        RSDK.DrawSprite(&self->nozzleAnimator, &self->flamePosR, false);
    }
}

void FlameSpring_Draw_Flame(void)
{
    RSDK_THIS(FlameSpring);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

#if GAME_INCLUDE_EDITOR
void FlameSpring_EditorDraw(void)
{
    RSDK_THIS(FlameSpring);

    self->flamePosL.x = self->position.x - 0x300 * RSDK.Sin1024(0x100);
    self->flamePosL.y = self->position.y - 0x40000;
    self->flamePosR.x = 0x300 * RSDK.Sin1024(0x100) + self->position.x;
    self->flamePosR.y = self->position.y - 0x40000;

    RSDK.SetSpriteAnimation(FlameSpring->aniFrames, 0, &self->mainAnimator, true, 0);
    RSDK.SetSpriteAnimation(FlameSpring->aniFrames, 1, &self->nozzleAnimator, true, 0);
    if (self->type > FLAMESPRING_LEFT)
        self->mainAnimator.frameID = 2;

    uint8 type = self->type % 3;
    if (self->mainAnimator.frameID != 1 && type != FLAMESPRING_RIGHT) {
        self->nozzleAnimator.frameID = self->flamePosL.x > self->position.x;
        RSDK.DrawSprite(&self->nozzleAnimator, &self->flamePosL, false);
    }

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    if (self->mainAnimator.frameID != 1 && type != FLAMESPRING_LEFT) {
        self->nozzleAnimator.frameID = self->flamePosR.x > self->position.x;
        RSDK.DrawSprite(&self->nozzleAnimator, &self->flamePosR, false);
    }
}

void FlameSpring_EditorLoad(void)
{
    FlameSpring->aniFrames = RSDK.LoadSpriteAnimation("FBZ/FlameSpring.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(FlameSpring, type);
    RSDK_ENUM_VAR("Circle", FLAMESPRING_BOTH);
    RSDK_ENUM_VAR("Right", FLAMESPRING_RIGHT);
    RSDK_ENUM_VAR("Left", FLAMESPRING_LEFT);
    RSDK_ENUM_VAR("Both Sides (No Spring)", FLAMESPRING_BOTH_NOSPRING);
    RSDK_ENUM_VAR("Right Only (No Spring)", FLAMESPRING_RIGHT_NOSPRING);
    RSDK_ENUM_VAR("Left Only (No Spring)", FLAMESPRING_LEFT_NOSPRING);

    RSDK_ACTIVE_VAR(FlameSpring, force);
    RSDK_ENUM_VAR("Weak", FLAMESPRING_FORCE_WEAK);
    RSDK_ENUM_VAR("Strong", FLAMESPRING_FORCE_STRONG);

    RSDK_ACTIVE_VAR(FlameSpring, playerAni);
    RSDK_ENUM_VAR("Spring Diagonal", FLAMESPRING_PANI_DIAGONAL);
    RSDK_ENUM_VAR("Spring Twirl", FLAMESPRING_PANI_TWIRL);
}
#endif

void FlameSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(FlameSpring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(FlameSpring, VAR_ENUM, force);
    RSDK_EDITABLE_VAR(FlameSpring, VAR_UINT8, playerAni);
    RSDK_EDITABLE_VAR(FlameSpring, VAR_UINT8, offset);
}
