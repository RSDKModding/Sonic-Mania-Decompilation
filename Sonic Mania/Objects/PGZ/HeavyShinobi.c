// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HeavyShinobi Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHeavyShinobi *HeavyShinobi;

void HeavyShinobi_Update(void)
{
    RSDK_THIS(HeavyShinobi);
    StateMachine_Run(self->state);
}

void HeavyShinobi_LateUpdate(void) {}

void HeavyShinobi_StaticUpdate(void) {}

void HeavyShinobi_Draw(void)
{
    RSDK_THIS(HeavyShinobi);
    StateMachine_Run(self->stateDraw);
}

void HeavyShinobi_Create(void *data)
{
    RSDK_THIS(HeavyShinobi);

    self->drawFX = FX_ROTATE | FX_FLIP;
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->type = voidToInt(data);
            switch (self->type) {
                case SHINOBI_MAIN:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = false;
                    self->drawOrder = Zone->drawOrderLow;
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 0, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->animator2, true, 0);
                    self->state         = HeavyShinobi_State_Setup;
                    self->stateDraw     = HeavyShinobi_StateDraw_Unknown1;
                    self->updateRange.y = 0x800000;
                    self->updateRange.x = 0x800000;
                    break;
                case SHINOBI_SLASH:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawOrder = Zone->drawOrderLow;
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 8, &self->animator1, true, 0);
                    self->inkEffect     = INK_ALPHA;
                    self->alpha         = 0x100;
                    self->state         = HeavyShinobi_State1_Unknown1;
                    self->stateDraw     = HeavyShinobi_StateDraw1_Unknown1;
                    self->updateRange.y = 0x800000;
                    self->updateRange.x = 0x800000;
                    break;
                case SHINOBI_SHURIKEN:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawOrder = Zone->drawOrderLow - 1;
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 9, &self->animator1, true, 0);
                    self->updateRange.x = 0x1000000;
                    self->updateRange.y = 0x1000000;
                    self->alpha         = 0xC0;
                    self->state         = HeavyShinobi_State2_Unknown1;
                    self->stateDraw     = HeavyShinobi_StateDraw2_Unknown1;
                    break;
                case SHINOBI_3:
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderLow;
                    self->drawFX        = FX_ROTATE | FX_FLIP;
                    self->state         = HeavyShinobi_State3_Unknown1;
                    self->stateDraw     = HeavyShinobi_StateDraw3_Unknown1;
                    self->updateRange.y = 0x800000;
                    self->updateRange.x = 0x800000;
                    break;
                case SHINOBI_BOUNDS:
                    self->active      = ACTIVE_NORMAL;
                    // Bug Details: Remember the PGZ2 boss skip?
                    // you guessed it, this is the fix, isPermanent just says "DO NOT OVERWRITE THIS ENTITY"
                    // since all entities spawned via RSDK.CreateEntity use the last 0x100 slots, and it loops around
#if RETRO_GAMEVER != VER_100
                    self->isPermanent = true;
#endif
                    self->visible     = true;
                    self->drawOrder   = Zone->fgLayerHigh - 1;
                    RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 0, &self->animator1, true, 0);
                    self->state         = HeavyShinobi_State4_Unknown1;
                    self->stateDraw     = HeavyShinobi_StateDraw4_Unknown1;
                    self->updateRange.y = 0x800000;
                    self->updateRange.x = 0x800000;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void HeavyShinobi_StageLoad(void)
{
    HeavyShinobi->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Shinobi.bin", SCOPE_STAGE);

    HeavyShinobi->hitbox1.left   = -16;
    HeavyShinobi->hitbox1.top    = -25;
    HeavyShinobi->hitbox1.right  = 16;
    HeavyShinobi->hitbox1.bottom = 14;

    HeavyShinobi->hitbox2.left   = -16;
    HeavyShinobi->hitbox2.top    = -25;
    HeavyShinobi->hitbox2.right  = 16;
    HeavyShinobi->hitbox2.bottom = 25;

    HeavyShinobi->hitbox3.left   = -16;
    HeavyShinobi->hitbox3.top    = -16;
    HeavyShinobi->hitbox3.right  = 16;
    HeavyShinobi->hitbox3.bottom = 16;

    HeavyShinobi->hitbox4.left   = -72;
    HeavyShinobi->hitbox4.top    = -64;
    HeavyShinobi->hitbox4.right  = 0;
    HeavyShinobi->hitbox4.bottom = 25;

    HeavyShinobi->hitbox5.left   = -8;
    HeavyShinobi->hitbox5.top    = -8;
    HeavyShinobi->hitbox5.right  = 8;
    HeavyShinobi->hitbox5.bottom = 8;

    HeavyShinobi->hitbox6.left   = -40;
    HeavyShinobi->hitbox6.top    = -336;
    HeavyShinobi->hitbox6.right  = 40;
    HeavyShinobi->hitbox6.bottom = 336;

    HeavyShinobi->hitbox7.left   = -3;
    HeavyShinobi->hitbox7.top    = -3;
    HeavyShinobi->hitbox7.right  = 3;
    HeavyShinobi->hitbox7.bottom = 3;

    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[0], true, 0);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[1], true, 0);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[2], true, 0);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[3], true, 0);
    HeavyShinobi->activeShurikens    = 0;
    HeavyShinobi->health             = 8;
    HeavyShinobi->invincibilityTimer = 0;
    HeavyShinobi->sfxHit             = RSDK.GetSfx("Stage/BossHit.wav");
    HeavyShinobi->sfxExplosion       = RSDK.GetSfx("Stage/Explosion2.wav");
    HeavyShinobi->sfxDefeat          = RSDK.GetSfx("PSZ/ShinobiDefeat.wav");
    HeavyShinobi->sfxDropIn          = RSDK.GetSfx("PSZ/ShinobiDropIn.wav");
    HeavyShinobi->sfxExplode         = RSDK.GetSfx("PSZ/ShinobiExplode.wav");
    HeavyShinobi->sfxGlitch          = RSDK.GetSfx("PSZ/ShinobiGlitch.wav");
    HeavyShinobi->sfxJump            = RSDK.GetSfx("PSZ/ShinobiJump.wav");
    HeavyShinobi->sfxParry           = RSDK.GetSfx("PSZ/ShinobiParry.wav");
    HeavyShinobi->sfxSlash           = RSDK.GetSfx("PSZ/ShinobiSlash.wav");
    HeavyShinobi->sfxStick           = RSDK.GetSfx("PSZ/ShinobiStick.wav");
    HeavyShinobi->sfxThrow           = RSDK.GetSfx("PSZ/ShinobiThrow.wav");
}

void HeavyShinobi_HandleAfterFX(void)
{
    RSDK_THIS(HeavyShinobi);

    for (int32 i = 15; i > 0; --i) {
        HeavyShinobi->storePos[i] = HeavyShinobi->storePos[i - 1];
    }
    HeavyShinobi->storePos[0].x = self->position.x;
    HeavyShinobi->storePos[0].y = self->position.y;

    for (int32 i = 0; i < 4; ++i) {
        int32 storeAnim = self->animator1.animationID;
        if (HeavyShinobi->animator1[i].animationID != storeAnim) {
            if (HeavyShinobi->storedAnimIDs[i] != storeAnim) {
                HeavyShinobi->storedAnimIDs[i] = self->animator1.animationID;
                HeavyShinobi->storedIDs[i]     = 4 * (i + 1);
            }
        }

        if (HeavyShinobi->storedIDs[i]) {
            --HeavyShinobi->storedIDs[i];
            if (!HeavyShinobi->storedIDs[i]) {
                int32 id = HeavyShinobi->storedAnimIDs[i];
                if (id && (id <= 14 || id > 16)) {
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, HeavyShinobi->storedAnimIDs[i], &HeavyShinobi->animator1[i], false, 0);
                }
                else {
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[i], false, 0);
                    HeavyShinobi->animator1[i].animationID = 255;
                }
            }
        }
    }

    if (HeavyShinobi->invincibilityTimer)
        HeavyShinobi->invincibilityTimer--;
}

void HeavyShinobi_HandleSlash(void *p)
{
    RSDK_THIS(HeavyShinobi);
    EntityPlayer *player = (EntityPlayer *)p;

    RSDK.PlaySfx(HeavyShinobi->sfxSlash, false, 255);
    self->direction = player->position.x >= self->position.x;
    RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 4, &self->animator1, true, 1);
    CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_SLASH), self->position.x, self->position.y)->direction = self->direction;

    int32 delay = 4;
    int32 alpha = 0xC0;
    for (int32 i = 3; i >= 0; --i) {
        EntityHeavyShinobi *child = CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_SLASH), self->position.x, self->position.y);
        child->animator1.frameDuration += delay;
        child->alpha     = alpha;
        child->direction = self->direction;

        delay += 4;
        alpha -= 32;
    }

    self->state = HeavyShinobi_State_Unknown2;
}

void HeavyShinobi_Unknown3(void)
{
    RSDK_THIS(HeavyShinobi);

    RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
    RSDK.PlaySfx(HeavyShinobi->sfxJump, false, 255);
    self->timer = 28;

    if (self->position.x >= (Zone->cameraBoundsL[0] + 144) << 16) {
        if (self->position.x <= (Zone->cameraBoundsR[0] - 144) << 16)
            self->velocity.x = (RSDK.Rand(0, 2) << 18) - 0x20000;
        else
            self->velocity.x = -0x20000;
    }
    else {
        self->velocity.x = 0x20000;
    }

    if (self->velocity.x >= 0)
        self->velocity.x += 0x1000;
    else
        self->velocity.x -= 0x1000;
    self->velocity.y = -0x70000;
    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 3, &self->animator1, true, 0);
    self->state = HeavyShinobi_State_Unknown3;
}

void HeavyShinobi_Explode(void)
{
    RSDK_THIS(HeavyShinobi);

    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(HeavyShinobi->sfxExplosion, false, 255);

        if (!(Zone->timer & 0xF)) {
            int32 x = RSDK.Rand(-19, 20) << 16;
            int32 y = RSDK.Rand(-24, 25) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), x + self->position.x, y + self->position.y);
            explosion->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void HeavyShinobi_State_Setup(void)
{
    RSDK_THIS(HeavyShinobi);

    if (++self->timer >= 2) {
        self->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsR[0]     = ScreenInfo->centerX + 80 + (self->position.x >> 16);
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsB[0]     = (self->position.y >> 16) + 68;
        self->position.y          = (ScreenInfo->position.y - 192) << 16;
        for (int32 i = 0; i < 16; ++i) HeavyShinobi->storePos[i] = self->position;

        self->active = ACTIVE_NORMAL;
        self->state  = HeavyShinobi_State_SetupArena;
    }
}

void HeavyShinobi_State_SetupArena(void)
{
    RSDK_THIS(HeavyShinobi);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]     = ScreenInfo->position.x;
    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x - 0x500000) {
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]     = (self->position.x >> 16) - ScreenInfo->centerX - 80;
        CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_BOUNDS), (Zone->cameraBoundsL[0] + 40) << 16, (Zone->cameraBoundsB[0] - 376) << 16);
        EntityHeavyShinobi *child =
            CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_BOUNDS), (Zone->cameraBoundsR[0] - 40) << 16, (Zone->cameraBoundsB[0] - 376) << 16);
        child->timer      = 1;
        child->position.y = (Zone->cameraBoundsB[0] - 99) << 16;
        child->state      = HeavyShinobi_State4_Unknown2;
        self->state     = HeavyShinobi_State_StartFight;
    }
}

void HeavyShinobi_State_StartFight(void)
{
    RSDK_THIS(HeavyShinobi);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]     = ScreenInfo->position.x;
    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]     = (self->position.x >> 16) - ScreenInfo->centerX - 80;
        Music_TransitionTrack(TRACK_HBHBOSS, 0.0125);
        self->visible = true;
        HeavyShinobi_Unknown3();
        self->velocity.x = 0;
        self->timer      = 0;
    }
}

void HeavyShinobi_State_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);

    HeavyShinobi_HandleAfterFX();
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    for (int32 i = 0; i < 4; ++i) RSDK.ProcessAnimation(&HeavyShinobi->animator1[i]);
    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;

    EntityPlayer *player = Player_GetNearestPlayerX();
    if (abs(player->position.x - self->position.x) < 0x500000 && player->state != Ice_State_FrozenPlayer) {
        HeavyShinobi_HandleSlash(player);
    }
    else {
        if (!HeavyShinobi->activeShurikens && --self->timer <= 0)
            HeavyShinobi_Unknown3();
    }
}

void HeavyShinobi_State_Unknown2(void)
{
    RSDK_THIS(HeavyShinobi);

    HeavyShinobi_HandleAfterFX();
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    for (int32 i = 0; i < 4; ++i) RSDK.ProcessAnimation(&HeavyShinobi->animator1[i]);
    if (self->animator1.frameID == 12)
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->animator2, false, 0);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true))
        self->velocity.y = 0;

    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        EntityPlayer *player = Player_GetNearestPlayerX();
        if (abs(player->position.x - self->position.x) >= 0x500000 || player->state == Ice_State_FrozenPlayer)
            HeavyShinobi_Unknown3();
        else
            HeavyShinobi_HandleSlash(player);
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &HeavyShinobi->hitbox2) && Player_CheckValidState(player)) {
            Ice_FreezePlayer(player);
            player->timer      = 3;
            player->onGround   = false;
            player->velocity.y = -0x38000;
            if (player->position.x < self->position.x)
                player->velocity.x = -0x30000;
            else
                player->velocity.x = 0x30000;
        }
    }
}

void HeavyShinobi_State_Unknown3(void)
{
    RSDK_THIS(HeavyShinobi);

    HeavyShinobi_HandleAfterFX();
    for (int32 i = 0; i < 4; ++i) RSDK.ProcessAnimation(&HeavyShinobi->animator1[i]);
    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (self->velocity.x >= 0) {
        if (self->position.x > (Zone->cameraBoundsR[0] - 104) << 16) {
            self->velocity.x = 0;
            self->position.x = (Zone->cameraBoundsR[0] - 104) << 16;
        }
    }
    else {
        if (self->position.x < (Zone->cameraBoundsL[0] + 104) << 16) {
            self->velocity.x = 0;
            self->position.x = (Zone->cameraBoundsL[0] + 104) << 16;
        }
    }

    if (!--self->timer) {
        if (!HeavyShinobi->activeShurikens) {
            int32 count = 0;
            switch (HeavyShinobi->health) {
                case 1:
                case 2:
                    count = 3;
                    RSDK.PlaySfx(HeavyShinobi->sfxThrow, false, 255);
                    break;
                case 3:
                case 4:
                case 5:
                    count = 2;
                    RSDK.PlaySfx(HeavyShinobi->sfxThrow, false, 255);
                    break;
                case 6:
                case 7:
                    count = 1;
                    RSDK.PlaySfx(HeavyShinobi->sfxThrow, false, 255);
                    break;
                case 8: count = 0;
                default: break;
            }

            for (int32 i = 0; i < count; ++i) {
                EntityHeavyShinobi *child = CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_SHURIKEN), self->position.x, self->position.y);
                child->direction          = FLIP_NONE;
                child->animator1.frameID  = RSDK.Rand(0, 8);
                child->timer              = 180;

                switch (count) {
                    case 1: child->angle = 64; break;
                    case 2:
                        if (!i)
                            child->angle = 96;
                        else
                            child->angle = 32;
                        break;
                    case 3:
                        switch (i) {
                            case 0: child->angle = 96; break;
                            case 1: child->angle = 64; break;
                            case 2: child->angle = 32; break;
                        }
                        break;
                }

                child->angle += RSDK.Rand(-16, 17);
                child->velocity.x = 0x300 * RSDK.Cos256(child->angle);
                child->velocity.y = 0x300 * RSDK.Sin256(child->angle);
                ++HeavyShinobi->activeShurikens;
            }
        }
    }

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true)) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->direction  = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->animator2, false, 0);
        self->timer = RSDK.Rand(30, 91);
        self->state = HeavyShinobi_State_Unknown1;
    }
    else {
        foreach_active(Player, player)
        {
            if (player->state != Ice_State_FrozenPlayer && Player_CheckBadnikTouch(player, self, &HeavyShinobi->hitbox1)
                && Player_CheckBossHit(player, self)) {
                if (player->position.x >= self->position.x) {
                    self->direction  = FLIP_X;
                    self->velocity.x = -0x20000;
                }
                else {
                    self->direction  = FLIP_NONE;
                    self->velocity.x = 0x20000;
                }
                self->velocity.y  = -0x40000;
                int32 angle           = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
                player->velocity.x  = 0x380 * RSDK.Cos256(angle);
                player->velocity.y  = 0x380 * RSDK.Sin256(angle);
                self->timer       = 180;
                self->glitchTimer = 0;
                RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 15, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->animator2, false, 0);
                RSDK.PlaySfx(HeavyShinobi->sfxParry, false, 255);
                self->state = HeavyShinobi_State_Glitched;
            }
        }
    }
}

void HeavyShinobi_State_Glitched(void)
{
    RSDK_THIS(HeavyShinobi);
    if (!(self->glitchTimer % 40))
        RSDK.PlaySfx(HeavyShinobi->sfxGlitch, false, 255);
    ++self->glitchTimer;
    HeavyShinobi_HandleAfterFX();
    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.x >= 0) {
        if (self->position.x > (Zone->cameraBoundsR[0] - 104) << 16) {
            self->velocity.x = 0;
            self->position.x = (Zone->cameraBoundsR[0] - 104) << 16;
        }
    }
    else {
        if (self->position.x < (Zone->cameraBoundsL[0] + 104) << 16) {
            self->velocity.x = 0;
            self->position.x = (Zone->cameraBoundsL[0] + 104) << 16;
        }
    }

    if (self->velocity.y > 0x2800 && RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true)) {
        RSDK.GetEntityByID(SLOT_PLAYER1);
        self->velocity.x = 0;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 16, &self->animator1, false, 0);
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->animator2, false, 0);
        if (--self->timer <= 0) {
            RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->animator2, false, 0);
            self->timer = RSDK.Rand(30, 91);
            self->state = HeavyShinobi_State_Unknown1;
            RSDK.StopSfx(HeavyShinobi->sfxGlitch);
        }
    }

    if (!HeavyShinobi->invincibilityTimer) {
        foreach_active(Player, player)
        {
            if (player->state != Ice_State_FrozenPlayer && Player_CheckBadnikTouch(player, self, &HeavyShinobi->hitbox1)
                && Player_CheckBossHit(player, self)) {
                if (HeavyShinobi->health)
                    HeavyShinobi->health--;

                if (HeavyShinobi->health) {
                    RSDK.PlaySfx(HeavyShinobi->sfxHit, false, 255);
                    self->timer -= 60;
                    HeavyShinobi->invincibilityTimer = 30;
                }
                else {
                    SceneInfo->timeEnabled = false;
                    Player_GiveScore(RSDK.GetEntityByID(SLOT_PLAYER1), 1000);
                    RSDK.PlaySfx(HeavyShinobi->sfxExplosion, false, 255);
                    self->timer = 120;
                    self->state = HeavyShinobi_State_Destroyed;
                }
                foreach_break;
            }
        }
    }
}

void HeavyShinobi_State_Destroyed(void)
{
    RSDK_THIS(HeavyShinobi);
    HeavyShinobi_HandleAfterFX();
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.x >= 0) {
        if (self->position.x > (Zone->cameraBoundsR[0] - 104) << 16) {
            self->velocity.x = 0;
            self->position.x = (Zone->cameraBoundsR[0] - 104) << 16;
        }
    }
    else {
        if (self->position.x < (Zone->cameraBoundsL[0] + 104) << 16) {
            self->velocity.x = 0;
            self->position.x = (Zone->cameraBoundsL[0] + 104) << 16;
        }
    }

    if (self->velocity.y > 0x2800 && RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true)) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->direction  = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 16, &self->animator1, false, 0);
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->animator2, false, 0);
    }

    HeavyShinobi_Explode();
    if (--self->timer <= 0) {
        self->velocity.y = -0x70000;
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 3, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
        RSDK.PlaySfx(HeavyShinobi->sfxDefeat, false, 255);
        self->state = HeavyShinobi_State_Finished;
    }
}

void HeavyShinobi_State_Finished(void)
{
    RSDK_THIS(HeavyShinobi);
    HeavyShinobi_HandleAfterFX();
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    for (int32 i = 0; i < 4; ++i) RSDK.ProcessAnimation(&HeavyShinobi->animator1[i]);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (self->velocity.y > 0)
        self->drawOrder = Zone->drawOrderHigh;

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        Zone->cameraBoundsR[0] += WIDE_SCR_XSIZE;
        HeavyShinobi->health = -1;
        destroyEntity(self);
    }
}

void HeavyShinobi_StateDraw_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->inkEffect = INK_ALPHA;
    self->alpha     = 0x60;

    for (int32 i = 3; i >= 0; --i) {
        RSDK.DrawSprite(&HeavyShinobi->animator1[i], &HeavyShinobi->storePos[i << 2], false);
        self->alpha += 0x20;
    }

    self->inkEffect = INK_NONE;
    if (HeavyShinobi->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    RSDK.DrawSprite(&self->animator1, NULL, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void HeavyShinobi_State1_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID != self->animator1.frameCount - 1) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &HeavyShinobi->hitbox4) && Player_CheckValidState(player)) {
                Ice_FreezePlayer(player);
                player->timer      = 3;
                player->onGround   = false;
                player->velocity.y = -0x38000;
                if (player->position.x < self->position.x)
                    player->velocity.x = -0x30000;
                else
                    player->velocity.x = 0x30000;
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void HeavyShinobi_StateDraw1_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void HeavyShinobi_State2_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);

    if (HeavyShinobi->health) {
        RSDK.ProcessAnimation(&self->animator1);
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        bool32 flag = false;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &HeavyShinobi->hitbox5)) {
                if (player->state == Ice_State_FrozenPlayer) {
                    self->playerPtr        = (Entity *)player;
                    self->playerDistance.x = self->position.x - player->position.x;
                    self->playerDistance.y = self->position.y - player->position.y;

                    flag = true;
                    foreach_break;
                }

#if RETRO_USE_PLUS
                if (Player_CheckMightyShellHit(player, self, -0x400, -0x600)) {
                    self->interaction = false;
                    self->state       = HeavyShinobi_State2_Unknown2;
                    --HeavyShinobi->activeShurikens;
                }
                else {
#endif
                    Player_CheckHit(player, self);
#if RETRO_USE_PLUS
                }
#endif
            }
        }

        if (!flag && RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true))
            flag = true;

        if (!flag) {
            foreach_active(HeavyShinobi, boss)
            {
                if (boss->type == SHINOBI_BOUNDS && RSDK.CheckObjectCollisionBox(boss, &HeavyShinobi->hitbox6, self, &HeavyShinobi->hitbox5, true)) {
                    flag = true;
                }
            }
        }

        if (flag) {
            if (self->animator1.frameID & 1)
                self->animator1.frameID = (self->animator1.frameID + 1) & 7;
            RSDK.PlaySfx(HeavyShinobi->sfxStick, false, 255);
            RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, (self->animator1.frameID >> 1) + 10, &self->animator2, true, 0);
            self->state = HeavyShinobi_State2_Unknown3;
        }

        if (!RSDK.CheckOnScreen(self, &self->updateRange))
            destroyEntity(self);
    }
    else {
        RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
        CREATE_ENTITY(Explosion, intToVoid(2), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh + 2;
        destroyEntity(self);
    }
}

void HeavyShinobi_State2_Unknown2(void)
{
    RSDK_THIS(HeavyShinobi);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->visible ^= 1;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void HeavyShinobi_State2_Unknown3(void)
{
    RSDK_THIS(HeavyShinobi);
    if (!HeavyShinobi->health) {
        RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
        CREATE_ENTITY(Explosion, intToVoid(2), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh + 2;
        destroyEntity(self);
    }
    else {
        RSDK.ProcessAnimation(&self->animator2);
        self->animator2.speed += 2;

        foreach_active(Player, player)
        {
            if (self->playerPtr == (Entity *)player) {
                if (player->state != Ice_State_FrozenPlayer) {
                    RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
                    CREATE_ENTITY(Explosion, intToVoid(2), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh + 2;
                    --HeavyShinobi->activeShurikens;
                    destroyEntity(self);
                    foreach_break;
                }
            }
            else if ((player->state != Ice_State_FrozenPlayer || !self->playerPtr) && Player_CheckBadnikTouch(player, self, &HeavyShinobi->hitbox5)
                     && Player_CheckHit2(player, self, true)) {
                RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
                CREATE_ENTITY(Explosion, intToVoid(2), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh + 2;
                --HeavyShinobi->activeShurikens;
                destroyEntity(self);
                foreach_break;
            }
        }

        EntityPlayer *playerPtr = (EntityPlayer *)self->playerPtr;
        if (playerPtr) {
            self->position.x = playerPtr->position.x + self->playerDistance.x;
            self->position.y = playerPtr->position.y + self->playerDistance.y;
        }

        if (--self->timer <= 0) {
            --HeavyShinobi->activeShurikens;
            RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(2), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh + 2;

            for (int32 i = 0; i < 5; ++i) {
                EntityHeavyShinobi *child = CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_3), self->position.x, self->position.y);
                child->rotation           = (self->animator1.frameID & 0xFFFFFFFE) << 6;
                child->direction          = self->direction;
                switch (i) {
                    case 0:
                        child->angle = (32 * ((self->animator1.frameID & 0xFE) - 2));
                        RSDK.Cos512(child->rotation);
                        child->position.y -= RSDK.Sin512(child->rotation) << 10;
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 17, &child->animator1, true, 0);
                        break;
                    case 1:
                        child->angle = (32 * (self->animator1.frameID & 0xFE) - 120);
                        child->position.x -= 0x380 * RSDK.Cos512(child->rotation);
                        child->position.y -= 0x180 * RSDK.Sin512(child->rotation);
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 18, &child->animator1, true, 0);
                        break;
                    case 2:
                        child->angle = (32 * (self->animator1.frameID & 0xFE) - 8);
                        child->position.x += 0x380 * RSDK.Cos512(child->rotation);
                        child->position.y -= 0x180 * RSDK.Sin512(child->rotation);
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 18, &child->animator1, true, 0);
                        child->direction ^= FLIP_X;
                        break;
                    case 3:
                        child->angle = (32 * ((self->animator1.frameID & 0xFE) + 3));
                        child->position.x -= 0x380 * RSDK.Cos512(child->rotation);
                        child->position.y += 0x380 * RSDK.Sin512(child->rotation);
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 19, &child->animator1, true, 0);
                        break;
                    case 4:
                        child->angle = (32 * ((self->animator1.frameID & 0xFE) + 1));
                        child->position.x += 0x380 * RSDK.Cos512(child->rotation);
                        child->position.y += 0x380 * RSDK.Sin512(child->rotation);
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 19, &child->animator1, true, 0);
                        child->direction ^= FLIP_X;
                        break;
                    default: break;
                }

                child->velocity.x = 0x300 * RSDK.Cos256(child->angle);
                child->velocity.y = 0x300 * RSDK.Sin256(child->angle);
            }

            if (playerPtr) {
                Ice_BreakPlayerBlock((Entity *)playerPtr);
                playerPtr->state = Player_State_Air;
                Player_CheckHit(playerPtr, self);
            }
            destroyEntity(self);
        }
        if (!RSDK.CheckOnScreen(self, &self->updateRange))
            destroyEntity(self);
    }
}

void HeavyShinobi_StateDraw2_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);

    RSDK.DrawSprite(&self->animator1, NULL, false);
    self->inkEffect = INK_ADD;

    RSDK.DrawSprite(&self->animator2, NULL, false);
    self->inkEffect = INK_NONE;
}

void HeavyShinobi_State3_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    if (HeavyShinobi->health) {
        RSDK.ProcessAnimation(&self->animator1);
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &HeavyShinobi->hitbox7) && player->state != Ice_State_FrozenPlayer) {
#if RETRO_USE_PLUS
                if (Player_CheckMightyShellHit(player, self, -0x400, -0x600)) {
                    self->interaction = false;
                    self->state       = HeavyShinobi_State2_Unknown2;
                }
                else {
#endif
                    Player_CheckHit(player, self);
#if RETRO_USE_PLUS
                }
#endif
            }
        }

        if (!RSDK.CheckOnScreen(self, &self->updateRange))
            destroyEntity(self);
    }
    else {
        RSDK.PlaySfx(HeavyShinobi->sfxExplosion, false, 255);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->position.x, self->position.y)->drawOrder = Zone->drawOrderLow;
        destroyEntity(self);
    }
}

void HeavyShinobi_StateDraw3_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void HeavyShinobi_State4_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x4800;

    if (self->position.y >= (Zone->cameraBoundsB[0] - 99) << 16) {
        self->position.y = (Zone->cameraBoundsB[0] - 99) << 16;
        self->state      = HeavyShinobi_State4_Unknown2;
    }

    foreach_active(Player, player) { Player_CheckCollisionBox(player, self, &HeavyShinobi->hitbox6); }
}

void HeavyShinobi_State4_Unknown2(void)
{
    RSDK_THIS(HeavyShinobi);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &HeavyShinobi->hitbox6)) {
            Hitbox *playerHitbox = Player_GetHitbox(player);

            int32 left  = 0;
            int32 right = 0;
            if (self->timer) {
                left  = HeavyShinobi->hitbox6.left;
                right = playerHitbox->right;
            }
            else {
                left  = HeavyShinobi->hitbox6.right;
                right = playerHitbox->left;
            }
            player->position.x = self->position.x + ((left - right) << 16);
        }
    }

    if (HeavyShinobi->health == 0xFF) {
        RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
        for (int32 i = 0; i < 0x80; ++i) {
            int32 x = RSDK.Rand(-64, 65) << 16;
            int32 y = RSDK.Rand(-80, 81) << 16;

            EntityIce *ice = CREATE_ENTITY(Ice, intToVoid(ICE_CHILD_SHARD), x + self->position.x, y + self->position.y);
            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 1, &ice->blockAnimator, true, 0);
            ice->velocity.x               = RSDK.Rand(-6, 8) << 15;
            ice->velocity.y               = RSDK.Rand(-10, 2) << 15;
            ice->direction                = RSDK.Rand(0, 4);
            ice->blockAnimator.speed      = RSDK.Rand(1, 4);
            ice->active                   = ACTIVE_NORMAL;
        }
        destroyEntity(self);
    }
}

void HeavyShinobi_StateDraw4_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    Vector2 drawPos;

    drawPos.x                 = self->position.x;
    drawPos.y                 = self->position.y - 0x880000;
    self->animator1.frameID = 3;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 4;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    drawPos.y += 0x500000;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    drawPos.y += 0x500000;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->direction         = FLIP_Y;
    self->animator1.frameID = 3;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
    self->direction = FLIP_NONE;
}

#if RETRO_INCLUDE_EDITOR
void HeavyShinobi_EditorDraw(void)
{
    RSDK_THIS(HeavyShinobi);
    self->active    = ACTIVE_BOUNDS;
    self->visible   = false;
    self->drawOrder = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->animator2, true, 0);
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    HeavyShinobi_StateDraw_Unknown1();

    if (showGizmos()) {
        int32 boundsL = (self->position.x >> 16) - WIDE_SCR_XCENTER - 80;
        int32 boundsR = (self->position.x >> 16) + WIDE_SCR_XCENTER + 80;
        int32 boundsB = (self->position.y >> 16) + 68;

        DrawHelpers_DrawArenaBounds(0x00C0F0, 1 | 0 | 4 | 8, -WIDE_SCR_XCENTER - 80, -SCREEN_YSIZE, WIDE_SCR_XCENTER + 80, 68);

        Vector2 startPos = self->position;

        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawOrder = Zone->fgLayerHigh - 1;
        RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 0, &self->animator1, true, 0);

        self->position.x = (boundsL + 40) << 16;
        self->position.y = (boundsB - 99) << 16;
        HeavyShinobi_StateDraw4_Unknown1();

        self->position.x = (boundsR - 40) << 16;
        self->position.y = (boundsB - 99) << 16;
        HeavyShinobi_StateDraw4_Unknown1();

        self->position = startPos;
    }
}

void HeavyShinobi_EditorLoad(void)
{
    HeavyShinobi->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Shinobi.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[0], true, 0);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[1], true, 0);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[2], true, 0);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[3], true, 0);
}
#endif

void HeavyShinobi_Serialize(void) {}
