#include "../SonicMania.h"

ObjectRubyPortal *RubyPortal;

void RubyPortal_Update(void)
{
    RSDK_THIS(RubyPortal);
    RSDK.ProcessAnimation(&entity->animator);
    StateMachine_Run(entity->state);
}

void RubyPortal_LateUpdate(void) {}

void RubyPortal_StaticUpdate(void) {}

void RubyPortal_Draw(void)
{
    RSDK_THIS(RubyPortal);
    RSDK.CopyPalette(0, 160, 1, 160, 16);
    RSDK.CopyPalette(2, 160, 0, 160, 16);
    RSDK.DrawSprite(&entity->animator, NULL, false);
    RSDK.CopyPalette(1, 160, 0, 160, 16);
}

void RubyPortal_Create(void *data)
{
    RSDK_THIS(RubyPortal);

    entity->drawFX    = FX_SCALE;
    entity->inkEffect = INK_ALPHA;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(RubyPortal->spriteIndex, 0, &entity->animator, true, 0);
        if (data) {
            entity->state = (Type_StateMachine)data;
        }
        else if (RSDK.CheckStageFolder("ERZ")) {
            entity->state = StateMachine_None;
        }
        else if (RSDK.CheckStageFolder("TMZ2")) {
            EntityWarpDoor *door = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, WarpDoor);
            if (door->objectID == WarpDoor->objectID) {
                entity->hitbox      = door->hitbox;
                door->hitbox.left   = 0;
                door->hitbox.left   = -0x800;
                door->hitbox.right  = 0;
                door->hitbox.bottom = -0x800;
            }
            entity->state = RubyPortal_Unknown2;
            if (StarPost->postIDs[0])
                TMZ2_DrawDynTiles2();
            else
                TMZ2_DrawDynTiles1();
        }
        else {
            entity->state = RubyPortal_Unknown3;
        }
    }
}

void RubyPortal_StageLoad(void)
{
    if (RSDK.CheckStageFolder("ERZ")) {
        RubyPortal->spriteIndex = RSDK.LoadSpriteAnimation("Phantom/Portal.bin", SCOPE_STAGE);
    }
#if RETRO_USE_PLUS
    else if (RSDK.CheckStageFolder("AIZ")) {
        RubyPortal->spriteIndex = RSDK.LoadSpriteAnimation("AIZ/Portal.bin", SCOPE_STAGE);
    }
    else {
#else // preplus has an explicit check
    else if (RSDK.CheckStageFolder("TMZ")) {
#endif
        RubyPortal->spriteIndex = RSDK.LoadSpriteAnimation("TMZ1/Portal.bin", SCOPE_STAGE);
    }

    RubyPortal->hitbox.left   = -24;
    RubyPortal->hitbox.top    = -23;
    RubyPortal->hitbox.right  = 24;
    RubyPortal->hitbox.bottom = 24;
    RubyPortal->openPortal    = false;
#if RETRO_USE_PLUS 
    RSDK.SetDebugValue("Open Portal", &RubyPortal->openPortal, DTYPE_BOOL, false, true);
#endif
}

void RubyPortal_Unknown1(void)
{
    RSDK_THIS(RubyPortal);
    if (!(Zone->timer & 1)) {
        int tx     = ((entity->position.x - 0x180000) >> 20);
        int spawnX = (tx << 20) + 0x80000;

        for (int x = 0; x < 4; ++x) {
            int ty     = (entity->position.y >> 20) - 8;
            int spawnY = (ty << 20) + 0x80000;
            for (int y = 4; y < 52; y += 3) {
                ushort tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
                if (tile != 0xFFFF) {
                    EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, intToVoid(1), spawnX, spawnY);
                    wall->drawOrder           = Zone->drawOrderLow + 1;
                    wall->layerID             = Zone->fgLow;
                    wall->tileInfo            = tile;
                    wall->drawFX              = FX_SCALE | FX_ROTATE | FX_FLIP;
                    wall->tilePos.x           = tx;
                    wall->tilePos.y           = ty;
                    wall->timer               = y;
                    wall->scale.x             = 0x200;
                    wall->scale.y             = 0x200;
                    wall->velocity.x          = RSDK.Rand(-0x20000, 0x20000);
                    wall->velocity.y          = RSDK.Rand(-0x20000, 0x20000);
                    wall->gravityStrength     = 0;
                    wall->active              = ACTIVE_NORMAL;
                    RSDK.SetTileInfo(Zone->fgLow, tx, ty, 0xFFFF);
                }

                tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
                if (tile != 0xFFFF) {
                    EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, intToVoid(1), spawnX, spawnY);
                    wall->drawOrder           = Zone->drawOrderHigh;
                    wall->layerID             = Zone->fgHigh;
                    wall->tileInfo            = tile;
                    wall->drawFX              = FX_SCALE | FX_ROTATE | FX_FLIP;
                    wall->tilePos.x           = tx;
                    wall->tilePos.y           = ty;
                    wall->timer               = y;
                    wall->scale.x             = 0x200;
                    wall->scale.y             = 0x200;
                    wall->velocity.x          = RSDK.Rand(-0x20000, 0x20000);
                    wall->velocity.y          = RSDK.Rand(-0x20000, 0x20000);
                    wall->gravityStrength     = 0;
                    wall->active              = ACTIVE_NORMAL;
                    RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                }
                spawnY += 0x100000;
            }
            spawnX += 0x100000;
        }
    }
}

void RubyPortal_Unknown2(void)
{
    RSDK_THIS(RubyPortal);
    if (TMZBarrier->flags == 15 || RubyPortal->openPortal)
        entity->state = RubyPortal_Unknown3;
}

void RubyPortal_Unknown3(void)
{
    RSDK_THIS(RubyPortal);

    if (++entity->timer >= 120) {
        entity->visible = true;
        if (globals->gameMode == MODE_MANIA) {
            entity->state = RubyPortal_Unknown4;
        }
        else if (!isMainGameMode()) {
            entity->state = RubyPortal_Unknown6;
        }
        else {
            if (WarpDoor) {
                entity->state = RubyPortal_Unknown4;
            }
            else if (entity->timer >= 240) {
                entity->state = RubyPortal_Unknown7;
            }
        }
    }
}

void RubyPortal_Unknown4(void)
{
    RSDK_THIS(RubyPortal);

    if (entity->alpha >= 256) {
        if (RSDK.CheckStageFolder("ERZ")) {
            entity->state = StateMachine_None;
        }
        else {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            if (Player_CheckCollisionTouch(player1, entity, &RubyPortal->hitbox)) {
                for (int p = 0; p < Player->playerCount; ++p) {
                    StarPost->postIDs[p] = 0;
                }

                SaveGame_SavePlayerState();
                globals->suppressAutoMusic = true;
                globals->suppressTitlecard = true;
                if (player1->invincibleTimer <= 1) {
                    if (player1->speedShoesTimer > 1)
                        player1->speedShoesTimer = 1;
                }
                else {
                    player1->invincibleTimer = 1;
                }
                ++RSDK_sceneInfo->listPos;
                if (!RSDK.CheckValidScene())
                    RSDK.LoadScene("Presentation", "Title Screen");
                Zone_StartFadeOut(16, 0xF0F0F0);
                entity->state = RubyPortal_Unknown5;

                int sfx     = RSDK.Rand(0, RUBYSFX_ATTACK6);
                int channel = RSDK.PlaySFX(WarpDoor->sfxRubyAttackL[sfx], 0, 0);
                RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
                channel = RSDK.PlaySFX(WarpDoor->sfxRubyAttackR[sfx], 0, 0);
                RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
            }
        }
    }
    else {
        entity->scale.x += 8;
        entity->scale.y += 8;
        entity->alpha += 4;
    }
}

void RubyPortal_Unknown5(void)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player->superState == 2 || player->state == Player_State_Transform)
        globals->restartPowerups |= 0x80;
    globals->restartMusicID = Music->activeTrack;
}

void RubyPortal_Unknown6(void)
{
    RSDK_THIS(RubyPortal);

    if (entity->alpha >= 256) {
        EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, WarpDoor);
        if (warpDoor->objectID == WarpDoor->objectID)
            warpDoor->hitbox = entity->hitbox;
        entity->state = StateMachine_None;
    }
    else {
        entity->scale.x += 8;
        entity->scale.y += 8;
        entity->alpha += 4;
    }
}

void RubyPortal_Unknown7(void)
{
    RSDK_THIS(RubyPortal);

    if (entity->alpha >= 256) {
        entity->state = StateMachine_None;
    }
    else {
        entity->scale.x += 8;
        entity->scale.y += 8;
        entity->alpha += 4;
    }
}

void RubyPortal_Unknown8(void)
{
    RSDK_THIS(RubyPortal);
    if (!entity->timer)
        entity->visible = true;
    entity->timer++;
    if (entity->timer >= 256) {
        if (entity->timer == 240) {
            entity->timer      = 0;
            entity->state      = RubyPortal_Unknown9;
            EntityFXFade *fade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
            fade->speedIn      = 512;
            fade->wait         = 16;
            fade->speedOut     = 16;
            PhantomRuby_PlaySFX(RUBYSFX_ATTACK1);
        }
    }
    else {
        entity->scale.x += 8;
        entity->scale.y += 8;
        entity->alpha += 4;
    }
}

void RubyPortal_Unknown9(void)
{
    RSDK_THIS(RubyPortal);
    RubyPortal_Unknown1();
    entity->position.x += entity->velocity.x;

    foreach_active(BreakableWall, wall)
    {
        int rx    = (entity->position.x - wall->position.x) >> 16;
        int ry    = (entity->position.y - wall->position.y) >> 16;
        int angle = RSDK.ATan2(rx, ry);
        wall->velocity.x += 8 * RSDK.Cos256(angle);
        wall->velocity.y += 8 * RSDK.Sin256(angle);
        wall->position.x += entity->velocity.x;
        ++wall->timer;
        if (wall->timer == 60) {
            wall->inkEffect = INK_ALPHA;
            wall->alpha     = 256;
        }
        else if (wall->timer > 60) {
            wall->alpha -= 8;
        }

        if (rx * rx + ry * ry < 0x900) {
            wall->scale.x -= 12;
            wall->scale.y -= 12;
            wall->velocity.x = (entity->position.x - wall->position.x) >> 3;
            wall->velocity.y = (entity->position.y - wall->position.y) >> 3;
            if (wall->scale.x <= 0)
                destroyEntity(wall);
        }
    }

    foreach_active(Debris, debris)
    {
        int rx = (entity->position.x - debris->position.x) >> 16;
        int ry = (entity->position.y - debris->position.y) >> 16;
        if (rx * rx + ry * ry < 0x900) {
            debris->scale.x -= 12;
            debris->scale.y -= 12;
            debris->gravity = 0;
            debris->position.x += entity->velocity.x;
            debris->velocity.x = (entity->position.x - debris->position.x) >> 3;
            debris->velocity.y = (entity->position.y - debris->position.y) >> 3;
            if (debris->scale.x <= 0)
                destroyEntity(debris);
        }
    }

    foreach_active(PhantomRuby, ruby) { ruby->position.x += entity->velocity.x; }
}

void RubyPortal_EditorDraw(void) {}

void RubyPortal_EditorLoad(void) {}

void RubyPortal_Serialize(void) {}
