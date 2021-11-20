#include "SonicMania.h"

ObjectDrillerdroid *Drillerdroid;

void Drillerdroid_Update(void)
{
    RSDK_THIS(Drillerdroid);
    StateMachine_Run(self->state);
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
        RSDK.StopSfx(Drillerdroid->sfxDrill);
        Drillerdroid->playingDrillSfx = false;
    }
}

void Drillerdroid_Draw(void)
{
    RSDK_THIS(Drillerdroid);
    StateMachine_Run(self->stateDraw);
}

void Drillerdroid_Create(void *data)
{
    RSDK_THIS(Drillerdroid);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->visible = true;
            if (data)
                self->type = voidToInt(data);
            if (self->type) {
                if (self->type == DRILLERDROID_TARGET || self->type == DRILLERDROID_UNKNOWN) {
                    self->active        = ACTIVE_NORMAL;
                    self->drawFX        = FX_FLIP;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 2, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 3, &self->animator2, true, 0);
                    self->drawOrder = Zone->drawOrderHigh;
                    self->state     = Drillerdroid_State1_Unknown;
                    self->stateDraw = Drillerdroid_StateDraw1_Unknown;
                }
            }
            else {
                self->active        = ACTIVE_BOUNDS;
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &self->animator2, true, 0);
                self->drawOrder  = Zone->drawOrderHigh;
                Drillerdroid->boss = (Entity *)self;
                self->state      = Drillerdroid_State_SetupArena;
                self->stateDraw  = Drillerdroid_StateDraw_Unknown1;
            }
        }
        else {
            destroyEntity(self);
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
    Drillerdroid->sfxHit             = RSDK.GetSfx("Stage/BossHit.wav");
    Drillerdroid->sfxExplosion       = RSDK.GetSfx("Stage/Explosion2.wav");
    Drillerdroid->sfxDrill           = RSDK.GetSfx("LRZ/Drill.wav");
    Drillerdroid->sfxImpact          = RSDK.GetSfx("Stage/Impact4.wav");
    Drillerdroid->sfxJump            = RSDK.GetSfx("LRZ/DrillJump.wav");
    Drillerdroid->sfxTargeting       = RSDK.GetSfx("Stage/Targeting1.wav");
    Drillerdroid->sfxSizzle          = RSDK.GetSfx("LRZ/Sizzle.wav");
    Drillerdroid->sfxDrop            = RSDK.GetSfx("Stage/Drop.wav");
    Drillerdroid->sfxFail            = RSDK.GetSfx("Stage/Fail.wav");
    Drillerdroid->sfxClang           = RSDK.GetSfx("Stage/Clang2.wav");
}

void Drillerdroid_Hit(void)
{
    RSDK_THIS(Drillerdroid);

    if (--self->health > 0) {
        RSDK.PlaySfx(Drillerdroid->sfxHit, false, 255);
        self->invincibilityTimer = 45;
    }
    else {
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        self->invincibilityTimer = 60;
        self->state              = Drillerdroid_State_Destroyed;
    }
}

void Drillerdroid_CheckPlayerCollisions(void)
{
    RSDK_THIS(Drillerdroid);

    if (self->invincibilityTimer) {
        self->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            if (!self->invincibilityTimer) {
                int playerID = RSDK.GetEntityID(player);
                if (Drillerdroid->playerTimers[playerID]) {
                    Drillerdroid->playerTimers[playerID]--;
                }
                else if (Player_CheckBadnikTouch(player, self, &Drillerdroid->hitbox1)) {
                    if (player->position.y < self->position.y) {
                        Player_CheckHit(player, self);
                    }
                    else if (self->alpha > 0x80 && player->shield != SHIELD_FIRE && !player->blinkTimer && !player->invincibleTimer) {
                        Player_CheckHit(player, self);
                    }
                    if (Player_CheckBossHit(player, self)) {
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
                    if (Player_CheckBadnikTouch(player, self, &Drillerdroid->hitbox2)
                        || Player_CheckBadnikTouch(player, self, &Drillerdroid->hitbox3)) {
                        if (self->alpha > 0x80 && player->shield != SHIELD_FIRE && !player->blinkTimer && !player->invincibleTimer) {
                            Player_CheckHit(player, self);
                        }
                        else if (Player_CheckBossHit(player, self)) {
                            if (!Drillerdroid->armorHealth) {
                                Drillerdroid_Hit();
                            }
                            else {
                                RSDK.PlaySfx(Drillerdroid->sfxClang, false, 255);
                                Drillerdroid->playerTimers[playerID] = 15;
                            }
                        }
                    }
                    else if (Player_CheckCollisionTouch(player, self, &Drillerdroid->hitbox4)
                             || Player_CheckCollisionTouch(player, self, &Drillerdroid->hitbox5)) {
                        Player_CheckHit(player, self);
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
            int x = self->position.y + (RSDK.Rand(-19, 20) << 16);
            int y = self->position.x + (RSDK.Rand(-24, 25) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void Drillerdroid_SpawnDebris(int offset)
{
    RSDK_THIS(Drillerdroid);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x    = RSDK.Rand(0, 6) << 15;
    debris->velocity.y    = RSDK.Rand(-12, -8) << 15;
    debris->gravity       = 0x3800;
    debris->drawOrder     = Zone->drawOrderLow;
    debris->updateRange.x = 0x400000;
    debris->updateRange.y = 0x400000;

    debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
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

    if (++self->timer >= 2) {
        self->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->screenBoundsR1[0]     = (self->position.x >> 16) + 324;
        Zone->screenBoundsB1[0]     = (self->position.y >> 16) + 96;
        Zone->screenBoundsT1[0]     = Zone->screenBoundsB1[0] - 240;
        self->startY            = self->position.y;
        self->active              = ACTIVE_NORMAL;
        self->position.y          = (ScreenInfo->position.y - 192) << 16;
        self->visible             = true;
        self->state               = Drillerdroid_State_StartBoss;
    }
}

void Drillerdroid_State_StartBoss(void)
{
    RSDK_THIS(Drillerdroid);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = ScreenInfo->position.x;
    if (player1->position.x > self->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (self->position.x >> 16) - 328;
        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        self->health                                                                                            = 6;
        CREATE_ENTITY(Drillerdroid, intToVoid(DRILLERDROID_TARGET), self->position.x, self->startY)->target = (Entity *)player1;
        self->position.x                                                                                        = 0;
        RSDK.PlaySfx(Drillerdroid->sfxTargeting, false, 255);
        self->state = Drillerdroid_State_Unknown17;
    }
}

void Drillerdroid_State_Unknown2(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    Drillerdroid_CheckPlayerCollisions();

    if (self->position.y >= ((Zone->screenBoundsB1[0] - 112) << 16)) {
        self->position.y = ((Zone->screenBoundsB1[0] - 112) << 16);
        RSDK.PlaySfx(Drillerdroid->sfxImpact, false, 255);
        Camera_ShakeScreen(0, 0, 4);
        self->velocity.y >>= 1;
        self->startY = self->position.y + 0x100000;

        if (!Drillerdroid->field_6E) {
            for (int i = 0; i < 4; ++i) {
                Drillerdroid_SpawnDebris(-0x300000);
                Drillerdroid_SpawnDebris(0x300000);
                Drillerdroid_SpawnDebris(-0x190000);
                Drillerdroid_SpawnDebris(0x190000);
            }

            Drillerdroid->field_6D = 2;
        }
        self->timer = 7;
        self->angle = 0;
        self->state = Drillerdroid_State_Unknown3;
    }
}

void Drillerdroid_State_Unknown3(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    Drillerdroid->field_34[0] = self->startY - self->position.y;
    Drillerdroid->field_34[1] = self->startY - self->position.y;

    self->velocity.y -= 0xE000;

    Drillerdroid_CheckPlayerCollisions();
    if (self->velocity.y < 0) {
        if (self->position.y < self->startY)
            self->state = Drillerdroid_State_Unknown4;
    }
}

void Drillerdroid_State_Unknown4(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    Drillerdroid->field_34[0] = self->startY - self->position.y;
    Drillerdroid->field_34[1] = self->startY - self->position.y;

    self->velocity.y += 0x14000;

    Drillerdroid_CheckPlayerCollisions();
    if (self->velocity.y > 0) {
        if (self->position.y > self->startY) {
            self->position.y        = self->startY;
            Drillerdroid->field_34[0] = 0;
            Drillerdroid->field_34[1] = 0;
            self->state             = Drillerdroid_State_Unknown5;
        }
    }
}

void Drillerdroid_State_Unknown5(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_CheckPlayerCollisions();
    if (--self->timer <= 0) {
        if (Drillerdroid->field_6D) {
            self->timer      = 240;
            self->velocity.y = 0x27000;
            RSDK.PlaySfx(Drillerdroid->sfxJump, false, 255);
            self->state = Drillerdroid_State_Unknown6;
        }
        else if (Drillerdroid->field_6E) {
            self->timer          = 180;
            Drillerdroid->field_70 = 2 * (3 * RSDK.Rand(-2, 3));
            self->state          = Drillerdroid_State_Unknown8;
        }
        else {
            self->timer = 60;
            self->state = Drillerdroid_State_Unknown10;
        }
    }
}

void Drillerdroid_State_Unknown6(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    Drillerdroid->field_34[0] = self->startY - self->position.y;
    Drillerdroid->field_34[1] = self->startY - self->position.y;

    self->velocity.y -= 0x8000;
    Drillerdroid_CheckPlayerCollisions();

    if (self->velocity.y < 0) {
        if (Drillerdroid->field_34[0] >= 0x40000) {
            self->timer             = self->velocity.y;
            Drillerdroid->field_34[0] = 0x40000;
            Drillerdroid->field_34[1] = 0x40000;
            self->state             = Drillerdroid_State_Unknown7;
        }
    }
}

void Drillerdroid_State_Unknown7(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    Drillerdroid_CheckPlayerCollisions();

    if (self->velocity.y > 0) {
        if (self->position.y >= (Zone->screenBoundsB1[0] - 96) << 16) {
            self->position.y = (Zone->screenBoundsB1[0] - 96) << 16;
            RSDK.PlaySfx(Drillerdroid->sfxImpact, false, 255);
            Camera_ShakeScreen(0, 0, 4);
            self->velocity.y >>= 1;
            self->startY = self->position.y;
            self->angle    = 0;
            if (--Drillerdroid->field_6D) {
                RSDK.PlaySfx(Drillerdroid->sfxJump, false, 255);
                self->state = Drillerdroid_State_Unknown6;
            }
            else {
                RSDK.PlaySfx(Drillerdroid->sfxDrop, false, 255);
                EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

                EntityBuckwildBall *ball = CREATE_ENTITY(BuckwildBall, NULL, self->position.x, ScreenInfo->position.y << 16);
                ball->startPos.x         = 0;

                int value = 0;
                if (Drillerdroid->field_61) {
                    if (Drillerdroid->field_61 == 4 || player1->position.x < self->position.x) {
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
                self->state = Drillerdroid_State_Unknown3;
            }
        }
    }
}

void Drillerdroid_State_Unknown8(void)
{
    RSDK_THIS(Drillerdroid);

    ++Drillerdroid->shouldPlayDrillSfx;
    RSDK.ProcessAnimation(&self->animator1);

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
        if (self->timer == 120) {
            EntityLavaGeyser *geyser = CREATE_ENTITY(LavaGeyser, NULL, self->position.x, self->position.y + 0x580000);
            geyser->duration         = 60;
            geyser->active           = ACTIVE_NORMAL;
            geyser->force            = 56 << 12;
            geyser->drawOrder        = Zone->drawOrderHigh - 1;
            geyser->state            = LavaGeyser_HandleSetup;
        }
        else if (self->timer < 60) {
            if (self->alpha < 512)
                self->alpha += 4;
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
            int offset = RSDK.Rand(0, ScreenInfo->width >> 5);
            EntityDebris *debris =
                CREATE_ENTITY(Debris, Debris_State_Fall, (32 * offset + ScreenInfo->position.x) << 16, (ScreenInfo->position.y + 24) << 16);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 7, &debris->animator, true, RSDK.Rand(0, 4));
            debris->gravity       = 0x1800;
            debris->drawOrder     = Zone->drawOrderLow;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;
        }
        else {
            int offset = RSDK.Rand(0, ScreenInfo->width / 48);
            EntityStalactite *stalactite =
                CREATE_ENTITY(Stalactite, Stalactite_State_Unknown4, (48 * offset + Drillerdroid->field_70 + ScreenInfo->position.x) << 16,
                              (ScreenInfo->position.y + 24) << 16);
            stalactite->updateRange.x = 0x400000;
            stalactite->updateRange.y = 0x400000;
        }
    }

    self->position.y ^= 0x10000;
    Drillerdroid_CheckPlayerCollisions();
    if (--self->timer <= 0)
        self->state = Drillerdroid_State_Unknown9;
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
        self->timer = 60;
        self->state = Drillerdroid_State_Unknown10;
    }
}

void Drillerdroid_State_Unknown10(void)
{
    RSDK_THIS(Drillerdroid);

    if (--self->timer <= 0) {
        if (Drillerdroid->field_6E == 1) {
            Drillerdroid->field_6E = 0;
            RSDK.PlaySfx(Drillerdroid->sfxSizzle, false, 255);

            int spawnX           = 0;
            EntityDebris *debris = NULL;
            switch (Drillerdroid->armorHealth) {
                default: break;
                case 0:
                    self->state = Drillerdroid_State_Unknown11;
                    Drillerdroid_Hit();
                    break;
                case 1:
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
                    debris->velocity.x    = -0x10000;
                    debris->velocity.y    = -0x30000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
                    debris->direction     = FLIP_X;
                    debris->drawFX        = FX_FLIP;
                    debris->velocity.x    = 0x10000;
                    debris->velocity.y    = -0x30000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    int spawnX = self->position.x - 0x300000;
                    debris     = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x    = -0x30000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    spawnX += 0xD0000;
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x    = 0x20000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    spawnX += 0x530000;
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x    = 0x30000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX - 851968, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x    = -0x20000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;
                    break;
                case 2:
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 4);
                    debris->velocity.x    = -0x10000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x300000;
                    debris->updateRange.y = 0x300000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
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
                self->invincibilityTimer = 45;
                --Drillerdroid->armorHealth;
            }

            self->state = Drillerdroid_State_Unknown11;
        }
        else {
            self->state = Drillerdroid_State_Unknown19;
        }
    }

    Drillerdroid_CheckPlayerCollisions();
}

void Drillerdroid_State_Unknown19(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid->field_34[0] += 0x10000;
    Drillerdroid->field_34[0] += 0x10000;
    self->position.y -= 0x10000;
    Drillerdroid_CheckPlayerCollisions();
    if (Drillerdroid->field_34[0] == 0x100000) {
        self->timer = 15;
        self->state = Drillerdroid_State_Unknown14;
    }
}

void Drillerdroid_State_Unknown14(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_CheckPlayerCollisions();
    if (--self->timer <= 0) {
        self->velocity.y = 0x78000;
        RSDK.PlaySfx(Drillerdroid->sfxJump, false, 255);
        self->state = Drillerdroid_State_Unknown15;
    }
}

void Drillerdroid_State_Unknown15(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x1C000;

    Drillerdroid->field_34[0] = self->startY - self->position.y;
    Drillerdroid->field_34[1] = self->startY - self->position.y;
    Drillerdroid_CheckPlayerCollisions();
    if (self->velocity.y < 0) {
        if (Drillerdroid->field_34[0] >= 0x100000) {
            Drillerdroid->field_34[0] = 0x100000;
            Drillerdroid->field_34[1] = 0x100000;
            self->state             = Drillerdroid_State_Unknown16;
        }
    }
}

void Drillerdroid_State_Unknown16(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (self->velocity.y >= 0) {
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
        self->position.x        = 0;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        CREATE_ENTITY(Drillerdroid, intToVoid(DRILLERDROID_TARGET), player1->position.x, player1->position.y)->target = (Entity *)player1;
        RSDK.PlaySfx(Drillerdroid->sfxTargeting, false, 255);
        self->state = Drillerdroid_State_Unknown17;
    }
}

void Drillerdroid_State_Unknown17(void)
{
    RSDK_THIS(Drillerdroid);

    if (self->position.x) {
        self->timer      = 240;
        self->velocity.y = -0x40000;
        self->state      = Drillerdroid_State_Unknown2;
        self->position.x = (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x + 0x400000) & 0xFF800000;

        bool32 flag = self->position.x < Zone->screenBoundsL1[0] << 16 || self->position.x > Zone->screenBoundsR1[0] << 16;
        int id      = 0;
        if (!flag) {
            Drillerdroid->field_61 = (((self->position.x >> 16) - Zone->screenBoundsL1[0] + 64) >> 7) - 1;
            id                     = Drillerdroid->field_5C[Drillerdroid->field_61];
            flag                   = id == 0xFF;
        }

        if (flag) {
            self->position.x = 0;
            RSDK.PlaySfx(Drillerdroid->sfxFail, false, 255);
            RSDK.StopSfx(Drillerdroid->sfxTargeting);
            self->timer = 60;
            self->state = Drillerdroid_State_Unknown18;
        }
        else if (id == 1) {
            EntityLRZRockPile *rocks                       = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1 + Drillerdroid->field_61, LRZRockPile);
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

    if (--self->timer <= 0) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        CREATE_ENTITY(Drillerdroid, intToVoid(DRILLERDROID_TARGET), player1->position.x, player1->position.y)->target = (Entity *)player1;
        RSDK.PlaySfx(Drillerdroid->sfxTargeting, false, 255);
        self->state = Drillerdroid_State_Unknown17;
    }
}

void Drillerdroid_State_Unknown11(void)
{
    RSDK_THIS(Drillerdroid);

    ++self->rotation;
    Drillerdroid->field_34[0] += 0x4000;
    Drillerdroid->field_34[1] += 0x4000;
    Drillerdroid->field_2C[0] += 0x4000;
    Drillerdroid->field_2C[1] += 0x4000;
    self->position.y -= 0x4000;
    Drillerdroid_CheckPlayerCollisions();
    if (Drillerdroid->field_34[0] == 0x100000) {
        self->timer = 90;
        self->state = Drillerdroid_State_Unknown12;
    }
}

void Drillerdroid_State_Unknown12(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_CheckPlayerCollisions();
    self->alpha -= 8;
    if (self->alpha <= 0) {
        self->alpha = 0;
        self->state = Drillerdroid_State_Unknown13;
    }
}

void Drillerdroid_State_Unknown13(void)
{
    RSDK_THIS(Drillerdroid);

    self->rotation -= 4;
    Drillerdroid->field_2C[0] -= 0x10000;
    Drillerdroid->field_2C[1] -= 0x10000;
    Drillerdroid_CheckPlayerCollisions();

    if (!Drillerdroid->field_2C[0]) {
        self->timer = 30;
        self->state = Drillerdroid_State_Unknown14;
    }
}

void Drillerdroid_State_Destroyed(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_Explode();
    if (!--self->invincibilityTimer) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 3);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 5);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        if (Drillerdroid->armorHealth == 1) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
            debris->direction     = FLIP_X;
            debris->drawFX        = FX_FLIP;
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 14336;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;

            int spawnX = self->position.x - 0x300000;
            debris     = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;

            spawnX += 0xD0000;
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;

            spawnX += 0x530000;
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX - 851968, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;
        }
        self->velocity.x        = RSDK.Rand(-6, 6) << 15;
        self->velocity.y        = RSDK.Rand(-10, -6) << 15;
        self->animator1.frameID = 4;
        self->stateDraw         = Drillerdroid_StateDraw_Unknown2;
        self->state             = Drillerdroid_State_Finish;

        foreach_active(SignPost, signPost) { signPost->position.x = self->position.x; }
    }
}

void Drillerdroid_State_Finish(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->visible ^= true;
    Drillerdroid_Explode();

    if (!RSDK.CheckOnScreen(self, NULL)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->timer = 0;
        self->state = Drillerdroid_State_DropSignPost;
    }
}

void Drillerdroid_State_DropSignPost(void)
{
    RSDK_THIS(Drillerdroid);

    if (++self->timer == 60) {
        foreach_all(SignPost, signPost)
        {
            signPost->active = ACTIVE_NORMAL;
            signPost->state  = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
        }

        LRZ1Setup->fadeTimer = 1;
        destroyEntity(self);
    }
}

void Drillerdroid_StateDraw_Unknown1(void)
{
    RSDK_THIS(Drillerdroid);
    Vector2 drawPos;

    RSDK.SetLimitedFade(0, 1, 2, self->alpha, 32, 41);

    if (self->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 32, 0xE0E0E0);
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    }
    self->animator1.frameID = 1;
    drawPos.x                 = self->position.x - 0x300000;
    drawPos.y                 = self->position.y - Drillerdroid->field_2C[0];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 2;
    drawPos.y                 = self->position.y + Drillerdroid->field_34[0];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 1;
    drawPos.x += 0xD0000;
    drawPos.y = self->position.y - Drillerdroid->field_2C[1];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 2;
    drawPos.y                 = self->position.y + Drillerdroid->field_34[1];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 1;
    drawPos.x += 0x530000;
    drawPos.y = self->position.y - Drillerdroid->field_2C[0];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 2;
    drawPos.y                 = self->position.y + Drillerdroid->field_34[0];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 1;
    drawPos.x -= 0xD0000;
    drawPos.y = self->position.y - Drillerdroid->field_2C[1];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 2;
    drawPos.y                 = self->position.y + Drillerdroid->field_34[1];
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 0;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    switch (Drillerdroid->armorHealth) {
        case 1:
            drawPos                   = self->position;
            self->animator2.frameID = 6;
            RSDK.DrawSprite(&self->animator2, NULL, false);

            self->drawFX |= FX_FLIP;
            self->direction = FLIP_X;
            RSDK.DrawSprite(&self->animator2, NULL, false);

            self->direction         = FLIP_NONE;
            self->drawFX            = 0;
            self->animator2.frameID = 7;
            drawPos.x -= 0x300000;
            drawPos.y -= Drillerdroid->field_2C[0];
            RSDK.DrawSprite(&self->animator2, &drawPos, false);

            drawPos.x += 0xD0000;
            drawPos.y = self->position.y - Drillerdroid->field_2C[1];
            RSDK.DrawSprite(&self->animator2, &drawPos, false);

            drawPos.x += 0x530000;
            drawPos.y = self->position.y - Drillerdroid->field_2C[0];
            RSDK.DrawSprite(&self->animator2, &drawPos, false);

            drawPos.x -= 0xD0000;
            drawPos.y = self->position.y - Drillerdroid->field_2C[1];

            RSDK.DrawSprite(&self->animator2, &drawPos, false);
            break;
        case 2:
            drawPos.x                 = self->position.x + 0x10000;
            drawPos.y                 = self->position.y - 0x140000;
            self->animator2.frameID = 4;
            RSDK.DrawSprite(&self->animator2, &drawPos, false);

            self->animator2.frameID = 5;
            //[Fallthrough]
        case 3:
            if (Drillerdroid->armorHealth == 3) {
                drawPos.x                 = self->position.x + 0x10000;
                drawPos.y                 = self->position.y - 0x140000;
                self->animator2.frameID = 0;
                RSDK.DrawSprite(&self->animator2, &drawPos, false);

                self->animator2.frameID = 1;
            }
            RSDK.DrawSprite(&self->animator2, &drawPos, false);

            drawPos                   = self->position;
            self->animator2.frameID = 2;
            RSDK.DrawSprite(&self->animator2, NULL, false);

            self->drawFX |= FX_FLIP;
            self->direction = FLIP_X;
            RSDK.DrawSprite(&self->animator2, NULL, false);

            self->direction         = FLIP_NONE;
            self->drawFX            = FX_NONE;
            self->animator2.frameID = 3;
            drawPos.x -= 0x300000;
            drawPos.y -= Drillerdroid->field_2C[0];
            RSDK.DrawSprite(&self->animator2, &drawPos, false);

            drawPos.x += 0xD0000;
            drawPos.y = self->position.y - Drillerdroid->field_2C[1];
            RSDK.DrawSprite(&self->animator2, &drawPos, false);

            drawPos.x += 0x530000;
            drawPos.y = self->position.y - Drillerdroid->field_2C[0];
            RSDK.DrawSprite(&self->animator2, &drawPos, false);

            drawPos.x -= 0xD0000;
            drawPos.y = self->position.y - Drillerdroid->field_2C[1];
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
            break;
    }
    RSDK.CopyPalette(1, 32, 0, 32, 10);
    RSDK.SetPaletteEntry(0, 32u, 0x282028);
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void Drillerdroid_StateDraw_Unknown2(void)
{
    RSDK_THIS(Drillerdroid);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Drillerdroid_State1_Unknown(void)
{
    RSDK_THIS(Drillerdroid);

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
        self->target                 = NULL;
        Drillerdroid->boss->position.x = self->position.x;

        if (self->position.x < (Zone->screenBoundsL1[0] + 64) << 16 || self->position.x > (Zone->screenBoundsR1[0] - 64) << 16)
            Drillerdroid->boss->position.x = (Zone->screenBoundsL1[0] + 64) << 16;
        else if (self->position.x > (Zone->screenBoundsR1[0] - 64) << 16)
            Drillerdroid->boss->position.x = (Zone->screenBoundsR1[0] - 64) << 16;
    }
    if (self->timer == 128)
        destroyEntity(self);
}

void Drillerdroid_StateDraw1_Unknown(void)
{
    RSDK_THIS(Drillerdroid);
    Vector2 drawPos;

    int x = ((self->position.x + 0x400000) & 0xFF800000) - 0x400000;
    if (!(Zone->timer & 8)) {
        RSDK.DrawLine(x + 0x10000, self->position.y, x + 0x7F0000, self->position.y, 0xE0E0E0, 192, INK_ADD, false);
        RSDK.DrawLine(x, self->position.y - 0x80000, x, self->position.y + 0x80000, 0xE0E0E0, 160, INK_ADD, false);
        RSDK.DrawLine(x + 0x800000, self->position.y - 0x80000, x + 0x800000, self->position.y + 0x80000, 0xE0E0E0, 160, INK_ADD, false);
    }

    self->animator1.frameID = 0;
    self->inkEffect         = INK_ALPHA;
    self->direction         = FLIP_NONE;
    drawPos.x                 = self->position.x;
    drawPos.y                 = self->position.y;
    drawPos.x                 = self->position.x - self->field_B0;
    drawPos.y -= self->field_B4;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->direction = FLIP_X;
    drawPos.x         = self->position.x;
    drawPos.y         = self->position.y;
    drawPos.x         = self->field_B0 + self->position.x;
    drawPos.y -= self->field_B4;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 1;
    self->direction         = FLIP_NONE;
    drawPos.x                 = self->position.x;
    drawPos.y                 = self->position.y;
    drawPos.x                 = self->position.x - self->field_B0;
    drawPos.y += self->field_B4;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->direction = FLIP_X;
    drawPos.x         = self->position.x;
    drawPos.y         = self->position.y;
    drawPos.x         = self->field_B0 + self->position.x;
    drawPos.y += self->field_B4;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->inkEffect = INK_NONE;
    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
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

    if (self->type) {
        if (self->type == DRILLERDROID_TARGET || self->type == DRILLERDROID_UNKNOWN) {
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 2, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 3, &self->animator2, true, 0);
            Drillerdroid_StateDraw1_Unknown();
        }
    }
    else {
        self->drawFX        = FX_NONE;
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &self->animator2, true, 0);
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
