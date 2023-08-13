// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SeeSaw Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSeeSaw *SeeSaw;

void SeeSaw_Update(void)
{
    RSDK_THIS(SeeSaw);

    if (self->tiltTimerL || self->tiltTimerR) {
        if (self->tiltTimerL == self->tiltTimerR) {
            self->targetTilt = SEESAW_TILT_M;
        }
        else {
            if (self->tiltTimerL > self->tiltTimerR)
                self->targetTilt = SEESAW_TILT_L;
            else
                self->targetTilt = SEESAW_TILT_R;
        }
    }
    else if (self->tiltTimerM != (self->tiltTimerR || self->tiltTimerL)) {
        self->targetTilt = SEESAW_TILT_M;
    }

    if (self->tilt != self->targetTilt) {
        self->tilt = self->tilt <= self->targetTilt ? self->tilt + 1 : self->tilt - 1;

        if (self->state == SeeSaw_State_None) {
            if (self->targetTilt == SEESAW_TILT_M) {
                self->velocity.x = -0x11400;
                self->velocity.y = -0x81800;
            }
            else {
                if (!self->orbTimer) {
                    if (self->launchVelocity < 0xA0000) {
                        self->velocity.x = -0xCC00;
                        self->velocity.y = -0xAF000;
                    }
                    else {
                        self->velocity.x = -0xA000;
                        self->velocity.y = -0xE0000;
                    }
                }
                else {
                    if (self->launchVelocity < 0x9C000) {
                        self->velocity.x = -0xF400;
                        self->velocity.y = -0x96000;
                    }
                    else {
                        self->velocity.x = -0x8000;
                        self->velocity.y = -0xA2000;
                    }
                }
            }

            if (self->orbPos.x < self->position.x)
                self->velocity.x = -self->velocity.x;

            self->state  = SeeSaw_State_OrbLaunched;
            self->active = ACTIVE_NORMAL;
        }
    }

    if (self->orbPos.x < self->position.x) {
        switch (self->tilt) {
            case SEESAW_TILT_L:
                SeeSaw->orbTargetPos.x = -0x280000;
                SeeSaw->orbTargetPos.y = -0x20000;
                break;

            case SEESAW_TILT_M:
                SeeSaw->orbTargetPos.x = -0x280000;
                SeeSaw->orbTargetPos.y = -0x160000;
                break;

            case SEESAW_TILT_R:
                SeeSaw->orbTargetPos.x = -0x280000;
                SeeSaw->orbTargetPos.y = -0x290000;
                break;
        }
    }
    else {
        switch (self->tilt) {
            case SEESAW_TILT_L:
                SeeSaw->orbTargetPos.x = 0x280000;
                SeeSaw->orbTargetPos.y = -0x290000;
                break;

            case SEESAW_TILT_M:
                SeeSaw->orbTargetPos.x = 0x280000;
                SeeSaw->orbTargetPos.y = -0x160000;
                break;

            case SEESAW_TILT_R:
                SeeSaw->orbTargetPos.x = 0x280000;
                SeeSaw->orbTargetPos.y = -0x20000;
                break;
        }
    }

    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->orbAnimator);

    self->tiltTimerL = 0;
    self->tiltTimerM = 0;
    self->tiltTimerR = 0;
    self->stood      = false;

    foreach_active(Player, player)
    {
        SeeSaw_SetupHitbox(player->position.x, self->prevTilt);

        if (SeeSaw->hitboxPlank.right) {
            if (player->velocity.y > self->launchVelocity)
                SeeSaw->launchVelocity = player->velocity.y + 0x7000;
            else
                SeeSaw->launchVelocity = self->launchVelocity;

            if (Player_CheckCollisionPlatform(player, self, &SeeSaw->hitboxPlank)) {
#if MANIA_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop) {
                    if (player->position.x >= self->position.x ? self->tilt != SEESAW_TILT_R : self->tilt != SEESAW_TILT_L)
                        player->state = Player_State_Air;
                }
#endif

                self->stood = true;

                if (self->tilt != self->prevTilt) {
                    int32 top = SeeSaw->hitboxPlank.top;
                    SeeSaw_SetupHitbox(player->position.x, self->tilt);
                    player->position.y += (SeeSaw->hitboxPlank.top - top) << 16;
                }

                player->position.y += 0x20000;

                if (self->orbSide) {
                    SeeSaw->launchVelocity = self->velocity.y;
                    if (abs(player->position.x - self->position.x) >= 0x80000) {
                        if (player->position.x >= self->position.x ? self->orbSide == 2 : self->orbSide == 1)
                            SeeSaw->launchVelocity = 0;
                    }

                    if (SeeSaw->launchVelocity) {
                        player->state    = Player_State_Air;
                        player->onGround = false;

                        if (self->state == SeeSaw_State_NoOrb || self->orbTimer)
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                        else
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_TWIRL, &player->animator, true, 0);

                        RSDK.PlaySfx(SeeSaw->sfxSpring, false, 255);
                        player->velocity.y = -SeeSaw->launchVelocity;
                    }
                }
                else {
                    if (self->state == SeeSaw_State_OrbIdle)
                        self->state = SeeSaw_State_None;
                    else if (self->state == SeeSaw_State_OrbLaunched)
                        SeeSaw->launchVelocity = self->launchVelocity;

                    if (abs(player->position.x - self->position.x) >= 0x80000) {
                        if (player->position.x >= self->position.x) {
                            ++self->tiltTimerR;
                            if (self->targetTilt != SEESAW_TILT_R)
                                self->launchVelocity = SeeSaw->launchVelocity;
                        }
                        else {
                            ++self->tiltTimerL;
                            if (self->targetTilt != SEESAW_TILT_L)
                                self->launchVelocity = SeeSaw->launchVelocity;
                        }
                    }
                    else {
                        ++self->tiltTimerM;
                    }
                }
            }
        }
    }

    self->prevTilt = self->tilt;
    self->orbSide  = 0;

    switch (self->prevTilt) {
        case SEESAW_TILT_L: self->rotation = 480; break;
        case SEESAW_TILT_M: self->rotation = 0; break;
        case SEESAW_TILT_R: self->rotation = 32; break;
    }

    if (self->state != SeeSaw_State_NoOrb) {
        int32 storeX   = self->position.x;
        int32 storeY   = self->position.y;
        self->position = self->orbPos;

        foreach_active(Player, playerPtr)
        {
            if (Player_CheckCollisionTouch(playerPtr, self, &SeeSaw->hitboxCactinaut)) {
#if MANIA_USE_PLUS
                if (!Player_CheckMightyUnspin(playerPtr, 0x300, 2, &player->uncurlTimer))
#endif
                    Player_Hurt(playerPtr, self);
            }
        }

        self->position.x = storeX;
        self->position.y = storeY;
    }
}

void SeeSaw_LateUpdate(void) {}

void SeeSaw_StaticUpdate(void) {}

void SeeSaw_Draw(void)
{
    RSDK_THIS(SeeSaw);

    RSDK.DrawSprite(&self->orbAnimator, &self->orbPos, false);
    RSDK.DrawSprite(&self->plankAnimator, NULL, false);
    RSDK.DrawSprite(&self->pivotAnimator, NULL, false);
}

void SeeSaw_Create(void *data)
{
    RSDK_THIS(SeeSaw);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 0, &self->pivotAnimator, true, 0);
        RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 1, &self->plankAnimator, true, 0);
        RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 2, &self->orbAnimator, true, 0);

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawFX        = FX_ROTATE | FX_FLIP;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1];

        if (self->side == FLIP_X) {
            self->targetTilt = SEESAW_TILT_R;
            self->orbPos.x   = self->position.x;
        }

        self->state = SeeSaw_State_OrbIdle;
    }
}

void SeeSaw_StageLoad(void)
{
    SeeSaw->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SeeSaw.bin", SCOPE_STAGE);

    SeeSaw->hitboxCactinaut.left   = -8;
    SeeSaw->hitboxCactinaut.top    = -8;
    SeeSaw->hitboxCactinaut.right  = 8;
    SeeSaw->hitboxCactinaut.bottom = 8;

    SeeSaw->sfxSpring = RSDK.GetSfx("Global/Spring.wav");
}

void SeeSaw_SetupHitbox(int32 playerX, int32 tilt)
{
    RSDK_THIS(SeeSaw);

    if (tilt == SEESAW_TILT_M) {
        SeeSaw->hitboxPlank.top   = -8;
        SeeSaw->hitboxPlank.right = 48;
    }
    else {
        int32 distance = CLAMP((((playerX - self->position.x) >> 16) + 48) >> 1, 0, 47);

        if (tilt != SEESAW_TILT_L)
            SeeSaw->hitboxPlank.top = SeeSaw->tiltHeightTable[distance] + 12;
        else
            SeeSaw->hitboxPlank.top = SeeSaw->tiltHeightTable[47 - distance] + 12;
        SeeSaw->hitboxPlank.right = 40;
    }

    SeeSaw->hitboxPlank.bottom = SeeSaw->hitboxPlank.top + 24;
    SeeSaw->hitboxPlank.left   = -SeeSaw->hitboxPlank.right;
}

void SeeSaw_State_OrbIdle(void)
{
    RSDK_THIS(SeeSaw);

    self->orbPos.x       = self->position.x + SeeSaw->orbTargetPos.x;
    self->orbPos.y       = self->position.y + SeeSaw->orbTargetPos.y;
    self->launchVelocity = 0;
}

void SeeSaw_State_None(void)
{
    // do absolutely nothing. not a thing.
}

void SeeSaw_State_NoOrb(void)
{
    // gone :(
}

void SeeSaw_State_OrbLaunched(void)
{
    RSDK_THIS(SeeSaw);

    self->orbPos.x += self->velocity.x;
    self->orbPos.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y > 0) {
        SeeSaw->orbTargetPos.y += self->position.y;

        if (self->orbPos.y >= SeeSaw->orbTargetPos.y) {
            self->orbPos.y = SeeSaw->orbTargetPos.y;
            self->state    = SeeSaw_State_OrbIdle;

            if (self->orbTimer && self->velocity.x)
                self->orbTimer = 0;

            if (self->orbPos.x >= self->position.x) {
                self->targetTilt = SEESAW_TILT_R;
                self->orbSide    = 2;
            }
            else {
                self->targetTilt = SEESAW_TILT_L;
                self->orbSide    = 1;
            }

            self->active = ACTIVE_BOUNDS;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void SeeSaw_EditorDraw(void)
{
    RSDK_THIS(SeeSaw);

    self->orbPos.x = self->position.x;
    self->orbPos.y = self->position.y;

    RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 0, &self->pivotAnimator, true, 0);
    RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 1, &self->plankAnimator, true, 0);
    RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 2, &self->orbAnimator, true, 0);

    SeeSaw_Draw();
}

void SeeSaw_EditorLoad(void)
{
    SeeSaw->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SeeSaw.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SeeSaw, side);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void SeeSaw_Serialize(void) { RSDK_EDITABLE_VAR(SeeSaw, VAR_UINT8, side); }
