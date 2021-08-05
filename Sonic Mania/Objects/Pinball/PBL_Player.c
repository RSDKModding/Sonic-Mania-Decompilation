#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Player *PBL_Player;

void PBL_Player_Update(void)
{
    RSDK_THIS(PBL_Player);
    StateMachine_Run(entity->stateInput);
    StateMachine_Run(entity->state);

    entity->angleX   = ((ushort)entity->angleX - (ushort)((abs(entity->velocity.y) + abs(entity->velocity.x)) >> 12)) & 0x3FF;
    entity->rotation = 4 * RSDK.ATan2(-entity->velocity.y, entity->velocity.x);
    if (!(entity->angleX & 0x100))
        entity->animator.animationID = PBL_Player->jumpModel;
    else
        entity->animator.animationID = PBL_Player->ballModel;
    RSDK.ProcessAnimation(&entity->animator);
}

void PBL_Player_LateUpdate(void)
{
    RSDK_THIS(PBL_Player);
    Matrix *mat = &PBL_Camera->matrix1;

    
    entity->depth = mat->values[2][3] + mat->values[2][0] * (entity->position.x >> 0x10) + mat->values[2][2] * (entity->position.y >> 0x10)
                    + mat->values[2][1] * (entity->height >> 0x10);
}

void PBL_Player_StaticUpdate(void) {}

void PBL_Player_Draw(void)
{
    RSDK_THIS(PBL_Player);
    if (entity->depth >= 0x4000) {
        RSDK.Prepare3DScene(PBL_Player->sceneIndex);
        RSDK.MatrixTranslateXYZ(&entity->matrix2, entity->position.x, entity->height + 0x100000, entity->position.y, true);
        RSDK.MatrixRotateX(&entity->matrix1, entity->angleX);
        RSDK.MatrixRotateY(&entity->matrix3, entity->rotation);
        RSDK.MatrixMultiply(&entity->matrix4, &entity->matrix1, &entity->matrix3);
        RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix4, &entity->matrix2);
        RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &PBL_Camera->matrix1);
        RSDK.MatrixMultiply(&entity->matrix4, &entity->matrix4, &PBL_Camera->matrix2);
        RSDK.AddModelTo3DScene(entity->animator.animationID, PBL_Player->sceneIndex, S3D_FLATCLR_SHADED_BLENDED_SCREEN, &entity->matrix3,
                               &entity->matrix4, 0xFFFFFF);
        RSDK.Draw3DScene(PBL_Player->sceneIndex);
    }
}

void PBL_Player_Create(void *data)
{
    RSDK_THIS(PBL_Player);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active          = ACTIVE_NORMAL;
        entity->visible         = true;
        entity->updateRange.x   = 0x800000;
        entity->updateRange.y   = 0x800000;
        entity->drawOrder       = 4;
        entity->stateInput      = PBL_Player_ProcessPlayerControl;
        entity->state           = PBL_Player_State_Launcher;
        entity->controllerID    = 1;
        entity->onGround        = false;
        entity->tileCollisions  = true;
        entity->collisionLayers = 1 << PBL_Setup->tableHigh;
        RSDK.SetModelAnimation(PBL_Player->jumpModel, &entity->animator, 128, 0, true, 0);

        foreach_all(PBL_Camera, camera) { camera->targetPtr = (Entity *)entity; }
    }
}

void PBL_Player_StageLoad(void)
{
    if (globals->playerID == ID_NONE) {
        globals->playerID       = ID_DEFAULT_PLAYER;
        globals->characterFlags = ID_SONIC;
    }

    switch (globals->playerID & 0xFF) {
        case ID_TAILS:
            PBL_Player->jumpModel       = RSDK.LoadMesh("Special/TailsJump.bin", SCOPE_STAGE);
            PBL_Player->ballModel       = RSDK.LoadMesh("Special/TailsBall.bin", SCOPE_STAGE);
            PBL_Player->outerBox.top    = -10;
            PBL_Player->outerBox.bottom = 10;
            PBL_Player->innerBox.top    = -10;
            PBL_Player->innerBox.bottom = 10;
            break;
        case ID_KNUCKLES:
            PBL_Player->jumpModel       = RSDK.LoadMesh("Special/KnuxJump.bin", SCOPE_STAGE);
            PBL_Player->ballModel       = RSDK.LoadMesh("Special/KnuxBall.bin", SCOPE_STAGE);
            PBL_Player->outerBox.top    = -12;
            PBL_Player->outerBox.bottom = 12;
            PBL_Player->innerBox.top    = -12;
            PBL_Player->innerBox.bottom = 12;
            break;
        case ID_MIGHTY:
            PBL_Player->jumpModel       = RSDK.LoadMesh("Special/MightyJump.bin", SCOPE_STAGE);
            PBL_Player->ballModel       = RSDK.LoadMesh("Special/MightyBall.bin", SCOPE_STAGE);
            PBL_Player->outerBox.top    = -12;
            PBL_Player->outerBox.bottom = 12;
            PBL_Player->innerBox.top    = -12;
            PBL_Player->innerBox.bottom = 12;
            break;
        case ID_RAY:
            PBL_Player->jumpModel       = RSDK.LoadMesh("Special/RayJump.bin", SCOPE_STAGE);
            PBL_Player->ballModel       = RSDK.LoadMesh("Special/RayBall.bin", SCOPE_STAGE);
            PBL_Player->outerBox.top    = -10;
            PBL_Player->outerBox.bottom = 10;
            PBL_Player->innerBox.top    = -10;
            PBL_Player->innerBox.bottom = 10;
            break;
        default:
            PBL_Player->jumpModel       = RSDK.LoadMesh("Special/SonicJump.bin", SCOPE_STAGE);
            PBL_Player->ballModel       = RSDK.LoadMesh("Special/SonicBall.bin", SCOPE_STAGE);
            PBL_Player->outerBox.top    = -12;
            PBL_Player->outerBox.bottom = 12;
            PBL_Player->innerBox.top    = -12;
            PBL_Player->innerBox.bottom = 12;
            break;
    }
    PBL_Player->outerBox.left  = -10;
    PBL_Player->outerBox.right = 10;
    PBL_Player->innerBox.left  = -10;
    PBL_Player->innerBox.right = 10;

    PBL_Player->sceneIndex = RSDK.Create3DScene("View:Pinball", 4096, SCOPE_STAGE);
    RSDK.SetDiffuseColour(PBL_Player->sceneIndex, 160, 160, 160);
    RSDK.SetDiffuseIntensity(PBL_Player->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularIntensity(PBL_Player->sceneIndex, 15, 15, 15);

    PBL_Player->sfxPlunger = RSDK.GetSFX("Pinball/Plunger.wav");
}

void PBL_Player_ProcessPlayerControl(void)
{
    RSDK_THIS(PBL_Player);
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

void PBL_Player_State_Launcher(void)
{
    RSDK_THIS(PBL_Player);
    if (!entity->onGround)
        entity->velocity.y += 0x3800;

    RSDK.ProcessTileCollisions(entity, &PBL_Player->outerBox, &PBL_Player->innerBox);
    if (++entity->timer == 60) {
        entity->velocity.y     = -0xE0000;
        entity->timer          = 0;
        entity->collisionPlane = 1;
        entity->onGround       = false;
        entity->state          = PBL_Player_State_Air;
        RSDK.PlaySFX(PBL_Player->sfxPlunger, 0, 255);
    }
}

void PBL_Player_State_Ground(void)
{
    RSDK_THIS(PBL_Player);
    if (!entity->angle) {
        if (abs(entity->groundVel) < 0x10000)
            entity->groundVel = RSDK.Rand(-0x20000, 0x20000);
    }

    entity->groundVel += 0x2800 * RSDK.Sin256(entity->angle) >> 8;
    if (entity->collisionMode) {
        if (entity->angle >= 0x40 && entity->angle <= 0xC0 && abs(entity->groundVel) < 0x20000) {
            entity->velocity.x    = (entity->groundVel * RSDK.Cos256(entity->angle)) >> 8;
            entity->velocity.y    = (entity->groundVel * RSDK.Sin256(entity->angle)) >> 8;
            entity->onGround      = false;
            entity->angle         = 0;
            entity->collisionMode = CMODE_FLOOR;
        }
    }
    if (!entity->onGround)
        entity->state = PBL_Player_State_Air;
    RSDK.ProcessTileCollisions(entity, &PBL_Player->outerBox, &PBL_Player->innerBox);
}

void PBL_Player_State_Air(void)
{
    RSDK_THIS(PBL_Player);
    entity->velocity.y += 0x2800;
    if (abs(entity->velocity.x) > abs(entity->velocity.y)) {
        if (RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_LWALL, entity->collisionPlane, entity->velocity.x + 0x100000, 0, false)) {
            entity->velocity.x = -(entity->velocity.x >> 1);
        }
        if (RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_RWALL, entity->collisionPlane, entity->velocity.x - 0x100000, 0, false)) {
            entity->velocity.x = -(entity->velocity.x >> 1);
        }
    }

    RSDK.ProcessTileCollisions(entity, &PBL_Player->outerBox, &PBL_Player->innerBox);
    if (entity->left) {
        entity->velocity.x -= 0x800;
    }
    else if (entity->right) {
        entity->velocity.x += 0x800;
    }
    if (entity->onGround) {
        if (!entity->collisionMode)
            entity->collisionPlane = 0;
        entity->state = PBL_Player_State_Ground;
    }
}

void PBL_Player_EditorDraw(void) {}

void PBL_Player_EditorLoad(void) {}

void PBL_Player_Serialize(void) {}
#endif
