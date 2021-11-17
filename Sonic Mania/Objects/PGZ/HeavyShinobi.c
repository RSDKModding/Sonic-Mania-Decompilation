#include "SonicMania.h"

ObjectHeavyShinobi *HeavyShinobi;

void HeavyShinobi_Update(void)
{
    RSDK_THIS(HeavyShinobi);
    StateMachine_Run(entity->state);
}

void HeavyShinobi_LateUpdate(void) {}

void HeavyShinobi_StaticUpdate(void) {}

void HeavyShinobi_Draw(void)
{
    RSDK_THIS(HeavyShinobi);
    StateMachine_Run(entity->stateDraw);
}

void HeavyShinobi_Create(void *data)
{
    RSDK_THIS(HeavyShinobi);

    entity->drawFX = FX_ROTATE | FX_FLIP;
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->type = voidToInt(data);
            switch (entity->type) {
                case SHINOBI_MAIN:
                    entity->active    = ACTIVE_BOUNDS;
                    entity->visible   = false;
                    entity->drawOrder = Zone->drawOrderLow;
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 0, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &entity->animator2, true, 0);
                    entity->state         = HeavyShinobi_State_Setup;
                    entity->stateDraw     = HeavyShinobi_StateDraw_Unknown1;
                    entity->updateRange.y = 0x800000;
                    entity->updateRange.x = 0x800000;
                    break;
                case SHINOBI_SLASH:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow;
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 8, &entity->animator1, true, 0);
                    entity->inkEffect     = INK_ALPHA;
                    entity->alpha         = 0x100;
                    entity->state         = HeavyShinobi_State1_Unknown1;
                    entity->stateDraw     = HeavyShinobi_StateDraw1_Unknown1;
                    entity->updateRange.y = 0x800000;
                    entity->updateRange.x = 0x800000;
                    break;
                case SHINOBI_SHURIKEN:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow - 1;
                    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 9, &entity->animator1, true, 0);
                    entity->updateRange.x = 0x1000000;
                    entity->updateRange.y = 0x1000000;
                    entity->alpha         = 0xC0;
                    entity->state         = HeavyShinobi_State2_Unknown1;
                    entity->stateDraw     = HeavyShinobi_StateDraw2_Unknown1;
                    break;
                case SHINOBI_3:
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderLow;
                    entity->drawFX        = FX_ROTATE | FX_FLIP;
                    entity->state         = HeavyShinobi_State3_Unknown1;
                    entity->stateDraw     = HeavyShinobi_StateDraw3_Unknown1;
                    entity->updateRange.y = 0x800000;
                    entity->updateRange.x = 0x800000;
                    break;
                case SHINOBI_BOUNDS:
                    entity->active      = ACTIVE_NORMAL;
                    entity->isPermanent = true;
                    entity->visible     = true;
                    entity->drawOrder   = Zone->fgLayerHigh - 1;
                    RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 0, &entity->animator1, true, 0);
                    entity->state         = HeavyShinobi_State4_Unknown1;
                    entity->stateDraw     = HeavyShinobi_StateDraw4_Unknown1;
                    entity->updateRange.y = 0x800000;
                    entity->updateRange.x = 0x800000;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(entity);
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
    HeavyShinobi->hitbox6.bottom               = 336;

    HeavyShinobi->hitbox7.left   = -3;
    HeavyShinobi->hitbox7.top    = -3;
    HeavyShinobi->hitbox7.right  = 3;
    HeavyShinobi->hitbox7.bottom = 3;

    RSDK.SetSpriteAnimation(0xFFFF, 0, HeavyShinobi->animator1, true, 0);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[1], true, 0);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[2], true, 0);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &HeavyShinobi->animator1[3], true, 0);
    HeavyShinobi->activeShurikens    = 0;
    HeavyShinobi->health             = 8;
    HeavyShinobi->invincibilityTimer = 0;
    HeavyShinobi->sfxHit             = RSDK.GetSFX("Stage/BossHit.wav");
    HeavyShinobi->sfxExplosion       = RSDK.GetSFX("Stage/Explosion2.wav");
    HeavyShinobi->sfxDefeat          = RSDK.GetSFX("PSZ/ShinobiDefeat.wav");
    HeavyShinobi->sfxDropIn          = RSDK.GetSFX("PSZ/ShinobiDropIn.wav");
    HeavyShinobi->sfxExplode         = RSDK.GetSFX("PSZ/ShinobiExplode.wav");
    HeavyShinobi->sfxGlitch          = RSDK.GetSFX("PSZ/ShinobiGlitch.wav");
    HeavyShinobi->sfxJump            = RSDK.GetSFX("PSZ/ShinobiJump.wav");
    HeavyShinobi->sfxParry           = RSDK.GetSFX("PSZ/ShinobiParry.wav");
    HeavyShinobi->sfxSlash           = RSDK.GetSFX("PSZ/ShinobiSlash.wav");
    HeavyShinobi->sfxStick           = RSDK.GetSFX("PSZ/ShinobiStick.wav");
    HeavyShinobi->sfxThrow           = RSDK.GetSFX("PSZ/ShinobiThrow.wav");
}

void HeavyShinobi_HandleAfterFX(void)
{
    RSDK_THIS(HeavyShinobi);

    for (int32 i = 15; i > 0; --i) {
        HeavyShinobi->storePos[i] = HeavyShinobi->storePos[i - 1];
    }
    HeavyShinobi->storePos[0].x = entity->position.x;
    HeavyShinobi->storePos[0].y = entity->position.y;

    for (int32 i = 0; i < 4; ++i) {
        int32 storeAnim = entity->animator1.animationID;
        if (HeavyShinobi->animator1[i].animationID != storeAnim) {
            if (HeavyShinobi->storedAnimIDs[i] != storeAnim) {
                HeavyShinobi->storedAnimIDs[i] = entity->animator1.animationID;
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
    entity->direction = player->position.x >= entity->position.x;
    RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 4, &entity->animator1, true, 1);
    CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_SLASH), entity->position.x, entity->position.y)->direction = entity->direction;

    int32 delay = 4;
    int32 alpha = 0xC0;
    for (int32 i = 3; i >= 0; --i) {
        EntityHeavyShinobi *child = CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_SLASH), entity->position.x, entity->position.y);
        child->animator1.frameDelay += delay;
        child->alpha     = alpha;
        child->direction = entity->direction;

        delay += 4;
        alpha -= 32;
    }

    entity->state = HeavyShinobi_State_Unknown2;
}

void HeavyShinobi_Unknown3(void)
{
    RSDK_THIS(HeavyShinobi);

    RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
    RSDK.PlaySfx(HeavyShinobi->sfxJump, false, 255);
    entity->timer = 28;

    if (entity->position.x >= (Zone->screenBoundsL1[0] + 144) << 16) {
        if (entity->position.x <= (Zone->screenBoundsR1[0] - 144) << 16)
            entity->velocity.x = (RSDK.Rand(0, 2) << 18) - 0x20000;
        else
            entity->velocity.x = -0x20000;
    }
    else {
        entity->velocity.x = 0x20000;
    }

    if (entity->velocity.x >= 0)
        entity->velocity.x += 0x1000;
    else
        entity->velocity.x -= 0x1000;
    entity->velocity.y = -0x70000;
    RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 3, &entity->animator1, true, 0);
    entity->state = HeavyShinobi_State_Unknown3;
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
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), x + entity->position.x, y + entity->position.y);
            explosion->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void HeavyShinobi_State_Setup(void)
{
    RSDK_THIS(HeavyShinobi);

    if (++entity->timer >= 2) {
        entity->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsR1[0]     = ScreenInfo->centerX + 80 + (entity->position.x >> 16);
        Zone->playerBoundActiveB[0] = true;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 68;
        entity->position.y          = (ScreenInfo->position.y - 192) << 16;
        for (int32 i = 0; i < 16; ++i) HeavyShinobi->storePos[i] = entity->position;

        entity->active = ACTIVE_NORMAL;
        entity->state  = HeavyShinobi_State_SetupArena;
    }
}

void HeavyShinobi_State_SetupArena(void)
{
    RSDK_THIS(HeavyShinobi);

    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = ScreenInfo->position.x;
    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x - 0x500000) {
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - ScreenInfo->centerX - 80;
        CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_BOUNDS), (Zone->screenBoundsL1[0] + 40) << 16, (Zone->screenBoundsB1[0] - 376) << 16);
        EntityHeavyShinobi *child =
            CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_BOUNDS), (Zone->screenBoundsR1[0] - 40) << 16, (Zone->screenBoundsB1[0] - 376) << 16);
        child->timer      = 1;
        child->position.y = (Zone->screenBoundsB1[0] - 99) << 16;
        child->state      = HeavyShinobi_State4_Unknown2;
        entity->state     = HeavyShinobi_State_StartFight;
    }
}

void HeavyShinobi_State_StartFight(void)
{
    RSDK_THIS(HeavyShinobi);

    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = ScreenInfo->position.x;
    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - ScreenInfo->centerX - 80;
        Music_TransitionTrack(TRACK_HBHBOSS, 0.0125);
        entity->visible = true;
        HeavyShinobi_Unknown3();
        entity->velocity.x = 0;
        entity->timer      = 0;
    }
}

void HeavyShinobi_State_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);

    HeavyShinobi_HandleAfterFX();
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    for (int32 i = 0; i < 4; ++i) RSDK.ProcessAnimation(&HeavyShinobi->animator1[i]);
    entity->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= entity->position.x;

    EntityPlayer *player = Player_GetNearestPlayerX();
    if (abs(player->position.x - entity->position.x) < 0x500000 && player->state != Ice_State_FrozenPlayer) {
        HeavyShinobi_HandleSlash(player);
    }
    else {
        if (!HeavyShinobi->activeShurikens && --entity->timer <= 0)
            HeavyShinobi_Unknown3();
    }
}

void HeavyShinobi_State_Unknown2(void)
{
    RSDK_THIS(HeavyShinobi);

    HeavyShinobi_HandleAfterFX();
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    for (int32 i = 0; i < 4; ++i) RSDK.ProcessAnimation(&HeavyShinobi->animator1[i]);
    if (entity->animator1.frameID == 12)
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &entity->animator2, false, 0);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true))
        entity->velocity.y = 0;

    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        EntityPlayer *player = Player_GetNearestPlayerX();
        if (abs(player->position.x - entity->position.x) >= 0x500000 || player->state == Ice_State_FrozenPlayer)
            HeavyShinobi_Unknown3();
        else
            HeavyShinobi_HandleSlash(player);
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &HeavyShinobi->hitbox2) && Player_CheckValidState(player)) {
            Ice_FreezePlayer(player);
            player->timer      = 3;
            player->onGround   = false;
            player->velocity.y = -0x38000;
            if (player->position.x < entity->position.x)
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
    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;

    if (entity->velocity.x >= 0) {
        if (entity->position.x > (Zone->screenBoundsR1[0] - 104) << 16) {
            entity->velocity.x = 0;
            entity->position.x = (Zone->screenBoundsR1[0] - 104) << 16;
        }
    }
    else {
        if (entity->position.x < (Zone->screenBoundsL1[0] + 104) << 16) {
            entity->velocity.x = 0;
            entity->position.x = (Zone->screenBoundsL1[0] + 104) << 16;
        }
    }

    if (!--entity->timer) {
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
                EntityHeavyShinobi *child = CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_SHURIKEN), entity->position.x, entity->position.y);
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

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true)) {
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->direction  = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= entity->position.x;
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &entity->animator2, false, 0);
        entity->timer = RSDK.Rand(30, 91);
        entity->state = HeavyShinobi_State_Unknown1;
    }
    else {
        foreach_active(Player, player)
        {
            if (player->state != Ice_State_FrozenPlayer && Player_CheckBadnikTouch(player, entity, &HeavyShinobi->hitbox1)
                && Player_CheckBossHit(player, entity)) {
                if (player->position.x >= entity->position.x) {
                    entity->direction  = FLIP_X;
                    entity->velocity.x = -0x20000;
                }
                else {
                    entity->direction  = FLIP_NONE;
                    entity->velocity.x = 0x20000;
                }
                entity->velocity.y  = -0x40000;
                int32 angle           = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                player->velocity.x  = 0x380 * RSDK.Cos256(angle);
                player->velocity.y  = 0x380 * RSDK.Sin256(angle);
                entity->timer       = 180;
                entity->glitchTimer = 0;
                RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 15, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &entity->animator2, false, 0);
                RSDK.PlaySfx(HeavyShinobi->sfxParry, false, 255);
                entity->state = HeavyShinobi_State_Glitched;
            }
        }
    }
}

void HeavyShinobi_State_Glitched(void)
{
    RSDK_THIS(HeavyShinobi);
    if (!(entity->glitchTimer % 40))
        RSDK.PlaySfx(HeavyShinobi->sfxGlitch, false, 255);
    ++entity->glitchTimer;
    HeavyShinobi_HandleAfterFX();
    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->velocity.x >= 0) {
        if (entity->position.x > (Zone->screenBoundsR1[0] - 104) << 16) {
            entity->velocity.x = 0;
            entity->position.x = (Zone->screenBoundsR1[0] - 104) << 16;
        }
    }
    else {
        if (entity->position.x < (Zone->screenBoundsL1[0] + 104) << 16) {
            entity->velocity.x = 0;
            entity->position.x = (Zone->screenBoundsL1[0] + 104) << 16;
        }
    }

    if (entity->velocity.y > 0x2800 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true)) {
        RSDK.GetEntityByID(SLOT_PLAYER1);
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 16, &entity->animator1, false, 0);
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &entity->animator2, false, 0);
        if (--entity->timer <= 0) {
            RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &entity->animator2, false, 0);
            entity->timer = RSDK.Rand(30, 91);
            entity->state = HeavyShinobi_State_Unknown1;
            RSDK.StopSFX(HeavyShinobi->sfxGlitch);
        }
    }

    if (!HeavyShinobi->invincibilityTimer) {
        foreach_active(Player, player)
        {
            if (player->state != Ice_State_FrozenPlayer && Player_CheckBadnikTouch(player, entity, &HeavyShinobi->hitbox1)
                && Player_CheckBossHit(player, entity)) {
                if (HeavyShinobi->health)
                    HeavyShinobi->health--;

                if (HeavyShinobi->health) {
                    RSDK.PlaySfx(HeavyShinobi->sfxHit, false, 255);
                    entity->timer -= 60;
                    HeavyShinobi->invincibilityTimer = 30;
                }
                else {
                    SceneInfo->timeEnabled = false;
                    Player_GiveScore(RSDK.GetEntityByID(SLOT_PLAYER1), 1000);
                    RSDK.PlaySfx(HeavyShinobi->sfxExplosion, false, 255);
                    entity->timer = 120;
                    entity->state = HeavyShinobi_State_Destroyed;
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
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->velocity.x >= 0) {
        if (entity->position.x > (Zone->screenBoundsR1[0] - 104) << 16) {
            entity->velocity.x = 0;
            entity->position.x = (Zone->screenBoundsR1[0] - 104) << 16;
        }
    }
    else {
        if (entity->position.x < (Zone->screenBoundsL1[0] + 104) << 16) {
            entity->velocity.x = 0;
            entity->position.x = (Zone->screenBoundsL1[0] + 104) << 16;
        }
    }

    if (entity->velocity.y > 0x2800 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x2A0000, true)) {
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->direction  = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= entity->position.x;
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 16, &entity->animator1, false, 0);
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 5, &entity->animator2, false, 0);
    }

    HeavyShinobi_Explode();
    if (--entity->timer <= 0) {
        entity->velocity.y = -0x70000;
        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 3, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        RSDK.PlaySfx(HeavyShinobi->sfxDefeat, false, 255);
        entity->state = HeavyShinobi_State_Finished;
    }
}

void HeavyShinobi_State_Finished(void)
{
    RSDK_THIS(HeavyShinobi);
    HeavyShinobi_HandleAfterFX();
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    for (int32 i = 0; i < 4; ++i) RSDK.ProcessAnimation(&HeavyShinobi->animator1[i]);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;

    if (entity->velocity.y > 0)
        entity->drawOrder = Zone->drawOrderHigh;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        Zone->screenBoundsR1[0] += 424;
        HeavyShinobi->health = -1;
        destroyEntity(entity);
    }
}

void HeavyShinobi_StateDraw_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    entity->inkEffect = INK_ALPHA;
    entity->alpha     = 0x60;

    for (int32 i = 3; i >= 0; --i) {
        RSDK.DrawSprite(&HeavyShinobi->animator1[i], &HeavyShinobi->storePos[i << 2], false);
        entity->alpha += 0x20;
    }

    entity->inkEffect = INK_NONE;
    if (HeavyShinobi->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    RSDK.DrawSprite(&entity->animator1, 0, false);
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void HeavyShinobi_State1_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID != entity->animator1.frameCount - 1) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &HeavyShinobi->hitbox4) && Player_CheckValidState(player)) {
                Ice_FreezePlayer(player);
                player->timer      = 3;
                player->onGround   = false;
                player->velocity.y = -0x38000;
                if (player->position.x < entity->position.x)
                    player->velocity.x = -0x30000;
                else
                    player->velocity.x = 0x30000;
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void HeavyShinobi_StateDraw1_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void HeavyShinobi_State2_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);

    if (HeavyShinobi->health) {
        RSDK.ProcessAnimation(&entity->animator1);
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;

        bool32 flag = false;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &HeavyShinobi->hitbox5)) {
                if (player->state == Ice_State_FrozenPlayer) {
                    entity->playerPtr        = (Entity *)player;
                    entity->playerDistance.x = entity->position.x - player->position.x;
                    entity->playerDistance.y = entity->position.y - player->position.y;

                    flag = true;
                    foreach_break;
                }

#if RETRO_USE_PLUS
                int32 anim = player->playerAnimator.animationID;
                if (player->characterID == ID_MIGHTY && (anim == ANI_CROUCH || anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH)) {
                    if (!player->uncurlTimer) {
                        RSDK.PlaySfx(Player->sfxPimPom, false, 255);
                        player->uncurlTimer = 30;
                    }
                    int32 angle           = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                    entity->velocity.x  = -0x400 * RSDK.Cos256(angle);
                    entity->velocity.y  = -0x600 * RSDK.Sin256(angle);
                    entity->interaction = false;
                    entity->state       = HeavyShinobi_State2_Unknown2;
                    --HeavyShinobi->activeShurikens;
                }
                else {
#endif
                    Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
                }
#endif
            }
        }

        if (!flag && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true))
            flag = true;

        if (!flag) {
            foreach_active(HeavyShinobi, boss)
            {
                if (boss->type == SHINOBI_BOUNDS && RSDK.CheckObjectCollisionBox(boss, &HeavyShinobi->hitbox6, entity, &HeavyShinobi->hitbox5, true)) {
                    flag = true;
                }
            }
        }

        if (flag) {
            if (entity->animator1.frameID & 1)
                entity->animator1.frameID = (entity->animator1.frameID + 1) & 7;
            RSDK.PlaySfx(HeavyShinobi->sfxStick, false, 255);
            RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, (entity->animator1.frameID >> 1) + 10, &entity->animator2, true, 0);
            entity->state = HeavyShinobi_State2_Unknown3;
        }

        if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
            destroyEntity(entity);
    }
    else {
        RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
        CREATE_ENTITY(Explosion, intToVoid(2), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh + 2;
        destroyEntity(entity);
    }
}

void HeavyShinobi_State2_Unknown2(void)
{
    RSDK_THIS(HeavyShinobi);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->visible ^= 1;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void HeavyShinobi_State2_Unknown3(void)
{
    RSDK_THIS(HeavyShinobi);
    if (!HeavyShinobi->health) {
        RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
        CREATE_ENTITY(Explosion, intToVoid(2), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh + 2;
        destroyEntity(entity);
    }
    else {
        RSDK.ProcessAnimation(&entity->animator2);
        entity->animator2.animationSpeed += 2;

        foreach_active(Player, player)
        {
            if (entity->playerPtr == (Entity *)player) {
                if (player->state != Ice_State_FrozenPlayer) {
                    RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
                    CREATE_ENTITY(Explosion, intToVoid(2), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh + 2;
                    --HeavyShinobi->activeShurikens;
                    destroyEntity(entity);
                    foreach_break;
                }
            }
            else if ((player->state != Ice_State_FrozenPlayer || !entity->playerPtr) && Player_CheckBadnikTouch(player, entity, &HeavyShinobi->hitbox5)
                     && Player_CheckHit2(player, entity, true)) {
                RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
                CREATE_ENTITY(Explosion, intToVoid(2), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh + 2;
                --HeavyShinobi->activeShurikens;
                destroyEntity(entity);
                foreach_break;
            }
        }

        EntityPlayer *playerPtr = (EntityPlayer *)entity->playerPtr;
        if (playerPtr) {
            entity->position.x = playerPtr->position.x + entity->playerDistance.x;
            entity->position.y = playerPtr->position.y + entity->playerDistance.y;
        }

        if (--entity->timer <= 0) {
            --HeavyShinobi->activeShurikens;
            RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(2), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh + 2;

            for (int32 i = 0; i < 5; ++i) {
                EntityHeavyShinobi *child = CREATE_ENTITY(HeavyShinobi, intToVoid(SHINOBI_3), entity->position.x, entity->position.y);
                child->rotation           = (entity->animator1.frameID & 0xFFFFFFFE) << 6;
                child->direction          = entity->direction;
                switch (i) {
                    case 0:
                        child->angle = (32 * ((entity->animator1.frameID & 0xFE) - 2));
                        RSDK.Cos512(child->rotation);
                        child->position.y -= RSDK.Sin512(child->rotation) << 10;
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 17, &child->animator1, true, 0);
                        break;
                    case 1:
                        child->angle = (32 * (entity->animator1.frameID & 0xFE) - 120);
                        child->position.x -= 0x380 * RSDK.Cos512(child->rotation);
                        child->position.y -= 0x180 * RSDK.Sin512(child->rotation);
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 18, &child->animator1, true, 0);
                        break;
                    case 2:
                        child->angle = (32 * (entity->animator1.frameID & 0xFE) - 8);
                        child->position.x += 0x380 * RSDK.Cos512(child->rotation);
                        child->position.y -= 0x180 * RSDK.Sin512(child->rotation);
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 18, &child->animator1, true, 0);
                        child->direction ^= FLIP_X;
                        break;
                    case 3:
                        child->angle = (32 * ((entity->animator1.frameID & 0xFE) + 3));
                        child->position.x -= 0x380 * RSDK.Cos512(child->rotation);
                        child->position.y += 0x380 * RSDK.Sin512(child->rotation);
                        RSDK.SetSpriteAnimation(HeavyShinobi->aniFrames, 19, &child->animator1, true, 0);
                        break;
                    case 4:
                        child->angle = (32 * ((entity->animator1.frameID & 0xFE) + 1));
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
                Ice_Unknown8((Entity *)playerPtr);
                playerPtr->state = Player_State_Air;
                Player_CheckHit(playerPtr, entity);
            }
            destroyEntity(entity);
        }
        if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
            destroyEntity(entity);
    }
}

void HeavyShinobi_StateDraw2_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);

    RSDK.DrawSprite(&entity->animator1, NULL, false);
    entity->inkEffect = INK_ADD;

    RSDK.DrawSprite(&entity->animator2, NULL, false);
    entity->inkEffect = INK_NONE;
}

void HeavyShinobi_State3_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    if (HeavyShinobi->health) {
        RSDK.ProcessAnimation(&entity->animator1);
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &HeavyShinobi->hitbox7) && player->state != Ice_State_FrozenPlayer) {
#if RETRO_USE_PLUS
                int32 anim = player->playerAnimator.animationID;
                if (player->characterID == ID_MIGHTY && (anim == ANI_CROUCH || anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH)) {
                    if (!player->uncurlTimer) {
                        RSDK.PlaySfx(Player->sfxPimPom, false, 255);
                        player->uncurlTimer = 30;
                    }
                    int32 angle           = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                    entity->velocity.x  = -0x400 * RSDK.Cos256(angle);
                    entity->velocity.y  = -0x600 * RSDK.Sin256(angle);
                    entity->interaction = false;
                    entity->state       = HeavyShinobi_State2_Unknown2;
                }
                else {
#endif
                    Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
                }
#endif
            }
        }

        if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
            destroyEntity(entity);
    }
    else {
        RSDK.PlaySfx(HeavyShinobi->sfxExplosion, false, 255);
        CREATE_ENTITY(Explosion, intToVoid(2), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderLow;
        destroyEntity(entity);
    }
}

void HeavyShinobi_StateDraw3_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void HeavyShinobi_State4_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x4800;

    if (entity->position.y >= (Zone->screenBoundsB1[0] - 99) << 16) {
        entity->position.y = (Zone->screenBoundsB1[0] - 99) << 16;
        entity->state      = HeavyShinobi_State4_Unknown2;
    }

    foreach_active(Player, player) { Player_CheckCollisionBox(player, entity, &HeavyShinobi->hitbox6); }
}

void HeavyShinobi_State4_Unknown2(void)
{
    RSDK_THIS(HeavyShinobi);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &HeavyShinobi->hitbox6)) {
            Hitbox *playerHitbox = Player_GetHitbox(player);

            int32 left  = 0;
            int32 right = 0;
            if (entity->timer) {
                left  = HeavyShinobi->hitbox6.left;
                right = playerHitbox->right;
            }
            else {
                left  = HeavyShinobi->hitbox6.right;
                right = playerHitbox->left;
            }
            player->position.x = entity->position.x + ((left - right) << 16);
        }
    }

    if (HeavyShinobi->health == 0xFF) {
        RSDK.PlaySfx(HeavyShinobi->sfxExplode, false, 255);
        for (int32 i = 0; i < 0x80; ++i) {
            int32 x = RSDK.Rand(-64, 65) << 16;
            int32 y = RSDK.Rand(-80, 81) << 16;

            EntityIce *ice = CREATE_ENTITY(Ice, intToVoid(3), x + entity->position.x, y + entity->position.y);
            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 1, &ice->animator1, true, 0);
            ice->velocity.x               = RSDK.Rand(-6, 8) << 15;
            ice->velocity.y               = RSDK.Rand(-10, 2) << 15;
            ice->direction                = RSDK.Rand(0, 4);
            ice->animator1.animationSpeed = RSDK.Rand(1, 4);
            ice->active                   = ACTIVE_NORMAL;
        }
        destroyEntity(entity);
    }
}

void HeavyShinobi_StateDraw4_Unknown1(void)
{
    RSDK_THIS(HeavyShinobi);
    Vector2 drawPos;

    drawPos.x                 = entity->position.x;
    drawPos.y                 = entity->position.y - 0x880000;
    entity->animator1.frameID = 3;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.y += 0x500000;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.y += 0x500000;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->direction         = FLIP_Y;
    entity->animator1.frameID = 3;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    entity->direction = FLIP_NONE;
}

#if RETRO_INCLUDE_EDITOR
void HeavyShinobi_EditorDraw(void) {}

void HeavyShinobi_EditorLoad(void) {}
#endif

void HeavyShinobi_Serialize(void) {}
