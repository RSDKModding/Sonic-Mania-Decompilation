// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PhantomEgg Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPhantomEgg *PhantomEgg;

void PhantomEgg_Update(void)
{
    RSDK_THIS(PhantomEgg);

    StateMachine_Run(self->state);
}

void PhantomEgg_LateUpdate(void) {}

void PhantomEgg_StaticUpdate(void) {}

void PhantomEgg_Draw(void)
{
    RSDK_THIS(PhantomEgg);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->crackAnimator, NULL, false);
    }
}

void PhantomEgg_Create(void *data)
{
    RSDK_THIS(PhantomEgg);

    self->drawFX = FX_FLIP;

    if (!SceneInfo->inEditor) {
        self->active           = ACTIVE_BOUNDS;
        self->updateRange.x    = 0x800000;
        self->updateRange.y    = 0x1000000;
        self->attackStateTable = PhantomEgg->attackStateTable1;
        self->type             = VOID_TO_INT(data);

        if (!data) {
            self->visible   = false;
            self->drawGroup = Zone->objectDrawGroup[0];

            self->hitbox.left   = -24;
            self->hitbox.top    = -24;
            self->hitbox.right  = 24;
            self->hitbox.bottom = 24;

            self->drawFX = FX_FLIP;
            self->active = ACTIVE_NORMAL;
            self->health = 16;

            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 0, &self->coreAnimator, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 17, &self->eggmanAnimator, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->armLAnimator, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &self->legAnimator, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 13, &self->rubyAnimator, true, 0);

            self->targetPos.x = self->position.x;
            self->targetPos.y = self->position.y;
            self->state       = PhantomEgg_State_SetupArena;
        }
    }
}

void PhantomEgg_StageLoad(void)
{
    PhantomEgg->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomEgg.bin", SCOPE_STAGE);

    PhantomEgg->savedGameProgress = false;

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreTMZ3.act", 0b0000000011111111);
        RSDK.LoadPalette(1, "EncoreTMZ3.act", 0b0000000011111111);
    }
#endif

    if (!PhantomEgg->setupPalette) {
        RSDK.CopyPalette(0, 0, 1, 0, 128);
        PhantomEgg->setupPalette = true;
    }

    RSDK.CopyPalette(1, 128, 0, 128, 128);
    RSDK.CopyPalette(0, 0, 4, 0, 128);

    PhantomEgg->disableSuperForm = false;

    PhantomEgg->sfxHit        = RSDK.GetSfx("Stage/BossHit.wav");
    PhantomEgg->sfxExplosion2 = RSDK.GetSfx("Stage/Explosion2.wav");
    PhantomEgg->sfxRocketJet  = RSDK.GetSfx("Stage/RocketJet.wav");
    PhantomEgg->sfxExplosion3 = RSDK.GetSfx("Stage/Explosion3.wav");
    PhantomEgg->sfxJump       = RSDK.GetSfx("TMZ3/Jump.wav");
    PhantomEgg->sfxLand       = RSDK.GetSfx("TMZ3/Land.wav");
    PhantomEgg->sfxRepel      = RSDK.GetSfx("TMZ3/Repel.wav");
    PhantomEgg->sfxShield     = RSDK.GetSfx("TMZ3/Shield.wav");
    PhantomEgg->sfxShock      = RSDK.GetSfx("TMZ3/Shock.wav");
    PhantomEgg->sfxSummon     = RSDK.GetSfx("TMZ3/Summon.wav");
    PhantomEgg->sfxMissile    = RSDK.GetSfx("TMZ3/Missile.wav");
}

void PhantomEgg_HandleAnimations(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&self->eggmanAnimator);

    if (self->rubyAnimator.animationID == 14) {
        RSDK.ProcessAnimation(&self->rubyAnimator);

        if (self->rubyAnimator.frameID == self->rubyAnimator.frameCount - 1)
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 13, &self->rubyAnimator, true, 0);
    }

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    if (self->eggmanAnimator.animationID == 17) {
        self->eggmanAnimator.speed = 0;
    }
    else {
        if (self->eggmanAnimator.animationID == 18 || self->eggmanAnimator.animationID == 19) {
            if (self->eggmanAnimator.frameID == self->eggmanAnimator.frameCount - 1)
                RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 17, &self->eggmanAnimator, true, 0);
        }
    }
}

void PhantomEgg_CheckPlayerCollisions(void)
{
    RSDK_THIS(PhantomEgg);

    if (RSDK.GetEntityCount(PhantomShield->classID, true) <= 0) {
        foreach_active(Player, player)
        {
            if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
                PhantomEgg_Hit();
            }
        }
    }
}

void PhantomEgg_Hit(void)
{
    RSDK_THIS(PhantomEgg);

    --self->health;
    if (!(self->health & 3)) {
        int32 id = (-2 - RSDK.GetEntityCount(TMZCable->classID, true)) & 3;

        foreach_active(TMZCable, cable)
        {
            if (cable->cableID == id || !self->health) {
                RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 9, &cable->animator, true, 0);
                cable->state = TMZCable_State_Destroyed;
                // Bug Details:
                // uncomment to fix a minor visual bug where the start few cable nodes wont be destroyed properly
                // cable->timer = 0;
            }
        }

        if (id == 1)
            self->attackStateTable = PhantomEgg->attackStateTable2;
        else
            self->attackStateTable = PhantomEgg->attackStateTable1;
    }

    if (self->health <= 0) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 20, &self->eggmanAnimator, true, 0);
        self->targetPos.x            = self->position.x;
        self->targetPos.y            = self->position.y;
        self->state                  = PhantomEgg_State_Destroyed;
        self->timer                  = 0;
        PhantomEgg->disableSuperForm = true;
        SceneInfo->timeEnabled       = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 19, &self->eggmanAnimator, true, 0);
        RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
    }
}

void PhantomEgg_Explode(Hitbox *hitbox)
{
    RSDK_THIS(PhantomEgg);

    if (!(Zone->timer % 7)) {
        RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);

        if (!(Zone->timer & 8)) {
            int32 x = self->position.x + (RSDK.Rand(hitbox->left, hitbox->right) << 16);
            int32 y = self->position.y + (RSDK.Rand(hitbox->top, hitbox->bottom) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }
}

void PhantomEgg_HandleNextAttack(void)
{
    RSDK_THIS(PhantomEgg);

    switch (self->attackStateTable[self->attackTimer]) {
        case PHANTOMEGG_ATTACK_JUMP:
            if (self->state != PhantomEgg_State_Attack_JumpLand) {
                self->groundVel      = 0;
                self->velocity.x     = 0;
                self->remainingJumps = 3;
                self->state          = PhantomEgg_State_Attack_Jumped;
                self->attackTimer++;
                break;
            }
            break;

        case PHANTOMEGG_ATTACK_SHOCK:
            self->timer = 0;
            self->state = PhantomEgg_State_Attack_CableShock;
            self->attackTimer++;
            break;

        case PHANTOMEGG_ATTACK_MISSILES:
            if (self->position.y >= PhantomEgg->boundsB - 0xA00000) {
                foreach_active(PhantomMissile, missile) { missile->timer = 16 * missile->id + 8; }

                ++self->attackTimer;
            }
            break;

        case PHANTOMEGG_ATTACK_WARP:
            self->timer = 0;
            self->state = PhantomEgg_State_Attack_PrepareWarp;

            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &self->legAnimator, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->armLAnimator, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->armRAnimator, true, 0);

            self->attackTimer++;
            break;

        default: self->attackTimer++; break;
    }

    self->attackTimer %= 32;
}

void PhantomEgg_SetupWarpFX(void)
{
    foreach_all(PhantomEgg, phantomEgg)
    {
        PhantomEgg->startScanline = ScreenInfo->center.y;
        PhantomEgg->endScanline   = ScreenInfo->size.y;

        RSDK.GetTileLayer(Zone->fgLayer[0])->scanlineCallback = PhantomEgg_Scanline_WarpFX;
        RSDK.GetTileLayer(Zone->fgLayer[1])->scanlineCallback = PhantomEgg_Scanline_WarpFX;

        PhantomRuby_PlaySfx(RUBYSFX_ATTACK1);

        phantomEgg->timer = 0;
        phantomEgg->state = PhantomEgg_State_Attack_HandleWarp;
        foreach_break;
    }
}

void PhantomEgg_HandlePhantomWarp(uint8 phantomID)
{
    PhantomEgg->boundsStoreL1 = Zone->cameraBoundsL[0];
    PhantomEgg->boundsStoreR1 = Zone->cameraBoundsR[0];
    PhantomEgg->boundsStoreT1 = Zone->cameraBoundsT[0];
    PhantomEgg->boundsStoreB1 = Zone->cameraBoundsB[0];

    Entity *targetPhantom = NULL;

    switch (phantomID & 3) {
        case 0: {
            foreach_all(PhantomGunner, gunner)
            {
                targetPhantom = (Entity *)gunner;
                foreach_break;
            }
            break;
        }

        case 1: {
            foreach_all(PhantomShinobi, shinobi)
            {
                targetPhantom = (Entity *)shinobi;
                foreach_break;
            }
            break;
        }

        case 2: {
            foreach_all(PhantomMystic, mystic)
            {
                targetPhantom = (Entity *)mystic;
                foreach_break;
            }
            break;
        }

        case 3: {
            foreach_all(PhantomRider, rider)
            {
                targetPhantom = (Entity *)rider;
                foreach_break;
            }
            break;
        }
    }

    if (targetPhantom) {
        int32 phantomSlot              = RSDK.GetEntitySlot(targetPhantom);
        EntityPlatformNode *nodeTop    = RSDK_GET_ENTITY(phantomSlot + 1, PlatformNode);
        EntityPlatformNode *nodeBottom = RSDK_GET_ENTITY(phantomSlot + 2, PlatformNode);
        EntityPlatformNode *nodeLeft   = RSDK_GET_ENTITY(phantomSlot + 3, PlatformNode);
        EntityPlatformNode *nodeRight  = RSDK_GET_ENTITY(phantomSlot + 4, PlatformNode);

        int32 offsetX = nodeBottom->position.x - PhantomEgg->boundsM;
        int32 offsetY = nodeBottom->position.y - (PhantomEgg->boundsStoreB1 << 16);

        for (int32 p = 0; p < Player->playerCount; ++p) {
            Zone->cameraBoundsL[p] = nodeLeft->position.x >> 16;
            Zone->cameraBoundsR[p] = nodeRight->position.x >> 16;
            Zone->cameraBoundsT[p] = nodeTop->position.y >> 16;
            Zone->cameraBoundsB[p] = nodeBottom->position.y >> 16;

            Zone->playerBoundsL[p] = nodeLeft->position.x;
            Zone->playerBoundsR[p] = nodeRight->position.x;
            Zone->playerBoundsT[p] = nodeTop->position.y;
            Zone->playerBoundsB[p] = nodeBottom->position.y;
            Zone->deathBoundary[p] = nodeBottom->position.y + 0x800000;
        }

        targetPhantom->active = ACTIVE_NORMAL;

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->boundsL      = Zone->cameraBoundsL[0];
        camera->boundsR      = Zone->cameraBoundsR[0];
        camera->boundsT      = Zone->cameraBoundsT[0];
        camera->boundsB      = Zone->cameraBoundsB[0];

        if ((phantomID & 3) == 3) {
            EntityPlatformNode *nodeStart = RSDK_GET_ENTITY(phantomSlot + 5, PlatformNode);
            camera->position.x            = nodeStart->position.x;
            camera->position.y            = nodeStart->position.y;

            foreach_active(Player, player)
            {
                player->position.x = camera->position.x;
                player->position.y = camera->position.y;
            }

            foreach_all(PhantomHand, hand)
            {
                hand->position.x = camera->position.x;
                hand->position.y = camera->position.y;
            }
        }
        else {
            camera->position.x += offsetX;
            camera->position.y += offsetY;
            foreach_active(Player, player)
            {
                player->position.x += offsetX;
                player->position.y += offsetY;
            }

            foreach_all(PhantomHand, hand)
            {
                hand->position.x += offsetX;
                hand->position.y += offsetY;
            }
        }

        foreach_all(PhantomMissile, missile) { missile->active = ACTIVE_NEVER; }
        foreach_all(TMZCable, cable) { cable->active = ACTIVE_NEVER; }
    }
}

void PhantomEgg_HandleReturnWarp(void)
{
    for (int32 p = 0; p < Player->playerCount; ++p) {
        Zone->cameraBoundsL[p] = PhantomEgg->boundsStoreL1;
        Zone->cameraBoundsR[p] = PhantomEgg->boundsStoreR1;
        Zone->cameraBoundsT[p] = PhantomEgg->boundsStoreT1;
        Zone->cameraBoundsB[p] = PhantomEgg->boundsStoreB1;

        Zone->playerBoundsL[p] = PhantomEgg->boundsStoreL1 << 16;
        Zone->playerBoundsR[p] = PhantomEgg->boundsStoreR1 << 16;
        Zone->playerBoundsT[p] = PhantomEgg->boundsStoreT1 << 16;
        Zone->playerBoundsB[p] = PhantomEgg->boundsStoreB1 << 16;
        Zone->deathBoundary[p] = Zone->playerBoundsB[p];
    }

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    camera->boundsL      = Zone->cameraBoundsL[0];
    camera->boundsR      = Zone->cameraBoundsR[0];
    camera->boundsT      = Zone->cameraBoundsT[0];
    camera->boundsB      = Zone->cameraBoundsB[0];
    camera->position.x   = PhantomEgg->boundsM;
    camera->position.y   = PhantomEgg->boundsStoreB1 - (ScreenInfo->center.y << 16);

    foreach_active(Player, player)
    {
        player->position.x = PhantomEgg->boundsM;
        player->position.y = (PhantomEgg->boundsStoreB1 - 40) << 16;
    }

    foreach_all(PhantomMissile, missile) { missile->active = ACTIVE_NORMAL; }
    foreach_all(TMZCable, cable) { cable->active = ACTIVE_NORMAL; }
}

void PhantomEgg_Scanline_WarpFX(ScanlineInfo *scanlines)
{
    TileLayer *fgLow = RSDK.GetTileLayer(Zone->fgLayer[0]);
    RSDK.ProcessParallax(fgLow);

    int32 line = 0;
    if (PhantomEgg->startScanline - PhantomEgg->endScanline >= 0)
        line = PhantomEgg->startScanline - PhantomEgg->endScanline;

    int32 lineY = scanlines[line].position.y;
    for (int32 l = 0; l < line; ++l) scanlines[l].position.y = lineY;

    line = PhantomEgg->startScanline + PhantomEgg->endScanline;
    if (line > ScreenInfo->size.y)
        line = ScreenInfo->size.y;

    lineY = scanlines[line].position.y;
    for (int32 l = line; l < ScreenInfo->size.y; ++l) scanlines[l].position.y = lineY;
}

void PhantomEgg_Draw_Normal(void)
{
    RSDK_THIS(PhantomEgg);

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(3, 32, 0, 32, 10);
        RSDK.CopyPalette(3, 128, 0, 128, 16);
        RSDK.SetPaletteEntry(0, 128, 0xF0F0F0);

        self->direction            = FLIP_NONE;
        self->coreAnimator.frameID = 1;
        RSDK.DrawSprite(&self->coreAnimator, NULL, false);
        RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);

        self->coreAnimator.frameID = 0;
        RSDK.DrawSprite(&self->coreAnimator, NULL, false);
        RSDK.DrawSprite(&self->armLAnimator, NULL, false);
        RSDK.DrawSprite(&self->legAnimator, NULL, false);
        RSDK.DrawSprite(&self->rubyAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->armLAnimator, NULL, false);
        RSDK.DrawSprite(&self->legAnimator, NULL, false);

        RSDK.CopyPalette(1, 32, 0, 32, 10);
        RSDK.CopyPalette(1, 128, 0, 128, 16);
        RSDK.SetPaletteEntry(0, 128, 0x000000);
    }
    else {
        self->direction            = FLIP_NONE;
        self->coreAnimator.frameID = 1;
        RSDK.DrawSprite(&self->coreAnimator, NULL, false);
        RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);

        self->coreAnimator.frameID = 0;
        RSDK.DrawSprite(&self->coreAnimator, NULL, false);
        RSDK.DrawSprite(&self->armLAnimator, NULL, false);
        RSDK.DrawSprite(&self->legAnimator, NULL, false);
        RSDK.DrawSprite(&self->rubyAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->armLAnimator, NULL, false);
        RSDK.DrawSprite(&self->legAnimator, NULL, false);
    }
}

void PhantomEgg_Draw_Cracked(void)
{
    RSDK_THIS(PhantomEgg);

    self->direction            = FLIP_NONE;
    self->coreAnimator.frameID = 1;
    RSDK.DrawSprite(&self->coreAnimator, NULL, false);
    RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);

    self->coreAnimator.frameID = 0;
    RSDK.DrawSprite(&self->coreAnimator, NULL, false);
    RSDK.DrawSprite(&self->crackAnimator, NULL, false);
    RSDK.DrawSprite(&self->armLAnimator, NULL, false);
    RSDK.DrawSprite(&self->legAnimator, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->armLAnimator, NULL, false);
    RSDK.DrawSprite(&self->legAnimator, NULL, false);
}

void PhantomEgg_State_SetupArena(void)
{
    RSDK_THIS(PhantomEgg);

    if (++self->timer >= 8) {
        self->timer = 0;

        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - ScreenInfo->center.x;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + ScreenInfo->center.x;
        Zone->cameraBoundsT[0]      = Zone->cameraBoundsB[0] - ScreenInfo->size.y;

        PhantomEgg->boundsL = (Zone->cameraBoundsL[0] + 64) << 16;
        PhantomEgg->boundsR = (Zone->cameraBoundsR[0] - 64) << 16;
        PhantomEgg->boundsM = self->position.x;
        PhantomEgg->boundsT = (Zone->cameraBoundsT[0] + 48) << 16;
        PhantomEgg->boundsB = (Zone->cameraBoundsB[0] - 96) << 16;

        self->position.y -= 0x1000000;
        self->active = ACTIVE_NORMAL;
        self->state  = PhantomEgg_State_DimArena;
    }
}

void PhantomEgg_State_DimArena(void)
{
    RSDK_THIS(PhantomEgg);

    if (self->timer) {
        self->timer += 4;
        RSDK.SetLimitedFade(0, 1, 2, self->timer, 128, 256);

        if (self->timer >= 384) {
            self->timer     = 0;
            self->visible   = true;
            self->stateDraw = PhantomEgg_Draw_Normal;
            self->state     = PhantomEgg_State_EnterEggman;

            foreach_active(TMZCable, cable)
            {
                cable->parentPos = &self->position;
                cable->state     = TMZCable_State_Idle;
            }

            int32 missileAngles[] = { -24, -10, 10, 24 };
            int32 angle           = 0;
            int32 id              = 0;
            foreach_all(PhantomMissile, missile)
            {
                missile->angle          = missileAngles[id];
                missile->oscillateAngle = angle;
                missile->active         = ACTIVE_NORMAL;
                missile->parent         = self;
                missile->id             = id++;
                angle += 64;
            }

            Music_TransitionTrack(TRACK_EGGMAN1, 0.0125);
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
            SceneInfo->timeEnabled = false;
            ++self->timer;
        }
    }
}

void PhantomEgg_State_EnterEggman(void)
{
    RSDK_THIS(PhantomEgg);

    int32 startY = self->position.y;
    self->velocity.y += self->position.y >= self->targetPos.y ? -0x3800 : 0x3800;

    if (self->velocity.y > 0x50000)
        self->velocity.y = 0x50000;

    self->position.y += self->velocity.y;
    if (startY >= self->targetPos.y) {
        if (self->position.y < self->targetPos.y) {
            ++self->timer;
            self->velocity.y = (4 * self->velocity.y) >> 3;
        }
    }
    else {
        if (self->position.y > self->targetPos.y) {
            ++self->timer;
            self->velocity.y = (4 * self->velocity.y) >> 3;
        }
    }

    if (self->timer > 4) {
        self->timer = 0;
        self->state = PhantomEgg_State_AdjustStartingPos;
    }
}

void PhantomEgg_State_AdjustStartingPos(void)
{
    RSDK_THIS(PhantomEgg);

    self->position.y += (self->targetPos.y - self->position.y) >> 3;

    if (++self->timer == 16) {
        self->timer = 0;
        foreach_active(TMZAlert, alert) { alert->state = TMZAlert_State_Activating; }
        self->state = PhantomEgg_State_IntroHover;
    }
}

void PhantomEgg_State_IntroHover(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    RSDK.SetLimitedFade(0, 1, 2, (RSDK.Cos256(self->timer >> 2) >> 1) + 128, 128, 256);
    self->timer += 11;

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->targetPos.y;

    if (self->timer == 3960) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 14, &self->rubyAnimator, true, 0);

        SceneInfo->milliseconds = 0;
        SceneInfo->seconds      = 0;
        SceneInfo->minutes      = 0;

        PhantomRuby_PlaySfx(RUBYSFX_ATTACK1);
    }

    if (self->timer == 4092) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 32;
        fxFade->speedOut     = 32;
    }

    if (self->timer >= 4608) {
        self->timer = 0;
        self->state = PhantomEgg_State_BeginFight;

        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void PhantomEgg_State_BeginFight(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    self->position.y = BadnikHelpers_Oscillate(self->targetPos.y, 3, 11);

    if (++self->timer == 30) {
        self->timer            = 0;
        SceneInfo->timeEnabled = true;
        self->velocity.x       = Player_GetNearestPlayerX()->position.x < self->position.x ? -0x60000 : 0x60000;
        self->state            = PhantomEgg_State_MoveAround;
        CREATE_ENTITY(PhantomShield, self, self->position.x, self->position.y);
    }
}

void PhantomEgg_State_MoveAround(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    int32 startX = self->position.x;
    if (self->position.x >= self->targetPos.x - 0x100000) {
        if (self->position.x > self->targetPos.x + 0x100000)
            self->velocity.x -= 0x1800;
    }
    else {
        self->velocity.x += 0x1800;
    }

    self->velocity.x = CLAMP(self->velocity.x, -0x50000, 0x50000);
    self->position.x += self->velocity.x;

    if (startX >= self->targetPos.x - 0x100000) {
        if (startX > self->targetPos.x + 0x100000 && self->position.x < self->targetPos.x + 0x100000)
            self->velocity.x = (7 * self->velocity.x) >> 3;
    }
    else {
        if (self->position.x > self->targetPos.x - 0x100000)
            self->velocity.x = (7 * self->velocity.x) >> 3;
    }

    if (self->timer <= 0) {
        self->timer            = RSDK.Rand(0x30, 0x60);
        self->targetVelocity.x = self->targetPos.x + RSDK.Rand(-0x600000, 0x600000);
        self->targetVelocity.y = self->targetPos.y + RSDK.Rand(-0x200000, 0x200000);

        int32 angle            = RSDK.ATan2((self->targetVelocity.x - self->targetPos.x) >> 16, (self->targetVelocity.y - self->targetPos.y) >> 16);
        self->targetVelocity.x = RSDK.Cos256(angle) << 9;
        self->targetVelocity.y = RSDK.Sin256(angle) << 9;
        PhantomEgg_HandleNextAttack();
    }
    else {
        self->timer--;
    }

    self->velocity.y += ((self->targetVelocity.y - self->velocity.y) >> 4);
    self->position.y += self->velocity.y;

    if (self->position.y < PhantomEgg->boundsT && self->targetVelocity.y < 0)
        self->targetVelocity.y = -self->targetVelocity.y;

    if (self->position.y > PhantomEgg->boundsB) {
        if (self->targetVelocity.y > 0)
            self->targetVelocity.y = -self->targetVelocity.y;
    }

    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Attack_Jumped(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    Hitbox *hitbox = RSDK.GetHitbox(&self->legAnimator, 0);
    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, hitbox->bottom << 16, true)) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 4, &self->armLAnimator, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 4, &self->armRAnimator, true, 0);

        self->state = PhantomEgg_State_Attack_JumpLand;
        RSDK.PlaySfx(PhantomEgg->sfxLand, false, 255);
    }

    if (self->velocity.x < 0 && self->position.x < PhantomEgg->boundsL)
        self->velocity.x = -self->velocity.x;

    if (self->velocity.x > 0 && self->position.x > PhantomEgg->boundsR)
        self->velocity.x = -self->velocity.x;

    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Attack_JumpLand(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&self->armLAnimator);
    RSDK.ProcessAnimation(&self->armRAnimator);
    RSDK.ProcessAnimation(&self->legAnimator);

    PhantomEgg_HandleAnimations();

    Hitbox *hitbox = RSDK.GetHitbox(&self->legAnimator, 0);
    RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, hitbox->bottom << 16, 16);

    if (self->legAnimator.frameID == 5)
        PhantomEgg_HandleNextAttack();

    if (self->legAnimator.frameID == 6) {
        self->velocity.x = RSDK.Rand(0, 256) > 128 ? -0x20000 : 0x20000;
        self->velocity.y = -0x80000;
        self->state      = PhantomEgg_State_Attack_JumpAttack;

        RSDK.PlaySfx(PhantomEgg->sfxJump, false, 255);
    }

    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Attack_JumpAttack(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&self->armLAnimator);
    RSDK.ProcessAnimation(&self->armRAnimator);
    RSDK.ProcessAnimation(&self->legAnimator);

    PhantomEgg_HandleAnimations();

    if (self->velocity.x < 0 && self->position.x < PhantomEgg->boundsL)
        self->velocity.x = -self->velocity.x;

    if (self->velocity.x > 0 && self->position.x > PhantomEgg->boundsR)
        self->velocity.x = -self->velocity.x;

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->legAnimator.frameID == self->legAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &self->legAnimator, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->armLAnimator, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->armRAnimator, true, 0);

        --self->remainingJumps;
        if (self->remainingJumps > 0)
            self->state = PhantomEgg_State_Attack_Jumped;
        else
            self->state = PhantomEgg_State_MoveAround;
    }

    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Attack_CableShock(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    if (self->timer < 160 && !(self->timer & 0xF))
        RSDK.PlaySfx(PhantomEgg->sfxShock, false, 255);

    if (++self->timer == 30) {
        foreach_active(TMZCable, cable)
        {
            if (cable->state != TMZCable_State_Destroyed) {
                RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 10, &cable->animator, true, 0);
                cable->state = TMZCable_State_Charge;
            }
        }

        foreach_active(PhantomShield, shield)
        {
            RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 2, &shield->animator, true, 0);
            shield->state = PhantomShield_State_Disappear;
        }
    }

    if (self->timer == 4 * self->health + 160)
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 14, &self->rubyAnimator, true, 0);

    if ((self->rubyAnimator.animationID == 14 && self->rubyAnimator.frameID == self->rubyAnimator.frameCount - 2) || self->timer > 288) {
        self->timer = 0;

        CREATE_ENTITY(PhantomShield, self, self->position.x, self->position.y);
        if (self->legAnimator.frameID > 0) {
            self->legAnimator.frameID = 6;
            self->legAnimator.timer   = 0;
            self->state               = PhantomEgg_State_Attack_JumpLand;
        }
        else {
            self->state = PhantomEgg_State_MoveAround;
        }
    }

    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Attack_PrepareWarp(void)
{
    RSDK_THIS(PhantomEgg);

    if (!self->timer) {
        foreach_active(PhantomShield, shield)
        {
            RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 2, &shield->animator, true, 0);
            shield->state = PhantomShield_State_Disappear;
        }
    }

    if (self->palBlendPercent < 112)
        self->palBlendPercent += 4;

    RSDK.SetLimitedFade(0, 1, 2, self->palBlendPercent, 128, 256);
    self->position.x += (PhantomEgg->boundsM - self->position.x) >> 4;
    self->position.y += ((PhantomEgg->boundsB - self->position.y) - 0x400000) >> 4;

    if (++self->timer == 60) {
        self->timer      = 0;
        self->position.x = PhantomEgg->boundsM;
        self->position.y = PhantomEgg->boundsB - 0x400000;

        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 5, &self->armLAnimator, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 6, &self->armRAnimator, true, 0);
        self->state = PhantomEgg_State_Attack_GrabPlayers;
    }
}

void PhantomEgg_State_Attack_GrabPlayers(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&self->armLAnimator);
    RSDK.ProcessAnimation(&self->armRAnimator);
    RSDK.SetLimitedFade(0, 1, 2, self->palBlendPercent, 128, 256);

    if (!self->timer) {
        CREATE_ENTITY(PhantomHand, self, self->position.x - 0x400000, self->position.y)->velocity.x = -0x8000;

        EntityPhantomHand *hand = CREATE_ENTITY(PhantomHand, self, self->position.x + 0x400000, self->position.y);
        hand->direction         = FLIP_X;
        hand->velocity.x        = 0x8000;
    }

    ++self->timer;
}

void PhantomEgg_State_Attack_HandleWarp(void)
{
    RSDK_THIS(PhantomEgg);

    if (PhantomEgg->endScanline == 96) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 16;
        fxFade->speedOut     = 16;
    }

    if (self->palBlendPercent > 0)
        self->palBlendPercent -= 4;

    RSDK.ProcessAnimation(&self->armLAnimator);
    RSDK.ProcessAnimation(&self->armRAnimator);
    RSDK.SetLimitedFade(0, 1, 2, self->palBlendPercent, 128, 256);

    if (PhantomEgg->endScanline <= 0) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->armLAnimator, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->armRAnimator, true, 0);

        if (self->visible) {
            self->state = PhantomEgg_State_Attack_WarpAway;
            PhantomEgg_HandlePhantomWarp(self->phantomID);
            self->phantomID = (self->phantomID + 1) & 3;
        }
        else {
            self->state = PhantomEgg_State_Attack_WarpReturn;
            PhantomEgg_HandleReturnWarp();
            self->visible = true;
        }
    }
    else {
        PhantomEgg->endScanline -= 4;
    }
}

void PhantomEgg_State_Attack_WarpAway(void)
{
    RSDK_THIS(PhantomEgg);

    if (PhantomEgg->endScanline >= ScreenInfo->size.y) {
        RSDK.GetTileLayer(Zone->fgLayer[0])->scanlineCallback = StateMachine_None;
        RSDK.GetTileLayer(Zone->fgLayer[1])->scanlineCallback = StateMachine_None;

        self->timer   = 0;
        self->visible = false;
        self->state   = StateMachine_None;
    }
    else {
        if (self->timer > 8)
            PhantomEgg->endScanline += 2;

        if (self->timer == 60) {
            foreach_active(PhantomHand, hand) { hand->state = PhantomHand_State_BreakApart; }
        }
        ++self->timer;
    }
}

void PhantomEgg_State_Attack_WarpReturn(void)
{
    RSDK_THIS(PhantomEgg);

    if (PhantomEgg->endScanline >= ScreenInfo->size.y) {
        RSDK.GetTileLayer(Zone->fgLayer[0])->scanlineCallback = StateMachine_None;
        RSDK.GetTileLayer(Zone->fgLayer[1])->scanlineCallback = StateMachine_None;

        CREATE_ENTITY(PhantomShield, self, self->position.x, self->position.y);
        self->timer = 0;
        self->state = PhantomEgg_State_MoveAround;
    }
    else {
        if (self->timer > 8)
            PhantomEgg->endScanline += 2;

        if (self->timer == 60) {
            foreach_active(PhantomHand, hand) { hand->state = PhantomHand_State_BreakApart; }
        }
        ++self->timer;
    }
}

void PhantomEgg_State_Destroyed(void)
{
    RSDK_THIS(PhantomEgg);

    self->position.x = self->targetPos.x + RSDK.Rand(-0x20000, 0x20000);
    self->position.y = self->targetPos.y + RSDK.Rand(-0x20000, 0x20000);

    if (!RSDK.GetEntityCount(TMZCable->classID, true)) {
        int32 id = 0;
        foreach_active(PhantomMissile, missile)
        {
            switch (id++) {
                case 0:
                    missile->velocity.x = -0x20000;
                    missile->velocity.y = -0x20000;
                    missile->groundVel  = 16;
                    break;

                case 1:
                    missile->velocity.x = -0x10000;
                    missile->velocity.y = -0x40000;
                    missile->groundVel  = 8;
                    break;

                case 2:
                    missile->velocity.x = 0x10000;
                    missile->velocity.y = -0x40000;
                    missile->groundVel  = -8;
                    break;

                case 3:
                    missile->velocity.x = 0x20000;
                    missile->velocity.y = -0x20000;
                    missile->groundVel  = -16;
                    break;

                default: break;
            }

            missile->state = PhantomMissile_State_Destroyed;
        }

        foreach_active(PhantomShield, shield)
        {
            RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 2, &shield->animator, true, 0);
            shield->state = PhantomShield_State_Disappear;
        }

        CREATE_ENTITY(TMZ2Outro, NULL, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &self->legAnimator, true, 1);

        for (int32 i = 0; i < 0x100; ++i) RSDK.SetPaletteEntry(7, i, RSDK.GetPaletteEntry(1, i) & 0xFF0000);

        self->state = PhantomEgg_State_Exploding;
    }
}

void PhantomEgg_State_Exploding(void)
{
    RSDK_THIS(PhantomEgg);

    self->targetPos.x += ((PhantomEgg->boundsM - self->targetPos.x) >> 5);
    self->targetPos.y += ((PhantomEgg->boundsB - self->targetPos.y - 0x400000) >> 5);

    self->position.x = self->targetPos.x + RSDK.Rand(-0x20000, 0x20000);
    self->position.y = self->targetPos.y + RSDK.Rand(-0x20000, 0x20000);

    PhantomEgg_Explode(&self->hitbox);

    if (++self->timer == 152) {
        self->timer      = 0;
        self->position.x = PhantomEgg->boundsM;
        self->position.y = PhantomEgg->boundsB - 0x400000;

        bool32 goodEnd = (CHECK_CHARACTER_ID(ID_SONIC, 1) || (CHECK_CHARACTER_ID(ID_KNUCKLES, 1) && CHECK_CHARACTER_ID(ID_KNUCKLES, 2)))
                         && SaveGame_AllChaosEmeralds();

#if MANIA_USE_PLUS
        if (SceneInfo->filter & FILTER_ENCORE)
            goodEnd = false; // no ERZ for encore modes
#endif
        if (goodEnd) {
            self->state = PhantomEgg_State_StartGoodEnd;
            Music_FadeOut(0.0125);
        }
        else {
            self->state = PhantomEgg_State_StartBadEnd;
        }
    }
}

void PhantomEgg_State_StartBadEnd(void)
{
    RSDK_THIS(PhantomEgg);

    if (++self->timer == 30) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 1, &self->crackAnimator, false, 0);
        self->stateDraw = PhantomEgg_Draw_Cracked;

        RSDK.PlaySfx(PhantomEgg->sfxRocketJet, false, 255);

        EntityPhantomRuby *ruby = CREATE_ENTITY(PhantomRuby, NULL, self->position.x, self->position.y + 0x100000);
        ruby->state             = PhantomRuby_State_MoveRotateGravity_CheckGround;
        ruby->velocity.x        = -0x10000;
        ruby->velocity.y        = -0x20000;
        self->state             = PhantomEgg_State_CrackOpen;
    }
}

void PhantomEgg_State_CrackOpen(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&self->crackAnimator);

    if (++self->timer == 60) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 512;
        fxFade->wait         = 16;
        fxFade->speedOut     = 16;
        RSDK.PlaySfx(PhantomEgg->sfxExplosion3, false, 255);
    }

    if (self->timer == 64) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 2, &self->crackAnimator, false, 0);

        self->drawGroup = Zone->objectDrawGroup[0] + 1;
        self->stateDraw = StateMachine_None;

        EntityEggman *eggman = CREATE_ENTITY(Eggman, NULL, self->position.x, self->position.y + 0x100000);
        RSDK.SetSpriteAnimation(Eggman->aniFrames, 7, &eggman->animator, true, 0);
        eggman->onGround = false;
        eggman->state    = Eggman_State_FallAndCollide;
    }

    foreach_active(Eggman, eggman)
    {
        eggman->position.x = self->position.x;
        eggman->position.y = self->position.y;
        eggman->velocity.y = 0;
    }

    if (self->timer == 96) {
        self->timer = 0;
        self->state = PhantomEgg_State_CrackedExploding;
    }
}

void PhantomEgg_State_CrackedExploding(void)
{
    RSDK_THIS(PhantomEgg);

    if (!(Zone->timer & 0xF)) {
        int32 x                    = self->position.x + RSDK.Rand(-0x380000, -0x180000);
        int32 y                    = self->position.y + RSDK.Rand(-0x300000, -0x100000);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), x, y);
        explosion->drawGroup       = Zone->objectDrawGroup[1];
    }

    if ((Zone->timer & 0xF) == 8) {
        int32 x                    = self->position.x + RSDK.Rand(0x180000, 0x380000);
        int32 y                    = self->position.y + RSDK.Rand(-0x300000, -0x100000);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), x, y);
        explosion->drawGroup       = Zone->objectDrawGroup[1];
    }

    if (++self->timer == 120)
        self->state = StateMachine_None;
}

void PhantomEgg_State_StartGoodEnd(void)
{
    RSDK_THIS(PhantomEgg);

    if (self->timer < 256 && !(Zone->timer % 3)) {
        RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);

        if (Zone->timer & 8) {
            int32 x = self->position.x + RSDK.Rand(-0x800000, 0x800000);
            int32 y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom + 64) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }

    if (++self->timer == 120) {
        CREATE_ENTITY(FXRuby, NULL, self->position.x, self->position.y + 0x100000)->radiusSpeed = 3;
        PhantomRuby_PlaySfx(RUBYSFX_REDCUBE);
    }

    if (self->timer == 320) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 16;
        fxFade->wait         = 32;
        fxFade->fadeOutBlack = 1;
        fxFade->speedOut     = 16;
        PhantomRuby_PlaySfx(RUBYSFX_ATTACK1);
    }

    if (self->timer >= 512) {
        if (globals->saveSlotID != NO_SAVE_SLOT) {
            if (self->timer == 512) {
                if (Zone_IsZoneLastAct())
                    GameProgress_MarkZoneCompleted(Zone_GetZoneID());

                SaveGame_SaveFile(PhantomEgg_SaveGameCB);
                UIWaitSpinner_StartWait();
            }

            if (PhantomEgg->savedGameProgress)
                UIWaitSpinner_FinishWait();
        }

        if (globals->saveSlotID == NO_SAVE_SLOT || PhantomEgg->savedGameProgress) {
            ++SceneInfo->listPos;
            RSDK.LoadScene();
            self->state = 0;
        }
    }
}

#if MANIA_USE_PLUS
void PhantomEgg_SaveGameCB(bool32 success) { PhantomEgg->savedGameProgress = true; }
#else
void PhantomEgg_SaveGameCB(void) { PhantomEgg->savedGameProgress = true; }
#endif

#if RETRO_INCLUDE_EDITOR
void PhantomEgg_EditorDraw(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 0, &self->coreAnimator, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 17, &self->eggmanAnimator, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->armLAnimator, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &self->legAnimator, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 13, &self->rubyAnimator, false, 0);

    PhantomEgg_Draw_Normal();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE, WIDE_SCR_XCENTER, 0, 1 | 2 | 4 | 0, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void PhantomEgg_EditorLoad(void)
{
    PhantomEgg->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomEgg.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PhantomEgg, type);
    RSDK_ENUM_VAR("Eggman", PHANTOMEGG_EGGMAN);
}
#endif

void PhantomEgg_Serialize(void) { RSDK_EDITABLE_VAR(PhantomEgg, VAR_ENUM, type); }
