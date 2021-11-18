#include "SonicMania.h"

ObjectRubyPortal *RubyPortal;

void RubyPortal_Update(void)
{
    RSDK_THIS(RubyPortal);
    RSDK.ProcessAnimation(&self->animator);
    StateMachine_Run(self->state);
}

void RubyPortal_LateUpdate(void) {}

void RubyPortal_StaticUpdate(void) {}

void RubyPortal_Draw(void)
{
    RSDK_THIS(RubyPortal);
    RSDK.CopyPalette(0, 160, 1, 160, 16);
    RSDK.CopyPalette(2, 160, 0, 160, 16);
    RSDK.DrawSprite(&self->animator, NULL, false);
    RSDK.CopyPalette(1, 160, 0, 160, 16);
}

void RubyPortal_Create(void *data)
{
    RSDK_THIS(RubyPortal);

    self->drawFX    = FX_SCALE;
    self->inkEffect = INK_ALPHA;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(RubyPortal->aniFrames, 0, &self->animator, true, 0);
#if RETRO_USE_PLUS
        if (data) {
            self->state = (Type_StateMachine)data;
        }
        else if (RSDK.CheckStageFolder("ERZ")) {
            self->state = StateMachine_None;
        }
        else if (RSDK.CheckStageFolder("TMZ2")) {
            EntityWarpDoor *door = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);
            if (door->objectID == WarpDoor->objectID) {
                self->hitbox      = door->hitbox;
                door->hitbox.left   = 0;
                door->hitbox.top    = 0x7FFF;
                door->hitbox.right  = 0;
                door->hitbox.bottom = 0x7FFF;
            }
            self->state = RubyPortal_Unknown2;
            if (StarPost->postIDs[0])
                TMZ2_DrawDynTiles_Ruby();
            else
                TMZ2_DrawDynTiles_Eggman();
        }
        else {
            self->state = RubyPortal_Unknown3;
        }
#else
        self->state = StateMachine_None;
        if (!RSDK.CheckStageFolder("ERZ")) {
            EntityWarpDoor *door = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);
            if (door->objectID == WarpDoor->objectID) {
                self->hitbox      = door->hitbox;
                door->hitbox.left   = 0;
                door->hitbox.top    = 0x7FFF;
                door->hitbox.right  = 0;
                door->hitbox.bottom = 0x7FFF;
            }
            self->state = RubyPortal_Unknown2;
            if (StarPost->postIDs[0])
                TMZ2_DrawDynTiles_Ruby();
            else
                TMZ2_DrawDynTiles_Eggman();
        }
#endif
    }
}

void RubyPortal_StageLoad(void)
{
    if (RSDK.CheckStageFolder("ERZ"))
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("Phantom/Portal.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
    else if (RSDK.CheckStageFolder("AIZ"))
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Portal.bin", SCOPE_STAGE);
    else
#else // preplus has an explicit check
    else if (RSDK.CheckStageFolder("TMZ2"))
#endif
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Portal.bin", SCOPE_STAGE);

    RubyPortal->hitbox.left   = -24;
    RubyPortal->hitbox.top    = -24;
    RubyPortal->hitbox.right  = 24;
    RubyPortal->hitbox.bottom = 24;
#if RETRO_USE_PLUS 
    RubyPortal->openPortal    = false;
    RSDK.SetDebugValue("Open Portal", &RubyPortal->openPortal, DTYPE_BOOL, false, true);
#endif
}

void RubyPortal_Unknown1(void)
{
    RSDK_THIS(RubyPortal);
    if (!(Zone->timer & 1)) {
        int32 tx     = ((self->position.x - 0x180000) >> 20);
        int32 spawnX = (tx << 20) + 0x80000;

        for (int32 x = 0; x < 4; ++x) {
            int32 ty     = (self->position.y >> 20) - 8;
            int32 spawnY = (ty << 20) + 0x80000;
            for (int32 y = 4; y < 52; y += 3) {
                uint16 tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
                if (tile != 0xFFFF) {
                    EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_FIXED), spawnX, spawnY);
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
                    EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_FIXED), spawnX, spawnY);
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
#if RETRO_USE_PLUS
    if (TMZBarrier->flags == 0xF || RubyPortal->openPortal)
#else
    if (TMZBarrier->flags == 0xF)
#endif
        self->state = RubyPortal_Unknown3;
}

void RubyPortal_Unknown3(void)
{
    RSDK_THIS(RubyPortal);

    if (++self->timer >= 120) {
        self->visible = true;
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_MANIA) {
            self->state = RubyPortal_Unknown4;
        }
        else if (!isMainGameMode()) {
            self->state = RubyPortal_Unknown6;
        }
        else {
            if (WarpDoor) {
                self->state = RubyPortal_Unknown4;
            }
            else if (self->timer >= 240) {
                self->state = RubyPortal_Unknown7;
            }
        }
#else
        if (!isMainGameMode())
            self->state = RubyPortal_Unknown6;
        else
            self->state = RubyPortal_Unknown4;
#endif
    }
}

void RubyPortal_Unknown4(void)
{
    RSDK_THIS(RubyPortal);

    if (self->alpha >= 256) {
        if (RSDK.CheckStageFolder("ERZ")) {
            self->state = StateMachine_None;
        }
        else {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            if (Player_CheckCollisionTouch(player1, self, &RubyPortal->hitbox)) {
                for (int32 p = 0; p < Player->playerCount; ++p) {
                    StarPost->postIDs[p] = 0;
                }

                SaveGame_SavePlayerState();

#if !RETRO_USE_PLUS && RETRO_GAMEVER != VER_100
                if (player1->superState == SUPERSTATE_SUPER)
                    globals->restartPowerups |= 0x80;
#endif

                globals->suppressAutoMusic = true;
                globals->suppressTitlecard = true;
                if (player1->invincibleTimer <= 1) {
                    if (player1->speedShoesTimer > 1)
                        player1->speedShoesTimer = 1;
                }
                else {
                    player1->invincibleTimer = 1;
                }
                ++SceneInfo->listPos;
                if (!RSDK.CheckValidScene())
                    RSDK.SetScene("Presentation", "Title Screen");
                Zone_StartFadeOut(16, 0xF0F0F0);

#if RETRO_USE_PLUS
                self->state = RubyPortal_Unknown5;
#endif

                int32 sfx     = RSDK.Rand(0, RUBYSFX_ATTACK6);
                int32 channel = RSDK.PlaySfx(WarpDoor->sfxRubyAttackL[sfx], 0, 0);
                RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
                channel = RSDK.PlaySfx(WarpDoor->sfxRubyAttackR[sfx], 0, 0);
                RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
            }
        }
    }
    else {
        self->scale.x += 8;
        self->scale.y += 8;
        self->alpha += 4;
    }
}

void RubyPortal_Unknown5(void)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player->superState == SUPERSTATE_SUPER || player->state == Player_State_Transform)
        globals->restartPowerups |= 0x80;
    globals->restartMusicID = Music->activeTrack;
}

void RubyPortal_Unknown6(void)
{
    RSDK_THIS(RubyPortal);

    if (self->alpha >= 256) {
        EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);
        if (warpDoor->objectID == WarpDoor->objectID)
            warpDoor->hitbox = self->hitbox;
        self->state = StateMachine_None;
    }
    else {
        self->scale.x += 8;
        self->scale.y += 8;
        self->alpha += 4;
    }
}

void RubyPortal_Unknown7(void)
{
    RSDK_THIS(RubyPortal);

    if (self->alpha >= 256) {
        self->state = StateMachine_None;
    }
    else {
        self->scale.x += 8;
        self->scale.y += 8;
        self->alpha += 4;
    }
}

void RubyPortal_Unknown8(void)
{
    RSDK_THIS(RubyPortal);
    if (!self->timer)
        self->visible = true;
    self->timer++;
    if (self->alpha >= 0x100) {
        if (self->timer == 240) {
            self->timer      = 0;
            self->state      = RubyPortal_Unknown9;
            EntityFXFade *fade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
            fade->speedIn      = 512;
            fade->wait         = 16;
            fade->speedOut     = 16;
            PhantomRuby_PlaySFX(RUBYSFX_ATTACK1);
        }
    }
    else {
        self->scale.x += 8;
        self->scale.y += 8;
        self->alpha += 4;
    }
}

void RubyPortal_Unknown9(void)
{
    RSDK_THIS(RubyPortal);
    RubyPortal_Unknown1();
    self->position.x += self->velocity.x;

    foreach_active(BreakableWall, wall)
    {
        int32 rx    = (self->position.x - wall->position.x) >> 16;
        int32 ry    = (self->position.y - wall->position.y) >> 16;
        int32 angle = RSDK.ATan2(rx, ry);
        wall->velocity.x += 8 * RSDK.Cos256(angle);
        wall->velocity.y += 8 * RSDK.Sin256(angle);
        wall->position.x += self->velocity.x;
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
            wall->velocity.x = (self->position.x - wall->position.x) >> 3;
            wall->velocity.y = (self->position.y - wall->position.y) >> 3;
            if (wall->scale.x <= 0)
                destroyEntity(wall);
        }
    }

    foreach_active(Debris, debris)
    {
        int32 rx = (self->position.x - debris->position.x) >> 16;
        int32 ry = (self->position.y - debris->position.y) >> 16;
        if (rx * rx + ry * ry < 0x900) {
            debris->scale.x -= 12;
            debris->scale.y -= 12;
            debris->gravity = 0;
            debris->position.x += self->velocity.x;
            debris->velocity.x = (self->position.x - debris->position.x) >> 3;
            debris->velocity.y = (self->position.y - debris->position.y) >> 3;
            if (debris->scale.x <= 0)
                destroyEntity(debris);
        }
    }

    foreach_active(PhantomRuby, ruby) { ruby->position.x += self->velocity.x; }
}

#if RETRO_INCLUDE_EDITOR
void RubyPortal_EditorDraw(void)
{
    RSDK_THIS(RubyPortal);
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->scale.x       = 0x200;
    self->scale.y       = 0x200;
    self->alpha         = 0xFF;
    RSDK.SetSpriteAnimation(RubyPortal->aniFrames, 0, &self->animator, true, 0);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void RubyPortal_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("ERZ"))
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("Phantom/Portal.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
    else if (RSDK.CheckStageFolder("AIZ"))
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Portal.bin", SCOPE_STAGE);
    else
#else // preplus has an explicit check
    else if (RSDK.CheckStageFolder("TMZ2"))
#endif
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Portal.bin", SCOPE_STAGE);
}
#endif

void RubyPortal_Serialize(void) {}
