#include "SonicMania.h"

ObjectUberCaterkiller *UberCaterkiller;

void UberCaterkiller_Update(void)
{
    RSDK_THIS(UberCaterkiller);
    StateMachine_Run(self->state);
}

void UberCaterkiller_LateUpdate(void) {}

void UberCaterkiller_StaticUpdate(void)
{
    foreach_all(UberCaterkiller, boss) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(boss)); }
}

void UberCaterkiller_Draw(void)
{
    RSDK_THIS(UberCaterkiller);
    int clipY2 = ScreenInfo->clipBound_Y2;
    if (self->aniID) {
        if (self->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 160, 0xE0E0E0);

        if (self->bodyScales[0] < 0x200 == (SceneInfo->currentDrawGroup == self->drawOrder)) {
            int clip = ((self->bodyScales[0] - 256) >> 1) + 160;
            if (ScreenInfo->height < clip)
                clip = ScreenInfo->height;
            ScreenInfo->clipBound_Y2 = clip;
            self->scale.x            = self->bodyScales[0];
            self->scale.y            = self->bodyScales[0];
            RSDK.DrawSprite(self->bodyAnimators[0], self->bodyPositions, false);
        }
        RSDK.SetPaletteEntry(0, 160, 0x200000);

        self->direction ^= FLIP_X;
        for (int i = 1; i < UberCaterkiller_SegmentCount; ++i) {
            int clip = ((self->bodyScales[i] - 0x100) >> 1) + 160;
            if (ScreenInfo->height < clip)
                clip = ScreenInfo->height;
            ScreenInfo->clipBound_Y2 = clip;

            self->rotation = (2 * self->bodyAngles[i] - 15) & 0x1E;
            if (self->bodyScales[i] < 0x200 == (SceneInfo->currentDrawGroup == self->drawOrder)) {
                self->scale.x           = self->bodyScales[1];
                self->scale.y           = self->bodyScales[1];
                self->animator2.frameID = self->bodyAngles[i] >> 4;
                RSDK.DrawSprite(self->bodyAnimators[i], &self->bodyPositions[i], false);
            }
        }
        self->direction ^= FLIP_X;
        self->rotation = 0;
    }
    else {
        self->direction ^= FLIP_X;

        for (int i = UberCaterkiller_SegmentCount - 1; i > 0; --i) {
            int clip = ((self->bodyScales[i] - 256) >> 1) + 160;
            if (ScreenInfo->height < clip)
                clip = ScreenInfo->height;
            ScreenInfo->clipBound_Y2 = clip;
            self->rotation           = (2 * self->bodyAngles[i] - 15) & 0x1E;
            if (self->bodyScales[i] < 0x200 == (SceneInfo->currentDrawGroup == self->drawOrder)) {
                self->scale.x           = self->bodyScales[i];
                self->scale.y           = self->bodyScales[i];
                self->animator2.frameID = self->bodyAngles[i] >> 4;
                RSDK.DrawSprite(self->bodyAnimators[i], &self->bodyPositions[i], false);
            }
        }

        self->direction ^= FLIP_X;
        self->rotation = 0;
        if (self->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 160, 0xE0E0E0);

        if (self->bodyScales[0] < 512 == (SceneInfo->currentDrawGroup == self->drawOrder)) {
            int clip = ((self->bodyScales[0] - 256) >> 1) + 160;
            if (ScreenInfo->height < clip)
                clip = ScreenInfo->height;
            ScreenInfo->clipBound_Y2 = clip;
            self->scale.x            = self->bodyScales[0];
            self->scale.y            = self->bodyScales[0];
            RSDK.DrawSprite(self->bodyAnimators[0], &self->bodyPositions[0], false);
        }
        RSDK.SetPaletteEntry(0, 160, 0x200000);
    }
    ScreenInfo->clipBound_Y2 = clipY2;
}

void UberCaterkiller_Create(void *data)
{
    RSDK_THIS(UberCaterkiller);
    if (!SceneInfo->inEditor) {
        self->visible       = false;
        self->drawFX        = FX_SCALE | FX_ROTATE | FX_FLIP;
        self->drawOrder     = Zone->drawOrderLow - 1;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->timer         = 0;
        for (int i = 1; i < UberCaterkiller_SegmentCount; ++i) self->bodyAnimators[i] = &self->animator2;

        self->bodyAnimators[0] = &self->animator1;
        self->health           = 6;
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 2, &self->animator2, true, 0);
        self->state = UberCaterkiller_State_SetupArena;
    }
}

void UberCaterkiller_StageLoad(void)
{
    UberCaterkiller->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/Sandworm.bin", SCOPE_STAGE);
    UberCaterkiller->hitbox.left   = -27;
    UberCaterkiller->hitbox.top    = -27;
    UberCaterkiller->hitbox.right  = 27;
    UberCaterkiller->hitbox.bottom = 27;
    UberCaterkiller->defeated      = false;
    UberCaterkiller->active        = ACTIVE_ALWAYS;
    UberCaterkiller->sfxRocketJet  = RSDK.GetSfx("Stage/RocketJet.wav");
    UberCaterkiller->sfxHit        = RSDK.GetSfx("Stage/BossHit.wav");
    UberCaterkiller->sfxExplosion2 = RSDK.GetSfx("Stage/Explosion2.wav");
    UberCaterkiller->sfxExplosion3 = RSDK.GetSfx("Stage/Explosion3.wav");
    UberCaterkiller->sfxSandSwim   = RSDK.GetSfx("MSZ/SandSwim.wav");
    UberCaterkiller->sfxBumper3    = RSDK.GetSfx("Stage/Bumper3.wav");
    UberCaterkiller->sfxCaterJump  = RSDK.GetSfx("MSZ/CaterJump.wav");
    DEBUGMODE_ADD_OBJ(UberCaterkiller);
}

void UberCaterkiller_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(UberCaterkiller, intToVoid(-0x20000), self->position.x, self->position.y);
}

void UberCaterkiller_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void UberCaterkiller_CheckPlayerCollisions(void)
{
    RSDK_THIS(UberCaterkiller);

    if (self->invincibilityTimer > 0) {
        self->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            uint8 *timer = &self->field_1DC[RSDK.GetEntityID(player)];
            if (*timer) {
                --*timer;
            }
            else {
                self->position.x = self->bodyPositions[0].x;
                self->position.y = self->bodyPositions[0].y;
                int scale          = self->bodyScales[0];

                Hitbox hitbox;
                hitbox.right  = (32 * scale) >> 9;
                hitbox.bottom = hitbox.right;
                hitbox.left   = -hitbox.right;
                hitbox.top    = -hitbox.right;

                if ((scale > 0x1C0 && scale < 0x240) && Player_CheckBadnikTouch(player, self, &hitbox) && Player_CheckBossHit(player, self)) {
                    if (--self->health <= 0) {
                        SceneInfo->timeEnabled = false;
                        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                        RSDK.PlaySfx(UberCaterkiller->sfxExplosion2, false, 255);
                        self->timer = 120;
                        self->state = UberCaterkiller_State_Destroyed;
                    }
                    else {
                        self->invincibilityTimer = 48;
                        RSDK.PlaySfx(UberCaterkiller->sfxHit, false, 255);
                    }
                    foreach_break;
                }
                else {
                    for (int i = 1; i < UberCaterkiller_SegmentCount; ++i) {
                        self->position.x = self->bodyPositions[i].x;
                        self->position.y = self->bodyPositions[i].y;
                        scale              = self->bodyScales[i];

                        if (scale > 0x1C0 && scale < 0x240) {
                            uint8 angle =
                                RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y) - self->bodyAngles[i];
                            hitbox.right  = (28 * scale) >> 9;
                            hitbox.bottom = hitbox.right;
                            hitbox.left   = -hitbox.right;
                            hitbox.top    = -hitbox.right;
                            if (Player_CheckBadnikTouch(player, self, &hitbox)) {
                                if (angle >= 0x80) {
#if RETRO_USE_PLUS
                                    if (!Player_CheckMightyUnspin(0x400, player, true, &player->uncurlTimer))
#endif
                                        Player_CheckHit(player, self);
                                }
                                else if (Player_CheckBossHit(player, self)) {
                                    RSDK.PlaySfx(UberCaterkiller->sfxBumper3, false, 255);
                                    *timer = 30;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    self->position.x = self->bodyPositions[0].x;
    self->position.y = self->bodyPositions[0].y;
}

void UberCaterkiller_Explode(void)
{
    RSDK_THIS(UberCaterkiller);
    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(UberCaterkiller->sfxExplosion2, false, 255);

        if (!(Zone->timer & 7)) {
            int x = RSDK.Rand(-19, 20) << 16;
            int y = RSDK.Rand(-24, 25) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x + self->position.x, y + self->position.y);
            explosion->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void UberCaterkiller_Unknown5(int id)
{
    RSDK_THIS(UberCaterkiller);

    if (self->bodyVelocity[id].y >= 0) {
        if (self->bodyUnknown[id]) {
            int pos = ((self->bodyScales[id] - 256) >> 1) + 160;
            if (pos > 256)
                pos = 256;

            if ((self->bodyPositions[id].y >> 16) - ScreenInfo->position.y > pos) {
                RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);
                EntityExplosion *explosion =
                    CREATE_ENTITY(Explosion, NULL, self->bodyPositions[id].x,
                                  (((self->bodyScales[id] << 15) - 0x7F8001) & 0xFFFF0000) + ((ScreenInfo->position.y + 160) << 16));
                RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
                explosion->drawFX       = FX_SCALE;
                explosion->scale.x      = self->bodyScales[id];
                explosion->scale.y      = self->bodyScales[id];
                explosion->drawOrder    = self->drawOrder;
                explosion->velocity.x   = -0x10000;
                self->bodyUnknown[id] = false;
            }
        }
    }
    else if (!self->bodyUnknown[id]) {
        int pos = ((self->bodyScales[id] - 256) >> 1) + 160;
        if (pos > 256)
            pos = 256;

        if ((self->bodyPositions[id].y >> 16) - ScreenInfo->position.y < pos) {
            RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, NULL, self->bodyPositions[id].x,
                              (((self->bodyScales[id] << 15) - 0x7F8001) & 0xFFFF0000) + ((ScreenInfo->position.y + 160) << 16));
            RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
            explosion->drawFX       = FX_SCALE;
            explosion->scale.x      = self->bodyScales[id];
            explosion->scale.y      = self->bodyScales[id];
            explosion->drawOrder    = self->drawOrder;
            explosion->velocity.x   = -0x10000;
            self->bodyUnknown[id] = true;
        }
    }
}

void UberCaterkiller_SetupUnknown(int x, int y)
{
    RSDK_THIS(UberCaterkiller);

    int delay = 0;
    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
        self->bodyVelocity[i].x  = x;
        self->bodyVelocity[i].y  = y;
        self->bodyTimers[i]      = delay;
        self->bodyPositions[i].x = self->position.x;
        self->bodyPositions[i].y = self->position.y;

        delay += 4;
    }

    self->direction = x < 0;
}

void UberCaterkiller_State_SetupArena(void)
{
    RSDK_THIS(UberCaterkiller);

    if (++self->timer == 60) {
        self->timer   = 0;
        self->visible = true;
        for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) self->bodyPositions[i].x = (ScreenInfo->position.x + 64) << 16;

        self->position.x    = self->bodyPositions[0].x;
        self->position.y    = self->bodyPositions[0].y;
        self->unknownValue2 = 0x100;
        UberCaterkiller_SetupBodySizes();
    }
}

void UberCaterkiller_SetupBodySizes(void)
{
    RSDK_THIS(UberCaterkiller);

    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) self->bodyScales[i] = 0x280;
    self->state = UberCaterkiller_State_Unknown1;
}

void UberCaterkiller_State_Unknown1(void)
{
    RSDK_THIS(UberCaterkiller);
    if (!(Zone->timer & 7))
        RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);

    if (!(Zone->timer & 3)) {
        EntityExplosion *explosion =
            CREATE_ENTITY(Explosion, NULL, self->bodyPositions[0].x,
                          (((self->bodyScales[0] << 15) - 0x7F8001) & 0xFFFF0000) + ((ScreenInfo->position.y + 160) << 16));
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
        explosion->drawFX     = FX_SCALE;
        explosion->scale.x    = self->bodyScales[0];
        explosion->scale.y    = self->bodyScales[0];
        explosion->drawOrder  = Zone->drawOrderLow - 1;
        explosion->velocity.x = -0x10000;
    }

    if (self->bodyScales[0] >= self->unknownValue2) {
        self->bodyScales[0] -= 4;
        if (self->bodyScales[0] < self->unknownValue2)
            self->bodyScales[0] = self->unknownValue2;
    }
    else {
        self->bodyScales[0] += 4;
        if (self->bodyScales[0] > self->unknownValue2)
            self->bodyScales[0] = self->unknownValue2;
    }

    if (self->bodyScales[0] == self->unknownValue2) {
        if (self->bodyStartX) {
            self->state = UberCaterkiller_State_Unknown2;
        }
        else {
            self->timer = 60;
            self->state = UberCaterkiller_State_Unknown5;
        }
    }
}

void UberCaterkiller_State_Unknown2(void)
{
    RSDK_THIS(UberCaterkiller);
    if (!(Zone->timer & 7))
        RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);

    if (!(Zone->timer & 3)) {
        EntityExplosion *explosion =
            CREATE_ENTITY(Explosion, NULL, self->bodyPositions[0].x,
                          (((self->bodyScales[0] << 15) - 0x7F8001) & 0xFFFF0000) + ((ScreenInfo->position.y + 160) << 16));
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
        explosion->drawFX     = FX_SCALE;
        explosion->scale.x    = self->bodyScales[0];
        explosion->scale.y    = self->bodyScales[0];
        explosion->drawOrder  = Zone->drawOrderLow - 1;
        explosion->velocity.x = -0x10000;
    }

    if (self->bodyPositions[0].x >= self->bodyStartX) {
        self->bodyPositions[0].x -= 0x40000;
        if (self->bodyPositions[0].x < self->bodyStartX)
            self->bodyPositions[0].x = self->bodyStartX;
    }
    else {
        self->bodyPositions[0].x += 0x20000;
        if (self->bodyPositions[0].x > self->bodyStartX)
            self->bodyPositions[0].x = self->bodyStartX;
    }

    if (self->bodyPositions[0].x == self->bodyStartX) {
        self->timer = 60;
        self->state = UberCaterkiller_State_Unknown3;
    }
}

void UberCaterkiller_State_Unknown3(void)
{
    RSDK_THIS(UberCaterkiller);
    if (!(Zone->timer & 7))
        RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);
    if (--self->timer <= 0) {
        self->timer = 0;

        int delay = 0;
        for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
            self->bodyVelocity[i].x  = 0;
            self->bodyVelocity[i].y  = -0x80000;
            self->bodyTimers[i]      = delay;
            self->bodyPositions[i].x = self->bodyStartX;
            self->bodyPositions[i].y = (ScreenInfo->position.y + 240) << 16;
            self->bodyScales[i]      = 0x200;

            delay++;
        }

        self->position.x = self->bodyPositions[0].x;
        self->position.y = self->bodyPositions[0].y;
        self->bodyStartX = 0;

        if (self->bodyPositions[UberCaterkiller_SegmentCount - 1].x <= (ScreenInfo->position.x + ScreenInfo->centerX) << 16)
            UberCaterkiller_SetupUnknown(0x40000, -0x60000);
        else
            UberCaterkiller_SetupUnknown(-0x40000, -0x60000);
        RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
        self->state = UberCaterkiller_State_Unknown4;
    }
}

void UberCaterkiller_State_Unknown4(void)
{
    RSDK_THIS(UberCaterkiller);
    RSDK.ProcessAnimation(&self->animator1);

    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
        UberCaterkiller_Unknown5(i);

        if (self->bodyTimers[i] > 0) {
            self->bodyTimers[i]--;
            if (!self->bodyTimers[i])
                RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);
        }
        else {
            self->bodyVelocity[i].y += 0x1C00;
            self->bodyPositions[i].y += self->bodyVelocity[i].y;
            if (self->bodyUnknown[i] == 1)
                self->bodyPositions[i].x += self->bodyVelocity[i].x;
        }
    }

    UberCaterkiller_CheckPlayerCollisions();

    if (self->bodyPositions[UberCaterkiller_SegmentCount - 1].y > (ScreenInfo->position.y + ScreenInfo->height + 64) << 16) {
        self->unknownValue2 = 0x100;
        UberCaterkiller_SetupBodySizes();

        switch (RSDK.Rand(0, 3) - 1) {
            default: break;
            case 0: self->bodyStartX = (ScreenInfo->position.x - 32) << 16; break;
            case 1: self->bodyStartX = (ScreenInfo->position.x + ScreenInfo->width + 32) << 16; break;
        }
    }
}

void UberCaterkiller_State_Unknown5(void)
{
    RSDK_THIS(UberCaterkiller);
    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) self->bodyPositions[i].x -= 0x10000;

    if (--self->timer <= 0) {
        self->timer = 0;

        int delay = 0;
        for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
            self->bodyVelocity[i].x  = 0;
            self->bodyVelocity[i].y  = -0x80000;
            self->bodyTimers[i]      = delay;
            self->bodyPositions[i].y = (ScreenInfo->position.y + 160) << 16;
            self->bodyScales[i]      = self->unknownValue2;

            delay++;
        }

        self->position.x   = self->bodyPositions[0].x;
        self->position.y   = self->bodyPositions[0].y;
        self->unknownValue = RSDK.Rand(0, 2);

        if (self->bodyPositions[UberCaterkiller_SegmentCount - 1].x <= (ScreenInfo->position.x + ScreenInfo->centerX) << 16)
            UberCaterkiller_SetupUnknown(0x18000, -0x80000);
        else
            UberCaterkiller_SetupUnknown(-0x18000, -0x80000);

        RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
        self->state = UberCaterkiller_State_Unknown6;
    }
}

void UberCaterkiller_State_Unknown6(void)
{
    RSDK_THIS(UberCaterkiller);
    RSDK.ProcessAnimation(&self->animator1);

    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
        if (self->bodyTimers[i] > 0) {
            self->bodyTimers[i]--;
        }
        else {
            self->bodyVelocity[i].y += 0x3800;

            self->bodyPositions[i].x += self->bodyVelocity[i].x;
            self->bodyPositions[i].y += self->bodyVelocity[i].y;
            if (self->bodyScales[i] < 0x400)
                self->bodyScales[i] += 8;
        }

        UberCaterkiller_Unknown5(i);
    }

    UberCaterkiller_CheckPlayerCollisions();

    if (self->bodyPositions[UberCaterkiller_SegmentCount - 1].y > (ScreenInfo->position.y + ScreenInfo->height + 64) << 16) {
        RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);
        EntityExplosion *explosion =
            CREATE_ENTITY(Explosion, NULL, self->bodyPositions[0].x,
                          (((self->bodyScales[0] << 15) - 0x7F8001) & 0xFFFF0000) + ((ScreenInfo->position.y + 160) << 16));
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
        explosion->drawFX    = FX_SCALE;
        explosion->scale.x   = 0x80;
        explosion->scale.y   = 0x80;
        explosion->drawOrder = self->drawOrder + 1;
        if (self->unknownValue) {
            self->position.y = (ScreenInfo->position.y + ScreenInfo->height + 64) << 16;
            if (self->bodyPositions[UberCaterkiller_SegmentCount - 1].x <= (ScreenInfo->position.x + ScreenInfo->centerX) << 16)
                UberCaterkiller_SetupUnknown(0x18000, -0xB8000);
            else
                UberCaterkiller_SetupUnknown(-0x18000, -0xB8000);

            for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) self->bodyScales[i] = 0x260;

            self->state = UberCaterkiller_State_Unknown7;
            RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
            self->aniID ^= 1;
        }
        else {
            UberCaterkiller_SetupBodySizes();
        }
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, self->aniID, &self->animator1, true, 0);
    }
}

void UberCaterkiller_State_Unknown7(void)
{
    RSDK_THIS(UberCaterkiller);
    RSDK.ProcessAnimation(&self->animator1);

    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
        UberCaterkiller_Unknown5(i);

        if (self->bodyTimers[i] > 0) {
            self->bodyTimers[i]--;
        }
        else {
            self->bodyVelocity[i].y += 0x3800;
            self->bodyPositions[i].y += self->bodyVelocity[i].y;
            if (self->bodyUnknown[i] == 1) {
                self->bodyPositions[i].x += self->bodyVelocity[i].x;

                if (self->aniID) {
                    if (self->bodyScales[i] > 0x180)
                        self->bodyScales[i] -= 3;
                }
                else {
                    if (self->bodyScales[i] < 0x280)
                        self->bodyScales[i] += 3;
                }
            }
        }
    }

    UberCaterkiller_CheckPlayerCollisions();
    if (self->bodyPositions[UberCaterkiller_SegmentCount - 1].y > (ScreenInfo->height + ScreenInfo->position.y + 64) << 16) {
        if (!self->aniID)
            --self->unknownValue;
        if (self->unknownValue) {
            self->position.y = (ScreenInfo->height + ScreenInfo->position.y + 64) << 16;
            if (self->bodyPositions[UberCaterkiller_SegmentCount - 1].x <= (ScreenInfo->position.x + ScreenInfo->centerX) << 16)
                UberCaterkiller_SetupUnknown(0x18000, -0xB8000);
            else
                UberCaterkiller_SetupUnknown(-0x18000, -0xB8000);
            self->aniID ^= 1;
            RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
        }
        else {
            self->unknownValue2 = 0x100;
            UberCaterkiller_SetupBodySizes();

            switch (RSDK.Rand(0, 3) - 1) {
                default: break;
                case 0: self->bodyStartX = (ScreenInfo->position.x - 32) << 16; break;
                case 1: self->bodyStartX = (ScreenInfo->position.x + ScreenInfo->width + 32) << 16; break;
            }
        }
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, self->aniID, &self->animator1, true, 0);
    }
}

void UberCaterkiller_State_Destroyed(void)
{
    RSDK_THIS(UberCaterkiller);
    UberCaterkiller_Explode();
    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) self->bodyPositions[i].x -= 0x10000;

    self->position.x -= 0x10000;
    if (--self->timer <= 0) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 32;
        RSDK.PlaySfx(UberCaterkiller->sfxExplosion3, false, 255);

        for (int i = 1; i < UberCaterkiller_SegmentCount; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->bodyPositions[i].x, self->bodyPositions[i].y);
            RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, self->bodyAnimators[i]->animationID, &debris->animator, true,
                                    self->bodyAnimators[i]->frameID);
            debris->velocity.x = 4 * RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = 4 * RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x4800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX |= FX_SCALE;
            debris->updateRange.x      = 0x400000;
            debris->updateRange.y      = 0x400000;
            debris->scale.x            = self->bodyScales[i];
            debris->scale.y            = self->bodyScales[i];
            debris->scaleInc.y         = RSDK.Rand(-4, 5);
            debris->scaleInc.x         = debris->scaleInc.y;
            self->bodyPositions[i].x = -0x800000;
            self->bodyPositions[i].y = -0x800000;
        }

        self->velocity.y = -0x40000;
        self->state      = UberCaterkiller_State_Finish;
    }
}

void UberCaterkiller_State_Finish(void)
{
    RSDK_THIS(UberCaterkiller);
    UberCaterkiller_Explode();
    self->position.y += self->velocity.y;
    self->bodyPositions[0].y = self->position.y;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        UberCaterkiller->defeated = true;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void UberCaterkiller_EditorDraw(void)
{
    RSDK_THIS(UberCaterkiller);
    RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 0, &self->animator1, true, 0);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void UberCaterkiller_EditorLoad(void) { UberCaterkiller->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Sandworm.bin", SCOPE_STAGE); }
#endif

void UberCaterkiller_Serialize(void) {}
