// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Hatch Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHatch *Hatch;

void Hatch_Update(void)
{
    RSDK_THIS(Hatch);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;

    if (self->useMoveLayer) {
        foreach_all(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                TileLayer *moveLayer = RSDK.GetTileLayer(Zone->moveLayer);
                if (!player->sidekick) {
                    moveLayer->scrollPos               = -self->vScrollPos;
                    moveLayer->scrollInfo[0].scrollPos = -self->hScrollPos;
                }

                player->collisionLayers |= Zone->moveLayerMask;
                player->moveLayerPosition.x = moveLayer->scrollInfo[0].scrollPos;
                player->moveLayerPosition.y = moveLayer->scrollPos;
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;

    StateMachine_Run(self->state);
}

void Hatch_LateUpdate(void) {}

void Hatch_StaticUpdate(void)
{
    foreach_all(Hatch, hatch) { RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(hatch)); }
}

void Hatch_Draw(void)
{
    RSDK_THIS(Hatch);

    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        RSDK.DrawSprite(&self->hatchAnimator, NULL, false);
    }
    else {
        if (self->hatchAnimator.animationID == 2 && self->hatchAnimator.frameID >= 3)
            RSDK.DrawSprite(&self->hatchAnimator, NULL, false);

        RSDK.DrawSprite(&self->baseAnimator, NULL, false);
    }
}

void Hatch_Create(void *data)
{
    RSDK_THIS(Hatch);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_BOUNDS;
        self->drawGroup = Zone->objectDrawGroup[0] + 1;

        self->updateRange.x = 16 * MAX(abs(self->subOff1.x), abs(self->subOff2.x));
        self->updateRange.y = 16 * MAX(abs(self->subOff1.y), abs(self->subOff2.y));

        RSDK.SetSpriteAnimation(Hatch->aniFrames, 0, &self->baseAnimator, true, 0);
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &self->hatchAnimator, true, 4);

        self->hitbox.left   = (self->subOff1.x >> 12) - ((self->position.x >> 16) & 0xF) + 16;
        self->hitbox.top    = (self->subOff1.y >> 12) - ((self->position.y >> 16) & 0xF) + 16;
        self->hitbox.right  = (self->subOff2.x >> 12) - ((self->position.x >> 16) & 0xF) + 16;
        self->hitbox.bottom = (self->subOff2.y >> 12) - ((self->position.y >> 16) & 0xF) + 16;
        self->startPos      = self->position;

        EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);
        if (warpDoor->classID == WarpDoor->classID) {
            self->hitboxWarpDoor    = warpDoor->hitbox;
            warpDoor->hitbox.left   = 0;
            warpDoor->hitbox.top    = 0x7FFF;
            warpDoor->hitbox.right  = 0;
            warpDoor->hitbox.bottom = 0x7FFF;
        }

        switch (self->go) {
            case HATCH_GO_SUBENTRYHATCH_SMOGONLY:
                if (!OOZSetup->useSmogEffect) {
                    self->vScrollPos += self->depth << 16;
                    self->position.y += self->depth << 16;
                    destroyEntity(RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor));
                    self->useMoveLayer = true;
                    break;
                }
                // [[fallthrough]]
            case HATCH_GO_SUBENTRYHATCH:
                self->visible      = true;
                self->state        = Hatch_State_SubEntryHatch;
                self->useMoveLayer = true;
                break;

            case HATCH_GO_SUBEXITHATCH_COPYTILES:
                // Copies the Hatch Tiles from the move layer
                RSDK.CopyTileLayer(Zone->moveLayer, (self->position.x >> 20) - 2, (self->position.y >> 20) - 1, Zone->moveLayer, 1, 7, 4,
                                    2); // Copy Hatch Tiles
                RSDK.CopyTileLayer(Zone->fgLayer[1], (self->position.x >> 20) - 3, (self->position.y >> 20) + 1, Zone->moveLayer, 16, 1, 6,
                                    2); // Copy [???] Tiles (There's nothing there in the scene)
                // [[fallthrough]]
            case HATCH_GO_SUBEXITHATCH_NOCOPY:
                self->visible      = false;
                self->useMoveLayer = false;
                self->state        = Hatch_State_SubExitHatch;
                break;

            case HATCH_GO_SUBEXIT:
                self->visible      = false;
                self->state        = Hatch_State_SubExit;
                self->useMoveLayer = true;
                break;

            default: break;
        }
    }
}

void Hatch_StageLoad(void)
{
    // OOZ1 Check...? why? its never used in OOZ1...
    if (RSDK.CheckSceneFolder("OOZ1") || RSDK.CheckSceneFolder("OOZ2"))
        Hatch->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Hatch.bin", SCOPE_STAGE);

    Hatch->hitboxSubExit.left   = -16;
    Hatch->hitboxSubExit.top    = -16;
    Hatch->hitboxSubExit.right  = 16;
    Hatch->hitboxSubExit.bottom = 16;

    Hatch->hitboxRange.left   = -128;
    Hatch->hitboxRange.top    = -128;
    Hatch->hitboxRange.right  = 128;
    Hatch->hitboxRange.bottom = 128;

    Hatch->hitboxL.left   = -24;
    Hatch->hitboxL.top    = 0;
    Hatch->hitboxL.right  = -12;
    Hatch->hitboxL.bottom = 8;

    Hatch->hitboxR.left   = 12;
    Hatch->hitboxR.top    = 0;
    Hatch->hitboxR.right  = 24;
    Hatch->hitboxR.bottom = 8;

    Hatch->hitboxEntry.left   = -12;
    Hatch->hitboxEntry.top    = 8;
    Hatch->hitboxEntry.right  = 12;
    Hatch->hitboxEntry.bottom = 24;

    Hatch->active = ACTIVE_ALWAYS;

    Hatch->sfxHatchOpen  = RSDK.GetSfx("OOZ/SubHatchOpen.wav");
    Hatch->sfxHatchClose = RSDK.GetSfx("OOZ/SubHatchClose.wav");
    Hatch->sfxDescend    = RSDK.GetSfx("OOZ/SubDescend.wav");
    Hatch->sfxSurface    = RSDK.GetSfx("OOZ/SubSurface.wav");
    Hatch->sfxGasPop     = RSDK.GetSfx("OOZ/GasPop.wav");
}

void Hatch_State_SubEntryHatch(void)
{
    RSDK_THIS(Hatch);

    RSDK.ProcessAnimation(&self->hatchAnimator);

    int32 entered = 0;
    foreach_all(Player, player)
    {
        // Bug Details:
        // this does foreach_all, instead of foreach_active
        // meaning that even killed players are included
        // Fix:
        // this entire block of code should have a Player_CheckValidState call to make sure the player isn't dead or some other "invalid" state

        // Extra notes: if you manage to die and fall into the hatch, you'll be brought back (sorta, this is because it changes your state from
        // Player_State_Death to Player_State_Static) though the death state stuff will still be applied so you'll be on the highest layer (until its
        // changed) and the player->active var will be set to ACTIVE_ALWAYS this means you can do really weird stuff such as move during the pause
        // menu

        if (Player_CheckCollisionBox(player, self, &Hatch->hitboxL) == C_TOP) {
            entered = 1;
        }
        else if (Player_CheckCollisionBox(player, self, &Hatch->hitboxR) == C_TOP) {
            entered = 1;
        }
        else if (Player_CheckCollisionBox(player, self, &Hatch->hitboxEntry) == C_TOP) {
            if (player->onGround) {
                if (!player->sidekick) {
                    self->active            = ACTIVE_NORMAL;
                    self->playerPtr         = player;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->groundVel       = 0;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->interaction     = false;

                    if (player->animator.animationID != ANI_JUMP)
                        RSDK.PlaySfx(Player->sfxRoll, false, 0xFF);

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->state = Player_State_Static;
                    RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &self->hatchAnimator, false, 0);
                    self->state = Hatch_State_PlayerEntered;
                    entered     = 2;
                    foreach_break;
                }
            }
            else {
                entered = 1;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &Hatch->hitboxRange))
                entered = 1;
        }
    }

    if (entered == 1) {
        if (self->hatchAnimator.animationID == 2)
            RSDK.PlaySfx(Hatch->sfxHatchOpen, false, 0xFF);

        RSDK.SetSpriteAnimation(Hatch->aniFrames, 1, &self->hatchAnimator, false, 0);
    }
    else if (!entered) {
        if (self->hatchAnimator.animationID == 1)
            RSDK.PlaySfx(Hatch->sfxHatchClose, false, 0xFF);

        RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &self->hatchAnimator, false, 0);
    }
}

void Hatch_State_PlayerEntered(void)
{
    RSDK_THIS(Hatch);

    RSDK.ProcessAnimation(&self->hatchAnimator);

    if (self->hatchAnimator.frameID == 4) {
        EntityPlayer *player = self->playerPtr;

        self->visible          = false;
        player->visible        = false;
        player->blinkTimer     = 0;
        player->tileCollisions = TILECOLLISION_NONE;

        RSDK.CopyTileLayer(Zone->moveLayer, (self->position.x >> 20) - 2, (self->position.y >> 20) - 1, Zone->moveLayer, 1, 7, 4, 2);
        RSDK.CopyTileLayer(Zone->fgLayer[1], (self->position.x >> 20) - 3, (self->position.y >> 20) + 1, Zone->moveLayer, 16, 1, 6, 2);

        Zone->deathBoundary[0] = 0x7FFF0000;
        Zone->deathBoundary[1] = 0x7FFF0000;
        Zone->deathBoundary[2] = 0x7FFF0000;
        Zone->deathBoundary[3] = 0x7FFF0000;

        RSDK.PlaySfx(Hatch->sfxDescend, false, 0xFF);
        self->state = Hatch_State_Descend;

        EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
        zone->fadeColor  = 0;
        zone->timer      = 0;
        zone->drawGroup  = 15;
        zone->visible    = true;
        zone->stateDraw  = Zone_Draw_Fade;
    }
}

void Hatch_State_Descend(void)
{
    RSDK_THIS(Hatch);

    EntityPlayer *player = self->playerPtr;
    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    ++self->timer;
    self->vScrollPos += 0x10000;
    self->position.y += 0x10000;
    player->position.y += 0x10000;

    zone->timer += 2;

    if (self->timer >= self->depth) {
        if (self->dest) { // XOffset of where to surface
            self->timer = 0;
            self->state = Hatch_State_MoveToDestPos;
        }
        else { // Uses the WarpDoor obj for transport
            player->state            = Player_State_Air;
            EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);
            if (warpDoor->classID == WarpDoor->classID) {
                Zone->cameraBoundsB[RSDK.GetEntitySlot(player)] = 0x7FFF;
                warpDoor->hitbox                                = self->hitboxWarpDoor;
                warpDoor->position.y                            = self->position.y;
                player->tileCollisions                          = TILECOLLISION_DOWN;
                player->interaction                             = true;
                player->visible                                 = true;
            }

            RSDK.GetTileLayer(Zone->moveLayer)->scrollPos = 0;

            self->position.x = self->startPos.x;
            self->position.y = self->startPos.y;

            if (self->go != HATCH_GO_SUBENTRYHATCH_SMOGONLY)
                self->useMoveLayer = false;

            self->state = Hatch_State_FadeIn;
        }
    }
}

void Hatch_State_MoveToDestPos(void)
{
    RSDK_THIS(Hatch);

    EntityPlayer *player = self->playerPtr;

    self->hScrollPos += self->dest << 16;
    self->position.x += self->dest << 16;

    player->position.x += self->dest << 16;
    player->camera->position.x += self->dest << 16;

    ScreenInfo[player->camera->screenID].position.x += self->dest;

    self->timer = 0;
    RSDK.PlaySfx(Hatch->sfxSurface, false, 0xFF);

    self->state = Hatch_State_Surfacing;
}

void Hatch_State_Surfacing(void)
{
    RSDK_THIS(Hatch);

    EntityPlayer *player = self->playerPtr;
    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    ++self->timer;

    self->vScrollPos -= 0x10000;
    self->position.y -= 0x10000;
    player->position.y -= 0x10000;

    zone->timer -= 2;

    if (self->timer >= self->depth) {
        self->timer      = 0;
        self->visible    = true;
        self->position.x = player->position.x;
        self->position.y = player->position.y + 0x80000;

        RSDK.SetSpriteAnimation(Hatch->aniFrames, 1, &self->hatchAnimator, false, 0);
        self->state = Hatch_State_OpenHatchReleasePlayer;
    }
}

void Hatch_State_OpenHatchReleasePlayer(void)
{
    RSDK_THIS(Hatch);

    EntityPlayer *player = self->playerPtr;
    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    RSDK.ProcessAnimation(&self->hatchAnimator);
    zone->timer -= 2;

    if (++self->timer == 30) {
        player->state          = Player_State_Air;
        player->velocity.y     = -0xA0000;
        player->onGround       = false;
        player->tileCollisions = TILECOLLISION_DOWN;
        player->interaction    = true;
        player->applyJumpCap   = false;
        player->visible        = true;
        RSDK.PlaySfx(Hatch->sfxGasPop, false, 255);
    }
    else if (self->timer == 38) {
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &self->hatchAnimator, false, 0);
        self->state = Hatch_State_CloseHatch;
    }
}

void Hatch_State_CloseHatch(void)
{
    RSDK_THIS(Hatch);

    EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
    zone->timer -= 2;

    RSDK.ProcessAnimation(&self->hatchAnimator);

    if (self->hatchAnimator.frameID >= self->hatchAnimator.frameCount - 1) {
        self->visible = false;
        self->state   = Hatch_State_FadeIn;
    }
}

void Hatch_State_FadeIn(void)
{
    RSDK_THIS(Hatch);

    EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    zone->timer -= 8;
    if (zone->timer <= 0) {
        self->active = ACTIVE_BOUNDS;
        self->state  = StateMachine_None;
        zone->timer  = 0;
    }
}

void Hatch_State_SubExit(void)
{
    RSDK_THIS(Hatch);

    foreach_all(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Hatch->hitboxSubExit) && !player->sidekick) {
            self->active           = ACTIVE_NORMAL;
            self->playerPtr        = player;
            self->stateStore       = player->state;
            self->interactionStore = player->interaction;
            self->tileColStore     = player->tileCollisions;

            player->velocity.x      = 0;
            player->groundVel       = 0;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->interaction     = false;
            player->tileCollisions  = TILECOLLISION_NONE;
            player->state           = Player_State_Static;

            self->state = Hatch_State_FadeOut;
            foreach_break;
        }
    }
}

void Hatch_State_FadeOut(void)
{
    RSDK_THIS(Hatch);

    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
    EntityPlayer *player = self->playerPtr;

    zone->timer += 8;
    if (zone->timer >= 512) {
        EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);

        if (warpDoor->classID == WarpDoor->classID) {
            Zone->cameraBoundsB[RSDK.GetEntitySlot(player)] = 0x7FFF;

            warpDoor->hitbox     = self->hitboxWarpDoor;
            warpDoor->position.y = player->position.y;

            player->state          = self->stateStore;
            player->tileCollisions = self->tileColStore;
            player->interaction    = self->interactionStore;
            player->visible        = true;
        }

        if (self->depth) {
            self->state = Hatch_State_FadeIn;
        }
        else {
            self->active = ACTIVE_BOUNDS;
            self->state  = StateMachine_None;
        }
    }
}

void Hatch_State_SubExitHatch(void)
{
    RSDK_THIS(Hatch);

    EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    if (zone->timer) {
        foreach_all(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Hatch->hitboxSubExit) && !player->sidekick) {
                self->hScrollPos += self->dest << 16;
                self->vScrollPos += self->depth << 16;
                self->useMoveLayer = true;

                if (self->go == HATCH_GO_SUBEXITHATCH_NOCOPY) {
                    self->state = StateMachine_None;
                }
                else {
                    self->playerPtr        = player;
                    self->active           = ACTIVE_NORMAL;
                    self->stateStore       = player->state;
                    self->interactionStore = player->interaction;
                    self->tileColStore     = player->tileCollisions;
                    self->position.x += self->dest << 16;
                    self->velocity.y = player->velocity.y;

                    player->camera->position.x += self->dest << 16;
                    player->camera->position.y += self->depth << 16;
                    player->position.x += self->dest << 16;
                    player->position.y += self->depth << 16;
                    player->velocity.y      = 0;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->interaction     = false;
                    player->tileCollisions  = TILECOLLISION_NONE;
                    player->visible         = false;
                    player->state           = Player_State_Static;

                    RSDK.PlaySfx(Hatch->sfxSurface, false, 255);
                    self->state = Hatch_State_Surfacing;
                }
                foreach_break;
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void Hatch_EditorDraw(void)
{
    RSDK_THIS(Hatch);

    self->drawGroup = Zone->objectDrawGroup[0] + 1;

    self->updateRange.x = 16 * MAX(abs(self->subOff1.x), abs(self->subOff2.x));
    self->updateRange.y = 16 * MAX(abs(self->subOff1.y), abs(self->subOff2.y));

    RSDK.SetSpriteAnimation(Hatch->aniFrames, 0, &self->baseAnimator, false, 0);
    RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &self->hatchAnimator, false, 4);

    RSDK.DrawSprite(&self->hatchAnimator, NULL, false);
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x, self->position.y + (self->depth << 16), 0x00FF00, INK_NONE, 0xFF);

        self->hitbox.left   = (self->subOff1.x >> 12) - ((self->position.x >> 16) & 0xF) + 16;
        self->hitbox.top    = (self->subOff1.y >> 12) - ((self->position.y >> 16) & 0xF) + 16;
        self->hitbox.right  = (self->subOff2.x >> 12) - ((self->position.x >> 16) & 0xF) + 16;
        self->hitbox.bottom = (self->subOff2.y >> 12) - ((self->position.y >> 16) & 0xF) + 16;

        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitbox, FLIP_NONE, 0xFF0000);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Hatch_EditorLoad(void)
{
    Hatch->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Hatch.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Hatch, go);
    RSDK_ENUM_VAR("Hatch Entry", HATCH_GO_SUBENTRYHATCH);
    RSDK_ENUM_VAR("Hatch Exit (Copy hatch tiles)", HATCH_GO_SUBEXITHATCH_COPYTILES);
    RSDK_ENUM_VAR("Hatch Entry (Only When Smog is active)", HATCH_GO_SUBENTRYHATCH_SMOGONLY);
    RSDK_ENUM_VAR("Submarine Exit", HATCH_GO_SUBEXIT);
    RSDK_ENUM_VAR("Hatch Exit (Don't copy hatch tiles)", HATCH_GO_SUBEXITHATCH_NOCOPY);
}
#endif

void Hatch_Serialize(void)
{
    RSDK_EDITABLE_VAR(Hatch, VAR_UINT8, go);
    RSDK_EDITABLE_VAR(Hatch, VAR_VECTOR2, subOff1);
    RSDK_EDITABLE_VAR(Hatch, VAR_VECTOR2, subOff2);
    RSDK_EDITABLE_VAR(Hatch, VAR_ENUM, depth);
    RSDK_EDITABLE_VAR(Hatch, VAR_ENUM, dest);
}
