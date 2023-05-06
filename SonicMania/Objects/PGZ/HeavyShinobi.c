// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HeavyShinobi Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
            self->type = VOID_TO_INT(data);

            switch (self->type) {
                case SHINOBI_MAIN:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = false;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 0, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->fxAnimator, true, 0);

                    self->state         = HeavyShinobi_State_Init;
                    self->stateDraw     = HeavyShinobi_Draw_Shinobi;
                    self->updateRange.y = 0x800000;
                    self->updateRange.x = 0x800000;
                    break;
                case SHINOBI_SLASH:

                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 8, &self->mainAnimator, true, 0);

                    self->inkEffect     = INK_ALPHA;
                    self->alpha         = 0x100;
                    self->state         = HeavyShinobi_StateSlash_Active;
                    self->stateDraw     = HeavyShinobi_Draw_Slash;
                    self->updateRange.y = 0x800000;
                    self->updateRange.x = 0x800000;
                    break;
                case SHINOBI_ASTERON:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0] - 1;

                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 9, &self->mainAnimator, true, 0);
                    self->updateRange.x = 0x1000000;
                    self->updateRange.y = 0x1000000;
                    self->alpha         = 0xC0;
                    self->state         = HeavyShinobi_StateAsteron_Thrown;
                    self->stateDraw     = HeavyShinobi_Draw_Asteron;
                    break;
                case SHINOBI_ASTERONSPIKE:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    self->drawFX        = FX_ROTATE | FX_FLIP;
                    self->state         = HeavyShinobi_State_AsteronSpike;
                    self->stateDraw     = HeavyShinobi_Draw_AsteronSpike;
                    self->updateRange.y = 0x800000;
                    self->updateRange.x = 0x800000;
                    break;
                case SHINOBI_BOUNDS:
                    self->active = ACTIVE_NORMAL;

                    // Bug Details: Remember the PGZ2 boss skip?
                    // you guessed it, this is the fix, isPermanent just says "DO NOT OVERWRITE THIS ENTITY"
                    // since all entities spawned via RSDK.CreateEntity use the last 0x100 slots, and it loops around
#if GAME_VERSION != VER_100
                    self->isPermanent = true;
#endif

                    self->visible   = true;
                    self->drawGroup = Zone->fgDrawGroup[1] - 1;
                    RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 0, &self->mainAnimator, true, 0);

                    self->state         = HeavyShinobi_StateBounds_WaitForPlayer;
                    self->stateDraw     = HeavyShinobi_Draw_Bounds;
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

    HeavyShinobi->hitboxShinobi.left   = -16;
    HeavyShinobi->hitboxShinobi.top    = -25;
    HeavyShinobi->hitboxShinobi.right  = 16;
    HeavyShinobi->hitboxShinobi.bottom = 14;

    HeavyShinobi->hitboxSlashRange.left   = -16;
    HeavyShinobi->hitboxSlashRange.top    = -25;
    HeavyShinobi->hitboxSlashRange.right  = 16;
    HeavyShinobi->hitboxSlashRange.bottom = 25;

    HeavyShinobi->hitboxUnused.left   = -16;
    HeavyShinobi->hitboxUnused.top    = -16;
    HeavyShinobi->hitboxUnused.right  = 16;
    HeavyShinobi->hitboxUnused.bottom = 16;

    HeavyShinobi->hitboxSlash.left   = -72;
    HeavyShinobi->hitboxSlash.top    = -64;
    HeavyShinobi->hitboxSlash.right  = 0;
    HeavyShinobi->hitboxSlash.bottom = 25;

    HeavyShinobi->hitboxAsteron.left   = -8;
    HeavyShinobi->hitboxAsteron.top    = -8;
    HeavyShinobi->hitboxAsteron.right  = 8;
    HeavyShinobi->hitboxAsteron.bottom = 8;

    HeavyShinobi->hitboxBounds.left   = -40;
    HeavyShinobi->hitboxBounds.top    = -336;
    HeavyShinobi->hitboxBounds.right  = 40;
    HeavyShinobi->hitboxBounds.bottom = 336;

    HeavyShinobi->hitboxAsteronSpike.left   = -3;
    HeavyShinobi->hitboxAsteronSpike.top    = -3;
    HeavyShinobi->hitboxAsteronSpike.right  = 3;
    HeavyShinobi->hitboxAsteronSpike.bottom = 3;

    RSDK.SetSpriteAnimation(-1, 0, &HeavyShinobi->fxTrailAnimator[0], true, 0);
    RSDK.SetSpriteAnimation(-1, 0, &HeavyShinobi->fxTrailAnimator[1], true, 0);
    RSDK.SetSpriteAnimation(-1, 0, &HeavyShinobi->fxTrailAnimator[2], true, 0);
    RSDK.SetSpriteAnimation(-1, 0, &HeavyShinobi->fxTrailAnimator[3], true, 0);

    HeavyShinobi->activeShurikens    = 0;
    HeavyShinobi->health             = 8;
    HeavyShinobi->invincibilityTimer = 0;

    HeavyShinobi->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    HeavyShinobi->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    HeavyShinobi->sfxDefeat    = RSDK.GetSfx("PSZ/ShinobiDefeat.wav");
    HeavyShinobi->sfxDropIn    = RSDK.GetSfx("PSZ/ShinobiDropIn.wav");
    HeavyShinobi->sfxExplode   = RSDK.GetSfx("PSZ/ShinobiExplode.wav");
    HeavyShinobi->sfxGlitch    = RSDK.GetSfx("PSZ/ShinobiGlitch.wav");
    HeavyShinobi->sfxJump      = RSDK.GetSfx("PSZ/ShinobiJump.wav");
    HeavyShinobi->sfxParry     = RSDK.GetSfx("PSZ/ShinobiParry.wav");
    HeavyShinobi->sfxSlash     = RSDK.GetSfx("PSZ/ShinobiSlash.wav");
    HeavyShinobi->sfxStick     = RSDK.GetSfx("PSZ/ShinobiStick.wav");
    HeavyShinobi->sfxThrow     = RSDK.GetSfx("PSZ/ShinobiThrow.wav");
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
        int32 storeAnim = self->mainAnimator.animationID;
        if (HeavyShinobi->fxTrailAnimator[i].animationID != storeAnim) {
            if (HeavyShinobi->storedAnimIDs[i] != storeAnim) {
                HeavyShinobi->storedAnimIDs[i] = self->mainAnimator.animationID;
                HeavyShinobi->storedIDs[i]     = 4 * (i + 1);
            }
        }

        if (HeavyShinobi->storedIDs[i]) {

            --HeavyShinobi->storedIDs[i];
            if (!HeavyShinobi->storedIDs[i]) {
                int32 id = HeavyShinobi->storedAnimIDs[i];
                if (id && (id <= 14 || id > 16)) {
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, HeavyShinobi->storedAnimIDs[i], &HeavyShinobi->fxTrailAnimator[i], false, 0);
                }
                else {
                    RSDK.SetSpriteAnimation(-1, 0, &HeavyShinobi->fxTrailAnimator[i], false, 0);
                    HeavyShinobi->fxTrailAnimator[i].animationID = (uint8)-1;
                }
            }
        }
    }

    if (HeavyShinobi->invincibilityTimer)
        HeavyShinobi->invincibilityTimer--;
}

void HeavyShinobi_HandleSlash(EntityPlayer *player)
{
    RSDK_THIS(HeavyShinobi);

    RSDK.PlaySfx(HeavyShinobi->sfxSlash, false, 255);

    self->direction = player->position.x >= self->position.x;
    RSDK.SetSpriteAnimation(-1, 0, &self->fxAnimator, true, 0);
    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 4, &self->mainAnimator, true, 1);

    CREATE_ENTITY(HeavyShinobi, INT_TO_VOID(SHINOBI_SLASH), self->position.x, self->position.y)->direction = self->direction;

    int32 delay = 4;
    int32 alpha = 0xC0;
    for (int32 i = 3; i >= 0; --i) {
        EntityHeavyShinobi *slash = CREATE_ENTITY(HeavyShinobi, INT_TO_VOID(SHINOBI_SLASH), self->position.x, self->position.y);
        slash->mainAnimator.frameDuration += delay;
        slash->alpha     = alpha;
        slash->direction = self->direction;

        delay += 4;
        alpha -= 32;
    }

    self->state = HeavyShinobi_State_Slash;
}

void HeavyShinobi_StartJump(void)
{
    RSDK_THIS(HeavyShinobi);

    RSDK.SetSpriteAnimation(-1, 0, &self->fxAnimator, true, 0);
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

    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 3, &self->mainAnimator, true, 0);
    self->state = HeavyShinobi_State_Jump;
}

void HeavyShinobi_Hit(void)
{
    RSDK_THIS(HeavyShinobi);

    if (HeavyShinobi->health)
        HeavyShinobi->health--;

    if (!HeavyShinobi->health) {
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        RSDK.PlaySfx(HeavyShinobi->sfxExplosion, false, 255);
        self->timer = 120;
        self->state = HeavyShinobi_State_Destroyed;
    }
    else {
        RSDK.PlaySfx(HeavyShinobi->sfxHit, false, 255);
        self->timer -= 60;
        HeavyShinobi->invincibilityTimer = 30;
    }
}

void HeavyShinobi_Explode(void)
{
    RSDK_THIS(HeavyShinobi);

    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(HeavyShinobi->sfxExplosion, false, 255);

        if (!(Zone->timer & 0xF)) {
            int32 x                    = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1] + 2;
        }
    }
}

void HeavyShinobi_State_Init(void)
{
    RSDK_THIS(HeavyShinobi);

    if (++self->timer >= 2) {
        self->timer = 0;

        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsR[0]      = ScreenInfo->center.x + 80 + (self->position.x >> 16);
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsB[0]      = (self->position.y >> 16) + 68;

        self->position.y = (ScreenInfo->position.y - 192) << 16;
        for (int32 i = 0; i < 16; ++i) HeavyShinobi->storePos[i] = self->position;

        self->active = ACTIVE_NORMAL;
        self->state  = HeavyShinobi_State_SetupArena;
    }
}

void HeavyShinobi_State_SetupArena(void)
{
    RSDK_THIS(HeavyShinobi);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]      = ScreenInfo->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x - 0x500000) {
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - ScreenInfo->center.x - 80;

        int32 y = (Zone->cameraBoundsB[0] - 376) << 16;

        CREATE_ENTITY(HeavyShinobi, INT_TO_VOID(SHINOBI_BOUNDS), (Zone->cameraBoundsL[0] + 40) << 16, y);

        EntityHeavyShinobi *rBounds = CREATE_ENTITY(HeavyShinobi, INT_TO_VOID(SHINOBI_BOUNDS), (Zone->cameraBoundsR[0] - 40) << 16, y);
        rBounds->timer              = 1;
        rBounds->position.y         = (Zone->cameraBoundsB[0] - 99) << 16;
        rBounds->state              = HeavyShinobi_StateBounds_Active;

        self->state = HeavyShinobi_State_StartFight;
    }
}

void HeavyShinobi_State_StartFight(void)
{
    RSDK_THIS(HeavyShinobi);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]      = ScreenInfo->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - ScreenInfo->center.x - 80;

        Music_TransitionTrack(TRACK_HBHBOSS, 0.0125);
        self->visible = true;
        HeavyShinobi_StartJump();
        self->velocity.x = 0;
        self->timer      = 0;
    }
}

void HeavyShinobi_State_Idle(void)
{
    RSDK_THIS(HeavyShinobi);

    HeavyShinobi_HandleAfterFX();

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->fxAnimator);
    for (int32 i = 0; i < 4; ++i) RSDK.ProcessAnimation(&HeavyShinobi->fxTrailAnimator[i]);

    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;

    EntityPlayer *player = Player_GetNearestPlayerX();

    if (abs(player->position.x - self->position.x) < 0x500000 && player->state != Ice_PlayerState_Frozen) {
        HeavyShinobi_HandleSlash(player);
    }
    else {
        if (!HeavyShinobi->activeShurikens && --self->timer <= 0)
            HeavyShinobi_StartJump();
    }
}

void HeavyShinobi_State_Slash(void)
{
    RSDK_THIS(HeavyShinobi);

    HeavyShinobi_HandleAfterFX();

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->fxAnimator);
    for (int32 i = 0; i < 4; ++i) RSDK.ProcessAnimation(&HeavyShinobi->fxTrailAnimator[i]);

    if (self->mainAnimator.frameID == 12)
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->fxAnimator, false, 0);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true))
        self->velocity.y = 0;

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1) {
        EntityPlayer *player = Player_GetNearestPlayerX();
        if (abs(player->position.x - self->position.x) >= 0x500000 || player->state == Ice_PlayerState_Frozen)
            HeavyShinobi_StartJump();
        else
            HeavyShinobi_HandleSlash(player);
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &HeavyShinobi->hitboxSlashRange) && Player_CheckValidState(player)) {
            Ice_FreezePlayer(player);
            player->timer      = 3;
            player->onGround   = false;
            player->velocity.x = player->position.x < self->position.x ? -0x30000 : 0x30000;
            player->velocity.y = -0x38000;
        }
    }
}

void HeavyShinobi_State_Jump(void)
{
    RSDK_THIS(HeavyShinobi);

    HeavyShinobi_HandleAfterFX();

    for (int32 i = 0; i < 4; ++i) RSDK.ProcessAnimation(&HeavyShinobi->fxTrailAnimator[i]);
    RSDK.ProcessAnimation(&self->mainAnimator);

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
                case 2: count = 3; break;

                case 3:
                case 4:
                case 5: count = 2; break;

                case 6:
                case 7: count = 1; break;

                case 8:
                default: count = 0; break;
            }

            if (count)
                RSDK.PlaySfx(HeavyShinobi->sfxThrow, false, 0xFF);

            for (int32 i = 0; i < count; ++i) {
                EntityHeavyShinobi *asteron   = CREATE_ENTITY(HeavyShinobi, INT_TO_VOID(SHINOBI_ASTERON), self->position.x, self->position.y);
                asteron->direction            = FLIP_NONE;
                asteron->mainAnimator.frameID = RSDK.Rand(0, 8);
                asteron->timer                = 180;

                switch (count) {
                    case 1: asteron->angle = 64; break;

                    case 2:
                        if (!i)
                            asteron->angle = 96;
                        else
                            asteron->angle = 32;
                        break;

                    case 3:
                        switch (i) {
                            case 0: asteron->angle = 96; break;
                            case 1: asteron->angle = 64; break;
                            case 2: asteron->angle = 32; break;
                        }
                        break;
                }

                asteron->angle += RSDK.Rand(-16, 17);
                asteron->velocity.x = 0x300 * RSDK.Cos256(asteron->angle);
                asteron->velocity.y = 0x300 * RSDK.Sin256(asteron->angle);
                ++HeavyShinobi->activeShurikens;
            }
        }
    }

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true)) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->direction  = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;

        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->fxAnimator, false, 0);
        self->timer = RSDK.Rand(30, 91);
        self->state = HeavyShinobi_State_Idle;
    }
    else {
        foreach_active(Player, player)
        {
            if (player->state != Ice_PlayerState_Frozen && Player_CheckBadnikTouch(player, self, &HeavyShinobi->hitboxShinobi)
                && Player_CheckBossHit(player, self)) {
                if (player->position.x >= self->position.x) {
                    self->direction  = FLIP_X;
                    self->velocity.x = -0x20000;
                }
                else {
                    self->direction  = FLIP_NONE;
                    self->velocity.x = 0x20000;
                }
                self->velocity.y = -0x40000;

                int32 angle        = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
                player->velocity.x = 0x380 * RSDK.Cos256(angle);
                player->velocity.y = 0x380 * RSDK.Sin256(angle);
                self->timer        = 180;
                self->glitchTimer  = 0;

                RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 15, &self->mainAnimator, true, 0);
                RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->fxAnimator, false, 0);
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

    RSDK.ProcessAnimation(&self->mainAnimator);

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

    if (self->velocity.y > 0x2800 && RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true)) {
        RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        self->velocity.x = 0;
        self->velocity.y = 0;

        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 16, &self->mainAnimator, false, 0);
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->fxAnimator, false, 0);

        if (--self->timer <= 0) {
            RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 0, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->fxAnimator, false, 0);

            self->timer = RSDK.Rand(30, 91);
            self->state = HeavyShinobi_State_Idle;
            RSDK.StopSfx(HeavyShinobi->sfxGlitch);
        }
    }

    if (!HeavyShinobi->invincibilityTimer) {
        foreach_active(Player, player)
        {
            if (player->state != Ice_PlayerState_Frozen && Player_CheckBadnikTouch(player, self, &HeavyShinobi->hitboxShinobi)
                && Player_CheckBossHit(player, self)) {
                HeavyShinobi_Hit();
                foreach_break;
            }
        }
    }
}

void HeavyShinobi_State_Destroyed(void)
{
    RSDK_THIS(HeavyShinobi);

    HeavyShinobi_HandleAfterFX();

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->fxAnimator);

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

    if (self->velocity.y > 0x2800 && RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true)) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->direction  = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;

        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 16, &self->mainAnimator, false, 0);
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->fxAnimator, false, 0);
    }

    HeavyShinobi_Explode();

    if (--self->timer <= 0) {
        self->velocity.y = -0x70000;

        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 3, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->fxAnimator, true, 0);

        RSDK.PlaySfx(HeavyShinobi->sfxDefeat, false, 255);
        self->state = HeavyShinobi_State_Finished;
    }
}

void HeavyShinobi_State_Finished(void)
{
    RSDK_THIS(HeavyShinobi);

    HeavyShinobi_HandleAfterFX();

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->fxAnimator);
    for (int32 i = 0; i < 4; ++i) RSDK.ProcessAnimation(&HeavyShinobi->fxTrailAnimator[i]);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (self->velocity.y > 0)
        self->drawGroup = Zone->objectDrawGroup[1];

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        Zone->cameraBoundsR[0] += WIDE_SCR_XSIZE;
        HeavyShinobi->health = -1;
        destroyEntity(self);
    }
}

void HeavyShinobi_Draw_Shinobi(void)
{
    RSDK_THIS(HeavyShinobi);

    RSDK.DrawSprite(&self->fxAnimator, NULL, false);

    self->inkEffect = INK_ALPHA;
    self->alpha     = 0x60;

    for (int32 i = 3; i >= 0; --i) {
        RSDK.DrawSprite(&HeavyShinobi->fxTrailAnimator[i], &HeavyShinobi->storePos[i << 2], false);
        self->alpha += 0x20;
    }

    self->inkEffect = INK_NONE;
    if (HeavyShinobi->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void HeavyShinobi_StateSlash_Active(void)
{
    RSDK_THIS(HeavyShinobi);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->mainAnimator.frameID != self->mainAnimator.frameCount - 1) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &HeavyShinobi->hitboxSlash) && Player_CheckValidState(player)) {
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

void HeavyShinobi_Draw_Slash(void)
{
    RSDK_THIS(HeavyShinobi);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void HeavyShinobi_StateAsteron_Thrown(void)
{
    RSDK_THIS(HeavyShinobi);

    if (HeavyShinobi->health) {
        RSDK.ProcessAnimation(&self->mainAnimator);

        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        bool32 isStuck = false;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &HeavyShinobi->hitboxAsteron)) {
                if (player->state == Ice_PlayerState_Frozen) {
                    self->playerPtr        = player;
                    self->playerDistance.x = self->position.x - player->position.x;
                    self->playerDistance.y = self->position.y - player->position.y;

                    isStuck = true;
                    foreach_break;
                }

#if MANIA_USE_PLUS
                if (Player_CheckMightyShellHit(player, self, -0x400, -0x600)) {
                    self->interaction = false;
                    self->state       = HeavyShinobi_StateAsteron_Debris;
                    --HeavyShinobi->activeShurikens;
                }
                else {
#endif
                    Player_Hurt(player, self);
#if MANIA_USE_PLUS
                }
#endif
            }
        }

        if (!isStuck && RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, true))
            isStuck = true;

        if (!isStuck) {
            foreach_active(HeavyShinobi, boss)
            {
                if (boss->type == SHINOBI_BOUNDS
                    && RSDK.CheckObjectCollisionBox(boss, &HeavyShinobi->hitboxBounds, self, &HeavyShinobi->hitboxAsteron, true))
                    isStuck = true;
            }
        }

        if (isStuck) {
            if (self->mainAnimator.frameID & 1)
                self->mainAnimator.frameID = (self->mainAnimator.frameID + 1) & 7;

            RSDK.PlaySfx(HeavyShinobi->sfxStick, false, 255);
            RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, (self->mainAnimator.frameID >> 1) + 10, &self->fxAnimator, true, 0);
            self->state = HeavyShinobi_StateAsteron_Explode;
        }

        if (!RSDK.CheckOnScreen(self, &self->updateRange))
            destroyEntity(self);
    }
    else {
        RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1] + 2;

        destroyEntity(self);
    }
}

void HeavyShinobi_StateAsteron_Debris(void)
{
    RSDK_THIS(HeavyShinobi);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->visible ^= true;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void HeavyShinobi_StateAsteron_Explode(void)
{
    RSDK_THIS(HeavyShinobi);

    if (!HeavyShinobi->health) {
        RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1] + 2;

        destroyEntity(self);
    }
    else {
        RSDK.ProcessAnimation(&self->fxAnimator);
        self->fxAnimator.speed += 2;

        foreach_active(Player, player)
        {
            if (self->playerPtr == player) {
                if (player->state != Ice_PlayerState_Frozen) {
                    RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
                    CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup =
                        Zone->objectDrawGroup[1] + 2;
                    --HeavyShinobi->activeShurikens;

                    destroyEntity(self);
                    foreach_break;
                }
            }
            else if ((player->state != Ice_PlayerState_Frozen || !self->playerPtr)
                     && Player_CheckBadnikTouch(player, self, &HeavyShinobi->hitboxAsteron) && Player_CheckItemBreak(player, self, true)) {
                RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
                CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1] + 2;
                --HeavyShinobi->activeShurikens;

                destroyEntity(self);
                foreach_break;
            }
        }

        EntityPlayer *playerPtr = self->playerPtr;
        if (playerPtr) {
            self->position.x = playerPtr->position.x + self->playerDistance.x;
            self->position.y = playerPtr->position.y + self->playerDistance.y;
        }

        if (--self->timer <= 0) {
            --HeavyShinobi->activeShurikens;
            RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1] + 2;

            for (int32 i = 0; i < 5; ++i) {
                EntityHeavyShinobi *child = CREATE_ENTITY(HeavyShinobi, INT_TO_VOID(SHINOBI_ASTERONSPIKE), self->position.x, self->position.y);
                child->rotation           = (self->mainAnimator.frameID & 0xFFFFFFFE) << 6;
                child->direction          = self->direction;

                switch (i) {
                    case 0:
                        child->angle = (32 * ((self->mainAnimator.frameID & 0xFE) - 2));
                        RSDK.Cos512(child->rotation);
                        child->position.y -= RSDK.Sin512(child->rotation) << 10;
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 17, &child->mainAnimator, true, 0);
                        break;

                    case 1:
                        child->angle = (32 * (self->mainAnimator.frameID & 0xFE) - 120);
                        child->position.x -= 0x380 * RSDK.Cos512(child->rotation);
                        child->position.y -= 0x180 * RSDK.Sin512(child->rotation);
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 18, &child->mainAnimator, true, 0);
                        break;

                    case 2:
                        child->angle = (32 * (self->mainAnimator.frameID & 0xFE) - 8);
                        child->position.x += 0x380 * RSDK.Cos512(child->rotation);
                        child->position.y -= 0x180 * RSDK.Sin512(child->rotation);
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 18, &child->mainAnimator, true, 0);
                        child->direction ^= FLIP_X;
                        break;

                    case 3:
                        child->angle = (32 * ((self->mainAnimator.frameID & 0xFE) + 3));
                        child->position.x -= 0x380 * RSDK.Cos512(child->rotation);
                        child->position.y += 0x380 * RSDK.Sin512(child->rotation);
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 19, &child->mainAnimator, true, 0);
                        break;

                    case 4:
                        child->angle = (32 * ((self->mainAnimator.frameID & 0xFE) + 1));
                        child->position.x += 0x380 * RSDK.Cos512(child->rotation);
                        child->position.y += 0x380 * RSDK.Sin512(child->rotation);
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 19, &child->mainAnimator, true, 0);
                        child->direction ^= FLIP_X;
                        break;

                    default: break;
                }

                child->velocity.x = 0x300 * RSDK.Cos256(child->angle);
                child->velocity.y = 0x300 * RSDK.Sin256(child->angle);
            }

            if (playerPtr) {
                Ice_BreakPlayerBlock(playerPtr);
                playerPtr->state = Player_State_Air;
                Player_Hurt(playerPtr, self);
            }

            destroyEntity(self);
        }

        if (!RSDK.CheckOnScreen(self, &self->updateRange))
            destroyEntity(self);
    }
}

void HeavyShinobi_Draw_Asteron(void)
{
    RSDK_THIS(HeavyShinobi);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    self->inkEffect = INK_ADD;

    RSDK.DrawSprite(&self->fxAnimator, NULL, false);
    self->inkEffect = INK_NONE;
}

void HeavyShinobi_State_AsteronSpike(void)
{
    RSDK_THIS(HeavyShinobi);

    if (HeavyShinobi->health) {
        RSDK.ProcessAnimation(&self->mainAnimator);

        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &HeavyShinobi->hitboxAsteronSpike) && player->state != Ice_PlayerState_Frozen) {
#if MANIA_USE_PLUS
                if (Player_CheckMightyShellHit(player, self, -0x400, -0x600)) {
                    self->interaction = false;
                    self->state       = HeavyShinobi_StateAsteron_Debris;
                }
                else {
#endif
                    Player_Hurt(player, self);
#if MANIA_USE_PLUS
                }
#endif
            }
        }

        if (!RSDK.CheckOnScreen(self, &self->updateRange))
            destroyEntity(self);
    }
    else {
        RSDK.PlaySfx(HeavyShinobi->sfxExplosion, false, 255);
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[0];

        destroyEntity(self);
    }
}

void HeavyShinobi_Draw_AsteronSpike(void)
{
    RSDK_THIS(HeavyShinobi);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void HeavyShinobi_StateBounds_WaitForPlayer(void)
{
    RSDK_THIS(HeavyShinobi);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x4800;

    if (self->position.y >= (Zone->cameraBoundsB[0] - 99) << 16) {
        self->position.y = (Zone->cameraBoundsB[0] - 99) << 16;
        self->state      = HeavyShinobi_StateBounds_Active;
    }

    foreach_active(Player, player) { Player_CheckCollisionBox(player, self, &HeavyShinobi->hitboxBounds); }
}

void HeavyShinobi_StateBounds_Active(void)
{
    RSDK_THIS(HeavyShinobi);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &HeavyShinobi->hitboxBounds)) {
            Hitbox *playerHitbox = Player_GetHitbox(player);

            int32 left  = 0;
            int32 right = 0;
            if (self->timer) {
                left  = HeavyShinobi->hitboxBounds.left;
                right = playerHitbox->right;
            }
            else {
                left  = HeavyShinobi->hitboxBounds.right;
                right = playerHitbox->left;
            }

            player->position.x = self->position.x + ((left - right) << 16);
        }
    }

    if (HeavyShinobi->health == -1) {
        RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);

        for (int32 i = 0; i < 0x80; ++i) {
            int32 x        = self->position.x + (RSDK.Rand(-64, 65) << 16);
            int32 y        = self->position.y + (RSDK.Rand(-80, 81) << 16);
            EntityIce *ice = CREATE_ENTITY(Ice, INT_TO_VOID(ICE_CHILD_SHARD), x, y);

            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 1, &ice->blockAnimator, true, 0);
            ice->velocity.x          = RSDK.Rand(-6, 8) << 15;
            ice->velocity.y          = RSDK.Rand(-10, 2) << 15;
            ice->direction           = RSDK.Rand(0, 4);
            ice->blockAnimator.speed = RSDK.Rand(1, 4);
            ice->active              = ACTIVE_NORMAL;
        }

        destroyEntity(self);
    }
}

void HeavyShinobi_Draw_Bounds(void)
{
    RSDK_THIS(HeavyShinobi);
    Vector2 drawPos;

    drawPos.x                  = self->position.x;
    drawPos.y                  = self->position.y - 0x880000;
    self->mainAnimator.frameID = 3;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    self->mainAnimator.frameID = 4;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    drawPos.y += 0x500000;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    drawPos.y += 0x500000;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    self->direction            = FLIP_Y;
    self->mainAnimator.frameID = 3;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    self->direction = FLIP_NONE;
}

#if GAME_INCLUDE_EDITOR
void HeavyShinobi_EditorDraw(void)
{
    RSDK_THIS(HeavyShinobi);

    self->active    = ACTIVE_BOUNDS;
    self->visible   = false;
    self->drawGroup = Zone->objectDrawGroup[0];
    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 0, &self->mainAnimator, true, 0);
    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &self->fxAnimator, true, 0);
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    HeavyShinobi_Draw_Shinobi();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);
        int32 boundsL = (self->position.x >> 16) - WIDE_SCR_XCENTER - 80;
        int32 boundsR = (self->position.x >> 16) + WIDE_SCR_XCENTER + 80;
        int32 boundsB = (self->position.y >> 16) + 68;

        Vector2 startPos = self->position;

        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawGroup = Zone->fgDrawGroup[1] - 1;
        RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 0, &self->mainAnimator, true, 0);

        self->position.x = (boundsL + 40) << 16;
        self->position.y = (boundsB - 99) << 16;
        HeavyShinobi_Draw_Bounds();

        self->position.x = (boundsR - 40) << 16;
        self->position.y = (boundsB - 99) << 16;
        HeavyShinobi_Draw_Bounds();

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER - 80, -SCREEN_YSIZE, WIDE_SCR_XCENTER + 80, 68, 1 | 0 | 4 | 8, 0x00C0F0);

        self->position = startPos;
        RSDK_DRAWING_OVERLAY(false);
    }
}

void HeavyShinobi_EditorLoad(void)
{
    HeavyShinobi->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Shinobi.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(-1, 0, &HeavyShinobi->fxTrailAnimator[0], true, 0);
    RSDK.SetSpriteAnimation(-1, 0, &HeavyShinobi->fxTrailAnimator[1], true, 0);
    RSDK.SetSpriteAnimation(-1, 0, &HeavyShinobi->fxTrailAnimator[2], true, 0);
    RSDK.SetSpriteAnimation(-1, 0, &HeavyShinobi->fxTrailAnimator[3], true, 0);
}
#endif

void HeavyShinobi_Serialize(void) {}
