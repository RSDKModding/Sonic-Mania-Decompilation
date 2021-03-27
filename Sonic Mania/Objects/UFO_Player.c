#include "../SonicMania.h"

ObjectUFO_Player *UFO_Player;

void UFO_Player_Update()
{
    RSDK_THIS(UFO_Player);

    if (entity->inputState)
        entity->inputState();

    if (entity->state)
        entity->state();

    RSDK.ProcessAnimation(&entity->playerData);
    if (UFO_Setup->timedOut)
        entity->groundVel -= entity->groundVel >> 5;
}

void UFO_Player_LateUpdate()
{
    RSDK_THIS(UFO_Player);
    Matrix *mat = &UFO_Camera->matWorld;

    entity->depth = mat->values[2][3] + mat->values[2][0] * (entity->position.x >> 0x10) + mat->values[2][2] * (entity->position.y >> 0x10)
                    + mat->values[2][1] * (entity->height >> 0x10);
}

void UFO_Player_StaticUpdate() {}

void UFO_Player_Draw()
{
    RSDK_THIS(UFO_Player);
    if (entity->depth >= 1) {
        RSDK.Prepare3DScene(UFO_Player->sceneIndex);
        int anim = entity->playerData.animationID;
        if (anim == 2 || anim == 3) {
            RSDK.MatrixTranslateXYZ(&entity->matrix2, entity->position.x, entity->height + 0x100000, entity->position.y, true);
            RSDK.MatrixRotateX(&entity->matrix1, entity->field_74);
            RSDK.MatrixRotateY(&entity->matrix3, entity->angle + (entity->angleZ >> 5));
            RSDK.MatrixMultiply(&entity->matrix4, &entity->matrix1, &entity->matrix3);
            RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix4, &entity->matrix2);
            RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &UFO_Camera->matWorld);
            RSDK.MatrixMultiply(&entity->matrix4, &entity->matrix4, &UFO_Camera->matView);
            RSDK.AddModelTo3DScene(entity->playerData.animationID, UFO_Player->sceneIndex, S3D_FLATCLR_SHADED_BLENDED_SCREEN, &entity->matrix3,
                                   &entity->matrix4, 0xFFFFFF);
        }
        else {
            RSDK.MatrixTranslateXYZ(&entity->matrix2, entity->position.x, entity->height, entity->position.y, true);
            RSDK.MatrixRotateZ(&entity->matrix1, entity->angleZ >> 5);
            RSDK.MatrixRotateY(&entity->matrix3, entity->angle + (entity->angleZ >> (6 - (byte)(UFO_Setup->machLevel))));
            RSDK.MatrixMultiply(&entity->matrix4, &entity->matrix1, &entity->matrix3);
            RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix4, &entity->matrix2);
            RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &UFO_Camera->matWorld);
            RSDK.MatrixRotateXYZ(&entity->matrix4, 0, entity->angle, 0);
            RSDK.MatrixMultiply(&entity->matrix4, &entity->matrix4, &UFO_Camera->matView);
            RSDK.AddMeshFrameTo3DScene(entity->playerData.animationID, UFO_Player->sceneIndex, &entity->playerData, S3D_FLATCLR_SHADED_BLENDED_SCREEN,
                                       &entity->matrix3, &entity->matrix4, 0xFFFFFF);
        }
        RSDK.Draw3DScene(UFO_Player->sceneIndex);
    }
}

void UFO_Player_Create(void *data)
{
    RSDK_THIS(UFO_Player);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_NORMAL;
        entity->visible       = true;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->drawFX        = FX_FLIP;
        entity->drawOrder     = 4;
        if (!entity->machQuota1)
            entity->machQuota1 = 32;
        if (!entity->machQuota2)
            entity->machQuota2 = 32;
        if (!entity->machQuota3)
            entity->machQuota3 = 32;
        if (entity->startingRings > 0)
            UFO_Setup->rings = entity->startingRings;

        UFO_Setup->machQuotas[0] = entity->machQuota1;
        UFO_Setup->machQuotas[1] = entity->machQuota2;
        UFO_Setup->machQuotas[2] = entity->machQuota3;
        entity->angle            = 0x200;
        entity->onGround         = true;
        UFO_Player_ChangeMachState();
        entity->inputState   = UFO_Player_ProcessPlayerControl;
        entity->controllerID = 0;
        entity->state        = UFO_Player_StateRun;
        RSDK.SetModelAnimation(UFO_Player->jogModel, &entity->playerData, 128, 0, true, 0);
    }
}

void UFO_Player_StageLoad()
{
    if (globals->playerID == ID_NONE) {
        globals->playerID = ID_DEFAULT_PLAYER;
    }

    switch (globals->playerID & 0xFF) {
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
#if RETRO_USE_PLUS
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
        default:
            UFO_Player->jogModel    = RSDK.LoadMesh("Special/SonicJog.bin", SCOPE_STAGE);
            UFO_Player->dashModel   = RSDK.LoadMesh("Special/SonicDash.bin", SCOPE_STAGE);
            UFO_Player->jumpModel   = RSDK.LoadMesh("Special/SonicJump.bin", SCOPE_STAGE);
            UFO_Player->ballModel   = RSDK.LoadMesh("Special/SonicBall.bin", SCOPE_STAGE);
            UFO_Player->tumbleModel = RSDK.LoadMesh("Special/SonicTumble.bin", SCOPE_STAGE);
            break;
    }

    UFO_Player->sceneIndex = RSDK.Create3DScene("View:Special", 4096, SCOPE_STAGE);
    RSDK.SetAmbientColour(UFO_Player->sceneIndex, 160, 160, 160);
    RSDK.SetDiffuseColour(UFO_Player->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularColour(UFO_Player->sceneIndex, 15, 15, 15);

    foreach_all(UFO_Player, player)
    {
        Entity *player1 = RSDK.GetEntityByID(SLOT_PLAYER1);
        RSDK.CopyEntity(player1, player, true);
    }

    UFO_Player->sfx_Jump         = RSDK.GetSFX("Global/Jump.wav");
    UFO_Player->sfx_Charge       = RSDK.GetSFX("Global/Charge.wav");
    UFO_Player->sfx_Release      = RSDK.GetSFX("Global/Release.wav");
    UFO_Player->sfx_LoseRings    = RSDK.GetSFX("Global/LoseRings.wav");
    UFO_Player->sfx_Drop         = RSDK.GetSFX("Stage/Drop.wav");
    UFO_Player->sfx_Bumper       = RSDK.GetSFX("Stage/Bumper.wav");
    UFO_Player->sfx_Spring       = RSDK.GetSFX("Global/Spring.wav");
    UFO_Player->sfx_Skid         = RSDK.GetSFX("Special/Skid.wav");
    UFO_Player->sfx_GrittyGround = RSDK.GetSFX("Special/GrittyGround.wav");

    EntityUFO_Player *player1 = (EntityUFO_Player *)RSDK.GetEntityByID(SLOT_PLAYER1);
    RSDK.ClearScreens();
    RSDK.AddScreen(&player1->position, 0x100000, 0x100000, true);
}

void UFO_Player_EditorDraw() {}

void UFO_Player_EditorLoad() {}

void UFO_Player_ProcessPlayerControl()
{
    RSDK_THIS(UFO_Player);
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
        entity->jumpHold  = controller->keyA.down | controller->keyB.down | controller->keyC.down | controller->keyX.down;

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

void UFO_Player_ChangeMachState()
{
    RSDK_THIS(UFO_Player);
    switch (UFO_Setup->machLevel) {
        default: break;
        case 0:
            UFO_Player->maxSpeed = 0x70000;
            entity->field_84     = 16;
            entity->field_8C     = 128;
            break;
        case 1:
            UFO_Player->maxSpeed = 0xA0000;
            entity->field_84     = 18;
            entity->field_8C     = 144;
            break;
        case 2:
            UFO_Player->maxSpeed = 0xE0000;
            entity->field_84     = 20;
            entity->field_8C     = 160;
            break;
    }
}

void UFO_Player_HandleBumperTiles()
{
    RSDK_THIS(UFO_Player);
    int flags = 0;

    ushort tile = RSDK.GetTileInfo(UFO_Setup->playFieldLayer, (entity->position.x - 0x80000) >> 20, (entity->position.y - 0x80000) >> 20);
    if (RSDK.GetTileBehaviour(tile, 0) == 1)
        flags = 1;

    tile = RSDK.GetTileInfo(UFO_Setup->playFieldLayer, (entity->position.x + 0x80000) >> 20, (entity->position.y - 0x80000) >> 20);
    if (RSDK.GetTileBehaviour(tile, 0) == 1)
        flags |= 2;

    tile = RSDK.GetTileInfo(UFO_Setup->playFieldLayer, (entity->position.x - 0x80000) >> 20, (entity->position.y + 0x80000) >> 20);
    if (RSDK.GetTileBehaviour(tile, 0) == 1)
        flags |= 4;

    tile = RSDK.GetTileInfo(UFO_Setup->playFieldLayer, (entity->position.x + 0x80000) >> 20, (entity->position.y + 0x80000) >> 20);
    if (RSDK.GetTileBehaviour(tile, 0) == 1)
        flags |= 8;

    if (flags) {
        if (!entity->bumperTimer)
            RSDK.PlaySFX(UFO_Player->sfx_Bumper, 0, 255);
        entity->bumperTimer = 16;
        switch (flags) {
            case 1:
            case 6:
            case 7:
                entity->velocity.y = 0x40000;
                entity->velocity.x = 0x40000;
                break;
            case 2:
            case 9:
            case 11:
                entity->velocity.x = -0x40000;
                entity->velocity.y = 0x40000;
                break;
            case 3: entity->velocity.y = 0x40000; break;
            case 4:
            case 13:
                entity->velocity.y = -0x40000;
                entity->velocity.x = 0x40000;
                break;
            case 5: entity->velocity.x = 0x40000; break;
            case 8:
            case 14:
            case 15:
                entity->velocity.x = -0x40000;
                entity->velocity.y = -0x40000;
                break;
            case 10: entity->velocity.x = -0x40000; break;
            case 12: entity->velocity.y = -0x40000; break;
            default: break;
        }
    }
}

void UFO_Player_HandleSpeedUp()
{
    RSDK_THIS(UFO_Player);

    ushort tile = RSDK.GetTileInfo(UFO_Setup->playFieldLayer, entity->position.x >> 20, entity->position.y >> 20);
    if (tile == 0xFFFF) {
        if (++entity->field_70 >= 2) {
            entity->interaction   = 0;
            entity->timer         = 0;
            entity->state         = UFO_Player_Unknown9;
            entity->camera->state = UFO_Camera_Unknown3;

            if (!UFO_Setup->timedOut)
                RSDK.PlaySFX(UFO_Player->sfx_Drop, 0, 255);
            return;
        }
    }
    else {
        entity->field_70 = 0;
    }

    if (entity->bumperTimer <= 0) {
        if (RSDK.GetTileBehaviour(tile, 0) == 2) {
            if (entity->groundVel > UFO_Player->maxSpeed - (UFO_Player->maxSpeed >> 2))
                entity->groundVel = entity->groundVel - (entity->groundVel >> 5);

            if (!(UFO_Setup->timer & 3)) {
                RSDK.CreateEntity(UFO_Dust->objectID, 0, entity->position.x, entity->position.y);
                RSDK.PlaySFX(UFO_Player->sfx_GrittyGround, 0, 255);
            }
        }
        else {
            if (entity->groundVel > UFO_Player->maxSpeed) {
                entity->groundVel -= 0x4000;
                if (entity->groundVel < UFO_Player->maxSpeed)
                    entity->groundVel = UFO_Player->maxSpeed;
            }
            else if (entity->groundVel < UFO_Player->maxSpeed) {
                entity->groundVel += 0x4000;
                if (entity->groundVel > UFO_Player->maxSpeed)
                    entity->groundVel = UFO_Player->maxSpeed;
            }
        }
    }
}

void UFO_Player_StateRun()
{
    RSDK_THIS(UFO_Player);
    int val = UFO_Player->maxSpeed - 0x87000;
    if (entity->right) {
        if (entity->angleZ < (val >> 11) + 1280)
            entity->angleZ += (val >> 14) + entity->field_8C;
    }
    else if (entity->left) {
        if (entity->angleZ > -1280 - (val >> 11))
            entity->angleZ -= entity->field_8C - (val >> 14);
    }
    else {
        entity->angleZ -= entity->angleZ >> 2;
    }

    if (entity->angleZ <= 0)
        entity->angle -= -entity->angleZ >> 8;
    else
        entity->angle += entity->angleZ >> 8;
    entity->angle &= 0x3FF;

    if (abs(entity->angleZ) <= 1280 || UFO_Setup->machLevel <= 1) {
        entity->field_90 = 0;
    }
    else {
        if (entity->field_90 >= 16) {
            if (!(UFO_Setup->timer & 3)) {
                RSDK.CreateEntity(UFO_Dust->objectID, 0, entity->position.x, entity->position.y);
            }
            if (!(UFO_Setup->timer & 7))
                RSDK.PlaySFX(UFO_Player->sfx_Skid, 0, 255);
        }
        else {
            entity->field_90++;
        }
    }

    if (entity->bumperTimer >= 1) {
        entity->bumperTimer--;
        UFO_Player_HandleSpeedUp();
    }
    else {
        UFO_Player_HandleSpeedUp();
        int x = (entity->groundVel >> 10) * RSDK.Sin1024(entity->angle);
        int y = (entity->groundVel >> 10) * RSDK.Cos1024(entity->angle);
        entity->velocity.x += (x - entity->velocity.x) / entity->field_84;
        entity->velocity.y += (-y - entity->velocity.y) / entity->field_84;
    }
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (!entity->jumpPress || entity->state == UFO_Player_Unknown9) {
        if (entity->groundVel <= 0xC0000) {
            RSDK.SetModelAnimation(UFO_Player->jogModel, &entity->playerData, 128, 0, 0, 0);
            entity->playerData.animationSpeed = (entity->groundVel >> 12) + 48;
        }
        else {
            RSDK.SetModelAnimation(UFO_Player->dashModel, &entity->playerData, 160, 0, 0, 0);
        }
        UFO_Player_HandleBumperTiles();
    }
    else {
        entity->gravityStrength = 0x50000;
        entity->onGround        = 0;
        entity->state           = UFO_Player_StateJump;
        RSDK.SetModelAnimation(UFO_Player->jumpModel, &entity->playerData, 128, 0, true, 0);
        RSDK.PlaySFX(UFO_Player->sfx_Jump, 0, 255);
        UFO_Player_HandleBumperTiles();
    }
}

void UFO_Player_StateJump()
{
    RSDK_THIS(UFO_Player);
    int val = UFO_Player->maxSpeed - 0x60000;

    int speed = 0;
    if (entity->right) {
        speed = (val >> 20) + 12;
        if (entity->angleZ < (val >> 10) + 1280)
            entity->angleZ += (entity->field_8C >> 1) + (val >> 14);
    }
    else if (entity->left) {
        speed = -12 - (val >> 20);
        if (entity->angleZ > -1280 - (val >> 10))
            entity->angleZ -= (entity->field_8C >> 1) - (val >> 14);
    }
    else {
        speed = 0;
        entity->angleZ -= entity->angleZ >> 4;
    }

    if (entity->angleZ <= 0)
        entity->angle -= -entity->angleZ >> 8;
    else
        entity->angle += entity->angleZ >> 8;
    entity->angle &= 0x3FF;

    entity->velocity.x -= entity->velocity.x >> 8;
    entity->velocity.y -= entity->velocity.y >> 8;

    entity->velocity.x += speed * RSDK.Cos1024(entity->angle);
    entity->velocity.y += speed * RSDK.Sin1024(entity->angle);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->gravityStrength += -0x3800 - (val >> 7);

    entity->height += entity->gravityStrength;
    entity->field_74 = (entity->field_74 - (UFO_Player->maxSpeed >> 13)) & 0x3FF;
    if (entity->field_74 & 0x100)
        entity->playerData.animationID = UFO_Player->ballModel;
    else
        entity->playerData.animationID = UFO_Player->jumpModel;

    if (entity->height < 0 && entity->gravityStrength <= 0) {
        entity->field_70 = 4;
        entity->height   = 0;
        entity->onGround = true;
        entity->state    = UFO_Player_StateRun;
        entity->angleZ   = entity->angleZ - (entity->angleZ >> 1);
    }
    UFO_Player_HandleBumperTiles();
}

void UFO_Player_HandleTilt()
{
    RSDK_THIS(UFO_Player);

    int tilt = 0;
    if (entity->right) {
        tilt = 8;
    }
    else if (entity->left) {
        tilt = -8;
    }

    entity->velocity.x -= entity->velocity.x >> 8;
    entity->velocity.y -= entity->velocity.y >> 8;
    entity->velocity.x += tilt * RSDK.Cos1024(entity->angle);
    entity->gravityStrength -= 0x5000;
    entity->velocity.y += tilt * RSDK.Sin1024(entity->angle);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->height += entity->gravityStrength;

    entity->field_74 = (entity->field_74 - (UFO_Player->maxSpeed >> 13)) & 0x3FF;
    if (entity->field_74 & 0x100)
        entity->playerData.animationID = UFO_Player->ballModel;
    else
        entity->playerData.animationID = UFO_Player->jumpModel;

    if (entity->height < 0) {
        entity->field_70 = 4;
        entity->height   = 0;
        entity->onGround = true;
        entity->state    = UFO_Player_StateRun;
        entity->angleZ   = entity->angleZ - (entity->angleZ >> 1);
    }
    UFO_Player_HandleBumperTiles();
}

void UFO_Player_Unknown8()
{
    RSDK_THIS(UFO_Player);
    int xVel = entity->velocity.x;
    int zVel = entity->velocity.y;
    entity->position.x += xVel;
    entity->position.y += zVel;
    entity->velocity.x = xVel - (xVel >> 8);
    entity->velocity.y = zVel - (zVel >> 8);

    if (!entity->onGround) {
        entity->gravityStrength -= 0x3800;
        entity->height += entity->gravityStrength;
        if (entity->height < 0) {
            entity->height   = 0;
            entity->onGround = true;
        }
    }

    UFO_Player_HandleBumperTiles();
    if (++entity->timer > 32) {
        entity->timer = 0;
        if (entity->onGround)
            entity->state = UFO_Player_StateRun;
        else
            entity->state = UFO_Player_StateJump;
    }
}

void UFO_Player_Unknown9()
{
    RSDK_THIS(UFO_Player);
    int x = entity->velocity.x;
    int y = entity->velocity.y;
    entity->gravityStrength -= 0x800;
    entity->position.x += x - (x >> 5);
    entity->velocity.x = x - (x >> 5);
    entity->position.y += y - (y >> 5);
    entity->height += entity->gravityStrength;
    entity->velocity.y = y - (y >> 5);
    if (entity->height < 0)
        entity->drawOrder = 2;
    if (RSDK_GET_ENTITY(SLOT_UFO_SETUP, UFO_Setup)->state == UFO_Setup_Unknown13) {
        ++entity->timer;
        if (entity->timer == 120) {
            UFO_Setup_Unknown7(entity);
        }
        else if (entity->timer == 16) {
            if (!UFO_Setup->timedOut) {
                UFO_Setup->timedOut = true;
                RSDK.CreateEntity(UFO_Message->objectID, (void *)2, entity->position.x, entity->position.y);
            }
        }
    }
}

void UFO_Player_Unknown10()
{
    RSDK_THIS(UFO_Player);
    int xVel = entity->velocity.x;
    int zVel = entity->velocity.y;
    ++entity->timer;
    entity->position.x += xVel - (xVel >> 4);
    entity->velocity.x = xVel - (xVel >> 4);
    entity->position.y += zVel - (zVel >> 4);
    entity->velocity.y = zVel - (zVel >> 4);

    entity->field_74 = (entity->field_74 - (UFO_Player->maxSpeed >> 13)) & 0x3FF;
    if (entity->timer == 2) {
        RSDK.SetModelAnimation(UFO_Player->jumpModel, &entity->playerData, 128, 0, true, 0);
        RSDK.PlaySFX(UFO_Player->sfx_Charge, 0, 255);
    }
    if (entity->timer >= 30) {
        entity->timer         = 0;
        entity->camera->state = UFO_Camera_Unknown4;
        entity->velocity.x    = (entity->circuitPtr->position.x - entity->position.x) >> 4;
        entity->velocity.y    = (entity->circuitPtr->position.y - entity->position.y) >> 4;
        entity->state         = UFO_Player_Unknown11;
        // entity->gravityStrength = (entity->circuitPtr->height - entity->height + 0x200000) >> 4;
        RSDK.PlaySFX(UFO_Player->sfx_Release, 0, 255);
    }
}

void UFO_Player_Unknown11()
{
    RSDK_THIS(UFO_Player);
    entity->position.x += entity->velocity.x;
    entity->gravityStrength -= 128;
    entity->position.y += entity->velocity.y;
    entity->height += entity->gravityStrength;
    ++entity->timer;

    entity->field_74 = (entity->field_74 - (UFO_Player->maxSpeed >> 13)) & 0x3FF;
    if (entity->timer == 16)
        UFO_Setup_Unknown6();
}

void UFO_Player_Serialize()
{
    RSDK_EDITABLE_VAR(UFO_Player, VAR_ENUM, machQuota1);
    RSDK_EDITABLE_VAR(UFO_Player, VAR_ENUM, machQuota2);
    RSDK_EDITABLE_VAR(UFO_Player, VAR_ENUM, machQuota3);
    RSDK_EDITABLE_VAR(UFO_Player, VAR_ENUM, startingRings);
}
