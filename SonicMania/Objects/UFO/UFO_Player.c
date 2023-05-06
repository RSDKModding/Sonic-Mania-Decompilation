// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_Player Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUFO_Player *UFO_Player;

void UFO_Player_Update(void)
{
    RSDK_THIS(UFO_Player);

    StateMachine_Run(self->stateInput);
    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->animator);
    if (UFO_Setup->timedOut)
        self->groundVel -= self->groundVel >> 5;
}

void UFO_Player_LateUpdate(void)
{
    RSDK_THIS(UFO_Player);
    Matrix *mat = &UFO_Camera->matWorld;

    int32 x = self->position.x >> 16;
    int32 y = self->height >> 16;
    int32 z = self->position.y >> 16;

    self->zdepth = mat->values[2][3] + mat->values[2][0] * x + mat->values[2][1] * y + mat->values[2][2] * z;
}

void UFO_Player_StaticUpdate(void) {}

void UFO_Player_Draw(void)
{
    RSDK_THIS(UFO_Player);

    if (self->zdepth >= 1) {
        RSDK.Prepare3DScene(UFO_Player->sceneIndex);

        int32 anim = self->animator.animationID;
        if (anim == 2 || anim == 3) {
            RSDK.MatrixTranslateXYZ(&self->matTransform, self->position.x, self->height + 0x100000, self->position.y, true);

            RSDK.MatrixRotateX(&self->matRotate, self->angleX);
            RSDK.MatrixRotateY(&self->matWorld, self->angle + (self->angleZ >> 5));

            RSDK.MatrixMultiply(&self->matNormal, &self->matRotate, &self->matWorld);
            RSDK.MatrixMultiply(&self->matWorld, &self->matNormal, &self->matTransform);
            RSDK.MatrixMultiply(&self->matWorld, &self->matWorld, &UFO_Camera->matWorld);
            RSDK.MatrixMultiply(&self->matNormal, &self->matNormal, &UFO_Camera->matView);

            RSDK.AddModelTo3DScene(self->animator.animationID, UFO_Player->sceneIndex, S3D_SOLIDCOLOR_SHADED_BLENDED_SCREEN, &self->matWorld,
                                   &self->matNormal, 0xFFFFFF);
        }
        else {
            RSDK.MatrixTranslateXYZ(&self->matTransform, self->position.x, self->height, self->position.y, true);

            RSDK.MatrixRotateZ(&self->matRotate, self->angleZ >> 5);
            RSDK.MatrixRotateY(&self->matWorld, self->angle + (self->angleZ >> (6 - (uint8)(UFO_Setup->machLevel))));

            RSDK.MatrixMultiply(&self->matNormal, &self->matRotate, &self->matWorld);
            RSDK.MatrixMultiply(&self->matWorld, &self->matNormal, &self->matTransform);
            RSDK.MatrixMultiply(&self->matWorld, &self->matWorld, &UFO_Camera->matWorld);
            RSDK.MatrixRotateXYZ(&self->matNormal, 0, self->angle, 0);
            RSDK.MatrixMultiply(&self->matNormal, &self->matNormal, &UFO_Camera->matView);

            RSDK.AddMeshFrameTo3DScene(self->animator.animationID, UFO_Player->sceneIndex, &self->animator, S3D_SOLIDCOLOR_SHADED_BLENDED_SCREEN,
                                       &self->matWorld, &self->matNormal, 0xFFFFFF);
        }

        RSDK.Draw3DScene(UFO_Player->sceneIndex);
    }
}

void UFO_Player_Create(void *data)
{
    RSDK_THIS(UFO_Player);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_NORMAL;
        self->visible       = true;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawFX        = FX_FLIP;
        self->drawGroup     = 4;

        if (!self->machQuota1)
            self->machQuota1 = 32;

        if (!self->machQuota2)
            self->machQuota2 = 32;

        if (!self->machQuota3)
            self->machQuota3 = 32;

        if (self->startingRings > 0)
            UFO_Setup->rings = self->startingRings;

        UFO_Setup->machQuotas[0] = self->machQuota1;
        UFO_Setup->machQuotas[1] = self->machQuota2;
        UFO_Setup->machQuotas[2] = self->machQuota3;
        self->angle              = 0x200;
        self->onGround           = true;

        UFO_Player_ChangeMachState();
        self->stateInput   = UFO_Player_Input_P1;
        self->controllerID = INPUT_NONE;
        self->state        = UFO_Player_State_Run;

        RSDK.SetModelAnimation(UFO_Player->jogModel, &self->animator, 128, 0, true, 0);
    }
}

void UFO_Player_StageLoad(void)
{
    if (globals->playerID == ID_NONE)
        globals->playerID = ID_DEFAULT_PLAYER;

    switch (GET_CHARACTER_ID(1)) {
        default:
        case ID_SONIC:
            UFO_Player->jogModel    = RSDK.LoadMesh("Special/SonicJog.bin", SCOPE_STAGE);
            UFO_Player->dashModel   = RSDK.LoadMesh("Special/SonicDash.bin", SCOPE_STAGE);
            UFO_Player->jumpModel   = RSDK.LoadMesh("Special/SonicJump.bin", SCOPE_STAGE);
            UFO_Player->ballModel   = RSDK.LoadMesh("Special/SonicBall.bin", SCOPE_STAGE);
            UFO_Player->tumbleModel = RSDK.LoadMesh("Special/SonicTumble.bin", SCOPE_STAGE);
            break;

        case ID_TAILS:
            UFO_Player->jogModel    = RSDK.LoadMesh("Special/TailsJog.bin", SCOPE_STAGE);
            UFO_Player->dashModel   = RSDK.LoadMesh("Special/TailsDash.bin", SCOPE_STAGE);
            UFO_Player->jumpModel   = RSDK.LoadMesh("Special/TailsJump.bin", SCOPE_STAGE);
            UFO_Player->ballModel   = RSDK.LoadMesh("Special/TailsBall.bin", SCOPE_STAGE);
            UFO_Player->tumbleModel = RSDK.LoadMesh("Special/TailsTumble.bin", SCOPE_STAGE);
            break;

        case ID_KNUCKLES:
            UFO_Player->jogModel    = RSDK.LoadMesh("Special/KnuxJog.bin", SCOPE_STAGE);
            UFO_Player->dashModel   = RSDK.LoadMesh("Special/KnuxDash.bin", SCOPE_STAGE);
            UFO_Player->jumpModel   = RSDK.LoadMesh("Special/KnuxJump.bin", SCOPE_STAGE);
            UFO_Player->ballModel   = RSDK.LoadMesh("Special/KnuxBall.bin", SCOPE_STAGE);
            UFO_Player->tumbleModel = RSDK.LoadMesh("Special/KnuxTumble.bin", SCOPE_STAGE);
            break;

#if MANIA_USE_PLUS
        case ID_MIGHTY:
            UFO_Player->jogModel    = RSDK.LoadMesh("Special/MightyJog.bin", SCOPE_STAGE);
            UFO_Player->dashModel   = RSDK.LoadMesh("Special/MightyDash.bin", SCOPE_STAGE);
            UFO_Player->jumpModel   = RSDK.LoadMesh("Special/MightyJump.bin", SCOPE_STAGE);
            UFO_Player->ballModel   = RSDK.LoadMesh("Special/MightyBall.bin", SCOPE_STAGE);
            UFO_Player->tumbleModel = RSDK.LoadMesh("Special/MightyTumble.bin", SCOPE_STAGE);
            break;

        case ID_RAY:
            UFO_Player->jogModel    = RSDK.LoadMesh("Special/RayJog.bin", SCOPE_STAGE);
            UFO_Player->dashModel   = RSDK.LoadMesh("Special/RayDash.bin", SCOPE_STAGE);
            UFO_Player->jumpModel   = RSDK.LoadMesh("Special/RayJump.bin", SCOPE_STAGE);
            UFO_Player->ballModel   = RSDK.LoadMesh("Special/RayBall.bin", SCOPE_STAGE);
            UFO_Player->tumbleModel = RSDK.LoadMesh("Special/RayTumble.bin", SCOPE_STAGE);
            break;
#endif
    }

    UFO_Player->sceneIndex = RSDK.Create3DScene("View:Special", 4096, SCOPE_STAGE);

    RSDK.SetDiffuseColor(UFO_Player->sceneIndex, 0xA0, 0xA0, 0xA0);
    RSDK.SetDiffuseIntensity(UFO_Player->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularIntensity(UFO_Player->sceneIndex, 15, 15, 15);

    foreach_all(UFO_Player, player)
    {
        EntityUFO_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, UFO_Player);
        RSDK.CopyEntity(player1, player, true);
    }

    UFO_Player->sfxJump         = RSDK.GetSfx("Global/Jump.wav");
    UFO_Player->sfxCharge       = RSDK.GetSfx("Global/Charge.wav");
    UFO_Player->sfxRelease      = RSDK.GetSfx("Global/Release.wav");
    UFO_Player->sfxLoseRings    = RSDK.GetSfx("Global/LoseRings.wav");
    UFO_Player->sfxDrop         = RSDK.GetSfx("Stage/Drop.wav");
    UFO_Player->sfxBumper       = RSDK.GetSfx("Stage/Bumper.wav");
    UFO_Player->sfxSpring       = RSDK.GetSfx("Global/Spring.wav");
    UFO_Player->sfxSkid         = RSDK.GetSfx("Special/Skid.wav");
    UFO_Player->sfxGrittyGround = RSDK.GetSfx("Special/GrittyGround.wav");

    EntityUFO_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, UFO_Player);
    RSDK.ClearCameras();
    RSDK.AddCamera(&player1->position, 0x100000, 0x100000, true);
}

void UFO_Player_Input_P1(void)
{
    RSDK_THIS(UFO_Player);

    if (self->controllerID < PLAYER_COUNT) {
        RSDKControllerState *controller = &ControllerInfo[self->controllerID];
        RSDKAnalogState *stick          = &AnalogStickInfoL[self->controllerID];

        self->up    = controller->keyUp.down;
        self->down  = controller->keyDown.down;
        self->left  = controller->keyLeft.down;
        self->right = controller->keyRight.down;

        self->up |= stick->keyUp.down;
        self->down |= stick->keyDown.down;
        self->left |= stick->keyLeft.down;
        self->right |= stick->keyRight.down;

#if MANIA_USE_PLUS
        self->up |= stick->vDelta > 0.3;
        self->down |= stick->vDelta < -0.3;
        self->left |= stick->hDelta < -0.3;
        self->right |= stick->hDelta > 0.3;
#else
        self->up |= stick->vDeltaL > 0.3;
        self->down |= stick->vDeltaL < -0.3;
        self->left |= stick->hDeltaL < -0.3;
        self->right |= stick->hDeltaL > 0.3;
#endif

        if (self->left && self->right) {
            self->left  = false;
            self->right = false;
        }

        self->jumpPress = controller->keyA.press || controller->keyB.press || controller->keyC.press || controller->keyX.press;
        self->jumpHold  = controller->keyA.down || controller->keyB.down || controller->keyC.down || controller->keyX.down;

        if ((controller->keyStart.press || Unknown_pausePress) && SceneInfo->state == ENGINESTATE_REGULAR) {
            EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
            if (!pauseMenu->classID) {
                RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
                pauseMenu->triggerPlayer  = RSDK.GetEntitySlot(self);
                pauseMenu->disableRestart = true;
            }
        }
    }
}

void UFO_Player_ChangeMachState(void)
{
    RSDK_THIS(UFO_Player);

    switch (UFO_Setup->machLevel) {
        default: break;

        case 0:
            UFO_Player->maxSpeed = 0x70000;
            self->velDivisor     = 16;
            self->angleVel       = 128;
            break;

        case 1:
            UFO_Player->maxSpeed = 0xA0000;
            self->velDivisor     = 18;
            self->angleVel       = 144;
            break;

        case 2:
            UFO_Player->maxSpeed = 0xE0000;
            self->velDivisor     = 20;
            self->angleVel       = 160;
            break;
    }
}

void UFO_Player_HandleBumperTiles(void)
{
    RSDK_THIS(UFO_Player);

    int32 bumpDirMasks = 0;

    uint16 tile = RSDK.GetTile(UFO_Setup->playFieldLayer, (self->position.x - 0x80000) >> 20, (self->position.y - 0x80000) >> 20);
    if (RSDK.GetTileFlags(tile, 0) == UFO_TFLAGS_BUMPER)
        bumpDirMasks = 0b0001;

    tile = RSDK.GetTile(UFO_Setup->playFieldLayer, (self->position.x + 0x80000) >> 20, (self->position.y - 0x80000) >> 20);
    if (RSDK.GetTileFlags(tile, 0) == UFO_TFLAGS_BUMPER)
        bumpDirMasks |= 0b0010;

    tile = RSDK.GetTile(UFO_Setup->playFieldLayer, (self->position.x - 0x80000) >> 20, (self->position.y + 0x80000) >> 20);
    if (RSDK.GetTileFlags(tile, 0) == UFO_TFLAGS_BUMPER)
        bumpDirMasks |= 0b0100;

    tile = RSDK.GetTile(UFO_Setup->playFieldLayer, (self->position.x + 0x80000) >> 20, (self->position.y + 0x80000) >> 20);
    if (RSDK.GetTileFlags(tile, 0) == UFO_TFLAGS_BUMPER)
        bumpDirMasks |= 0b1000;

    if (bumpDirMasks) {
        if (!self->bumperTimer)
            RSDK.PlaySfx(UFO_Player->sfxBumper, false, 0xFF);

        self->bumperTimer = 16;
        switch (bumpDirMasks) {
            default:
            case 0b0000: break;

            case 0b0001:
            case 0b0110:
            case 0b0111:
                self->velocity.y = 0x40000;
                self->velocity.x = 0x40000;
                break;

            case 0b0010:
            case 0b1001:
            case 0b1011:
                self->velocity.x = -0x40000;
                self->velocity.y = 0x40000;
                break;

            case 0b0011: self->velocity.y = 0x40000; break;

            case 0b0100:
            case 0b1101:
                self->velocity.y = -0x40000;
                self->velocity.x = 0x40000;
                break;

            case 0b0101: self->velocity.x = 0x40000; break;

            case 0b1000:
            case 0b1110:
            case 0b1111:
                self->velocity.x = -0x40000;
                self->velocity.y = -0x40000;
                break;

            case 0b1010: self->velocity.x = -0x40000; break;
            case 0b1100: self->velocity.y = -0x40000; break;
        }
    }
}

void UFO_Player_HandleSpeedUp(void)
{
    RSDK_THIS(UFO_Player);

    uint16 tile = RSDK.GetTile(UFO_Setup->playFieldLayer, self->position.x >> 20, self->position.y >> 20);
    if (tile == (uint16)-1) {
        if (++self->courseOutTimer >= 2) {
            self->interaction   = 0;
            self->timer         = 0;
            self->state         = UFO_Player_State_CourseOut;
            self->camera->state = UFO_Camera_State_CourseOut;

            if (!UFO_Setup->timedOut)
                RSDK.PlaySfx(UFO_Player->sfxDrop, false, 0xFF);

            return;
        }
    }
    else {
        self->courseOutTimer = 0;
    }

    if (self->bumperTimer <= 0) {
        if (RSDK.GetTileFlags(tile, 0) == UFO_TFLAGS_GRITTYGROUND) {
            if (self->groundVel > UFO_Player->maxSpeed - (UFO_Player->maxSpeed >> 2))
                self->groundVel = self->groundVel - (self->groundVel >> 5);

            if (!(UFO_Setup->timer & 3)) {
                CREATE_ENTITY(UFO_Dust, NULL, self->position.x, self->position.y);
                RSDK.PlaySfx(UFO_Player->sfxGrittyGround, false, 255);
            }
        }
        else {
            if (self->groundVel > UFO_Player->maxSpeed) {
                self->groundVel -= 0x4000;

                if (self->groundVel < UFO_Player->maxSpeed)
                    self->groundVel = UFO_Player->maxSpeed;
            }
            else if (self->groundVel < UFO_Player->maxSpeed) {
                self->groundVel += 0x4000;

                if (self->groundVel > UFO_Player->maxSpeed)
                    self->groundVel = UFO_Player->maxSpeed;
            }
        }
    }
}

void UFO_Player_State_Run(void)
{
    RSDK_THIS(UFO_Player);

    int32 turnSpeed = UFO_Player->maxSpeed - 0x87000;

    if (self->right) {
        if (self->angleZ < (turnSpeed >> 11) + 1280)
            self->angleZ += (turnSpeed >> 14) + self->angleVel;
    }
    else if (self->left) {
        if (self->angleZ > -1280 - (turnSpeed >> 11))
            self->angleZ -= self->angleVel - (turnSpeed >> 14);
    }
    else {
        self->angleZ -= self->angleZ >> 2;
    }

    if (self->angleZ <= 0)
        self->angle -= -self->angleZ >> 8;
    else
        self->angle += self->angleZ >> 8;

    self->angle &= 0x3FF;

    if (abs(self->angleZ) <= 1280 || UFO_Setup->machLevel <= 1) {
        self->skidTimer = 0;
    }
    else {
        if (self->skidTimer >= 16) {
            if (!(UFO_Setup->timer & 3))
                CREATE_ENTITY(UFO_Dust, NULL, self->position.x, self->position.y);

            if (!(UFO_Setup->timer & 7))
                RSDK.PlaySfx(UFO_Player->sfxSkid, false, 255);
        }
        else {
            self->skidTimer++;
        }
    }

    if (self->bumperTimer >= 1) {
        self->bumperTimer--;
        UFO_Player_HandleSpeedUp();
    }
    else {
        UFO_Player_HandleSpeedUp();

        int32 x = (self->groundVel >> 10) * RSDK.Sin1024(self->angle);
        int32 y = (self->groundVel >> 10) * RSDK.Cos1024(self->angle);

        self->velocity.x += (x - self->velocity.x) / self->velDivisor;
        self->velocity.y += (-y - self->velocity.y) / self->velDivisor;
    }

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->jumpPress && self->state != UFO_Player_State_CourseOut) {
        self->gravityStrength = 0x50000;
        self->onGround        = false;
        self->state           = UFO_Player_State_Jump;

        RSDK.SetModelAnimation(UFO_Player->jumpModel, &self->animator, 128, 0, true, 0);
        RSDK.PlaySfx(UFO_Player->sfxJump, false, 0xFF);

        UFO_Player_HandleBumperTiles();
    }
    else {
        if (self->groundVel <= 0xC0000) {
            RSDK.SetModelAnimation(UFO_Player->jogModel, &self->animator, 128, 0, 0, 0);
            self->animator.speed = (self->groundVel >> 12) + 48;
        }
        else {
            RSDK.SetModelAnimation(UFO_Player->dashModel, &self->animator, 160, 0, 0, 0);
        }

        UFO_Player_HandleBumperTiles();
    }
}

void UFO_Player_State_Jump(void)
{
    RSDK_THIS(UFO_Player);

    int32 turnSpeed = UFO_Player->maxSpeed - 0x60000;

    int32 speed = 0;
    if (self->right) {
        speed = (turnSpeed >> 20) + 12;

        if (self->angleZ < 1280 + (turnSpeed >> 10))
            self->angleZ += (self->angleVel >> 1) + (turnSpeed >> 14);
    }
    else if (self->left) {
        speed = -12 - (turnSpeed >> 20);

        if (self->angleZ > -1280 - (turnSpeed >> 10))
            self->angleZ -= (self->angleVel >> 1) - (turnSpeed >> 14);
    }
    else {
        speed = 0;

        self->angleZ -= self->angleZ >> 4;
    }

    if (self->angleZ <= 0)
        self->angle -= -self->angleZ >> 8;
    else
        self->angle += self->angleZ >> 8;

    self->angle &= 0x3FF;

    self->velocity.x -= self->velocity.x >> 8;
    self->velocity.y -= self->velocity.y >> 8;
    self->velocity.x += speed * RSDK.Cos1024(self->angle);
    self->velocity.y += speed * RSDK.Sin1024(self->angle);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->gravityStrength += -0x3800 - (turnSpeed >> 7);

    self->height += self->gravityStrength;
    self->angleX = (self->angleX - (UFO_Player->maxSpeed >> 13)) & 0x3FF;

    self->animator.animationID = (self->angleX & 0x100) ? UFO_Player->ballModel : UFO_Player->jumpModel;

    if (self->height < 0 && self->gravityStrength <= 0) {
        self->courseOutTimer = 4;
        self->height         = 0;
        self->onGround       = true;
        self->state          = UFO_Player_State_Run;
        self->angleZ -= (self->angleZ >> 1);
    }

    UFO_Player_HandleBumperTiles();
}

void UFO_Player_State_Springboard(void)
{
    RSDK_THIS(UFO_Player);

    int32 tilt = 0;
    if (self->right)
        tilt = 8;
    else if (self->left)
        tilt = -8;

    self->velocity.x -= self->velocity.x >> 8;
    self->velocity.y -= self->velocity.y >> 8;

    self->velocity.x += tilt * RSDK.Cos1024(self->angle);
    self->velocity.y += tilt * RSDK.Sin1024(self->angle);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    self->gravityStrength -= 0x5000;
    self->height += self->gravityStrength;

    self->angleX = (self->angleX - (UFO_Player->maxSpeed >> 13)) & 0x3FF;

    self->animator.animationID = (self->angleX & 0x100) ? UFO_Player->ballModel : UFO_Player->jumpModel;

    if (self->height < 0) {
        self->courseOutTimer = 4;
        self->height         = 0;
        self->onGround       = true;
        self->state          = UFO_Player_State_Run;
        self->angleZ -= (self->angleZ >> 1);
    }

    UFO_Player_HandleBumperTiles();
}

void UFO_Player_State_Trip(void)
{
    RSDK_THIS(UFO_Player);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    self->velocity.x -= self->velocity.x >> 8;
    self->velocity.y -= self->velocity.y >> 8;

    if (!self->onGround) {
        self->gravityStrength -= 0x3800;
        self->height += self->gravityStrength;

        if (self->height < 0) {
            self->height   = 0;
            self->onGround = true;
        }
    }

    UFO_Player_HandleBumperTiles();

    if (++self->timer > 32) {
        self->timer = 0;

        if (self->onGround)
            self->state = UFO_Player_State_Run;
        else
            self->state = UFO_Player_State_Jump;
    }
}

void UFO_Player_State_CourseOut(void)
{
    RSDK_THIS(UFO_Player);

    self->velocity.x -= (self->velocity.x >> 5);
    self->velocity.y -= (self->velocity.y >> 5);
    self->gravityStrength -= 0x800;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->height += self->gravityStrength;

    if (self->height < 0)
        self->drawGroup = 2;

    if (RSDK_GET_ENTITY(SLOT_UFO_SETUP, UFO_Setup)->state == UFO_Setup_State_HandleRingDrain) {
        ++self->timer;

        if (self->timer == 120) {
            UFO_Setup_Finish_Fail();
        }
        else if (self->timer == 16) {
            if (!UFO_Setup->timedOut) {
                UFO_Setup->timedOut = true;
                CREATE_ENTITY(UFO_Message, INT_TO_VOID(UFO_MESSAGE_COURSEOUT), self->position.x, self->position.y);
            }
        }
    }
}

void UFO_Player_State_UFOCaught_Charge(void)
{
    RSDK_THIS(UFO_Player);

    ++self->timer;

    self->velocity.x -= (self->velocity.x >> 4);
    self->velocity.y -= (self->velocity.y >> 4);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    self->angleX = (self->angleX - (UFO_Player->maxSpeed >> 13)) & 0x3FF;

    if (self->timer == 2) {
        RSDK.SetModelAnimation(UFO_Player->jumpModel, &self->animator, 128, 0, true, 0);
        RSDK.PlaySfx(UFO_Player->sfxCharge, false, 255);
    }

    if (self->timer >= 30) {
        self->timer         = 0;
        self->camera->state = UFO_Camera_State_UFOCaught;
        self->state         = UFO_Player_State_UFOCaught_Released;

        self->velocity.x      = (self->circuitPtr->position.x - self->position.x) >> 4;
        self->velocity.y      = (self->circuitPtr->position.y - self->position.y) >> 4;
        self->gravityStrength = (self->circuitPtr->height - self->height + 0x200000) >> 4;

        RSDK.PlaySfx(UFO_Player->sfxRelease, false, 255);
    }
}

void UFO_Player_State_UFOCaught_Released(void)
{
    RSDK_THIS(UFO_Player);

    self->gravityStrength -= 0x80;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->height += self->gravityStrength;

    ++self->timer;

    self->angleX = (self->angleX - (UFO_Player->maxSpeed >> 13)) & 0x3FF;

    if (self->timer == 16)
        UFO_Setup_Finish_Win();
}

#if GAME_INCLUDE_EDITOR
void UFO_Player_EditorDraw(void)
{
    RSDK_THIS(UFO_Player);

    RSDK.SetSpriteAnimation(UFO_Player->aniFrames, 0, &self->animator, true, 7);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void UFO_Player_EditorLoad(void) { UFO_Player->aniFrames = RSDK.LoadSpriteAnimation("Editor/PlayerIcons.bin", SCOPE_STAGE); }
#endif

void UFO_Player_Serialize(void)
{
    RSDK_EDITABLE_VAR(UFO_Player, VAR_ENUM, machQuota1);
    RSDK_EDITABLE_VAR(UFO_Player, VAR_ENUM, machQuota2);
    RSDK_EDITABLE_VAR(UFO_Player, VAR_ENUM, machQuota3);
    RSDK_EDITABLE_VAR(UFO_Player, VAR_ENUM, startingRings);
}
