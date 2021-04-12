#include "../SonicMania.h"

ObjectBSS_Player *BSS_Player;

void BSS_Player_Update(void)
{
    RSDK_THIS(BSS_Player);
    EntityBSS_Setup *setup = RSDK_GET_ENTITY(SLOT_BSS_SETUP, BSS_Setup);

    StateMachine_Run(entity->inputState);
    if (entity->onGround) {
        if (entity->jumpPress) {
            entity->velocity.y = -0x100000;
            entity->onGround   = 0;
            RSDK.SetSpriteAnimation(entity->spriteIndex, 2, &entity->playerData, 0, 0);
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
            entity->onGround        = 1;
            if (!entity->sideKick) {
                if (entity->playerData.animationID == 3)
                    setup->globeSpeed >>= 1;
                setup->field_8C = 2;
            }

            if (setup->maxSpeed)
                RSDK.SetSpriteAnimation(entity->spriteIndex, 1, &entity->playerData, 0, 0);
            else
                RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->playerData, 0, 0);
        }
    }

    int grav           = (entity->gravityStrength >> 1) - (entity->gravityStrength >> 4);
    entity->position.y = grav;
    if (RSDK_sceneInfo->entitySlot)
        entity->position.y += 0xBA0000;
    else
        entity->position.y += 0xAA0000;

    if (entity->playerData.animationID == 1) {
        entity->playerData.animationTimer += abs(setup->globeSpeed);
        entity->playerData.animationSpeed = abs(setup->globeSpeed);

        if (entity->playerData.animationTimer > 0x1F) {
            entity->playerData.animationTimer &= 0x1F;
            if (setup->globeSpeed <= 0) {
                if (!--entity->playerData.frameID)
                    entity->playerData.frameID = 11;
            }
            else if (++entity->playerData.frameID > 11) {
                entity->playerData.frameID = 0;
            }
        }
    }
    else if (entity->playerData.animationID >= 2) {
        entity->playerData.animationSpeed = maxVal(abs(setup->maxSpeed), 0x10);
        RSDK.ProcessAnimation(&entity->playerData);
    }

    entity->tailData.animationSpeed = abs(setup->maxSpeed) + 40;
    RSDK.ProcessAnimation(&entity->tailData);
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
    RSDK.DrawSprite(&entity->playerData, &drawPos, true);

    byte charID = globals->playerID & 0xFF;
    if (charID == ID_TAILS && entity->playerData.animationID == 1)
        RSDK.DrawSprite(&entity->tailData, &drawPos, true);
#if RETRO_USE_PLUS
    else if (charID == ID_RAY && entity->playerData.animationID == 1)
        RSDK.DrawSprite(&entity->tailData, &drawPos, true);
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
                RSDK.SetSpriteAnimation(entity->spriteIndex, 4, &entity->tailData, true, 0);
                break;
            case ID_KNUCKLES: entity->spriteIndex = BSS_Player->knuxSpriteIndex; break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: entity->spriteIndex = BSS_Player->mightySpriteIndex; break;
            case ID_RAY:
                entity->spriteIndex = BSS_Player->raySpriteIndex;
                RSDK.SetSpriteAnimation(entity->spriteIndex, 4, &entity->tailData, true, 0);
                break;
#endif
            default: entity->spriteIndex = BSS_Player->sonicSpriteIndex; break;
        }

        if (RSDK_sceneInfo->entitySlot) {
            entity->inputState = BSS_Player_HandleP2Inputs;
            entity->sideKick   = true;
        }
        else {
            entity->inputState = BSS_Player_HandleP1Inputs;
            entity->controllerID = CONT_P1;
            entity->sideKick   = false;
        }
        RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->playerData, true, 0);
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
    RSDK.ResetEntitySlot(SLOT_PLAYER1, BSS_Player->objectID, 0);
    BSS_Player->sfx_Jump = RSDK.GetSFX("Global/Jump.wav");
}

void BSS_Player_HandleP1Inputs(void)
{
    RSDK_THIS(BSS_Player);
    if (entity->controllerID < PLAYER_MAX) {
        ControllerState *controller = &RSDK_controller[entity->controllerID];
        entity->up                  = controller->keyUp.down;
        entity->down                = controller->keyDown.down;
        entity->left                = controller->keyLeft.down;
        entity->right               = controller->keyRight.down;

        entity->up |= RSDK_stickL[entity->controllerID].keyUp.down;
        entity->down |= RSDK_stickL[entity->controllerID].keyDown.down;
        entity->left |= RSDK_stickL[entity->controllerID].keyLeft.down;
        entity->right |= RSDK_stickL[entity->controllerID].keyRight.down;
        if (entity->left && entity->right) {
            entity->left  = false;
            entity->right = false;
        }
        entity->jumpPress = controller->keyA.press | controller->keyB.press | controller->keyC.press | controller->keyX.press;
        //entity->jumpHold  = controller->keyA.down | controller->keyB.down | controller->keyC.down | controller->keyX.down;

#if RETRO_USE_PLUS
        if (RSDK_controller[entity->controllerID].keyStart.press || RSDK_unknown->field_10 == 1) {
#else
        if (RSDK_controller[entity->controllerID].keyStart.press) {
#endif

            if (RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
                Entity *pauseMenu = RSDK.GetEntityByID(SLOT_PAUSEMENU);
                if (!pauseMenu->objectID) {
                    RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, 0);
                    // pauseMenu->field_7C = entity->playerID;
                    // pauseMenu->field_80 = true;
                }
            }
        }
    }
}

void BSS_Player_HandleP2Inputs(void)
{
    RSDK_THIS(BSS_Player);
    RSDK.GetEntityByID(SLOT_PLAYER1);
    BSS_Player->jumpPressState *= 2;
    BSS_Player->jumpPressState |= entity->jumpPress;
    entity->jumpPress = BSS_Player->jumpPressState >> 15;
}

void BSS_Player_EditorDraw(void) {}

void BSS_Player_EditorLoad(void) {}

void BSS_Player_Serialize(void) {}
