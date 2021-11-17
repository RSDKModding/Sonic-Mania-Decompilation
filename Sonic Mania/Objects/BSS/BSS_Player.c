#include "SonicMania.h"

ObjectBSS_Player *BSS_Player;

void BSS_Player_Update(void)
{
    RSDK_THIS(BSS_Player);
    EntityBSS_Setup *setup = RSDK_GET_ENTITY(SLOT_BSS_SETUP, BSS_Setup);

    StateMachine_Run(entity->stateInput);
    if (entity->onGround) {
        if (entity->jumpPress) {
            entity->velocity.y = -0x100000;
            entity->onGround   = false;
            RSDK.SetSpriteAnimation(entity->aniFrames, 2, &entity->playerAnimator, false, 0);
            RSDK.PlaySfx(BSS_Player->sfxJump, 0, 255);
        }
    }
    else {
        entity->gravityStrength += entity->velocity.y;
        int32 speed = setup->maxSpeed;
        if (!speed)
            speed = 16;

        entity->velocity.y += (speed << 12);
        if (entity->gravityStrength >= 0) {
            entity->gravityStrength = 0;
            entity->onGround        = true;
            if (!entity->sideKick) {
                if (entity->playerAnimator.animationID == 3)
                    setup->globeSpeed >>= 1;
                setup->globeSpeedInc = 2;
            }

            if (setup->maxSpeed)
                RSDK.SetSpriteAnimation(entity->aniFrames, 1, &entity->playerAnimator, false, 0);
            else
                RSDK.SetSpriteAnimation(entity->aniFrames, 0, &entity->playerAnimator, false, 0);
        }
    }

    int32 grav           = (entity->gravityStrength >> 1) - (entity->gravityStrength >> 4);
    entity->position.y = grav;
    if (SceneInfo->entitySlot)
        entity->position.y += 0xBA0000;
    else
        entity->position.y += 0xAA0000;

    if (entity->playerAnimator.animationID == 1) {
        entity->playerAnimator.animationTimer += abs(setup->globeSpeed);
        entity->playerAnimator.animationSpeed = abs(setup->globeSpeed);

        if (entity->playerAnimator.animationTimer > 0x1F) {
            entity->playerAnimator.animationTimer &= 0x1F;
            if (setup->globeSpeed <= 0) {
                if (--entity->playerAnimator.frameID < 0)
                    entity->playerAnimator.frameID = 11;
            }
            else if (++entity->playerAnimator.frameID > 11) {
                entity->playerAnimator.frameID = 0;
            }
        }
    }
    else if (entity->playerAnimator.animationID >= 2) {
        entity->playerAnimator.animationSpeed = maxVal(abs(setup->maxSpeed), 0x10);
        RSDK.ProcessAnimation(&entity->playerAnimator);
    }

    entity->tailAnimator.animationSpeed = abs(setup->maxSpeed) + 40;
    RSDK.ProcessAnimation(&entity->tailAnimator);
}

void BSS_Player_LateUpdate(void) {}

void BSS_Player_StaticUpdate(void) {}

void BSS_Player_Draw(void)
{
    RSDK_THIS(BSS_Player);
    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x = ScreenInfo->centerX << 16;
    RSDK.DrawSprite(&entity->playerAnimator, &drawPos, true);

    uint8 charID = globals->playerID & 0xFF;
    if (charID == ID_TAILS && entity->playerAnimator.animationID == 1)
        RSDK.DrawSprite(&entity->tailAnimator, &drawPos, true);
#if RETRO_USE_PLUS
    else if (charID == ID_RAY && entity->playerAnimator.animationID == 1)
        RSDK.DrawSprite(&entity->tailAnimator, &drawPos, true);
#endif
}

void BSS_Player_Create(void *data)
{
    RSDK_THIS(BSS_Player);
    if (!SceneInfo->inEditor) {
        entity->active        = ACTIVE_NORMAL;
        entity->visible       = true;
        entity->drawOrder     = 4;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        switch (globals->playerID & 0xFF) {
            case ID_TAILS:
                entity->aniFrames = BSS_Player->tailsSpriteIndex;
                RSDK.SetSpriteAnimation(entity->aniFrames, 4, &entity->tailAnimator, true, 0);
                break;
            case ID_KNUCKLES: entity->aniFrames = BSS_Player->knuxSpriteIndex; break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: entity->aniFrames = BSS_Player->mightySpriteIndex; break;
            case ID_RAY:
                entity->aniFrames = BSS_Player->raySpriteIndex;
                RSDK.SetSpriteAnimation(entity->aniFrames, 4, &entity->tailAnimator, true, 0);
                break;
#endif
            default: entity->aniFrames = BSS_Player->sonicSpriteIndex; break;
        }

        if (SceneInfo->entitySlot) {
            entity->stateInput = BSS_Player_ProcessP2Input;
            entity->sideKick   = true;
        }
        else {
            entity->stateInput   = BSS_Player_ProcessP1Input;
            entity->controllerID = CONT_P1;
            entity->sideKick     = false;
        }
        RSDK.SetSpriteAnimation(entity->aniFrames, 0, &entity->playerAnimator, true, 0);
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
    BSS_Player->sfxJump = RSDK.GetSFX("Global/Jump.wav");
}

void BSS_Player_ProcessP1Input(void)
{
    RSDK_THIS(BSS_Player);
    if (entity->controllerID < PLAYER_MAX) {
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
                            ControllerInfo[entity->controllerID].keyRight.down = true;
                            break;
                        case 1:
                            ControllerInfo->keyDown.down |= true;
                            ControllerInfo[entity->controllerID].keyDown.down = true;
                            break;
                        case 2:
                            ControllerInfo->keyLeft.down |= true;
                            ControllerInfo[entity->controllerID].keyLeft.down = true;
                            break;
                        case 3:
                            ControllerInfo->keyUp.down |= true;
                            ControllerInfo[entity->controllerID].keyUp.down = true;
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
                    ControllerInfo[entity->controllerID].keyA.down = true;
                    break;
                }
            }
        }

        if (!entity->touchJump) {
            ControllerInfo->keyA.press |= ControllerInfo->keyA.down;
            ControllerInfo[entity->controllerID].keyA.press |= ControllerInfo[entity->controllerID].keyA.down;
        }
        entity->touchJump = ControllerInfo[entity->controllerID].keyA.down;

        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= ScreenInfo->width - 0x80 && ty >= 0 && tx <= ScreenInfo->width && ty <= 0x40) {
                    if (SceneInfo->state == ENGINESTATE_REGULAR) {
                        EntityPauseMenu *pauseMenu = RSDK.GetEntityByID(SLOT_PAUSEMENU);
                        if (!pauseMenu->objectID) {
                            RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                            pauseMenu->triggerPlayer  = RSDK.GetEntityID(entity);
                            pauseMenu->disableRestart = true;
                        }
                    }
                    break;
                }
            }
        }
#endif

        RSDKControllerState *controller = &ControllerInfo[entity->controllerID];
        entity->up                  = controller->keyUp.down;
        entity->down                = controller->keyDown.down;
        entity->left                = controller->keyLeft.down;
        entity->right               = controller->keyRight.down;

        entity->up |= AnalogStickInfoL[entity->controllerID].keyUp.down;
        entity->down |= AnalogStickInfoL[entity->controllerID].keyDown.down;
        entity->left |= AnalogStickInfoL[entity->controllerID].keyLeft.down;
        entity->right |= AnalogStickInfoL[entity->controllerID].keyRight.down;
        entity->up |= AnalogStickInfoL[entity->controllerID].vDelta > 0.3;
        entity->down |= AnalogStickInfoL[entity->controllerID].vDelta < -0.3;
        entity->left |= AnalogStickInfoL[entity->controllerID].hDelta < -0.3;
        entity->right |= AnalogStickInfoL[entity->controllerID].hDelta > 0.3;

        if (entity->left && entity->right) {
            entity->left  = false;
            entity->right = false;
        }
        entity->jumpPress = controller->keyA.press || controller->keyB.press || controller->keyC.press || controller->keyX.press;

#if RETRO_USE_PLUS
        if (controller->keyStart.press || UnknownInfo->field_10 == 1) {
#else
        if (controller->keyStart.press) {
#endif

            if (SceneInfo->state == ENGINESTATE_REGULAR) {
                EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                if (!pauseMenu->objectID) {
                    RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                    pauseMenu->triggerPlayer  = RSDK.GetEntityID(entity);
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
    BSS_Player->jumpPressState |= entity->jumpPress;
    BSS_Player->jumpPressState &= 0xFFFF;

    entity->jumpPress = BSS_Player->jumpPressState >> 15;
}

#if RETRO_INCLUDE_EDITOR
void BSS_Player_EditorDraw(void) {}

void BSS_Player_EditorLoad(void) {}
#endif

void BSS_Player_Serialize(void) {}
