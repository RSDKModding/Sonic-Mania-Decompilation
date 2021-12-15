// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HeavyMystic Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHeavyMystic *HeavyMystic;

void HeavyMystic_Update(void)
{
    RSDK_THIS(HeavyMystic);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    if (self->timer2) {
        self->timer2--;
        HeavyMystic_Unknown2();
    }

    StateMachine_Run(self->state);

    self->position.x += TornadoPath->moveVel.x;
    self->position.y += TornadoPath->moveVel.y;
}

void HeavyMystic_LateUpdate(void) {}

void HeavyMystic_StaticUpdate(void)
{
    foreach_active(HeavyMystic, mystic)
    {
        if (mystic->type == MYSTIC_BOX) {
            RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(mystic));
        }
    }
}

void HeavyMystic_Draw(void)
{
    RSDK_THIS(HeavyMystic);
    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        if (self->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 158, 0xE0E0E0);
        RSDK.DrawSprite(&self->animator, NULL, false);
        RSDK.SetPaletteEntry(0, 158, 0x000000);
    }
}

void HeavyMystic_Create(void *data)
{
    RSDK_THIS(HeavyMystic);
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            if (data)
                self->type = voidToInt(data);
            switch (self->type) {
                case MYSTIC_MISCHIEF:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = false;
                    self->drawOrder = Zone->drawOrderLow + 2;
                    self->drawFX    = FX_FLIP;
                    self->health    = 6;
                    RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
                    self->state = HeavyMystic_State0_Unknown1;
                    break;
                case MYSTIC_BOSS:
                    if (globals->gameMode >= MODE_TIMEATTACK) {
                        destroyEntity(self);
                    }
                    else {
                        self->active              = ACTIVE_BOUNDS;
                        self->visible             = false;
                        self->drawOrder           = Zone->drawOrderLow + 1;
                        self->drawFX              = FX_FLIP;
                        self->hitbox.left         = -22;
                        self->hitbox.top          = -22;
                        self->hitbox.right        = 22;
                        self->hitbox.bottom       = 22;
                        self->health              = 8;
                        HeavyMystic->curtainLinePos = 0xD00000;
                        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
                        self->state = HeavyMystic_State1_Unknown1;
                    }
                    break;
                case MYSTIC_BOX:
                    if (globals->gameMode >= MODE_TIMEATTACK) {
                        destroyEntity(self);
                    }
                    else {
                        self->scale.x   = 0x200;
                        self->scale.y   = 0x200;
                        self->active    = ACTIVE_BOUNDS;
                        self->visible   = true;
                        self->drawOrder = Zone->drawOrderLow;
                        self->targetPos  = self->position;
                        self->drawFX    = FX_SCALE | FX_ROTATE | FX_FLIP;
                        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &self->animator, true, 0);
                        self->state     = HeavyMystic_State2_Unknown1;
                        self->stateDraw = HeavyMystic_StateDraw2_Unknown1;
                    }
                    break;
                case MYSTIC_CORK:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawOrder = Zone->drawOrderLow;
                    self->drawFX    = FX_FLIP;
                    RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 4, &self->animator, true, 0);
                    self->state               = HeavyMystic_State3_Unknown1;
                    self->hitbox.left = -3;
                    self->hitbox.top = -3;
                    self->hitbox.right = 3;
                    self->hitbox.bottom = 3;
                    break;
                case MYSTIC_BOMB:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawOrder = Zone->drawOrderLow;
                    self->drawFX    = FX_FLIP;
                    RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 11, &self->animator, true, 0);
                    self->state         = HeavyMystic_State4_Unknown1;
                    self->hitbox.left   = -8;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 8;
                    break;
                case MYSTIC_DEBRIS:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawOrder = Zone->drawOrderLow;
                    self->drawFX    = FX_FLIP;
                    RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 16, &self->animator, true, 0);
                    self->state         = HeavyMystic_State5_Unknown1;
                    self->hitbox.left   = -8;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 8;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void HeavyMystic_StageLoad(void)
{
    HeavyMystic->aniFrames      = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);
    HeavyMystic->rouguesFrames  = RSDK.LoadSpriteAnimation("MSZ/Rogues.bin", SCOPE_STAGE);
    HeavyMystic->active         = ACTIVE_ALWAYS;
    HeavyMystic->curtainLinePos = 0;
    HeavyMystic->sfxHit         = RSDK.GetSfx("Stage/BossHit.wav");
    HeavyMystic->sfxExplosion   = RSDK.GetSfx("Stage/Explosion2.wav");
    HeavyMystic->sfxMagicBox    = RSDK.GetSfx("MSZ/MagicBox.wav");
    HeavyMystic->sfxClack       = RSDK.GetSfx("Stage/Clack.wav");
    HeavyMystic->sfxImpact      = RSDK.GetSfx("Stage/Impact3.wav");
    HeavyMystic->sfxPowerDown   = RSDK.GetSfx("Stage/PowerDown.wav");
    HeavyMystic->sfxPon         = RSDK.GetSfx("Stage/Pon.wav");
    HeavyMystic->sfxDrop        = RSDK.GetSfx("Stage/Drop.wav");
    HeavyMystic->sfxImpact2     = RSDK.GetSfx("Stage/Impact3.wav");
    HeavyMystic->sfxAssemble    = RSDK.GetSfx("Stage/Assemble.wav");
    HeavyMystic->sfxPoof        = RSDK.GetSfx("MSZ/MysticPoof.wav");
    HeavyMystic->sfxTwinkle     = RSDK.GetSfx("MSZ/MysticTwinkle.wav");
    HeavyMystic->sfxHat         = RSDK.GetSfx("MSZ/MysticHat.wav");
    HeavyMystic->sfxHatNode     = RSDK.GetSfx("MSZ/MysticHatNode.wav");
    HeavyMystic->sfxBleeps      = RSDK.GetSfx("MSZ/MysticBleeps.wav");
    HeavyMystic->sfxAppear1     = RSDK.GetSfx("MSZ/MysticAppearAct1.wav");
    HeavyMystic->sfxTransform   = RSDK.GetSfx("MSZ/MysticTransform.wav");
}

void HeavyMystic_Unknown1(int32 x, int32 y)
{
    int32 velX = RSDK.Rand(-0xC000, 0xC000);
    int32 velY = RSDK.Rand(-0xC000, 0xC000);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
    debris->drawOrder    = Zone->drawOrderLow;
    debris->drawFX       = FX_FLIP;
    debris->direction    = RSDK.Rand(0, 4);
    debris->velocity.x   = velX;
    debris->velocity.y   = velY;
    debris->inkEffect    = INK_SUB;
    debris->alpha        = 0xE0;
    RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, RSDK.Rand(0, 2) + 5, &debris->animator, true, 0);
    debris->timer = 3 * debris->animator.frameCount;

    debris             = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
    debris->velocity.x = velX;
    debris->velocity.y = velY;
    debris->drawOrder  = Zone->drawOrderLow;
    debris->inkEffect  = INK_ADD;
    debris->alpha      = 0xFF;
    RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, RSDK.Rand(0, 2) + 7, &debris->animator, true, 0);
    debris->timer = 3 * debris->animator.frameCount;
}

void HeavyMystic_Unknown2(void)
{
    RSDK_THIS(HeavyMystic);
    if (!(Zone->timer & 7)) {
        HeavyMystic_Unknown1(self->position.x + RSDK.Rand(-0x200000, -0x100000), self->position.y + RSDK.Rand(-0x100000, 0));
        HeavyMystic_Unknown1(self->position.x + RSDK.Rand(0x100000, 0x200000), self->position.y + RSDK.Rand(-0x100000, 0));
    }
}

void HeavyMystic_CheckPlayerCollisions(void)
{
    RSDK_THIS(HeavyMystic);
    if (!self->timer2 && !self->invincibilityTimer) {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
                HeavyMystic_Hit();
                foreach_break;
            }
        }
    }
}

void HeavyMystic_Hit(void)
{
    RSDK_THIS(HeavyMystic);
    --self->health;
    --self->rougeHealth;
    if (self->health <= 0) {
        RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);
        self->timer               = 120;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        self->state = HeavyMystic_State_Destroyed;
    }
    else {
        self->timer -= 30;
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(HeavyMystic->sfxHit, false, 255);
    }
}

void HeavyMystic_CheckPlayerCollisions2(void)
{
    RSDK_THIS(HeavyMystic);
    if (!self->timer2) {
        if (self->invincibilityTimer > 0)
            self->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
                RSDK.PlaySfx(HeavyMystic->sfxImpact, false, 255);
                if (player->position.x < self->position.x)
                    self->velocity.x = 0x20000;
                else
                    self->velocity.x = -0x20000;
                self->velocity.y = -0x40000;
                RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 3, &self->animator, true, 0);
                self->state = HeavyMystic_State_RougeHit;
            }
        }
    }
}

void HeavyMystic_CheckPlayerCollisions3(void)
{
    RSDK_THIS(HeavyMystic);
    if (!self->timer2) {
        if (self->invincibilityTimer > 0)
            self->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox)) {
#if RETRO_USE_PLUS

                if (self->animator.animationID == ANI_SKIDTURN
                    && ((self->direction == FLIP_NONE && player->position.x > self->position.x)
                        || (self->direction == FLIP_X && player->position.x < self->position.x))) {
                    if (!Player_CheckMightyUnspin(0x600, player, false, &player->uncurlTimer))
                        Player_CheckHit(player, self);
                }
                else
#endif
                    if (Player_CheckBossHit(player, self)) {
                    RSDK.PlaySfx(HeavyMystic->sfxImpact, false, 255);
                    if (self->invincibilityTimer > 0)
                        self->invincibilityTimer--;
                    RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 15, &self->animator, true, 0);
                    self->state = HeavyMystic_State_RougeHit;
                }
            }
        }
    }
}

void HeavyMystic_CheckPlayerCollisions4(void)
{
    RSDK_THIS(HeavyMystic);
    if (!self->timer2) {

        if (self->invincibilityTimer > 0)
            self->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
                RSDK.PlaySfx(HeavyMystic->sfxImpact, false, 255);

                if (player->position.x < self->position.x)
                    self->velocity.x = 0x20000;
                else
                    self->velocity.x = -0x20000;
                self->velocity.y = -0x40000;
                RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 9, &self->animator, true, 0);
                self->state = HeavyMystic_State_RougeHit;
            }
        }
    }
}

void HeavyMystic_Explode(void)
{
    RSDK_THIS(HeavyMystic);
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

void HeavyMystic_ScanlineCB(ScanlineInfo *scanlines)
{
    int curtainPos = HeavyMystic->curtainLinePos;
    int move       = (0x1100000 - curtainPos) >> 6;
    int max        = maxVal(curtainPos - 0x400000, 0x100000);

    int posY = (ScreenInfo->position.y + 207) << 16;
    if (curtainPos > max) {
        int id = HeavyMystic->curtainLinePos >> 16;

        for (int i = 0; i < ((curtainPos - max - 1) >> 16) + 1; ++i) {
            scanlines[id--].position.y = posY;
            posY -= move;
        }
    }

    for (int i = HeavyMystic->curtainLinePos >> 16; i < 208; ++i) {
        scanlines[i].position.y = 0x6200000;
    }
}

void HeavyMystic_State0_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    RSDK.ProcessAnimation(&self->animator);
    if (++self->timer >= 8) {
        self->velocity.x = 0x20000;
        //I'd prolly put a position check here too, its not hard to go into the special stage, then not hit the starpost, doing so causes a softlock
        if (StarPost->postIDs[0])
            HeavyMystic->curtainLinePos = 1;
        self->targetPos.x = self->position.x;
        self->targetPos.y = self->position.y;
        self->position.y += 0xC00000;
        self->visible    = true;
        self->timer      = 120;
        self->state      = HeavyMystic_State0_Unknown2;
    }
}

void HeavyMystic_State1_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    if (++self->timer >= 8) {
        self->timer        = 0;
        HeavyMystic->boundsL = (Zone->cameraBoundsL[0] + 64) << 16;
        HeavyMystic->boundsR = (Zone->cameraBoundsR[0] - 64) << 16;
        HeavyMystic->boundsM = self->position.x;
        HeavyMystic->boundsT = (Zone->cameraBoundsT[0] + 48) << 16;
        HeavyMystic->boundsB = (Zone->cameraBoundsB[0] - 8) << 16;
        self->state        = HeavyMystic_State1_Unknown2;
    }
}

void HeavyMystic_State1_Unknown2(void)
{
    RSDK_THIS(HeavyMystic);

    if (self->timer) {
        self->timer++;
        if (self->timer == 120) {
            Zone->cameraBoundsT[0] = Zone->cameraBoundsB[0] - ScreenInfo->height;
            MSZSetup_Unknown1(self->position.x + 0x4000000, 0);
            self->position.y += 0x500000;
            self->targetPos.x                                 = self->position.x;
            self->targetPos.y                                 = self->position.y;
            self->timer                                      = 0;
            self->inkEffect                                  = INK_ALPHA;
            self->visible                                    = true;
            self->state                                      = HeavyMystic_State1_Unknown3;
            RSDK.GetSceneLayer(Zone->fgHigh)->scanlineCallback = HeavyMystic_ScanlineCB;

            foreach_active(MSZSpotlight, spotlight) { spotlight->state = MSZSpotlight_Unknown1; }
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.y > self->position.y) {
            Music_TransitionTrack(TRACK_HBHBOSS, 0.0125);
            Zone->playerBoundActiveL[0] = true;
            Zone->playerBoundActiveR[0] = true;
            Zone->cameraBoundsL[0]     = (self->position.x >> 16) - ScreenInfo->centerX;
            Zone->cameraBoundsR[0]     = (self->position.x >> 16) + ScreenInfo->centerX;
            Zone->cameraBoundsB[0]     = (self->position.y >> 16) + 256;
            ++self->timer;
            self->active = ACTIVE_NORMAL;
        }
    }
}

void HeavyMystic_State0_Unknown2(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();

    if (self->timer > 0) {
        self->timer--;
    }
    else {
        self->velocity.y -= 0x1800;
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
    }
    if (self->timer == 50)
        RSDK.PlaySfx(HeavyMystic->sfxAppear1, false, 255);

    if (self->position.y <= self->targetPos.y)
        self->state = HeavyMystic_State0_Unknown3;
}

void HeavyMystic_State0_Unknown3(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();
    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->position.y >= self->targetPos.y) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->position.y = self->targetPos.y;
        RSDK.PlaySfx(HeavyMystic->sfxBleeps, false, 255);
        self->state = HeavyMystic_State0_Unknown4;
    }
}

void HeavyMystic_State1_Unknown3(void)
{
    RSDK_THIS(HeavyMystic);

    if (self->alpha > 0xC0)
        HeavyMystic_Unknown2();

#if RETRO_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        if (self->timer < 256) {
            self->timer += 3;
            RSDK.SetLimitedFade(0, 1, 5, self->timer, 128, 255);
        }
    }
#endif

    if (HeavyMystic->curtainLinePos <= 0x100000) {
        self->timer     = 0;
        self->inkEffect = INK_NONE;
        self->state     = HeavyMystic_State1_Unknown4;
    }
    else {
        HeavyMystic->curtainLinePos -= 0x20000;
    }
}

void HeavyMystic_State0_Unknown7(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    HeavyMystic_Unknown2();
    HeavyMystic_Unknown2();
}

void HeavyMystic_State0_Unknown4(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    self->position.x += 0x8000;

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    HeavyMystic_Unknown2();
    HeavyMystic_Unknown2();

    if (++self->timer == 90) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 1, &self->animator, true, 0);
        RSDK.PlaySfx(HeavyMystic->sfxHat, false, 255);
        self->state = HeavyMystic_State0_Unknown5;
    }
}

void HeavyMystic_State0_Unknown9(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();
    HeavyMystic_Unknown2();

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 3, &self->animator, true, 0);
        RSDK.PlaySfx(HeavyMystic->sfxTransform, false, 255);
        self->state = HeavyMystic_State0_Unknown10;
    }
}

void HeavyMystic_State0_Unknown6(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();
    HeavyMystic_Unknown2();

    if (++self->timer == 60) {
        if (RSDK.CheckStageFolder("MSZCutscene")) {
            self->state = HeavyMystic_State0_Unknown7;
        }
        else {
            self->timer = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 2, &self->animator, true, 0);
            RSDK.PlaySfx(HeavyMystic->sfxHat, false, 255);
            self->state = HeavyMystic_State0_Unknown8;
        }
    }
}

void HeavyMystic_State0_Unknown5(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
        self->state = HeavyMystic_State0_Unknown6;
    }
}

void HeavyMystic_State0_Unknown8(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();
    if (self->animator.frameID == 8 && !self->timer) {
        RSDK.PlaySfx(HeavyMystic->sfxHatNode, false, 255);
        CREATE_ENTITY(Hatterkiller, intToVoid(RSDK.Rand(-0x20000, 0x20000)), self->position.x + 0x320000, self->position.y);
        self->timer = 1;
    }

    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
        self->state = HeavyMystic_State0_Unknown9;
    }
}

void HeavyMystic_State0_Unknown10(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();

    if (self->animator.frameID == self->animator.frameCount - 1) {
        if (!HeavyMystic->curtainLinePos) {
            HeavyMystic->curtainLinePos = 1;
            CREATE_ENTITY(Vultron, intToVoid(1), self->position.x, self->position.y);
            CREATE_ENTITY(Vultron, intToVoid(1), self->position.x, self->position.y);
        }
        RSDK.PlaySfx(HeavyMystic->sfxPoof, false, 255);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
        destroyEntity(self);
    }
}

void HeavyMystic_State_Destroyed(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Explode();
    if (--self->timer <= 0) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
        self->state = HeavyMystic_State_Finish;
    }
}

void HeavyMystic_State_Finish(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->state = HeavyMystic_State_CloseCurtains;
    }
}

void HeavyMystic_State_CloseCurtains(void)
{
    RSDK_THIS(HeavyMystic);

#if RETRO_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        if (self->timer < 256) {
            self->timer += 3;
            RSDK.SetLimitedFade(0, 5, 1, self->timer, 128, 255);
        }
    }
#endif

    if (HeavyMystic->curtainLinePos >= 0xD00000) {
        HeavyMystic->curtainLinePos                        = 0xD00000;
        RSDK.GetSceneLayer(Zone->fgHigh)->scanlineCallback = 0;
        Zone->cameraBoundsR[0] += 0x350;
        destroyEntity(self);
    }
    else {
        HeavyMystic->curtainLinePos += 0x20000;
    }
}

void HeavyMystic_State1_Unknown4(void) {}

void HeavyMystic_State1_Unknown5(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    HeavyMystic_Unknown2();
    HeavyMystic_Unknown2();

    if (++self->timer == 45) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 1, &self->animator, true, 0);
        self->state = HeavyMystic_State1_Unknown6;
    }
}

void HeavyMystic_State1_Unknown6(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();
    if (self->animator.frameID == self->animator.frameCount - 8)
        RSDK.PlaySfx(HeavyMystic->sfxHat, false, 255);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
        self->state = HeavyMystic_State1_Unknown7;
    }
}

void HeavyMystic_State1_Unknown7(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    if (self->angle == 192) {
        foreach_active(HeavyMystic, boss)
        {
            if (boss->type == MYSTIC_BOX)
                boss->state = HeavyMystic_State2_Unknown4;
        }
        RSDK.PlaySfx(HeavyMystic->sfxClack, false, 255);
        self->state = HeavyMystic_State1_Unknown8;
    }
}

void HeavyMystic_State1_Unknown8(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    if (self->angle == 252)
        self->state = HeavyMystic_State1_Unknown9;
}

void HeavyMystic_State1_Unknown9(void)
{
    RSDK_THIS(HeavyMystic);

    if (++self->timer == 60) {
        self->timer   = 0;
        self->visible = false;

        foreach_active(HeavyMystic, boss)
        {
            if (boss->type == MYSTIC_BOX)
                boss->state = HeavyMystic_State2_Unknown5;
        }
        self->state = HeavyMystic_State1_Unknown10;
    }
}

void HeavyMystic_State1_Unknown10(void)
{
    RSDK_THIS(HeavyMystic);

    foreach_active(HeavyMystic, boss)
    {
        if (boss->type == MYSTIC_BOX && boss->state == HeavyMystic_State2_Unknown8) {
            self->position.x = boss->position.x;
            self->position.y = boss->position.y;
            boss->position     = boss->targetPos;
            self->visible    = 1;
            self->state      = HeavyMystic_State1_ShowRouge;
        }
    }
}

void HeavyMystic_State1_ShowRouge(void)
{
    RSDK_THIS(HeavyMystic);
    self->velocity.x = 0;
    self->velocity.y = 0;
    self->rougeHealth   = 2;
    RSDK.PlaySfx(HeavyMystic->sfxTwinkle, false, 255);

    self->velocity.y = -0x40000;
    self->timer2     = 75;
    switch (self->rougeID) {
        case 0:
        case 3:
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 2, &self->animator, true, 4);
            self->state    = HeavyMystic_State1_Unknown12;
            self->field_7C = 8;
            break;
        case 1:
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 6, &self->animator, true, 3);
            self->state = HeavyMystic_State1_Unknown15;
            break;
        case 2:
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 13, &self->animator, true, 3);
            self->state = HeavyMystic_State1_Unknown19;
            break;
        default: break;
    }
}

void HeavyMystic_State1_MysticReveal(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    HeavyMystic_CheckPlayerCollisions();
    if (--self->timer <= 0) {
        foreach_active(HeavyMystic, boss)
        {
            if (boss->type == MYSTIC_BOX) {
                self->targetPos.x = boss->position.x;
                self->targetPos.y = boss->position.y;
            }
        }

        if (self->rougeHealth > 0) {
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 3, &self->animator, true, 0);
            self->state = HeavyMystic_State1_Unknown24;
        }
        else {
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
            self->state = HeavyMystic_State1_Unknown22;
        }
    }
}

void HeavyMystic_State1_Unknown22(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();

    if (self->position.x != self->targetPos.x) {
        if (self->position.x <= self->targetPos.x) {
            self->position.x += self->velocity.x;
            if (self->position.x > self->targetPos.x)
                self->position.x = self->targetPos.x;
            else if (self->velocity.x < 0x30000)
                self->velocity.x += 0x3800;
        }
        else {
            self->position.x += self->velocity.x;
            if (self->position.x < self->targetPos.x)
                self->position.x = self->targetPos.x;
            else if (self->velocity.x > -0x30000)
                self->velocity.x -= 0x3800;
        }
    }

    if (self->position.y <= self->targetPos.y) {
        self->position.y = self->targetPos.y;
        self->angle      = 128;
        if (self->position.x == self->targetPos.x) {
            ++self->rougeID;
            self->state = HeavyMystic_State1_Unknown7;
        }
        else {
            self->state = HeavyMystic_State1_Unknown23;
        }
    }
    else {
        self->position.y += self->velocity.y;
        if (self->velocity.y > -0x20000)
            self->velocity.y -= 0x2000;
    }
}

void HeavyMystic_State1_Unknown23(void)
{
    RSDK_THIS(HeavyMystic);
    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    if (self->position.x == self->targetPos.x) {
        ++self->rougeID;
        self->position.y = self->targetPos.y;
        self->state      = HeavyMystic_State1_Unknown7;
    }
    else if (self->position.x > self->targetPos.x) {
        self->position.x += self->velocity.x;
        if (self->position.x >= self->targetPos.x) {
            if (self->velocity.x > -0x30000)
                self->velocity.x -= 0x3800;
        }
        else
            self->position.x = self->targetPos.x;
    }
    else {
        self->position.x += self->velocity.x;
        if (self->position.x > self->targetPos.x)
            self->position.x = self->targetPos.x;
        else if (self->velocity.x < 0x30000)
            self->velocity.x += 0x3800;
    }
}

void HeavyMystic_State1_Unknown24(void)
{
    RSDK_THIS(HeavyMystic);
    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_Unknown2();
    if (self->animator.frameID == self->animator.frameCount - 5)
        RSDK.PlaySfx(HeavyMystic->sfxTransform, false, 255);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
        self->velocity.x                                                                                     = 0;
        self->velocity.y                                                                                     = -0x20000;
        RSDK.PlaySfx(HeavyMystic->sfxPoof, false, 255);

        self->velocity.y = -0x40000;
        self->timer2     = 75;
        switch (self->rougeID) {
            case 0:
            case 3:
                RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 2, &self->animator, true, 4);
                self->state    = HeavyMystic_State1_Unknown12;
                self->field_7C = 8;
                break;
            case 1:
                RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 6, &self->animator, true, 3);
                self->state = HeavyMystic_State1_Unknown15;
                break;
            case 2:
                RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 13, &self->animator, true, 3);
                self->state = HeavyMystic_State1_Unknown19;
                break;
            default: break;
        }
    }
}

void HeavyMystic_State1_Unknown13(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK.ProcessAnimation(&self->animator);
    self->direction = player1->position.x < self->position.x;
    if (!--self->timer) {
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 1, &self->animator, true, 0);
        self->state = HeavyMystic_State1_Unknown14;
    }
    HeavyMystic_CheckPlayerCollisions2();
}

void HeavyMystic_State1_Unknown14(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->velocity.y = -0x50000;
        self->field_7C   = 8;
        self->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 2, &self->animator, true, 0);
        self->state = HeavyMystic_State1_Unknown12;
    }
    HeavyMystic_CheckPlayerCollisions2();
}

void HeavyMystic_State1_Unknown12(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->direction = player1->position.x < self->position.x;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.x >= 0) {
        if (self->position.x > (ScreenInfo->position.x + ScreenInfo->width - 16) << 16)
            self->velocity.x = -self->velocity.x;
    }
    else if (self->position.x < (ScreenInfo->position.x + 16) << 16)
        self->velocity.x = -self->velocity.x;

    RSDK.ProcessAnimation(&self->animator);
    if (self->velocity.y > 0 && RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x140000, true)) {
        if (--self->field_7C > 0) {
            self->velocity.y = -0x50000;
            self->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 2, &self->animator, true, 0);
            self->state = HeavyMystic_State1_Unknown12;
        }
        else {
            self->timer      = 15;
            self->velocity.x = 0;
            self->velocity.y = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 0, &self->animator, true, 0);
            self->state = HeavyMystic_State1_Unknown13;
        }
    }

    if (self->field_7C && !(Zone->timer & 0x3F)) {
        RSDK.PlaySfx(HeavyMystic->sfxPon, false, 255);
        EntityHeavyMystic *boss = CREATE_ENTITY(HeavyMystic, intToVoid(MYSTIC_CORK), self->position.x, self->position.y - 0x40000);
        if (self->direction == FLIP_NONE) {
            boss->position.x += 0x180000;
            boss->velocity.x = 0x20000;
            boss->direction  = self->direction;
        }
        else {
            boss->position.x -= 0x180000;
            boss->velocity.x = -0x20000;
            boss->direction  = self->direction;
        }
    }
    HeavyMystic_CheckPlayerCollisions2();
}

void HeavyMystic_State_RougeHit(void)
{
    RSDK_THIS(HeavyMystic);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.x >= 0) {
        if (self->position.x > (ScreenInfo->position.x + ScreenInfo->width - 16) << 16)
            self->velocity.x = 0;
    }
    else if (self->position.x < (ScreenInfo->position.x + 16) << 16)
        self->velocity.x = 0;

    self->visible ^= 1;
    RSDK.ProcessAnimation(&self->animator);
    if (self->velocity.y > 0) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x240000, true)) {
            RSDK.PlaySfx(HeavyMystic->sfxPowerDown, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
            self->direction ^= FLIP_X;
            self->timer      = 90;
            self->field_7C   = 0;
            self->visible    = true;
            self->targetPos.y = self->position.y;
            self->velocity.x = 0;
            self->velocity.y = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 4, &self->animator, true, 0);
            self->state = HeavyMystic_State1_MysticReveal;
        }
    }
}

void HeavyMystic_State1_Unknown20(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK.ProcessAnimation(&self->animator);
    self->direction = player1->position.x < self->position.x;
    if (!--self->timer) {
        self->velocity.y = -0x40000;
        self->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
        if (self->field_7C == 1) {
            self->timer = 240;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 14, &self->animator, true, 0);
            self->state = HeavyMystic_State1_Unknown21;
        }
        else {
            self->field_7C = 1;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 13, &self->animator, true, 0);
            self->state = HeavyMystic_State1_Unknown19;
        }
    }
    HeavyMystic_CheckPlayerCollisions3();
}

void HeavyMystic_State1_Unknown21(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == 3 || self->animator.frameID == 10) {
        if (self->field_7C != self->animator.frameID)
            RSDK.PlaySfx(HeavyMystic->sfxImpact2, false, 255);
        self->field_7C = self->animator.frameID;
    }
    if (--self->timer <= 0) {
        self->field_7C   = 0;
        self->timer      = 15;
        self->velocity.x = 0;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 12, &self->animator, true, 0);
        self->state = HeavyMystic_State1_Unknown20;
    }
    if (self->field_7C != 1) {
        if (!(Zone->timer & 3))
            Camera_ShakeScreen(0, 0, 2);

        if (!(Zone->timer & 0xF)) {
            int x = RSDK.Rand(0, ScreenInfo->width >> 5);
            EntityHeavyMystic *boss = CREATE_ENTITY(HeavyMystic, intToVoid(MYSTIC_DEBRIS), (ScreenInfo->position.x + 16 + 32 * x) << 16,
                                                    (ScreenInfo->position.y - 16) << 16);
            boss->animator.frameID = RSDK.Rand(0, 6);
        }
    }
    HeavyMystic_CheckPlayerCollisions3();
}

void HeavyMystic_State1_Unknown19(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->direction = player1->position.x < self->position.x;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.x >= 0) {
        if (self->position.x > (ScreenInfo->position.x + ScreenInfo->width - 16) << 16)
            self->velocity.x = -self->velocity.x;
    }
    else if (self->position.x < (ScreenInfo->position.x + 16) << 16)
        self->velocity.x = -self->velocity.x;

    RSDK.ProcessAnimation(&self->animator);
    if (self->velocity.y > 0 && RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x200000, true)) {
        self->timer = 15;
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 12, &self->animator, true, 0);
        self->state = HeavyMystic_State1_Unknown20;
    }
    HeavyMystic_CheckPlayerCollisions3();
}

void HeavyMystic_State1_Unknown16(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->direction = player1->position.x < self->position.x;
    RSDK.ProcessAnimation(&self->animator);

    if (!--self->timer) {
        self->velocity.y = -0x68000;
        self->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x20000 : 0x20000;
        if (self->field_7C == 1) {
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 8, &self->animator, true, 0);
            self->state = HeavyMystic_State1_Unknown17;
        }
        else {
            self->field_7C = 1;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 6, &self->animator, true, 0);
            self->state = HeavyMystic_State1_Unknown15;
        }
    }
    HeavyMystic_CheckPlayerCollisions4();
}

void HeavyMystic_State1_Unknown17(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    if (self->field_7C == 1 && self->animator.frameID == 3) {
        self->field_7C = 2;
        RSDK.PlaySfx(HeavyMystic->sfxDrop, false, 255);
        EntityHeavyMystic *boss = CREATE_ENTITY(HeavyMystic, intToVoid(MYSTIC_BOMB), self->position.x, self->position.y + 0x130000);
        if (self->direction) {
            boss->position.x += 0xB0000;
            boss->velocity.x = -0x40000;
        }
        else {
            boss->position.x -= 0xB0000;
            boss->velocity.x = 0x40000;
        }
        boss->velocity.y = -0x40000;
        boss->direction  = self->direction;
    }
    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->field_7C = 1;
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 7, &self->animator, true, 0);
        self->state = HeavyMystic_State1_Unknown18;
    }
    HeavyMystic_CheckPlayerCollisions4();
}

void HeavyMystic_State1_Unknown18(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    if (self->field_7C == 1 && self->animator.frameID == 2) {
        self->field_7C = 2;
        RSDK.PlaySfx(HeavyMystic->sfxDrop, false, 255);
        EntityHeavyMystic *boss = CREATE_ENTITY(HeavyMystic, intToVoid(MYSTIC_BOMB), self->position.x, self->position.y - 0x130000);
        if (self->direction) {
            boss->position.x += 0xB0000;
            boss->velocity.x = -0x40000;
        }
        else {
            boss->position.x -= 0xB0000;
            boss->velocity.x = 0x40000;
        }
        boss->velocity.y = -0x40000;
        boss->direction  = self->direction;
    }
    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->field_7C   = 0;
        self->timer      = 5;
        self->velocity.x = 0;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 5, &self->animator, true, 0);
        self->state = HeavyMystic_State1_Unknown16;
    }
    HeavyMystic_CheckPlayerCollisions4();
}

void HeavyMystic_State1_Unknown15(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->direction = player1->position.x < self->position.x;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.x >= 0) {
        if (self->position.x > (ScreenInfo->position.x + ScreenInfo->width - 16) << 16)
            self->velocity.x = -self->velocity.x;
    }
    else if (self->position.x < (ScreenInfo->position.x + 16) << 16)
        self->velocity.x = -self->velocity.x;

    RSDK.ProcessAnimation(&self->animator);
    if (self->velocity.y > 0 && RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x140000, true) == true) {
        if (--self->field_7C <= 0) {
            self->field_7C   = 1;
            self->timer      = 15;
            self->velocity.x = 0;
            self->velocity.y = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 5, &self->animator, true, 0);
            self->state = HeavyMystic_State1_Unknown16;
        }
        else {
            self->velocity.y = -0x50000;
            self->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 6, &self->animator, true, 0);
        }
    }
    HeavyMystic_CheckPlayerCollisions4();
}

void HeavyMystic_State3_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    RSDK.ProcessAnimation(&self->animator);
    self->position.x += self->velocity.x;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
#if RETRO_USE_PLUS
            if (Player_CheckMightyShellHit(player, self, -0x300, -0x400))
                self->state = HeavyMystic_State3_Unknown2;
            else 
#endif
                Player_CheckHit(player, self);
            RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh + 2;
            destroyEntity(self);
            foreach_break;
        }
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void HeavyMystic_State3_Unknown2(void)
{
    RSDK_THIS(HeavyMystic);

    self->visible ^= 1;
    RSDK.ProcessAnimation(&self->animator);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void HeavyMystic_State4_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x300, player, false, &player->uncurlTimer))
#endif
                Player_CheckHit(player, self);
            RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh + 2;
            destroyEntity(self);
            foreach_break;
        }
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void HeavyMystic_State5_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    RSDK.ProcessAnimation(&self->animator);
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x300, player, false, &player->uncurlTimer))
#endif
                Player_CheckHit(player, self);
        }
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void HeavyMystic_State2_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    self->timer = 2;
    if (HeavyMystic->curtainLinePos <= 0x100000) {
        self->state     = HeavyMystic_State2_Unknown2;
        self->stateDraw = HeavyMystic_StateDraw2_Unknown2;
    }
}

void HeavyMystic_State2_Unknown2(void) {}

void HeavyMystic_State2_Unknown4(void)
{
    RSDK_THIS(HeavyMystic);
    self->scale.x -= 32;
    if (self->scale.x <= 0) {
        if (self->timer == 2) {
            foreach_active(HeavyMystic, boss)
            {
                if (boss->state == HeavyMystic_State1_Unknown4)
                    boss->state = HeavyMystic_State1_Unknown5;
            }

            self->timer     = 1;
            self->stateDraw = HeavyMystic_StateDraw2_Unknown1;
            self->state     = HeavyMystic_State2_Unknown3;
        }
        else {
            self->timer     = 2;
            self->stateDraw = HeavyMystic_StateDraw2_Unknown2;
            self->state     = HeavyMystic_State2_Unknown3;
        }
    }
}

void HeavyMystic_State2_Unknown3(void)
{
    RSDK_THIS(HeavyMystic);

    self->scale.x += 32;
    if (self->scale.x >= 512)
        self->state = HeavyMystic_State2_Unknown2;
}

void HeavyMystic_State2_Unknown5(void)
{
    RSDK_THIS(HeavyMystic);
    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(HeavyMystic->sfxMagicBox, false, 255);
        int x = RSDK.Rand(-24, 25) << 16;
        int y = RSDK.Rand(-24, 25) << 16;
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), x + self->position.x, y + self->position.y)->drawOrder = Zone->drawOrderHigh + 2;
    }

    self->scale.x = RSDK.Cos256(self->angle + 64) + 0x200;
    self->scale.y = RSDK.Sin256(self->angle) + 0x200;
    if (self->angle == 768) {
        RSDK.StopSfx(HeavyMystic->sfxMagicBox);
        RSDK.PlaySfx(HeavyMystic->sfxPowerDown, false, 255);
        self->angle     = 0;
        self->timer     = 0;
        self->state     = HeavyMystic_State2_Unknown6;
        self->stateDraw = HeavyMystic_StateDraw2_Unknown3;
    }
    else {
        self->angle += 8;
    }
}

void HeavyMystic_State2_Unknown6(void)
{
    RSDK_THIS(HeavyMystic);
    if (++self->timer > 30) {
        RSDK.PlaySfx(HeavyMystic->sfxDrop, false, 255);
        self->state = HeavyMystic_State2_Unknown7;
    }
}

void HeavyMystic_State2_Unknown7(void)
{
    RSDK_THIS(HeavyMystic);
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x280000, true)) {
        Camera_ShakeScreen(0, 0, 6);
        RSDK.PlaySfx(HeavyMystic->sfxImpact2, false, 255);
        RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        debris->drawOrder    = Zone->drawOrderLow;
        debris->drawFX       = FX_FLIP;
        debris->direction    = FLIP_NONE;
        debris->velocity.y   = -0x40000;
        debris->gravity      = 0x2800;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &debris->animator, true, 0);
        debris->animator.frameID = 0;

        debris             = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x280000, self->position.y);
        debris->drawOrder  = Zone->drawOrderHigh;
        debris->drawFX     = FX_FLIP;
        debris->direction  = FLIP_NONE;
        debris->velocity.x = -0x20000;
        debris->velocity.y = -0x40000;
        debris->gravity    = 0x2800;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &debris->animator, true, 0);
        debris->animator.frameID = 2;

        debris             = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x280000, self->position.y);
        debris->drawOrder  = Zone->drawOrderHigh;
        debris->drawFX     = FX_FLIP;
        debris->direction  = FLIP_X;
        debris->velocity.x = 0x20000;
        debris->velocity.y = -0x40000;
        debris->gravity    = 0x2800;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &debris->animator, true, 0);
        debris->animator.frameID = 2;

        self->timer      = 0;
        self->visible    = 0;
        self->velocity.y = 0;
        self->state      = HeavyMystic_State2_Unknown8;
    }
}

void HeavyMystic_State2_Unknown8(void)
{
    RSDK_THIS(HeavyMystic);
    if (++self->timer > 96) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh + 2;
        self->visible                                                                                         = true;
        self->scale.x                                                                                         = 0;
        self->scale.y                                                                                         = 0;
        RSDK.PlaySfx(HeavyMystic->sfxAssemble, false, 255);
        self->stateDraw = HeavyMystic_StateDraw2_Unknown3;
        self->state     = HeavyMystic_State2_Unknown9;
    }
}

void HeavyMystic_State2_Unknown9(void)
{
    RSDK_THIS(HeavyMystic);
    if (self->scale.x < 512) {
        self->scale.y += 8;
        self->scale.x = self->scale.y;
        self->rotation -= 8;
    }
    else {
        if (self->rotation & 0x1FF) {
            self->rotation -= 8;
        }
        else {
            RSDK.PlaySfx(HeavyMystic->sfxClack, false, 255);
            self->timer     = 2;
            self->stateDraw = HeavyMystic_StateDraw2_Unknown2;
            self->state     = HeavyMystic_State2_Unknown4;
        }
        self->rotation &= 0x1FF;
    }
}

void HeavyMystic_StateDraw2_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    Vector2 drawPos;

    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        self->drawFX           = FX_NONE;
        self->animator.frameID = 0;
        RSDK.DrawSprite(&self->animator, NULL, false);

        drawPos.x                = self->position.x - 0x280000;
        drawPos.y                = self->position.y;
        self->animator.frameID = self->timer;
        self->drawFX           = FX_SCALE | FX_ROTATE | FX_FLIP;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        drawPos.x += 0x500000;
        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->direction = FLIP_NONE;
    }
}

void HeavyMystic_StateDraw2_Unknown2(void)
{
    RSDK_THIS(HeavyMystic);
    Vector2 drawPos;

    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        self->drawFX = FX_NONE;
        if (self->state != HeavyMystic_State2_Unknown5) {
            self->animator.frameID = 0;
            RSDK.DrawSprite(&self->animator, NULL, false);
        }
    }
    else {
        drawPos.y                = self->position.y;
        self->animator.frameID = self->timer;
        self->drawFX           = FX_SCALE | FX_ROTATE | FX_FLIP;
        drawPos.x                = self->position.x - 0x280000;
        if (self->state == HeavyMystic_State2_Unknown5)
            drawPos.x += 0x1400 * (0x200 - self->scale.x);
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->direction = FLIP_X;
        drawPos.x         = self->position.x + 0x280000;
        if (self->state == HeavyMystic_State2_Unknown5)
            drawPos.x += 0x1400 * (self->scale.x - 0x200);
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        self->direction = FLIP_NONE;
    }
}

void HeavyMystic_StateDraw2_Unknown3(void)
{
    RSDK_THIS(HeavyMystic);

    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        self->drawFX           = FX_SCALE | FX_ROTATE;
        self->animator.frameID = 3;
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void HeavyMystic_EditorDraw(void)
{
    RSDK_THIS(HeavyMystic);

    switch (self->type) {
        case MYSTIC_MISCHIEF:
            self->drawFX = FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
            break;
        case MYSTIC_BOSS:
            self->drawFX = FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
            break;
        case MYSTIC_BOX:
            self->drawFX = FX_SCALE | FX_ROTATE | FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &self->animator, true, 0);
            break;
        case MYSTIC_CORK:
            self->drawFX    = FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 4, &self->animator, true, 0);
            break;
        case MYSTIC_BOMB:
            self->drawFX    = FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 11, &self->animator, true, 0);
            break;
        case MYSTIC_DEBRIS:
            self->drawFX    = FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 16, &self->animator, true, 0);
            break;
        default: break;
    }

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void HeavyMystic_EditorLoad(void)
{
    HeavyMystic->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);
    HeavyMystic->rouguesFrames = RSDK.LoadSpriteAnimation("MSZ/Rogues.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(HeavyMystic, type);
    RSDK_ENUM_VAR("Mischief", MYSTIC_MISCHIEF);
    RSDK_ENUM_VAR("Boss", MYSTIC_BOSS);
    RSDK_ENUM_VAR("Box", MYSTIC_BOX);
    RSDK_ENUM_VAR("Cork", MYSTIC_CORK);
    RSDK_ENUM_VAR("Bomb", MYSTIC_BOMB);
    RSDK_ENUM_VAR("Debris", MYSTIC_DEBRIS);
}
#endif

void HeavyMystic_Serialize(void) { RSDK_EDITABLE_VAR(HeavyMystic, VAR_ENUM, type); }
