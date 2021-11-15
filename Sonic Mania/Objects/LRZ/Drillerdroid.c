#include "SonicMania.h"

ObjectDrillerdroid *Drillerdroid;

void Drillerdroid_Update(void)
{
    RSDK_THIS(Drillerdroid);
    StateMachine_Run(entity->state);
}

void Drillerdroid_LateUpdate(void) {}

void Drillerdroid_StaticUpdate(void)
{
    if (Drillerdroid->shouldPlayDrillSfx) {
        if (!Drillerdroid->playingDrillSfx) {
            RSDK.PlaySfx(Drillerdroid->sfxDrill, 43643, 255);
            Drillerdroid->playingDrillSfx = true;
        }
        Drillerdroid->shouldPlayDrillSfx = 0;
    }
    else if (Drillerdroid->playingDrillSfx) {
        RSDK.StopSFX(Drillerdroid->sfxDrill);
        Drillerdroid->playingDrillSfx = false;
    }
}

void Drillerdroid_Draw(void)
{
    RSDK_THIS(Drillerdroid);
    StateMachine_Run(entity->stateDraw);
}

void Drillerdroid_Create(void *data)
{
    RSDK_THIS(Drillerdroid);

    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->visible = true;
            if (data)
                entity->type = voidToInt(data);
            if (entity->type) {
                if (entity->type == DRILLERDROID_TARGET || entity->type == DRILLERDROID_UNKNOWN) {
                    entity->active        = ACTIVE_NORMAL;
                    entity->drawFX        = FX_FLIP;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 2, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 3, &entity->animator2, true, 0);
                    entity->drawOrder = Zone->drawOrderHigh;
                    entity->state     = Drillerdroid_State1_Unknown;
                    entity->stateDraw = Drillerdroid_StateDraw1_Unknown;
                }
            }
            else {
                entity->active        = ACTIVE_BOUNDS;
                entity->updateRange.x = 0x800000;
                entity->updateRange.y = 0x800000;
                RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &entity->animator2, true, 0);
                entity->drawOrder  = Zone->drawOrderHigh;
                Drillerdroid->boss = (Entity *)entity;
                entity->state      = Drillerdroid_State_SetupArena;
                entity->stateDraw  = Drillerdroid_StateDraw_Unknown1;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void Drillerdroid_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Drillerdroid->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Drillerdroid.bin", SCOPE_STAGE);
    Drillerdroid->ticFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);

    Drillerdroid->hitbox1.left   = -25;
    Drillerdroid->hitbox1.top    = -16;
    Drillerdroid->hitbox1.right  = 25;
    Drillerdroid->hitbox1.bottom = 16;

    Drillerdroid->hitbox2.left   = -58;
    Drillerdroid->hitbox2.top    = -74;
    Drillerdroid->hitbox2.right  = -25;
    Drillerdroid->hitbox2.bottom = 16;

    Drillerdroid->hitbox3.left   = 25;
    Drillerdroid->hitbox3.top    = -74;
    Drillerdroid->hitbox3.right  = 58;
    Drillerdroid->hitbox3.bottom = 16;

    Drillerdroid->hitbox4.left   = -58;
    Drillerdroid->hitbox4.top    = 16;
    Drillerdroid->hitbox4.right  = -25;
    Drillerdroid->hitbox4.bottom = 53;

    Drillerdroid->hitbox5.left   = 25;
    Drillerdroid->hitbox5.top    = 16;
    Drillerdroid->hitbox5.right  = 58;
    Drillerdroid->hitbox5.bottom = 53;

    Drillerdroid->field_2C[0]        = 0;
    Drillerdroid->field_2C[1]        = 0;
    Drillerdroid->field_34[0]        = 0x100000;
    Drillerdroid->field_34[1]        = 0x100000;
    Drillerdroid->field_3C[0]        = 4;
    Drillerdroid->field_3C[1]        = 0;
    Drillerdroid->field_44[0]        = 4;
    Drillerdroid->field_44[1]        = 0;
    Drillerdroid->field_4C[0]        = 0;
    Drillerdroid->field_4C[1]        = 0;
    Drillerdroid->field_54[0]        = 0;
    Drillerdroid->field_54[1]        = 0;
    Drillerdroid->field_5C[0]        = 1;
    Drillerdroid->field_5C[1]        = 1;
    Drillerdroid->field_5C[2]        = 1;
    Drillerdroid->field_5C[3]        = 1;
    Drillerdroid->field_5C[4]        = 1;
    Drillerdroid->active             = ACTIVE_ALWAYS;
    Drillerdroid->shouldPlayDrillSfx = 0;
    Drillerdroid->playingDrillSfx    = false;
    Drillerdroid->armorHealth           = 3;
    Drillerdroid->field_6D           = 0;
    Drillerdroid->sfxHit             = RSDK.GetSFX("Stage/BossHit.wav");
    Drillerdroid->sfxExplosion       = RSDK.GetSFX("Stage/Explosion2.wav");
    Drillerdroid->sfxDrill           = RSDK.GetSFX("LRZ/Drill.wav");
    Drillerdroid->sfxImpact          = RSDK.GetSFX("Stage/Impact4.wav");
    Drillerdroid->sfxJump            = RSDK.GetSFX("LRZ/DrillJump.wav");
    Drillerdroid->sfxTargeting       = RSDK.GetSFX("Stage/Targeting1.wav");
    Drillerdroid->sfxSizzle          = RSDK.GetSFX("LRZ/Sizzle.wav");
    Drillerdroid->sfxDrop            = RSDK.GetSFX("Stage/Drop.wav");
    Drillerdroid->sfxFail            = RSDK.GetSFX("Stage/Fail.wav");
    Drillerdroid->sfxClang           = RSDK.GetSFX("Stage/Clang2.wav");
}

void Drillerdroid_Hit(void)
{
    RSDK_THIS(Drillerdroid);

    if (--entity->health > 0) {
        RSDK.PlaySfx(Drillerdroid->sfxHit, false, 255);
        entity->invincibilityTimer = 45;
    }
    else {
        RSDK_sceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        entity->invincibilityTimer = 60;
        entity->state              = Drillerdroid_State_Destroyed;
    }
}

void Drillerdroid_CheckPlayerCollisions(void)
{
    RSDK_THIS(Drillerdroid);

    if (entity->invincibilityTimer) {
        entity->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            if (!entity->invincibilityTimer) {
                int playerID = RSDK.GetEntityID(player);
                if (Drillerdroid->playerTimers[playerID]) {
                    Drillerdroid->playerTimers[playerID]--;
                }
                else if (Player_CheckBadnikTouch(player, entity, &Drillerdroid->hitbox1)) {
                    if (player->position.y < entity->position.y) {
                        Player_CheckHit(player, entity);
                    }
                    else if (entity->alpha > 0x80 && player->shield != SHIELD_FIRE && !player->blinkTimer && !player->invincibleTimer) {
                        Player_CheckHit(player, entity);
                    }
                    if (Player_CheckBossHit(player, entity)) {
                        if (Drillerdroid->armorHealth <= 1) {
                            Drillerdroid_Hit();
                        }
                        else {
                            RSDK.PlaySfx(Drillerdroid->sfxClang, false, 255);
                            Drillerdroid->playerTimers[playerID] = 15;
                        }
                    }
                }
                else {
                    if (Player_CheckBadnikTouch(player, entity, &Drillerdroid->hitbox2)
                        || Player_CheckBadnikTouch(player, entity, &Drillerdroid->hitbox3)) {
                        if (entity->alpha > 0x80 && player->shield != SHIELD_FIRE && !player->blinkTimer && !player->invincibleTimer) {
                            Player_CheckHit(player, entity);
                        }
                        else if (Player_CheckBossHit(player, entity)) {
                            if (!Drillerdroid->armorHealth) {
                                Drillerdroid_Hit();
                            }
                            else {
                                RSDK.PlaySfx(Drillerdroid->sfxClang, false, 255);
                                Drillerdroid->playerTimers[playerID] = 15;
                            }
                        }
                    }
                    else if (Player_CheckCollisionTouch(player, entity, &Drillerdroid->hitbox4)
                             || Player_CheckCollisionTouch(player, entity, &Drillerdroid->hitbox5)) {
                        Player_CheckHit(player, entity);
                    }
                }
            }
        }
    }
}

void Drillerdroid_Explode(void)
{
    RSDK_THIS(Drillerdroid);

    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(Drillerdroid->sfxExplosion, false, 255);
        if (!(Zone->timer & 3)) {
            int x = entity->position.y + (RSDK.Rand(-19, 20) << 16);
            int y = entity->position.x + (RSDK.Rand(-24, 25) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void Drillerdroid_SpawnDebris(int offset)
{
    RSDK_THIS(Drillerdroid);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + entity->position.x, entity->position.y + 0x400000);
    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x    = RSDK.Rand(0, 6) << 15;
    debris->velocity.y    = RSDK.Rand(-12, -8) << 15;
    debris->gravity       = 0x3800;
    debris->drawOrder     = Zone->drawOrderLow;
    debris->updateRange.x = 0x400000;
    debris->updateRange.y = 0x400000;

    debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + entity->position.x, entity->position.y + 0x400000);
    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x    = RSDK.Rand(-6, 0) << 15;
    debris->velocity.y    = RSDK.Rand(-12, -8) << 15;
    debris->gravity       = 0x3800;
    debris->drawOrder     = Zone->drawOrderLow;
    debris->updateRange.x = 0x400000;
    debris->updateRange.y = 0x400000;
}

void Drillerdroid_State_SetupArena(void)
{
    RSDK_THIS(Drillerdroid);

    if (++entity->timer >= 2) {
        entity->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + 324;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 96;
        Zone->screenBoundsT1[0]     = Zone->screenBoundsB1[0] - 240;
        entity->startY            = entity->position.y;
        entity->active              = ACTIVE_NORMAL;
        entity->position.y          = (RSDK_screens->position.y - 192) << 16;
        entity->visible             = true;
        entity->state               = Drillerdroid_State_StartBoss;
    }
}

void Drillerdroid_State_StartBoss(void)
{
    RSDK_THIS(Drillerdroid);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = RSDK_screens->position.x;
    if (player1->position.x > entity->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - 328;
        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        entity->health                                                                                            = 6;
        CREATE_ENTITY(Drillerdroid, intToVoid(DRILLERDROID_TARGET), entity->position.x, entity->startY)->target = (Entity *)player1;
        entity->position.x                                                                                        = 0;
        RSDK.PlaySfx(Drillerdroid->sfxTargeting, false, 255);
        entity->state = Drillerdroid_State_Unknown17;
    }
}

void Drillerdroid_State_Unknown2(void)
{
    RSDK_THIS(Drillerdroid);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    Drillerdroid_CheckPlayerCollisions();

    if (entity->position.y >= ((Zone->screenBoundsB1[0] - 112) << 16)) {
        entity->position.y = ((Zone->screenBoundsB1[0] - 112) << 16);
        RSDK.PlaySfx(Drillerdroid->sfxImpact, false, 255);
        Camera_ShakeScreen(0, 0, 4);
        entity->velocity.y >>= 1;
        entity->startY = entity->position.y + 0x100000;

        if (!Drillerdroid->field_6E) {
            for (int i = 0; i < 4; ++i) {
                Drillerdroid_SpawnDebris(-0x300000);
                Drillerdroid_SpawnDebris(0x300000);
                Drillerdroid_SpawnDebris(-0x190000);
                Drillerdroid_SpawnDebris(0x190000);
            }

            Drillerdroid->field_6D = 2;
        }
        entity->timer = 7;
        entity->angle = 0;
        entity->state = Drillerdroid_State_Unknown3;
    }
}

void Drillerdroid_State_Unknown3(void)
{
    RSDK_THIS(Drillerdroid);

    entity->position.y += entity->velocity.y;
    Drillerdroid->field_34[0] = entity->startY - entity->position.y;
    Drillerdroid->field_34[1] = entity->startY - entity->position.y;

    entity->velocity.y -= 0xE000;

    Drillerdroid_CheckPlayerCollisions();
    if (entity->velocity.y < 0) {
        if (entity->position.y < entity->startY)
            entity->state = Drillerdroid_State_Unknown4;
    }
}

void Drillerdroid_State_Unknown4(void)
{
    RSDK_THIS(Drillerdroid);

    entity->position.y += entity->velocity.y;
    Drillerdroid->field_34[0] = entity->startY - entity->position.y;
    Drillerdroid->field_34[1] = entity->startY - entity->position.y;

    entity->velocity.y += 0x14000;

    Drillerdroid_CheckPlayerCollisions();
    if (entity->velocity.y > 0) {
        if (entity->position.y > entity->startY) {
            entity->position.y        = entity->startY;
            Drillerdroid->field_34[0] = 0;
            Drillerdroid->field_34[1] = 0;
            entity->state             = Drillerdroid_State_Unknown5;
        }
    }
}

void Drillerdroid_State_Unknown5(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_CheckPlayerCollisions();
    if (--entity->timer <= 0) {
        if (Drillerdroid->field_6D) {
            entity->timer      = 240;
            entity->velocity.y = 0x27000;
            RSDK.PlaySfx(Drillerdroid->sfxJump, false, 255);
            entity->state = Drillerdroid_State_Unknown6;
        }
        else if (Drillerdroid->field_6E) {
            entity->timer          = 180;
            Drillerdroid->field_70 = 2 * (3 * RSDK.Rand(-2, 3));
            entity->state          = Drillerdroid_State_Unknown8;
        }
        else {
            entity->timer = 60;
            entity->state = Drillerdroid_State_Unknown10;
        }
    }
}

void Drillerdroid_State_Unknown6(void)
{
    RSDK_THIS(Drillerdroid);

    entity->position.y += entity->velocity.y;
    Drillerdroid->field_34[0] = entity->startY - entity->position.y;
    Drillerdroid->field_34[1] = entity->startY - entity->position.y;

    entity->velocity.y -= 0x8000;
    Drillerdroid_CheckPlayerCollisions();

    if (entity->velocity.y < 0) {
        if (Drillerdroid->field_34[0] >= 0x40000) {
            entity->timer             = entity->velocity.y;
            Drillerdroid->field_34[0] = 0x40000;
            Drillerdroid->field_34[1] = 0x40000;
            entity->state             = Drillerdroid_State_Unknown7;
        }
    }
}

void Drillerdroid_State_Unknown7(void)
{
    RSDK_THIS(Drillerdroid);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    Drillerdroid_CheckPlayerCollisions();

    if (entity->velocity.y > 0) {
        if (entity->position.y >= (Zone->screenBoundsB1[0] - 96) << 16) {
            entity->position.y = (Zone->screenBoundsB1[0] - 96) << 16;
            RSDK.PlaySfx(Drillerdroid->sfxImpact, false, 255);
            Camera_ShakeScreen(0, 0, 4);
            entity->velocity.y >>= 1;
            entity->startY = entity->position.y;
            entity->angle    = 0;
            if (--Drillerdroid->field_6D) {
                RSDK.PlaySfx(Drillerdroid->sfxJump, false, 255);
                entity->state = Drillerdroid_State_Unknown6;
            }
            else {
                RSDK.PlaySfx(Drillerdroid->sfxDrop, false, 255);
                EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

                EntityBuckwildBall *ball = CREATE_ENTITY(BuckwildBall, NULL, entity->position.x, RSDK_screens->position.y << 16);
                ball->startPos.x         = 0;

                int value = 0;
                if (Drillerdroid->field_61) {
                    if (Drillerdroid->field_61 == 4 || player1->position.x < entity->position.x) {
                        ball->position.x -= 0x800000;
                        ball->direction = FLIP_NONE;
                        value           = Drillerdroid->field_61 - 1;
                    }
                    else {
                        ball->position.x += 0x800000;
                        ball->direction = FLIP_X;
                        value           = Drillerdroid->field_61 + 1;
                    }
                }
                else {
                    ball->position.x += 0x800000;
                    ball->direction = FLIP_X;
                    value           = Drillerdroid->field_61 + 1;
                }
                ball->field_B8  = value;
                ball->visible   = true;
                ball->active    = ACTIVE_NORMAL;
                ball->drawOrder = Zone->drawOrderLow;
                ball->respawn   = false;
                ball->timerSfx  = 0;
                RSDK.SetSpriteAnimation(BuckwildBall->aniFrames, 0, &ball->animator, true, 0);
                ball->state   = BuckwildBall_State_Unknown3;
                entity->state = Drillerdroid_State_Unknown3;
            }
        }
    }
}

void Drillerdroid_State_Unknown8(void)
{
    RSDK_THIS(Drillerdroid);

    ++Drillerdroid->shouldPlayDrillSfx;
    RSDK.ProcessAnimation(&entity->animator1);

    for (int i = 0; i < 2; ++i) {
        if (Drillerdroid->field_3C[i]) {
            Drillerdroid->field_3C[i]--;
        }
        else if (Drillerdroid->field_4C[i]) {
            Drillerdroid->field_2C[i] -= 0x20000;
            if (!Drillerdroid->field_2C[i]) {
                Drillerdroid->field_4C[i] = 0;
                Drillerdroid->field_3C[i] = 10;
            }
        }
        else {
            Drillerdroid->field_2C[i] += 0x40000;
            if (Drillerdroid->field_2C[i] == 0x100000)
                Drillerdroid->field_4C[i] = 1;
        }

        if (Drillerdroid->field_44[i]) {
            Drillerdroid->field_44[i]--;
        }
        else if (Drillerdroid->field_54[i]) {
            Drillerdroid->field_34[i] -= 0x40000;
            if (!Drillerdroid->field_34[i]) {
                Drillerdroid->field_54[i] = 0;
                Drillerdroid->field_44[i] = 5;
            }
        }
        else {
            Drillerdroid->field_34[i] += 0x80000;
            if (Drillerdroid->field_34[i] == 0x100000)
                Drillerdroid->field_54[i] = 1;
        }
    }

    if (Drillerdroid->field_6E) {
        if (entity->timer == 120) {
            EntityLavaGeyser *geyser = CREATE_ENTITY(LavaGeyser, NULL, entity->position.x, entity->position.y + 0x580000);
            geyser->duration         = 60;
            geyser->active           = ACTIVE_NORMAL;
            geyser->force            = 56 << 12;
            geyser->drawOrder        = Zone->drawOrderHigh - 1;
            geyser->state            = LavaGeyser_HandleSetup;
        }
        else if (entity->timer < 60) {
            if (entity->alpha < 512)
                entity->alpha += 4;
        }
    }
    else if (Drillerdroid->field_34[0] == 0x80000) {
        Drillerdroid_SpawnDebris(-0x300000);
        Drillerdroid_SpawnDebris(0x300000);
    }
    else if (Drillerdroid->field_34[1] == 0x80000) {
        Drillerdroid_SpawnDebris(-0x190000);
        Drillerdroid_SpawnDebris(0x190000);
    }

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (!camera->shakePos.y)
        camera->shakePos.y = 4;

    if (!(Zone->timer & 7)) {
        if (Zone->timer & 0xF) {
            int offset = RSDK.Rand(0, RSDK_screens->width >> 5);
            EntityDebris *debris =
                CREATE_ENTITY(Debris, Debris_State_Fall, (32 * offset + RSDK_screens->position.x) << 16, (RSDK_screens->position.y + 24) << 16);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 7, &debris->animator, true, RSDK.Rand(0, 4));
            debris->gravity       = 0x1800;
            debris->drawOrder     = Zone->drawOrderLow;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;
        }
        else {
            int offset = RSDK.Rand(0, RSDK_screens->width / 48);
            EntityStalactite *stalactite =
                CREATE_ENTITY(Stalactite, Stalactite_State_Unknown4, (48 * offset + Drillerdroid->field_70 + RSDK_screens->position.x) << 16,
                              (RSDK_screens->position.y + 24) << 16);
            stalactite->updateRange.x = 0x400000;
            stalactite->updateRange.y = 0x400000;
        }
    }

    entity->position.y ^= 0x10000;
    Drillerdroid_CheckPlayerCollisions();
    if (--entity->timer <= 0)
        entity->state = Drillerdroid_State_Unknown9;
}

void Drillerdroid_State_Unknown9(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid->field_2C[0] -= 0x20000;
    if (Drillerdroid->field_2C[0] < 0)
        Drillerdroid->field_2C[0] = 0;

    Drillerdroid->field_2C[1] -= 0x20000;
    if (Drillerdroid->field_2C[1] < 0)
        Drillerdroid->field_2C[1] = 0;

    Drillerdroid->field_34[0] -= 0x20000;
    if (Drillerdroid->field_34[0] < 0)
        Drillerdroid->field_34[0] = 0;

    Drillerdroid->field_34[1] -= 0x20000;
    if (Drillerdroid->field_34[1] < 0)
        Drillerdroid->field_34[1] = 0;

    if (!Drillerdroid->field_2C[0] && !Drillerdroid->field_2C[1] && !Drillerdroid->field_34[0] && !Drillerdroid->field_34[1]) {
        entity->timer = 60;
        entity->state = Drillerdroid_State_Unknown10;
    }
}

void Drillerdroid_State_Unknown10(void)
{
    RSDK_THIS(Drillerdroid);

    if (--entity->timer <= 0) {
        if (Drillerdroid->field_6E == 1) {
            Drillerdroid->field_6E = 0;
            RSDK.PlaySfx(Drillerdroid->sfxSizzle, false, 255);

            int spawnX           = 0;
            EntityDebris *debris = NULL;
            switch (Drillerdroid->armorHealth) {
                default: break;
                case 0:
                    entity->state = Drillerdroid_State_Unknown11;
                    Drillerdroid_Hit();
                    break;
                case 1:
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
                    debris->velocity.x    = -0x10000;
                    debris->velocity.y    = -0x30000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
                    debris->direction     = FLIP_X;
                    debris->drawFX        = FX_FLIP;
                    debris->velocity.x    = 0x10000;
                    debris->velocity.y    = -0x30000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    int spawnX = entity->position.x - 0x300000;
                    debris     = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, entity->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x    = -0x30000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    spawnX += 0xD0000;
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, entity->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x    = 0x20000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    spawnX += 0x530000;
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, entity->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x    = 0x30000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX - 851968, entity->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x    = -0x20000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;
                    break;
                case 2:
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 4);
                    debris->velocity.x    = -0x10000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x300000;
                    debris->updateRange.y = 0x300000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 5);
                    debris->velocity.x    = 0x10000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x300000;
                    debris->updateRange.y = 0x300000;
                    break;
            }

            if (Drillerdroid->armorHealth > 0) {
                RSDK.PlaySfx(Drillerdroid->sfxHit, false, 255);
                entity->invincibilityTimer = 45;
                --Drillerdroid->armorHealth;
            }

            entity->state = Drillerdroid_State_Unknown11;
        }
        else {
            entity->state = Drillerdroid_State_Unknown19;
        }
    }

    Drillerdroid_CheckPlayerCollisions();
}

void Drillerdroid_State_Unknown19(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid->field_34[0] += 0x10000;
    Drillerdroid->field_34[0] += 0x10000;
    entity->position.y -= 0x10000;
    Drillerdroid_CheckPlayerCollisions();
    if (Drillerdroid->field_34[0] == 0x100000) {
        entity->timer = 15;
        entity->state = Drillerdroid_State_Unknown14;
    }
}

void Drillerdroid_State_Unknown14(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_CheckPlayerCollisions();
    if (--entity->timer <= 0) {
        entity->velocity.y = 0x78000;
        RSDK.PlaySfx(Drillerdroid->sfxJump, false, 255);
        entity->state = Drillerdroid_State_Unknown15;
    }
}

void Drillerdroid_State_Unknown15(void)
{
    RSDK_THIS(Drillerdroid);

    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x1C000;

    Drillerdroid->field_34[0] = entity->startY - entity->position.y;
    Drillerdroid->field_34[1] = entity->startY - entity->position.y;
    Drillerdroid_CheckPlayerCollisions();
    if (entity->velocity.y < 0) {
        if (Drillerdroid->field_34[0] >= 0x100000) {
            Drillerdroid->field_34[0] = 0x100000;
            Drillerdroid->field_34[1] = 0x100000;
            entity->state             = Drillerdroid_State_Unknown16;
        }
    }
}

void Drillerdroid_State_Unknown16(void)
{
    RSDK_THIS(Drillerdroid);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (entity->velocity.y >= 0) {
        Drillerdroid->field_2C[0] = 0;
        Drillerdroid->field_2C[1] = 0;
        Drillerdroid->field_34[0] = 0x100000;
        Drillerdroid->field_34[1] = 0x100000;
        Drillerdroid->field_3C[0] = 4;
        Drillerdroid->field_3C[1] = 0;
        Drillerdroid->field_44[0] = 4;
        Drillerdroid->field_44[0] = 0;
        Drillerdroid->field_4C[0] = 0;
        Drillerdroid->field_4C[1] = 0;
        Drillerdroid->field_54[0] = 0;
        Drillerdroid->field_54[1] = 0;
        entity->position.x        = 0;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        CREATE_ENTITY(Drillerdroid, intToVoid(DRILLERDROID_TARGET), player1->position.x, player1->position.y)->target = (Entity *)player1;
        RSDK.PlaySfx(Drillerdroid->sfxTargeting, false, 255);
        entity->state = Drillerdroid_State_Unknown17;
    }
}

void Drillerdroid_State_Unknown17(void)
{
    RSDK_THIS(Drillerdroid);

    if (entity->position.x) {
        entity->timer      = 240;
        entity->velocity.y = -0x40000;
        entity->state      = Drillerdroid_State_Unknown2;
        entity->position.x = (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x + 0x400000) & 0xFF800000;

        bool32 flag = entity->position.x < Zone->screenBoundsL1[0] << 16 || entity->position.x > Zone->screenBoundsR1[0] << 16;
        int id      = 0;
        if (!flag) {
            Drillerdroid->field_61 = (((entity->position.x >> 16) - Zone->screenBoundsL1[0] + 64) >> 7) - 1;
            id                     = Drillerdroid->field_5C[Drillerdroid->field_61];
            flag                   = id == 0xFF;
        }

        if (flag) {
            entity->position.x = 0;
            RSDK.PlaySfx(Drillerdroid->sfxFail, false, 255);
            RSDK.StopSFX(Drillerdroid->sfxTargeting);
            entity->timer = 60;
            entity->state = Drillerdroid_State_Unknown18;
        }
        else if (id == 1) {
            EntityLRZRockPile *rocks                       = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1 + Drillerdroid->field_61, LRZRockPile);
            rocks->active                                  = ACTIVE_NORMAL;
            rocks->timer                                   = 120;
            rocks->flag                                    = true;
            Drillerdroid->field_6E                         = 1;
            Drillerdroid->field_5C[Drillerdroid->field_61] = 0;
        }
    }
}

void Drillerdroid_State_Unknown18(void)
{
    RSDK_THIS(Drillerdroid);

    if (--entity->timer <= 0) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        CREATE_ENTITY(Drillerdroid, intToVoid(DRILLERDROID_TARGET), player1->position.x, player1->position.y)->target = (Entity *)player1;
        RSDK.PlaySfx(Drillerdroid->sfxTargeting, false, 255);
        entity->state = Drillerdroid_State_Unknown17;
    }
}

void Drillerdroid_State_Unknown11(void)
{
    RSDK_THIS(Drillerdroid);

    ++entity->rotation;
    Drillerdroid->field_34[0] += 0x4000;
    Drillerdroid->field_34[1] += 0x4000;
    Drillerdroid->field_2C[0] += 0x4000;
    Drillerdroid->field_2C[1] += 0x4000;
    entity->position.y -= 0x4000;
    Drillerdroid_CheckPlayerCollisions();
    if (Drillerdroid->field_34[0] == 0x100000) {
        entity->timer = 90;
        entity->state = Drillerdroid_State_Unknown12;
    }
}

void Drillerdroid_State_Unknown12(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_CheckPlayerCollisions();
    entity->alpha -= 8;
    if (entity->alpha <= 0) {
        entity->alpha = 0;
        entity->state = Drillerdroid_State_Unknown13;
    }
}

void Drillerdroid_State_Unknown13(void)
{
    RSDK_THIS(Drillerdroid);

    entity->rotation -= 4;
    Drillerdroid->field_2C[0] -= 0x10000;
    Drillerdroid->field_2C[1] -= 0x10000;
    Drillerdroid_CheckPlayerCollisions();

    if (!Drillerdroid->field_2C[0]) {
        entity->timer = 30;
        entity->state = Drillerdroid_State_Unknown14;
    }
}

void Drillerdroid_State_Destroyed(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_Explode();
    if (!--entity->invincibilityTimer) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 3);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 5);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        if (Drillerdroid->armorHealth == 1) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
            debris->direction     = FLIP_X;
            debris->drawFX        = FX_FLIP;
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 14336;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;

            int spawnX = entity->position.x - 0x300000;
            debris     = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, entity->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;

            spawnX += 0xD0000;
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, entity->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;

            spawnX += 0x530000;
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, entity->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX - 851968, entity->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;
        }
        entity->velocity.x        = RSDK.Rand(-6, 6) << 15;
        entity->velocity.y        = RSDK.Rand(-10, -6) << 15;
        entity->animator1.frameID = 4;
        entity->stateDraw         = Drillerdroid_StateDraw_Unknown2;
        entity->state             = Drillerdroid_State_Finish;

        foreach_active(SignPost, signPost) { signPost->position.x = entity->position.x; }
    }
}

void Drillerdroid_State_Finish(void)
{
    RSDK_THIS(Drillerdroid);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->visible ^= true;
    Drillerdroid_Explode();

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        entity->timer = 0;
        entity->state = Drillerdroid_State_DropSignPost;
    }
}

void Drillerdroid_State_DropSignPost(void)
{
    RSDK_THIS(Drillerdroid);

    if (++entity->timer == 60) {
        foreach_all(SignPost, signPost)
        {
            signPost->active = ACTIVE_NORMAL;
            signPost->state  = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfx_Twinkle, false, 255);
        }

        LRZ1Setup->fadeTimer = 1;
        destroyEntity(entity);
    }
}

void Drillerdroid_StateDraw_Unknown1(void)
{
    RSDK_THIS(Drillerdroid);
    Vector2 drawPos;

    RSDK.SetLimitedFade(0, 1, 2, entity->alpha, 32, 41);

    if (entity->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 32, 0xE0E0E0);
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    }
    entity->animator1.frameID = 1;
    drawPos.x                 = entity->position.x - 0x300000;
    drawPos.y                 = entity->position.y - Drillerdroid->field_2C[0];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 2;
    drawPos.y                 = entity->position.y + Drillerdroid->field_34[0];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 1;
    drawPos.x += 0xD0000;
    drawPos.y = entity->position.y - Drillerdroid->field_2C[1];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 2;
    drawPos.y                 = entity->position.y + Drillerdroid->field_34[1];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 1;
    drawPos.x += 0x530000;
    drawPos.y = entity->position.y - Drillerdroid->field_2C[0];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 2;
    drawPos.y                 = entity->position.y + Drillerdroid->field_34[0];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 1;
    drawPos.x -= 0xD0000;
    drawPos.y = entity->position.y - Drillerdroid->field_2C[1];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 2;
    drawPos.y                 = entity->position.y + Drillerdroid->field_34[1];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 0;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    switch (Drillerdroid->armorHealth) {
        case 1:
            drawPos                   = entity->position;
            entity->animator2.frameID = 6;
            RSDK.DrawSprite(&entity->animator2, NULL, false);

            entity->drawFX |= FX_FLIP;
            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->animator2, NULL, false);

            entity->direction         = FLIP_NONE;
            entity->drawFX            = 0;
            entity->animator2.frameID = 7;
            drawPos.x -= 0x300000;
            drawPos.y -= Drillerdroid->field_2C[0];
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

            drawPos.x += 0xD0000;
            drawPos.y = entity->position.y - Drillerdroid->field_2C[1];
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

            drawPos.x += 0x530000;
            drawPos.y = entity->position.y - Drillerdroid->field_2C[0];
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

            drawPos.x -= 0xD0000;
            drawPos.y = entity->position.y - Drillerdroid->field_2C[1];

            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
            break;
        case 2:
            drawPos.x                 = entity->position.x + 0x10000;
            drawPos.y                 = entity->position.y - 0x140000;
            entity->animator2.frameID = 4;
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

            entity->animator2.frameID = 5;
            //[Fallthrough]
        case 3:
            if (Drillerdroid->armorHealth == 3) {
                drawPos.x                 = entity->position.x + 0x10000;
                drawPos.y                 = entity->position.y - 0x140000;
                entity->animator2.frameID = 0;
                RSDK.DrawSprite(&entity->animator2, &drawPos, false);

                entity->animator2.frameID = 1;
            }
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

            drawPos                   = entity->position;
            entity->animator2.frameID = 2;
            RSDK.DrawSprite(&entity->animator2, NULL, false);

            entity->drawFX |= FX_FLIP;
            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->animator2, NULL, false);

            entity->direction         = FLIP_NONE;
            entity->drawFX            = FX_NONE;
            entity->animator2.frameID = 3;
            drawPos.x -= 0x300000;
            drawPos.y -= Drillerdroid->field_2C[0];
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

            drawPos.x += 0xD0000;
            drawPos.y = entity->position.y - Drillerdroid->field_2C[1];
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

            drawPos.x += 0x530000;
            drawPos.y = entity->position.y - Drillerdroid->field_2C[0];
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

            drawPos.x -= 0xD0000;
            drawPos.y = entity->position.y - Drillerdroid->field_2C[1];
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
            break;
    }
    RSDK.CopyPalette(1, 32, 0, 32, 10);
    RSDK.SetPaletteEntry(0, 32u, 0x282028);
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void Drillerdroid_StateDraw_Unknown2(void)
{
    RSDK_THIS(Drillerdroid);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Drillerdroid_State1_Unknown(void)
{
    RSDK_THIS(Drillerdroid);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->target) {
        entity->position.x = entity->target->position.x;
        entity->position.y = entity->target->position.y;
    }
    entity->alpha += 32;
    entity->field_B0 -= 0x20000;
    if (entity->field_B0 <= 0xC0000) {
        entity->alpha    = 0;
        entity->field_B0 = 0x2C0000;
    }

    entity->field_B4 = entity->field_B0;
    if (++entity->timer == 60)
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 4, &entity->animator3, true, 0);

    if (entity->timer == 96) {
        entity->target                 = NULL;
        Drillerdroid->boss->position.x = entity->position.x;

        if (entity->position.x < (Zone->screenBoundsL1[0] + 64) << 16 || entity->position.x > (Zone->screenBoundsR1[0] - 64) << 16)
            Drillerdroid->boss->position.x = (Zone->screenBoundsL1[0] + 64) << 16;
        else if (entity->position.x > (Zone->screenBoundsR1[0] - 64) << 16)
            Drillerdroid->boss->position.x = (Zone->screenBoundsR1[0] - 64) << 16;
    }
    if (entity->timer == 128)
        destroyEntity(entity);
}

void Drillerdroid_StateDraw1_Unknown(void)
{
    RSDK_THIS(Drillerdroid);
    Vector2 drawPos;

    int x = ((entity->position.x + 0x400000) & 0xFF800000) - 0x400000;
    if (!(Zone->timer & 8)) {
        RSDK.DrawLine(x + 0x10000, entity->position.y, x + 0x7F0000, entity->position.y, 0xE0E0E0, 192, INK_ADD, false);
        RSDK.DrawLine(x, entity->position.y - 0x80000, x, entity->position.y + 0x80000, 0xE0E0E0, 160, INK_ADD, false);
        RSDK.DrawLine(x + 0x800000, entity->position.y - 0x80000, x + 0x800000, entity->position.y + 0x80000, 0xE0E0E0, 160, INK_ADD, false);
    }

    entity->animator1.frameID = 0;
    entity->inkEffect         = INK_ALPHA;
    entity->direction         = FLIP_NONE;
    drawPos.x                 = entity->position.x;
    drawPos.y                 = entity->position.y;
    drawPos.x                 = entity->position.x - entity->field_B0;
    drawPos.y -= entity->field_B4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->direction = FLIP_X;
    drawPos.x         = entity->position.x;
    drawPos.y         = entity->position.y;
    drawPos.x         = entity->field_B0 + entity->position.x;
    drawPos.y -= entity->field_B4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 1;
    entity->direction         = FLIP_NONE;
    drawPos.x                 = entity->position.x;
    drawPos.y                 = entity->position.y;
    drawPos.x                 = entity->position.x - entity->field_B0;
    drawPos.y += entity->field_B4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->direction = FLIP_X;
    drawPos.x         = entity->position.x;
    drawPos.y         = entity->position.y;
    drawPos.x         = entity->field_B0 + entity->position.x;
    drawPos.y += entity->field_B4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->inkEffect = INK_NONE;
    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void Drillerdroid_EditorDraw(void)
{
    RSDK_THIS(Drillerdroid);
    Drillerdroid->field_2C[0] = 0;
    Drillerdroid->field_2C[1] = 0;
    Drillerdroid->field_34[0] = 0x100000;
    Drillerdroid->field_34[1] = 0x100000;
    Drillerdroid->field_3C[0] = 4;
    Drillerdroid->field_3C[1] = 0;
    Drillerdroid->field_44[0] = 4;
    Drillerdroid->field_44[1] = 0;
    Drillerdroid->field_4C[0] = 0;
    Drillerdroid->field_4C[1] = 0;
    Drillerdroid->field_54[0] = 0;
    Drillerdroid->field_54[1] = 0;
    Drillerdroid->field_5C[0] = 1;
    Drillerdroid->field_5C[1] = 1;
    Drillerdroid->field_5C[2] = 1;
    Drillerdroid->field_5C[3] = 1;
    Drillerdroid->field_5C[4] = 1;

    if (entity->type) {
        if (entity->type == DRILLERDROID_TARGET || entity->type == DRILLERDROID_UNKNOWN) {
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 2, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 3, &entity->animator2, true, 0);
            Drillerdroid_StateDraw1_Unknown();
        }
    }
    else {
        entity->drawFX        = FX_NONE;
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &entity->animator2, true, 0);
        Drillerdroid_StateDraw_Unknown1();

        DrawHelpers_DrawArenaBounds(0x00C0F0, 1 | 2 | 4 | 8, -328, -SCREEN_YSIZE, 324, 96);
    }
}

void Drillerdroid_EditorLoad(void)
{
    Drillerdroid->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Drillerdroid.bin", SCOPE_STAGE);
    Drillerdroid->ticFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Drillerdroid, type);
    RSDK_ENUM_VAR("Drillerdroid", DRILLERDROID_MAIN);
}
#endif

void Drillerdroid_Serialize(void) { RSDK_EDITABLE_VAR(Drillerdroid, VAR_UINT8, type); }
