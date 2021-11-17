#include "SonicMania.h"

ObjectPhantomEgg *PhantomEgg;

void PhantomEgg_Update(void)
{
    RSDK_THIS(PhantomEgg);
    StateMachine_Run(entity->state);
}

void PhantomEgg_LateUpdate(void) {}

void PhantomEgg_StaticUpdate(void) {}

void PhantomEgg_Draw(void)
{
    RSDK_THIS(PhantomEgg);

    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void PhantomEgg_Create(void *data)
{
    RSDK_THIS(PhantomEgg);

    entity->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        entity->active           = ACTIVE_BOUNDS;
        entity->updateRange.x    = 0x800000;
        entity->updateRange.y    = 0x1000000;
        entity->attackStateTable = PhantomEgg->attackStateTable1;
        entity->type             = voidToInt(data);
        if (!data) {
            entity->visible       = false;
            entity->drawOrder     = Zone->drawOrderLow;
            entity->hitbox.left   = -24;
            entity->hitbox.top    = -24;
            entity->hitbox.right  = 24;
            entity->hitbox.bottom = 24;
            entity->drawFX        = FX_FLIP;
            entity->active        = ACTIVE_NORMAL;
            entity->health        = 16;
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 0, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 17, &entity->animator6, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &entity->animator4, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &entity->animator3, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 13, &entity->animator7, true, 0);
            entity->field_80.x = entity->position.x;
            entity->field_80.y = entity->position.y;
            entity->state      = PhantomEgg_State_SetupArena;
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

    RSDK.ProcessAnimation(&entity->animator6);
    if (entity->animator7.animationID == 14) {
        RSDK.ProcessAnimation(&entity->animator7);
        if (entity->animator7.frameID == entity->animator7.frameCount - 1)
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 13, &entity->animator7, true, 0);
    }

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    if (entity->animator6.animationID == 17) {
        entity->animator6.animationSpeed = 0;
    }
    else {
        if (entity->animator6.animationID == 18 || entity->animator6.animationID == 19) {
            if (entity->animator6.frameID == entity->animator6.frameCount - 1)
                RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 17, &entity->animator6, true, 0);
        }
    }
}

void PhantomEgg_CheckPlayerCollisions(void)
{
    RSDK_THIS(PhantomEgg);

    if (RSDK.GetEntityCount(PhantomShield->objectID, true) <= 0) {
        foreach_active(Player, player)
        {
            if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
                PhantomEgg_Hit();
            }
        }
    }
}

void PhantomEgg_Hit(void)
{
    RSDK_THIS(PhantomEgg);

    --entity->health;
    if (!(entity->health & 3)) {
        int id = (-2 - RSDK.GetEntityCount(TMZCable->objectID, true)) & 3;

        foreach_active(TMZCable, cable)
        {
            if (cable->cableID == id || !entity->health) {
                RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 9, &cable->animator, true, 0);
                cable->state = TMZCable_Unknown6;
                // uncomment to fix a minor visual bug where the start few cable nodes wont be destroyed properly
                // cable->timer = 0;
            }
        }

        if (id == 1)
            entity->attackStateTable = PhantomEgg->attackStateTable2;
        else
            entity->attackStateTable = PhantomEgg->attackStateTable1;
    }

    if (entity->health <= 0) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 20, &entity->animator6, true, 0);
        entity->field_80.x          = entity->position.x;
        entity->field_80.y          = entity->position.y;
        entity->state               = PhantomEgg_State_Unknown16;
        entity->timer               = 0;
        PhantomEgg->superFlag       = true;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        entity->invincibilityTimer = 48;
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 19, &entity->animator6, true, 0);
        RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
    }
}

void PhantomEgg_Explode(Hitbox *hitbox)
{
    RSDK_THIS(PhantomEgg);

    if (!(Zone->timer % 7)) {
        RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);
        if (!(Zone->timer & 8)) {
            int x = entity->position.x + (RSDK.Rand(hitbox->left, hitbox->right) << 16);
            int y = entity->position.y + (RSDK.Rand(hitbox->top, hitbox->bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }
}

void PhantomEgg_HandleNextAttack(void)
{
    RSDK_THIS(PhantomEgg);

    switch (entity->attackStateTable[entity->attackTimer]) {
        case 1:
            if (entity->state != PhantomEgg_State_Unknown8) {
                entity->groundVel  = 0;
                entity->velocity.x = 0;
                entity->field_70   = 3;
                entity->state      = PhantomEgg_State_Unknown7;
                entity->attackTimer++;
                break;
            }
            break;
        case 2:
            entity->timer = 0;
            entity->state = PhantomEgg_State_Unknown10;
            entity->attackTimer++;
            break;
        case 3:
            if (entity->position.y >= PhantomEgg->boundsB - 0xA00000) {
                foreach_active(PhantomMissile, missile) { missile->timer = 16 * missile->id + 8; }
                ++entity->attackTimer;
            }
            break;
        case 4:
            entity->timer = 0;
            entity->state = PhantomEgg_State_Unknown11;
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &entity->animator3, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &entity->animator4, true, 0);
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &entity->animator5, true, 0);
            entity->attackTimer++;
            break;
        default: entity->attackTimer++; break;
    }
    entity->attackTimer = entity->attackTimer % 32;
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

    if (entity->invincibilityTimer & 1) {
        RSDK.CopyPalette(3, 32, 0, 32, 10);
        RSDK.CopyPalette(3, 128, 0, 128, 16);
        RSDK.SetPaletteEntry(0, 128, 0xF0F0F0);

        entity->direction         = FLIP_NONE;
        entity->animator2.frameID = 1;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator6, NULL, false);

        entity->animator2.frameID = 0;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator4, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
        RSDK.DrawSprite(&entity->animator7, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator4, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);

        RSDK.CopyPalette(1, 32, 0, 32, 10);
        RSDK.CopyPalette(1, 128, 0, 128, 16);
        RSDK.SetPaletteEntry(0, 128, 0x000000);
    }
    else {
        entity->direction         = FLIP_NONE;
        entity->animator2.frameID = 1;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator6, NULL, false);

        entity->animator2.frameID = 0;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator4, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
        RSDK.DrawSprite(&entity->animator7, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator4, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
}

void PhantomEgg_StateDraw_Cracked(void)
{
    RSDK_THIS(PhantomEgg);

    entity->direction         = FLIP_NONE;
    entity->animator2.frameID = 1;
    RSDK.DrawSprite(&entity->animator2, 0, false);
    RSDK.DrawSprite(&entity->animator6, 0, false);

    entity->animator2.frameID = 0;
    RSDK.DrawSprite(&entity->animator2, 0, false);
    RSDK.DrawSprite(&entity->animator1, 0, false);
    RSDK.DrawSprite(&entity->animator4, 0, false);
    RSDK.DrawSprite(&entity->animator3, 0, false);

    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator4, 0, false);
    RSDK.DrawSprite(&entity->animator3, 0, false);
}

void PhantomEgg_State_SetupArena(void)
{
    RSDK_THIS(PhantomEgg);

    if (++entity->timer >= 8) {
        entity->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - ScreenInfo->centerX;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + ScreenInfo->centerX;
        Zone->screenBoundsT1[0]     = Zone->screenBoundsB1[0] - ScreenInfo->height;
        PhantomEgg->boundsL         = (Zone->screenBoundsL1[0] + 64) << 16;
        PhantomEgg->boundsR         = (Zone->screenBoundsR1[0] - 64) << 16;
        PhantomEgg->boundsM         = entity->position.x;
        PhantomEgg->boundsT         = (Zone->screenBoundsT1[0] + 48) << 16;
        PhantomEgg->boundsB         = (Zone->screenBoundsB1[0] - 96) << 16;
        entity->position.y -= 0x1000000;
        entity->active = ACTIVE_NORMAL;
        entity->state  = PhantomEgg_State_Unknown1;
    }
}

void PhantomEgg_State_Unknown1(void)
{
    RSDK_THIS(PhantomEgg);

    if (entity->timer) {
        entity->timer += 4;
        RSDK.SetLimitedFade(0, 1, 2, entity->timer, 128, 256);
        if (entity->timer >= 384) {
            entity->timer     = 0;
            entity->visible   = true;
            entity->stateDraw = PhantomEgg_StateDraw_Normal;
            entity->state     = PhantomEgg_State_Unknown2;

            foreach_active(TMZCable, cable)
            {
                cable->posPtr = &entity->position;
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
                missile->parent   = (Entity *)entity;
                missile->id       = id++;
                angle += 64;
            }
            Music_TransitionTrack(TRACK_EGGMAN1, 0.0125);
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x) {
            SceneInfo->timeEnabled = false;
            ++entity->timer;
        }
    }
}

void PhantomEgg_State_Unknown2(void)
{
    RSDK_THIS(PhantomEgg);

    int startY = entity->position.y;
    if (entity->position.y >= entity->field_80.y)
        entity->velocity.y -= 0x3800;
    else
        entity->velocity.y += 0x3800;

    if (entity->velocity.y > 0x50000)
        entity->velocity.y = 0x50000;

    entity->position.y += entity->velocity.y;
    if (startY >= entity->field_80.y) {
        if (entity->position.y < entity->field_80.y) {
            ++entity->timer;
            entity->velocity.y = (4 * entity->velocity.y) >> 3;
        }
    }
    else if (entity->position.y > entity->field_80.y) {
        ++entity->timer;
        entity->velocity.y = (4 * entity->velocity.y) >> 3;
    }

    if (entity->timer > 4) {
        entity->timer = 0;
        entity->state = PhantomEgg_State_Unknown3;
    }
}

void PhantomEgg_State_Unknown3(void)
{
    RSDK_THIS(PhantomEgg);

    ++entity->timer;
    entity->position.y += (entity->field_80.y - entity->position.y) >> 3;
    if (entity->timer == 16) {
        entity->timer = 0;
        foreach_active(TMZAlert, alert) { alert->state = TMZAlert_Unknown1; }
        entity->state = PhantomEgg_State_Unknown4;
    }
}

void PhantomEgg_State_Unknown4(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    RSDK.SetLimitedFade(0, 1, 2, (RSDK.Cos256(entity->timer >> 2) >> 1) + 128, 128, 256);
    entity->timer += 11;

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->field_80.y;

    if (entity->timer == 3960) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 14, &entity->animator7, true, 0);
        SceneInfo->milliseconds = 0;
        SceneInfo->seconds      = 0;
        SceneInfo->minutes      = 0;
        PhantomRuby_PlaySFX(1);
    }

    if (entity->timer == 4092) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn      = 32;
        fxFade->speedOut     = 32;
    }

    if (entity->timer >= 4608) {
        entity->timer = 0;
        entity->state = PhantomEgg_State_Unknown5;
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void PhantomEgg_State_Unknown5(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->field_80.y;

    if (++entity->timer == 30) {
        entity->timer               = 0;
        SceneInfo->timeEnabled = true;
        if (Player_GetNearestPlayerX()->position.x < entity->position.x)
            entity->velocity.x = -0x60000;
        else
            entity->velocity.x = 0x60000;
        entity->state = PhantomEgg_State_Unknown6;
        CREATE_ENTITY(PhantomShield, entity, entity->position.x, entity->position.y);
    }
}

void PhantomEgg_State_Unknown6(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    int startX = entity->position.x;
    if (entity->position.x >= entity->field_80.x - 0x100000) {
        if (entity->position.x > entity->field_80.x + 0x100000)
            entity->velocity.x -= 0x1800;
    }
    else {
        entity->velocity.x += 0x1800;
    }

    entity->velocity.x = clampVal(entity->velocity.x, -0x50000, 0x50000);
    entity->position.x += entity->velocity.x;

    if (startX >= entity->field_80.x - 0x100000) {
        if (startX > entity->field_80.x + 0x100000 && entity->position.x < entity->field_80.x + 0x100000)
            entity->velocity.x = (7 * entity->velocity.x) >> 3;
    }
    else if (entity->position.x > entity->field_80.x - 0x100000) {
        entity->velocity.x = (7 * entity->velocity.x) >> 3;
    }

    if (entity->timer <= 0) {
        entity->timer    = RSDK.Rand(0x30, 0x60);
        entity->field_88 = entity->field_80.x + RSDK.Rand(-0x600000, 0x600000);
        entity->field_8C = RSDK.Rand(-0x200000, 0x200000) + entity->field_80.y;

        int angle        = RSDK.ATan2((entity->field_88 - entity->field_80.x) >> 16, (entity->field_8C - entity->field_80.y) >> 16);
        entity->field_88 = RSDK.Cos256(angle) << 9;
        entity->field_8C = RSDK.Sin256(angle) << 9;
        PhantomEgg_HandleNextAttack();
    }
    else {
        entity->timer--;
    }

    entity->velocity.y = entity->velocity.y + ((entity->field_8C - entity->velocity.y) >> 4);
    entity->position.y += entity->velocity.y;

    if (entity->position.y < PhantomEgg->boundsT && entity->field_8C < 0)
        entity->field_8C = -entity->field_8C;

    if (entity->position.y > PhantomEgg->boundsB) {
        if (entity->field_8C > 0)
            entity->field_8C = -entity->field_8C;
    }

    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Unknown7(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();
    entity->position.x += entity->velocity.x;
    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;

    Hitbox *hitbox = RSDK.GetHitbox(&entity->animator3, 0);
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, hitbox->bottom << 16, true) == true) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 4, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 4, &entity->animator5, true, 0);
        entity->state = PhantomEgg_State_Unknown8;
        RSDK.PlaySfx(PhantomEgg->sfxLand, false, 255);
    }

    if (entity->velocity.x < 0) {
        if (entity->position.x < PhantomEgg->boundsL)
            entity->velocity.x = -entity->velocity.x;
    }
    if (entity->velocity.x > 0 && entity->position.x > PhantomEgg->boundsR)
        entity->velocity.x = -entity->velocity.x;

    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Unknown8(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator5);
    RSDK.ProcessAnimation(&entity->animator3);
    PhantomEgg_HandleAnimations();

    Hitbox *hitbox = RSDK.GetHitbox(&entity->animator3, 0);
    RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, hitbox->bottom << 16, 16);
    if (entity->animator3.frameID == 5)
        PhantomEgg_HandleNextAttack();
    if (entity->animator3.frameID == 6) {
        entity->velocity.y = -0x80000;
        entity->state      = PhantomEgg_State_Unknown9;

        if (RSDK.Rand(0, 256) > 128)
            entity->velocity.x = -0x20000;
        else
            entity->velocity.x = 0x20000;
        RSDK.PlaySfx(PhantomEgg->sfxJump, false, 255);
    }
    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Unknown9(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator5);
    RSDK.ProcessAnimation(&entity->animator3);
    PhantomEgg_HandleAnimations();

    if (entity->velocity.x < 0) {
        if (entity->position.x < PhantomEgg->boundsL)
            entity->velocity.x = -entity->velocity.x;
    }
    if (entity->velocity.x > 0 && entity->position.x > PhantomEgg->boundsR)
        entity->velocity.x = -entity->velocity.x;

    entity->position.x += entity->velocity.x;
    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;
    if (entity->animator3.frameID == entity->animator3.frameCount - 1) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &entity->animator5, true, 0);
        --entity->field_70;
        if (entity->field_70 > 0)
            entity->state = PhantomEgg_State_Unknown7;
        else
            entity->state = PhantomEgg_State_Unknown6;
    }
    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Unknown10(void)
{
    RSDK_THIS(PhantomEgg);

    PhantomEgg_HandleAnimations();

    if (entity->timer < 160 && !(entity->timer & 0xF))
        RSDK.PlaySfx(PhantomEgg->sfxShock, false, 255);

    if (++entity->timer == 30) {
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

    if (entity->timer == 4 * entity->health + 160)
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 14, &entity->animator7, true, 0);

    if (entity->animator7.animationID == 14 && entity->animator7.frameID == entity->animator7.frameCount - 2 || entity->timer > 288) {
        entity->timer = 0;
        CREATE_ENTITY(PhantomShield, entity, entity->position.x, entity->position.y);
        if (entity->animator3.frameID > 0) {
            entity->animator3.frameID        = 6;
            entity->animator3.animationTimer = 0;
            entity->state                    = PhantomEgg_State_Unknown8;
        }
        else {
            entity->state = PhantomEgg_State_Unknown6;
        }
    }
    PhantomEgg_CheckPlayerCollisions();
}

void PhantomEgg_State_Unknown11(void)
{
    RSDK_THIS(PhantomEgg);

    if (!entity->timer) {
        foreach_active(PhantomShield, shield)
        {
            RSDK.SetSpriteAnimation(PhantomShield->aniFrames, 2, &shield->animator, true, 0);
            shield->state = PhantomShield_Unknown3;
        }
    }

    if (entity->palBlendPercent < 112)
        entity->palBlendPercent += 4;

    RSDK.SetLimitedFade(0, 1, 2, entity->palBlendPercent, 128, 256);
    entity->position.x += (PhantomEgg->boundsM - entity->position.x) >> 4;
    entity->position.y += ((PhantomEgg->boundsB - entity->position.y) - 0x400000) >> 4;

    if (++entity->timer == 60) {
        entity->timer      = 0;
        entity->position.x = PhantomEgg->boundsM;
        entity->position.y = PhantomEgg->boundsB - 0x400000;
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 5, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 6, &entity->animator5, true, 0);
        entity->state = PhantomEgg_State_Unknown12;
    }
}

void PhantomEgg_State_Unknown12(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator5);
    RSDK.SetLimitedFade(0, 1, 2, entity->palBlendPercent, 128, 256);
    if (!entity->timer) {
        CREATE_ENTITY(PhantomHand, entity, entity->position.x - 0x400000, entity->position.y)->velocity.x = -0x8000;
        EntityPhantomHand *hand = CREATE_ENTITY(PhantomHand, entity, entity->position.x + 0x400000, entity->position.y);
        hand->direction         = FLIP_X;
        hand->velocity.x        = 0x8000;
    }
    ++entity->timer;
}

void PhantomEgg_State_Unknown13(void)
{
    RSDK_THIS(PhantomEgg);

    if (PhantomEgg->endScanline == 96) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn      = 16;
        fxFade->speedOut     = 16;
    }

    if (entity->palBlendPercent > 0)
        entity->palBlendPercent -= 4;

    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator5);
    RSDK.SetLimitedFade(0, 1, 2, entity->palBlendPercent, 128, 256);

    if (PhantomEgg->endScanline <= 0) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &entity->animator5, true, 0);
        if (entity->visible) {
            entity->state = PhantomEgg_State_Unknown14;
            PhantomEgg_HandlePhantomWarp(entity->phantomID);
            entity->phantomID = (entity->phantomID + 1) & 3;
        }
        else {
            entity->state = PhantomEgg_State_Unknown15;
            PhantomEgg_HandleReturnWarp();
            entity->visible = true;
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

        entity->timer   = 0;
        entity->visible = 0;
        entity->state   = 0;
    }
    else {
        if (entity->timer > 8)
            PhantomEgg->endScanline += 2;

        if (entity->timer == 60) {
            foreach_active(PhantomHand, hand) { hand->state = PhantomHand_State_Unknown9; }
        }
        ++entity->timer;
    }
}

void PhantomEgg_State_Unknown15(void)
{
    RSDK_THIS(PhantomEgg);

    if (PhantomEgg->endScanline >= ScreenInfo->height) {
        RSDK.GetSceneLayer(Zone->fgLow)->scanlineCallback  = NULL;
        RSDK.GetSceneLayer(Zone->fgHigh)->scanlineCallback = NULL;

        CREATE_ENTITY(PhantomShield, entity, entity->position.x, entity->position.y);
        entity->timer = 0;
        entity->state = PhantomEgg_State_Unknown6;
    }
    else {
        if (entity->timer > 8)
            PhantomEgg->endScanline += 2;

        if (entity->timer == 60) {
            foreach_active(PhantomHand, hand) { hand->state = PhantomHand_State_Unknown9; }
        }
        ++entity->timer;
    }
}

void PhantomEgg_State_Unknown16(void)
{
    RSDK_THIS(PhantomEgg);

    entity->position.x = entity->field_80.x + RSDK.Rand(-0x20000, 0x20000);
    entity->position.y = entity->field_80.y + RSDK.Rand(-0x20000, 0x20000);

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

        CREATE_ENTITY(TMZ2Outro, NULL, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &entity->animator3, true, 1);

        for (int i = 0; i < 0x100; ++i) {
            RSDK.SetPaletteEntry(7, i, RSDK.GetPaletteEntry(1, i) & 0xFF0000);
        }

        entity->state = PhantomEgg_State_Unknown17;
    }
}

void PhantomEgg_State_Unknown17(void)
{
    RSDK_THIS(PhantomEgg);

    entity->field_80.x = entity->field_80.x + ((PhantomEgg->boundsM - entity->field_80.x) >> 5);
    entity->field_80.y = entity->field_80.y + ((PhantomEgg->boundsB - entity->field_80.y - 0x400000) >> 5);
    entity->position.x = entity->field_80.x + RSDK.Rand(-0x20000, 0x20000);
    entity->position.y = entity->field_80.y + RSDK.Rand(-0x20000, 0x20000);
    PhantomEgg_Explode(&entity->hitbox);

    if (++entity->timer == 152) {
        entity->timer      = 0;
        entity->position.x = PhantomEgg->boundsM;
        entity->position.y = PhantomEgg->boundsB - 0x400000;

        bool32 goodEnd = (checkPlayerID(ID_SONIC, 1) || (checkPlayerID(ID_KNUCKLES, 1) && checkPlayerID(ID_KNUCKLES, 2)))
                         && SaveGame->saveRAM->chaosEmeralds == 0x7F;

#if RETRO_USE_PLUS
        if (SceneInfo->filter & FILTER_ENCORE)
            goodEnd = false; // no ERZ for encore modes
#endif
        if (goodEnd) {
            entity->state = PhantomEgg_State_Unknown21;
            Music_FadeOut(0.0125);
        }
        else {
            entity->state = PhantomEgg_State_Unknown18;
        }
    }
}

void PhantomEgg_State_Unknown18(void)
{
    RSDK_THIS(PhantomEgg);

    if (++entity->timer == 30) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 1, &entity->animator1, false, 0);
        entity->stateDraw = PhantomEgg_StateDraw_Cracked;
        RSDK.PlaySfx(PhantomEgg->sfxRocketJet, false, 255);

        EntityPhantomRuby *ruby = CREATE_ENTITY(PhantomRuby, NULL, entity->position.x, entity->position.y + 0x100000);
        ruby->state             = PhantomRuby_Unknown8;
        ruby->velocity.x        = -0x10000;
        ruby->velocity.y        = -0x20000;
        entity->state           = PhantomEgg_State_Unknown19;
    }
}

void PhantomEgg_State_Unknown19(void)
{
    RSDK_THIS(PhantomEgg);

    RSDK.ProcessAnimation(&entity->animator1);
    if (++entity->timer == 60) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn      = 512;
        fxFade->wait         = 16;
        fxFade->speedOut     = 16;
        RSDK.PlaySfx(PhantomEgg->sfxExplosion3, false, 255);
    }
    if (entity->timer == 64) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 2, &entity->animator1, false, 0);
        entity->drawOrder = Zone->drawOrderLow + 1;
        entity->stateDraw = StateMachine_None;

        EntityEggman *eggman = CREATE_ENTITY(Eggman, NULL, entity->position.x, entity->position.y + 0x100000);
        RSDK.SetSpriteAnimation(Eggman->aniFrames, 7, &eggman->animator, true, 0);
        eggman->onGround = false;
        eggman->state    = Eggman_Unknown6;
    }

    foreach_active(Eggman, eggman)
    {
        eggman->position.x = entity->position.x;
        eggman->position.y = entity->position.y;
        eggman->velocity.y = 0;
    }

    if (entity->timer == 96) {
        entity->timer = 0;
        entity->state = PhantomEgg_State_Unknown20;
    }
}

void PhantomEgg_State_Unknown20(void)
{
    RSDK_THIS(PhantomEgg);

    if (!(Zone->timer & 0xF)) {
        int x                      = entity->position.x + RSDK.Rand(-0x380000, -0x180000);
        int y                      = entity->position.y + RSDK.Rand(-0x300000, -0x100000);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), x, y);
        explosion->drawOrder       = Zone->drawOrderHigh;
    }

    if ((Zone->timer & 0xF) == 8) {
        int x                      = entity->position.x + RSDK.Rand(0x180000, 0x380000);
        int y                      = entity->position.y + RSDK.Rand(-0x300000, -0x100000);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), x, y);
        explosion->drawOrder       = Zone->drawOrderHigh;
    }
    if (++entity->timer == 120)
        entity->state = 0;
}

void PhantomEgg_State_Unknown21(void)
{
    RSDK_THIS(PhantomEgg);

    if (entity->timer < 256 && !(Zone->timer % 3)) {
        RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);
        if (Zone->timer & 8) {
            int x = entity->position.x + RSDK.Rand(-0x800000, 0x800000);
            int y = entity->position.y + (RSDK.Rand(entity->hitbox.top, entity->hitbox.bottom + 64) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder =
                Zone->drawOrderHigh;
        }
    }

    if (++entity->timer == 120) {
        CREATE_ENTITY(FXRuby, NULL, entity->position.x, entity->position.y + 0x100000)->radiusSpeed = 3;
        PhantomRuby_PlaySFX(RUBYSFX_REDCUBE);
    }

    if (entity->timer == 320) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn      = 16;
        fxFade->wait         = 32;
        fxFade->fadeOutBlack = 1;
        fxFade->speedOut     = 16;
        PhantomRuby_PlaySFX(RUBYSFX_ATTACK1);
    }

    if (entity->timer >= 512) {
        if (globals->saveSlotID != NO_SAVE_SLOT) {
            if (entity->timer == 512) {
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
            entity->state = 0;
        }
    }
}

void PhantomEgg_SaveGameCB(int32 status) { PhantomEgg->savedGameProgress = true; }

#if RETRO_INCLUDE_EDITOR
void PhantomEgg_EditorDraw(void)
{
    RSDK_THIS(PhantomEgg);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 0, &entity->animator2, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 17, &entity->animator6, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 3, &entity->animator4, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 8, &entity->animator3, false, 0);
    RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 13, &entity->animator7, false, 0);

    PhantomEgg_StateDraw_Normal();
}

void PhantomEgg_EditorLoad(void) { PhantomEgg->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomEgg.bin", SCOPE_STAGE); }
#endif

void PhantomEgg_Serialize(void) { RSDK_EDITABLE_VAR(PhantomEgg, VAR_ENUM, type); }
