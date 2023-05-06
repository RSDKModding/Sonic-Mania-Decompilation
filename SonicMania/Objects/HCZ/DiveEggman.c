// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DiveEggman Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDiveEggman *DiveEggman;

void DiveEggman_Update(void)
{
    RSDK_THIS(DiveEggman);

    StateMachine_Run(self->state);
}

void DiveEggman_LateUpdate(void) {}

void DiveEggman_StaticUpdate(void) {}

void DiveEggman_Draw(void)
{
    RSDK_THIS(DiveEggman);

    if (self->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 130, 0xE0E0E0);

    RSDK.DrawSprite(&self->animator, NULL, false);

    RSDK.SetPaletteEntry(0, 130, 0x001840);
}

void DiveEggman_Create(void *data)
{
    RSDK_THIS(DiveEggman);

    if (globals->gameMode == MODE_TIMEATTACK) {
        destroyEntity(self);
    }
    else {
        self->drawFX = FX_FLIP;
        if (!SceneInfo->inEditor) {
            self->visible = true;
            if (data) {
                switch (VOID_TO_INT(data)) {
                    default: break;

                    case DIVEEGGMAN_BOMB:
                        self->active    = ACTIVE_NORMAL;
                        self->drawGroup = Zone->playerDrawGroup[0] + 1;
                        self->timer     = 480;
                        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 5, &self->animator, true, 0);
                        self->state = DiveEggman_StateBomb_Idle;
                        break;
                }
            }
            else {
                self->active            = ACTIVE_BOUNDS;
                self->drawGroup         = Zone->playerDrawGroup[0] + 2;
                self->updateRange.x     = 0x800000;
                self->startY            = self->position.y;
                self->updateRange.y     = 0x800000;
                self->health            = 4;
                DiveEggman->screwMobile = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, ScrewMobile);
                RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 0, &self->animator, true, 0);
                self->state = DiveEggman_StateEggman_AwaitPlayer;
            }
        }
    }
}

void DiveEggman_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("HCZ")) {
        DiveEggman->diveFrames = RSDK.LoadSpriteAnimation("HCZ/DiveEggman.bin", SCOPE_STAGE);
        DiveEggman->aniFrames  = RSDK.LoadSpriteAnimation("Eggman/EggmanHCZ1.bin", SCOPE_STAGE);
    }

    DiveEggman->unusedHitbox1.left   = -8;
    DiveEggman->unusedHitbox1.top    = -8;
    DiveEggman->unusedHitbox1.right  = 8;
    DiveEggman->unusedHitbox1.bottom = 8;

    DiveEggman->hitboxBomb.left   = -8;
    DiveEggman->hitboxBomb.top    = -8;
    DiveEggman->hitboxBomb.right  = 8;
    DiveEggman->hitboxBomb.bottom = 8;

    DiveEggman->hitboxEggman.left   = -26;
    DiveEggman->hitboxEggman.top    = -22;
    DiveEggman->hitboxEggman.right  = 7;
    DiveEggman->hitboxEggman.bottom = 11;

    DiveEggman->sfxBigFan       = RSDK.GetSfx("HCZ/BigFan.wav");
    DiveEggman->sfxHit          = RSDK.GetSfx("Stage/BossHit.wav");
    DiveEggman->sfxExplosion    = RSDK.GetSfx("Stage/Explosion2.wav");
    DiveEggman->sfxRockemSockem = RSDK.GetSfx("Stage/RockemSockem.wav");
}

void DiveEggman_Hit(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.PlaySfx(DiveEggman->sfxHit, false, 255);

    self->invincibilityTimer = 30;
    if (self->health)
        self->health--;

    if (!self->health) {
        self->timer            = 120;
        self->drawGroup        = Zone->hudDrawGroup - 1;
        self->state            = DiveEggman_StateEggman_Destroyed;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
}

void DiveEggman_Explode(void)
{
    RSDK_THIS(DiveEggman);

    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(DiveEggman->sfxExplosion, false, 255);

        if (!(Zone->timer & 0xF)) {
            int32 x                    = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);

            if (self->timer <= 40)
                explosion->drawGroup = self->drawGroup;
            else
                explosion->drawGroup = self->drawGroup - 1;
        }
    }
}

void DiveEggman_StateEggman_AwaitPlayer(void)
{
    RSDK_THIS(DiveEggman);

    EntityScrewMobile *screwMobile = DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&self->animator);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    if (screwMobile->state != ScrewMobile_State_CheckPlayerEnter) {
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &self->animator, true, 0);
        self->velocity.x = -0x10000;
        self->active     = ACTIVE_NORMAL;
        self->state      = DiveEggman_StateEggman_Swimming;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void DiveEggman_StateEggman_Swimming(void)
{
    RSDK_THIS(DiveEggman);

    EntityScrewMobile *screwMobile = DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&self->animator);
    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x += self->velocity.x;

    if (self->velocity.y > 0) {
        self->position.y += self->velocity.y;
        self->velocity.y -= 0x400;
    }

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x200000, 0, true)
        || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x200000, 0, true)) {
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
    }

    if (screwMobile->propellerAnimator.speed >= 0xFF) {
        if (self->position.x <= screwMobile->position.x) {
            self->direction  = FLIP_NONE;
            self->velocity.x = 0x8000;
        }
        else {
            self->direction  = FLIP_X;
            self->velocity.x = -0x8000;
        }

        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 2, &self->animator, false, 0);
        self->state = DiveEggman_StateEggman_InWhirlpool;
    }

    if (--self->timer <= 0) {
        self->timer = 180;
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 4, &self->animator, false, 0);
        self->state = DiveEggman_StateEggman_PlaceBomb;
    }
}

void DiveEggman_StateEggman_InWhirlpool(void)
{
    RSDK_THIS(DiveEggman);

    EntityScrewMobile *screwMobile = DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&self->animator);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x += self->velocity.x;

    if (self->position.y >= Water->waterLevel + 0x180000)
        self->position.y -= 0x2000;

    if (screwMobile->propellerAnimator.speed >= 0x100) {
        if (abs(self->position.x - screwMobile->position.x) < 0x100000) {
            int32 x = self->position.x - screwMobile->position.x;
            int32 y = MathHelpers_SquareRoot(0x100 - (x >> 16) * (x >> 16)) << 16;

            self->angle = RSDK.ATan2(x, y);
            if (self->angle < 0x80)
                self->drawGroup = Zone->playerDrawGroup[0] + 2;
            else
                self->drawGroup = Zone->hudDrawGroup - 1;
            RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 3, &self->animator, false, 0);
            self->state = DiveEggman_StateEggman_WhirlpoolRise;
        }
    }
    else {
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x10000;
        else
            self->velocity.x = 0x10000;

        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &self->animator, false, 0);
        self->state = DiveEggman_StateEggman_Swimming;
    }
}

void DiveEggman_StateEggman_WhirlpoolRise(void)
{
    RSDK_THIS(DiveEggman);

    EntityScrewMobile *screwMobile = DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&self->animator);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x = (RSDK.Cos256(self->angle) << 12) + screwMobile->position.x;
    self->position.y -= 0x10000;

    if ((self->angle & 0xFF) < 0x80)
        self->drawGroup = Zone->playerDrawGroup[0] + 2;
    else
        self->drawGroup = Zone->hudDrawGroup - 1;
    self->angle += 4;

    if (screwMobile->propellerAnimator.speed >= 0x100) {
        if (self->position.y < screwMobile->position.y + 0x280000) {
            DiveEggman_Hit();
            screwMobile->whirlPoolTimer = 60;
        }
    }
    else {
        if (self->position.y < Water->waterLevel - (screwMobile->whirlpoolHeight << 16)) {
            self->velocity.y = -0x10000;
            self->state      = DiveEggman_StateEggman_Falling;
            self->velocity.x = RSDK.Cos256(self->angle) << 8;
        }
    }
}

void DiveEggman_StateEggman_Falling(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.ProcessAnimation(&self->animator);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (self->position.y >= Water->waterLevel) {
        self->velocity.y >>= 2;
        CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), self->position.x, Water->waterLevel);

        RSDK.PlaySfx(Water->sfxSplash, false, 255);
        self->velocity.x = self->direction == FLIP_NONE ? -0x10000 : 0x10000;
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &self->animator, false, 0);
        self->state = DiveEggman_StateEggman_Swimming;
    }
}

void DiveEggman_StateEggman_PlaceBomb(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.ProcessAnimation(&self->animator);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    if (self->animator.frameID >= self->animator.frameCount - 1) {
        EntityDiveEggman *bomb = CREATE_ENTITY(DiveEggman, INT_TO_VOID(DIVEEGGMAN_BOMB), self->position.x, self->position.y + 0x20000);
        bomb->position.x += self->direction ? 0x1A0000 : -0x1A0000;

        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &self->animator, false, 0);
        self->state = DiveEggman_StateEggman_Swimming;
    }
}

void DiveEggman_StateEggman_Destroyed(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.ProcessAnimation(&self->animator);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    DiveEggman_Explode();

    if (--self->timer <= 0) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        RSDK.SetSpriteAnimation(DiveEggman->aniFrames, 0, &self->animator, true, 0);
        self->velocity.y = -0x20000;
        self->timer      = 0x2000;
        self->state      = DiveEggman_StateEggman_Finish;
    }
}

void DiveEggman_StateEggman_Finish(void)
{
    RSDK_THIS(DiveEggman);

    EntityScrewMobile *screwMobile = DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&self->animator);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x += self->velocity.x;

    self->position.y += self->velocity.y;
    self->velocity.y += self->timer;

    if (self->timer == 0x2000 && self->position.y >= Water->waterLevel) {
        self->velocity.y >>= 2;
        self->timer = 0x1000;

        CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), self->position.x, Water->waterLevel);
        RSDK.PlaySfx(Water->sfxSplash, false, 255);
    }

    if (!RSDK.CheckOnScreen(self, NULL)) {
        Zone->cameraBoundsT[0] = 0;
        screwMobile->state     = ScrewMobile_State_BossFinished;
        destroyEntity(self);
    }
}

bool32 DiveEggman_CheckNoBombExplode(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.ProcessAnimation(&self->animator);

    if (!--self->timer) {
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
        RSDK.PlaySfx(DiveEggman->sfxExplosion, false, 255);

        EntityWater *water = CREATE_ENTITY(Water, INT_TO_VOID(WATER_BUBBLE), self->position.x, self->position.y);
        water->velocity.y  = -0x8800;
        water->angle       = 2 * RSDK.Rand(0, 256);
        water->bubbleX     = water->position.x;
        water->childPtr    = 0;
        RSDK.SetSpriteAnimation(Water->aniFrames, 3, &water->animator, true, 0);

        destroyEntity(self);
        return false;
    }

    return true;
}

void DiveEggman_StateBomb_Idle(void)
{
    RSDK_THIS(DiveEggman);

    if (self->velocity.x) {
        if (self->velocity.x <= 0)
            self->velocity.x += 0x2000;
        else
            self->velocity.x -= 0x2000;
    }

    if (self->velocity.y > 0) {
        self->position.y += self->velocity.y;
        self->velocity.y -= 0x400;
    }

    if (DiveEggman_CheckNoBombExplode()) {
        EntityScrewMobile *screwMobile = DiveEggman->screwMobile;

        if (screwMobile->propellerAnimator.speed >= 0xFF) {
            self->state      = DiveEggman_StateBomb_InWhirlpool;
            self->velocity.x = self->position.x > screwMobile->position.x ? -0x10000 : 0x10000;
        }
    }
}

void DiveEggman_StateBomb_InWhirlpool(void)
{
    RSDK_THIS(DiveEggman);

    EntityScrewMobile *screwMobile = DiveEggman->screwMobile;

    if (DiveEggman_CheckNoBombExplode()) {
        self->position.x += self->velocity.x;

        if (self->position.y >= Water->waterLevel + 0x100000)
            self->position.y -= 0x2000;

        if (screwMobile->propellerAnimator.speed >= 0x100) {
            if (abs(self->position.x - screwMobile->position.x) < 0x100000) {
                int32 x = self->position.x - screwMobile->position.x;
                int32 y = MathHelpers_SquareRoot(0x100 - (x >> 16) * (x >> 16)) << 16;

                self->angle = RSDK.ATan2(x, y);
                if (self->angle < 0x80)
                    self->drawGroup = Zone->playerDrawGroup[0] + 2;
                else
                    self->drawGroup = Zone->hudDrawGroup - 1;

                self->state = DiveEggman_StateBomb_WhirlpoolRise;
            }
        }
        else {
            self->state = DiveEggman_StateBomb_Idle;
        }
    }
}

void DiveEggman_StateBomb_WhirlpoolRise(void)
{
    RSDK_THIS(DiveEggman);

    EntityScrewMobile *screwMobile = DiveEggman->screwMobile;

    if (DiveEggman_CheckNoBombExplode()) {
        self->position.x = (RSDK.Cos256(self->angle) << 12) + screwMobile->position.x;
        self->position.y -= 0x10000;

        if ((self->angle & 0xFF) < 0x80)
            self->drawGroup = Zone->playerDrawGroup[0] + 2;
        else
            self->drawGroup = Zone->hudDrawGroup - 1;
        self->angle += 4;

        if (screwMobile->propellerAnimator.speed >= 0x100) {
            if (self->position.y < screwMobile->position.y + 0x180000) {
                self->timer = 1;
                --screwMobile->health; // ??? what the...
                screwMobile->invincibilityTimer = 30;
                screwMobile->whirlPoolTimer     = 30;
                EntityPlayer *player1           = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

                // Hit that player!
                if (!player1->blinkTimer && !player1->invincibleTimer) {
                    if (player1->shield) {
                        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player1), Shield);
                        if (shield->classID == Shield->classID)
                            destroyEntity(shield);

                        player1->shield     = SHIELD_NONE;
                        player1->blinkTimer = 120;
                        RSDK.PlaySfx(Player->sfxHurt, false, 255);
                    }
                    else {
                        if (player1->rings <= 0) {
                            player1->deathType = PLAYER_DEATH_DIE_USESFX;
                            RSDK.PlaySfx(Player->sfxHurt, false, 255);
                            ScrewMobile->fanSfxTimer = 0;
                        }
                        else {
                            player1->blinkTimer = 120;
                            if (!player1->hyperRing)
                                Ring_LoseRings(player1, player1->rings, player1->collisionPlane);
                            else
                                Ring_LoseHyperRings(player1, player1->rings, player1->collisionPlane);

                            player1->hyperRing     = false;
                            player1->rings         = 0;
                            player1->ringExtraLife = 100;
                            RSDK.PlaySfx(Player->sfxLoseRings, false, 255);
                        }
                    }
                    DiveEggman_CheckNoBombExplode();
                }
            }
        }
        else {
            if (self->position.y < Water->waterLevel - (screwMobile->whirlpoolHeight << 16)) {
                self->velocity.y = -0x10000;
                self->state      = DiveEggman_StateBomb_Falling;
                self->velocity.x = (RSDK.Cos256(self->angle) << 8);
            }
        }
    }
}

void DiveEggman_StateBomb_Falling(void)
{
    RSDK_THIS(DiveEggman);

    if (DiveEggman_CheckNoBombExplode()) {
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
        self->velocity.y += 0x2000;

        if (self->position.y >= Water->waterLevel) {
            self->velocity.y >>= 2;

            CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), self->position.x, Water->waterLevel);
            RSDK.PlaySfx(Water->sfxSplash, false, 255);

            self->state = DiveEggman_StateBomb_Idle;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void DiveEggman_EditorDraw(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 0, &self->animator, true, 0);

    DiveEggman_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        EntityCollapsingPlatform *screwMobile = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, CollapsingPlatform);
        if (ScrewMobile && screwMobile->classID == ScrewMobile->classID)
            DrawHelpers_DrawArrow(self->position.x, self->position.y, screwMobile->position.x, screwMobile->position.y, 0xFFFF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void DiveEggman_EditorLoad(void) { DiveEggman->diveFrames = RSDK.LoadSpriteAnimation("HCZ/DiveEggman.bin", SCOPE_STAGE); }
#endif

void DiveEggman_Serialize(void) {}
