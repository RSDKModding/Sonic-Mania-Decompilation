// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DiveEggman Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
                if (voidToInt(data) == 1) {
                    self->active    = ACTIVE_NORMAL;
                    self->drawOrder = Zone->playerDrawLow + 1;
                    self->timer     = 480;
                    RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 5, &self->animator, true, 0);
                    self->state = DiveEggman_State2_Unknown1;
                }
            }
            else {
                self->active          = ACTIVE_BOUNDS;
                self->drawOrder       = Zone->playerDrawLow + 2;
                self->updateRange.x   = 0x800000;
                self->startY          = self->position.y;
                self->updateRange.y   = 0x800000;
                self->health          = 4;
                DiveEggman->screwMobile = (Entity *)RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, ScrewMobile);
                RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 0, &self->animator, true, 0);
                self->state = DiveEggman_State_Unknown1;
            }
        }
    }
}

void DiveEggman_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ")) {
        DiveEggman->diveFrames = RSDK.LoadSpriteAnimation("HCZ/DiveEggman.bin", SCOPE_STAGE);
        DiveEggman->aniFrames  = RSDK.LoadSpriteAnimation("Eggman/EggmanHCZ1.bin", SCOPE_STAGE);
    }

    DiveEggman->hitbox1.left   = -8;
    DiveEggman->hitbox1.top    = -8;
    DiveEggman->hitbox1.right  = 8;
    DiveEggman->hitbox1.bottom = 8;

    DiveEggman->hitbox2.left   = -8;
    DiveEggman->hitbox2.top    = -8;
    DiveEggman->hitbox2.right  = 8;
    DiveEggman->hitbox2.bottom = 8;

    DiveEggman->hitbox3.left   = -26;
    DiveEggman->hitbox3.top    = -22;
    DiveEggman->hitbox3.right  = 7;
    DiveEggman->hitbox3.bottom = 11;

    DiveEggman->sfxBigFan       = RSDK.GetSfx("HCZ/BigFan.wav");
    DiveEggman->sfxHit          = RSDK.GetSfx("Stage/BossHit.wav");
    DiveEggman->sfxExplosion    = RSDK.GetSfx("Stage/Explosion2.wav");
    DiveEggman->sfxRockemSockem = RSDK.GetSfx("Stage/RockemSockem.wav");
}

void DiveEggman_Explode(void)
{
    RSDK_THIS(DiveEggman);

    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(DiveEggman->sfxExplosion, false, 255);

        if (!(Zone->timer & 0xF)) {
            int x                      = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int y                      = self->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            if (self->timer <= 40)
                explosion->drawOrder = self->drawOrder;
            else
                explosion->drawOrder = self->drawOrder - 1;
        }
    }
}

void DiveEggman_State_Unknown1(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&self->animator);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;
    if (screwMobile->state != ScrewMobile_State_CheckPlayerEnter) {
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &self->animator, true, 0);
        self->velocity.x = -0x10000;
        self->active     = ACTIVE_NORMAL;
        self->state      = DiveEggman_State_Unknown2;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void DiveEggman_State_Unknown2(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&self->animator);
    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x += self->velocity.x;

    if (self->velocity.y > 0) {
        self->position.y += self->velocity.y;
        self->velocity.y -= 0x400;
    }

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x200000, 0, true)
        || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x200000, 0, true)) {
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
    }

    if (screwMobile->animator2.speed >= 0xFF) {
        if (self->position.x <= screwMobile->position.x) {
            self->direction  = FLIP_NONE;
            self->velocity.x = 0x8000;
        }
        else {
            self->direction  = FLIP_X;
            self->velocity.x = -0x8000;
        }
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 2, &self->animator, false, 0);
        self->state = DiveEggman_State_Unknown3;
    }
    if (--self->timer <= 0) {
        self->timer = 180;
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 4, &self->animator, false, 0);
        self->state = DiveEggman_State_Unknown6;
    }
}

void DiveEggman_State_Unknown3(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&self->animator);
    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x += self->velocity.x;

    if (self->position.y >= Water->waterLevel + 0x180000)
        self->position.y -= 0x2000;

    if (screwMobile->animator2.speed >= 0x100) {
        if (abs(self->position.x - screwMobile->position.x) < 0x100000) {
            int dist      = self->position.x - screwMobile->position.x;
            int y         = MathHelpers_SquareRoot((0x100 - (dist >> 16) * (dist >> 16))) << 16;
            self->angle = RSDK.ATan2(dist, y);
            if (self->angle < 0x80)
                self->drawOrder = Zone->playerDrawLow + 2;
            else
                self->drawOrder = Zone->hudDrawOrder - 1;
            RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 3, &self->animator, false, 0);
            self->state = DiveEggman_State_Unknown4;
        }
    }
    else {
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x10000;
        else
            self->velocity.x = 0x10000;
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &self->animator, false, 0);
        self->state = DiveEggman_State_Unknown2;
    }
}

void DiveEggman_State_Unknown4(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&self->animator);
    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.y -= 0x10000;
    self->position.x = (RSDK.Cos256(self->angle) << 12) + screwMobile->position.x;
    if ((self->angle & 0xFF) < 0x80)
        self->drawOrder = Zone->playerDrawLow + 2;
    else
        self->drawOrder = Zone->hudDrawOrder - 1;
    self->angle += 4;

    if (screwMobile->animator2.speed >= 0x100) {
        if (self->position.y < screwMobile->position.y + 0x280000) {
            RSDK.PlaySfx(DiveEggman->sfxHit, false, 255);
            self->invincibilityTimer = 30;
            if (self->health) {
                self->health--;
            }

            if (!self->health) {
                self->timer               = 120;
                self->drawOrder           = Zone->hudDrawOrder - 1;
                self->state               = DiveEggman_State_Destroyed;
                SceneInfo->timeEnabled = false;
                Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
            }
            screwMobile->whirlPoolTimer = 60;
        }
    }
    else {
        if (self->position.y < Water->waterLevel - (screwMobile->field_74 << 16)) {
            self->velocity.y = -0x10000;
            self->state      = DiveEggman_State_Unknown5;
            self->velocity.x = RSDK.Cos256(self->angle) << 8;
        }
    }
}

void DiveEggman_State_Unknown5(void)
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
        CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), self->position.x, Water->waterLevel);
        RSDK.PlaySfx(Water->sfxSplash, false, 255);
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x10000;
        else
            self->velocity.x = 0x10000;
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &self->animator, false, 0);
        self->state = DiveEggman_State_Unknown2;
    }
}

void DiveEggman_State_Unknown6(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.ProcessAnimation(&self->animator);
    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    if (self->animator.frameID >= self->animator.frameCount - 1) {
        EntityDiveEggman *child = CREATE_ENTITY(DiveEggman, intToVoid(1), self->position.x, self->position.y + 0x20000);
        if (self->direction)
            child->position.x += 0x1A0000;
        else
            child->position.x -= 0x1A0000;
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &self->animator, false, 0);
        self->state = DiveEggman_State_Unknown2;
    }
}

void DiveEggman_State_Destroyed(void)
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
        self->state      = DiveEggman_State_Finish;
    }
}

void DiveEggman_State_Finish(void)
{
    RSDK_THIS(DiveEggman);

    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;
    RSDK.ProcessAnimation(&self->animator);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;
    self->position.x += self->velocity.x;

    self->position.y += self->velocity.y;
    self->velocity.y += self->timer;
    if (self->timer == 0x2000 && self->position.y >= Water->waterLevel) {
        self->velocity.y >>= 2;
        self->timer = 0x1000;
        CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), self->position.x, Water->waterLevel);
        RSDK.PlaySfx(Water->sfxSplash, false, 255);
    }

    if (!RSDK.CheckOnScreen(self, NULL)) {
        Zone->cameraBoundsT[0] = 0;
        screwMobile->state      = ScrewMobile_State_BossFinished;
        destroyEntity(self);
    }
}

bool32 DiveEggman_Unknown10(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.ProcessAnimation(&self->animator);
    if (!--self->timer) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(DiveEggman->sfxExplosion, false, 255);
        EntityWater *water = CREATE_ENTITY(Water, intToVoid(WATER_BUBBLE), self->position.x, self->position.y);
        water->velocity.y  = -0x8800;
        water->angle       = 2 * RSDK.Rand(0, 256);
        water->bubbleX    = water->position.x;
        water->childPtr    = 0;
        RSDK.SetSpriteAnimation(Water->aniFrames, 3, &water->animator, true, 0);
        destroyEntity(self);
        return false;
    }
    return true;
}

void DiveEggman_State2_Unknown1(void)
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

    if (DiveEggman_Unknown10()) {
        EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;
        if (screwMobile->animator2.speed >= 0xFF) {
            self->state = DiveEggman_State2_Unknown2;
            if (self->position.x > screwMobile->position.x)
                self->velocity.x = -0x10000;
            else
                self->velocity.x = 0x10000;
        }
    }
}

void DiveEggman_State2_Unknown2(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    if (DiveEggman_Unknown10()) {
        self->position.x += self->velocity.x;

        if (self->position.y >= Water->waterLevel + 0x100000)
            self->position.y -= 0x2000;

        if (screwMobile->animator2.speed >= 0x100) {
            if (abs(self->position.x - screwMobile->position.x) < 0x100000) {
                int dist      = self->position.x - screwMobile->position.x;
                int y         = MathHelpers_SquareRoot((0x100 - (dist >> 16) * (dist >> 16))) << 16;
                self->angle = RSDK.ATan2(dist, y);
                if (self->angle < 0x80)
                    self->drawOrder = Zone->playerDrawLow + 2;
                else
                    self->drawOrder = Zone->hudDrawOrder - 1;
                self->state = DiveEggman_State2_Unknown3;
            }
        }
        else {
            self->state = DiveEggman_State2_Unknown1;
        }
    }
}

void DiveEggman_State2_Unknown3(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    if (DiveEggman_Unknown10()) {
        self->position.y -= 0x10000;
        self->position.x = (RSDK.Cos256(self->angle) << 12) + screwMobile->position.x;
        if ((self->angle & 0xFF) < 0x80)
            self->drawOrder = Zone->playerDrawLow + 2;
        else
            self->drawOrder = Zone->hudDrawOrder - 1;
        self->angle += 4;
        if (screwMobile->animator2.speed >= 0x100) {
            if (self->position.y < screwMobile->position.y + 0x180000) {
                self->timer = 1;
                --screwMobile->field_7C;
                screwMobile->invincibilityTimer = 30;
                screwMobile->whirlPoolTimer     = 30;
                EntityPlayer *player1           = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

                if (!player1->blinkTimer && !player1->invincibleTimer) {
                    if (player1->shield) {
                        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player1), Shield);
                        if (shield->objectID == Shield->objectID)
                            destroyEntity(shield);
                        player1->shield     = SHIELD_NONE;
                        player1->blinkTimer = 120;
                        RSDK.PlaySfx(Player->sfxHurt, false, 255);
                    }
                    else {
                        if (player1->rings <= 0) {
                            player1->deathType = PLAYER_DEATH_DIE_USESFX;
                            RSDK.PlaySfx(Player->sfxHurt, false, 255);
                            ScrewMobile->shouldPlayFanSfx = 0;
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
                    DiveEggman_Unknown10();
                }
            }
        }
        else {
            if (self->position.y < Water->waterLevel - (screwMobile->field_74 << 16)) {
                self->velocity.y = -0x10000;
                self->state      = DiveEggman_State2_Unknown4;
                self->velocity.x = (RSDK.Cos256(self->angle) << 8);
            }
        }
    }
}

void DiveEggman_State2_Unknown4(void)
{
    RSDK_THIS(DiveEggman);

    if (DiveEggman_Unknown10()) {
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
        self->velocity.y += 0x2000;
        if (self->position.y >= Water->waterLevel) {
            self->velocity.y >>= 2;
            CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), self->position.x, Water->waterLevel);
            RSDK.PlaySfx(Water->sfxSplash, false, 255);
            self->state = DiveEggman_State2_Unknown1;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void DiveEggman_EditorDraw(void)
{
    RSDK_THIS(DiveEggman);
    RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 0, &self->animator, true, 0);

    DiveEggman_Draw();
}

void DiveEggman_EditorLoad(void) { DiveEggman->diveFrames = RSDK.LoadSpriteAnimation("HCZ/DiveEggman.bin", SCOPE_STAGE); }
#endif

void DiveEggman_Serialize(void) {}
