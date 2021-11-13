#include "SonicMania.h"

ObjectDrillerdroidO *DrillerdroidO;

void DrillerdroidO_Update(void)
{
    RSDK_THIS(DrillerdroidO);
    StateMachine_Run(entity->state);
}

void DrillerdroidO_LateUpdate(void) {}

void DrillerdroidO_StaticUpdate(void)
{
    if (DrillerdroidO->shouldPlayDrillSfx) {
        if (!DrillerdroidO->playingDrillSfx) {
            RSDK.PlaySfx(DrillerdroidO->sfxDrill, 43643, 255);
            DrillerdroidO->playingDrillSfx = true;
        }
        DrillerdroidO->shouldPlayDrillSfx = 0;
    }
    else if (DrillerdroidO->playingDrillSfx) {
        RSDK.StopSFX(DrillerdroidO->sfxDrill);
        DrillerdroidO->playingDrillSfx = false;
    }
}

void DrillerdroidO_Draw(void)
{
    RSDK_THIS(DrillerdroidO);
    StateMachine_Run(entity->stateDraw);
}

void DrillerdroidO_Create(void *data)
{
    RSDK_THIS(DrillerdroidO);

    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->visible = true;
            if (data)
                entity->type = voidToInt(data);
            switch (entity->type) {
                case DRILLERDROIDO_MAIN:
                    entity->active        = ACTIVE_BOUNDS;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 5, &entity->animator2, true, 0);
                    entity->drawOrder             = Zone->drawOrderHigh;
                    DrillerdroidO->boss           = (Entity *)entity;
                    DrillerdroidO->childSlotStart = RSDK_sceneInfo->entitySlot + 2;
                    DrillerdroidO->field_8C       = 4;
                    entity->state                 = DrillerdroidO_State_SetupArena;
                    entity->stateDraw             = DrillerdroidO_StateDraw_Unknown;
                    break;
                case DRILLERDROIDO_1:
                    entity->active        = ACTIVE_BOUNDS;
                    entity->drawFX        = FX_FLIP;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(DrillerdroidO->ticFrames, 0, &entity->animator1, true, 0);
                    entity->drawOrder = Zone->drawOrderHigh;
                    entity->state     = DrillerdroidO_State1_Unknown;
                    entity->stateDraw = DrillerdroidO_StateDraw1_Unknown;
                    break;
                case DRILLERDROIDO_2:
                case DRILLERDROIDO_3:
                case DRILLERDROIDO_4:
                    entity->active        = ACTIVE_NORMAL;
                    entity->drawFX        = FX_FLIP;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 2, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 3, &entity->animator2, true, 0);
                    entity->drawOrder = Zone->drawOrderHigh;
                    entity->state     = DrillerdroidO_State2_Unknown;
                    entity->stateDraw = DrillerdroidO_StateDraw2_Unknown;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void DrillerdroidO_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        DrillerdroidO->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Drillerdroid.bin", SCOPE_STAGE);
    DrillerdroidO->ticFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);

    DrillerdroidO->hitbox1.left   = -25;
    DrillerdroidO->hitbox1.top    = -16;
    DrillerdroidO->hitbox1.right  = 25;
    DrillerdroidO->hitbox1.bottom = 16;

    DrillerdroidO->hitbox2.left   = -58;
    DrillerdroidO->hitbox2.top    = -74;
    DrillerdroidO->hitbox2.right  = -25;
    DrillerdroidO->hitbox2.bottom = 16;

    DrillerdroidO->hitbox3.left   = 25;
    DrillerdroidO->hitbox3.top    = -74;
    DrillerdroidO->hitbox3.right  = 58;
    DrillerdroidO->hitbox3.bottom = 16;

    DrillerdroidO->hitbox4.left   = -58;
    DrillerdroidO->hitbox4.top    = 16;
    DrillerdroidO->hitbox4.right  = -25;
    DrillerdroidO->hitbox4.bottom = 53;

    DrillerdroidO->hitbox5.left   = 25;
    DrillerdroidO->hitbox5.top    = 16;
    DrillerdroidO->hitbox5.right  = 58;
    DrillerdroidO->hitbox5.bottom = 53;

    DrillerdroidO->hitbox6.left   = -6;
    DrillerdroidO->hitbox6.top    = 16;
    DrillerdroidO->hitbox6.right  = 6;
    DrillerdroidO->hitbox6.bottom = 16;

    DrillerdroidO->hitbox7.left   = -58;
    DrillerdroidO->hitbox7.top    = 37;
    DrillerdroidO->hitbox7.right  = -42;
    DrillerdroidO->hitbox7.bottom = 53;

    DrillerdroidO->hitbox8.left   = 42;
    DrillerdroidO->hitbox8.top    = 37;
    DrillerdroidO->hitbox8.right  = 58;
    DrillerdroidO->hitbox8.bottom = 53;

    DrillerdroidO->hitbox9.left   = -32;
    DrillerdroidO->hitbox9.top    = -192;
    DrillerdroidO->hitbox9.right  = 32;
    DrillerdroidO->hitbox9.bottom = 192;

    DrillerdroidO->field_64[0]        = 4;
    DrillerdroidO->field_5C[0]        = 4;
    DrillerdroidO->field_64[1]        = 0;
    DrillerdroidO->field_5C[1]        = 0;
    DrillerdroidO->field_54[0]        = 0x100000;
    DrillerdroidO->field_54[1]        = 0x100000;
    DrillerdroidO->field_4C[0]        = 0;
    DrillerdroidO->field_4C[1]        = 0;
    DrillerdroidO->field_74[0]        = 0;
    DrillerdroidO->field_74[1]        = 0;
    DrillerdroidO->field_6C[0]        = 0;
    DrillerdroidO->field_6C[1]        = 0;
    DrillerdroidO->active             = ACTIVE_ALWAYS;
    DrillerdroidO->shouldPlayDrillSfx = 0;
    DrillerdroidO->playingDrillSfx    = false;
    DrillerdroidO->field_90           = 0;
    DrillerdroidO->sfxHit             = RSDK.GetSFX("Stage/BossHit.wav");
    DrillerdroidO->sfxExplosion       = RSDK.GetSFX("Stage/Explosion2.wav");
    DrillerdroidO->sfxDrill           = RSDK.GetSFX("LRZ/Drill.wav");
    DrillerdroidO->sfxImpact          = RSDK.GetSFX("Stage/Impact4.wav");
    DrillerdroidO->sfxJump            = RSDK.GetSFX("LRZ/DrillJump.wav");
    DrillerdroidO->sfxTargeting       = RSDK.GetSFX("Stage/Targeting1.wav");
    DrillerdroidO->sfxSizzle          = RSDK.GetSFX("LRZ/Sizzle.wav");
}

void DrillerdroidO_CheckPlayerCollisions(void)
{
    RSDK_THIS(DrillerdroidO);

    if (entity->invincibilityTimer)
        entity->invincibilityTimer--;

    foreach_active(Player, player)
    {
        int playerX = player->position.y;
        int playerY = player->position.x;
        int velX    = player->velocity.x;
        int velY    = player->velocity.y;
        int side    = Player_CheckCollisionBox(player, entity, &DrillerdroidO->hitbox1);

        switch (side) {
            case C_TOP:
                Player_CheckHit(player, entity);
                if (entity->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;
            case C_BOTTOM:
                if (entity->rotation) {
                    if (!entity->invincibilityTimer) {
                        player->velocity.x = velX;
                        player->velocity.y = velY;
                        player->position.x = playerY;
                        player->position.y = playerX;
                        if (Player_CheckBossHit(player, entity)) {
                            if (!--entity->health) {
                                RSDK_sceneInfo->timeEnabled = false;
                                Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                                entity->invincibilityTimer = 60;
                                DrillerdroidO->field_90    = 0;
                                entity->state              = DrillerdroidO_State_Destroyed;
                            }
                            else {
                                RSDK.PlaySfx(DrillerdroidO->sfxHit, false, 255);
                                entity->invincibilityTimer = 30;
                            }
                        }
                    }
                }
                else if (entity->velocity.y >= 0) {
                    player->collisionFlagV |= 2;
                }
                continue;
        }

        Player_CheckCollisionBox(player, entity, &DrillerdroidO->hitbox2);
        Player_CheckCollisionBox(player, entity, &DrillerdroidO->hitbox3);

        if (entity->state == DrillerdroidO_State_Unknown6) {
            if (Player_CheckCollisionTouch(player, entity, &DrillerdroidO->hitbox4)
                || Player_CheckCollisionTouch(player, entity, &DrillerdroidO->hitbox5)) {
                Player_CheckHit(player, entity);
            }
        }
        else {
            if (entity->state == DrillerdroidO_State_Unknown2 && (Player_CheckCollisionBox(player, entity, &DrillerdroidO->hitbox4) == C_BOTTOM
                || Player_CheckCollisionBox(player, entity, &DrillerdroidO->hitbox5) == C_BOTTOM)) {
                if (entity->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                else
                    Player_CheckHit(player, entity);
            }
        }
    }
}

void DrillerdroidO_Explode(void)
{
    RSDK_THIS(DrillerdroidO);

    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(Drillerdroid->sfxExplosion, false, 255);
        if (!(Zone->timer & 3)) {
            int x = entity->position.y + (RSDK.Rand(-19, 20) << 16);
            int y = entity->position.x + (RSDK.Rand(-24, 25) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void DrillerdroidO_SpawnDebris(int offset)
{
    RSDK_THIS(DrillerdroidO);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + entity->position.x, entity->position.y + 0x400000);
    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x    = RSDK.Rand(0, 6) << 15;
    debris->velocity.y    = RSDK.Rand(-12, -8) << 15;
    debris->gravity       = 0x3800;
    debris->drawOrder     = Zone->drawOrderLow;
    debris->updateRange.x = 0x400000;
    debris->updateRange.y = 0x400000;

    debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + entity->position.x, entity->position.y + 0x400000);
    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x    = RSDK.Rand(-6, 0) << 15;
    debris->velocity.y    = RSDK.Rand(-12, -8) << 15;
    debris->gravity       = 0x3800;
    debris->drawOrder     = Zone->drawOrderLow;
    debris->updateRange.x = 0x400000;
    debris->updateRange.y = 0x400000;
}

void DrillerdroidO_State_SetupArena(void)
{
    RSDK_THIS(DrillerdroidO);

    if (++entity->timer >= 2) {
        entity->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + RSDK_screens->centerX;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 96;
        Zone->screenBoundsT1[0]     = Zone->screenBoundsB1[0] - 240;
        entity->startY              = entity->position.y;
        entity->active              = ACTIVE_NORMAL;
        entity->position.y          = (RSDK_screens->position.y - 192) << 16;
        entity->visible             = true;
        entity->state               = DrillerdroidO_State_StartBoss;
    }
}

void DrillerdroidO_State_StartBoss(void)
{
    RSDK_THIS(DrillerdroidO);

    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = RSDK_screens->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX;
        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        entity->health = 6;
        CREATE_ENTITY(DrillerdroidO, intToVoid(DRILLERDROIDO_2), entity->position.x, entity->startY);
        entity->position.x = 0;
        RSDK.PlaySfx(DrillerdroidO->sfxTargeting, false, 255);
        entity->state           = DrillerdroidO_State_Unknown1;
        DrillerdroidO->field_84 = 1;
        DrillerdroidO->field_90 = 1;
    }
}

void DrillerdroidO_State_Unknown2(void)
{
    RSDK_THIS(DrillerdroidO);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    DrillerdroidO_CheckPlayerCollisions();

    if (entity->position.y >= (Zone->screenBoundsB1[0] - 112) << 16) {
        entity->position.y = (Zone->screenBoundsB1[0] - 112) << 16;
        RSDK.PlaySfx(DrillerdroidO->sfxImpact, false, 255);

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        if (!camera->shakePos.y)
            camera->shakePos.y = 4;

        entity->velocity.y >>= 1;
        entity->startY = entity->position.y + 0x100000;
        if (!DrillerdroidO->field_84) {
            for (int i = 0; i < 4; ++i) {
                DrillerdroidO_SpawnDebris(-0x300000);
                DrillerdroidO_SpawnDebris(0x300000);
                DrillerdroidO_SpawnDebris(-0x190000);
                DrillerdroidO_SpawnDebris(0x190000);
            }
        }

        EntityLRZRockPile *pile = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, LRZRockPile);
        pile->timer             = 1;
        pile->flag              = true;
        entity->timer           = 60;
        entity->angle           = 0;
        entity->state           = DrillerdroidO_State_Unknown3;
    }
}

void DrillerdroidO_State_Unknown3(void)
{
    RSDK_THIS(DrillerdroidO);

    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0xE000;
    DrillerdroidO->field_54[0] = entity->startY - entity->position.y;
    DrillerdroidO->field_54[1] = entity->startY - entity->position.y;
    DrillerdroidO_CheckPlayerCollisions();

    if (entity->velocity.y < 0) {
        if (entity->position.y < entity->startY)
            entity->state = DrillerdroidO_State_Unknown4;
    }
}

void DrillerdroidO_State_Unknown4(void)
{
    RSDK_THIS(DrillerdroidO);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x14000;
    DrillerdroidO->field_54[0] = entity->startY - entity->position.y;
    DrillerdroidO->field_54[1] = entity->startY - entity->position.y;
    DrillerdroidO_CheckPlayerCollisions();

    if (entity->velocity.y > 0) {
        if (entity->position.y > entity->startY) {
            entity->position.y         = entity->startY;
            DrillerdroidO->field_54[0] = 0;
            DrillerdroidO->field_54[1] = 0;
            entity->state              = DrillerdroidO_State_Unknown5;
        }
    }
}

void DrillerdroidO_State_Unknown5(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_CheckPlayerCollisions();
    if (--entity->timer <= 0) {
        if (DrillerdroidO->field_84 == 1) {
            EntityLavaGeyser *geyser = CREATE_ENTITY(LavaGeyser, NULL, entity->position.x, entity->position.y + 0x580000);
            geyser->force            = 56 << 12;
            geyser->duration         = 120;
            geyser->drawOrder        = Zone->drawOrderHigh;
            geyser->state            = LavaGeyser_HandleSetup;
        }
        entity->timer = 240;
        entity->state = DrillerdroidO_State_Unknown6;
    }
}

void DrillerdroidO_State_Unknown6(void)
{
    RSDK_THIS(DrillerdroidO);

    ++DrillerdroidO->shouldPlayDrillSfx;
    RSDK.ProcessAnimation(&entity->animator1);

    for (int i = 0; i < 2; ++i) {
        if (DrillerdroidO->field_5C[i]) {
            DrillerdroidO->field_5C[i]--;
        }
        else if (DrillerdroidO->field_6C[i]) {
            DrillerdroidO->field_4C[i] -= 0x20000;
            if (!DrillerdroidO->field_4C[i]) {
                DrillerdroidO->field_6C[i] = 0;
                DrillerdroidO->field_5C[i] = 10;
            }
        }
        else {
            DrillerdroidO->field_4C[i] += 0x40000;
            if (DrillerdroidO->field_4C[i] == 0x100000)
                DrillerdroidO->field_6C[i] = 1;
        }

        if (DrillerdroidO->field_64[i]) {
            DrillerdroidO->field_64[i]--;
        }
        else if (DrillerdroidO->field_74[i]) {
            DrillerdroidO->field_54[i] -= 0x40000;
            if (!DrillerdroidO->field_54[i]) {
                DrillerdroidO->field_74[i] = 0;
                DrillerdroidO->field_64[i] = 5;
            }
        }
        else {
            DrillerdroidO->field_54[i] += 0x80000;
            if (DrillerdroidO->field_54[i] == 0x100000)
                DrillerdroidO->field_74[i] = 1;
        }
    }

    if (!DrillerdroidO->field_84) {
        if (DrillerdroidO->field_54[0] == 0x80000) {
            DrillerdroidO_SpawnDebris(-0x300000);
            DrillerdroidO_SpawnDebris(0x300000);
        }
        else if (DrillerdroidO->field_54[1] == 0x80000) {
            DrillerdroidO_SpawnDebris(-0x190000);
            DrillerdroidO_SpawnDebris(0x190000);
        }
    }

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (!camera->shakePos.y)
        camera->shakePos.y = 4;

    entity->position.y ^= 0x10000;
    DrillerdroidO_CheckPlayerCollisions();

    if (DrillerdroidO->field_84 == 1 && entity->timer < 180) {
        if (entity->alpha < 512) {
            entity->alpha += 4;
        }
    }
    if (--entity->timer <= 0)
        entity->state = DrillerdroidO_State_Unknown7;
}

void DrillerdroidO_State_Unknown7(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO->field_4C[0] -= 0x20000;
    if (DrillerdroidO->field_4C[0] < 0)
        DrillerdroidO->field_4C[0] = 0;

    DrillerdroidO->field_4C[1] -= 0x20000;
    if (DrillerdroidO->field_4C[1] < 0)
        DrillerdroidO->field_4C[1] = 0;

    DrillerdroidO->field_54[0] -= 0x20000;
    if (DrillerdroidO->field_54[0] < 0)
        DrillerdroidO->field_54[0] = 0;

    DrillerdroidO->field_54[1] -= 0x20000;
    if (DrillerdroidO->field_54[1] < 0)
        DrillerdroidO->field_54[1] = 0;

    if (!DrillerdroidO->field_4C[0] && !DrillerdroidO->field_4C[1] && !DrillerdroidO->field_54[0] && !DrillerdroidO->field_54[1]) {
        entity->timer = 60;
        entity->state = DrillerdroidO_State_Unknown8;
    }
}

void DrillerdroidO_State_Unknown8(void)
{
    RSDK_THIS(DrillerdroidO);

    if (--entity->timer <= 0) {
        if (DrillerdroidO->field_84 == 1) {
            DrillerdroidO->field_84 = 0;
            RSDK.PlaySfx(DrillerdroidO->sfxSizzle, false, 255);
            entity->state = DrillerdroidO_State_Unknown16;
        }
        else {
            entity->state = DrillerdroidO_State_Unknown9;
        }
    }
    DrillerdroidO_CheckPlayerCollisions();
}

void DrillerdroidO_State_Unknown9(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO->field_54[0] += 0x10000;
    DrillerdroidO->field_54[1] += 0x10000;
    entity->position.y -= 0x10000;
    DrillerdroidO_CheckPlayerCollisions();

    if (DrillerdroidO->field_54[0] == 0x100000) {
        entity->timer = 30;
        entity->state = DrillerdroidO_State_Unknown10;
    }
}

void DrillerdroidO_State_Unknown10(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_CheckPlayerCollisions();
    if (--entity->timer <= 0) {
        entity->velocity.y = 0x78000;
        RSDK.PlaySfx(DrillerdroidO->sfxJump, false, 255);
        entity->state = DrillerdroidO_State_Unknown11;
    }
}

void DrillerdroidO_State_Unknown11(void)
{
    RSDK_THIS(DrillerdroidO);

    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x1C000;
    DrillerdroidO->field_54[0] = entity->startY - entity->position.y;
    DrillerdroidO->field_54[1] = entity->startY - entity->position.y;

    if (entity->velocity.y < 0) {
        if (DrillerdroidO->field_54[0] >= 0x100000) {
            DrillerdroidO->field_54[0] = 0x100000;
            DrillerdroidO->field_54[1] = 0x100000;
            entity->state              = DrillerdroidO_State_Unknown12;
        }
    }
}

void DrillerdroidO_State_Unknown12(void)
{
    RSDK_THIS(DrillerdroidO);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (entity->velocity.y >= 0) {
        DrillerdroidO->field_64[0] = 4;
        DrillerdroidO->field_5C[0] = 4;
        DrillerdroidO->field_64[1] = 0;
        DrillerdroidO->field_5C[1] = 0;
        DrillerdroidO->field_54[0] = 0x100000;
        DrillerdroidO->field_54[1] = 0x100000;
        DrillerdroidO->field_4C[0] = 0;
        DrillerdroidO->field_4C[1] = 0;
        DrillerdroidO->field_6C[0] = 0;
        DrillerdroidO->field_6C[1] = 0;
        DrillerdroidO->field_74[0] = 0;
        DrillerdroidO->field_74[1] = 0;
        entity->position.x         = 0;

        if (entity->health <= DrillerdroidO->field_8C && DrillerdroidO->field_8C) {
            entity->timer           = 90;
            DrillerdroidO->field_90 = 0;
            entity->state           = DrillerdroidO_State_Unknown13;
        }
        else {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            CREATE_ENTITY(DrillerdroidO, intToVoid(DRILLERDROIDO_2), player1->position.x, player1->position.y)->target = (Entity *)player1;
            RSDK.PlaySfx(DrillerdroidO->sfxTargeting, false, 255);
            entity->state = DrillerdroidO_State_Unknown1;
        }
    }
}

void DrillerdroidO_State_Unknown13(void)
{
    RSDK_THIS(DrillerdroidO);

    if (--entity->timer <= 60) {
        ++DrillerdroidO->shouldPlayDrillSfx;
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        if (!camera->shakePos.y)
            camera->shakePos.y = 4;
    }

    if (entity->timer <= 0) {
        DrillerdroidO->field_8C -= 2;
        for (int i = 0; i < 4; ++i) {
            EntityLRZRockPile *pile = RSDK_GET_ENTITY(DrillerdroidO->childSlotStart++, LRZRockPile);
            pile->timer             = 1;
            pile->flag              = true;
        }
        entity->timer = 30;
        entity->state = DrillerdroidO_State_Unknown14;
    }
}

void DrillerdroidO_State_Unknown14(void)
{
    RSDK_THIS(DrillerdroidO);

    if (--entity->timer <= 0) {
        entity->position.y += 0x1000000;
        Zone->screenBoundsB1[0] += 256;
        entity->state = DrillerdroidO_State_Unknown15;
    }
}

void DrillerdroidO_State_Unknown15(void)
{
    RSDK_THIS(DrillerdroidO);

    Zone->screenBoundsT1[0] = RSDK_screens->position.y;
    if (Zone->screenBoundsT1[0] == Zone->screenBoundsB1[0] - 240) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        CREATE_ENTITY(DrillerdroidO, intToVoid(DRILLERDROIDO_2), player1->position.x, player1->position.y)->target = (Entity *)player1;
        RSDK.PlaySfx(DrillerdroidO->sfxTargeting, false, 255);
        DrillerdroidO->field_90 = 1;
        entity->state           = DrillerdroidO_State_Unknown1;
    }
}

void DrillerdroidO_State_Unknown1(void)
{
    RSDK_THIS(DrillerdroidO);

    if (entity->position.x) {
        entity->timer = 240;
        entity->state = DrillerdroidO_State_Unknown2;

        foreach_active(DrillerdroidO, target)
        {
            if (target->type == 1 && entity->position.y < target->position.y
                && RSDK.CheckObjectCollisionTouchBox(target, &DrillerdroidO->hitbox9, entity, &DrillerdroidO->hitbox6)) {
                entity->position.x      = target->position.x;
                DrillerdroidO->field_84 = 1;
                foreach_break;
            }
        }

        foreach_active(DrillerdroidO, drillerdroid)
        {
            if (drillerdroid->type == 1 && entity->position.y < drillerdroid->position.y) {
                if (RSDK.CheckObjectCollisionTouchBox(drillerdroid, &DrillerdroidO->hitbox9, entity, &DrillerdroidO->hitbox7)) {
                    entity->position.x = drillerdroid->position.x + 0x550000;
                    foreach_break;
                }
                if (RSDK.CheckObjectCollisionTouchBox(drillerdroid, &DrillerdroidO->hitbox9, entity, &DrillerdroidO->hitbox8)) {
                    entity->position.x = drillerdroid->position.x - 0x550000;
                    foreach_break;
                }
            }
        }
    }
}

void DrillerdroidO_State_Unknown16(void)
{
    RSDK_THIS(DrillerdroidO);

    ++entity->rotation;
    DrillerdroidO->field_54[0] += 0x4000;
    DrillerdroidO->field_54[1] += 0x4000;
    DrillerdroidO->field_4C[0] += 0x4000;
    DrillerdroidO->field_4C[1] += 0x4000;
    entity->position.y -= 0x4000;
    DrillerdroidO_CheckPlayerCollisions();
    if (DrillerdroidO->field_54[0] == 0x100000) {
        entity->timer = 90;
        entity->state = DrillerdroidO_State_Unknown17;
    }
}

void DrillerdroidO_State_Unknown17(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_CheckPlayerCollisions();

    entity->alpha -= 6;
    if (entity->alpha < 0)
        entity->alpha = 0;
    if (--entity->timer <= 0)
        entity->state = DrillerdroidO_State_Unknown18;
}

void DrillerdroidO_State_Unknown18(void)
{
    RSDK_THIS(DrillerdroidO);

    entity->rotation -= 4;
    DrillerdroidO->field_4C[0] -= 0x10000;
    DrillerdroidO->field_4C[1] -= 0x10000;
    DrillerdroidO_CheckPlayerCollisions();

    if (!DrillerdroidO->field_4C[0]) {
        entity->timer = 60;
        entity->state = DrillerdroidO_State_Unknown10;
    }
}

void DrillerdroidO_State_Destroyed(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_Explode();
    if (!--entity->invincibilityTimer) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 3);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 5);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        entity->velocity.x        = RSDK.Rand(-4, 4) << 15;
        entity->velocity.y        = (RSDK.Rand(-8, -4) << 15);
        entity->animator1.frameID = 4;
        entity->stateDraw         = DrillerdroidO_StateDraw_Unknown2;
        entity->state             = DrillerdroidO_State_Finish;
    }
}

void DrillerdroidO_State_Finish(void)
{
    RSDK_THIS(DrillerdroidO);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->visible ^= true;
    DrillerdroidO_Explode();

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        Zone->screenBoundsR1[0] += 424;
        destroyEntity(entity);
    }
}

void DrillerdroidO_StateDraw_Unknown(void)
{
    RSDK_THIS(DrillerdroidO);
    Vector2 drawPos;

    RSDK.SetLimitedFade(0, 1, 2, entity->alpha, 32, 41);
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (entity->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 32, 0xE0E0E0);
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    }

    entity->animator1.frameID = 1;
    drawPos.x -= 0x300000;
    drawPos.y -= DrillerdroidO->field_4C[0];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 2;
    drawPos.y                 = entity->position.y + DrillerdroidO->field_54[0];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 1;
    drawPos.x += 0xD0000;
    drawPos.y = entity->position.y - DrillerdroidO->field_4C[1];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 2;
    drawPos.y                 = entity->position.y + DrillerdroidO->field_54[1];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 1;
    drawPos.x += 0x530000;
    drawPos.y = entity->position.y - DrillerdroidO->field_4C[0];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 2;
    drawPos.y                 = entity->position.y + DrillerdroidO->field_54[0];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 1;
    drawPos.x -= 0xD0000;
    drawPos.y = entity->position.y - DrillerdroidO->field_4C[1];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 2;
    drawPos.y                 = entity->position.y + DrillerdroidO->field_54[1];
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 0;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->drawFX |= FX_ROTATE;
    drawPos.x                 = entity->position.x + 0x10000;
    drawPos.y                 = entity->position.y - 0x140000;
    entity->animator2.frameID = 0;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    entity->animator2.frameID = 1;
    entity->rotation          = 0x200 - entity->rotation;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    entity->rotation = 0x200 - entity->rotation;
    entity->drawFX   = 0;
    RSDK.CopyPalette(1, 32, 0, 32, 10);
    RSDK.SetPaletteEntry(0, 32, 0x282028);
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void DrillerdroidO_StateDraw_Unknown2(void)
{
    RSDK_THIS(DrillerdroidO);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void DrillerdroidO_State2_Unknown(void)
{
    RSDK_THIS(DrillerdroidO);

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
        entity->target                  = NULL;
        DrillerdroidO->boss->position.x = entity->position.x;

        if (entity->position.x < (Zone->screenBoundsL1[0] + 64) << 16 || entity->position.x > (Zone->screenBoundsR1[0] - 64) << 16)
            DrillerdroidO->boss->position.x = (Zone->screenBoundsL1[0] + 64) << 16;
        else if (entity->position.x > (Zone->screenBoundsR1[0] - 64) << 16)
            DrillerdroidO->boss->position.x = (Zone->screenBoundsR1[0] - 64) << 16;
    }
    if (entity->timer == 128)
        destroyEntity(entity);
}

void DrillerdroidO_StateDraw2_Unknown(void)
{
    RSDK_THIS(DrillerdroidO);
    Vector2 drawPos;

    entity->animator1.frameID = 0;
    entity->inkEffect         = INK_ALPHA;
    entity->direction         = FLIP_NONE;
    drawPos.x                 = entity->position.x - entity->field_B0;
    drawPos.y                 = entity->position.y - entity->field_B4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->direction = FLIP_X;
    drawPos.x         = entity->position.x + entity->field_B0;
    drawPos.y         = entity->position.y - entity->field_B4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 1;
    entity->direction         = FLIP_NONE;
    drawPos.x                 = entity->position.x - entity->field_B0;
    drawPos.y                 = entity->position.y + entity->field_B4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->direction = FLIP_X;
    drawPos.x         = entity->position.x + entity->field_B0;
    drawPos.y         = entity->position.y + entity->field_B4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->inkEffect = INK_NONE;
    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void DrillerdroidO_State1_Unknown(void)
{
    RSDK_THIS(DrillerdroidO);

    bool32 flag = true;
    foreach_active(DrillerdroidO, drillerdroid)
    {
        if (!drillerdroid->type && RSDK.CheckObjectCollisionTouchBox(drillerdroid, &DrillerdroidO->hitbox6, entity, &DrillerdroidO->hitbox9)) {
            flag = false;
            foreach_break;
        }
    }

    if (flag && DrillerdroidO->field_90) {
        if (!(Zone->timer & 0x3F)) {
            EntityLRZFireball *fireball = CREATE_ENTITY(LRZFireball, LRZFireball_StateChild_Type2, entity->position.x, entity->position.y + 0x940000);
            fireball->angle             = 0xEE;
            fireball->rotation          = 0xEE;
            fireball->groundVel         = -0x300;
            fireball->velocity.x        = fireball->groundVel * RSDK.Sin512(0x12);
            fireball->velocity.y        = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);

            fireball             = CREATE_ENTITY(LRZFireball, LRZFireball_StateChild_Type2, entity->position.x, entity->position.y + 0x940000);
            fireball->angle      = 0x116;
            fireball->rotation   = 0x116;
            fireball->groundVel  = -0x300;
            fireball->velocity.x = fireball->groundVel * RSDK.Sin512(-0x16);
            fireball->velocity.y = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);
            RSDK.PlaySfx(LRZFireball->sfxFireball, false, 255);
        }
    }
}

void DrillerdroidO_StateDraw1_Unknown(void) {}

#if RETRO_INCLUDE_EDITOR
void DrillerdroidO_EditorDraw(void)
{
    RSDK_THIS(DrillerdroidO);
    switch (entity->type) {
        case DRILLERDROIDO_MAIN:
            RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 5, &entity->animator2, true, 0);
            DrillerdroidO_StateDraw_Unknown();
            break;
        case DRILLERDROIDO_1:
            RSDK.SetSpriteAnimation(DrillerdroidO->ticFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 3, &entity->animator2, true, 0);
            DrillerdroidO_StateDraw1_Unknown();
            break;
        case DRILLERDROIDO_2:
        case DRILLERDROIDO_3:
        case DRILLERDROIDO_4:
            RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 2, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 3, &entity->animator2, true, 0);
            DrillerdroidO_StateDraw2_Unknown();
            break;
        default: break;
    }
}

void DrillerdroidO_EditorLoad(void)
{
    DrillerdroidO->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Drillerdroid.bin", SCOPE_STAGE);
    DrillerdroidO->ticFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(DrillerdroidO, type);
    RSDK_ENUM_VAR("Drillerdroid", DRILLERDROID_MAIN);
    RSDK_ENUM_VAR("Type 1", DRILLERDROID_1);
}
#endif

void DrillerdroidO_Serialize(void) { RSDK_EDITABLE_VAR(DrillerdroidO, VAR_UINT8, type); }
