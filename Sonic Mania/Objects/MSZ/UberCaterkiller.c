#include "SonicMania.h"

ObjectUberCaterkiller *UberCaterkiller;

void UberCaterkiller_Update(void)
{
    RSDK_THIS(UberCaterkiller);
    StateMachine_Run(entity->state);
}

void UberCaterkiller_LateUpdate(void) {}

void UberCaterkiller_StaticUpdate(void)
{
    foreach_all(UberCaterkiller, boss) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(boss)); }
}

void UberCaterkiller_Draw(void)
{
    RSDK_THIS(UberCaterkiller);
    int clipY2 = RSDK_screens->clipBound_Y2;
    if (entity->aniID) {
        if (entity->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 160, 0xE0E0E0);

        if (entity->bodyScales[0] < 0x200 == (RSDK_sceneInfo->currentDrawGroup == entity->drawOrder)) {
            int clip = ((entity->bodyScales[0] - 256) >> 1) + 160;
            if (RSDK_screens->height < clip)
                clip = RSDK_screens->height;
            RSDK_screens->clipBound_Y2 = clip;
            entity->scale.x            = entity->bodyScales[0];
            entity->scale.y            = entity->bodyScales[0];
            RSDK.DrawSprite(entity->bodyAnimators[0], entity->bodyPositions, false);
        }
        RSDK.SetPaletteEntry(0, 160, 0x200000);

        entity->direction ^= FLIP_X;
        for (int i = 1; i < UberCaterkiller_SegmentCount; ++i) {
            int clip = ((entity->bodyScales[i] - 0x100) >> 1) + 160;
            if (RSDK_screens->height < clip)
                clip = RSDK_screens->height;
            RSDK_screens->clipBound_Y2 = clip;

            entity->rotation = (2 * entity->bodyAngles[i] - 15) & 0x1E;
            if (entity->bodyScales[i] < 0x200 == (RSDK_sceneInfo->currentDrawGroup == entity->drawOrder)) {
                entity->scale.x           = entity->bodyScales[1];
                entity->scale.y           = entity->bodyScales[1];
                entity->animator2.frameID = entity->bodyAngles[i] >> 4;
                RSDK.DrawSprite(entity->bodyAnimators[i], &entity->bodyPositions[i], false);
            }
        }
        entity->direction ^= FLIP_X;
        entity->rotation = 0;
    }
    else {
        entity->direction ^= FLIP_X;

        for (int i = UberCaterkiller_SegmentCount - 1; i > 0; --i) {
            int clip = ((entity->bodyScales[i] - 256) >> 1) + 160;
            if (RSDK_screens->height < clip)
                clip = RSDK_screens->height;
            RSDK_screens->clipBound_Y2 = clip;
            entity->rotation           = (2 * entity->bodyAngles[i] - 15) & 0x1E;
            if (entity->bodyScales[i] < 0x200 == (RSDK_sceneInfo->currentDrawGroup == entity->drawOrder)) {
                entity->scale.x           = entity->bodyScales[i];
                entity->scale.y           = entity->bodyScales[i];
                entity->animator2.frameID = entity->bodyAngles[i] >> 4;
                RSDK.DrawSprite(entity->bodyAnimators[i], &entity->bodyPositions[i], false);
            }
        }

        entity->direction ^= FLIP_X;
        entity->rotation = 0;
        if (entity->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 160, 0xE0E0E0);

        if (entity->bodyScales[0] < 512 == (RSDK_sceneInfo->currentDrawGroup == entity->drawOrder)) {
            int clip = ((entity->bodyScales[0] - 256) >> 1) + 160;
            if (RSDK_screens->height < clip)
                clip = RSDK_screens->height;
            RSDK_screens->clipBound_Y2 = clip;
            entity->scale.x            = entity->bodyScales[0];
            entity->scale.y            = entity->bodyScales[0];
            RSDK.DrawSprite(entity->bodyAnimators[0], &entity->bodyPositions[0], false);
        }
        RSDK.SetPaletteEntry(0, 160, 0x200000);
    }
    RSDK_screens->clipBound_Y2 = clipY2;
}

void UberCaterkiller_Create(void *data)
{
    RSDK_THIS(UberCaterkiller);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = false;
        entity->drawFX        = FX_SCALE | FX_ROTATE | FX_FLIP;
        entity->drawOrder     = Zone->drawOrderLow - 1;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->timer         = 0;
        for (int i = 1; i < UberCaterkiller_SegmentCount; ++i) entity->bodyAnimators[i] = &entity->animator2;

        entity->bodyAnimators[0] = &entity->animator1;
        entity->health           = 6;
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 2, &entity->animator2, true, 0);
        entity->state = UberCaterkiller_State_SetupArena;
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
    UberCaterkiller->sfxRocketJet  = RSDK.GetSFX("Stage/RocketJet.wav");
    UberCaterkiller->sfxHit        = RSDK.GetSFX("Stage/BossHit.wav");
    UberCaterkiller->sfxExplosion2 = RSDK.GetSFX("Stage/Explosion2.wav");
    UberCaterkiller->sfxExplosion3 = RSDK.GetSFX("Stage/Explosion3.wav");
    UberCaterkiller->sfxSandSwim   = RSDK.GetSFX("MSZ/SandSwim.wav");
    UberCaterkiller->sfxBumper3    = RSDK.GetSFX("Stage/Bumper3.wav");
    UberCaterkiller->sfxCaterJump  = RSDK.GetSFX("MSZ/CaterJump.wav");
    DEBUGMODE_ADD_OBJ(UberCaterkiller);
}

void UberCaterkiller_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(UberCaterkiller, intToVoid(-0x20000), entity->position.x, entity->position.y);
}

void UberCaterkiller_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void UberCaterkiller_CheckPlayerCollisions(void)
{
    RSDK_THIS(UberCaterkiller);

    if (entity->invincibilityTimer > 0) {
        entity->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            uint8 *timer = &entity->field_1DC[RSDK.GetEntityID(player)];
            if (*timer) {
                --*timer;
            }
            else {
                entity->position.x = entity->bodyPositions[0].x;
                entity->position.y = entity->bodyPositions[0].y;
                int scale          = entity->bodyScales[0];

                Hitbox hitbox;
                hitbox.right  = (32 * scale) >> 9;
                hitbox.bottom = hitbox.right;
                hitbox.left   = -hitbox.right;
                hitbox.top    = -hitbox.right;

                if ((scale > 0x1C0 && scale < 0x240) && Player_CheckBadnikTouch(player, entity, &hitbox) && Player_CheckBossHit(player, entity)) {
                    if (--entity->health <= 0) {
                        RSDK_sceneInfo->timeEnabled = false;
                        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                        RSDK.PlaySfx(UberCaterkiller->sfxExplosion2, false, 255);
                        entity->timer = 120;
                        entity->state = UberCaterkiller_State_Destroyed;
                    }
                    else {
                        entity->invincibilityTimer = 48;
                        RSDK.PlaySfx(UberCaterkiller->sfxHit, false, 255);
                    }
                    foreach_break;
                }
                else {
                    for (int i = 1; i < UberCaterkiller_SegmentCount; ++i) {
                        entity->position.x = entity->bodyPositions[i].x;
                        entity->position.y = entity->bodyPositions[i].y;
                        scale              = entity->bodyScales[i];

                        if (scale > 0x1C0 && scale < 0x240) {
                            uint8 angle =
                                RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y) - entity->bodyAngles[i];
                            hitbox.right  = (28 * scale) >> 9;
                            hitbox.bottom = hitbox.right;
                            hitbox.left   = -hitbox.right;
                            hitbox.top    = -hitbox.right;
                            if (Player_CheckBadnikTouch(player, entity, &hitbox)) {
                                if (angle >= 0x80) {
#if RETRO_USE_PLUS
                                    if (!Player_CheckMightyUnspin(0x400, player, true, &player->uncurlTimer))
#endif
                                        Player_CheckHit(player, entity);
                                }
                                else if (Player_CheckBossHit(player, entity)) {
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
    entity->position.x = entity->bodyPositions[0].x;
    entity->position.y = entity->bodyPositions[0].y;
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
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x + entity->position.x, y + entity->position.y);
            explosion->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void UberCaterkiller_Unknown5(int id)
{
    RSDK_THIS(UberCaterkiller);

    if (entity->bodyVelocity[id].y >= 0) {
        if (entity->bodyUnknown[id]) {
            int pos = ((entity->bodyScales[id] - 256) >> 1) + 160;
            if (pos > 256)
                pos = 256;

            if ((entity->bodyPositions[id].y >> 16) - RSDK_screens->position.y > pos) {
                RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);
                EntityExplosion *explosion =
                    CREATE_ENTITY(Explosion, NULL, entity->bodyPositions[id].x,
                                  (((entity->bodyScales[id] << 15) - 0x7F8001) & 0xFFFF0000) + ((RSDK_screens->position.y + 160) << 16));
                RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
                explosion->drawFX       = FX_SCALE;
                explosion->scale.x      = entity->bodyScales[id];
                explosion->scale.y      = entity->bodyScales[id];
                explosion->drawOrder    = entity->drawOrder;
                explosion->velocity.x   = -0x10000;
                entity->bodyUnknown[id] = false;
            }
        }
    }
    else if (!entity->bodyUnknown[id]) {
        int pos = ((entity->bodyScales[id] - 256) >> 1) + 160;
        if (pos > 256)
            pos = 256;

        if ((entity->bodyPositions[id].y >> 16) - RSDK_screens->position.y < pos) {
            RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, NULL, entity->bodyPositions[id].x,
                              (((entity->bodyScales[id] << 15) - 0x7F8001) & 0xFFFF0000) + ((RSDK_screens->position.y + 160) << 16));
            RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
            explosion->drawFX       = FX_SCALE;
            explosion->scale.x      = entity->bodyScales[id];
            explosion->scale.y      = entity->bodyScales[id];
            explosion->drawOrder    = entity->drawOrder;
            explosion->velocity.x   = -0x10000;
            entity->bodyUnknown[id] = true;
        }
    }
}

void UberCaterkiller_SetupUnknown(int x, int y)
{
    RSDK_THIS(UberCaterkiller);

    int delay = 0;
    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
        entity->bodyVelocity[i].x  = x;
        entity->bodyVelocity[i].y  = y;
        entity->bodyTimers[i]      = delay;
        entity->bodyPositions[i].x = entity->position.x;
        entity->bodyPositions[i].y = entity->position.y;

        delay += 4;
    }

    entity->direction = x < 0;
}

void UberCaterkiller_State_SetupArena(void)
{
    RSDK_THIS(UberCaterkiller);

    if (++entity->timer == 60) {
        entity->timer   = 0;
        entity->visible = true;
        for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) entity->bodyPositions[i].x = (RSDK_screens->position.x + 64) << 16;

        entity->position.x    = entity->bodyPositions[0].x;
        entity->position.y    = entity->bodyPositions[0].y;
        entity->unknownValue2 = 0x100;
        UberCaterkiller_SetupBodySizes();
    }
}

void UberCaterkiller_SetupBodySizes(void)
{
    RSDK_THIS(UberCaterkiller);

    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) entity->bodyScales[i] = 0x280;
    entity->state = UberCaterkiller_State_Unknown1;
}

void UberCaterkiller_State_Unknown1(void)
{
    RSDK_THIS(UberCaterkiller);
    if (!(Zone->timer & 7))
        RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);

    if (!(Zone->timer & 3)) {
        EntityExplosion *explosion =
            CREATE_ENTITY(Explosion, NULL, entity->bodyPositions[0].x,
                          (((entity->bodyScales[0] << 15) - 0x7F8001) & 0xFFFF0000) + ((RSDK_screens->position.y + 160) << 16));
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
        explosion->drawFX     = FX_SCALE;
        explosion->scale.x    = entity->bodyScales[0];
        explosion->scale.y    = entity->bodyScales[0];
        explosion->drawOrder  = Zone->drawOrderLow - 1;
        explosion->velocity.x = -0x10000;
    }

    if (entity->bodyScales[0] >= entity->unknownValue2) {
        entity->bodyScales[0] -= 4;
        if (entity->bodyScales[0] < entity->unknownValue2)
            entity->bodyScales[0] = entity->unknownValue2;
    }
    else {
        entity->bodyScales[0] += 4;
        if (entity->bodyScales[0] > entity->unknownValue2)
            entity->bodyScales[0] = entity->unknownValue2;
    }

    if (entity->bodyScales[0] == entity->unknownValue2) {
        if (entity->bodyStartX) {
            entity->state = UberCaterkiller_State_Unknown2;
        }
        else {
            entity->timer = 60;
            entity->state = UberCaterkiller_State_Unknown5;
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
            CREATE_ENTITY(Explosion, NULL, entity->bodyPositions[0].x,
                          (((entity->bodyScales[0] << 15) - 0x7F8001) & 0xFFFF0000) + ((RSDK_screens->position.y + 160) << 16));
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
        explosion->drawFX     = FX_SCALE;
        explosion->scale.x    = entity->bodyScales[0];
        explosion->scale.y    = entity->bodyScales[0];
        explosion->drawOrder  = Zone->drawOrderLow - 1;
        explosion->velocity.x = -0x10000;
    }

    if (entity->bodyPositions[0].x >= entity->bodyStartX) {
        entity->bodyPositions[0].x -= 0x40000;
        if (entity->bodyPositions[0].x < entity->bodyStartX)
            entity->bodyPositions[0].x = entity->bodyStartX;
    }
    else {
        entity->bodyPositions[0].x += 0x20000;
        if (entity->bodyPositions[0].x > entity->bodyStartX)
            entity->bodyPositions[0].x = entity->bodyStartX;
    }

    if (entity->bodyPositions[0].x == entity->bodyStartX) {
        entity->timer = 60;
        entity->state = UberCaterkiller_State_Unknown3;
    }
}

void UberCaterkiller_State_Unknown3(void)
{
    RSDK_THIS(UberCaterkiller);
    if (!(Zone->timer & 7))
        RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);
    if (--entity->timer <= 0) {
        entity->timer = 0;

        int delay = 0;
        for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
            entity->bodyVelocity[i].x  = 0;
            entity->bodyVelocity[i].y  = -0x80000;
            entity->bodyTimers[i]      = delay;
            entity->bodyPositions[i].x = entity->bodyStartX;
            entity->bodyPositions[i].y = (RSDK_screens->position.y + 240) << 16;
            entity->bodyScales[i]      = 0x200;

            delay++;
        }

        entity->position.x = entity->bodyPositions[0].x;
        entity->position.y = entity->bodyPositions[0].y;
        entity->bodyStartX = 0;

        if (entity->bodyPositions[UberCaterkiller_SegmentCount - 1].x <= (RSDK_screens->position.x + RSDK_screens->centerX) << 16)
            UberCaterkiller_SetupUnknown(0x40000, -0x60000);
        else
            UberCaterkiller_SetupUnknown(-0x40000, -0x60000);
        RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
        entity->state = UberCaterkiller_State_Unknown4;
    }
}

void UberCaterkiller_State_Unknown4(void)
{
    RSDK_THIS(UberCaterkiller);
    RSDK.ProcessAnimation(&entity->animator1);

    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
        UberCaterkiller_Unknown5(i);

        if (entity->bodyTimers[i] > 0) {
            entity->bodyTimers[i]--;
            if (!entity->bodyTimers[i])
                RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);
        }
        else {
            entity->bodyVelocity[i].y += 0x1C00;
            entity->bodyPositions[i].y += entity->bodyVelocity[i].y;
            if (entity->bodyUnknown[i] == 1)
                entity->bodyPositions[i].x += entity->bodyVelocity[i].x;
        }
    }

    UberCaterkiller_CheckPlayerCollisions();

    if (entity->bodyPositions[UberCaterkiller_SegmentCount - 1].y > (RSDK_screens->position.y + RSDK_screens->height + 64) << 16) {
        entity->unknownValue2 = 0x100;
        UberCaterkiller_SetupBodySizes();

        switch (RSDK.Rand(0, 3) - 1) {
            default: break;
            case 0: entity->bodyStartX = (RSDK_screens->position.x - 32) << 16; break;
            case 1: entity->bodyStartX = (RSDK_screens->position.x + RSDK_screens->width + 32) << 16; break;
        }
    }
}

void UberCaterkiller_State_Unknown5(void)
{
    RSDK_THIS(UberCaterkiller);
    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) entity->bodyPositions[i].x -= 0x10000;

    if (--entity->timer <= 0) {
        entity->timer = 0;

        int delay = 0;
        for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
            entity->bodyVelocity[i].x  = 0;
            entity->bodyVelocity[i].y  = -0x80000;
            entity->bodyTimers[i]      = delay;
            entity->bodyPositions[i].y = (RSDK_screens->position.y + 160) << 16;
            entity->bodyScales[i]      = entity->unknownValue2;

            delay++;
        }

        entity->position.x   = entity->bodyPositions[0].x;
        entity->position.y   = entity->bodyPositions[0].y;
        entity->unknownValue = RSDK.Rand(0, 2);

        if (entity->bodyPositions[UberCaterkiller_SegmentCount - 1].x <= (RSDK_screens->position.x + RSDK_screens->centerX) << 16)
            UberCaterkiller_SetupUnknown(0x18000, -0x80000);
        else
            UberCaterkiller_SetupUnknown(-0x18000, -0x80000);

        RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
        entity->state = UberCaterkiller_State_Unknown6;
    }
}

void UberCaterkiller_State_Unknown6(void)
{
    RSDK_THIS(UberCaterkiller);
    RSDK.ProcessAnimation(&entity->animator1);

    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
        if (entity->bodyTimers[i] > 0) {
            entity->bodyTimers[i]--;
        }
        else {
            entity->bodyVelocity[i].y += 0x3800;

            entity->bodyPositions[i].x += entity->bodyVelocity[i].x;
            entity->bodyPositions[i].y += entity->bodyVelocity[i].y;
            if (entity->bodyScales[i] < 0x400)
                entity->bodyScales[i] += 8;
        }

        UberCaterkiller_Unknown5(i);
    }

    UberCaterkiller_CheckPlayerCollisions();

    if (entity->bodyPositions[UberCaterkiller_SegmentCount - 1].y > (RSDK_screens->position.y + RSDK_screens->height + 64) << 16) {
        RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);
        EntityExplosion *explosion =
            CREATE_ENTITY(Explosion, NULL, entity->bodyPositions[0].x,
                          (((entity->bodyScales[0] << 15) - 0x7F8001) & 0xFFFF0000) + ((RSDK_screens->position.y + 160) << 16));
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
        explosion->drawFX    = FX_SCALE;
        explosion->scale.x   = 0x80;
        explosion->scale.y   = 0x80;
        explosion->drawOrder = entity->drawOrder + 1;
        if (entity->unknownValue) {
            entity->position.y = (RSDK_screens->position.y + RSDK_screens->height + 64) << 16;
            if (entity->bodyPositions[UberCaterkiller_SegmentCount - 1].x <= (RSDK_screens->position.x + RSDK_screens->centerX) << 16)
                UberCaterkiller_SetupUnknown(0x18000, -0xB8000);
            else
                UberCaterkiller_SetupUnknown(-0x18000, -0xB8000);

            for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) entity->bodyScales[i] = 0x260;

            entity->state = UberCaterkiller_State_Unknown7;
            RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
            entity->aniID ^= 1;
        }
        else {
            UberCaterkiller_SetupBodySizes();
        }
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, entity->aniID, &entity->animator1, true, 0);
    }
}

void UberCaterkiller_State_Unknown7(void)
{
    RSDK_THIS(UberCaterkiller);
    RSDK.ProcessAnimation(&entity->animator1);

    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) {
        UberCaterkiller_Unknown5(i);

        if (entity->bodyTimers[i] > 0) {
            entity->bodyTimers[i]--;
        }
        else {
            entity->bodyVelocity[i].y += 0x3800;
            entity->bodyPositions[i].y += entity->bodyVelocity[i].y;
            if (entity->bodyUnknown[i] == 1) {
                entity->bodyPositions[i].x += entity->bodyVelocity[i].x;

                if (entity->aniID) {
                    if (entity->bodyScales[i] > 0x180)
                        entity->bodyScales[i] -= 3;
                }
                else {
                    if (entity->bodyScales[i] < 0x280)
                        entity->bodyScales[i] += 3;
                }
            }
        }
    }

    UberCaterkiller_CheckPlayerCollisions();
    if (entity->bodyPositions[UberCaterkiller_SegmentCount - 1].y > (RSDK_screens->height + RSDK_screens->position.y + 64) << 16) {
        if (!entity->aniID)
            --entity->unknownValue;
        if (entity->unknownValue) {
            entity->position.y = (RSDK_screens->height + RSDK_screens->position.y + 64) << 16;
            if (entity->bodyPositions[UberCaterkiller_SegmentCount - 1].x <= (RSDK_screens->position.x + RSDK_screens->centerX) << 16)
                UberCaterkiller_SetupUnknown(0x18000, -0xB8000);
            else
                UberCaterkiller_SetupUnknown(-0x18000, -0xB8000);
            entity->aniID ^= 1;
            RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
        }
        else {
            entity->unknownValue2 = 0x100;
            UberCaterkiller_SetupBodySizes();

            switch (RSDK.Rand(0, 3) - 1) {
                default: break;
                case 0: entity->bodyStartX = (RSDK_screens->position.x - 32) << 16; break;
                case 1: entity->bodyStartX = (RSDK_screens->position.x + RSDK_screens->width + 32) << 16; break;
            }
        }
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, entity->aniID, &entity->animator1, true, 0);
    }
}

void UberCaterkiller_State_Destroyed(void)
{
    RSDK_THIS(UberCaterkiller);
    UberCaterkiller_Explode();
    for (int i = 0; i < UberCaterkiller_SegmentCount; ++i) entity->bodyPositions[i].x -= 0x10000;

    entity->position.x -= 0x10000;
    if (--entity->timer <= 0) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 32;
        RSDK.PlaySfx(UberCaterkiller->sfxExplosion3, false, 255);

        for (int i = 1; i < UberCaterkiller_SegmentCount; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->bodyPositions[i].x, entity->bodyPositions[i].y);
            RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, entity->bodyAnimators[i]->animationID, &debris->animator, true,
                                    entity->bodyAnimators[i]->frameID);
            debris->velocity.x = 4 * RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = 4 * RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x4800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX |= FX_SCALE;
            debris->updateRange.x      = 0x400000;
            debris->updateRange.y      = 0x400000;
            debris->scale.x            = entity->bodyScales[i];
            debris->scale.y            = entity->bodyScales[i];
            debris->scaleInc.y         = RSDK.Rand(-4, 5);
            debris->scaleInc.x         = debris->scaleInc.y;
            entity->bodyPositions[i].x = -0x800000;
            entity->bodyPositions[i].y = -0x800000;
        }

        entity->velocity.y = -0x40000;
        entity->state      = UberCaterkiller_State_Finish;
    }
}

void UberCaterkiller_State_Finish(void)
{
    RSDK_THIS(UberCaterkiller);
    UberCaterkiller_Explode();
    entity->position.y += entity->velocity.y;
    entity->bodyPositions[0].y = entity->position.y;
    entity->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        UberCaterkiller->defeated = true;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void UberCaterkiller_EditorDraw(void)
{
    RSDK_THIS(UberCaterkiller);
    RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void UberCaterkiller_EditorLoad(void) { UberCaterkiller->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Sandworm.bin", SCOPE_STAGE); }
#endif

void UberCaterkiller_Serialize(void) {}
