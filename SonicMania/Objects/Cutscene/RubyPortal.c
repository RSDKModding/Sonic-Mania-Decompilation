// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RubyPortal Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->updateRange.x = TO_FIXED(128);
        self->updateRange.y = TO_FIXED(128);

        RSDK.SetSpriteAnimation(RubyPortal->aniFrames, 0, &self->animator, true, 0);

#if MANIA_USE_PLUS
        if (data) {
            self->state = (Type_StateMachine)data;
        }
        else if (RSDK.CheckSceneFolder("ERZ")) {
            self->state = StateMachine_None;
        }
        else if (RSDK.CheckSceneFolder("TMZ2")) {
            EntityWarpDoor *door = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);

            if (door->classID == WarpDoor->classID) {
                self->hitbox        = door->hitbox;
                door->hitbox.left   = 0;
                door->hitbox.top    = 0x7FFF;
                door->hitbox.right  = 0;
                door->hitbox.bottom = 0x7FFF;
            }

            self->state = RubyPortal_State_AwaitOpenTMZ2;

            if (StarPost->postIDs[0])
                TMZ2Setup_DrawDynTiles_Ruby();
            else
                TMZ2Setup_DrawDynTiles_Eggman();
        }
        else {
            self->state = RubyPortal_State_Opening;
        }
#else
        self->state = StateMachine_None;

        if (!RSDK.CheckSceneFolder("ERZ")) {
            EntityWarpDoor *door = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);

            if (door->classID == WarpDoor->classID) {
                self->hitbox        = door->hitbox;
                door->hitbox.left   = 0;
                door->hitbox.top    = 0x7FFF;
                door->hitbox.right  = 0;
                door->hitbox.bottom = 0x7FFF;
            }

            self->state = RubyPortal_State_AwaitOpenTMZ2;

            if (StarPost->postIDs[0])
                TMZ2Setup_DrawDynTiles_Ruby();
            else
                TMZ2Setup_DrawDynTiles_Eggman();
        }
#endif
    }
}

void RubyPortal_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("ERZ"))
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("Phantom/Portal.bin", SCOPE_STAGE);
#if MANIA_USE_PLUS
    else if (RSDK.CheckSceneFolder("AIZ"))
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Portal.bin", SCOPE_STAGE);
    else
#else // preplus has an explicit check
    else if (RSDK.CheckSceneFolder("TMZ2"))
#endif
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Portal.bin", SCOPE_STAGE);

    RubyPortal->hitbox.left   = -24;
    RubyPortal->hitbox.top    = -24;
    RubyPortal->hitbox.right  = 24;
    RubyPortal->hitbox.bottom = 24;

#if MANIA_USE_PLUS
    RubyPortal->openPortal = false;
    RSDK.AddViewableVariable("Open Portal", &RubyPortal->openPortal, VIEWVAR_BOOL, false, true);
#endif
}

#if MANIA_USE_PLUS
void RubyPortal_HandleTileDestruction(void)
{
    RSDK_THIS(RubyPortal);

    if (!(Zone->timer & 1)) {
        int32 tx     = ((self->position.x - 0x180000) >> 20);
        int32 spawnX = (tx << 20) + 0x80000;

        for (int32 x = 0; x < 4; ++x) {
            int32 ty     = (self->position.y >> 20) - 8;
            int32 spawnY = (ty << 20) + 0x80000;

            for (int32 y = 4; y < 52; y += 3) {
                uint16 tile = RSDK.GetTile(Zone->fgLayer[0], tx, ty);
                if (tile != (uint16)-1) {
                    EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_FIXED), spawnX, spawnY);

                    wall->drawGroup       = Zone->objectDrawGroup[0] + 1;
                    wall->targetLayer     = Zone->fgLayer[0];
                    wall->tileInfo        = tile;
                    wall->drawFX          = FX_SCALE | FX_ROTATE | FX_FLIP;
                    wall->tilePos.x       = tx;
                    wall->tilePos.y       = ty;
                    wall->timer           = y;
                    wall->scale.x         = 0x200;
                    wall->scale.y         = 0x200;
                    wall->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
                    wall->velocity.y      = RSDK.Rand(-0x20000, 0x20000);
                    wall->gravityStrength = 0;
                    wall->active          = ACTIVE_NORMAL;

                    RSDK.SetTile(Zone->fgLayer[0], tx, ty, -1);
                }

                tile = RSDK.GetTile(Zone->fgLayer[1], tx, ty);
                if (tile != (uint16)-1) {
                    EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_FIXED), spawnX, spawnY);

                    wall->drawGroup       = Zone->objectDrawGroup[1];
                    wall->targetLayer     = Zone->fgLayer[1];
                    wall->tileInfo        = tile;
                    wall->drawFX          = FX_SCALE | FX_ROTATE | FX_FLIP;
                    wall->tilePos.x       = tx;
                    wall->tilePos.y       = ty;
                    wall->timer           = y;
                    wall->scale.x         = 0x200;
                    wall->scale.y         = 0x200;
                    wall->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
                    wall->velocity.y      = RSDK.Rand(-0x20000, 0x20000);
                    wall->gravityStrength = 0;
                    wall->active          = ACTIVE_NORMAL;

                    RSDK.SetTile(Zone->fgLayer[1], tx, ty, -1);
                }

                spawnY += 0x100000;
            }

            spawnX += 0x100000;
        }
    }
}
#endif

void RubyPortal_State_AwaitOpenTMZ2(void)
{
    RSDK_THIS(RubyPortal);

#if MANIA_USE_PLUS
    if (TMZBarrier->clearedBarriers == (1 | 2 | 4 | 8) || RubyPortal->openPortal)
#else
    if (TMZBarrier->clearedBarriers == (1 | 2 | 4 | 8))
#endif
        self->state = RubyPortal_State_Opening;
}

void RubyPortal_State_Opening(void)
{
    RSDK_THIS(RubyPortal);

    if (++self->timer >= 120) {
        self->visible = true;

#if MANIA_USE_PLUS
        if (globals->gameMode == MODE_MANIA)
            self->state = RubyPortal_State_Opened;
        else if (!isMainGameMode())
            self->state = RubyPortal_State_Open_WarpDoor;
        else if (WarpDoor)
            self->state = RubyPortal_State_Opened;
        else if (self->timer >= 240)
            self->state = RubyPortal_State_Open_Cutscene;
#else
        if (!isMainGameMode())
            self->state = RubyPortal_State_Open_WarpDoor;
        else
            self->state = RubyPortal_State_Opened;
#endif
    }
}

void RubyPortal_State_Opened(void)
{
    RSDK_THIS(RubyPortal);

    if (self->alpha >= 0x100) {
        if (RSDK.CheckSceneFolder("ERZ")) {
            self->state = StateMachine_None;
        }
        else {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            if (Player_CheckCollisionTouch(player1, self, &RubyPortal->hitbox)) {
                for (int32 p = 0; p < Player->playerCount; ++p) StarPost->postIDs[p] = 0;

                SaveGame_SavePlayerState();

#if !MANIA_USE_PLUS && GAME_VERSION != VER_100
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

#if MANIA_USE_PLUS
                self->state = RubyPortal_State_SaveGameState;
#else
                self->state = StateMachine_None;
#endif

                int32 sfx     = RSDK.Rand(0, RUBYSFX_ATTACK6);
                int32 channel = RSDK.PlaySfx(WarpDoor->sfxRubyAttackL[sfx], false, 0x00);
                RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);

                channel = RSDK.PlaySfx(WarpDoor->sfxRubyAttackR[sfx], false, 0x00);
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

#if MANIA_USE_PLUS
void RubyPortal_State_SaveGameState(void)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player->superState == SUPERSTATE_SUPER || player->state == Player_State_Transform)
        globals->restartPowerups |= 0x80;

    globals->restartMusicID = Music->activeTrack;
}
#endif

void RubyPortal_State_Open_WarpDoor(void)
{
    RSDK_THIS(RubyPortal);

    if (self->alpha >= 0x100) {
        EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);
        if (warpDoor->classID == WarpDoor->classID)
            warpDoor->hitbox = self->hitbox;

        self->state = StateMachine_None;
    }
    else {
        self->scale.x += 8;
        self->scale.y += 8;

        self->alpha += 4;
    }
}

void RubyPortal_State_Open_Cutscene(void)
{
    RSDK_THIS(RubyPortal);

    if (self->alpha >= 0x100) {
        self->state = StateMachine_None;
    }
    else {
        self->scale.x += 8;
        self->scale.y += 8;
        self->alpha += 4;
    }
}

#if MANIA_USE_PLUS
void RubyPortal_State_EncoreEnd(void)
{
    RSDK_THIS(RubyPortal);

    if (!self->timer)
        self->visible = true;

    self->timer++;
    if (self->alpha >= 0x100) {
        if (self->timer == 240) {
            self->timer = 0;
            self->state = RubyPortal_State_EncoreRampage;

            EntityFXFade *fade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
            fade->speedIn      = 512;
            fade->wait         = 16;
            fade->speedOut     = 16;

            PhantomRuby_PlaySfx(RUBYSFX_ATTACK1);
        }
    }
    else {
        self->scale.x += 8;
        self->scale.y += 8;
        self->alpha += 4;
    }
}

void RubyPortal_State_EncoreRampage(void)
{
    RSDK_THIS(RubyPortal);

    RubyPortal_HandleTileDestruction();

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
            wall->alpha     = 0x100;
        }
        else if (wall->timer > 60) {
            wall->alpha -= 8;
        }

        if (rx * rx + ry * ry < 0x900) {
            wall->scale.x -= 0x0C;
            wall->scale.y -= 0x0C;
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
            debris->scale.x -= 0x0C;
            debris->scale.y -= 0x0C;

            debris->gravityStrength = 0;
            debris->position.x += self->velocity.x;
            debris->velocity.x = (self->position.x - debris->position.x) >> 3;
            debris->velocity.y = (self->position.y - debris->position.y) >> 3;

            if (debris->scale.x <= 0)
                destroyEntity(debris);
        }
    }

    foreach_active(PhantomRuby, ruby) { ruby->position.x += self->velocity.x; }
}
#endif

#if RETRO_INCLUDE_EDITOR
void RubyPortal_EditorDraw(void)
{
    RSDK_THIS(RubyPortal);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->scale.x   = 0x200;
    self->scale.y   = 0x200;
    self->alpha     = 0xFF;
    RSDK.SetSpriteAnimation(RubyPortal->aniFrames, 0, &self->animator, true, 0);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void RubyPortal_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("ERZ"))
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("Phantom/Portal.bin", SCOPE_STAGE);
#if MANIA_USE_PLUS
    else if (RSDK.CheckSceneFolder("AIZ"))
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Portal.bin", SCOPE_STAGE);
    else
#else // preplus has an explicit check
    else if (RSDK.CheckSceneFolder("TMZ2"))
#endif
        RubyPortal->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Portal.bin", SCOPE_STAGE);
}
#endif

void RubyPortal_Serialize(void) {}
