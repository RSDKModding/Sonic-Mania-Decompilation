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
        int32 speed = setup->maxSpeed;
        if (!speed)
            speed = 16;

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

    int32 grav           = (self->gravityStrength >> 1) - (self->gravityStrength >> 4);
    self->position.y = grav;
    if (SceneInfo->entitySlot)
        self->position.y += 0xBA0000;
    else
        self->position.y += 0xAA0000;

    if (self->animator.animationID == 1) {
        self->animator.animationTimer += abs(setup->globeSpeed);
        self->animator.animationSpeed = abs(setup->globeSpeed);

        if (self->animator.animationTimer > 0x1F) {
            self->animator.animationTimer &= 0x1F;
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
        self->animator.animationSpeed = maxVal(abs(setup->maxSpeed), 0x10);
        RSDK.ProcessAnimation(&self->animator);
    }

    self->tailAnimator.animationSpeed = abs(setup->maxSpeed) + 40;
    RSDK.ProcessAnimation(&self->tailAnimator);
}

void BSS_Player_LateUpdate(void) {}

void BSS_Player_StaticUpdate(void) {}

void BSS_Player_Draw(void)
{
    RSDK_THIS(BSS_Player);
    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x = ScreenInfo->centerX << 16;
    RSDK.DrawSprite(&self->animator, &drawPos, true);

    uint8 charID = globals->playerID & 0xFF;
    if (charID == ID_TAILS && self->animator.animationID == 1)
        RSDK.DrawSprite(&self->tailAnimator, &drawPos, true);
#if RETRO_USE_PLUS
    else if (charID == ID_RAY && self->animator.animationID == 1)
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
        switch (globals->playerID & 0xFF) {
            case ID_TAILS:
                self->aniFrames = BSS_Player->tailsSpriteIndex;
                RSDK.SetSpriteAnimation(self->aniFrames, 4, &self->tailAnimator, true, 0);
                break;
            case ID_KNUCKLES: self->aniFrames = BSS_Player->knuxSpriteIndex; break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: self->aniFrames = BSS_Player->mightySpriteIndex; break;
            case ID_RAY:
                self->aniFrames = BSS_Player->raySpriteIndex;
                RSDK.SetSpriteAnimation(self->aniFrames, 4, &self->tailAnimator, true, 0);
                break;
#endif
            default: self->aniFrames = BSS_Player->sonicSpriteIndex; break;
        }

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
    BSS_Player->sonicSpriteIndex  = RSDK.LoadSpriteAnimation("SpecialBS/Sonic.bin", SCOPE_STAGE);
    BSS_Player->tailsSpriteIndex  = RSDK.LoadSpriteAnimation("SpecialBS/Tails.bin", SCOPE_STAGE);
    BSS_Player->knuxSpriteIndex   = RSDK.LoadSpriteAnimation("SpecialBS/Knuckles.bin", SCOPE_STAGE);
    BSS_Player->mightySpriteIndex = RSDK.LoadSpriteAnimation("SpecialBS/Mighty.bin", SCOPE_STAGE);
    BSS_Player->raySpriteIndex    = RSDK.LoadSpriteAnimation("SpecialBS/Ray.bin", SCOPE_STAGE);
    if (globals->playerID == ID_NONE)
        globals->playerID = ID_DEFAULT_PLAYER;
    RSDK.ResetEntitySlot(SLOT_PLAYER1, BSS_Player->objectID, NULL);
    BSS_Player->sfxJump = RSDK.GetSfx("Global/Jump.wav");
}

void BSS_Player_ProcessP1Input(void)
{
    RSDK_THIS(BSS_Player);
    if (self->controllerID < PLAYER_MAX) {
#if RETRO_USE_TOUCH_CONTROLS
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
                        EntityPauseMenu *pauseMenu = RSDK.GetEntityByID(SLOT_PAUSEMENU);
                        if (!pauseMenu->objectID) {
                            RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
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
        self->up                  = controller->keyUp.down;
        self->down                = controller->keyDown.down;
        self->left                = controller->keyLeft.down;
        self->right               = controller->keyRight.down;

        self->up |= AnalogStickInfoL[self->controllerID].keyUp.down;
        self->down |= AnalogStickInfoL[self->controllerID].keyDown.down;
        self->left |= AnalogStickInfoL[self->controllerID].keyLeft.down;
        self->right |= AnalogStickInfoL[self->controllerID].keyRight.down;
        self->up |= AnalogStickInfoL[self->controllerID].vDelta > 0.3;
        self->down |= AnalogStickInfoL[self->controllerID].vDelta < -0.3;
        self->left |= AnalogStickInfoL[self->controllerID].hDelta < -0.3;
        self->right |= AnalogStickInfoL[self->controllerID].hDelta > 0.3;

        if (self->left && self->right) {
            self->left  = false;
            self->right = false;
        }
        self->jumpPress = controller->keyA.press || controller->keyB.press || controller->keyC.press || controller->keyX.press;

#if RETRO_USE_PLUS
        if (controller->keyStart.press || UnknownInfo->field_10 == 1) {
#else
        if (controller->keyStart.press) {
#endif

            if (SceneInfo->state == ENGINESTATE_REGULAR) {
                EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                if (!pauseMenu->objectID) {
                    RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
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
    //EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
    BSS_Player->jumpPressState <<= 1;
    BSS_Player->jumpPressState |= self->jumpPress;
    BSS_Player->jumpPressState &= 0xFFFF;

    self->jumpPress = BSS_Player->jumpPressState >> 15;
}

#if RETRO_INCLUDE_EDITOR
void BSS_Player_EditorDraw(void) {}

void BSS_Player_EditorLoad(void) {}
#endif

void BSS_Player_Serialize(void) {}
