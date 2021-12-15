// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MeterDroid Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMeterDroid *MeterDroid;

void MeterDroid_Update(void)
{
    RSDK_THIS(MeterDroid);
    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;
    StateMachine_Run(self->state);
}

void MeterDroid_LateUpdate(void) {}

void MeterDroid_StaticUpdate(void) {}

void MeterDroid_Draw(void)
{
    RSDK_THIS(MeterDroid);

    if (self->field_6C > 0) {
        RSDK.SetLimitedFade(0, 1, 3, self->field_6C, 32, 47);

        if (self->stateDraw) {
            StateMachine_Run(self->stateDraw);
        }
        else {
            RSDK.DrawSprite(&self->animator1, NULL, false);
        }
        RSDK.CopyPalette(1, 32, 0, 32, 16);
    }
    else if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 32, 0, 32, 16);

        if (self->stateDraw) {
            StateMachine_Run(self->stateDraw);
            RSDK.CopyPalette(1, 32, 0, 32, 16);
        }
        else {
            RSDK.DrawSprite(&self->animator1, NULL, false);
        }
    }
    else {
        if (self->stateDraw) {
            StateMachine_Run(self->stateDraw);
        }
        else {
            RSDK.DrawSprite(&self->animator1, NULL, false);
        }
    }
}

void MeterDroid_Create(void *data)
{
    RSDK_THIS(MeterDroid);
    if (!SceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(self);
        }
        else {
            self->field_70      = self->position;
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            self->health        = 6;
            self->drawOrder     = Zone->drawOrderLow;
            self->state         = MeterDroid_State_Setup;
            RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 8, &self->animator2, true, 0);
        }
    }
}

void MeterDroid_StageLoad(void)
{
    MeterDroid->aniFrames      = RSDK.LoadSpriteAnimation("OOZ/MeterDroid.bin", SCOPE_STAGE);
    MeterDroid->hitbox1.left   = -14;
    MeterDroid->hitbox1.top    = -14;
    MeterDroid->hitbox1.right  = 14;
    MeterDroid->hitbox1.bottom = 14;
    MeterDroid->hitbox2.left   = 14;
    MeterDroid->hitbox2.top    = -12;
    MeterDroid->hitbox2.right  = 32;
    MeterDroid->hitbox2.bottom = 12;
    MeterDroid->hitbox3.left   = -14;
    MeterDroid->hitbox3.top    = -14;
    MeterDroid->hitbox3.right  = 14;
    MeterDroid->hitbox3.bottom = 14;
    MeterDroid->sfxHit         = RSDK.GetSfx("Stage/BossHit.wav");
    MeterDroid->sfxExplosion   = RSDK.GetSfx("Stage/Explosion2.wav");
    MeterDroid->sfxDestroy     = RSDK.GetSfx("Global/Destroy.wav");
    MeterDroid->sfxToss        = RSDK.GetSfx("OOZ/Toss.wav");
    MeterDroid->sfxGrab        = RSDK.GetSfx("Global/Grab.wav");
    MeterDroid->sfxValve       = RSDK.GetSfx("OOZ/Valve.wav");
    MeterDroid->sfxWrench      = RSDK.GetSfx("OOZ/Wrench.wav");
}

void MeterDroid_CheckPlayerCollisions(void)
{
    RSDK_THIS(MeterDroid);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer) {
            if (Player_CheckBadnikTouch(player, self, &MeterDroid->hitbox1) && Player_CheckBossHit(player, self)) {
                MeterDroid_Hit();
            }
            if (!self->invincibilityTimer) {
                if (Player_CheckCollisionTouch(player, self, &MeterDroid->hitbox2)) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(0x400, player, false, &player->uncurlTimer))
#endif
                        Player_CheckHit(player, self);
                }
            }
        }
    }

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    self->direction     = player1->position.x >= self->position.x;
}

void MeterDroid_CheckPlayerCollisions2(void)
{
    RSDK_THIS(MeterDroid);

    foreach_active(Player, player)
    {
        int32 storeX = self->position.x;
        int32 storeY = self->position.y;
        if (!self->invincibilityTimer) {
            if (Player_CheckBadnikTouch(player, self, &MeterDroid->hitbox1) && Player_CheckBossHit(player, self)) {
                MeterDroid_Hit();
            }
            if (!self->invincibilityTimer) {
                if (Player_CheckCollisionTouch(player, self, &MeterDroid->hitbox2)) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(0x400, player, false, &player->uncurlTimer))
#endif
                        Player_CheckHit(player, self);
                }
            }
        }
        self->position.x = self->field_88.x;
        self->position.y = self->field_88.y;
        if (Player_CheckCollisionTouch(player, self, &MeterDroid->hitbox3)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x600, player, false, &player->uncurlTimer))
#endif
                Player_CheckHit(player, self);
        }

        self->position.x = storeX;
        self->position.y = storeY;
    }
}

void MeterDroid_CheckPlayerCollisions3(void)
{
    RSDK_THIS(MeterDroid);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer) {
            if (Player_CheckBadnikTouch(player, self, &MeterDroid->hitbox1) && Player_CheckBossHit(player, self)) {
                MeterDroid_Hit();
            }
            if (!self->invincibilityTimer) {
                if (Player_CheckCollisionTouch(player, self, &MeterDroid->hitbox2)) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(0x400, player, false, &player->uncurlTimer))
#endif
                        Player_CheckHit(player, self);
                }
            }
        }
    }
}

void MeterDroid_Hit(void)
{
    RSDK_THIS(MeterDroid);

    if (--self->health <= 0) {
        self->state = MeterDroid_State_Die;
        self->timer = 0;
        if (self->stateDraw == MeterDroid_StateDraw_Unknown2) {
            int32 x = RSDK.Rand(MeterDroid->hitbox1.left, MeterDroid->hitbox1.right) << 16;
            int32 y = RSDK.Rand(MeterDroid->hitbox1.top, MeterDroid->hitbox1.bottom) << 16;
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), x + self->field_88.x, y + self->field_88.y)->drawOrder = Zone->drawOrderHigh;
            self->stateDraw                                                                                 = MeterDroid_StateDraw_Unknown1;
        }
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(MeterDroid->sfxHit, false, 255);
    }
}

void MeterDroid_Unknown5(void)
{
    EntityValve *valves[5];

    RSDK_THIS(MeterDroid);

    valves[0] = NULL;
    int32 id   = 0;
    foreach_active(Valve, valve) { valves[id++ + 1] = valve; }

    valves[0]         = valves[RSDK.Rand(0, id) + 1];
    self->valvePtr  = (Entity*)valves[0];
    self->direction = valves[0]->direction ^ 1;
    self->field_78  = valves[0]->position.x;
    self->field_7C  = valves[0]->position.y;
    if (valves[0]->direction)
        self->field_78 -= 0x2E0000;
    else
        self->field_78 += 0x2E0000;
    self->field_7C -= 0xC0000;

    self->field_80   = (self->field_78 - self->position.x) >> 11;
    self->field_84   = (self->field_7C - self->position.y) >> 11;
    self->velocity.x = 0;
    self->velocity.y = 0;
}

void MeterDroid_PopPlatforms(void)
{
    int32 delay = 30;
    foreach_active(GasPlatform, platform)
    {
        platform->type          = 2;
        platform->timer = delay;
        delay += 16;
        platform->state = GasPlatform_Unknown4;
    }
}

void MeterDroid_StateDraw_Unknown1(void)
{
    RSDK_THIS(MeterDroid);

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void MeterDroid_StateDraw_Unknown3(void)
{
    RSDK_THIS(MeterDroid);

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->animator4, NULL, false);

    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void MeterDroid_StateDraw_Unknown2(void)
{
    RSDK_THIS(MeterDroid);

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->animator3, &self->field_88, false);
}

void MeterDroid_State_Setup(void)
{
    RSDK_THIS(MeterDroid);
    if (++self->timer >= 8) {
        self->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]     = (self->position.x >> 16) - ScreenInfo->centerX;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsR[0]     = (self->position.x >> 16) + ScreenInfo->centerX;
        Zone->cameraBoundsT[0]     = Zone->cameraBoundsB[0] - ScreenInfo->height - 64;
        MeterDroid->boundsL         = (Zone->cameraBoundsL[0] + 64) << 16;
        MeterDroid->boundsR         = (Zone->cameraBoundsR[0] - 64) << 16;
        MeterDroid->startX          = self->position.x;
        MeterDroid->boundsT         = (Zone->cameraBoundsT[0] + 48) << 16;
        MeterDroid->boundsB         = (Zone->cameraBoundsB[0] - 8) << 16;
        self->position.x += 0x800000;
        self->visible   = true;
        self->stateDraw = MeterDroid_StateDraw_Unknown1;
        self->state     = MeterDroid_State_Unknown1;
    }
}

void MeterDroid_State_Unknown1(void)
{
    RSDK_THIS(MeterDroid);
    self->position.y = BadnikHelpers_Oscillate(self->field_70.y, 4, 10);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);

    if (self->timer) {
        self->timer++;
        if (self->timer == 120) {
            self->timer = 0;
            self->state = MeterDroid_State_Unknown2;
        }
    }
    else {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->position.x > self->position.x - 0x800000) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            ++self->timer;
        }
    }
}

void MeterDroid_State_Unknown2(void)
{
    RSDK_THIS(MeterDroid);
    self->position.y = BadnikHelpers_Oscillate(self->field_70.y, 4, 10);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    if (++self->timer == 90) {
        self->timer = 0;
        self->state = MeterDroid_State_Unknown3;
    }
    MeterDroid_CheckPlayerCollisions();
}

void MeterDroid_State_Unknown3(void)
{
    RSDK_THIS(MeterDroid);
    self->position.y = BadnikHelpers_Oscillate(self->field_70.y, 4, 10);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    if (++self->timer == 60) {
        self->timer    = 0;
        self->field_A0 = 0;
        self->angle    = 64;
        if (self->position.x < MeterDroid->startX) {
            self->direction = FLIP_X;
            self->state     = MeterDroid_State_Unknown5;
        }
        else {
            self->direction = FLIP_NONE;
            self->state     = MeterDroid_State_Unknown4;
        }
    }
    MeterDroid_CheckPlayerCollisions();
}

void MeterDroid_State_Unknown4(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);

    self->position.x += (self->field_A0 * RSDK.Cos256(self->angle)) >> 8;
    self->position.y += (self->field_A0 * RSDK.Sin256(self->angle)) >> 8;
    if (self->angle > 128) {
        self->field_A0 -= 0x1400;
        if (self->position.y < self->field_70.y)
            self->position.y = self->field_70.y;
    }
    else {
        self->field_A0 += 0x1400;
    }

    self->angle++;
    if (self->angle == 192) {
        self->field_70.x = self->position.x;
        self->field_70.y = self->position.y;
        self->angle      = 0;
        self->state      = MeterDroid_State_Unknown6;
    }
    MeterDroid_CheckPlayerCollisions();
}

void MeterDroid_State_Unknown5(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);

    self->position.x += (self->field_A0 * RSDK.Cos256(self->angle)) >> 8;
    self->position.y += (self->field_A0 * RSDK.Sin256(self->angle)) >> 8;
    if (self->angle < 0) {
        self->field_A0 -= 0x1400;
        if (self->position.y < self->field_70.y)
            self->position.y = self->field_70.y;
    }
    else {
        self->field_A0 += 0x1400;
    }

    self->angle--;
    if (self->angle == -64) {
        self->field_70.x = self->position.x;
        self->field_70.y = self->position.y;
        self->angle      = 0;
        self->state      = MeterDroid_State_Unknown6;
    }
    MeterDroid_CheckPlayerCollisions();
}

void MeterDroid_State_Unknown6(void)
{
    RSDK_THIS(MeterDroid);
    self->position.y = BadnikHelpers_Oscillate(self->field_70.y, 4, 10);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    ++self->timer;
    if (self->timer >= 18) {
        if (self->animator1.animationID == 2 && self->animator1.frameID == self->animator1.frameCount - 1)
            RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 0, &self->animator1, true, 0);
    }
    else if (self->timer == 16) {
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 2, &self->animator1, true, 0);
    }
    if (self->timer == 36) {
        self->field_88.x = self->position.x;
        self->field_88.y = self->position.y;
        if (self->direction == FLIP_NONE)
            self->field_88.x -= 0x300000;
        else
            self->field_88.x += 0x300000;
        self->field_88.y -= 0x100000;
        self->field_78 = self->field_88.x;
        self->field_7C = self->field_88.y;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        self->field_90.x    = (player1->position.x + 16 * player1->velocity.x - self->field_88.x) >> 5;
        self->field_90.y    = (player1->position.y + 16 * player1->velocity.y - self->field_88.y) >> 5;
        self->field_98.x    = -(self->field_90.x >> 6);
        self->field_98.y    = -(self->field_90.y >> 6);
        RSDK.PlaySfx(MeterDroid->sfxToss, false, 255);
        RSDK.PlaySfx(MeterDroid->sfxWrench, false, 255);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 9, &self->animator3, true, 0);
        self->stateDraw = MeterDroid_StateDraw_Unknown2;
    }

    if (self->stateDraw == MeterDroid_StateDraw_Unknown2) {
        int32 x = self->field_90.x + self->field_98.x;
        int32 y = self->field_90.y + self->field_98.y;
        self->field_88.x += x;
        self->field_88.y += y;
        self->field_90.x = x;
        self->field_90.y = y;
        if (self->timer == 60)
            self->state = MeterDroid_State_Unknown7;
        MeterDroid_CheckPlayerCollisions2();
    }
    else {
        MeterDroid_CheckPlayerCollisions();
    }
}

void MeterDroid_State_Unknown7(void)
{
    RSDK_THIS(MeterDroid);
    self->position.y = BadnikHelpers_Oscillate(self->field_70.y, 4, 10);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    self->field_90.x += self->field_98.x;
    self->field_90.y += self->field_98.y;
    self->field_88.x += self->field_90.x;
    self->field_88.y += self->field_90.y;

    int32 rx = abs(self->field_78 - self->field_88.x) >> 16;
    int32 ry = abs(self->field_7C - self->field_88.y) >> 16;

    if (rx * rx + ry * ry < 512) {
        RSDK.PlaySfx(MeterDroid->sfxGrab, false, 255);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 3, &self->animator1, true, 0);
        self->stateDraw = MeterDroid_StateDraw_Unknown1;
        self->state     = MeterDroid_State_Unknown8;
    }
    MeterDroid_CheckPlayerCollisions2();
}

void MeterDroid_State_Unknown8(void)
{
    RSDK_THIS(MeterDroid);
    self->position.y = BadnikHelpers_Oscillate(self->field_70.y, 4, 10);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    MeterDroid_CheckPlayerCollisions();
    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &self->animator1, true, 0);
        MeterDroid_Unknown5();
        self->state = MeterDroid_State_Unknown9;
    }
    MeterDroid_CheckPlayerCollisions();
}

void MeterDroid_State_Unknown9(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);

    if (abs(self->velocity.x) < 0x40000) {
        if (abs(self->velocity.x) < 0x40000)
            self->velocity.x += self->field_80;
    }
    self->velocity.y += self->field_84;
    self->position.y += self->velocity.y;
    self->position.x += self->velocity.x;

    int32 rx = abs(self->field_78 - self->position.x) >> 16;
    int32 ry = abs(self->field_7C - self->position.y) >> 16;

    if (rx * rx + ry * ry < 96) {
        self->direction = self->valvePtr->direction;
        self->drawOrder = Zone->drawOrderLow - 1;
        self->state     = MeterDroid_State_Unknown10;
    }
    MeterDroid_CheckPlayerCollisions3();
}

void MeterDroid_State_Unknown10(void)
{
    RSDK_THIS(MeterDroid);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    self->field_6C += 10;
    ++self->timer;

    self->velocity.x = self->velocity.x - (self->velocity.x >> 3);
    self->velocity.y = self->velocity.y - (self->velocity.y >> 3);
    self->position.x += ((self->field_78 - self->position.x) >> 3) + self->velocity.x;
    self->position.y += ((self->field_7C - self->position.y) >> 3) + self->velocity.y;
    if (self->timer == 24) {
        self->position.x = self->field_78;
        self->position.y = self->field_7C;
        self->timer      = 0;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 4, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 6, &self->animator4, true, 0);
        EntityValve *valve = (EntityValve *)self->valvePtr;
        RSDK.SetSpriteAnimation(Valve->aniFrames, 0, &valve->animator1, true, 0);
        RSDK.SetSpriteAnimation(Valve->aniFrames, 2, &valve->animator2, true, 0);
        RSDK.PlaySfx(MeterDroid->sfxValve, false, 255);
        self->state     = MeterDroid_State_Unknown11;
        self->stateDraw = MeterDroid_StateDraw_Unknown3;
    }
    MeterDroid_CheckPlayerCollisions3();
}

void MeterDroid_State_Unknown11(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator2);

    if (++self->timer == 60)
        MeterDroid_PopPlatforms();
    if (self->timer == 90) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 5, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 7, &self->animator4, true, 0);
        EntityValve *valve = (EntityValve *)self->valvePtr;
        RSDK.SetSpriteAnimation(Valve->aniFrames, 1, &valve->animator1, true, 0);
        RSDK.SetSpriteAnimation(Valve->aniFrames, 3, &valve->animator2, true, 0);
        self->state = MeterDroid_State_Unknown12;
    }
}

void MeterDroid_State_Unknown12(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator2);

    if (self->animator1.frameID == self->animator1.frameCount - 1)
        self->state = MeterDroid_State_Unknown13;
}

void MeterDroid_State_Unknown13(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&self->animator2);

    ++self->timer;
    if (self->timer > 30) {
        if (self->field_6C > 0) {
            self->field_6C -= 16;
        }
    }
    if (self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &self->animator1, true, 0);
        self->field_70.x = self->position.x;
        self->field_70.y = self->position.y;
        self->drawOrder  = Zone->drawOrderLow;
        self->state      = MeterDroid_State_Unknown3;
        self->stateDraw  = MeterDroid_StateDraw_Unknown1;
    }
}

void MeterDroid_State_Die(void)
{
    RSDK_THIS(MeterDroid);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MeterDroid->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(2 * (RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS),
                                                       (RSDK.Rand(-208, 208) + ScreenInfo->centerX + ScreenInfo->position.x) << 16,
                                                       (RSDK.Rand(-112, 112) + ScreenInfo->centerY + ScreenInfo->position.y) << 16);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }

    ++self->timer;
    if (self->timer == 60) {
        MeterDroid->debrisSpeeds[2]  = self->direction;
        MeterDroid->debrisSpeeds[6]  = self->direction;
        MeterDroid->debrisSpeeds[10] = self->direction;
        MeterDroid->debrisSpeeds[14] = self->direction;
        if (self->direction == FLIP_X) {
            MeterDroid->debrisSpeeds[3]  = -MeterDroid->debrisSpeeds[3];
            MeterDroid->debrisSpeeds[7]  = -MeterDroid->debrisSpeeds[7];
            MeterDroid->debrisSpeeds[11] = -MeterDroid->debrisSpeeds[11];
            MeterDroid->debrisSpeeds[15] = -MeterDroid->debrisSpeeds[15];
        }
        Debris_FallFlickerAnimSetup(MeterDroid->aniFrames, MeterDroid->debrisSpeeds, 10);
        self->visible = false;
    }
    else if (self->timer == 90) {
        self->timer   = 0;
        self->visible = false;
        Music_FadeOut(0.025);
        CREATE_ENTITY(OOZFlames, NULL, self->position.x, self->position.y);
        self->state = MeterDroid_State_FinishAct;
    }
}

void MeterDroid_State_FinishAct(void)
{
    RSDK_THIS(MeterDroid);
    if (!(Zone->timer % 3)) {
        if (self->timer < 180) {
            if (self->timer <= 60)
                RSDK.PlaySfx(MeterDroid->sfxExplosion, Zone->timer % 3, 255);
            else
                RSDK.PlaySfx(MeterDroid->sfxDestroy, Zone->timer % 3, 255);
        }

        if (Zone->timer & 4) {
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(2 * (RSDK.Rand(0, 256) > 192) + 1),
                                                       (RSDK.Rand(-208, 208) + ScreenInfo->centerX + ScreenInfo->position.x) << 16,
                                                       (RSDK.Rand(-112, 112) + ScreenInfo->centerY + ScreenInfo->position.y) << 16);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }
    if (++self->timer == 180) {
        for (int32 p = 0; p < Player->playerCount; ++p) {
            StarPost->postIDs[p] = 0;
        }

        SaveGame_SavePlayerState();
        globals->enableIntro = true;
        ++SceneInfo->listPos;
        if (!RSDK.CheckValidScene())
            RSDK.SetScene("Presentation", "Title Screen");
        Zone_StartFadeOut(10, 0xF0F0F0);
    }
    if (self->timer == 240)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void MeterDroid_EditorDraw(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 8, &self->animator2, false, 0);

    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void MeterDroid_EditorLoad(void) { MeterDroid->aniFrames = RSDK.LoadSpriteAnimation("OOZ/MeterDroid.bin", SCOPE_STAGE); }
#endif

void MeterDroid_Serialize(void) {}
