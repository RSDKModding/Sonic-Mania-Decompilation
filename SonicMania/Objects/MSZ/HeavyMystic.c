// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HeavyMystic Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHeavyMystic *HeavyMystic;

void HeavyMystic_Update(void)
{
    RSDK_THIS(HeavyMystic);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    if (self->particleFXTimer) {
        self->particleFXTimer--;
        HeavyMystic_HandleParticleFX();
    }

    StateMachine_Run(self->state);

    self->position.x += TornadoPath->moveVel.x;
    self->position.y += TornadoPath->moveVel.y;
}

void HeavyMystic_LateUpdate(void) {}

void HeavyMystic_StaticUpdate(void)
{
    foreach_active(HeavyMystic, mysticBox)
    {
        if (mysticBox->type == MYSTIC_BOX) {
            RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(mysticBox));
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
                self->type = VOID_TO_INT(data);

            switch (self->type) {
                case MYSTIC_MISCHIEF:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = false;
                    self->drawGroup = Zone->objectDrawGroup[0] + 2;
                    self->drawFX    = FX_FLIP;
                    self->health    = 6;

                    RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);

                    self->state = HeavyMystic_StateMischief_Setup;
                    break;

                case MYSTIC_BOSS:
                    if (globals->gameMode >= MODE_TIMEATTACK) {
                        destroyEntity(self);
                    }
                    else {
                        self->active    = ACTIVE_BOUNDS;
                        self->visible   = false;
                        self->drawGroup = Zone->objectDrawGroup[0] + 1;
                        self->drawFX    = FX_FLIP;

                        self->hitbox.left   = -22;
                        self->hitbox.top    = -22;
                        self->hitbox.right  = 22;
                        self->hitbox.bottom = 22;

                        self->health                = 8;
                        HeavyMystic->curtainLinePos = 0xD00000;

                        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
                        self->state = HeavyMystic_StateBoss_SetupArena;
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
                        self->drawGroup = Zone->objectDrawGroup[0];
                        self->targetPos = self->position;
                        self->drawFX    = FX_SCALE | FX_ROTATE | FX_FLIP;

                        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &self->animator, true, 0);
                        self->state     = HeavyMystic_StateBox_AwaitCurtainRise;
                        self->stateDraw = HeavyMystic_Draw_BoxOpened;
                    }
                    break;

                case MYSTIC_CORK:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];
                    self->drawFX    = FX_FLIP;

                    RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 4, &self->animator, true, 0);
                    self->state = HeavyMystic_StateCork_Fired;

                    self->hitbox.left   = -3;
                    self->hitbox.top    = -3;
                    self->hitbox.right  = 3;
                    self->hitbox.bottom = 3;
                    break;

                case MYSTIC_BOMB:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];
                    self->drawFX    = FX_FLIP;

                    RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 11, &self->animator, true, 0);
                    self->state = HeavyMystic_State_Bomb;

                    self->hitbox.left   = -8;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 8;
                    break;

                case MYSTIC_DEBRIS:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];
                    self->drawFX    = FX_FLIP;

                    RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 16, &self->animator, true, 0);
                    self->state = HeavyMystic_State_BarkDebris;

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
    HeavyMystic->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);
    HeavyMystic->roguesFrames = RSDK.LoadSpriteAnimation("MSZ/Rogues.bin", SCOPE_STAGE);

    HeavyMystic->active         = ACTIVE_ALWAYS;
    HeavyMystic->curtainLinePos = 0;

    HeavyMystic->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    HeavyMystic->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    HeavyMystic->sfxMagicBox  = RSDK.GetSfx("MSZ/MagicBox.wav");
    HeavyMystic->sfxClack     = RSDK.GetSfx("Stage/Clack.wav");
    HeavyMystic->sfxImpact    = RSDK.GetSfx("Stage/Impact3.wav");
    HeavyMystic->sfxPowerDown = RSDK.GetSfx("Stage/PowerDown.wav");
    HeavyMystic->sfxPon       = RSDK.GetSfx("Stage/Pon.wav");
    HeavyMystic->sfxDrop      = RSDK.GetSfx("Stage/Drop.wav");
    HeavyMystic->sfxImpact2   = RSDK.GetSfx("Stage/Impact3.wav");
    HeavyMystic->sfxAssemble  = RSDK.GetSfx("Stage/Assemble.wav");
    HeavyMystic->sfxPoof      = RSDK.GetSfx("MSZ/MysticPoof.wav");
    HeavyMystic->sfxTwinkle   = RSDK.GetSfx("MSZ/MysticTwinkle.wav");
    HeavyMystic->sfxHat       = RSDK.GetSfx("MSZ/MysticHat.wav");
    HeavyMystic->sfxHatNode   = RSDK.GetSfx("MSZ/MysticHatNode.wav");
    HeavyMystic->sfxBleeps    = RSDK.GetSfx("MSZ/MysticBleeps.wav");
    HeavyMystic->sfxAppear1   = RSDK.GetSfx("MSZ/MysticAppearAct1.wav");
    HeavyMystic->sfxTransform = RSDK.GetSfx("MSZ/MysticTransform.wav");
}

void HeavyMystic_SpawnParticleFX(int32 x, int32 y)
{
    int32 velX = RSDK.Rand(-0xC000, 0xC000);
    int32 velY = RSDK.Rand(-0xC000, 0xC000);

    EntityDebris *particles = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
    particles->drawGroup    = Zone->objectDrawGroup[0];
    particles->drawFX       = FX_FLIP;
    particles->direction    = RSDK.Rand(0, 4);
    particles->velocity.x   = velX;
    particles->velocity.y   = velY;
    particles->inkEffect    = INK_SUB;
    particles->alpha        = 0xE0;
    RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, RSDK.Rand(0, 2) + 5, &particles->animator, true, 0);
    particles->timer = 3 * particles->animator.frameCount;

    particles             = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
    particles->velocity.x = velX;
    particles->velocity.y = velY;
    particles->drawGroup  = Zone->objectDrawGroup[0];
    particles->inkEffect  = INK_ADD;
    particles->alpha      = 0xFF;
    RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, RSDK.Rand(0, 2) + 7, &particles->animator, true, 0);
    particles->timer = 3 * particles->animator.frameCount;
}

void HeavyMystic_HandleParticleFX(void)
{
    RSDK_THIS(HeavyMystic);

    if (!(Zone->timer & 7)) {
        HeavyMystic_SpawnParticleFX(self->position.x + RSDK.Rand(-0x200000, -0x100000), self->position.y + RSDK.Rand(-0x100000, 0));
        HeavyMystic_SpawnParticleFX(self->position.x + RSDK.Rand(0x100000, 0x200000), self->position.y + RSDK.Rand(-0x100000, 0));
    }
}

void HeavyMystic_CheckPlayerCollisions(void)
{
    RSDK_THIS(HeavyMystic);

    if (!self->particleFXTimer && !self->invincibilityTimer) {
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
    --self->rogueHealth;

    if (self->health <= 0) {
        RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);
        self->timer            = 120;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        self->state = HeavyMystic_StateBoss_Destroyed;
    }
    else {
        self->timer -= 30;
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(HeavyMystic->sfxHit, false, 255);
    }
}

void HeavyMystic_CheckPlayerCollisions_Fang(void)
{
    RSDK_THIS(HeavyMystic);

    if (!self->particleFXTimer) {
        if (self->invincibilityTimer > 0)
            self->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
                RSDK.PlaySfx(HeavyMystic->sfxImpact, false, 255);
                self->velocity.x = player->position.x < self->position.x ? 0x20000 : -0x20000;
                self->velocity.y = -0x40000;
                RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 3, &self->animator, true, 0);
                self->state = HeavyMystic_StateBoss_RogueHit;
            }
        }
    }
}

void HeavyMystic_CheckPlayerCollisions_Bark(void)
{
    RSDK_THIS(HeavyMystic);

    if (!self->particleFXTimer) {
        if (self->invincibilityTimer > 0)
            self->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox)) {
#if MANIA_USE_PLUS

                if (self->animator.animationID == ANI_SKID_TURN
                    && ((self->direction == FLIP_NONE && player->position.x > self->position.x)
                        || (self->direction == FLIP_X && player->position.x < self->position.x))) {
                    if (!Player_CheckMightyUnspin(player, 0x600, false, &player->uncurlTimer))
                        Player_Hurt(player, self);
                }
                else
#endif	
                    if (Player_CheckBossHit(player, self)) {
						
                    RSDK.PlaySfx(HeavyMystic->sfxImpact, false, 255);

                    self->velocity.x = player->position.x < self->position.x ? 0x20000 : -0x20000;
                    self->velocity.y = -0x40000;
                    RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 15, &self->animator, true, 0);
                    self->state = HeavyMystic_StateBoss_RogueHit;
                }
            }
        }
    }
}

void HeavyMystic_CheckPlayerCollisions_Bean(void)
{
    RSDK_THIS(HeavyMystic);

    if (!self->particleFXTimer) {

        if (self->invincibilityTimer > 0)
            self->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
                RSDK.PlaySfx(HeavyMystic->sfxImpact, false, 255);

                self->velocity.x = player->position.x < self->position.x ? 0x20000 : -0x20000;
                self->velocity.y = -0x40000;
                RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 9, &self->animator, true, 0);
                self->state = HeavyMystic_StateBoss_RogueHit;
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
            int32 x                    = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1] + 2;
        }
    }
}

void HeavyMystic_Scanline_Curtains(ScanlineInfo *scanlines)
{
    int32 curtainPos = HeavyMystic->curtainLinePos;
    int32 moveY      = (0x1100000 - curtainPos) >> 6;
    int32 max        = MAX(curtainPos - 0x400000, 0x100000);

    int32 posY = (ScreenInfo->position.y + 207) << 16;
    if (curtainPos > max) {
        int32 scanlineID = HeavyMystic->curtainLinePos >> 16;

        for (int32 i = 0; i < ((curtainPos - max - 1) >> 16) + 1; ++i) {
            scanlines[scanlineID--].position.y = posY;
            posY -= moveY;
        }
    }

    for (int32 i = HeavyMystic->curtainLinePos >> 16; i < 208; ++i) {
        scanlines[i].position.y = 0x6200000;
    }
}

void HeavyMystic_StateMischief_Setup(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    if (++self->timer >= 8) {
        self->velocity.x = 0x20000;

        // I'd prolly put a position check here too, its not hard to: go into the special stage, then not hit the starpost, doing so causes a softlock
        if (StarPost->postIDs[0])
            HeavyMystic->curtainLinePos = 1;

        self->targetPos = self->position;
        self->position.y += 0xC00000;
        self->visible = true;
        self->timer   = 120;
        self->state   = HeavyMystic_StateMischief_EnterMystic;
    }
}

void HeavyMystic_StateBoss_SetupArena(void)
{
    RSDK_THIS(HeavyMystic);
    if (++self->timer >= 8) {
        self->timer = 0;

        HeavyMystic->boundsL = (Zone->cameraBoundsL[0] + 64) << 16;
        HeavyMystic->boundsR = (Zone->cameraBoundsR[0] - 64) << 16;
        HeavyMystic->boundsM = self->position.x;
        HeavyMystic->boundsT = (Zone->cameraBoundsT[0] + 48) << 16;
        HeavyMystic->boundsB = (Zone->cameraBoundsB[0] - 8) << 16;

        self->state = HeavyMystic_StateBoss_AwaitPlayer;
    }
}

void HeavyMystic_StateBoss_AwaitPlayer(void)
{
    RSDK_THIS(HeavyMystic);

    if (self->timer) {
        if (++self->timer == 120) {
            Zone->cameraBoundsT[0] = Zone->cameraBoundsB[0] - ScreenInfo->size.y;
            MSZSetup_SetBGScrollOrigin(self->position.x + 0x4000000, 0);

            self->position.y += 0x500000;
            self->targetPos = self->position;
            self->timer     = 0;
            self->inkEffect = INK_ALPHA;
            self->visible   = true;

            self->state                                           = HeavyMystic_StateBoss_BeginShow;
            RSDK.GetTileLayer(Zone->fgLayer[1])->scanlineCallback = HeavyMystic_Scanline_Curtains;

            foreach_active(MSZSpotlight, spotlight) { spotlight->state = MSZSpotlight_State_Appear; }
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.y > self->position.y) {
            Music_TransitionTrack(TRACK_HBHBOSS, 0.0125);

            Zone->playerBoundActiveL[0] = true;
            Zone->playerBoundActiveR[0] = true;

            Zone->cameraBoundsL[0] = (self->position.x >> 16) - ScreenInfo->center.x;
            Zone->cameraBoundsR[0] = (self->position.x >> 16) + ScreenInfo->center.x;
            Zone->cameraBoundsB[0] = (self->position.y >> 16) + 256;

            ++self->timer;
            self->active = ACTIVE_NORMAL;
        }
    }
}

void HeavyMystic_StateMischief_EnterMystic(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    HeavyMystic_HandleParticleFX();

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
        self->state = HeavyMystic_StateMischief_HandleAppearArc;
}

void HeavyMystic_StateMischief_HandleAppearArc(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->position.y >= self->targetPos.y) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->position.y = self->targetPos.y;

        RSDK.PlaySfx(HeavyMystic->sfxBleeps, false, 255);
        self->state = HeavyMystic_StateMischief_MoveIntoPlace;
    }
}

void HeavyMystic_StateBoss_BeginShow(void)
{
    RSDK_THIS(HeavyMystic);

    if (self->alpha > 0xC0)
        HeavyMystic_HandleParticleFX();

#if MANIA_USE_PLUS
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
        self->state     = HeavyMystic_StateBoss_AwaitBoxOpened;
    }
    else {
        HeavyMystic->curtainLinePos -= 0x20000;
    }
}

void HeavyMystic_StateMischief_Idle(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    HeavyMystic_HandleParticleFX();
    HeavyMystic_HandleParticleFX();
}

void HeavyMystic_StateMischief_MoveIntoPlace(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);
    self->position.x += 0x8000;

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    HeavyMystic_HandleParticleFX();
    HeavyMystic_HandleParticleFX();

    if (++self->timer == 90) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 1, &self->animator, true, 0);
        RSDK.PlaySfx(HeavyMystic->sfxHat, false, 255);
        self->state = HeavyMystic_StateMischief_GoodDay;
    }
}

void HeavyMystic_StateMischief_FinishedMagicTrick(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();
    HeavyMystic_HandleParticleFX();

    if (++self->timer == 60) {
        self->timer = 0;

        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 3, &self->animator, true, 0);
        RSDK.PlaySfx(HeavyMystic->sfxTransform, false, 255);
        self->state = HeavyMystic_StateMischief_Disappear;
    }
}

void HeavyMystic_StateMischief_PrepareMagicTrick(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();
    HeavyMystic_HandleParticleFX();

    if (++self->timer == 60) {
        if (RSDK.CheckSceneFolder("MSZCutscene")) {
            self->state = HeavyMystic_StateMischief_Idle;
        }
        else {
            self->timer = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 2, &self->animator, true, 0);
            RSDK.PlaySfx(HeavyMystic->sfxHat, false, 255);
            self->state = HeavyMystic_StateMischief_ConjureHatterkiller;
        }
    }
}

void HeavyMystic_StateMischief_GoodDay(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
        self->state = HeavyMystic_StateMischief_PrepareMagicTrick;
    }
}

void HeavyMystic_StateMischief_ConjureHatterkiller(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();

    if (self->animator.frameID == 8 && !self->timer) {
        RSDK.PlaySfx(HeavyMystic->sfxHatNode, false, 255);
        CREATE_ENTITY(Hatterkiller, INT_TO_VOID(RSDK.Rand(-0x20000, 0x20000)), self->position.x + 0x320000, self->position.y);
        self->timer = 1;
    }

    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
        self->state = HeavyMystic_StateMischief_FinishedMagicTrick;
    }
}

void HeavyMystic_StateMischief_Disappear(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();

    if (self->animator.frameID == self->animator.frameCount - 1) {
        if (!HeavyMystic->curtainLinePos) {
            HeavyMystic->curtainLinePos = 1;

            CREATE_ENTITY(Vultron, INT_TO_VOID(VULTRON_TARGET), self->position.x, self->position.y);
            CREATE_ENTITY(Vultron, INT_TO_VOID(VULTRON_TARGET), self->position.x, self->position.y);
        }

        RSDK.PlaySfx(HeavyMystic->sfxPoof, false, 255);
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];

        destroyEntity(self);
    }
}

void HeavyMystic_StateBoss_Destroyed(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_Explode();

    if (--self->timer <= 0) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
        self->state = HeavyMystic_StateBoss_Finish;
    }
}

void HeavyMystic_StateBoss_Finish(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->state = HeavyMystic_StateBoss_CloseCurtains;
    }
}

void HeavyMystic_StateBoss_CloseCurtains(void)
{
    RSDK_THIS(HeavyMystic);

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        if (self->timer < 256) {
            self->timer += 3;
            RSDK.SetLimitedFade(0, 5, 1, self->timer, 128, 255);
        }
    }
#endif

    if (HeavyMystic->curtainLinePos >= 0xD00000) {
        HeavyMystic->curtainLinePos = 0xD00000;

        RSDK.GetTileLayer(Zone->fgLayer[1])->scanlineCallback = StateMachine_None;
        Zone->cameraBoundsR[0] += WIDE_SCR_XSIZE * 2;
        destroyEntity(self);
    }
    else {
        HeavyMystic->curtainLinePos += 0x20000;
    }
}

void HeavyMystic_StateBoss_AwaitBoxOpened(void) {}

void HeavyMystic_StateBoss_EnterMystic(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    HeavyMystic_HandleParticleFX();
    HeavyMystic_HandleParticleFX();

    if (++self->timer == 45) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 1, &self->animator, true, 0);
        self->state = HeavyMystic_StateBoss_GoodDay;
    }
}

void HeavyMystic_StateBoss_GoodDay(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();

    if (self->animator.frameID == self->animator.frameCount - 8)
        RSDK.PlaySfx(HeavyMystic->sfxHat, false, 255);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
        self->state = HeavyMystic_StateBoss_BoxCloseDelay;
    }
}

void HeavyMystic_StateBoss_BoxCloseDelay(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    if (self->angle == 192) {
        foreach_active(HeavyMystic, boss)
        {
            if (boss->type == MYSTIC_BOX)
                boss->state = HeavyMystic_StateBox_CloseDoors;
        }

        RSDK.PlaySfx(HeavyMystic->sfxClack, false, 255);
        self->state = HeavyMystic_StateBoss_AwaitBoxClosing;
    }
}

void HeavyMystic_StateBoss_AwaitBoxClosing(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    if (self->angle == 252)
        self->state = HeavyMystic_StateBoss_AwaitBoxClosed;
}

void HeavyMystic_StateBoss_AwaitBoxClosed(void)
{
    RSDK_THIS(HeavyMystic);

    if (++self->timer == 60) {
        self->timer   = 0;
        self->visible = false;

        foreach_active(HeavyMystic, boss)
        {
            if (boss->type == MYSTIC_BOX)
                boss->state = HeavyMystic_StateBox_Transforming;
        }

        self->state = HeavyMystic_StateBoss_Transforming;
    }
}

void HeavyMystic_StateBoss_Transforming(void)
{
    RSDK_THIS(HeavyMystic);

    foreach_active(HeavyMystic, boss)
    {
        if (boss->type == MYSTIC_BOX && boss->state == HeavyMystic_StateBox_ShowContents) {
            self->position = boss->position;
            boss->position = boss->targetPos;
            self->visible  = true;
            self->state    = HeavyMystic_StateBoss_ShowRogue;
        }
    }
}

void HeavyMystic_StateBoss_ShowRogue(void)
{
    RSDK_THIS(HeavyMystic);

    self->velocity.x  = 0;
    self->velocity.y  = 0;
    self->rogueHealth = 2;
    RSDK.PlaySfx(HeavyMystic->sfxTwinkle, false, 255);

    self->velocity.y      = -0x40000;
    self->particleFXTimer = 75;
    switch (self->rogueID) {
        case 0: // Fang
        case 3: // Fang
            RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 2, &self->animator, true, 4);
            self->state    = HeavyMystic_StateBoss_FangHop;
            self->attackID = 8;
            break;

        case 1: // Bean
            RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 6, &self->animator, true, 3);
            self->state = HeavyMystic_StateBoss_BeanJump;
            break;

        case 2: // Bark
            RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 13, &self->animator, true, 3);
            self->state = HeavyMystic_StateBoss_BarkJump;
            break;

        default: break;
    }
}

void HeavyMystic_StateBoss_MysticReveal(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    HeavyMystic_CheckPlayerCollisions();
    if (--self->timer <= 0) {
        foreach_active(HeavyMystic, boss)
        {
            if (boss->type == MYSTIC_BOX) {
                self->targetPos = boss->position;
            }
        }

        if (self->rogueHealth > 0) {
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 3, &self->animator, true, 0);
            self->state = HeavyMystic_StateBoss_TransformBackIntoRogue;
        }
        else {
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &self->animator, true, 0);
            self->state = HeavyMystic_StateBoss_MoveToBoxY;
        }
    }
}

void HeavyMystic_StateBoss_MoveToBoxY(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();

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
        self->angle      = 0x80;

        if (self->position.x == self->targetPos.x) {
            ++self->rogueID;
            self->state = HeavyMystic_StateBoss_BoxCloseDelay;
        }
        else {
            self->state = HeavyMystic_StateBoss_MoveToBoxX;
        }
    }
    else {
        self->position.y += self->velocity.y;

        if (self->velocity.y > -0x20000)
            self->velocity.y -= 0x2000;
    }
}

void HeavyMystic_StateBoss_MoveToBoxX(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 4, 11);

    if (self->position.x == self->targetPos.x) {
        ++self->rogueID;

        self->position.y = self->targetPos.y;
        self->state      = HeavyMystic_StateBoss_BoxCloseDelay;
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

void HeavyMystic_StateBoss_TransformBackIntoRogue(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    HeavyMystic_HandleParticleFX();

    if (self->animator.frameID == self->animator.frameCount - 5)
        RSDK.PlaySfx(HeavyMystic->sfxTransform, false, 255);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
        self->velocity.x                                                                                     = 0;
        self->velocity.y                                                                                     = -0x20000;
        RSDK.PlaySfx(HeavyMystic->sfxPoof, false, 255);

        self->velocity.y      = -0x40000;
        self->particleFXTimer = 75;
        switch (self->rogueID) {
            case 0:
            case 3:
                RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 2, &self->animator, true, 4);
                self->state    = HeavyMystic_StateBoss_FangHop;
                self->attackID = 8;
                break;

            case 1:
                RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 6, &self->animator, true, 3);
                self->state = HeavyMystic_StateBoss_BeanJump;
                break;

            case 2:
                RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 13, &self->animator, true, 3);
                self->state = HeavyMystic_StateBoss_BarkJump;
                break;

            default: break;
        }
    }
}

void HeavyMystic_StateBoss_FangIdle(void)
{
    RSDK_THIS(HeavyMystic);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK.ProcessAnimation(&self->animator);

    self->direction = player1->position.x < self->position.x;

    if (!--self->timer) {
        RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 1, &self->animator, true, 0);
        self->state = HeavyMystic_StateBoss_FangTell;
    }

    HeavyMystic_CheckPlayerCollisions_Fang();
}

void HeavyMystic_StateBoss_FangTell(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
        self->velocity.y = -0x50000;
        self->attackID   = 8;
        RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 2, &self->animator, true, 0);
        self->state = HeavyMystic_StateBoss_FangHop;
    }

    HeavyMystic_CheckPlayerCollisions_Fang();
}

void HeavyMystic_StateBoss_FangHop(void)
{
    RSDK_THIS(HeavyMystic);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->direction = player1->position.x < self->position.x;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.x >= 0) {
        if (self->position.x > (ScreenInfo->position.x + ScreenInfo->size.x - 16) << 16)
            self->velocity.x = -self->velocity.x;
    }
    else {
        if (self->position.x < (ScreenInfo->position.x + 16) << 16)
            self->velocity.x = -self->velocity.x;
    }

    RSDK.ProcessAnimation(&self->animator);

    if (self->velocity.y > 0 && RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x140000, true)) {
        if (--self->attackID > 0) {
            self->velocity.y = -0x50000;
            self->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
            RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 2, &self->animator, true, 0);
            self->state = HeavyMystic_StateBoss_FangHop;
        }
        else {
            self->timer      = 15;
            self->velocity.x = 0;
            self->velocity.y = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 0, &self->animator, true, 0);
            self->state = HeavyMystic_StateBoss_FangIdle;
        }
    }

    if (self->attackID && !(Zone->timer & 0x3F)) {
        RSDK.PlaySfx(HeavyMystic->sfxPon, false, 255);
        EntityHeavyMystic *cork = CREATE_ENTITY(HeavyMystic, INT_TO_VOID(MYSTIC_CORK), self->position.x, self->position.y - 0x40000);
        if (self->direction == FLIP_NONE) {
            cork->position.x += 0x180000;
            cork->velocity.x = 0x20000;
            cork->direction  = self->direction;
        }
        else {
            cork->position.x -= 0x180000;
            cork->velocity.x = -0x20000;
            cork->direction  = self->direction;
        }
    }

    HeavyMystic_CheckPlayerCollisions_Fang();
}

void HeavyMystic_StateBoss_RogueHit(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.x >= 0) {
        if (self->position.x > (ScreenInfo->position.x + ScreenInfo->size.x - 16) << 16)
            self->velocity.x = 0;
    }
    else {
        if (self->position.x < (ScreenInfo->position.x + 16) << 16)
            self->velocity.x = 0;
    }

    self->visible ^= true;
    RSDK.ProcessAnimation(&self->animator);

    if (self->velocity.y > 0) {
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x240000, true)) {
            RSDK.PlaySfx(HeavyMystic->sfxPowerDown, false, 255);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
            self->direction ^= FLIP_X;
            self->timer       = 90;
            self->attackID    = 0;
            self->visible     = true;
            self->targetPos.y = self->position.y;
            self->velocity.x  = 0;
            self->velocity.y  = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 4, &self->animator, true, 0);
            self->state = HeavyMystic_StateBoss_MysticReveal;
        }
    }
}

void HeavyMystic_StateBoss_BarkIdle(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK.ProcessAnimation(&self->animator);

    self->direction = player1->position.x < self->position.x;

    if (!--self->timer) {
        self->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
        self->velocity.y = -0x40000;

        if (self->attackID == 1) {
            self->timer = 240;
            RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 14, &self->animator, true, 0);
            self->state = HeavyMystic_StateBoss_BarkPounding;
        }
        else {
            self->attackID = 1;
            RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 13, &self->animator, true, 0);
            self->state = HeavyMystic_StateBoss_BarkJump;
        }
    }

    HeavyMystic_CheckPlayerCollisions_Bark();
}

void HeavyMystic_StateBoss_BarkPounding(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    // check when bark's hand hits the grounds
    if (self->animator.frameID == 3 || self->animator.frameID == 10) {
        if (self->attackID != self->animator.frameID)
            RSDK.PlaySfx(HeavyMystic->sfxImpact2, false, 255);
        self->attackID = self->animator.frameID;
    }

    if (--self->timer <= 0) {
        self->attackID   = 0;
        self->timer      = 15;
        self->velocity.x = 0;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 12, &self->animator, true, 0);
        self->state = HeavyMystic_StateBoss_BarkIdle;
    }

    if (self->attackID != 1) {
        if (!(Zone->timer & 3))
            Camera_ShakeScreen(0, 0, 2);

        if (!(Zone->timer & 0xF)) {
            int32 x                   = ScreenInfo->position.x + 16 + 32 * RSDK.Rand(0, ScreenInfo->size.x >> 5);
            EntityHeavyMystic *debris = CREATE_ENTITY(HeavyMystic, INT_TO_VOID(MYSTIC_DEBRIS), x << 16, (ScreenInfo->position.y - 16) << 16);
            debris->animator.frameID  = RSDK.Rand(0, 6);
        }
    }

    HeavyMystic_CheckPlayerCollisions_Bark();
}

void HeavyMystic_StateBoss_BarkJump(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->direction = player1->position.x < self->position.x;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.x >= 0) {
        if (self->position.x > (ScreenInfo->position.x + ScreenInfo->size.x - 16) << 16)
            self->velocity.x = -self->velocity.x;
    }
    else {
        if (self->position.x < (ScreenInfo->position.x + 16) << 16)
            self->velocity.x = -self->velocity.x;
    }

    RSDK.ProcessAnimation(&self->animator);

    if (self->velocity.y > 0 && RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x200000, true)) {
        self->timer = 15;
        RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 12, &self->animator, true, 0);
        self->state = HeavyMystic_StateBoss_BarkIdle;
    }

    HeavyMystic_CheckPlayerCollisions_Bark();
}

void HeavyMystic_StateBoss_BeanIdle(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->direction = player1->position.x < self->position.x;

    RSDK.ProcessAnimation(&self->animator);

    if (!--self->timer) {
        self->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x20000 : 0x20000;
        self->velocity.y = -0x68000;

        if (self->attackID == 1) {
            RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 8, &self->animator, true, 0);
            self->state = HeavyMystic_StateBoss_BeanBomb1Throw;
        }
        else {
            self->attackID = 1;
            RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 6, &self->animator, true, 0);
            self->state = HeavyMystic_StateBoss_BeanJump;
        }
    }

    HeavyMystic_CheckPlayerCollisions_Bean();
}

void HeavyMystic_StateBoss_BeanBomb1Throw(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    if (self->attackID == 1 && self->animator.frameID == 3) {
        self->attackID = 2;
        RSDK.PlaySfx(HeavyMystic->sfxDrop, false, 255);

        EntityHeavyMystic *bomb = CREATE_ENTITY(HeavyMystic, INT_TO_VOID(MYSTIC_BOMB), self->position.x, self->position.y + 0x130000);
        if (self->direction) {
            bomb->position.x += 0xB0000;
            bomb->velocity.x = -0x40000;
        }
        else {
            bomb->position.x -= 0xB0000;
            bomb->velocity.x = 0x40000;
        }

        bomb->velocity.y = -0x40000;
        bomb->direction  = self->direction;
    }

    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->attackID = 1;
        RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 7, &self->animator, true, 0);
        self->state = HeavyMystic_StateBoss_BeanBomb2Throw;
    }

    HeavyMystic_CheckPlayerCollisions_Bean();
}

void HeavyMystic_StateBoss_BeanBomb2Throw(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    if (self->attackID == 1 && self->animator.frameID == 2) {
        self->attackID = 2;
        RSDK.PlaySfx(HeavyMystic->sfxDrop, false, 255);

        EntityHeavyMystic *bomb = CREATE_ENTITY(HeavyMystic, INT_TO_VOID(MYSTIC_BOMB), self->position.x, self->position.y - 0x130000);
        if (self->direction) {
            bomb->position.x += 0xB0000;
            bomb->velocity.x = -0x40000;
        }
        else {
            bomb->position.x -= 0xB0000;
            bomb->velocity.x = 0x40000;
        }

        bomb->velocity.y = -0x40000;
        bomb->direction  = self->direction;
    }

    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->attackID   = 0;
        self->timer      = 5;
        self->velocity.x = 0;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 5, &self->animator, true, 0);
        self->state = HeavyMystic_StateBoss_BeanIdle;
    }

    HeavyMystic_CheckPlayerCollisions_Bean();
}

void HeavyMystic_StateBoss_BeanJump(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->direction = player1->position.x < self->position.x;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.x >= 0) {
        if (self->position.x > (ScreenInfo->position.x + ScreenInfo->size.x - 16) << 16)
            self->velocity.x = -self->velocity.x;
    }
    else {
        if (self->position.x < (ScreenInfo->position.x + 16) << 16)
            self->velocity.x = -self->velocity.x;
    }

    RSDK.ProcessAnimation(&self->animator);

    if (self->velocity.y > 0 && RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x140000, true) == true) {
        if (--self->attackID <= 0) {
            self->attackID   = 1;
            self->timer      = 15;
            self->velocity.x = 0;
            self->velocity.y = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 5, &self->animator, true, 0);
            self->state = HeavyMystic_StateBoss_BeanIdle;
        }
        else {
            self->velocity.y = -0x50000;
            self->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
            RSDK.SetSpriteAnimation(HeavyMystic->roguesFrames, 6, &self->animator, true, 0);
        }
    }

    HeavyMystic_CheckPlayerCollisions_Bean();
}

void HeavyMystic_StateCork_Fired(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
#if MANIA_USE_PLUS
            if (Player_CheckMightyShellHit(player, self, -0x300, -0x400))
                self->state = HeavyMystic_StateCork_MightyRebound;
            else
#endif
                Player_Hurt(player, self);

            RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1] + 2;
            destroyEntity(self);
            foreach_break;
        }
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if MANIA_USE_PLUS
void HeavyMystic_StateCork_MightyRebound(void)
{
    RSDK_THIS(HeavyMystic);

    self->visible ^= true;

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}
#endif

void HeavyMystic_State_Bomb(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x300, false, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
            RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1] + 2;
            destroyEntity(self);
            foreach_break;
        }
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void HeavyMystic_State_BarkDebris(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x300, false, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void HeavyMystic_StateBox_AwaitCurtainRise(void)
{
    RSDK_THIS(HeavyMystic);

    self->timer = 2;
    if (HeavyMystic->curtainLinePos <= 0x100000) {
        self->state     = HeavyMystic_StateBox_Idle;
        self->stateDraw = HeavyMystic_Draw_BoxTransforming;
    }
}

void HeavyMystic_StateBox_Idle(void) {}

void HeavyMystic_StateBox_CloseDoors(void)
{
    RSDK_THIS(HeavyMystic);

    self->scale.x -= 0x20;
    if (self->scale.x <= 0) {
        if (self->timer == 2) {
            foreach_active(HeavyMystic, boss)
            {
                if (boss->state == HeavyMystic_StateBoss_AwaitBoxOpened)
                    boss->state = HeavyMystic_StateBoss_EnterMystic;
            }

            self->timer     = 1;
            self->stateDraw = HeavyMystic_Draw_BoxOpened;
            self->state     = HeavyMystic_StateBox_OpenDoors;
        }
        else {
            self->timer     = 2;
            self->stateDraw = HeavyMystic_Draw_BoxTransforming;
            self->state     = HeavyMystic_StateBox_OpenDoors;
        }
    }
}

void HeavyMystic_StateBox_OpenDoors(void)
{
    RSDK_THIS(HeavyMystic);

    self->scale.x += 0x20;
    if (self->scale.x >= 0x200)
        self->state = HeavyMystic_StateBox_Idle;
}

void HeavyMystic_StateBox_Transforming(void)
{
    RSDK_THIS(HeavyMystic);

    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(HeavyMystic->sfxMagicBox, false, 255);
        int32 x                                                                 = self->position.x + (RSDK.Rand(-24, 25) << 16);
        int32 y                                                                 = self->position.y + (RSDK.Rand(-24, 25) << 16);
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), x, y)->drawGroup = Zone->objectDrawGroup[1] + 2;
    }

    self->scale.x = 0x200 + RSDK.Cos256(self->angle + 0x40);
    self->scale.y = 0x200 + RSDK.Sin256(self->angle);

    if (self->angle == 0x300) {
        RSDK.StopSfx(HeavyMystic->sfxMagicBox);
        RSDK.PlaySfx(HeavyMystic->sfxPowerDown, false, 255);
        self->angle     = 0;
        self->timer     = 0;
        self->state     = HeavyMystic_StateBox_TransformFinish;
        self->stateDraw = HeavyMystic_Draw_BoxClosed;
    }
    else {
        self->angle += 8;
    }
}

void HeavyMystic_StateBox_TransformFinish(void)
{
    RSDK_THIS(HeavyMystic);

    if (++self->timer > 30) {
        RSDK.PlaySfx(HeavyMystic->sfxDrop, false, 255);
        self->state = HeavyMystic_StateBox_Dropping;
    }
}

void HeavyMystic_StateBox_Dropping(void)
{
    RSDK_THIS(HeavyMystic);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x280000, true)) {
        Camera_ShakeScreen(0, 0, 6);
        RSDK.PlaySfx(HeavyMystic->sfxImpact2, false, 255);
        RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);

        EntityDebris *debris    = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        debris->drawGroup       = Zone->objectDrawGroup[0];
        debris->drawFX          = FX_FLIP;
        debris->direction       = FLIP_NONE;
        debris->velocity.y      = -0x40000;
        debris->gravityStrength = 0x2800;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &debris->animator, true, 0);
        debris->animator.frameID = 0;

        debris                  = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x280000, self->position.y);
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->drawFX          = FX_FLIP;
        debris->direction       = FLIP_NONE;
        debris->velocity.x      = -0x20000;
        debris->velocity.y      = -0x40000;
        debris->gravityStrength = 0x2800;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &debris->animator, true, 0);
        debris->animator.frameID = 2;

        debris                  = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x280000, self->position.y);
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->drawFX          = FX_FLIP;
        debris->direction       = FLIP_X;
        debris->velocity.x      = 0x20000;
        debris->velocity.y      = -0x40000;
        debris->gravityStrength = 0x2800;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &debris->animator, true, 0);
        debris->animator.frameID = 2;

        self->timer      = 0;
        self->visible    = false;
        self->velocity.y = 0;
        self->state      = HeavyMystic_StateBox_ShowContents;
    }
}

void HeavyMystic_StateBox_ShowContents(void)
{
    RSDK_THIS(HeavyMystic);

    if (++self->timer > 96) {
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1] + 2;
        self->visible                                                                                         = true;
        self->scale.x                                                                                         = 0;
        self->scale.y                                                                                         = 0;
        RSDK.PlaySfx(HeavyMystic->sfxAssemble, false, 255);
        self->stateDraw = HeavyMystic_Draw_BoxClosed;
        self->state     = HeavyMystic_StateBox_Reappear;
    }
}

void HeavyMystic_StateBox_Reappear(void)
{
    RSDK_THIS(HeavyMystic);

    if (self->scale.x < 0x200) {
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
            self->stateDraw = HeavyMystic_Draw_BoxTransforming;
            self->state     = HeavyMystic_StateBox_CloseDoors;
        }

        self->rotation &= 0x1FF;
    }
}

void HeavyMystic_Draw_BoxOpened(void)
{
    RSDK_THIS(HeavyMystic);

    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        self->drawFX           = FX_NONE;
        self->animator.frameID = 0;
        RSDK.DrawSprite(&self->animator, NULL, false);

        Vector2 drawPos;
        drawPos.x              = self->position.x - 0x280000;
        drawPos.y              = self->position.y;
        self->animator.frameID = self->timer;
        self->drawFX           = FX_SCALE | FX_ROTATE | FX_FLIP;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        drawPos.x += 0x500000;
        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->direction = FLIP_NONE;
    }
}

void HeavyMystic_Draw_BoxTransforming(void)
{
    RSDK_THIS(HeavyMystic);

    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        self->drawFX = FX_NONE;
        if (self->state != HeavyMystic_StateBox_Transforming) {
            self->animator.frameID = 0;
            RSDK.DrawSprite(&self->animator, NULL, false);
        }
    }
    else {
        Vector2 drawPos;

        drawPos.x              = self->position.x - 0x280000;
        drawPos.y              = self->position.y;
        self->animator.frameID = self->timer;
        self->drawFX           = FX_SCALE | FX_ROTATE | FX_FLIP;
        if (self->state == HeavyMystic_StateBox_Transforming)
            drawPos.x += 0x1400 * (0x200 - self->scale.x);
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->direction = FLIP_X;
        drawPos.x       = self->position.x + 0x280000;
        if (self->state == HeavyMystic_StateBox_Transforming)
            drawPos.x += 0x1400 * (self->scale.x - 0x200);
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        self->direction = FLIP_NONE;
    }
}

void HeavyMystic_Draw_BoxClosed(void)
{
    RSDK_THIS(HeavyMystic);

    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        self->drawFX           = FX_SCALE | FX_ROTATE;
        self->animator.frameID = 3;
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
}

#if GAME_INCLUDE_EDITOR
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

            if (showGizmos()) {
                RSDK_DRAWING_OVERLAY(true);

                DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, (256 - SCREEN_YSIZE), WIDE_SCR_XCENTER, 256, 1 | 2 | 4 | 8, 0x00C0F0);

                RSDK_DRAWING_OVERLAY(false);
            }
            break;

        case MYSTIC_BOX:
            self->drawFX = FX_SCALE | FX_ROTATE | FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &self->animator, true, 0);
            break;

        default: break;
    }

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void HeavyMystic_EditorLoad(void)
{
    HeavyMystic->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);
    HeavyMystic->roguesFrames = RSDK.LoadSpriteAnimation("MSZ/Rogues.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(HeavyMystic, type);
    RSDK_ENUM_VAR("Mischief", MYSTIC_MISCHIEF);
    RSDK_ENUM_VAR("Boss", MYSTIC_BOSS);
    RSDK_ENUM_VAR("Box", MYSTIC_BOX);
    // RSDK_ENUM_VAR("Cork", MYSTIC_CORK);
    // RSDK_ENUM_VAR("Bomb", MYSTIC_BOMB);
    // RSDK_ENUM_VAR("Debris", MYSTIC_DEBRIS);
}
#endif

void HeavyMystic_Serialize(void) { RSDK_EDITABLE_VAR(HeavyMystic, VAR_ENUM, type); }
