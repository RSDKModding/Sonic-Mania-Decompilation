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
            RSDK.SetSpriteAnimation(entity->spriteIndex, 2, &entity->playerAnimator, false, 0);
            RSDK.PlaySFX(BSS_Player->sfx_Jump, 0, 255);
        }
    }
    else {
        entity->gravityStrength += entity->velocity.y;
        int speed = setup->maxSpeed;
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
                RSDK.SetSpriteAnimation(entity->spriteIndex, 1, &entity->playerAnimator, false, 0);
            else
                RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->playerAnimator, false, 0);
        }
    }

    int grav           = (entity->gravityStrength >> 1) - (entity->gravityStrength >> 4);
    entity->position.y = grav;
    if (RSDK_sceneInfo->entitySlot)
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
    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x = RSDK_screens->centerX << 16;
    RSDK.DrawSprite(&entity->playerAnimator, &drawPos, true);

    byte charID = globals->playerID & 0xFF;
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
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_NORMAL;
        entity->visible       = true;
        entity->drawOrder     = 4;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        switch (globals->playerID & 0xFF) {
            case ID_TAILS:
                entity->spriteIndex = BSS_Player->tailsSpriteIndex;
                RSDK.SetSpriteAnimation(entity->spriteIndex, 4, &entity->tailAnimator, true, 0);
                break;
            case ID_KNUCKLES: entity->spriteIndex = BSS_Player->knuxSpriteIndex; break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: entity->spriteIndex = BSS_Player->mightySpriteIndex; break;
            case ID_RAY:
                entity->spriteIndex = BSS_Player->raySpriteIndex;
                RSDK.SetSpriteAnimation(entity->spriteIndex, 4, &entity->tailAnimator, true, 0);
                break;
#endif
            default: entity->spriteIndex = BSS_Player->sonicSpriteIndex; break;
        }

        if (RSDK_sceneInfo->entitySlot) {
            entity->stateInput = BSS_Player_ProcessP2Input;
            entity->sideKick   = true;
        }
        else {
            entity->stateInput   = BSS_Player_ProcessP1Input;
            entity->controllerID = CONT_P1;
            entity->sideKick     = false;
        }
        RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->playerAnimator, true, 0);
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
    BSS_Player->sfx_Jump = RSDK.GetSFX("Global/Jump.wav");
}

void BSS_Player_ProcessP1Input(void)
{
    RSDK_THIS(BSS_Player);
    if (entity->controllerID < PLAYER_MAX) {
#if RETRO_USE_TOUCH_CONTROLS
        for (int t = 0; t < RSDK_touchMouse->count; ++t) {
            int tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
            int ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);

            if (RSDK_touchMouse->down[t]) {
                if (tx >= 0 && ty >= 96 && tx <= RSDK_screens->centerX && ty <= RSDK_screens->height) {
                    int tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
                    int ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);
                    tx -= 64;
                    ty -= 192;

                    switch (((RSDK.ATan2(tx, ty) + 32) & 0xFF) >> 6) {
                        case 0:
                            entity->upState   = 0;
                            entity->downState = 0;
                            entity->leftState = 0;
                            ++entity->rightState;
                            RSDK_controller->keyRight.press   = entity->rightState == 1;
                            RSDK_controller[1].keyRight.press = entity->rightState == 1;
                            RSDK_controller->keyRight.down    = entity->rightState != 0;
                            RSDK_controller[1].keyRight.down  = entity->rightState != 0;
                            break;
                        case 1:
                            entity->upState = 0;
                            ++entity->downState;
                            entity->leftState                = 0;
                            entity->rightState               = 0;
                            RSDK_controller->keyDown.press   = entity->downState == 1;
                            RSDK_controller[1].keyDown.press = entity->downState == 1;
                            RSDK_controller->keyDown.down    = entity->downState != 0;
                            RSDK_controller[1].keyDown.down  = entity->downState != 0;
                            break;
                        case 2:
                            entity->upState   = 0;
                            entity->downState = 0;
                            ++entity->leftState;
                            entity->rightState               = 0;
                            RSDK_controller->keyLeft.press   = entity->leftState == 1;
                            RSDK_controller[1].keyLeft.press = entity->leftState == 1;
                            RSDK_controller->keyLeft.down    = entity->leftState != 0;
                            RSDK_controller[1].keyLeft.down  = entity->leftState != 0;
                            break;
                        case 3:
                            ++entity->upState;
                            entity->downState              = 0;
                            entity->leftState              = 0;
                            entity->rightState             = 0;
                            RSDK_controller->keyUp.press   = entity->upState == 1;
                            RSDK_controller[1].keyUp.press = entity->upState == 1;
                            RSDK_controller->keyUp.down    = entity->upState != 0;
                            RSDK_controller[1].keyUp.down  = entity->upState != 0;
                            break;
                    }
                }
                break;
            }
        }

        bool32 jumpFlag = false;
        for (int t = 0; t < RSDK_touchMouse->count; ++t) {
            int tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
            int ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);

            if (RSDK_touchMouse->down[t]) {
                if (tx >= RSDK_screens->centerX && ty >= 96 && tx <= RSDK_screens->width && ty <= RSDK_screens->height) {
                    entity->jumpState++;
                    RSDK_controller->keyA.press   = entity->jumpState == 1;
                    RSDK_controller[1].keyA.press = entity->jumpState == 1;
                    RSDK_controller->keyA.down    = entity->jumpState != 0;
                    RSDK_controller[1].keyA.down  = entity->jumpState != 0;
                    jumpFlag                      = true;
                    break;
                }
            }
        }

        if (!jumpFlag) {
            entity->jumpState = 0;
        }

        for (int t = 0; t < RSDK_touchMouse->count; ++t) {
            int tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
            int ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);

            if (RSDK_touchMouse->down[t]) {
                if (tx >= RSDK_screens->width - 0x80 && ty >= 0 && tx <= RSDK_screens->width && ty <= 0x40) {
                    if (RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
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

        ControllerState *controller = &RSDK_controller[entity->controllerID];
        entity->up                  = controller->keyUp.down;
        entity->down                = controller->keyDown.down;
        entity->left                = controller->keyLeft.down;
        entity->right               = controller->keyRight.down;

        entity->up |= RSDK_stickL[entity->controllerID].keyUp.down;
        entity->down |= RSDK_stickL[entity->controllerID].keyDown.down;
        entity->left |= RSDK_stickL[entity->controllerID].keyLeft.down;
        entity->right |= RSDK_stickL[entity->controllerID].keyRight.down;
        entity->up |= RSDK_stickL[entity->controllerID].vDelta > 0.3;
        entity->down |= RSDK_stickL[entity->controllerID].vDelta < -0.3;
        entity->left |= RSDK_stickL[entity->controllerID].hDelta < -0.3;
        entity->right |= RSDK_stickL[entity->controllerID].hDelta > 0.3;

        if (entity->left && entity->right) {
            entity->left  = false;
            entity->right = false;
        }
        entity->jumpPress = controller->keyA.press || controller->keyB.press || controller->keyC.press || controller->keyX.press;

#if RETRO_USE_PLUS
        if (RSDK_controller[entity->controllerID].keyStart.press || RSDK_unknown->field_10 == 1) {
#else
        if (RSDK_controller[entity->controllerID].keyStart.press) {
#endif

            if (RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
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
    EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
    BSS_Player->jumpPressState <<= 1;
    BSS_Player->jumpPressState |= entity->jumpPress;
    BSS_Player->jumpPressState &= 0xFFFF;

    entity->jumpPress = BSS_Player->jumpPressState >> 15;
}

void BSS_Player_EditorDraw(void) {}

void BSS_Player_EditorLoad(void) {}

void BSS_Player_Serialize(void) {}
