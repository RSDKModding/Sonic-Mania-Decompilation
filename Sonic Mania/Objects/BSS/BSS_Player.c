// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BSS_Player Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectBSS_Player *BSS_Player;

void BSS_Player_Update(void)
{
    RSDK_THIS(BSS_Player);

    EntityBSS_Setup *setup = RSDK_GET_ENTITY(SLOT_BSS_SETUP, BSS_Setup);

    StateMachine_Run(self->stateInput);

    if (self->onGround) {
        if (self->jumpPress) {
            self->velocity.y = -0x100000;
            self->onGround   = false;
            RSDK.SetSpriteAnimation(self->aniFrames, 2, &self->animator, false, 0);
            RSDK.PlaySfx(BSS_Player->sfxJump, false, 255);
        }
    }
    else {
        self->gravityStrength += self->velocity.y;

        int32 speed = !setup->maxSpeed ? 16 : setup->maxSpeed;
        self->velocity.y += (speed << 12);
        if (self->gravityStrength >= 0) {
            self->gravityStrength = 0;
            self->onGround        = true;

            if (!self->sidekick) {
                if (self->animator.animationID == 3)
                    setup->globeSpeed >>= 1;

                setup->globeSpeedInc = 2;
            }

            if (setup->maxSpeed)
                RSDK.SetSpriteAnimation(self->aniFrames, 1, &self->animator, false, 0);
            else
                RSDK.SetSpriteAnimation(self->aniFrames, 0, &self->animator, false, 0);
        }
    }

    self->position.y = (self->gravityStrength >> 1) - (self->gravityStrength >> 4);
    self->position.y += SceneInfo->entitySlot ? 0xBA0000 : 0xAA0000;

    if (self->animator.animationID == 1) {
        self->animator.timer += abs(setup->globeSpeed);
        self->animator.speed = abs(setup->globeSpeed);

        if (self->animator.timer > 0x1F) {
            self->animator.timer &= 0x1F;

            if (setup->globeSpeed <= 0) {
                if (--self->animator.frameID < 0)
                    self->animator.frameID = 11;
            }
            else if (++self->animator.frameID > 11) {
                self->animator.frameID = 0;
            }
        }
    }
    else if (self->animator.animationID >= 2) {
        self->animator.speed = maxVal(abs(setup->maxSpeed), 0x10);
        RSDK.ProcessAnimation(&self->animator);
    }

    self->tailAnimator.speed = abs(setup->maxSpeed) + 40;
    RSDK.ProcessAnimation(&self->tailAnimator);
}

void BSS_Player_LateUpdate(void) {}

void BSS_Player_StaticUpdate(void) {}

void BSS_Player_Draw(void)
{
    RSDK_THIS(BSS_Player);

    Vector2 drawPos;
    drawPos.x = ScreenInfo->centerX << 16;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->animator, &drawPos, true);

    // if we're tails, draw his tail
    if (CHECK_CHARACTER_ID(ID_TAILS, 1) && self->animator.animationID == 1)
        RSDK.DrawSprite(&self->tailAnimator, &drawPos, true);
#if MANIA_USE_PLUS
    // if we're ray, draw his tail
    else if (CHECK_CHARACTER_ID(ID_RAY, 1) && self->animator.animationID == 1)
        RSDK.DrawSprite(&self->tailAnimator, &drawPos, true);
#endif
}

void BSS_Player_Create(void *data)
{
    RSDK_THIS(BSS_Player);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_NORMAL;
        self->visible       = true;
        self->drawOrder     = 4;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        switch (GET_CHARACTER_ID(1)) {
            default:
            case ID_SONIC: self->aniFrames = BSS_Player->sonicFrames; break;

            case ID_TAILS:
                self->aniFrames = BSS_Player->tailsFrames;
                RSDK.SetSpriteAnimation(self->aniFrames, 4, &self->tailAnimator, true, 0);
                break;

            case ID_KNUCKLES: self->aniFrames = BSS_Player->knuxFrames; break;

#if MANIA_USE_PLUS
            case ID_MIGHTY: self->aniFrames = BSS_Player->mightyFrames; break;

            case ID_RAY:
                self->aniFrames = BSS_Player->rayFrames;
                RSDK.SetSpriteAnimation(self->aniFrames, 4, &self->tailAnimator, true, 0);
                break;
#endif
        }

        // The BSS Player gets reset into P1 slot, no other player entities ever get set, so sidekick BSS player behaviour goes unused...
        if (SceneInfo->entitySlot) {
            self->stateInput = BSS_Player_ProcessP2Input;
            self->sidekick   = true;
        }
        else {
            self->stateInput   = BSS_Player_ProcessP1Input;
            self->controllerID = CONT_P1;
            self->sidekick     = false;
        }

        RSDK.SetSpriteAnimation(self->aniFrames, 0, &self->animator, true, 0);
    }
}

void BSS_Player_StageLoad(void)
{
    BSS_Player->sonicFrames  = RSDK.LoadSpriteAnimation("SpecialBS/Sonic.bin", SCOPE_STAGE);
    BSS_Player->tailsFrames  = RSDK.LoadSpriteAnimation("SpecialBS/Tails.bin", SCOPE_STAGE);
    BSS_Player->knuxFrames   = RSDK.LoadSpriteAnimation("SpecialBS/Knuckles.bin", SCOPE_STAGE);
    BSS_Player->mightyFrames = RSDK.LoadSpriteAnimation("SpecialBS/Mighty.bin", SCOPE_STAGE);
    BSS_Player->rayFrames    = RSDK.LoadSpriteAnimation("SpecialBS/Ray.bin", SCOPE_STAGE);

    if (globals->playerID == ID_NONE)
        globals->playerID = ID_DEFAULT_PLAYER;

    RSDK.ResetEntitySlot(SLOT_PLAYER1, BSS_Player->classID, NULL);

    BSS_Player->sfxJump = RSDK.GetSfx("Global/Jump.wav");
}

void BSS_Player_ProcessP1Input(void)
{
    RSDK_THIS(BSS_Player);

    if (self->controllerID < PLAYER_MAX) {
#if MANIA_USE_TOUCH_CONTROLS
        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= 0 && ty >= 96 && tx <= ScreenInfo->centerX && ty <= ScreenInfo->height) {
                    int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
                    int32 ty = (TouchInfo->y[t] * ScreenInfo->height);
                    tx -= 64;
                    ty -= 192;

                    switch (((RSDK.ATan2(tx, ty) + 32) & 0xFF) >> 6) {
                        case 0:
                            ControllerInfo->keyRight.down |= true;
                            ControllerInfo[self->controllerID].keyRight.down = true;
                            break;

                        case 1:
                            ControllerInfo->keyDown.down |= true;
                            ControllerInfo[self->controllerID].keyDown.down = true;
                            break;

                        case 2:
                            ControllerInfo->keyLeft.down |= true;
                            ControllerInfo[self->controllerID].keyLeft.down = true;
                            break;

                        case 3:
                            ControllerInfo->keyUp.down |= true;
                            ControllerInfo[self->controllerID].keyUp.down = true;
                            break;
                    }
                    break;
                }
            }
        }

        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= ScreenInfo->centerX && ty >= 96 && tx <= ScreenInfo->width && ty <= ScreenInfo->height) {
                    ControllerInfo->keyA.down |= true;
                    ControllerInfo[self->controllerID].keyA.down = true;
                    break;
                }
            }
        }

        if (!self->touchJump) {
            ControllerInfo->keyA.press |= ControllerInfo->keyA.down;
            ControllerInfo[self->controllerID].keyA.press |= ControllerInfo[self->controllerID].keyA.down;
        }
        self->touchJump = ControllerInfo[self->controllerID].keyA.down;

        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= ScreenInfo->width - 0x80 && ty >= 0 && tx <= ScreenInfo->width && ty <= 0x40) {
                    if (SceneInfo->state == ENGINESTATE_REGULAR) {
                        EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                        if (!pauseMenu->classID) {
                            RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
                            pauseMenu->triggerPlayer  = RSDK.GetEntityID(self);
                            pauseMenu->disableRestart = true;
                        }
                    }
                    break;
                }
            }
        }
#endif

        RSDKControllerState *controller = &ControllerInfo[self->controllerID];
        self->up                        = controller->keyUp.down;
        self->down                      = controller->keyDown.down;
        self->left                      = controller->keyLeft.down;
        self->right                     = controller->keyRight.down;

        self->up |= AnalogStickInfoL[self->controllerID].keyUp.down;
        self->down |= AnalogStickInfoL[self->controllerID].keyDown.down;
        self->left |= AnalogStickInfoL[self->controllerID].keyLeft.down;
        self->right |= AnalogStickInfoL[self->controllerID].keyRight.down;

#if MANIA_USE_PLUS
        self->up |= AnalogStickInfoL[self->controllerID].vDelta > 0.3;
        self->down |= AnalogStickInfoL[self->controllerID].vDelta < -0.3;
        self->left |= AnalogStickInfoL[self->controllerID].hDelta < -0.3;
        self->right |= AnalogStickInfoL[self->controllerID].hDelta > 0.3;
#else
        self->up |= AnalogStickInfoL[self->controllerID].vDeltaL > 0.3;
        self->down |= AnalogStickInfoL[self->controllerID].vDeltaL < -0.3;
        self->left |= AnalogStickInfoL[self->controllerID].hDeltaL < -0.3;
        self->right |= AnalogStickInfoL[self->controllerID].hDeltaL > 0.3;
#endif

        if (self->left && self->right) {
            self->left  = false;
            self->right = false;
        }
        self->jumpPress = controller->keyA.press || controller->keyB.press || controller->keyC.press || controller->keyX.press;

        if (controller->keyStart.press || Unknown_pausePress) {
            if (SceneInfo->state == ENGINESTATE_REGULAR) {
                EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                if (!pauseMenu->classID) {
                    RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
                    pauseMenu->triggerPlayer  = RSDK.GetEntityID(self);
                    pauseMenu->disableRestart = true;
                }
            }
        }
    }
}

void BSS_Player_ProcessP2Input(void)
{
    RSDK_THIS(BSS_Player);

    // EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
    BSS_Player->jumpPressState <<= 1;
    BSS_Player->jumpPressState |= self->jumpPress;
    BSS_Player->jumpPressState &= 0xFFFF;

    self->jumpPress = BSS_Player->jumpPressState >> 15;
}

#if RETRO_INCLUDE_EDITOR
void BSS_Player_EditorDraw(void)
{
    RSDK_THIS(BSS_Player);

    RSDK.SetSpriteAnimation(BSS_Player->sonicFrames, 0, &self->animator, false, 0);

    RSDK.DrawSprite(&self->animator, NULL, true);
}

void BSS_Player_EditorLoad(void) { BSS_Player->sonicFrames = RSDK.LoadSpriteAnimation("SpecialBS/Sonic.bin", SCOPE_STAGE); }
#endif

void BSS_Player_Serialize(void) {}
