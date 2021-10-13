#include "SonicMania.h"

ObjectHatch *Hatch;

void Hatch_Update(void)
{
    RSDK_THIS(Hatch);

    int32 posX           = entity->position.x;
    int32 posY           = entity->position.y;
    entity->position.x = entity->startPos.x;
    entity->position.y = entity->startPos.y;
    if (entity->flag) {
        foreach_all(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                TileLayer *moveLayer = RSDK.GetSceneLayer(Zone->moveLayer);
                if (!player->sidekick) {
                    moveLayer->scrollPos               = -entity->vScrollPos;
                    moveLayer->scrollInfo[0].scrollPos = -entity->hScrollPos;
                }
                player->collisionLayers |= Zone->moveID;
                player->moveOffset.x = moveLayer->scrollInfo[0].scrollPos;
                player->moveOffset.y = moveLayer->scrollPos;
            }
        }
    }
    entity->position.x = posX;
    entity->position.y = posY;

    StateMachine_Run(entity->state);
}

void Hatch_LateUpdate(void) {}

void Hatch_StaticUpdate(void)
{
    foreach_all(Hatch, hatch) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(hatch)); }
}

void Hatch_Draw(void)
{
    RSDK_THIS(Hatch);
    if (RSDK_sceneInfo->currentDrawGroup == RSDK_sceneInfo->entity->drawOrder) {
        RSDK.DrawSprite(&entity->animator2, 0, false);
    }
    else {
        if (entity->animator2.animationID == 2 && entity->animator2.frameID >= 3)
            RSDK.DrawSprite(&entity->animator2, 0, false);
        RSDK.DrawSprite(&entity->animator1, 0, false);
    }
}

void Hatch_Create(void *data)
{
    RSDK_THIS(Hatch);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_BOUNDS;
        entity->drawOrder = Zone->drawOrderLow + 1;

        entity->updateRange.x = 16 * maxVal(abs(entity->subOff1.x), abs(entity->subOff2.x));
        entity->updateRange.y = 16 * maxVal(abs(entity->subOff1.y), abs(entity->subOff2.y));

        RSDK.SetSpriteAnimation(Hatch->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &entity->animator2, true, 4);

        entity->hitbox.left   = (entity->subOff1.x >> 12) - ((entity->position.x >> 16) & 0xF) + 16;
        entity->hitbox.top    = (entity->subOff1.y >> 12) - ((entity->position.y >> 16) & 0xF) + 16;
        entity->hitbox.right  = (entity->subOff2.x >> 12) - ((entity->position.x >> 16) & 0xF) + 16;
        entity->hitbox.bottom = (entity->subOff2.y >> 12) - ((entity->position.y >> 16) & 0xF) + 16;
        entity->startPos      = entity->position;

        EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, WarpDoor);
        if (warpDoor->objectID == WarpDoor->objectID) {
            entity->warpHitbox  = warpDoor->hitbox;
            warpDoor->hitbox.left   = 0;
            warpDoor->hitbox.top    = -0x800;
            warpDoor->hitbox.right  = 0;
            warpDoor->hitbox.bottom = -0x800;
        }

        switch (entity->go) {
            case 0:
                entity->visible = true;
                entity->state   = Hatch_Unknown1;
                entity->flag    = true;
                break;
            case 1:
                RSDK.CopyTileLayer(Zone->moveLayer, (entity->position.x >> 20) - 2, (entity->position.y >> 20) - 1, Zone->moveLayer, 1, 7, 4, 2);
                RSDK.CopyTileLayer(Zone->fgHigh, (entity->position.x >> 20) - 3, (entity->position.y >> 20) + 1, Zone->moveLayer, 16, 1, 6, 2);
                entity->visible = false;
                entity->flag    = false;
                entity->state   = Hatch_Unknown11;
                break;
            case 2:
                if (OOZSetup->flags == true) {
                    entity->visible = true;
                    entity->state   = Hatch_Unknown1;
                    entity->flag    = true;
                }
                else {
                    entity->vScrollPos += entity->depth << 16;
                    entity->position.y += entity->depth << 16;
                    destroyEntity(RSDK.GetEntityByID(RSDK_sceneInfo->entitySlot - 1));
                    entity->flag = true;
                }
                break;
            case 3:
                entity->visible = false;
                entity->state   = Hatch_Unknown9;
                entity->flag    = true;
                break;
            case 4:
                entity->visible = false;
                entity->flag    = false;
                entity->state   = Hatch_Unknown11;
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
    Hatch->sfxHatchOpen   = RSDK.GetSFX("OOZ/SubHatchOpen.wav");
    Hatch->sfxHatchClose  = RSDK.GetSFX("OOZ/SubHatchClose.wav");
    Hatch->sfxDescend     = RSDK.GetSFX("OOZ/SubDescend.wav");
    Hatch->sfxSurface     = RSDK.GetSFX("OOZ/SubSurface.wav");
    Hatch->sfxGasPop      = RSDK.GetSFX("OOZ/GasPop.wav");
}

void Hatch_Unknown1(void)
{
    RSDK_THIS(Hatch);
    RSDK.ProcessAnimation(&entity->animator2);

    int32 flag = 0;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &Hatch->hitbox3) == C_TOP) {
            flag = 1;
        }
        else if (Player_CheckCollisionBox(player, entity, &Hatch->hitbox4) == C_TOP) {
            flag = 1;
        }
        else if (Player_CheckCollisionBox(player, entity, &Hatch->hitbox5) == C_TOP) {
            if (player->onGround) {
                if (!player->sidekick) {
                    entity->active          = ACTIVE_NORMAL;
                    entity->playerPtr       = (Entity *)player;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->groundVel       = 0;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->interaction     = false;

                    if (player->playerAnimator.animationID != ANI_JUMP)
                        RSDK.PlaySfx(Player->sfx_Roll, false, 255);

                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->state = Player_State_None;
                    RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &entity->animator2, false, 0);
                    entity->state = Hatch_Unknown2;
                    flag          = 2;
                    foreach_break;
                }
            }
            else {
                flag = 1;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, entity, &Hatch->hitbox2))
                flag = 1;
        }
    }

    if (flag == 1) {
        if (entity->animator2.animationID == 2)
            RSDK.PlaySfx(Hatch->sfxHatchOpen, false, 255);
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 1, &entity->animator2, false, 0);
    }
    else if (!flag) {
        if (entity->animator2.animationID == 1)
            RSDK.PlaySfx(Hatch->sfxHatchClose, false, 255);
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &entity->animator2, false, 0);
    }
}

void Hatch_Unknown2(void)
{
    RSDK_THIS(Hatch);
    RSDK.ProcessAnimation(&entity->animator2);

    if (entity->animator2.frameID == 4) {
        EntityPlayer *player   = (EntityPlayer *)entity->playerPtr;
        entity->visible        = false;
        player->visible        = false;
        player->blinkTimer     = 0;
        player->tileCollisions = false;
        RSDK.CopyTileLayer(Zone->moveLayer, (entity->position.x >> 20) - 2, (entity->position.y >> 20) - 1, Zone->moveLayer, 1, 7, 4, 2);
        RSDK.CopyTileLayer(Zone->fgHigh, (entity->position.x >> 20) - 3, (entity->position.y >> 20) + 1, Zone->moveLayer, 16, 1, 6, 2);
        Zone->deathBoundary[0] = 0x7FFF0000;
        Zone->deathBoundary[1] = 0x7FFF0000;
        Zone->deathBoundary[2] = 0x7FFF0000;
        Zone->deathBoundary[3] = 0x7FFF0000;
        RSDK.PlaySfx(Hatch->sfxDescend, false, 255);
        entity->state    = Hatch_Unknown3;
        EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
        zone->fadeColour = 0;
        zone->timer      = 0;
        zone->drawOrder  = 15;
        zone->visible    = true;
        zone->stateDraw  = Zone_StateDraw_Fadeout;
    }
}

void Hatch_Unknown3(void)
{
    RSDK_THIS(Hatch);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;
    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    ++entity->timer;
    entity->vScrollPos += 0x10000;
    entity->position.y += 0x10000;
    player->position.y += 0x10000;

    zone->timer += 2;

    if (entity->timer >= entity->depth) {
        if (entity->dest) {
            entity->timer = 0;
            entity->state = Hatch_Unknown4;
        }
        else {
            player->state            = Player_State_Air;
            EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, WarpDoor);
            if (warpDoor->objectID == WarpDoor->objectID) {
                Zone->screenBoundsB1[RSDK.GetEntityID(player)] = 0x7FFF;
                warpDoor->hitbox                               = entity->warpHitbox;
                warpDoor->position.y                           = entity->position.y;
                player->tileCollisions                         = true;
                player->interaction                            = true;
                player->visible                                = 1;
            }
            RSDK.GetSceneLayer(Zone->moveLayer)->scrollPos = 0;
            entity->position.x                             = entity->startPos.x;
            entity->position.y                             = entity->startPos.y;
            if (entity->go != 2)
                entity->flag = false;
            entity->state = Hatch_Unknown8;
        }
    }
}

void Hatch_Unknown4(void)
{
    RSDK_THIS(Hatch);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    entity->hScrollPos += entity->dest << 16;
    entity->position.x += entity->dest << 16;
    player->position.x += entity->dest << 16;
    player->camera->position.x += entity->dest << 16;
    RSDK_screens[player->camera->screenID].position.x += entity->dest;
    entity->timer = 0;
    RSDK.PlaySfx(Hatch->sfxSurface, false, 255);
    entity->state = Hatch_Unknown5;
}

void Hatch_Unknown5(void)
{
    RSDK_THIS(Hatch);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;
    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    ++entity->timer;
    entity->vScrollPos -= 0x10000;
    entity->position.y -= 0x10000;
    zone->timer -= 2;
    player->position.y -= 0x10000;

    if (entity->timer >= entity->depth) {
        entity->timer      = 0;
        entity->visible    = true;
        entity->position.x = player->position.x;
        entity->position.y = player->position.y;
        entity->position.y += 0x80000;
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 1, &entity->animator2, false, 0);
        entity->state = Hatch_Unknown6;
    }
}

void Hatch_Unknown6(void)
{
    RSDK_THIS(Hatch);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;
    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    RSDK.ProcessAnimation(&entity->animator2);
    zone->timer -= 2;

    ++entity->timer;
    if (entity->timer == 30) {
        player->state          = Player_State_Air;
        player->velocity.y     = -0xA0000;
        player->onGround       = false;
        player->tileCollisions = true;
        player->interaction    = true;
        player->jumpAbility    = 0;
        player->visible        = true;
        RSDK.PlaySfx(Hatch->sfxGasPop, false, 255);
    }
    else if (entity->timer == 38) {
        RSDK.SetSpriteAnimation(Hatch->aniFrames, 2, &entity->animator2, false, 0);
        entity->state = Hatch_Unknown7;
    }
}

void Hatch_Unknown7(void)
{
    RSDK_THIS(Hatch);
    EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    RSDK.ProcessAnimation(&entity->animator2);
    zone->timer -= 2;
    if (entity->animator2.frameID >= entity->animator2.frameCount - 1) {
        entity->visible = false;
        entity->state   = Hatch_Unknown8;
    }
}

void Hatch_Unknown8(void)
{
    RSDK_THIS(Hatch);
    EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);

    zone->timer -= 8;
    if (zone->timer <= 0) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = StateMachine_None;
        zone->timer    = 0;
    }
}

void Hatch_Unknown9(void)
{
    RSDK_THIS(Hatch);

    foreach_all(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Hatch->hitbox1) && !player->sidekick) {
            entity->active           = ACTIVE_NORMAL;
            entity->playerPtr        = (Entity *)player;
            entity->stateStore       = player->state;
            entity->interactionStore = player->interaction;
            entity->tileColStore     = player->tileCollisions;
            player->velocity.x       = 0;
            player->groundVel        = 0;
            player->nextAirState     = StateMachine_None;
            player->nextGroundState  = StateMachine_None;
            player->interaction      = false;
            player->tileCollisions   = false;
            player->state            = Player_State_None;
            entity->state            = Hatch_Unknown10;
            foreach_break;
        }
    }
}

void Hatch_Unknown10(void)
{
    RSDK_THIS(Hatch);
    EntityZone *zone     = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    zone->timer += 8;
    if (zone->timer >= 512) {
        EntityWarpDoor *warpDoor = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, WarpDoor);

        if (warpDoor->objectID == WarpDoor->objectID) {
            Zone->screenBoundsB1[RSDK.GetEntityID(player)] = 0x7FFF;
            warpDoor->hitbox                               = entity->warpHitbox;
            warpDoor->position.y                           = player->position.y;
            player->state                                  = entity->stateStore;
            player->tileCollisions                         = entity->tileColStore;
            player->interaction                            = entity->interactionStore;
            player->visible                                = true;
        }
        if (entity->depth) {
            entity->state = Hatch_Unknown8;
        }
        else {
            entity->active = ACTIVE_BOUNDS;
            entity->state  = StateMachine_None;
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
            if (Player_CheckCollisionTouch(player, entity, &Hatch->hitbox1) && !player->sidekick) {
                entity->hScrollPos += entity->dest << 16;
                entity->vScrollPos += entity->depth << 16;
                entity->flag = true;
                if (entity->go == 4) {
                    entity->state = StateMachine_None;
                }
                else {
                    entity->playerPtr        = (Entity *)player;
                    entity->active           = ACTIVE_NORMAL;
                    entity->stateStore       = player->state;
                    entity->interactionStore = player->interaction;
                    entity->tileColStore     = player->tileCollisions;
                    entity->position.x += entity->dest << 16;
                    entity->velocity.y = player->velocity.y;
                    player->camera->position.x += entity->dest << 16;
                    player->camera->position.y += entity->depth << 16;
                    player->position.x += entity->dest << 16;
                    player->position.y += entity->depth << 16;
                    player->velocity.y      = 0;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->interaction     = false;
                    player->tileCollisions  = false;
                    player->state           = Player_State_None;
                    player->visible         = 0;
                    RSDK.PlaySfx(Hatch->sfxSurface, false, 255);
                    entity->state = Hatch_Unknown5;
                }
                foreach_break;
            }
        }
    }
}

void Hatch_EditorDraw(void) {}

void Hatch_EditorLoad(void) {}

void Hatch_Serialize(void)
{
    RSDK_EDITABLE_VAR(Hatch, VAR_UINT8, go);
    RSDK_EDITABLE_VAR(Hatch, VAR_VECTOR2, subOff1);
    RSDK_EDITABLE_VAR(Hatch, VAR_VECTOR2, subOff2);
    RSDK_EDITABLE_VAR(Hatch, VAR_ENUM, depth);
    RSDK_EDITABLE_VAR(Hatch, VAR_ENUM, dest);
}
