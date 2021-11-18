#include "SonicMania.h"

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
        RSDK.DrawSprite(&self->animator1, NULL, false);
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
        self->type             = voidToInt(data);
        if (!data) {
            self->visible       = false;
            self->drawOrder     = Zone->drawOrderLow;
            self->hitbox.left   = -24;
            self->hitbox.top    = -24;
            self->hitbox.right  = 24;
            self->hitbox.bottom = 24;
            self->drawFX        = FX_FLIP;
            self->active        = ACTIVE_NORMAL;
            self->health        = 16;
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 0, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 17, &self->animator6, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->animator4, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &self->animator3, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 13, &self->animator7, true, 0);
            self->field_80.x = self->position.x;
            self->field_80.y = self->position.y;
            self->state      = PhantomEgg_State_SetupArena;
        }
    }
}

void PhantomEgg_StageLoad(void)
{
    PhantomEgg->aniFrames         = RSDK.LoadSpriteAnimation("Phantom/PhantomEgg.bin", SCOPE_STAGE);
    PhantomEgg->savedGameProgress = false;
#if RETRO_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreTMZ3.act", 0xFF);
        RSDK.LoadPalette(1, "EncoreTMZ3.act", 0xFF);
    }
#endif
    if (!PhantomEgg->setupPalette) {
        RSDK.CopyPalette(0, 0, 1, 0, 128);
        PhantomEgg->setupPalette = true;
    }
    RSDK.CopyPalette(1, 128, 0, 128, 128);
    RSDK.CopyPalette(0, 0, 4, 0, 128);
    PhantomEgg->superFlag     = false;
    PhantomEgg->sfxHit        = RSDK.GetSFX("Stage/BossHit.wav");
    PhantomEgg->sfxExplosion2 = RSDK.GetSFX("Stage/Explosion2.wav");
    PhantomEgg->sfxRocketJet  = RSDK.GetSFX("Stage/RocketJet.wav");
    PhantomEgg->sfxExplosion3 = RSDK.GetSFX("Stage/Explosion3.wav");
    PhantomEgg->sfxJump       = RSDK.GetSFX("TMZ3/Jump.wav");
    PhantomEgg->sfxLand       = RSDK.GetSFX("TMZ3/Land.wav");
    PhantomEgg->sfxRepel      = RSDK.GetSFX("TMZ3/Repel.wav");
    PhantomEgg->sfxShield     = RSDK.GetSFX("TMZ3/Shield.wav");
    PhantomEgg->sfxShock      = RSDK.GetSFX("TMZ3/Shock.wav");
    PhantomEgg->sfxSummon     = RSDK.GetSFX("TMZ3/Summon.wav");
    PhantomEgg->sfxMissile    = RSDK.GetSFX("TMZ3/Missile.wav");
}

void PhantomEgg_HandleAnimations(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&self->animator6);
    if (self->animator7.animationID == 14) {
        RSDK.ProcessAnimation(&self->animator7);
        if (self->animator7.frameID == self->animator7.frameCount - 1)
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 13, &self->animator7, true, 0);
    }

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    if (self->animator6.animationID == 17) {
        self->animator6.animationSpeed = 0;
    }
    else {
        if (self->animator6.animationID == 18 || self->animator6.animationID == 19) {
            if (self->animator6.frameID == self->animator6.frameCount - 1)
                RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 17, &self->animator6, true, 0);
        }
    }
}

void PhantomEgg_CheckPlayerCollisions(void)
{
    RSDK_THIS(PhantomEgg);

    if (RSDK.GetEntityCount(PhantomShield->objectID, true) <= 0) {
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
        int id = (-2 - RSDK.GetEntityCount(TMZCable->objectID, true)) & 3;

        foreach_active(TMZCable, cable)
        {
            if (cable->cableID == id || !self->health) {
                RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 9, &cable->animator, true, 0);
                cable->state = TMZCable_Unknown6;
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
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 20, &self->animator6, true, 0);
        self->field_80.x          = self->position.x;
        self->field_80.y          = self->position.y;
        self->state               = PhantomEgg_State_Unknown16;
        self->timer               = 0;
        PhantomEgg->superFlag       = true;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 19, &self->animator6, true, 0);
        RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
    }
}

void PhantomEgg_Explode(Hitbox *hitbox)
{
    RSDK_THIS(PhantomEgg);

    if (!(Zone->timer % 7)) {
        RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);
        if (!(Zone->timer & 8)) {
            int x = self->position.x + (RSDK.Rand(hitbox->left, hitbox->right) << 16);
            int y = self->position.y + (RSDK.Rand(hitbox->top, hitbox->bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }
}

void PhantomEgg_HandleNextAttack(void)
{
    RSDK_THIS(PhantomEgg);

    switch (self->attackStateTable[self->attackTimer]) {
        case 1:
            if (self->state != PhantomEgg_State_Unknown8) {
                self->groundVel  = 0;
                self->velocity.x = 0;
                self->field_70   = 3;
                self->state      = PhantomEgg_State_Unknown7;
                self->attackTimer++;
                break;
            }
            break;
        case 2:
            self->timer = 0;
            self->state = PhantomEgg_State_Unknown10;
            self->attackTimer++;
            break;
        case 3:
            if (self->position.y >= PhantomEgg->boundsB - 0xA00000) {
                foreach_active(PhantomMissile, missile) { missile->timer = 16 * missile->id + 8; }
                ++self->attackTimer;
            }
            break;
        case 4:
            self->timer = 0;
            self->state = PhantomEgg_State_Unknown11;
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &self->animator3, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->animator4, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->animator5, true, 0);
            self->attackTimer++;
            break;
        default: self->attackTimer++; break;
    }
    self->attackTimer = self->attackTimer % 32;
}

void PhantomEgg_SetupScanlineCB(void)
{
    foreach_all(PhantomEgg, phantomEgg)
    {
        PhantomEgg->startScanline                          = ScreenInfo->centerY;
        PhantomEgg->endScanline                            = ScreenInfo->height;
        RSDK.GetSceneLayer(Zone->fgLow)->scanlineCallback  = PhantomEgg_ScanlineCB;
        RSDK.GetSceneLayer(Zone->fgHigh)->scanlineCallback = PhantomEgg_ScanlineCB;
        PhantomRuby_PlaySFX(RUBYSFX_ATTACK1);
        phantomEgg->timer = 0;
        phantomEgg->state = PhantomEgg_State_Unknown13;
        foreach_break;
    }
}

void PhantomEgg_HandlePhantomWarp(uint8 phantomID)
{
    RSDK_THIS(PhantomEgg);

    int id                    = phantomID & 3;
    PhantomEgg->boundsStoreL1 = Zone->screenBoundsL1[0];
    PhantomEgg->boundsStoreR1 = Zone->screenBoundsR1[0];
    PhantomEgg->boundsStoreT1 = Zone->screenBoundsT1[0];
    PhantomEgg->boundsStoreB1 = Zone->screenBoundsB1[0];

    Entity *phantomHeavy = NULL;

    switch (phantomID & 3) {
        case 0: {
            foreach_all(PhantomGunner, gunner)
            {
                phantomHeavy = (Entity *)gunner;
                foreach_break;
            }
            break;
        }
        case 1: {
            foreach_all(PhantomShinobi, shinobi)
            {
                phantomHeavy = (Entity *)shinobi;
                foreach_break;
            }
            break;
        }
        case 2: {
            foreach_all(PhantomMystic, mystic)
            {
                phantomHeavy = (Entity *)mystic;
                foreach_break;
            }
            break;
        }
        case 3: {
            foreach_all(PhantomRider, rider)
            {
                phantomHeavy = (Entity *)rider;
                foreach_break;
            }
            break;
        }
    }

    if (phantomHeavy) {
        int phantomSlot                = RSDK.GetEntityID(phantomHeavy);
        EntityPlatformNode *nodeTop    = RSDK_GET_ENTITY(phantomSlot + 1, PlatformNode);
        EntityPlatformNode *nodeBottom = RSDK_GET_ENTITY(phantomSlot + 2, PlatformNode);
        EntityPlatformNode *nodeLeft   = RSDK_GET_ENTITY(phantomSlot + 3, PlatformNode);
        EntityPlatformNode *nodeRight  = RSDK_GET_ENTITY(phantomSlot + 4, PlatformNode);

        int offsetX = nodeBottom->position.x - PhantomEgg->boundsM;
        int offsetY = nodeBottom->position.y - (PhantomEgg->boundsStoreB1 << 16);

        for (int p = 0; p < Player->playerCount; ++p) {
            Zone->screenBoundsL1[p] = nodeLeft->position.x >> 16;
            Zone->screenBoundsR1[p] = nodeRight->position.x >> 16;
            Zone->screenBoundsT1[p] = nodeTop->position.y >> 16;
            Zone->screenBoundsB1[p] = nodeBottom->position.y >> 16;

            Zone->screenBoundsL2[p] = nodeLeft->position.x;
            Zone->screenBoundsR2[p] = nodeRight->position.x;
            Zone->screenBoundsT2[p] = nodeTop->position.y;
            Zone->screenBoundsB2[p] = nodeBottom->position.y;
            Zone->deathBoundary[p]  = nodeBottom->position.y + 0x800000;
        }

        phantomHeavy->active = ACTIVE_NORMAL;

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->boundsL      = Zone->screenBoundsL1[0];
        camera->boundsR      = Zone->screenBoundsR1[0];
        camera->boundsT      = Zone->screenBoundsT1[0];
        camera->boundsB      = Zone->screenBoundsB1[0];

        if (id == 3) {
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
    RSDK_THIS(PhantomEgg);

    for (int p = 0; p < Player->playerCount; ++p) {
        Zone->screenBoundsL1[p] = PhantomEgg->boundsStoreL1;
        Zone->screenBoundsR1[p] = PhantomEgg->boundsStoreR1;
        Zone->screenBoundsT1[p] = PhantomEgg->boundsStoreT1;
        Zone->screenBoundsB1[p] = PhantomEgg->boundsStoreB1;

        Zone->screenBoundsL2[p] = PhantomEgg->boundsStoreL1 << 16;
        Zone->screenBoundsR2[p] = PhantomEgg->boundsStoreR1 << 16;
        Zone->screenBoundsT2[p] = PhantomEgg->boundsStoreT1 << 16;
        Zone->screenBoundsB2[p] = PhantomEgg->boundsStoreB1 << 16;
        Zone->deathBoundary[p]  = Zone->screenBoundsB2[p];
    }

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    camera->boundsL      = Zone->screenBoundsL1[0];
    camera->boundsR      = Zone->screenBoundsR1[0];
    camera->boundsT      = Zone->screenBoundsT1[0];
    camera->boundsB      = Zone->screenBoundsB1[0];
    camera->position.x   = PhantomEgg->boundsM;
    camera->position.y   = PhantomEgg->boundsStoreB1 - (ScreenInfo->centerY << 16);

    foreach_active(Player, player)
    {
        player->position.x = PhantomEgg->boundsM;
        player->position.y = (PhantomEgg->boundsStoreB1 - 40) << 16;
    }

    foreach_all(PhantomMissile, missile) { missile->active = ACTIVE_NORMAL; }
    foreach_all(TMZCable, cable) { cable->active = ACTIVE_NORMAL; }
}

void PhantomEgg_ScanlineCB(ScanlineInfo *scanlines)
{
    TileLayer *fgLow = RSDK.GetSceneLayer(Zone->fgLow);
    RSDK.ProcessParallax(fgLow);

    int line = 0;
    if (PhantomEgg->startScanline - PhantomEgg->endScanline >= 0)
        line = PhantomEgg->startScanline - PhantomEgg->endScanline;

    int lineY = scanlines[line].position.y;
    for (int l = 0; l < line; ++l) scanlines[l].position.y = lineY;

    line = PhantomEgg->startScanline + PhantomEgg->endScanline;
    if (line > ScreenInfo->height)
        line = ScreenInfo->height;

    lineY = scanlines[line].position.y;
    for (int l = line; l < ScreenInfo->height; ++l) scanlines[l].position.y = lineY;
}

void PhantomEgg_StateDraw_Normal(void)
{
    RSDK_THIS(PhantomEgg);

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(3, 32, 0, 32, 10);
        RSDK.CopyPalette(3, 128, 0, 128, 16);
        RSDK.SetPaletteEntry(0, 128, 0xF0F0F0);

        self->direction         = FLIP_NONE;
        self->animator2.frameID = 1;
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator6, NULL, false);

        self->animator2.frameID = 0;
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator4, NULL, false);
        RSDK.DrawSprite(&self->animator3, NULL, false);
        RSDK.DrawSprite(&self->animator7, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator4, NULL, false);
        RSDK.DrawSprite(&self->animator3, NULL, false);

        RSDK.CopyPalette(1, 32, 0, 32, 10);
        RSDK.CopyPalette(1, 128, 0, 128, 16);
        RSDK.SetPaletteEntry(0, 128, 0x000000);
    }
    else {
        self->direction         = FLIP_NONE;
        self->animator2.frameID = 1;
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator6, NULL, false);

        self->animator2.frameID = 0;
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator4, NULL, false);
        RSDK.DrawSprite(&self->animator3, NULL, false);
        RSDK.DrawSprite(&self->animator7, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator4, NULL, false);
        RSDK.DrawSprite(&self->animator3, NULL, false);
    }
}

void PhantomEgg_StateDraw_Cracked(void)
{
    RSDK_THIS(PhantomEgg);

    self->direction         = FLIP_NONE;
    self->animator2.frameID = 1;
    RSDK.DrawSprite(&self->animator2, 0, false);
    RSDK.DrawSprite(&self->animator6, 0, false);

    self->animator2.frameID = 0;
    RSDK.DrawSprite(&self->animator2, 0, false);
    RSDK.DrawSprite(&self->animator1, 0, false);
    RSDK.DrawSprite(&self->animator4, 0, false);
    RSDK.DrawSprite(&self->animator3, 0, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator4, 0, false);
    RSDK.DrawSprite(&self->animator3, 0, false);
}

void PhantomEgg_State_SetupArena(void)
{
    RSDK_THIS(PhantomEgg);

    if (++self->timer >= 8) {
        self->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsL1[0]     = (self->position.x >> 16) - ScreenInfo->centerX;
        Zone->screenBoundsR1[0]     = (self->position.x >> 16) + ScreenInfo->centerX;
        Zone->screenBoundsT1[0]     = Zone->screenBoundsB1[0] - ScreenInfo->height;
        PhantomEgg->boundsL         = (Zone->screenBoundsL1[0] + 64) << 16;
        PhantomEgg->boundsR         = (Zone->screenBoundsR1[0] - 64) << 16;
        PhantomEgg->boundsM         = self->position.x;
        PhantomEgg->boundsT         = (Zone->screenBoundsT1[0] + 48) << 16;
        PhantomEgg->boundsB         = (Zone->screenBoundsB1[0] - 96) << 16;
        self->position.y -= 0x1000000;
        self->active = ACTIVE_NORMAL;
        self->state  = PhantomEgg_State_Unknown1;
    }
}

void PhantomEgg_State_Unknown1(void)
{
    RSDK_THIS(PhantomEgg);

    if (self->timer) {
        self->timer += 4;
        RSDK.SetLimitedFade(0, 1, 2, self->timer, 128, 256);
        if (self->timer >= 384) {
            self->timer     = 0;
            self->visible   = true;
            self->stateDraw = PhantomEgg_StateDraw_Normal;
            self->state     = PhantomEgg_State_Unknown2;

            foreach_active(TMZCable, cable)
            {
                cable->posPtr = &self->position;
                cable->state  = TMZCable_Unknown2;
            }

            int missileAngles[] = { -24, -10, 10, 24 };
            int angle           = 0;
            int id              = 0;
            foreach_all(PhantomMissile, missile)
            {
                missile->angle    = missileAngles[id];
                missile->field_60 = angle;
                missile->active   = ACTIVE_NORMAL;
                missile->parent   = (Entity *)self;
                missile->id       = id++;
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

void PhantomEgg_State_Unknown2(void)
{
    RSDK_THIS(PhantomEgg);

    int startY = self->position.y;
    if (self->position.y >= self->field_80.y)
        self->velocity.y -= 0x3800;
    else
        self->velocity.y += 0x3800;

    if (self->velocity.y > 0x50000)
        self->velocity.y = 0x50000;

    self->position.y += self->velocity.y;
    if (startY >= self->field_80.y) {
        if (self->position.y < self->field_80.y) {
            ++self->timer;
            self->velocity.y = (4 * self->velocity.y) >> 3;
        }
    }
    else if (self->position.y > self->field_80.y) {
        ++self->timer;
        self->velocity.y = (4 * self->velocity.y) >> 3;
    }

    if (self->timer > 4) {
        self->timer = 0;
        self->state = PhantomEgg_State_Unknown3;
    }
}

void PhantomEgg_State_Unknown3(void)
{
    RSDK_THIS(PhantomEgg);

    ++self->timer;
    self->position.y += (self->field_80.y - self->position.y) >> 3;
    if (self->timer == 16) {
        self->timer = 0;
        foreach_active(TMZAlert, alert) { alert->state = TMZAlert_Unknown1; }
        self->state = PhantomEgg_State_Unknown4;
    }
}

void PhantomEgg_State_Unknown4(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    RSDK.SetLimitedFade(0, 1, 2, (RSDK.Cos256(self->timer >> 2) >> 1) + 128, 128, 256);
    self->timer += 11;

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->field_80.y;

    if (self->timer == 3960) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 14, &self->animator7, true, 0);
        SceneInfo->milliseconds = 0;
        SceneInfo->seconds      = 0;
        SceneInfo->minutes      = 0;
        PhantomRuby_PlaySFX(1);
    }

    if (self->timer == 4092) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 32;
        fxFade->speedOut     = 32;
    }

    if (self->timer >= 4608) {
        self->timer = 0;
        self->state = PhantomEgg_State_Unknown5;
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void PhantomEgg_State_Unknown5(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->field_80.y;

    if (++self->timer == 30) {
        self->timer               = 0;
        SceneInfo->timeEnabled = true;
        if (Player_GetNearestPlayerX()->position.x < self->position.x)
            self->velocity.x = -0x60000;
        else
            self->velocity.x = 0x60000;
        self->state = PhantomEgg_State_Unknown6;
        CREATE_ENTITY(PhantomShield, self, self->position.x, self->position.y);
    }
}

void PhantomEgg_State_Unknown6(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    int startX = self->position.x;
    if (self->position.x >= self->field_80.x - 0x100000) {
        if (self->position.x > self->field_80.x + 0x100000)
            self->velocity.x -= 0x1800;
    }
    else {
        self->velocity.x += 0x1800;
    }

    self->velocity.x = clampVal(self->velocity.x, -0x50000, 0x50000);
    self->position.x += self->velocity.x;

    if (startX >= self->field_80.x - 0x100000) {
        if (startX > self->field_80.x + 0x100000 && self->position.x < self->field_80.x + 0x100000)
            self->velocity.x = (7 * self->velocity.x) >> 3;
    }
    else if (self->position.x > self->field_80.x - 0x100000) {
        self->velocity.x = (7 * self->velocity.x) >> 3;
    }

    if (self->timer <= 0) {
        self->timer    = RSDK.Rand(0x30, 0x60);
        self->field_88 = self->field_80.x + RSDK.Rand(-0x600000, 0x600000);
        self->field_8C = RSDK.Rand(-0x200000, 0x200000) + self->field_80.y;

        int angle        = RSDK.ATan2((self->field_88 - self->field_80.x) >> 16, (self->field_8C - self->field_80.y) >> 16);
        self->field_88 = RSDK.Cos256(angle) << 9;
        self->field_8C = RSDK.Sin256(angle) << 9;
        PhantomEgg_HandleNextAttack();
    }
    else {
        self->timer--;
    }

    self->velocity.y = self->velocity.y + ((self->field_8C - self->velocity.y) >> 4);
    self->position.y += self->velocity.y;

    if (self->position.y < PhantomEgg->boundsT && self->field_8C < 0)
        self->field_8C = -self->field_8C;

    if (self->position.y > PhantomEgg->boundsB) {
        if (self->field_8C > 0)
            self->field_8C = -self->field_8C;
    }

    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Unknown7(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();
    self->position.x += self->velocity.x;
    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;

    Hitbox *hitbox = RSDK.GetHitbox(&self->animator3, 0);
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, hitbox->bottom << 16, true) == true) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 4, &self->animator4, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 4, &self->animator5, true, 0);
        self->state = PhantomEgg_State_Unknown8;
        RSDK.PlaySfx(PhantomEgg->sfxLand, false, 255);
    }

    if (self->velocity.x < 0) {
        if (self->position.x < PhantomEgg->boundsL)
            self->velocity.x = -self->velocity.x;
    }
    if (self->velocity.x > 0 && self->position.x > PhantomEgg->boundsR)
        self->velocity.x = -self->velocity.x;

    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Unknown8(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator5);
    RSDK.ProcessAnimation(&self->animator3);
    PhantomEgg_HandleAnimations();

    Hitbox *hitbox = RSDK.GetHitbox(&self->animator3, 0);
    RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, hitbox->bottom << 16, 16);
    if (self->animator3.frameID == 5)
        PhantomEgg_HandleNextAttack();
    if (self->animator3.frameID == 6) {
        self->velocity.y = -0x80000;
        self->state      = PhantomEgg_State_Unknown9;

        if (RSDK.Rand(0, 256) > 128)
            self->velocity.x = -0x20000;
        else
            self->velocity.x = 0x20000;
        RSDK.PlaySfx(PhantomEgg->sfxJump, false, 255);
    }
    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Unknown9(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator5);
    RSDK.ProcessAnimation(&self->animator3);
    PhantomEgg_HandleAnimations();

    if (self->velocity.x < 0) {
        if (self->position.x < PhantomEgg->boundsL)
            self->velocity.x = -self->velocity.x;
    }
    if (self->velocity.x > 0 && self->position.x > PhantomEgg->boundsR)
        self->velocity.x = -self->velocity.x;

    self->position.x += self->velocity.x;
    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;
    if (self->animator3.frameID == self->animator3.frameCount - 1) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->animator4, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->animator5, true, 0);
        --self->field_70;
        if (self->field_70 > 0)
            self->state = PhantomEgg_State_Unknown7;
        else
            self->state = PhantomEgg_State_Unknown6;
    }
    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Unknown10(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    if (self->timer < 160 && !(self->timer & 0xF))
        RSDK.PlaySfx(PhantomEgg->sfxShock, false, 255);

    if (++self->timer == 30) {
        foreach_active(TMZCable, cable)
        {
            if (cable->state != TMZCable_Unknown6) {
                RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 10, &cable->animator, true, 0);
                cable->state = TMZCable_Unknown3;
            }
        }

        foreach_active(PhantomShield, shield)
        {
            RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 2, &shield->animator, true, 0);
            shield->state = PhantomShield_Unknown3;
        }
    }

    if (self->timer == 4 * self->health + 160)
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 14, &self->animator7, true, 0);

    if (self->animator7.animationID == 14 && self->animator7.frameID == self->animator7.frameCount - 2 || self->timer > 288) {
        self->timer = 0;
        CREATE_ENTITY(PhantomShield, self, self->position.x, self->position.y);
        if (self->animator3.frameID > 0) {
            self->animator3.frameID        = 6;
            self->animator3.animationTimer = 0;
            self->state                    = PhantomEgg_State_Unknown8;
        }
        else {
            self->state = PhantomEgg_State_Unknown6;
        }
    }
    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Unknown11(void)
{
    RSDK_THIS(PhantomEgg);

    if (!self->timer) {
        foreach_active(PhantomShield, shield)
        {
            RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 2, &shield->animator, true, 0);
            shield->state = PhantomShield_Unknown3;
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
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 5, &self->animator4, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 6, &self->animator5, true, 0);
        self->state = PhantomEgg_State_Unknown12;
    }
}

void PhantomEgg_State_Unknown12(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator5);
    RSDK.SetLimitedFade(0, 1, 2, self->palBlendPercent, 128, 256);
    if (!self->timer) {
        CREATE_ENTITY(PhantomHand, self, self->position.x - 0x400000, self->position.y)->velocity.x = -0x8000;
        EntityPhantomHand *hand = CREATE_ENTITY(PhantomHand, self, self->position.x + 0x400000, self->position.y);
        hand->direction         = FLIP_X;
        hand->velocity.x        = 0x8000;
    }
    ++self->timer;
}

void PhantomEgg_State_Unknown13(void)
{
    RSDK_THIS(PhantomEgg);

    if (PhantomEgg->endScanline == 96) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 16;
        fxFade->speedOut     = 16;
    }

    if (self->palBlendPercent > 0)
        self->palBlendPercent -= 4;

    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator5);
    RSDK.SetLimitedFade(0, 1, 2, self->palBlendPercent, 128, 256);

    if (PhantomEgg->endScanline <= 0) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->animator4, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->animator5, true, 0);
        if (self->visible) {
            self->state = PhantomEgg_State_Unknown14;
            PhantomEgg_HandlePhantomWarp(self->phantomID);
            self->phantomID = (self->phantomID + 1) & 3;
        }
        else {
            self->state = PhantomEgg_State_Unknown15;
            PhantomEgg_HandleReturnWarp();
            self->visible = true;
        }
    }
    else
        PhantomEgg->endScanline -= 4;
}

void PhantomEgg_State_Unknown14(void)
{
    RSDK_THIS(PhantomEgg);

    if (PhantomEgg->endScanline >= ScreenInfo->height) {
        RSDK.GetSceneLayer(Zone->fgLow)->scanlineCallback  = NULL;
        RSDK.GetSceneLayer(Zone->fgHigh)->scanlineCallback = NULL;

        self->timer   = 0;
        self->visible = 0;
        self->state   = 0;
    }
    else {
        if (self->timer > 8)
            PhantomEgg->endScanline += 2;

        if (self->timer == 60) {
            foreach_active(PhantomHand, hand) { hand->state = PhantomHand_State_Unknown9; }
        }
        ++self->timer;
    }
}

void PhantomEgg_State_Unknown15(void)
{
    RSDK_THIS(PhantomEgg);

    if (PhantomEgg->endScanline >= ScreenInfo->height) {
        RSDK.GetSceneLayer(Zone->fgLow)->scanlineCallback  = NULL;
        RSDK.GetSceneLayer(Zone->fgHigh)->scanlineCallback = NULL;

        CREATE_ENTITY(PhantomShield, self, self->position.x, self->position.y);
        self->timer = 0;
        self->state = PhantomEgg_State_Unknown6;
    }
    else {
        if (self->timer > 8)
            PhantomEgg->endScanline += 2;

        if (self->timer == 60) {
            foreach_active(PhantomHand, hand) { hand->state = PhantomHand_State_Unknown9; }
        }
        ++self->timer;
    }
}

void PhantomEgg_State_Unknown16(void)
{
    RSDK_THIS(PhantomEgg);

    self->position.x = self->field_80.x + RSDK.Rand(-0x20000, 0x20000);
    self->position.y = self->field_80.y + RSDK.Rand(-0x20000, 0x20000);

    if (!RSDK.GetEntityCount(TMZCable->objectID, true)) {
        int id = 0;
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
        }

        foreach_active(PhantomShield, shield)
        {
            RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 2, &shield->animator, true, 0);
            shield->state = PhantomShield_Unknown3;
        }

        CREATE_ENTITY(TMZ2Outro, NULL, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &self->animator3, true, 1);

        for (int i = 0; i < 0x100; ++i) {
            RSDK.SetPaletteEntry(7, i, RSDK.GetPaletteEntry(1, i) & 0xFF0000);
        }

        self->state = PhantomEgg_State_Unknown17;
    }
}

void PhantomEgg_State_Unknown17(void)
{
    RSDK_THIS(PhantomEgg);

    self->field_80.x = self->field_80.x + ((PhantomEgg->boundsM - self->field_80.x) >> 5);
    self->field_80.y = self->field_80.y + ((PhantomEgg->boundsB - self->field_80.y - 0x400000) >> 5);
    self->position.x = self->field_80.x + RSDK.Rand(-0x20000, 0x20000);
    self->position.y = self->field_80.y + RSDK.Rand(-0x20000, 0x20000);
    PhantomEgg_Explode(&self->hitbox);

    if (++self->timer == 152) {
        self->timer      = 0;
        self->position.x = PhantomEgg->boundsM;
        self->position.y = PhantomEgg->boundsB - 0x400000;

        bool32 goodEnd = (checkPlayerID(ID_SONIC, 1) || (checkPlayerID(ID_KNUCKLES, 1) && checkPlayerID(ID_KNUCKLES, 2)))
                         && SaveGame->saveRAM->chaosEmeralds == 0x7F;

#if RETRO_USE_PLUS
        if (SceneInfo->filter & FILTER_ENCORE)
            goodEnd = false; // no ERZ for encore modes
#endif
        if (goodEnd) {
            self->state = PhantomEgg_State_Unknown21;
            Music_FadeOut(0.0125);
        }
        else {
            self->state = PhantomEgg_State_Unknown18;
        }
    }
}

void PhantomEgg_State_Unknown18(void)
{
    RSDK_THIS(PhantomEgg);

    if (++self->timer == 30) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 1, &self->animator1, false, 0);
        self->stateDraw = PhantomEgg_StateDraw_Cracked;
        RSDK.PlaySfx(PhantomEgg->sfxRocketJet, false, 255);

        EntityPhantomRuby *ruby = CREATE_ENTITY(PhantomRuby, NULL, self->position.x, self->position.y + 0x100000);
        ruby->state             = PhantomRuby_Unknown8;
        ruby->velocity.x        = -0x10000;
        ruby->velocity.y        = -0x20000;
        self->state           = PhantomEgg_State_Unknown19;
    }
}

void PhantomEgg_State_Unknown19(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&self->animator1);
    if (++self->timer == 60) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 512;
        fxFade->wait         = 16;
        fxFade->speedOut     = 16;
        RSDK.PlaySfx(PhantomEgg->sfxExplosion3, false, 255);
    }
    if (self->timer == 64) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 2, &self->animator1, false, 0);
        self->drawOrder = Zone->drawOrderLow + 1;
        self->stateDraw = StateMachine_None;

        EntityEggman *eggman = CREATE_ENTITY(Eggman, NULL, self->position.x, self->position.y + 0x100000);
        RSDK.SetSpriteAnimation(Eggman->aniFrames, 7, &eggman->animator, true, 0);
        eggman->onGround = false;
        eggman->state    = Eggman_Unknown6;
    }

    foreach_active(Eggman, eggman)
    {
        eggman->position.x = self->position.x;
        eggman->position.y = self->position.y;
        eggman->velocity.y = 0;
    }

    if (self->timer == 96) {
        self->timer = 0;
        self->state = PhantomEgg_State_Unknown20;
    }
}

void PhantomEgg_State_Unknown20(void)
{
    RSDK_THIS(PhantomEgg);

    if (!(Zone->timer & 0xF)) {
        int x                      = self->position.x + RSDK.Rand(-0x380000, -0x180000);
        int y                      = self->position.y + RSDK.Rand(-0x300000, -0x100000);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), x, y);
        explosion->drawOrder       = Zone->drawOrderHigh;
    }

    if ((Zone->timer & 0xF) == 8) {
        int x                      = self->position.x + RSDK.Rand(0x180000, 0x380000);
        int y                      = self->position.y + RSDK.Rand(-0x300000, -0x100000);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), x, y);
        explosion->drawOrder       = Zone->drawOrderHigh;
    }
    if (++self->timer == 120)
        self->state = 0;
}

void PhantomEgg_State_Unknown21(void)
{
    RSDK_THIS(PhantomEgg);

    if (self->timer < 256 && !(Zone->timer % 3)) {
        RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);
        if (Zone->timer & 8) {
            int x = self->position.x + RSDK.Rand(-0x800000, 0x800000);
            int y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom + 64) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder =
                Zone->drawOrderHigh;
        }
    }

    if (++self->timer == 120) {
        CREATE_ENTITY(FXRuby, NULL, self->position.x, self->position.y + 0x100000)->radiusSpeed = 3;
        PhantomRuby_PlaySFX(RUBYSFX_REDCUBE);
    }

    if (self->timer == 320) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 16;
        fxFade->wait         = 32;
        fxFade->fadeOutBlack = 1;
        fxFade->speedOut     = 16;
        PhantomRuby_PlaySFX(RUBYSFX_ATTACK1);
    }

    if (self->timer >= 512) {
        if (globals->saveSlotID != NO_SAVE_SLOT) {
            if (self->timer == 512) {
                if (Zone_IsAct2())
                    GameProgress_MarkZoneCompleted(Zone_GetZoneID());
                SaveGame_SaveFile(PhantomEgg_SaveGameCB);
                UIWaitSpinner_Wait();
            }
            if (PhantomEgg->savedGameProgress)
                UIWaitSpinner_Wait2();
        }

        if (globals->saveSlotID == NO_SAVE_SLOT || PhantomEgg->savedGameProgress) {
            ++SceneInfo->listPos;
            RSDK.LoadScene();
            self->state = 0;
        }
    }
}

void PhantomEgg_SaveGameCB(int32 status) { PhantomEgg->savedGameProgress = true; }

#if RETRO_INCLUDE_EDITOR
void PhantomEgg_EditorDraw(void)
{
    RSDK_THIS(PhantomEgg);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 0, &self->animator2, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 17, &self->animator6, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &self->animator4, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &self->animator3, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 13, &self->animator7, false, 0);

    PhantomEgg_StateDraw_Normal();
}

void PhantomEgg_EditorLoad(void) { PhantomEgg->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomEgg.bin", SCOPE_STAGE); }
#endif

void PhantomEgg_Serialize(void) { RSDK_EDITABLE_VAR(PhantomEgg, VAR_ENUM, type); }
