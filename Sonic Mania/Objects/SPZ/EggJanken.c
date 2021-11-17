#include "SonicMania.h"

ObjectEggJanken *EggJanken = NULL;

void EggJanken_Update(void)
{
    RSDK_THIS(EggJanken);

    entity->solidMoveOffset.x = entity->position.x & 0xFFFF0000;
    entity->solidPos.x        = entity->position.x & 0xFFFF0000;
    entity->solidMoveOffset.y = entity->position.y & 0xFFFF0000;
    entity->solidPos.y        = entity->position.y & 0xFFFF0000;

    if (entity->invincibilityTimer) {
        entity->invincibilityTimer--;
        if (!(entity->invincibilityTimer & 1)) {
            if (entity->invincibilityTimer & 2)
                RSDK.SetPaletteEntry(0, 128, 0xFFFFFF);
            else
                RSDK.SetPaletteEntry(0, 128, 0x000000);
        }
    }
    entity->state1();

    entity->rotation   = entity->fullRotation >> 8;
    entity->position.x = entity->startPos.x;
    entity->position.y = entity->startPos.y;
    if (entity->isMoving)
        EggJanken_HandleMovement();
    entity->solidMoveOffset.x -= entity->position.x & 0xFFFF0000;
    entity->solidMoveOffset.y -= entity->position.y & 0xFFFF0000;
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator5);

    entity->state2();

    for (entity->armID = 0; entity->armID < EggJanken_ArmCount; ++entity->armID) {
        for (entity->armJointID = 0; entity->armJointID < EggJanken_SegmentCount; ++entity->armJointID) {
            entity->stateArm[entity->armID]();
        }
    }

    if (entity->stateDraw != EggJanken_StateDraw_Unknown3)
        EggJanken_CheckPlayerCollisions();
}

void EggJanken_LateUpdate(void) {}

void EggJanken_StaticUpdate(void) {}

void EggJanken_Draw(void)
{
    RSDK_THIS(EggJanken);
    StateMachine_Run(entity->stateDraw);
}

void EggJanken_Create(void *data)
{
    RSDK_THIS(EggJanken);

    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 7, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 2, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 4, &entity->animator4, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 5, &entity->animator5, true, 0);
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->visible       = false;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->drawFX |= FX_ROTATE | FX_FLIP;
    entity->startPos.x        = entity->position.x;
    entity->startPos.y        = entity->position.y;
    entity->yCap              = entity->position.y + 0x100000;
    entity->state2            = EggJanken_State2_Unknown3;
    entity->eyeFrames[0]      = 3;
    entity->eyeFrames[1]      = 4;
    entity->health            = 3;
    entity->animator3.frameID = 1;
    entity->stateArm[0]       = EggJanken_StateArm_Unknown1;
    entity->stateArm[1]       = EggJanken_StateArm_Unknown1;
    entity->state1            = EggJanken_State1_SetupArena;
}

void EggJanken_StageLoad(void)
{
    EggJanken->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggJanken.bin", SCOPE_STAGE);

    EggJanken->hitbox1.left   = -32;
    EggJanken->hitbox1.top    = -35;
    EggJanken->hitbox1.right  = 32;
    EggJanken->hitbox1.bottom = 29;

    EggJanken->hitbox2.left   = -16;
    EggJanken->hitbox2.top    = 29;
    EggJanken->hitbox2.right  = 16;
    EggJanken->hitbox2.bottom = 37;

    EggJanken->hitbox3.left   = -16;
    EggJanken->hitbox3.top    = 29;
    EggJanken->hitbox3.right  = 16;
    EggJanken->hitbox3.bottom = 41;

    EggJanken->hitbox4.left   = -12;
    EggJanken->hitbox4.top    = -12;
    EggJanken->hitbox4.right  = 12;
    EggJanken->hitbox4.bottom = 12;

    EggJanken->stateJankenResult[0] = EggJanken_StateResult_Draw;
    EggJanken->stateJankenResult[3] = EggJanken_StateResult_Lose;
    EggJanken->stateJankenResult[6] = EggJanken_StateResult_Win;
    EggJanken->stateJankenResult[1] = EggJanken_StateResult_Win;
    EggJanken->stateJankenResult[4] = EggJanken_StateResult_Draw;
    EggJanken->stateJankenResult[7] = EggJanken_StateResult_Lose;
    EggJanken->stateJankenResult[2] = EggJanken_StateResult_Lose;
    EggJanken->stateJankenResult[5] = EggJanken_StateResult_Win;
    EggJanken->stateJankenResult[8] = EggJanken_StateResult_Draw;
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 3, &EggJanken->animator, true, 0);
    EggJanken->sfxBeep3     = RSDK.GetSFX("Stage/Beep3.wav");
    EggJanken->sfxBeep4     = RSDK.GetSFX("Stage/Beep4.wav");
    EggJanken->sfxFail      = RSDK.GetSFX("Stage/Fail.wav");
    EggJanken->sfxClick     = RSDK.GetSFX("Stage/Click.wav");
    EggJanken->sfxHit       = RSDK.GetSFX("Stage/BossHit.wav");
    EggJanken->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    EggJanken->sfxDrop      = RSDK.GetSFX("Stage/Drop.wav");
    EggJanken->sfxImpact4   = RSDK.GetSFX("Stage/Impact4.wav");
    EggJanken->sfxImpact3   = RSDK.GetSFX("Stage/Impact3.wav");
}

void EggJanken_CheckPlayerCollisions(void)
{
    RSDK_THIS(EggJanken);

    int storeX = entity->position.x;
    int storeY = entity->position.y;

    foreach_active(Player, player)
    {
        entity->position.x = entity->solidPos.x;
        entity->position.y = entity->solidPos.y;
        switch (Player_CheckCollisionBox(player, entity, &EggJanken->hitbox1)) {
            default: break;
            case C_TOP:
                player->position.x -= entity->solidMoveOffset.x;
                player->position.y -= entity->solidMoveOffset.y;
                player->position.y &= 0xFFFF0000;
                break;
            case C_BOTTOM:
                if (player->onGround)
                    player->hurtFlag = 1;
                break;
        }

        entity->position.x = storeX;
        entity->position.y = storeY;

        if (Player_CheckValidState(player)) {
            Player_CheckCollisionBox(player, entity, &EggJanken->hitbox2);

            if (Player_CheckCollisionTouch(player, entity, &EggJanken->hitbox3)) {
                if (player->velocity.y < 0 && !entity->animator3.frameID) {
                    entity->animator3.frameID = 1;
                    if (entity->state1 != EggJanken_State1_Unknown2) {
                        RSDK.StopSFX(EggJanken->sfxBeep3);
                        RSDK.PlaySfx(EggJanken->sfxClick, false, 255);
                        entity->state1           = EggJanken_State1_Unknown7;
                        entity->stateArm[0]      = EggJanken_StateArm_Unknown8;
                        entity->stateArm[1]      = EggJanken_StateArm_Unknown8;
                        entity->state2           = EggJanken_State2_Unknown3;
                        entity->jankenResult2[0] = entity->eyeFrames[0];
                        entity->jankenResult2[1] = entity->eyeFrames[1];
                        entity->eyeFrames[0]     = 11;
                        entity->eyeFrames[1]     = 11;
                        entity->storedXVel       = entity->velocity.x;
                        entity->velocity.x       = 0;
                        entity->timer            = 0;
                    }
                }
            }

            for (int i = 0; i < EggJanken_ArmCount; ++i) {
                entity->position.x = entity->armPos[i].x;
                entity->position.y = entity->armPos[i].y;
                if (Player_CheckCollisionTouch(player, entity, &EggJanken->hitbox4)) {
                    // This object goes unused so it wasn't updated for plus, but if it was there'd 100% be a checkMightyUnspin call here
                    Player_CheckHit(player, entity);
                }
            }
            entity->position.x = storeX;
            entity->position.y = storeY;
        }
    }
}

void EggJanken_HandleMovement(void)
{
    RSDK_THIS(EggJanken);

    if (entity->velocity.x >= 0) {
        if (entity->startPos.x >= (RSDK_screens->position.x + RSDK_screens->width - 48) << 16)
            entity->velocity.x = -entity->velocity.x;
    }
    else if (entity->startPos.x <= (RSDK_screens->position.x + 48) << 16)
        entity->velocity.x = -entity->velocity.x;

    entity->startPos.x += entity->velocity.x;

    // this is not the same code as BadnikHelpers_Oscillate, guess this is an older variant?
    entity->position.y += RSDK.Sin256(entity->angle) << 10;
    entity->angle = (entity->angle + 4) & 0xFF;
}

void EggJanken_StateResult_Win(void)
{
    RSDK_THIS(EggJanken);

    RSDK.PlaySfx(EggJanken->sfxBeep4, false, 255);
    entity->state1 = EggJanken_State1_JankenWin;
}

void EggJanken_StateResult_Lose(void)
{
    RSDK_THIS(EggJanken);

    RSDK.PlaySfx(EggJanken->sfxFail, false, 255);
    entity->state1 = EggJanken_State1_JankenLose;
}

void EggJanken_StateResult_Draw(void)
{
    RSDK_THIS(EggJanken);

    RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
    entity->state1 = EggJanken_State1_JankenDraw;
}

void EggJanken_State1_SetupArena(void)
{
    RSDK_THIS(EggJanken);

    if (++entity->timer >= 2) {
        entity->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + RSDK_screens->centerX;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 208;
        entity->startPos.y -= 0x1000000;
        entity->active = ACTIVE_NORMAL;
        entity->state1 = EggJanken_State1_StartFight;
    }
}

void EggJanken_State1_StartFight(void)
{
    RSDK_THIS(EggJanken);

    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = RSDK_screens->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x) {
        entity->visible             = true;
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX;
        entity->state1              = EggJanken_State1_Unknown1;
        entity->stateDraw           = EggJanken_StateDraw_Unknown1;
        entity->timer               = 272;
    }
}

void EggJanken_State1_Unknown1(void)
{
    RSDK_THIS(EggJanken);

    if (!--entity->timer) {
        entity->isMoving          = true;
        entity->animator3.frameID = 0;
        RSDK.PlaySfx(EggJanken->sfxClick, false, 255);
        entity->state1 = EggJanken_State1_Unknown2;
    }
    else {
        entity->startPos.y += 0x10000;
    }
}

void EggJanken_State1_Unknown2(void)
{
    RSDK_THIS(EggJanken);

    if (entity->animator3.frameID == 1) {
        RSDK.SetSpriteAnimation(EggJanken->aniFrames, 8, &entity->animator2, true, 0);
        entity->state1 = EggJanken_State1_Unknown3;

        EntityEggJankenPart *part = CREATE_ENTITY(EggJankenPart, intToVoid(1), entity->position.x, entity->position.y);
        part->velocity.x          = -0x20000;
        part->velocity.y          = -0x20000;
        part->angle               = -1;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(2), entity->position.x, entity->position.y);
        part->velocity.x = 0x20000;
        part->velocity.y = -0x20000;
        part->angle      = 1;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(3), entity->position.x, entity->position.y);
        part->velocity.x = -0x10000;
        part->velocity.y = -0x10000;
        part->angle      = -1;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(4), entity->position.x, entity->position.y);
        part->velocity.x = 0x10000;
        part->velocity.y = -0x10000;
        part->angle      = 1;

        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
    }
}

void EggJanken_State1_Unknown3(void)
{
    RSDK_THIS(EggJanken);

    if (++entity->timer >= 120) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(EggJanken->aniFrames, 1, &entity->animator2, true, 0);
        entity->state1    = EggJanken_State1_Unknown4;
        entity->stateDraw = EggJanken_StateDraw_Unknown2;
        for (entity->armID = 0; entity->armID < EggJanken_ArmCount; ++entity->armID) {
            entity->armRadiusSpeed[entity->armID] = 0x40000;
            for (int s = 0; s < EggJanken_SegmentCount; ++s) entity->jointAngles[EggJanken_SegmentCount * entity->armID + s] = 0xC0;
            entity->stateArm[entity->armID]                                 = EggJanken_StateArm_Unknown2;
        }

        EntityEggJankenPart *part = CREATE_ENTITY(EggJankenPart, intToVoid(5), entity->position.x, entity->position.y);
        part->velocity.x          = -0x40000;
        part->velocity.y          = -0x20000;
        part->angle               = -3;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(6), entity->position.x, entity->position.y);
        part->velocity.x = -0x30000;
        part->velocity.y = -0x10000;
        part->angle      = -3;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(5), entity->position.x, entity->position.y);
        part->direction  = FLIP_X;
        part->velocity.x = 0x40000;
        part->velocity.y = -0x20000;
        part->angle      = 3;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(6), entity->position.x, entity->position.y);
        part->direction  = FLIP_X;
        part->velocity.x = 0x30000;
        part->velocity.y = -0x10000;
        part->angle      = 3;

        RSDK.PlaySfx(EggJanken->sfxHit, false, 255);
    }
}

void EggJanken_State1_Unknown4(void)
{
    RSDK_THIS(EggJanken);

    if (entity->radius < 0x2300)
        entity->radius += 0x100;
    if (entity->stateArm[0] == EggJanken_StateArm_Unknown7) {
        for (entity->armID = 0; entity->armID < EggJanken_ArmCount; ++entity->armID) {
            entity->jointFlags[entity->armID]     = 0;
            entity->jointAngleVels[entity->armID] = 4;
            for (entity->armJointID = 0; entity->armJointID < EggJanken_SegmentCount; ++entity->armJointID) {
                entity->jointAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID]       = 0;
                entity->jointTargetAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] = 0;
                entity->jointDirection[EggJanken_SegmentCount * entity->armID + entity->armJointID]    = 0;
                entity->jointDelays[EggJanken_SegmentCount * entity->armID + entity->armJointID]       = 4 * entity->armJointID;
            }
        }

        entity->stateArm[0] = EggJanken_StateArm_Unknown3;
        entity->stateArm[1] = EggJanken_StateArm_Unknown3;
        entity->state1      = EggJanken_State1_Unknown5;
    }
}

void EggJanken_State1_Unknown5(void)
{
    RSDK_THIS(EggJanken);

    if (entity->stateArm[0] == EggJanken_StateArm_Unknown7) {
        entity->stateArm[0] = EggJanken_StateArm_Unknown4;
        entity->stateArm[1] = EggJanken_StateArm_Unknown4;
    }

    if ((entity->jointAngles[0] & 0xF8) == 0x88) {
        if ((entity->jointAngles[1] & 0xF8) == 0x80) {
            entity->velocity.x = 0x10000;
            entity->state1     = EggJanken_State1_Unknown6;
            entity->state2     = EggJanken_State2_Unknown1;
        }
    }
}

void EggJanken_State1_Unknown6(void) {}

void EggJanken_State1_Destroyed(void)
{
    RSDK_THIS(EggJanken);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(EggJanken->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x                                                                 = entity->position.x + (RSDK.Rand(-24, 24) << 16);
            int y                                                                 = entity->position.y + (RSDK.Rand(-24, 24) << 16);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }

    EntityEggJankenPart *part = NULL;
    switch (++entity->timer) {
        case 60:
            part                      = CREATE_ENTITY(EggJankenPart, intToVoid(11), entity->position.x, entity->position.y);
            entity->animator3.frameID = 2;
            break;
        case 90:
            entity->stateDraw = EggJanken_StateDraw_Unknown1;
            entity->armID     = 0;
            for (entity->armID = 0; entity->armID < EggJanken_ArmCount; ++entity->armID) {
                part             = CREATE_ENTITY(EggJankenPart, intToVoid(7), entity->armPos[entity->armID].x, entity->armPos[entity->armID].y);
                part->velocity.x = -0x20000;
                part->velocity.y = -0x20000;

                part             = CREATE_ENTITY(EggJankenPart, intToVoid(8), entity->armPos[entity->armID].x, entity->armPos[entity->armID].y);
                part->velocity.x = 0x20000;
                part->velocity.y = -0x20000;

                part             = CREATE_ENTITY(EggJankenPart, intToVoid(9), entity->armPos[entity->armID].x, entity->armPos[entity->armID].y);
                part->velocity.x = -0x10000;
                part->velocity.y = -0x10000;

                part             = CREATE_ENTITY(EggJankenPart, intToVoid(10), entity->armPos[entity->armID].x, entity->armPos[entity->armID].y);
                part->velocity.x = 0x10000;
                part->velocity.y = -0x10000;

                entity->armPos[entity->armID].x = 0;
                entity->armPos[entity->armID].y = 0;
            }
            break;
        case 120:
            entity->stateDraw = EggJanken_StateDraw_Unknown3;
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
            break;
        case 180: {
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
            EntitySignPost *signPost = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, SignPost);
            signPost->position.x     = entity->position.x;
            signPost->state          = SignPost_State_Fall;
            entity->state1           = EggJanken_State1_Unknown6;
            break;
        }
        default: break;
    }
    if (entity->timer > 120) {
        entity->startPos.y += entity->velocity.y;
        entity->position.y = entity->startPos.y;
        entity->velocity.y += 0x3800;
    }
}

void EggJanken_State1_Unknown7(void)
{
    RSDK_THIS(EggJanken);

    if (entity->stateArm[0] == EggJanken_StateArm_Unknown7 && ++entity->timer == 30) {
        int choice1          = entity->jankenResult2[0];
        int choice2          = entity->jankenResult2[1];
        entity->eyeFrames[0] = choice1;
        entity->eyeFrames[1] = choice2;
        EggJanken->stateJankenResult[3 * choice1 + choice2]();
        entity->timer = 0;
    }
}

void EggJanken_State1_JankenWin(void)
{
    RSDK_THIS(EggJanken);

    if (++entity->timer == 60) {
        entity->eyeFrames[0] = 5;
        entity->eyeFrames[1] = 6;
    }

    if (entity->timer == 90) {
        for (entity->armID = 0; entity->armID < EggJanken_ArmCount; ++entity->armID) {
            entity->armRadiusSpeed[entity->armID] = 0x40000;
            for (int s = 0; s < EggJanken_SegmentCount; ++s) entity->jointAngles[EggJanken_SegmentCount * entity->armID + s] = 0xC0;
            entity->stateArm[entity->armID]                                 = EggJanken_StateArm_Unknown2;
        }
        entity->state1 = EggJanken_State1_Unknown9;
        entity->timer  = 0;
    }

    if (entity->timer > 60 && (Zone->timer & 1)) {
        if (entity->fullRotation == 0x800)
            entity->fullRotation = 0x1F800;
        else
            entity->fullRotation = 0x800;
    }
}

void EggJanken_State1_Unknown9(void)
{
    RSDK_THIS(EggJanken);

    if (Zone->timer & 1) {
        if (entity->fullRotation == 0x800)
            entity->fullRotation = 0x1F800;
        else
            entity->fullRotation = 0x800;
    }

    if (entity->stateArm[0] == EggJanken_StateArm_Unknown7) {
        entity->fullRotation = 0;
        entity->timer        = 0;
        if (!entity->health) {
            entity->isMoving            = false;
            RSDK_sceneInfo->timeEnabled = false;
            entity->state1              = EggJanken_State1_Destroyed;
            EntityPlayer *player1       = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            if (player1->superState == SUPERSTATE_SUPER)
                player1->superState = SUPERSTATE_FADEOUT;
            Player_GiveScore(player1, 1000);
            entity->invincibilityTimer = 0;
        }
        else {
            entity->state1 = EggJanken_State1_Unknown10;
        }
    }
}

void EggJanken_Unknown18(void)
{
    RSDK_THIS(EggJanken);

    entity->state1      = EggJanken_State1_Unknown11;
    entity->stateArm[0] = EggJanken_StateArm_Unknown3;
    entity->stateArm[1] = EggJanken_StateArm_Unknown3;

    for (entity->armID = 0; entity->armID < EggJanken_ArmCount; ++entity->armID) {
        entity->jointFlags[entity->armID]     = 0;
        entity->jointAngleVels[entity->armID] = 4;
        for (entity->armJointID = 0; entity->armJointID < EggJanken_SegmentCount; ++entity->armJointID) {
            entity->jointAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID]       = 0;
            entity->jointTargetAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] = 0;
            entity->jointDirection[EggJanken_SegmentCount * entity->armID + entity->armJointID]    = 0;
            entity->jointDelays[EggJanken_SegmentCount * entity->armID + entity->armJointID]       = 4 * entity->armJointID;
        }
    }

    entity->eyeFrames[0] = 3;
    entity->eyeFrames[1] = 4;
}

void EggJanken_State1_Unknown10(void)
{
    RSDK_THIS(EggJanken);

    if (!entity->invincibilityTimer && ++entity->timer == 30) {
        EggJanken_Unknown18();
        entity->timer = 0;
    }
}

void EggJanken_State1_Unknown11(void)
{
    RSDK_THIS(EggJanken);

    if (entity->stateArm[0] == EggJanken_StateArm_Unknown7) {
        entity->state1      = EggJanken_State1_Unknown12;
        entity->stateArm[0] = EggJanken_StateArm_Unknown4;
        entity->stateArm[1] = EggJanken_StateArm_Unknown4;
    }
}

void EggJanken_State1_Unknown12(void)
{
    RSDK_THIS(EggJanken);

    if ((entity->jointAngles[0] & 0xF8) == 0x88) {
        if ((entity->jointAngles[1] & 0xF8) == 0x80) {
            entity->velocity.x = entity->storedXVel;
            entity->state1     = EggJanken_State1_Unknown6;
            entity->state2     = EggJanken_State2_Unknown1;
        }
    }
}

void EggJanken_State1_JankenDraw(void)
{
    RSDK_THIS(EggJanken);

    if (++entity->timer == 60) {
        entity->eyeFrames[0] = 10;
        entity->eyeFrames[1] = 10;
    }

    if (entity->timer == 120) {
        entity->state1                                 = EggJanken_State1_Unknown14;
        entity->attackingArmID                         = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= entity->position.x;
        entity->armRadiusSpeed[entity->attackingArmID] = 0x40000;
        entity->jointAngles[EggJanken_SegmentCount * entity->attackingArmID + 3] = 0xC0;
        entity->jointAngles[EggJanken_SegmentCount * entity->attackingArmID + 2] = 0xC0;
        entity->jointAngles[EggJanken_SegmentCount * entity->attackingArmID + 1] = 0xC0;
        entity->jointAngles[EggJanken_SegmentCount * entity->attackingArmID + 0] = 0xC0;
        entity->stateArm[entity->attackingArmID]                                 = EggJanken_StateArm_Unknown3;
        entity->timer                                                            = 0;
    }
}

void EggJanken_State1_Unknown14(void)
{
    RSDK_THIS(EggJanken);

    if (entity->stateArm[entity->attackingArmID] == EggJanken_StateArm_Unknown7) {
        entity->fullRotation = 0;

        entity->jointAngleVels[entity->attackingArmID] = 8;
        for (entity->armJointID = 0; entity->armJointID < EggJanken_SegmentCount; ++entity->armJointID) {
            int slot                        = EggJanken_SegmentCount * entity->attackingArmID + entity->armJointID;
            entity->jointDelays[slot]       = 4 * entity->armJointID;
            entity->jointTargetAngles[slot] = 240;
            entity->jointDirection[slot]    = 0;
        }

        entity->stateArm[entity->attackingArmID] = EggJanken_StateArm_Unknown5;
        entity->state1                           = EggJanken_State1_Unknown15;
    }
}

void EggJanken_State1_Unknown15(void)
{
    RSDK_THIS(EggJanken);

    if (entity->stateArm[entity->attackingArmID] == EggJanken_StateArm_Unknown7) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        entity->jointAngleVels[entity->attackingArmID] = 8;
        for (entity->armJointID = 0; entity->armJointID < EggJanken_SegmentCount; ++entity->armJointID) {
            int slot = EggJanken_SegmentCount * entity->attackingArmID + entity->armJointID;

            entity->jointDelays[slot] = 4 * entity->armJointID;
            int distX                 = entity->position.x - player1->position.x;
            int distY                 = (entity->position.y - player1->position.y) >> 16;
            if (entity->attackingArmID)
                entity->jointTargetAngles[slot] = (uint8)(0x40 - RSDK.ATan2((distX + 0x280000) >> 16, distY));
            else
                entity->jointTargetAngles[slot] = (uint8)(RSDK.ATan2((distX - 0x280000) >> 16, distY) - 0x40);
            entity->jointDirection[slot] = 1;
        }

        entity->stateArm[entity->attackingArmID]       = EggJanken_StateArm_Unknown5;
        entity->armRadiusSpeed[entity->attackingArmID] = 0x38000;

        if (entity->attackingArmID) {
            int distX = abs((entity->position.x - player1->position.x + 0x280000) >> 16);
            int distY = abs((entity->position.y - player1->position.y) >> 16);
            int inc   = 0x180 * (distX + distY - 120);
            entity->armRadiusSpeed[entity->attackingArmID] += inc;
        }
        else {
            int distX = abs((entity->position.x - player1->position.x - 0x280000) >> 16);
            int distY = abs((entity->position.y - player1->position.y) >> 16);
            int inc   = 0x180 * (distY + distX - 120);
            entity->armRadiusSpeed[entity->attackingArmID] += inc;
        }
        entity->state1 = EggJanken_State1_Unknown16;
    }
    else if (entity->attackingArmID) {
        entity->fullRotation -= 0x200;
    }
    else {
        entity->fullRotation += 0x200;
    }
}

void EggJanken_State1_Unknown16(void)
{
    RSDK_THIS(EggJanken);

    if (entity->stateArm[entity->attackingArmID] == EggJanken_StateArm_Unknown7) {
        entity->state1 = EggJanken_State1_Unknown17;
        entity->timer  = 0;
    }
    else if (entity->attackingArmID) {
        entity->fullRotation += 0x200;
        if (entity->fullRotation > 0)
            entity->fullRotation = 0;
    }
    else {
        entity->fullRotation -= 0x200;
        if (entity->fullRotation < 0)
            entity->fullRotation = 0;
    }
}

void EggJanken_State1_Unknown17(void)
{
    RSDK_THIS(EggJanken);

    if (++entity->timer == 30) {
        entity->timer = 0;
        EggJanken_Unknown18();
    }
}

void EggJanken_State1_JankenLose(void)
{
    RSDK_THIS(EggJanken);

    if (++entity->timer == 60) {
        entity->eyeFrames[0] = 3;
        entity->eyeFrames[1] = 4;
    }
    if (entity->timer == 120) {
        entity->state1   = EggJanken_State1_Unknown19;
        entity->isMoving = false;
        entity->timer    = 0;
    }
}

void EggJanken_State1_Unknown19(void)
{
    RSDK_THIS(EggJanken);

    entity->fullRotation += 0x800;
    if ((entity->fullRotation & 0x1FFFF) == 0x10000) {
        for (entity->armID = 0; entity->armID < EggJanken_ArmCount; ++entity->armID) {
            entity->armRadiusSpeed[entity->armID] = 0x40000;
            for (int s = 0; s < EggJanken_SegmentCount; ++s) entity->jointAngles[EggJanken_SegmentCount * entity->armID + s] = 0xC0;
            entity->stateArm[entity->armID]                                 = EggJanken_StateArm_Unknown3;
        }

        entity->state1 = EggJanken_State1_Unknown20;
    }
}

void EggJanken_State1_Unknown21(void)
{
    RSDK_THIS(EggJanken);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (abs(entity->position.x - player1->position.x) >= 0xC0000) {
        if (entity->position.x >= player1->position.x)
            entity->startPos.x -= 0x10000;
        else
            entity->startPos.x += 0x10000;
    }
    else {
        for (entity->armID = 0; entity->armID < EggJanken_ArmCount; ++entity->armID) {
            for (entity->armJointID = 0; entity->armJointID < EggJanken_SegmentCount; ++entity->armJointID) {
                entity->jointAngleVels[entity->armID]                                                  = 8;
                entity->jointDelays[EggJanken_SegmentCount * entity->armID + entity->armJointID]       = 4 * entity->armJointID;
                entity->jointTargetAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] = 160;
                entity->jointDirection[EggJanken_SegmentCount * entity->armID + entity->armJointID]    = 1;
            }
            entity->stateArm[entity->armID] = EggJanken_StateArm_Unknown6;
        }

        RSDK.PlaySfx(EggJanken->sfxDrop, false, 255);
        entity->state1 = EggJanken_State1_Unknown22;
    }
}

void EggJanken_State1_Unknown20(void)
{
    RSDK_THIS(EggJanken);

    if (entity->stateArm[0] == EggJanken_StateArm_Unknown7)
        entity->state1 = EggJanken_State1_Unknown21;
}

void EggJanken_State1_Unknown22(void)
{
    RSDK_THIS(EggJanken);

    entity->startPos.y += entity->velocity.y;
    entity->position.y = entity->startPos.y;
    entity->velocity.y += 0x7000;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x200000, true)) {
        for (entity->armID = 0; entity->armID < EggJanken_ArmCount; ++entity->armID) {
            for (entity->armJointID = 0; entity->armJointID < EggJanken_SegmentCount; ++entity->armJointID) {
                entity->jointAngleVels[entity->armID]                                                  = 8;
                entity->jointDelays[EggJanken_SegmentCount * entity->armID + entity->armJointID]       = 4 * entity->armJointID;
                entity->jointTargetAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] = 200;
                entity->jointDirection[EggJanken_SegmentCount * entity->armID + entity->armJointID]    = 0;
            }
            entity->stateArm[entity->armID] = EggJanken_StateArm_Unknown6;
        }

        RSDK.PlaySfx(EggJanken->sfxImpact4, false, 255);
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->shakePos.y   = 4;
        entity->velocity.y   = 0;
        entity->state1       = EggJanken_State1_Unknown23;
    }
}

void EggJanken_State1_Unknown23(void)
{
    RSDK_THIS(EggJanken);

    if (entity->stateArm[0] == EggJanken_StateArm_Unknown7) {
        RSDK.PlaySfx(EggJanken->sfxImpact3, false, 255);
        entity->state1 = EggJanken_State1_Unknown24;
    }
}

void EggJanken_State1_Unknown24(void)
{
    RSDK_THIS(EggJanken);

    if (++entity->timer == 60) {
        entity->timer       = 0;
        entity->stateArm[0] = EggJanken_StateArm_Unknown8;
        entity->stateArm[1] = EggJanken_StateArm_Unknown8;
        entity->state1      = EggJanken_State1_Unknown25;
    }
}

void EggJanken_State1_Unknown25(void)
{
    RSDK_THIS(EggJanken);

    if (entity->stateArm[0] == EggJanken_StateArm_Unknown7)
        entity->state1 = EggJanken_State1_Unknown26;
}

void EggJanken_State1_Unknown26(void)
{
    RSDK_THIS(EggJanken);

    entity->startPos.y = entity->startPos.y - 0x20000;
    entity->position.y = entity->startPos.y - 0x20000;
    if (entity->position.y < entity->yCap) {
        entity->startPos.y = entity->yCap;
        entity->position.y = entity->yCap;
        entity->state1     = EggJanken_State1_Unknown27;
    }
}

void EggJanken_State1_Unknown27(void)
{
    RSDK_THIS(EggJanken);

    entity->fullRotation += 1024;
    if ((entity->fullRotation & 0x1FFFF) == 0) {
        entity->angle    = 0;
        entity->isMoving = true;
        EggJanken_Unknown18();
    }
}

void EggJanken_State2_Unknown1(void)
{
    RSDK_THIS(EggJanken);

    entity->timer2            = 0;
    entity->eyeFrames[0]      = RSDK.Rand(0, 3);
    entity->eyeFrames[1]      = RSDK.Rand(0, 3);
    entity->state2            = EggJanken_State2_Unknown2;
    entity->animator3.frameID = 0;
    RSDK.PlaySfx(EggJanken->sfxClick, false, 255);
}

void EggJanken_State2_Unknown2(void)
{
    RSDK_THIS(EggJanken);

    ++entity->timer2;

    switch (entity->health) {
        default: break;
        case 1:
            if (!(entity->timer2 & 0x3F)) {
                entity->eyeFrames[0] = RSDK.Rand(0, 3);
                entity->eyeFrames[1] = RSDK.Rand(0, 3);
                RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
            }
            break;
        case 2:
            if (!(entity->timer2 & 0x3F)) {
                entity->eyeFrames[0] = (entity->eyeFrames[0] + 2) % 3;
                RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
            }

            if (!(entity->timer2 & 0x7F)) {
                entity->eyeFrames[1] = (entity->eyeFrames[1] + 1) % 3u;
                RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
            }
            break;
        case 3:
            if (entity->health == 3) {
                if (!(entity->timer2 & 0x3F)) {
                    entity->eyeFrames[0] = (entity->eyeFrames[0] + 1) % 3;
                    RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
                }

                if (!(entity->timer2 & 0x7F)) {
                    entity->eyeFrames[1] = (entity->eyeFrames[1] + 1) % 3u;
                    RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
                }
            }
            break;
    }
}

void EggJanken_State2_Unknown3(void) {}

void EggJanken_Unknown40(void)
{
    RSDK_THIS(EggJanken);

    int slot = entity->armJointID + EggJanken_SegmentCount * entity->armID;

    entity->jointAngleVels[entity->armID] = 4;
    entity->jointDelays[slot]             = 4 * entity->armJointID;
    entity->jointDirection[slot] ^= 1;
    entity->jointTargetAngles[slot] = entity->jointAngles[slot];
    if (entity->jointDirection[slot]) {
        entity->jointTargetAngles[slot] = 136 - (8 * entity->armJointID);
        if (entity->armJointID == EggJanken_JointCount)
            entity->jointTargetAngles[slot] -= 8;
    }
    else {
        entity->jointTargetAngles[slot] = 224 - (8 * entity->armJointID);
        if (entity->armJointID == EggJanken_JointCount)
            entity->jointTargetAngles[slot] -= 8;
    }
    if (entity->armJointID == EggJanken_JointCount) {
        entity->jointFlags[entity->armID] = 0;
    }
}

void EggJanken_StateArm_Unknown1(void) {}

void EggJanken_StateArm_Unknown7(void) {}

void EggJanken_StateArm_Unknown8(void)
{
    RSDK_THIS(EggJanken);

    if (entity->armRadius[entity->armID])
        entity->armRadius[entity->armID] -= 0x8000;
    else
        entity->stateArm[entity->armID] = EggJanken_StateArm_Unknown7;
}

void EggJanken_StateArm_Unknown3(void)
{
    RSDK_THIS(EggJanken);

    if (entity->armRadius[entity->armID] != 0x800000) {
        entity->armRadius[entity->armID] += 0x10000;
    }
    else {
        entity->armRadiusSpeed[entity->armID] = 0x8000;
        entity->stateArm[entity->armID]       = EggJanken_StateArm_Unknown7;
    }
}

void EggJanken_StateArm_Unknown2(void)
{
    RSDK_THIS(EggJanken);

    entity->armRadius[entity->armID] += entity->armRadiusSpeed[entity->armID];
    entity->armRadiusSpeed[entity->armID] -= 0x800;

    if (entity->armRadius[entity->armID] <= 0) {
        if (entity->state1 == EggJanken_State1_Unknown9 && !entity->armID) {
            RSDK.PlaySfx(EggJanken->sfxHit, false, 255);
            entity->invincibilityTimer = 60;
            entity->eyeFrames[0]       = 8;
            entity->eyeFrames[1]       = 9;
            --entity->health;
        }
        entity->armRadius[entity->armID] = 0;
        entity->stateArm[entity->armID]  = EggJanken_StateArm_Unknown7;
    }
}

void EggJanken_StateArm_Unknown4(void)
{
    RSDK_THIS(EggJanken);

    if (entity->jointFlags[entity->armID] == ((1 << EggJanken_SegmentCount) - 1))
        EggJanken_Unknown40();

    int slot = entity->armJointID + EggJanken_SegmentCount * entity->armID;

    if (entity->jointDelays[slot]) {
        entity->jointDelays[slot]--;
    }
    else {
        if (entity->jointDirection[slot]) {
            if (entity->jointAngles[slot] > entity->jointTargetAngles[slot])
                entity->jointAngles[slot] -= entity->jointAngleVels[entity->armID];
            else
                entity->jointFlags[entity->armID] |= (1 << entity->armJointID);
        }
        else if (entity->jointAngles[slot] < entity->jointTargetAngles[slot])
            entity->jointAngles[slot] += entity->jointAngleVels[entity->armID];
        else
            entity->jointFlags[entity->armID] |= (1 << entity->armJointID);
    }
}

void EggJanken_StateArm_Unknown5(void)
{
    RSDK_THIS(EggJanken);

    int slot = entity->armJointID + EggJanken_SegmentCount * entity->armID;

    if (entity->jointDelays[slot]) {
        entity->jointDelays[slot]--;
    }
    else {
        if (entity->jointDirection[slot]) {
            if (entity->jointAngles[slot] > entity->jointTargetAngles[slot]) {
                entity->jointAngles[slot] -= entity->jointAngleVels[entity->armID];
            }
            else {
                entity->jointFlags[entity->armID] |= (1 << entity->armJointID);
                entity->jointAngles[slot] = entity->jointTargetAngles[slot];
            }
        }
        else if (entity->jointAngles[slot] < entity->jointTargetAngles[slot]) {
            entity->jointAngles[slot] += entity->jointAngleVels[entity->armID];
        }
        else {
            entity->jointFlags[entity->armID] |= (1 << entity->armJointID);
            entity->jointAngles[slot] = entity->jointTargetAngles[slot];
        }
    }

    entity->armRadius[entity->armID] += entity->armRadiusSpeed[entity->armID];
    entity->armRadiusSpeed[entity->armID] -= 0x800;

    if (entity->armRadius[entity->armID] <= 0) {
        entity->jointFlags[entity->armID] = 0;
        entity->armRadius[entity->armID]  = 0;
        entity->stateArm[entity->armID]   = EggJanken_StateArm_Unknown7;
    }
}

void EggJanken_StateArm_Unknown6(void)
{
    RSDK_THIS(EggJanken);

    if (entity->jointFlags[entity->armID] == ((1 << EggJanken_SegmentCount) - 1)) {
        entity->jointFlags[entity->armID] = 0;
        entity->stateArm[entity->armID]   = EggJanken_StateArm_Unknown7;
    }

    int slot = entity->armJointID + EggJanken_SegmentCount * entity->armID;

    if (entity->jointDelays[slot]) {
        entity->jointDelays[slot]--;
    }
    else {
        if (entity->jointDirection[slot]) {
            if (entity->jointAngles[slot] > entity->jointTargetAngles[slot]) {
                entity->jointAngles[slot] -= entity->jointAngleVels[entity->armID];
            }
            else {
                entity->jointFlags[entity->armID] |= (1 << entity->armJointID);
                entity->jointAngles[slot] = entity->jointTargetAngles[slot];
            }
        }
        else if (entity->jointAngles[slot] < entity->jointTargetAngles[slot]) {
            entity->jointAngles[slot] += entity->jointAngleVels[entity->armID];
        }
        else {
            entity->jointFlags[entity->armID] |= (1 << entity->armJointID);
            entity->jointAngles[slot] = entity->jointTargetAngles[slot];
        }
    }
}

void EggJanken_StateDraw_Unknown1(void)
{
    RSDK_THIS(EggJanken);

    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator4, NULL, false);
    RSDK.DrawSprite(&entity->animator5, NULL, false);
}

void EggJanken_StateDraw_Unknown2(void)
{
    RSDK_THIS(EggJanken);
    Vector2 drawPos;

    entity->armID = 0;
    drawPos.x     = entity->radius * RSDK.Sin256(0xC0 - (entity->rotation >> 1));
    drawPos.y     = entity->radius * RSDK.Cos256(0xC0 - (entity->rotation >> 1));
    drawPos.x += entity->position.x;
    drawPos.y += entity->position.y;
    EggJanken->animator.frameID = 0;
    RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);

    int radius                  = (entity->armRadius[entity->armID] >> 16);
    EggJanken->animator.frameID = 1;
    entity->armJointID          = 0;
    for (entity->armJointID = 0; entity->armJointID < EggJanken_JointCount; ++entity->armJointID) {
        drawPos.x +=
            32 * radius * RSDK.Sin256(entity->jointAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] + (entity->rotation >> 1));
        drawPos.y -=
            32 * radius * RSDK.Cos256(entity->jointAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] + (entity->rotation >> 1));
        RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);
    }

    EggJanken->animator.frameID = 2;
    drawPos.x += (radius * RSDK.Sin256(entity->jointAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] + (entity->rotation >> 1)))
                 << 6;
    drawPos.y -= (radius * RSDK.Cos256(entity->jointAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] + (entity->rotation >> 1)))
                 << 6;
    RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);

    entity->armPos[0] = drawPos;
    entity->armID     = 1;
    drawPos.x         = entity->radius * RSDK.Sin256(0x40 - (entity->rotation >> 1));
    drawPos.y         = entity->radius * RSDK.Cos256(0x40 - (entity->rotation >> 1));
    drawPos.x += entity->position.x;
    drawPos.y += entity->position.y;
    EggJanken->animator.frameID = 0;
    RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);

    radius                      = (entity->armRadius[entity->armID] >> 16);
    EggJanken->animator.frameID = 1;
    entity->armJointID          = 0;
    for (entity->armJointID = 0; entity->armJointID < EggJanken_JointCount; ++entity->armJointID) {
        drawPos.x -=
            32 * radius * RSDK.Sin256(entity->jointAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] - (entity->rotation >> 1));
        drawPos.y -=
            32 * radius * RSDK.Cos256(entity->jointAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] - (entity->rotation >> 1));
        RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);
    }

    EggJanken->animator.frameID = 2;
    drawPos.x -= (radius * RSDK.Sin256(entity->jointAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] - (entity->rotation >> 1)))
                 << 6;
    drawPos.y -= (radius * RSDK.Cos256(entity->jointAngles[EggJanken_SegmentCount * entity->armID + entity->armJointID] - (entity->rotation >> 1)))
                 << 6;
    RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);

    entity->armPos[1] = drawPos;
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    if (Zone->timer & 1) {
        entity->inkEffect                                                    = INK_ADD;
        entity->alpha                                                        = 128;
        entity->animator1.frameID                                            = entity->eyeFrames[0];
        RSDK.GetFrame(EggJanken->aniFrames, 0, entity->eyeFrames[0])->pivotX = -24;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->animator1.frameID                                            = entity->eyeFrames[1];
        RSDK.GetFrame(EggJanken->aniFrames, 0, entity->eyeFrames[1])->pivotX = 0;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->inkEffect = INK_NONE;
        entity->alpha     = 512;
    }
    RSDK.DrawSprite(&entity->animator4, NULL, false);
    RSDK.DrawSprite(&entity->animator5, NULL, false);
}

void EggJanken_StateDraw_Unknown3(void)
{
    RSDK_THIS(EggJanken);

    if (Zone->timer & 1) {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator4, NULL, false);
        RSDK.DrawSprite(&entity->animator5, NULL, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void EggJanken_EditorDraw(void)
{
    RSDK_THIS(EggJanken);
    EggJanken_StateDraw_Unknown1();

    if (showGizmos())
        DrawHelpers_DrawArenaBounds(0x00C0F0, 1 | 0 | 4 | 8, -212, -SCREEN_YSIZE, 212, 208);
}

void EggJanken_EditorLoad(void)
{
    EggJanken->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggJanken.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 3, &EggJanken->animator, true, 0);
}
#endif

void EggJanken_Serialize(void) {}
