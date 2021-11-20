#include "SonicMania.h"

ObjectDrillerdroidO *DrillerdroidO;

void DrillerdroidO_Update(void)
{
    RSDK_THIS(DrillerdroidO);
    StateMachine_Run(self->state);
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
    StateMachine_Run(self->stateDraw);
}

void DrillerdroidO_Create(void *data)
{
    RSDK_THIS(DrillerdroidO);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->visible = true;
            if (data)
                self->type = voidToInt(data);
            switch (self->type) {
                case DRILLERDROIDO_MAIN:
                    self->active        = ACTIVE_BOUNDS;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 5, &self->animator2, true, 0);
                    self->drawOrder             = Zone->drawOrderHigh;
                    DrillerdroidO->boss           = (Entity *)self;
                    DrillerdroidO->childSlotStart = SceneInfo->entitySlot + 2;
                    DrillerdroidO->field_8C       = 4;
                    self->state                 = DrillerdroidO_State_SetupArena;
                    self->stateDraw             = DrillerdroidO_StateDraw_Boss;
                    break;
                case DRILLERDROIDO_FIREBALLEMITTER:
                    self->active        = ACTIVE_BOUNDS;
                    self->drawFX        = FX_FLIP;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(DrillerdroidO->ticFrames, 0, &self->animator1, true, 0);
                    self->drawOrder = Zone->drawOrderHigh;
                    self->state     = DrillerdroidO_State_FireballEmitter;
                    self->stateDraw = DrillerdroidO_StateDraw_FireballEmitter;
                    break;
                case DRILLERDROIDO_TARGET:
                case DRILLERDROIDO_UNKNOWN3:
                case DRILLERDROIDO_UNKNOWN4:
                    self->active        = ACTIVE_NORMAL;
                    self->drawFX        = FX_FLIP;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 2, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 3, &self->animator2, true, 0);
                    self->drawOrder = Zone->drawOrderHigh;
                    self->state     = DrillerdroidO_State_Target;
                    self->stateDraw = DrillerdroidO_StateDraw_Target;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(self);
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
    DrillerdroidO->emitFireballs      = false;
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

    if (self->invincibilityTimer)
        self->invincibilityTimer--;

    foreach_active(Player, player)
    {
        int playerX = player->position.y;
        int playerY = player->position.x;
        int velX    = player->velocity.x;
        int velY    = player->velocity.y;
        int side    = Player_CheckCollisionBox(player, self, &DrillerdroidO->hitbox1);

        switch (side) {
            case C_TOP:
                Player_CheckHit(player, self);
                if (self->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;
            case C_BOTTOM:
                if (self->rotation) {
                    if (!self->invincibilityTimer) {
                        player->velocity.x = velX;
                        player->velocity.y = velY;
                        player->position.x = playerY;
                        player->position.y = playerX;
                        if (Player_CheckBossHit(player, self)) {
                            if (!--self->health) {
                                SceneInfo->timeEnabled = false;
                                Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                                self->invincibilityTimer = 60;
                                DrillerdroidO->emitFireballs = false;
                                self->state              = DrillerdroidO_State_Destroyed;
                            }
                            else {
                                RSDK.PlaySfx(DrillerdroidO->sfxHit, false, 255);
                                self->invincibilityTimer = 30;
                            }
                        }
                    }
                }
                else if (self->velocity.y >= 0) {
                    player->collisionFlagV |= 2;
                }
                continue;
        }

        Player_CheckCollisionBox(player, self, &DrillerdroidO->hitbox2);
        Player_CheckCollisionBox(player, self, &DrillerdroidO->hitbox3);

        if (self->state == DrillerdroidO_State_Unknown6) {
            if (Player_CheckCollisionTouch(player, self, &DrillerdroidO->hitbox4)
                || Player_CheckCollisionTouch(player, self, &DrillerdroidO->hitbox5)) {
                Player_CheckHit(player, self);
            }
        }
        else {
            if (self->state == DrillerdroidO_State_Unknown2 && (Player_CheckCollisionBox(player, self, &DrillerdroidO->hitbox4) == C_BOTTOM
                || Player_CheckCollisionBox(player, self, &DrillerdroidO->hitbox5) == C_BOTTOM)) {
                if (self->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                else
                    Player_CheckHit(player, self);
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
            int x = self->position.y + (RSDK.Rand(-19, 20) << 16);
            int y = self->position.x + (RSDK.Rand(-24, 25) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void DrillerdroidO_SpawnDebris(int offset)
{
    RSDK_THIS(DrillerdroidO);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x    = RSDK.Rand(0, 6) << 15;
    debris->velocity.y    = RSDK.Rand(-12, -8) << 15;
    debris->gravity       = 0x3800;
    debris->drawOrder     = Zone->drawOrderLow;
    debris->updateRange.x = 0x400000;
    debris->updateRange.y = 0x400000;

    debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
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

    if (++self->timer >= 2) {
        self->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->screenBoundsR1[0]     = (self->position.x >> 16) + ScreenInfo->centerX;
        Zone->screenBoundsB1[0]     = (self->position.y >> 16) + 96;
        Zone->screenBoundsT1[0]     = Zone->screenBoundsB1[0] - 240;
        self->startY              = self->position.y;
        self->active              = ACTIVE_NORMAL;
        self->position.y          = (ScreenInfo->position.y - 192) << 16;
        self->visible             = true;
        self->state               = DrillerdroidO_State_StartBoss;
    }
}

void DrillerdroidO_State_StartBoss(void)
{
    RSDK_THIS(DrillerdroidO);

    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = ScreenInfo->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (self->position.x >> 16) - ScreenInfo->centerX;
        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        self->health = 6;
        CREATE_ENTITY(DrillerdroidO, intToVoid(DRILLERDROIDO_TARGET), self->position.x, self->startY);
        self->position.x = 0;
        RSDK.PlaySfx(DrillerdroidO->sfxTargeting, false, 255);
        self->state           = DrillerdroidO_State_Unknown1;
        DrillerdroidO->field_84 = 1;
        DrillerdroidO->emitFireballs = true;
    }
}

void DrillerdroidO_State_Unknown2(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    DrillerdroidO_CheckPlayerCollisions();

    if (self->position.y >= (Zone->screenBoundsB1[0] - 112) << 16) {
        self->position.y = (Zone->screenBoundsB1[0] - 112) << 16;
        RSDK.PlaySfx(DrillerdroidO->sfxImpact, false, 255);

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        if (!camera->shakePos.y)
            camera->shakePos.y = 4;

        self->velocity.y >>= 1;
        self->startY = self->position.y + 0x100000;
        if (!DrillerdroidO->field_84) {
            for (int i = 0; i < 4; ++i) {
                DrillerdroidO_SpawnDebris(-0x300000);
                DrillerdroidO_SpawnDebris(0x300000);
                DrillerdroidO_SpawnDebris(-0x190000);
                DrillerdroidO_SpawnDebris(0x190000);
            }
        }

        EntityLRZRockPile *pile = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, LRZRockPile);
        pile->timer             = 1;
        pile->flag              = true;
        self->timer           = 60;
        self->angle           = 0;
        self->state           = DrillerdroidO_State_Unknown3;
    }
}

void DrillerdroidO_State_Unknown3(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.y += self->velocity.y;
    self->velocity.y -= 0xE000;
    DrillerdroidO->field_54[0] = self->startY - self->position.y;
    DrillerdroidO->field_54[1] = self->startY - self->position.y;
    DrillerdroidO_CheckPlayerCollisions();

    if (self->velocity.y < 0) {
        if (self->position.y < self->startY)
            self->state = DrillerdroidO_State_Unknown4;
    }
}

void DrillerdroidO_State_Unknown4(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x14000;
    DrillerdroidO->field_54[0] = self->startY - self->position.y;
    DrillerdroidO->field_54[1] = self->startY - self->position.y;
    DrillerdroidO_CheckPlayerCollisions();

    if (self->velocity.y > 0) {
        if (self->position.y > self->startY) {
            self->position.y         = self->startY;
            DrillerdroidO->field_54[0] = 0;
            DrillerdroidO->field_54[1] = 0;
            self->state              = DrillerdroidO_State_Unknown5;
        }
    }
}

void DrillerdroidO_State_Unknown5(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_CheckPlayerCollisions();
    if (--self->timer <= 0) {
        if (DrillerdroidO->field_84 == 1) {
            EntityLavaGeyser *geyser = CREATE_ENTITY(LavaGeyser, NULL, self->position.x, self->position.y + 0x580000);
            geyser->force            = 56 << 12;
            geyser->duration         = 120;
            geyser->drawOrder        = Zone->drawOrderHigh;
            geyser->state            = LavaGeyser_HandleSetup;
        }
        self->timer = 240;
        self->state = DrillerdroidO_State_Unknown6;
    }
}

void DrillerdroidO_State_Unknown6(void)
{
    RSDK_THIS(DrillerdroidO);

    ++DrillerdroidO->shouldPlayDrillSfx;
    RSDK.ProcessAnimation(&self->animator1);

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

    self->position.y ^= 0x10000;
    DrillerdroidO_CheckPlayerCollisions();

    if (DrillerdroidO->field_84 == 1 && self->timer < 180) {
        if (self->alpha < 512) {
            self->alpha += 4;
        }
    }
    if (--self->timer <= 0)
        self->state = DrillerdroidO_State_Unknown7;
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
        self->timer = 60;
        self->state = DrillerdroidO_State_Unknown8;
    }
}

void DrillerdroidO_State_Unknown8(void)
{
    RSDK_THIS(DrillerdroidO);

    if (--self->timer <= 0) {
        if (DrillerdroidO->field_84 == 1) {
            DrillerdroidO->field_84 = 0;
            RSDK.PlaySfx(DrillerdroidO->sfxSizzle, false, 255);
            self->state = DrillerdroidO_State_Unknown16;
        }
        else {
            self->state = DrillerdroidO_State_Unknown9;
        }
    }
    DrillerdroidO_CheckPlayerCollisions();
}

void DrillerdroidO_State_Unknown9(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO->field_54[0] += 0x10000;
    DrillerdroidO->field_54[1] += 0x10000;
    self->position.y -= 0x10000;
    DrillerdroidO_CheckPlayerCollisions();

    if (DrillerdroidO->field_54[0] == 0x100000) {
        self->timer = 30;
        self->state = DrillerdroidO_State_Unknown10;
    }
}

void DrillerdroidO_State_Unknown10(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_CheckPlayerCollisions();
    if (--self->timer <= 0) {
        self->velocity.y = 0x78000;
        RSDK.PlaySfx(DrillerdroidO->sfxJump, false, 255);
        self->state = DrillerdroidO_State_Unknown11;
    }
}

void DrillerdroidO_State_Unknown11(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x1C000;
    DrillerdroidO->field_54[0] = self->startY - self->position.y;
    DrillerdroidO->field_54[1] = self->startY - self->position.y;

    if (self->velocity.y < 0) {
        if (DrillerdroidO->field_54[0] >= 0x100000) {
            DrillerdroidO->field_54[0] = 0x100000;
            DrillerdroidO->field_54[1] = 0x100000;
            self->state              = DrillerdroidO_State_Unknown12;
        }
    }
}

void DrillerdroidO_State_Unknown12(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (self->velocity.y >= 0) {
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
        self->position.x         = 0;

        if (self->health <= DrillerdroidO->field_8C && DrillerdroidO->field_8C) {
            self->timer           = 90;
            DrillerdroidO->emitFireballs = false;
            self->state           = DrillerdroidO_State_Unknown13;
        }
        else {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            CREATE_ENTITY(DrillerdroidO, intToVoid(DRILLERDROIDO_TARGET), player1->position.x, player1->position.y)->target = (Entity *)player1;
            RSDK.PlaySfx(DrillerdroidO->sfxTargeting, false, 255);
            self->state = DrillerdroidO_State_Unknown1;
        }
    }
}

void DrillerdroidO_State_Unknown13(void)
{
    RSDK_THIS(DrillerdroidO);

    if (--self->timer <= 60) {
        ++DrillerdroidO->shouldPlayDrillSfx;
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        if (!camera->shakePos.y)
            camera->shakePos.y = 4;
    }

    if (self->timer <= 0) {
        DrillerdroidO->field_8C -= 2;
        for (int i = 0; i < 4; ++i) {
            EntityLRZRockPile *pile = RSDK_GET_ENTITY(DrillerdroidO->childSlotStart++, LRZRockPile);
            pile->timer             = 1;
            pile->flag              = true;
        }
        self->timer = 30;
        self->state = DrillerdroidO_State_Unknown14;
    }
}

void DrillerdroidO_State_Unknown14(void)
{
    RSDK_THIS(DrillerdroidO);

    if (--self->timer <= 0) {
        self->position.y += 0x1000000;
        Zone->screenBoundsB1[0] += 256;
        self->state = DrillerdroidO_State_Unknown15;
    }
}

void DrillerdroidO_State_Unknown15(void)
{
    RSDK_THIS(DrillerdroidO);

    Zone->screenBoundsT1[0] = ScreenInfo->position.y;
    if (Zone->screenBoundsT1[0] == Zone->screenBoundsB1[0] - 240) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        CREATE_ENTITY(DrillerdroidO, intToVoid(DRILLERDROIDO_TARGET), player1->position.x, player1->position.y)->target = (Entity *)player1;
        RSDK.PlaySfx(DrillerdroidO->sfxTargeting, false, 255);
        DrillerdroidO->emitFireballs = true;
        self->state           = DrillerdroidO_State_Unknown1;
    }
}

void DrillerdroidO_State_Unknown1(void)
{
    RSDK_THIS(DrillerdroidO);

    if (self->position.x) {
        self->timer = 240;
        self->state = DrillerdroidO_State_Unknown2;

        foreach_active(DrillerdroidO, target)
        {
            if (target->type == 1 && self->position.y < target->position.y
                && RSDK.CheckObjectCollisionTouchBox(target, &DrillerdroidO->hitbox9, self, &DrillerdroidO->hitbox6)) {
                self->position.x      = target->position.x;
                DrillerdroidO->field_84 = 1;
                foreach_break;
            }
        }

        foreach_active(DrillerdroidO, drillerdroid)
        {
            if (drillerdroid->type == 1 && self->position.y < drillerdroid->position.y) {
                if (RSDK.CheckObjectCollisionTouchBox(drillerdroid, &DrillerdroidO->hitbox9, self, &DrillerdroidO->hitbox7)) {
                    self->position.x = drillerdroid->position.x + 0x550000;
                    foreach_break;
                }
                if (RSDK.CheckObjectCollisionTouchBox(drillerdroid, &DrillerdroidO->hitbox9, self, &DrillerdroidO->hitbox8)) {
                    self->position.x = drillerdroid->position.x - 0x550000;
                    foreach_break;
                }
            }
        }
    }
}

void DrillerdroidO_State_Unknown16(void)
{
    RSDK_THIS(DrillerdroidO);

    ++self->rotation;
    DrillerdroidO->field_54[0] += 0x4000;
    DrillerdroidO->field_54[1] += 0x4000;
    DrillerdroidO->field_4C[0] += 0x4000;
    DrillerdroidO->field_4C[1] += 0x4000;
    self->position.y -= 0x4000;
    DrillerdroidO_CheckPlayerCollisions();
    if (DrillerdroidO->field_54[0] == 0x100000) {
        self->timer = 90;
        self->state = DrillerdroidO_State_Unknown17;
    }
}

void DrillerdroidO_State_Unknown17(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_CheckPlayerCollisions();

    self->alpha -= 6;
    if (self->alpha < 0)
        self->alpha = 0;
    if (--self->timer <= 0)
        self->state = DrillerdroidO_State_Unknown18;
}

void DrillerdroidO_State_Unknown18(void)
{
    RSDK_THIS(DrillerdroidO);

    self->rotation -= 4;
    DrillerdroidO->field_4C[0] -= 0x10000;
    DrillerdroidO->field_4C[1] -= 0x10000;
    DrillerdroidO_CheckPlayerCollisions();

    if (!DrillerdroidO->field_4C[0]) {
        self->timer = 60;
        self->state = DrillerdroidO_State_Unknown10;
    }
}

void DrillerdroidO_State_Destroyed(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_Explode();
    if (!--self->invincibilityTimer) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 3);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 5);
        debris->velocity.x    = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y    = RSDK.Rand(-8, -4) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        self->velocity.x        = RSDK.Rand(-4, 4) << 15;
        self->velocity.y        = (RSDK.Rand(-8, -4) << 15);
        self->animator1.frameID = 4;
        self->stateDraw         = DrillerdroidO_StateDraw_Unknown2;
        self->state             = DrillerdroidO_State_Finish;
    }
}

void DrillerdroidO_State_Finish(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->visible ^= true;
    DrillerdroidO_Explode();

    if (!RSDK.CheckOnScreen(self, NULL)) {
        Zone->screenBoundsR1[0] += 424;
        destroyEntity(self);
    }
}

void DrillerdroidO_StateDraw_Boss(void)
{
    RSDK_THIS(DrillerdroidO);
    Vector2 drawPos;

    RSDK.SetLimitedFade(0, 1, 2, self->alpha, 32, 41);
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (self->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 32, 0xE0E0E0);
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    }

    self->animator1.frameID = 1;
    drawPos.x -= 0x300000;
    drawPos.y -= DrillerdroidO->field_4C[0];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 2;
    drawPos.y                 = self->position.y + DrillerdroidO->field_54[0];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 1;
    drawPos.x += 0xD0000;
    drawPos.y = self->position.y - DrillerdroidO->field_4C[1];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 2;
    drawPos.y                 = self->position.y + DrillerdroidO->field_54[1];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 1;
    drawPos.x += 0x530000;
    drawPos.y = self->position.y - DrillerdroidO->field_4C[0];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 2;
    drawPos.y                 = self->position.y + DrillerdroidO->field_54[0];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 1;
    drawPos.x -= 0xD0000;
    drawPos.y = self->position.y - DrillerdroidO->field_4C[1];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 2;
    drawPos.y                 = self->position.y + DrillerdroidO->field_54[1];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 0;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->drawFX |= FX_ROTATE;
    drawPos.x                 = self->position.x + 0x10000;
    drawPos.y                 = self->position.y - 0x140000;
    self->animator2.frameID = 0;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    self->animator2.frameID = 1;
    self->rotation          = 0x200 - self->rotation;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    self->rotation = 0x200 - self->rotation;
    self->drawFX   = 0;
    RSDK.CopyPalette(1, 32, 0, 32, 10);
    RSDK.SetPaletteEntry(0, 32, 0x282028);
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void DrillerdroidO_StateDraw_Unknown2(void)
{
    RSDK_THIS(DrillerdroidO);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void DrillerdroidO_State_Target(void)
{
    RSDK_THIS(DrillerdroidO);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->target) {
        self->position.x = self->target->position.x;
        self->position.y = self->target->position.y;
    }
    self->alpha += 32;
    self->field_B0 -= 0x20000;
    if (self->field_B0 <= 0xC0000) {
        self->alpha    = 0;
        self->field_B0 = 0x2C0000;
    }

    self->field_B4 = self->field_B0;
    if (++self->timer == 60)
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 4, &self->animator3, true, 0);

    if (self->timer == 96) {
        self->target                  = NULL;
        DrillerdroidO->boss->position.x = self->position.x;

        if (self->position.x < (Zone->screenBoundsL1[0] + 64) << 16 || self->position.x > (Zone->screenBoundsR1[0] - 64) << 16)
            DrillerdroidO->boss->position.x = (Zone->screenBoundsL1[0] + 64) << 16;
        else if (self->position.x > (Zone->screenBoundsR1[0] - 64) << 16)
            DrillerdroidO->boss->position.x = (Zone->screenBoundsR1[0] - 64) << 16;
    }
    if (self->timer == 128)
        destroyEntity(self);
}

void DrillerdroidO_StateDraw_Target(void)
{
    RSDK_THIS(DrillerdroidO);
    Vector2 drawPos;

    self->animator1.frameID = 0;
    self->inkEffect         = INK_ALPHA;
    self->direction         = FLIP_NONE;
    drawPos.x                 = self->position.x - self->field_B0;
    drawPos.y                 = self->position.y - self->field_B4;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->direction = FLIP_X;
    drawPos.x         = self->position.x + self->field_B0;
    drawPos.y         = self->position.y - self->field_B4;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 1;
    self->direction         = FLIP_NONE;
    drawPos.x                 = self->position.x - self->field_B0;
    drawPos.y                 = self->position.y + self->field_B4;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->direction = FLIP_X;
    drawPos.x         = self->position.x + self->field_B0;
    drawPos.y         = self->position.y + self->field_B4;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->inkEffect = INK_NONE;
    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

void DrillerdroidO_State_FireballEmitter(void)
{
    RSDK_THIS(DrillerdroidO);

    bool32 flag = true;
    foreach_active(DrillerdroidO, drillerdroid)
    {
        if (!drillerdroid->type && RSDK.CheckObjectCollisionTouchBox(drillerdroid, &DrillerdroidO->hitbox6, self, &DrillerdroidO->hitbox9)) {
            flag = false;
            foreach_break;
        }
    }

    if (flag && DrillerdroidO->emitFireballs) {
        if (!(Zone->timer & 0x3F)) {
            EntityLRZFireball *fireball = CREATE_ENTITY(LRZFireball, LRZFireball_StateChild_Type2, self->position.x, self->position.y + 0x940000);
            fireball->angle             = 0xEE;
            fireball->rotation          = 0xEE;
            fireball->groundVel         = -0x300;
            fireball->velocity.x        = fireball->groundVel * RSDK.Sin512(0x12);
            fireball->velocity.y        = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);

            fireball             = CREATE_ENTITY(LRZFireball, LRZFireball_StateChild_Type2, self->position.x, self->position.y + 0x940000);
            fireball->angle      = 0x116;
            fireball->rotation   = 0x116;
            fireball->groundVel  = -0x300;
            fireball->velocity.x = fireball->groundVel * RSDK.Sin512(-0x16);
            fireball->velocity.y = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);
            RSDK.PlaySfx(LRZFireball->sfxFireball, false, 255);
        }
    }
}

void DrillerdroidO_StateDraw_FireballEmitter(void) {}

#if RETRO_INCLUDE_EDITOR
void DrillerdroidO_EditorDraw(void)
{
    RSDK_THIS(DrillerdroidO);
    DrillerdroidO->field_64[0] = 4;
    DrillerdroidO->field_5C[0] = 4;
    DrillerdroidO->field_64[1] = 0;
    DrillerdroidO->field_5C[1] = 0;
    DrillerdroidO->field_54[0] = 0x100000;
    DrillerdroidO->field_54[1] = 0x100000;
    DrillerdroidO->field_4C[0] = 0;
    DrillerdroidO->field_4C[1] = 0;
    DrillerdroidO->field_74[0] = 0;
    DrillerdroidO->field_74[1] = 0;
    DrillerdroidO->field_6C[0] = 0;
    DrillerdroidO->field_6C[1] = 0;

    switch (self->type) {
        case DRILLERDROIDO_MAIN:
            RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 5, &self->animator2, true, 0);
            DrillerdroidO_StateDraw_Boss();

            // Note: its actually centerX offset on left/right but mania is usually always at 424 width anyways so its pretty much the same
            DrawHelpers_DrawArenaBounds(0x00C0F0, 1 | 2 | 4 | 8, -212, -SCREEN_YSIZE, 212, 96);
            break;
        case DRILLERDROIDO_FIREBALLEMITTER:
            RSDK.SetSpriteAnimation(DrillerdroidO->ticFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 3, &self->animator2, true, 0);

            DrawHelpers_DrawRectOutline(0xFFFF00, self->position.x, self->position.y + (0xC0 << 15), 0x40 << 16, 0xC0 << 16);
            break;
        default: break;
    }
}

void DrillerdroidO_EditorLoad(void)
{
    DrillerdroidO->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Drillerdroid.bin", SCOPE_STAGE);
    DrillerdroidO->ticFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(DrillerdroidO, type);
    RSDK_ENUM_VAR("Drillerdroid", DRILLERDROIDO_MAIN);
    RSDK_ENUM_VAR("Fireball Emitter", DRILLERDROIDO_FIREBALLEMITTER);
}
#endif

void DrillerdroidO_Serialize(void) { RSDK_EDITABLE_VAR(DrillerdroidO, VAR_UINT8, type); }
