// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CrimsonEye Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCrimsonEye *CrimsonEye;

void CrimsonEye_Update(void)
{
    RSDK_THIS(CrimsonEye);

    StateMachine_Run(self->state);
}

void CrimsonEye_LateUpdate(void) {}

void CrimsonEye_StaticUpdate(void) {}

void CrimsonEye_Draw(void)
{
    RSDK_THIS(CrimsonEye);

    StateMachine_Run(self->stateDraw);
}

void CrimsonEye_Create(void *data)
{
    RSDK_THIS(CrimsonEye);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            if (data)
                self->type = VOID_TO_INT(data);

            switch (self->type) {
                case CRIMSONEYE_CONTAINER:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0] + 1;

                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 0, &self->animator, true, 0);

                    self->updateRange.x        = 0x800000;
                    self->updateRange.y        = 0x800000;
                    CrimsonEye->containerPos.x = self->position.x;
                    CrimsonEye->containerPos.y = self->position.y - 0xA00000;

                    self->state     = CrimsonEye_StateContainer_SetupArena;
                    self->stateDraw = CrimsonEye_Draw_Container;
                    break;

                case CRIMSONEYE_CORE:
                    self->active    = ACTIVE_NEVER;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0] + 1;

                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &self->animator, true, 0);
                    self->originPos = self->position;

                    self->state     = CrimsonEye_StateCore_ContainerActive;
                    self->stateDraw = CrimsonEye_Draw_Core;

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    break;

                case CRIMSONEYE_BALL:
                    self->drawFX    = FX_SCALE;
                    self->active    = ACTIVE_NEVER;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0] + 1;

                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 2, &self->animator, true, 0);
                    self->scale.x = 0x200;
                    self->scale.y = 0x200;

                    self->state     = CrimsonEye_StateBall_Spinning;
                    self->stateDraw = CrimsonEye_Draw_Simple;

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    break;

                case CRIMSONEYE_ARROW:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 3, &self->animator, true, 0);
                    self->scale.x = 0x200;
                    self->scale.y = 0x200;

                    self->state     = CrimsonEye_State_Arrow;
                    self->stateDraw = CrimsonEye_Draw_Arrow;

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    break;

                case CRIMSONEYE_SPIKE:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0] + 1;

                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 2, &self->animator, true, 0);

                    self->state     = CrimsonEye_StateSpike_Harmful;
                    self->stateDraw = CrimsonEye_Draw_Simple;

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    break;

                case CRIMSONEYE_SHOT:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0] + 1;

                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 4, &self->animator, true, 0);

                    self->state     = CrimsonEye_State_Shot;
                    self->stateDraw = CrimsonEye_Draw_Simple;

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    break;

                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void CrimsonEye_StageLoad(void)
{
    CrimsonEye->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/CrimsonEye.bin", SCOPE_STAGE);

    CrimsonEye->hitboxEye.left   = -12;
    CrimsonEye->hitboxEye.top    = -12;
    CrimsonEye->hitboxEye.right  = 12;
    CrimsonEye->hitboxEye.bottom = 12;

    CrimsonEye->hitboxBall.left   = -16;
    CrimsonEye->hitboxBall.top    = -16;
    CrimsonEye->hitboxBall.right  = 16;
    CrimsonEye->hitboxBall.bottom = 16;

    CrimsonEye->hitboxOrb.left   = -3;
    CrimsonEye->hitboxOrb.top    = -3;
    CrimsonEye->hitboxOrb.right  = 3;
    CrimsonEye->hitboxOrb.bottom = 3;

    CrimsonEye->hitboxBlock.left   = -21;
    CrimsonEye->hitboxBlock.top    = -23;
    CrimsonEye->hitboxBlock.right  = 21;
    CrimsonEye->hitboxBlock.bottom = 23;

    CrimsonEye->hitboxElecOrb.left   = -6;
    CrimsonEye->hitboxElecOrb.top    = -6;
    CrimsonEye->hitboxElecOrb.right  = 6;
    CrimsonEye->hitboxElecOrb.bottom = 6;

    CrimsonEye->hitboxUnused1.left   = -14;
    CrimsonEye->hitboxUnused1.top    = -32;
    CrimsonEye->hitboxUnused1.right  = 14;
    CrimsonEye->hitboxUnused1.bottom = -23;

    CrimsonEye->hitboxUnused2.left   = -14;
    CrimsonEye->hitboxUnused2.top    = 23;
    CrimsonEye->hitboxUnused2.right  = 14;
    CrimsonEye->hitboxUnused2.bottom = 32;

    CrimsonEye->health              = 17;
    CrimsonEye->invincibilityTimer  = 0;
    CrimsonEye->ballSpinAngleX      = 0;
    CrimsonEye->ballSpinRadius      = 0x4800;
    CrimsonEye->ballOffsetY         = 0;
    CrimsonEye->elevatorSpeed       = 0;
    CrimsonEye->targetElevatorSpeed = -0x8000;
    CrimsonEye->elevatorGravity     = 0x800;
    CrimsonEye->destroyedBallCount  = 0;
    CrimsonEye->shotsRemaining      = 0;

    CrimsonEye->liftBackground = RSDK.GetTileLayer(RSDK.GetTileLayerID("Lift Background"));

    CrimsonEye->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    CrimsonEye->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    CrimsonEye->sfxHover     = RSDK.GetSfx("TMZ1/Hover.wav");
    CrimsonEye->sfxHover2    = RSDK.GetSfx("TMZ1/Hover2.wav");
    CrimsonEye->sfxElevator  = RSDK.GetSfx("TMZ1/Elevator.wav");
    CrimsonEye->sfxShot      = RSDK.GetSfx("Stage/Shot.wav");
    CrimsonEye->sfxBeep      = RSDK.GetSfx("Stage/Beep4.wav");
    CrimsonEye->sfxHullClose = RSDK.GetSfx("Stage/HullClose.wav");
    CrimsonEye->sfxButton    = RSDK.GetSfx("Stage/Button.wav");
    CrimsonEye->sfxImpact    = RSDK.GetSfx("Stage/Impact5.wav");
}

void CrimsonEye_Explode(void)
{
    RSDK_THIS(CrimsonEye);

    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(CrimsonEye->sfxExplosion, false, 255);

        int32 x = self->position.x;
        int32 y = self->position.y;

        if (CrimsonEye->health) {
            x += RSDK.Rand(-32, 33) << 16;
            y += RSDK.Rand(-32, 33) << 16;
        }
        else {
            x += RSDK.Rand(-19, 20) << 16;
            y += RSDK.Rand(-24, 25) << 16;
        }

        EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS)), x, y);
        explosion->drawGroup       = Zone->objectDrawGroup[1] + 2;
    }
}

void CrimsonEye_Hit(void)
{
    RSDK_THIS(CrimsonEye);

    if (CrimsonEye->health)
        CrimsonEye->health--;

    if (CrimsonEye->health) {
        if (CrimsonEye->health == 9) {
            CrimsonEye_DestroyBall();
            RSDK.PlaySfx(CrimsonEye->sfxExplosion, false, 0xFF);
            self->timer = 120;
            self->state = CrimsonEye_StateCore_BreakOut;
        }
        else {
            CrimsonEye_DestroyBall();
            RSDK.PlaySfx(CrimsonEye->sfxHit, false, 0xFF);
            CrimsonEye->invincibilityTimer = 48;
        }
    }
    else {
        RSDK.PlaySfx(CrimsonEye->sfxExplosion, false, 0xFF);
        self->timer = 120;

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 14);
        debris->velocity.x      = -0x30000;
        debris->velocity.y      = -0x40000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;
        debris->delay           = 120;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 15);
        debris->velocity.x      = 0x30000;
        debris->velocity.y      = -0x40000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;
        debris->delay           = 120;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 16);
        debris->velocity.x      = -0x20000;
        debris->velocity.y      = -0x20000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;
        debris->delay           = 120;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 17);
        debris->velocity.x      = 0x20000;
        debris->velocity.y      = -0x20000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;
        debris->delay           = 120;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 12);
        debris->velocity.x      = 0;
        debris->velocity.y      = -0x30000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 13);
        debris->velocity.x      = 0;
        debris->velocity.y      = 0;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x160000, self->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 4);
        debris->drawFX          = FX_ROTATE;
        debris->rotation        = self->rotation;
        debris->velocity.x      = -0x20000;
        debris->velocity.y      = -0x20000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x160000, self->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 5);
        debris->drawFX          = FX_ROTATE;
        debris->rotation        = 0x1FF - self->rotation;
        debris->velocity.x      = 0x20000;
        debris->velocity.y      = -0x20000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        int32 velX = RSDK.Rand(-6, 6) << 15;
        int32 velY = RSDK.Rand(-10, -6) << 15;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, CrimsonEye->eyePositions[0].x, CrimsonEye->eyePositions[0].y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 1);
        debris->velocity.x      = velX;
        debris->velocity.y      = velY;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;
        debris->delay           = 60;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, CrimsonEye->eyePositions[1].x, CrimsonEye->eyePositions[1].y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 2);
        debris->velocity.x      = velX;
        debris->velocity.y      = velY;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;
        debris->delay           = 60;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, CrimsonEye->eyePositions[2].x, CrimsonEye->eyePositions[2].y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 3);
        debris->velocity.x      = velX;
        debris->velocity.y      = velY;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;
        debris->delay           = 60;

        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        self->visible = false;
        self->state   = CrimsonEye_StateCore_Explode;
    }
}

void CrimsonEye_SetupBG2Layer(void)
{
    for (int32 y = 0; y < 48; ++y) {
        uint16 tile = RSDK.GetTile(1, 0, y + 24 * (2 - y / 24));

        RSDK.SetTile(1, 0, y, tile);
        RSDK.SetTile(1, 31, y, tile);

        for (int32 x = 1; x < 31; ++x) RSDK.SetTile(1, x, y, -1);
    }

    TileLayer *bg2Layer = RSDK.GetTileLayer(1);
    for (int32 i = 0; i < bg2Layer->scrollInfoCount; ++i) {
        bg2Layer->scrollInfo[i].parallaxFactor = 0x200;
        bg2Layer->scrollInfo[i].scrollPos      = -0x1C0000;
    }

    bg2Layer->drawGroup[0]   = Zone->hudDrawGroup - 1;
    bg2Layer->parallaxFactor = 0x200;
    CrimsonEye->bg2Layer     = bg2Layer;

    RSDK.SetDrawGroupProperties(Zone->hudDrawGroup - 1, false, CrimsonEye_DrawHook_EnableFGSilhouette);
    RSDK.SetDrawGroupProperties(Zone->hudDrawGroup, false, CrimsonEye_DrawHook_DisableFGSilhouette);
}

// Manages the black strucures that move in the foreground
void CrimsonEye_DrawHook_DisableFGSilhouette(void) { RSDK.SetActivePalette(0, 0, ScreenInfo->size.y); }
void CrimsonEye_DrawHook_EnableFGSilhouette(void) { RSDK.SetActivePalette(5, 0, ScreenInfo->size.y); }

void CrimsonEye_SetArrowDir(int32 type)
{
    foreach_active(CrimsonEye, boss)
    {
        if (boss->stateDraw == CrimsonEye_Draw_Arrow)
            boss->type = type;
    }
}

void CrimsonEye_StateContainer_SetupArena(void)
{
    RSDK_THIS(CrimsonEye);

    if (++self->timer >= 2) {
        self->timer = 0;

        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + ScreenInfo->center.x + 80;
        Zone->cameraBoundsB[0]      = (self->position.y >> 16) + 124;
        Zone->cameraBoundsT[0]      = Zone->cameraBoundsB[0] - ScreenInfo->size.y;

        self->active = ACTIVE_NORMAL;
        CREATE_ENTITY(TMZ1Setup, NULL, 0, 0);

        RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, CrimsonEye)->active = ACTIVE_NORMAL;
        for (int32 i = 2; i < 14; ++i) {
            EntityCrimsonEye *child = RSDK_GET_ENTITY(SceneInfo->entitySlot + i, CrimsonEye);
            child->active           = ACTIVE_NORMAL;
            if (i < 10)
                child->angle = 32 * (9 - i);
        }

        self->state = CrimsonEye_StateContainer_AwaitPlayer;
    }
}

void CrimsonEye_StateContainer_AwaitPlayer(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->ballSpinAngleX += 4;

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]      = ScreenInfo->position.x;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (!self->timer && ScreenInfo->position.x + ScreenInfo->center.x > (self->position.x >> 16) - 256) {
        CrimsonEye_SetupBG2Layer();
        self->timer = 1;
    }

    if (player1->position.x > self->position.x - 0x500000) {
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - ScreenInfo->center.x - 80;

        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        RSDK.PlaySfx(CrimsonEye->sfxElevator, false, 255);

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        if (!camera->shakePos.y)
            camera->shakePos.y = 4;

        self->timer = 15;
        self->state = CrimsonEye_StateContainer_StartFight;
    }
}

void CrimsonEye_StateContainer_StartFight(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->ballSpinAngleX += 4;

    if (!--self->timer) {
        self->stateEye = CrimsonEye_StateEye_EnterBalls;
        self->state    = CrimsonEye_StateContainer_CoreActive;
    }
}

void CrimsonEye_StateContainer_CoreActive(void)
{
    RSDK_THIS(CrimsonEye);

    StateMachine_Run(self->stateEye);

    if (CrimsonEye->elevatorSpeed == CrimsonEye->targetElevatorSpeed && CrimsonEye->elevatorGravity != 0x800 && CrimsonEye->health) {
        if (--self->timer == 30) {
            RSDK.PlaySfx(CrimsonEye->sfxBeep, false, 255);

            if (CrimsonEye->targetElevatorSpeed) {
                CrimsonEye->nextTargetElevatorSpeed = 0;
                CrimsonEye_SetArrowDir(CE_ARROW_OFF);
            }
            else if (RSDK.Rand(0, 3)) {
                CrimsonEye->nextTargetElevatorSpeed = -0x80000;
                CrimsonEye_SetArrowDir(CE_ARROW_UP);
            }
            else {
                CrimsonEye->nextTargetElevatorSpeed = 0x80000;
                CrimsonEye_SetArrowDir(CE_ARROW_DOWN);
            }
        }

        if (self->timer <= 0) {
            RSDK.PlaySfx(CrimsonEye->sfxElevator, false, 255);

            Camera_ShakeScreen(0, 4, 8);

            RSDK.PlaySfx(CrimsonEye->sfxHullClose, false, 255);

            self->timer                     = RSDK.Rand(120, 180);
            CrimsonEye->targetElevatorSpeed = CrimsonEye->nextTargetElevatorSpeed;

            EntityCrimsonEye *child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, CrimsonEye);

            if (CrimsonEye->targetElevatorSpeed != -0x80000) {
                if (!CrimsonEye->targetElevatorSpeed) {
                    child->velocity.y         = -CrimsonEye->elevatorSpeed;
                    CrimsonEye->elevatorSpeed = 0;
                }
                else {
                    if (CrimsonEye->targetElevatorSpeed == 0x80000)
                        child->velocity.y = CrimsonEye->elevatorSpeed >> 2;
                }
            }
            else
                child->velocity.y = CrimsonEye->elevatorSpeed >> 2;
        }
    }

    CrimsonEye_StateContainer_HandleElevator(); // its the exact same code

    if (CrimsonEye->health == 9)
        self->stateEye = StateMachine_None;

    if (CrimsonEye->health == 8 && !self->animator.frameID) {
        self->animator.frameID = 1;
        self->drawGroup        = Zone->objectDrawGroup[0];

        // Destroy Container
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = -0x20000;
        debris->velocity.y      = -0x20000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;
        debris->delay           = 60;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 0, &debris->animator, true, 3);
        debris->velocity.x      = 0x20000;
        debris->velocity.y      = -0x20000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;
        debris->delay           = 60;
    }

    if (self->drawGroup > Zone->objectDrawGroup[0]) {
        foreach_active(Ring, ring)
        {
            if (ring->drawGroup == Zone->objectDrawGroup[0])
                ring->drawGroup++;
        }
    }

    if (!CrimsonEye->health) {
        CrimsonEye->targetElevatorSpeed = 0;
        self->timer                     = 0;
        self->state                     = CrimsonEye_StateContainer_HandleElevator;

        foreach_active(CrimsonEye, part)
        {
            if (part->stateDraw == CrimsonEye_Draw_Arrow)
                part->type = CE_ARROW_OFF;
        }
    }
}

void CrimsonEye_StateContainer_HandleElevator(void)
{
    if (CrimsonEye->elevatorSpeed != CrimsonEye->targetElevatorSpeed) {
        if (CrimsonEye->elevatorSpeed >= CrimsonEye->targetElevatorSpeed) {
            CrimsonEye->elevatorSpeed -= 0x800;

            if (CrimsonEye->elevatorSpeed < CrimsonEye->targetElevatorSpeed)
                CrimsonEye->elevatorSpeed = CrimsonEye->targetElevatorSpeed;
        }
        else {
            CrimsonEye->elevatorSpeed += 0x800;

            if (CrimsonEye->elevatorSpeed > CrimsonEye->targetElevatorSpeed)
                CrimsonEye->elevatorSpeed = CrimsonEye->targetElevatorSpeed;
        }
    }

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        // NOTE:
        // according to IDA, the original code is
        // if (player->classID)
        // but this crashes the game when super & without a sidekick so this fix will stay
        if (player->classID == Player->classID)
            player->gravityStrength = 0x3800 - CrimsonEye->elevatorSpeed / 0x30;
    }

    TileLayer *elevatorLayer = CrimsonEye->liftBackground;
    elevatorLayer->scrollPos = -0x800000;
    for (int32 i = 0; i < elevatorLayer->scrollInfoCount; ++i) {
        elevatorLayer->scrollInfo[i].scrollSpeed = elevatorLayer->scrollInfo[i].parallaxFactor * (CrimsonEye->elevatorSpeed / 24);
    }

    CrimsonEye->bg2Layer->scrollSpeed = 6 * CrimsonEye->elevatorSpeed;
}

void CrimsonEye_StateContainer_Explode(void)
{
    RSDK_THIS(CrimsonEye);

    if (self->timer < 48) {
        if (!(Zone->timer & 3)) {
            RSDK.PlaySfx(ItemBox->sfxDestroy, false, 255);

            int32 x                    = self->position.x + RSDK.Rand(-0x200000, 0x200000);
            int32 y                    = self->position.y + RSDK.Rand(-0x800000, 0x800000);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), x, y);

            explosion->drawGroup = Zone->objectDrawGroup[1] + 2;
        }
    }

    if (++self->timer == 48) {
        Debris_CreateFromEntries(CrimsonEye->aniFrames, CrimsonEye->debrisInfo, 5);
        self->visible = false;
        Camera_ShakeScreen(0, 0, 4);
        self->state = CrimsonEye_StateContainer_MoveElevatorToTMZ2Entry;
    }
}

void CrimsonEye_StateContainer_MoveElevatorToTMZ2Entry(void)
{
    if (CrimsonEye->elevatorSpeed != CrimsonEye->targetElevatorSpeed) {
        if (CrimsonEye->elevatorSpeed >= CrimsonEye->targetElevatorSpeed) {
            CrimsonEye->elevatorSpeed -= 0x800;

            if (CrimsonEye->elevatorSpeed < CrimsonEye->targetElevatorSpeed)
                CrimsonEye->elevatorSpeed = CrimsonEye->targetElevatorSpeed;
        }
        else {
            CrimsonEye->elevatorSpeed += 0x800;

            if (CrimsonEye->elevatorSpeed > CrimsonEye->targetElevatorSpeed)
                CrimsonEye->elevatorSpeed = CrimsonEye->targetElevatorSpeed;
        }
    }

    TileLayer *liftLayer = CrimsonEye->liftBackground;
    liftLayer->scrollPos = -0x800000;
    for (int32 i = 0; i < liftLayer->scrollInfoCount; ++i) {
        liftLayer->scrollInfo[i].scrollSpeed = liftLayer->scrollInfo[i].parallaxFactor * (CrimsonEye->elevatorSpeed / 24);
    }

    CrimsonEye->bg2Layer->scrollSpeed = 6 * CrimsonEye->elevatorSpeed;

    TileLayer *moveLayer   = RSDK.GetTileLayer(Zone->moveLayer);
    moveLayer->scrollSpeed = (CrimsonEye->elevatorSpeed / 24) << 6;
}

void CrimsonEye_Draw_Container(void)
{
    RSDK_THIS(CrimsonEye);

    if (!self->animator.frameID && (CrimsonEye->invincibilityTimer & 1))
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    RSDK.DrawSprite(&self->animator, NULL, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void CrimsonEye_StateEye_EnterBalls(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->ballSpinAngleX += 4;
    CrimsonEye->ballOffsetY += 0x40000;

    if (CrimsonEye->ballOffsetY >= 0x800000) {
        CrimsonEye->ballOffsetY = 0x800000;
        self->stateEye          = CrimsonEye_StateEye_SpinningBalls;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_SpinningBalls(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->ballSpinAngleX += 4;

    if (++self->ballTimer == 120) {
        self->ballTimer = 0;
        self->stateEye  = CrimsonEye_StateEye_LowerBalls;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_LowerBalls(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->ballSpinAngleX += 4;
    CrimsonEye->ballOffsetY += self->velocity.y;

    self->velocity.y += 0x3800;

    if (CrimsonEye->ballOffsetY >= 0xEC0000) {
        CrimsonEye->ballOffsetY = 0xEC0000;

        self->velocity.y = (self->velocity.y >> 3) - (self->velocity.y >> 1);

        if (self->velocity.y > -0x10000)
            self->stateEye = CrimsonEye_StateEye_LoweredBalls;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_LoweredBalls(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->ballSpinAngleX += 4;

    if (++self->ballTimer == 120) {
        self->ballTimer = 0;
        self->stateEye  = CrimsonEye_StateEye_ExtendBallAttack;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_ExtendBallAttack(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->ballSpinAngleX += 4;
    self->ballRadiusTimer = (self->ballRadiusTimer + 1) & 0x7F;

    CrimsonEye->ballSpinRadius = 80 * RSDK.Sin256(self->ballRadiusTimer) + 0x4800;

    if (!self->ballRadiusTimer)
        self->stateEye = CrimsonEye_StateEye_FinishExtendAttack;

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_FinishExtendAttack(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->ballSpinAngleX += 4;

    if (++self->ballTimer == 120) {
        self->ballTimer = 0;
        self->stateEye  = CrimsonEye_StateEye_RaiseBalls;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_RaiseBalls(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->ballSpinAngleX += 4;
    CrimsonEye->ballOffsetY -= 0x40000;

    if (CrimsonEye->ballOffsetY <= 0x800000) {
        CrimsonEye->ballOffsetY = 0x800000;
        self->stateEye          = CrimsonEye_StateEye_SpinningBalls;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_ShootShot(void)
{
    RSDK_THIS(CrimsonEye);

    EntityCrimsonEye *child = CREATE_ENTITY(CrimsonEye, INT_TO_VOID(CRIMSONEYE_SHOT), self->position.x + 0x260000, self->position.y);
    child->velocity.x       = 0x280 * RSDK.Cos512(self->rotation);
    child->velocity.y       = -0x280 * RSDK.Sin512(self->rotation);
    child->position.y += self->rotation < 0x100 ? -0x110000 : 0x110000;

    child             = CREATE_ENTITY(CrimsonEye, INT_TO_VOID(CRIMSONEYE_SHOT), self->position.x - 0x260000, self->position.y);
    child->velocity.x = -0x280 * RSDK.Cos512(self->rotation);
    child->velocity.y = -0x280 * RSDK.Sin512(self->rotation);
    child->position.y += self->rotation < 0x100 ? -0x110000 : 0x110000;
    RSDK.PlaySfx(CrimsonEye->sfxShot, false, 255);
}

void CrimsonEye_SetupEyePositions(void)
{
    RSDK_THIS(CrimsonEye);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    // Eyeball
    CrimsonEye->eyePositions[0].x = self->position.x;
    CrimsonEye->eyePositions[0].y = self->position.y;

    CrimsonEye->eyePositions[0].x += CLAMP(10 * ((player1->position.x - self->position.x) >> 8), -0xC0000, 0xC0000);
    CrimsonEye->eyePositions[0].x &= 0xFFFF0000;

    // Eye shine
    CrimsonEye->eyePositions[2] = CrimsonEye->eyePositions[0];

    // Iris thingy
    CrimsonEye->eyePositions[1] = CrimsonEye->eyePositions[0];
    CrimsonEye->eyePositions[1].x += CLAMP(3 * ((player1->position.x - self->position.x) >> 8), -0x30000, 0x30000);
    CrimsonEye->eyePositions[1].x &= 0xFFFF0000;
}

void CrimsonEye_DestroyBall(void)
{
    if (CrimsonEye->destroyedBallCount < 8) {
        EntityCrimsonEye *child = RSDK_GET_ENTITY(SceneInfo->entitySlot + CrimsonEye->ballDestroyOrder[CrimsonEye->destroyedBallCount], CrimsonEye);
        child->state            = CrimsonEye_StateBall_Destroyed;
        child->timer            = 30;
        child->drawGroup        = Zone->objectDrawGroup[0];
        child->scale.y          = 0x200;
        child->scale.x          = 0x200;

        ++CrimsonEye->destroyedBallCount;
    }
}

void CrimsonEye_CheckPlayerCollisions(void)
{
    RSDK_THIS(CrimsonEye);

    if (CrimsonEye->invincibilityTimer) {
        CrimsonEye->invincibilityTimer--;
    }
    else {
        Vector2 storePos = self->position;
        foreach_active(Player, player)
        {
            self->position = CrimsonEye->eyePositions[0];
            if (Player_CheckBadnikTouch(player, self, &CrimsonEye->hitboxEye) && Player_CheckBossHit(player, self)) {
                self->position = storePos;
                CrimsonEye_Hit();
                foreach_break;
            }
        }
        self->position = storePos;
    }
}

void CrimsonEye_StateCore_ContainerActive(void)
{
    CrimsonEye_SetupEyePositions();
    CrimsonEye_CheckPlayerCollisions();
}

void CrimsonEye_StateCore_BreakOut(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye_SetupEyePositions();
    CrimsonEye_Explode();

    if (--self->timer <= 0) {
        self->timer = 90;
        --CrimsonEye->health;
        self->drawGroup = Zone->objectDrawGroup[0];
        self->state     = CrimsonEye_StateCore_BrokenOut;
    }
}

void CrimsonEye_StateCore_BrokenOut(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye_SetupEyePositions();
    CrimsonEye_Explode();

    if (--self->timer <= 0) {
        CrimsonEye->elevatorGravity = 0x4000;

        self->velocity.x = -0x10000;
        self->timer      = 90;
        self->drawGroup  = Zone->objectDrawGroup[0] + 1;
        self->state      = CrimsonEye_StateCore_Hovering;
    }
}

void CrimsonEye_StateCore_Hovering(void)
{
    RSDK_THIS(CrimsonEye);

    self->position.x += self->velocity.x;

    if (abs(self->velocity.y) <= 0x10000) {
        if (self->originPos.y <= (Zone->cameraBoundsB[0] - 144) << 16) {
            self->originPos.y += 0x10000;

            if (self->originPos.y > (Zone->cameraBoundsB[0] - 144) << 16)
                self->originPos.y = (Zone->cameraBoundsB[0] - 144) << 16;
        }
        else {
            self->originPos.y -= 0x10000;

            if (self->originPos.y < (Zone->cameraBoundsB[0] - 144) << 16)
                self->originPos.y = (Zone->cameraBoundsB[0] - 144) << 16;
        }
    }

    self->originPos.y -= self->velocity.y;

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);
    if (self->position.x <= (Zone->cameraBoundsL[0] + 64) << 16 || self->position.x >= (Zone->cameraBoundsR[0] - 64) << 16) {
        self->velocity.x = -self->velocity.x;
    }

    int32 storeY     = self->position.y;
    self->position.y = self->originPos.y;

    foreach_active(InvisibleBlock, block)
    {
        int32 side = RSDK.CheckObjectCollisionBox(block, &block->hitbox, self, &CrimsonEye->hitboxBlock, true);
        if (side == C_TOP) {
            if (self->velocity.y < 0)
                self->velocity.y = 0;

            self->rotation             = 0x40;
            CrimsonEye->shotsRemaining = 8;
            CrimsonEye->shotTimer      = 15;

            self->state = CrimsonEye_StateCore_ImpactLift;

            RSDK.PlaySfx(CrimsonEye->sfxButton, false, 0xFF);
            RSDK.PlaySfx(CrimsonEye->sfxImpact, false, 0xFF);
        }
        else if (side == C_BOTTOM) {
            if (self->velocity.y > 0)
                self->velocity.y = 0;

            self->rotation             = 0x1C0;
            CrimsonEye->shotsRemaining = 8;
            CrimsonEye->shotTimer      = 15;

            self->state = CrimsonEye_StateCore_ImpactLift;

            RSDK.PlaySfx(CrimsonEye->sfxButton, false, 0xFF);
            RSDK.PlaySfx(CrimsonEye->sfxImpact, false, 0xFF);
        }
    }

    self->originPos.y = self->position.y;
    self->position.y  = storeY;

    CrimsonEye_SetupEyePositions();
    CrimsonEye_CheckPlayerCollisions();

    if (!(Zone->timer & 0x3F))
        RSDK.PlaySfx(CrimsonEye->sfxHover2, false, 255);
}

void CrimsonEye_StateCore_ImpactLift(void)
{
    RSDK_THIS(CrimsonEye);

    if (--CrimsonEye->shotTimer <= 0) {
        CrimsonEye->shotTimer = 15;

        if (--CrimsonEye->shotsRemaining != 0xFF)
            CrimsonEye_ShootShot();
        else
            self->state = CrimsonEye_StateCore_Hovering;
    }

    CrimsonEye_SetupEyePositions();
    CrimsonEye_CheckPlayerCollisions();
}

void CrimsonEye_StateCore_Explode(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye_Explode();

    if (--self->timer <= 0) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->state = CrimsonEye_StateCore_SpawnSignPost;
    }
}

void CrimsonEye_StateCore_SpawnSignPost(void)
{
    RSDK_THIS(CrimsonEye);

    if (++self->timer == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = self->position.x;
            signPost->position.y = (ScreenInfo->position.y - 64) << 16;
            signPost->state      = SignPost_State_Falling;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
        }

        destroyEntity(self);
    }
}

void CrimsonEye_Draw_Core(void)
{
    RSDK_THIS(CrimsonEye);

    if (CrimsonEye->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    if (self->state != CrimsonEye_StateCore_ContainerActive && self->state != CrimsonEye_StateCore_BreakOut) {
        self->animator.frameID = 12;
        RSDK.DrawSprite(&self->animator, NULL, false);

        self->animator.frameID = 13;
        RSDK.DrawSprite(&self->animator, NULL, false);

        self->drawFX |= FX_ROTATE;
        Vector2 drawPos        = self->position;
        drawPos.x              = self->position.x - 0x160000;
        self->animator.frameID = 4;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->rotation = 0x1FF - self->rotation;
        drawPos.x += 0x2C0000;
        self->animator.frameID = 5;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->drawFX &= ~FX_ROTATE;
        self->rotation         = 0x1FF - self->rotation;
        self->animator.frameID = 0;
        RSDK.DrawSprite(&self->animator, NULL, false);
    }

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &CrimsonEye->eyePositions[0], false);

    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &CrimsonEye->eyePositions[1], false);

    self->inkEffect        = INK_ALPHA;
    self->alpha            = 0xC0;
    self->animator.frameID = 3;
    RSDK.DrawSprite(&self->animator, &CrimsonEye->eyePositions[2], false);

    self->inkEffect = INK_NONE;
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void CrimsonEye_CheckPlayerCollisions_Ball(void)
{
    RSDK_THIS(CrimsonEye);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &CrimsonEye->hitboxBall)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x600, 2, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }
}

void CrimsonEye_StateBall_Spinning(void)
{
    RSDK_THIS(CrimsonEye);

    uint8 angle = ((self->angle & 0xFF) + CrimsonEye->ballSpinAngleX);

    self->position.x = CrimsonEye->ballSpinRadius * RSDK.Cos256(angle) + CrimsonEye->containerPos.x;
    self->position.y = (RSDK.Sin256(CrimsonEye->ballSpinAngleY) << 13) + CrimsonEye->containerPos.y + CrimsonEye->ballOffsetY;

    if (angle <= 0x80) {
        self->drawGroup        = Zone->objectDrawGroup[0];
        self->animator.frameID = 0;
        self->scale.x          = 2 * abs(angle - 0x40) + 0x182;
        self->scale.y          = 2 * abs(angle - 0x40) + 0x182;
    }
    else {
        self->animator.frameID = 1;
        self->drawGroup        = Zone->objectDrawGroup[0] + 1;
        self->scale.x          = 2 * (0x13F - abs(angle - 0xC0));
        self->scale.y          = 2 * (0x13F - abs(angle - 0xC0));

        CrimsonEye_CheckPlayerCollisions_Ball();
    }
}

void CrimsonEye_StateBall_Destroyed(void)
{
    RSDK_THIS(CrimsonEye);

    self->position.y -= 0x18000;

    CrimsonEye_CheckPlayerCollisions_Ball();

    if (--self->timer <= 0) {
        RSDK.PlaySfx(CrimsonEye->sfxExplosion, false, 255);
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1] + 2;

        int32 angle = 0;
        for (int32 i = 0; i < 8; ++i) {
            EntityCrimsonEye *spike = CREATE_ENTITY(CrimsonEye, INT_TO_VOID(CRIMSONEYE_SPIKE), self->position.x, self->position.y);
            spike->animator.frameID = i + 2;
            spike->velocity.x       = RSDK.Cos256(angle) << 9;
            spike->velocity.y       = RSDK.Sin256(angle) << 9;
            angle += 32;
        }

        destroyEntity(self);
    }
}

void CrimsonEye_Draw_Simple(void)
{
    RSDK_THIS(CrimsonEye);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void CrimsonEye_StateSpike_Harmful(void)
{
    RSDK_THIS(CrimsonEye);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &CrimsonEye->hitboxOrb)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x600, 2, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void CrimsonEye_StateSpike_Debris(void)
{
    RSDK_THIS(CrimsonEye);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->visible ^= true;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void CrimsonEye_State_Shot(void)
{
    RSDK_THIS(CrimsonEye);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &CrimsonEye->hitboxElecOrb)) {
            Player_ElementHurt(player, self, SHIELD_LIGHTNING);
        }
    }

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, true)
        || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, -0x80000, true)) {
        self->velocity.y = -self->velocity.y;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void CrimsonEye_State_Arrow(void)
{
    // hi poe
}

void CrimsonEye_Draw_Arrow(void)
{
    RSDK_THIS(CrimsonEye);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->animator.frameID = self->type;
    if (self->type != CE_ARROW_OFF)
        RSDK.DrawSprite(&self->animator, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void CrimsonEye_EditorDraw(void)
{
    RSDK_THIS(CrimsonEye);

    switch (self->type) {
        case CRIMSONEYE_CONTAINER:
            self->drawGroup = Zone->objectDrawGroup[0] + 1;

            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 0, &self->animator, true, 0);

            self->updateRange.x        = 0x800000;
            self->updateRange.y        = 0x800000;
            CrimsonEye->containerPos.x = self->position.x;
            CrimsonEye->containerPos.y = self->position.y - 0xA00000;

            self->stateDraw = CrimsonEye_Draw_Container;
            break;

        case CRIMSONEYE_CORE:
            self->drawGroup = Zone->objectDrawGroup[0] + 1;

            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &self->animator, true, 0);

            self->originPos     = self->position;
            self->stateDraw     = CrimsonEye_Draw_Core;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;

            // Eyeball
            CrimsonEye->eyePositions[0].x = self->position.x & 0xFFFF0000;
            CrimsonEye->eyePositions[0].y = self->position.y;

            // Iris thingy
            CrimsonEye->eyePositions[1] = CrimsonEye->eyePositions[0];

            // Eye shine
            CrimsonEye->eyePositions[2] = CrimsonEye->eyePositions[0];
            break;

        case CRIMSONEYE_BALL:
            self->drawFX    = FX_SCALE;
            self->drawGroup = Zone->objectDrawGroup[0] + 1;

            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 2, &self->animator, true, 0);
            self->scale.x = 0x200;
            self->scale.y = 0x200;

            self->stateDraw     = CrimsonEye_Draw_Simple;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            break;

        case CRIMSONEYE_ARROW:
            self->drawGroup = Zone->objectDrawGroup[0];

            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 3, &self->animator, true, 0);
            self->scale.x = 0x200;
            self->scale.y = 0x200;

            self->stateDraw     = CrimsonEye_Draw_Arrow;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            break;

        case CRIMSONEYE_SPIKE:
            self->drawGroup = Zone->objectDrawGroup[0] + 1;

            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 2, &self->animator, true, 0);

            self->stateDraw     = CrimsonEye_Draw_Simple;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            break;

        case CRIMSONEYE_SHOT:
            self->drawGroup = Zone->objectDrawGroup[0] + 1;

            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 4, &self->animator, true, 0);

            self->stateDraw     = CrimsonEye_Draw_Simple;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            break;

        default: break;
    }

    StateMachine_Run(self->stateDraw);

    if (showGizmos() && self->type == CRIMSONEYE_CONTAINER) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER - 80, -SCREEN_YSIZE, WIDE_SCR_XCENTER + 80, 124, 1 | 2 | 4 | 8, 0x00C0F0);

        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < 9; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s + i);
            if (!child)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xE0E0E0, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void CrimsonEye_EditorLoad(void)
{
    CrimsonEye->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/CrimsonEye.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(CrimsonEye, type);
    RSDK_ENUM_VAR("Container", CRIMSONEYE_CONTAINER);
    RSDK_ENUM_VAR("Core", CRIMSONEYE_CORE);
    RSDK_ENUM_VAR("Ball", CRIMSONEYE_BALL);
    RSDK_ENUM_VAR("Arrow", CRIMSONEYE_ARROW);
}
#endif

void CrimsonEye_Serialize(void) { RSDK_EDITABLE_VAR(CrimsonEye, VAR_ENUM, type); }
