// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Hatch Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHatch *Hatch;

void Hatch_Update(void)
{
    RSDK_THIS(Hatch);

    int32 posX           = self->position.x;
    int32 posY           = self->position.y;
    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
    if (self->flag) {
        foreach_all(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                TileLayer *moveLayer = RSDK.GetSceneLayer(Zone->moveLayer);
                if (!player->sidekick) {
                    moveLayer->scrollPos               = -self->vScrollPos;
                    moveLayer->scrollInfo[0].scrollPos = -self->hScrollPos;
                }
                player->collisionLayers |= Zone->moveID;
                player->moveOffset.x = moveLayer->scrollInfo[0].scrollPos;
                player->moveOffset.y = moveLayer->scrollPos;
            }
        }
    }
    self->position.x = posX;
    self->position.y = posY;

    StateMachine_Run(self->state);
}

void Hatch_LateUpdate(void) {}

void Hatch_StaticUpdate(void)
{
    foreach_all(Hatch, hatch) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(hatch)); }
}

void Hatch_Draw(void)
{
    RSDK_THIS(Hatch);
    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        RSDK.DrawSprite(&self->animator2, NULL, false);
    }
    else {
        if (self->animator2.animationID == 2 && self->animator2.frameID >= 3)
            RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
}

void Hatch_Create(void *data)
{
    RSDK_THIS(Hatch);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_BOUNDS;
        self->drawOrder = Zone->drawOrderLow + 1;

        self->updateRange.x = 16 * maxVal(abs(self->subOff1.x), abs(self->subOff2.x));
        self->updateRange.y = 16 * maxVal(abs(self->subOff1.y), abs(self->subOff2.y));

        RSDK.SetSpriteAnimation(Hatch->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &self->animator2, true, 4);

        self->hitbox.left   = (self->subOff1.x >> 12) - ((self->position.x >> 16) & 0xF) + 16;
        self->hitbox.top    = (self->subOff1.y >> 12) - ((self->position.y >> 16) & 0xF) + 16;
        self->hitbox.right  = (self->subOff2.x >> 12) - ((self->position.x >> 16) & 0xF) + 16;
        self->hitbox.bottom = (self->subOff2.y >> 12) - ((self->position.y >> 16) & 0xF) + 16;
        self->startPos      = self->position;

        EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);
        if (warpDoor->objectID == WarpDoor->objectID) {
            self->warpHitbox        = warpDoor->hitbox;
            warpDoor->hitbox.left   = 0;
            warpDoor->hitbox.top    = 0x7FFF;
            warpDoor->hitbox.right  = 0;
            warpDoor->hitbox.bottom = 0x7FFF;
        }

        switch (self->go) {
            case 0:
                self->visible = true;
                self->state   = Hatch_Unknown1;
                self->flag    = true;
                break;
            case 1:
                RSDK.CopyTileLayer(Zone->moveLayer, (self->position.x >> 20) - 2, (self->position.y >> 20) - 1, Zone->moveLayer, 1, 7, 4, 2);
                RSDK.CopyTileLayer(Zone->fgHigh, (self->position.x >> 20) - 3, (self->position.y >> 20) + 1, Zone->moveLayer, 16, 1, 6, 2);
                self->visible = false;
                self->flag    = false;
                self->state   = Hatch_Unknown11;
                break;
            case 2:
                if (OOZSetup->flags == true) {
                    self->visible = true;
                    self->state   = Hatch_Unknown1;
                    self->flag    = true;
                }
                else {
                    self->vScrollPos += self->depth << 16;
                    self->position.y += self->depth << 16;
                    destroyEntity(RSDK.GetEntityByID(SceneInfo->entitySlot - 1));
                    self->flag = true;
                }
                break;
            case 3:
                self->visible = false;
                self->state   = Hatch_Unknown9;
                self->flag    = true;
                break;
            case 4:
                self->visible = false;
                self->flag    = false;
                self->state   = Hatch_Unknown11;
                break;
            default: break;
        }
    }
}

void Hatch_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        Hatch->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Hatch.bin", SCOPE_STAGE);
    Hatch->hitbox1.left   = -16;
    Hatch->hitbox1.top    = -16;
    Hatch->hitbox1.right  = 16;
    Hatch->hitbox1.bottom = 16;
    Hatch->hitbox2.left   = -128;
    Hatch->hitbox2.top    = -128;
    Hatch->hitbox2.right  = 128;
    Hatch->hitbox2.bottom = 128;
    Hatch->hitbox3.left   = -24;
    Hatch->hitbox3.top    = 0;
    Hatch->hitbox3.right  = -12;
    Hatch->hitbox3.bottom = 8;
    Hatch->hitbox4.left   = 12;
    Hatch->hitbox4.top    = 0;
    Hatch->hitbox4.right  = 24;
    Hatch->hitbox4.bottom = 8;
    Hatch->hitbox5.left   = -12;
    Hatch->hitbox5.top    = 8;
    Hatch->hitbox5.right  = 12;
    Hatch->hitbox5.bottom = 24;
    Hatch->active         = ACTIVE_ALWAYS;
    Hatch->sfxHatchOpen   = RSDK.GetSfx("OOZ/SubHatchOpen.wav");
    Hatch->sfxHatchClose  = RSDK.GetSfx("OOZ/SubHatchClose.wav");
    Hatch->sfxDescend     = RSDK.GetSfx("OOZ/SubDescend.wav");
    Hatch->sfxSurface     = RSDK.GetSfx("OOZ/SubSurface.wav");
    Hatch->sfxGasPop      = RSDK.GetSfx("OOZ/GasPop.wav");
}

void Hatch_Unknown1(void)
{
    RSDK_THIS(Hatch);
    RSDK.ProcessAnimation(&self->animator2);

    int32 flag = 0;
    foreach_all(Player, player)
    {
        // Bug Details:
        // this does foreach_all, instead of foreach_active
        // meaning that even killed players are included
        // Fix:
        // this entire block of code should have a Player_CheckValidState call to make sure the player isn't dead or some other "invalid" state

        // Extra notes: if you manage to die and fall into the hatch, you'll be brough back sorta (this is because it changes your state from Player_State_Death to Player_State_None)
        // though the death state stuff will still be applied so you'll be on the highest layer (until its changed) and the player->active var will be set to ACTIVE_ALWAYS
        // this means you can do really weird stuff such as move during the pause menu

        if (Player_CheckCollisionBox(player, self, &Hatch->hitbox3) == C_TOP) {
            flag = 1;
        }
        else if (Player_CheckCollisionBox(player, self, &Hatch->hitbox4) == C_TOP) {
            flag = 1;
        }
        else if (Player_CheckCollisionBox(player, self, &Hatch->hitbox5) == C_TOP) {
            if (player->onGround) {
                if (!player->sidekick) {
                    self->active            = ACTIVE_NORMAL;
                    self->playerPtr         = (Entity *)player;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->groundVel       = 0;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->interaction     = false;

                    if (player->animator.animationID != ANI_JUMP)
                        RSDK.PlaySfx(Player->sfxRoll, false, 255);

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->state = Player_State_None;
                    RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &self->animator2, false, 0);
                    self->state = Hatch_Unknown2;
                    flag        = 2;
                    foreach_break;
                }
            }
            else {
                flag = 1;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &Hatch->hitbox2))
                flag = 1;
        }
    }

    if (flag == 1) {
        if (self->animator2.animationID == 2)
            RSDK.PlaySfx(Hatch->sfxHatchOpen, false, 255);
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 1, &self->animator2, false, 0);
    }
    else if (!flag) {
        if (self->animator2.animationID == 1)
            RSDK.PlaySfx(Hatch->sfxHatchClose, false, 255);
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &self->animator2, false, 0);
    }
}

void Hatch_Unknown2(void)
{
    RSDK_THIS(Hatch);
    RSDK.ProcessAnimation(&self->animator2);

    if (self->animator2.frameID == 4) {
        EntityPlayer *player   = (EntityPlayer *)self->playerPtr;
        self->visible        = false;
        player->visible        = false;
        player->blinkTimer     = 0;
        player->tileCollisions = false;
        RSDK.CopyTileLayer(Zone->moveLayer, (self->position.x >> 20) - 2, (self->position.y >> 20) - 1, Zone->moveLayer, 1, 7, 4, 2);
        RSDK.CopyTileLayer(Zone->fgHigh, (self->position.x >> 20) - 3, (self->position.y >> 20) + 1, Zone->moveLayer, 16, 1, 6, 2);
        Zone->deathBoundary[0] = 0x7FFF0000;
        Zone->deathBoundary[1] = 0x7FFF0000;
        Zone->deathBoundary[2] = 0x7FFF0000;
        Zone->deathBoundary[3] = 0x7FFF0000;
        RSDK.PlaySfx(Hatch->sfxDescend, false, 255);
        self->state    = Hatch_Unknown3;
        EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
        zone->fadeColour = 0;
        zone->timer      = 0;
        zone->drawOrder  = 15;
        zone->visible    = true;
        zone->stateDraw  = Zone_Draw_Fade;
    }
}

void Hatch_Unknown3(void)
{
    RSDK_THIS(Hatch);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;
    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    ++self->timer;
    self->vScrollPos += 0x10000;
    self->position.y += 0x10000;
    player->position.y += 0x10000;

    zone->timer += 2;

    if (self->timer >= self->depth) {
        if (self->dest) {
            self->timer = 0;
            self->state = Hatch_Unknown4;
        }
        else {
            player->state            = Player_State_Air;
            EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);
            if (warpDoor->objectID == WarpDoor->objectID) {
                Zone->cameraBoundsB[RSDK.GetEntityID(player)] = 0x7FFF;
                warpDoor->hitbox                               = self->warpHitbox;
                warpDoor->position.y                           = self->position.y;
                player->tileCollisions                         = true;
                player->interaction                            = true;
                player->visible                                = 1;
            }
            RSDK.GetSceneLayer(Zone->moveLayer)->scrollPos = 0;
            self->position.x                             = self->startPos.x;
            self->position.y                             = self->startPos.y;
            if (self->go != 2)
                self->flag = false;
            self->state = Hatch_Unknown8;
        }
    }
}

void Hatch_Unknown4(void)
{
    RSDK_THIS(Hatch);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    self->hScrollPos += self->dest << 16;
    self->position.x += self->dest << 16;
    player->position.x += self->dest << 16;
    player->camera->position.x += self->dest << 16;
    ScreenInfo[player->camera->screenID].position.x += self->dest;
    self->timer = 0;
    RSDK.PlaySfx(Hatch->sfxSurface, false, 255);
    self->state = Hatch_Unknown5;
}

void Hatch_Unknown5(void)
{
    RSDK_THIS(Hatch);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;
    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    ++self->timer;
    self->vScrollPos -= 0x10000;
    self->position.y -= 0x10000;
    zone->timer -= 2;
    player->position.y -= 0x10000;

    if (self->timer >= self->depth) {
        self->timer      = 0;
        self->visible    = true;
        self->position.x = player->position.x;
        self->position.y = player->position.y;
        self->position.y += 0x80000;
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 1, &self->animator2, false, 0);
        self->state = Hatch_Unknown6;
    }
}

void Hatch_Unknown6(void)
{
    RSDK_THIS(Hatch);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;
    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    RSDK.ProcessAnimation(&self->animator2);
    zone->timer -= 2;

    ++self->timer;
    if (self->timer == 30) {
        player->state          = Player_State_Air;
        player->velocity.y     = -0xA0000;
        player->onGround       = false;
        player->tileCollisions = true;
        player->interaction    = true;
        player->jumpAbility    = 0;
        player->visible        = true;
        RSDK.PlaySfx(Hatch->sfxGasPop, false, 255);
    }
    else if (self->timer == 38) {
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &self->animator2, false, 0);
        self->state = Hatch_Unknown7;
    }
}

void Hatch_Unknown7(void)
{
    RSDK_THIS(Hatch);
    EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    RSDK.ProcessAnimation(&self->animator2);
    zone->timer -= 2;
    if (self->animator2.frameID >= self->animator2.frameCount - 1) {
        self->visible = false;
        self->state   = Hatch_Unknown8;
    }
}

void Hatch_Unknown8(void)
{
    RSDK_THIS(Hatch);
    EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    zone->timer -= 8;
    if (zone->timer <= 0) {
        self->active = ACTIVE_BOUNDS;
        self->state  = StateMachine_None;
        zone->timer    = 0;
    }
}

void Hatch_Unknown9(void)
{
    RSDK_THIS(Hatch);

    foreach_all(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Hatch->hitbox1) && !player->sidekick) {
            self->active           = ACTIVE_NORMAL;
            self->playerPtr        = (Entity *)player;
            self->stateStore       = player->state;
            self->interactionStore = player->interaction;
            self->tileColStore     = player->tileCollisions;
            player->velocity.x       = 0;
            player->groundVel        = 0;
            player->nextAirState     = StateMachine_None;
            player->nextGroundState  = StateMachine_None;
            player->interaction      = false;
            player->tileCollisions   = false;
            player->state            = Player_State_None;
            self->state            = Hatch_Unknown10;
            foreach_break;
        }
    }
}

void Hatch_Unknown10(void)
{
    RSDK_THIS(Hatch);
    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    zone->timer += 8;
    if (zone->timer >= 512) {
        EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, WarpDoor);

        if (warpDoor->objectID == WarpDoor->objectID) {
            Zone->cameraBoundsB[RSDK.GetEntityID(player)] = 0x7FFF;
            warpDoor->hitbox                               = self->warpHitbox;
            warpDoor->position.y                           = player->position.y;
            player->state                                  = self->stateStore;
            player->tileCollisions                         = self->tileColStore;
            player->interaction                            = self->interactionStore;
            player->visible                                = true;
        }
        if (self->depth) {
            self->state = Hatch_Unknown8;
        }
        else {
            self->active = ACTIVE_BOUNDS;
            self->state  = StateMachine_None;
        }
    }
}

void Hatch_Unknown11(void)
{
    RSDK_THIS(Hatch);
    EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    if (zone->timer) {
        foreach_all(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Hatch->hitbox1) && !player->sidekick) {
                self->hScrollPos += self->dest << 16;
                self->vScrollPos += self->depth << 16;
                self->flag = true;
                if (self->go == 4) {
                    self->state = StateMachine_None;
                }
                else {
                    self->playerPtr        = (Entity *)player;
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
                    player->tileCollisions  = false;
                    player->state           = Player_State_None;
                    player->visible         = 0;
                    RSDK.PlaySfx(Hatch->sfxSurface, false, 255);
                    self->state = Hatch_Unknown5;
                }
                foreach_break;
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Hatch_EditorDraw(void)
{
    RSDK_THIS(Hatch);

    self->drawOrder = Zone->drawOrderLow + 1;

    self->updateRange.x = 16 * maxVal(abs(self->subOff1.x), abs(self->subOff2.x));
    self->updateRange.y = 16 * maxVal(abs(self->subOff1.y), abs(self->subOff2.y));

    RSDK.SetSpriteAnimation(Hatch->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &self->animator2, false, 4);

    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Hatch_EditorLoad(void) { Hatch->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Hatch.bin", SCOPE_STAGE); }
#endif

void Hatch_Serialize(void)
{
    RSDK_EDITABLE_VAR(Hatch, VAR_UINT8, go);
    RSDK_EDITABLE_VAR(Hatch, VAR_VECTOR2, subOff1);
    RSDK_EDITABLE_VAR(Hatch, VAR_VECTOR2, subOff2);
    RSDK_EDITABLE_VAR(Hatch, VAR_ENUM, depth);
    RSDK_EDITABLE_VAR(Hatch, VAR_ENUM, dest);
}
