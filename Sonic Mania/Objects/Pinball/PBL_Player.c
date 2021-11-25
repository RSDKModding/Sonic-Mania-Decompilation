#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Player *PBL_Player;

void PBL_Player_Update(void)
{
    RSDK_THIS(PBL_Player);
    StateMachine_Run(self->stateInput);
    StateMachine_Run(self->state);

    self->angleX   = ((uint16)self->angleX - (uint16)((abs(self->velocity.y) + abs(self->velocity.x)) >> 12)) & 0x3FF;
    self->rotation = 4 * RSDK.ATan2(-self->velocity.y, self->velocity.x);
    if (!(self->angleX & 0x100))
        self->animator.animationID = PBL_Player->jumpFrames;
    else
        self->animator.animationID = PBL_Player->ballFrames;
    RSDK.ProcessAnimation(&self->animator);
}

void PBL_Player_LateUpdate(void)
{
    RSDK_THIS(PBL_Player);
    Matrix *mat = &PBL_Camera->matWorld;

    
    self->depth3D = mat->values[2][3] + mat->values[2][0] * (self->position.x >> 0x10) + mat->values[2][2] * (self->position.y >> 0x10)
                    + mat->values[2][1] * (self->height >> 0x10);
}

void PBL_Player_StaticUpdate(void) {}

void PBL_Player_Draw(void)
{
    RSDK_THIS(PBL_Player);
    if (self->depth3D >= 0x4000) {
        RSDK.Prepare3DScene(PBL_Player->sceneIndex);
        RSDK.MatrixTranslateXYZ(&self->matrix2, self->position.x, self->height + 0x100000, self->position.y, true);
        RSDK.MatrixRotateX(&self->matrix1, self->angleX);
        RSDK.MatrixRotateY(&self->matrix3, self->rotation);
        RSDK.MatrixMultiply(&self->matrix4, &self->matrix1, &self->matrix3);
        RSDK.MatrixMultiply(&self->matrix3, &self->matrix4, &self->matrix2);
        RSDK.MatrixMultiply(&self->matrix3, &self->matrix3, &PBL_Camera->matWorld);
        RSDK.MatrixMultiply(&self->matrix4, &self->matrix4, &PBL_Camera->matNormalItem);
        RSDK.AddModelTo3DScene(self->animator.animationID, PBL_Player->sceneIndex, S3D_FLATCLR_SHADED_BLENDED_SCREEN, &self->matrix3,
                               &self->matrix4, 0xFFFFFF);
        RSDK.Draw3DScene(PBL_Player->sceneIndex);
    }
}

void PBL_Player_Create(void *data)
{
    RSDK_THIS(PBL_Player);
    if (!SceneInfo->inEditor) {
        self->active          = ACTIVE_NORMAL;
        self->visible         = true;
        self->updateRange.x   = 0x800000;
        self->updateRange.y   = 0x800000;
        self->drawOrder       = 4;
        self->stateInput      = PBL_Player_ProcessPlayerControl;
        self->state           = PBL_Player_State_Launcher;
        self->controllerID    = 1;
        self->onGround        = false;
        self->tileCollisions  = true;
        self->collisionLayers = 1 << PBL_Setup->tableHigh;
        RSDK.SetModelAnimation(PBL_Player->jumpFrames, &self->animator, 128, 0, true, 0);

        foreach_all(PBL_Camera, camera) { camera->targetPtr = (Entity *)self; }
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
            PBL_Player->jumpFrames       = RSDK.LoadMesh("Special/TailsJump.bin", SCOPE_STAGE);
            PBL_Player->ballFrames       = RSDK.LoadMesh("Special/TailsBall.bin", SCOPE_STAGE);
            PBL_Player->outerBox.top    = -10;
            PBL_Player->outerBox.bottom = 10;
            PBL_Player->innerBox.top    = -10;
            PBL_Player->innerBox.bottom = 10;
            break;
        case ID_KNUCKLES:
            PBL_Player->jumpFrames       = RSDK.LoadMesh("Special/KnuxJump.bin", SCOPE_STAGE);
            PBL_Player->ballFrames       = RSDK.LoadMesh("Special/KnuxBall.bin", SCOPE_STAGE);
            PBL_Player->outerBox.top    = -12;
            PBL_Player->outerBox.bottom = 12;
            PBL_Player->innerBox.top    = -12;
            PBL_Player->innerBox.bottom = 12;
            break;
        case ID_MIGHTY:
            PBL_Player->jumpFrames       = RSDK.LoadMesh("Special/MightyJump.bin", SCOPE_STAGE);
            PBL_Player->ballFrames       = RSDK.LoadMesh("Special/MightyBall.bin", SCOPE_STAGE);
            PBL_Player->outerBox.top    = -12;
            PBL_Player->outerBox.bottom = 12;
            PBL_Player->innerBox.top    = -12;
            PBL_Player->innerBox.bottom = 12;
            break;
        case ID_RAY:
            PBL_Player->jumpFrames       = RSDK.LoadMesh("Special/RayJump.bin", SCOPE_STAGE);
            PBL_Player->ballFrames       = RSDK.LoadMesh("Special/RayBall.bin", SCOPE_STAGE);
            PBL_Player->outerBox.top    = -10;
            PBL_Player->outerBox.bottom = 10;
            PBL_Player->innerBox.top    = -10;
            PBL_Player->innerBox.bottom = 10;
            break;
        default:
            PBL_Player->jumpFrames       = RSDK.LoadMesh("Special/SonicJump.bin", SCOPE_STAGE);
            PBL_Player->ballFrames       = RSDK.LoadMesh("Special/SonicBall.bin", SCOPE_STAGE);
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

    PBL_Player->sfxPlunger = RSDK.GetSfx("Pinball/Plunger.wav");
}

void PBL_Player_ProcessPlayerControl(void)
{
    RSDK_THIS(PBL_Player);
    if (self->controllerID < PLAYER_MAX) {
#if RETRO_USE_TOUCH_CONTROLS
        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= ScreenInfo->centerX && ty >= ScreenInfo->centerY && tx <= ScreenInfo->width && ty <= ScreenInfo->height) {
                    ControllerInfo[1].keyX.down = true;
                    break;
                }
            }
        }

        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= 0 && ty >= ScreenInfo->centerY && tx <= ScreenInfo->centerX && ty <= ScreenInfo->height) {
                    ControllerInfo[1].keyB.down = true;
                    break;
                }
            }
        }

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

        if (controller[self->controllerID].keyStart.press || Unknown_pausePress) {
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

void PBL_Player_State_Launcher(void)
{
    RSDK_THIS(PBL_Player);
    if (!self->onGround)
        self->velocity.y += 0x3800;

    RSDK.ProcessTileCollisions(self, &PBL_Player->outerBox, &PBL_Player->innerBox);
    if (++self->timer == 60) {
        self->velocity.y     = -0xE0000;
        self->timer          = 0;
        self->collisionPlane = 1;
        self->onGround       = false;
        self->state          = PBL_Player_State_Air;
        RSDK.PlaySfx(PBL_Player->sfxPlunger, false, 255);
    }
}

void PBL_Player_State_Ground(void)
{
    RSDK_THIS(PBL_Player);
    if (!self->angle) {
        if (abs(self->groundVel) < 0x10000)
            self->groundVel = RSDK.Rand(-0x20000, 0x20000);
    }

    self->groundVel += 0x2800 * RSDK.Sin256(self->angle) >> 8;
    if (self->collisionMode) {
        if (self->angle >= 0x40 && self->angle <= 0xC0 && abs(self->groundVel) < 0x20000) {
            self->velocity.x    = (self->groundVel * RSDK.Cos256(self->angle)) >> 8;
            self->velocity.y    = (self->groundVel * RSDK.Sin256(self->angle)) >> 8;
            self->onGround      = false;
            self->angle         = 0;
            self->collisionMode = CMODE_FLOOR;
        }
    }
    if (!self->onGround)
        self->state = PBL_Player_State_Air;
    RSDK.ProcessTileCollisions(self, &PBL_Player->outerBox, &PBL_Player->innerBox);
}

void PBL_Player_State_Air(void)
{
    RSDK_THIS(PBL_Player);
    self->velocity.y += 0x2800;
    if (abs(self->velocity.x) > abs(self->velocity.y)) {
        if (RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_LWALL, self->collisionPlane, self->velocity.x + 0x100000, 0, false)) {
            self->velocity.x = -(self->velocity.x >> 1);
        }
        if (RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_RWALL, self->collisionPlane, self->velocity.x - 0x100000, 0, false)) {
            self->velocity.x = -(self->velocity.x >> 1);
        }
    }

    RSDK.ProcessTileCollisions(self, &PBL_Player->outerBox, &PBL_Player->innerBox);
    if (self->left) {
        self->velocity.x -= 0x800;
    }
    else if (self->right) {
        self->velocity.x += 0x800;
    }
    if (self->onGround) {
        if (!self->collisionMode)
            self->collisionPlane = 0;
        self->state = PBL_Player_State_Ground;
    }
}

#if RETRO_INCLUDE_EDITOR
void PBL_Player_EditorDraw(void)
{
    RSDK_THIS(PBL_Player);
    RSDK.SetSpriteAnimation(PBL_Player->aniFrames, 0, &self->animator, true, 7);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PBL_Player_EditorLoad(void) { PBL_Player->aniFrames = RSDK.LoadSpriteAnimation("Editor/PlayerIcons.bin", SCOPE_STAGE); }
#endif

void PBL_Player_Serialize(void) {}
#endif
