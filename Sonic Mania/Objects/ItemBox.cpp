#include "../SonicMania.hpp"

ObjectItemBox *ItemBox;

void ItemBox_Update()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;

    if (entity->state)
        entity->state();

    if (entity->type == 17) {
        if (entity->contentsData.animationID == 2 || (uint)(entity->contentsData.animationID - 7) <= 1) {
#if RETRO_USE_PLUS
            if (options->characterFlags != 0x1F || options->gameMode != MODE_ENCORE) {
#endif
                RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 7, &entity->contentsData, 0, 0);
#if RETRO_USE_PLUS
                if (options->gameMode == MODE_ENCORE) {
                    int id = 0;
                    while ((1 << entity->contentsData.frameID) & options->characterFlags) {
                        if (++entity->contentsData.frameID > 4)
                            entity->contentsData.frameID = 0;
                        if (++id > 5) {
                            RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 8, &entity->contentsData, 0, 0);
                            RSDK.PrintLog(SEVERITY_NONE, "Bad Change Item State");
                        }
                    }
                }
#endif
#if RETRO_USE_PLUS
            }
            else {
                RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 8, &entity->contentsData, 0, 0);
            }
#endif
        }
    }
}

void ItemBox_LateUpdate() {}

void ItemBox_StaticUpdate()
{
    if (!(Zone->timer & 3)) {
        RSDK.RotatePalette(0, 60, 63, true);
        RSDK.RotatePalette(1, 60, 63, true);
    }
}

void ItemBox_Draw()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    if (!entity->hidden) {
        if (entity->isContents == 1) {
            if (RSDK_sceneInfo->currentDrawGroup == Zone->uiDrawLow) {
                entity->drawFX = FX_NONE;
                RSDK.DrawSprite(&entity->contentsData, &entity->contentsPos, 0);
            }
            else {
                entity->drawFX    = FX_FLIP;
                entity->inkEffect = INK_NONE;
                RSDK.DrawSprite(&entity->brokenData, 0, 0);
                RSDK.AddDrawListRef(Zone->uiDrawLow, RSDK_sceneInfo->entitySlot);
            }
        }
        else {
            entity->inkEffect = 0;
            RSDK.DrawSprite(&entity->brokenData, 0, 0);
            RSDK.DrawSprite(&entity->contentsData, &entity->contentsPos, 0);
            entity->inkEffect = INK_ADD;
            RSDK.DrawSprite(&entity->overlayData, 0, 0);
            entity->inkEffect = INK_NONE;
            RSDK.DrawSprite(&entity->debrisData, 0, 0);
        }
    }
}

void ItemBox_Create(void *data)
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    if (data)
        entity->type = (int)(size_t)data;
    if (entity->state != ItemBox_State_Broken) {
        RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 0, &entity->brokenData, true, 0);
        RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 2, &entity->contentsData, true, 0);
        RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 3, &entity->overlayData, true, 0);
        RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 4, &entity->debrisData, true, 0);

        EntityPlayer *player = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
        switch (entity->type) {
            case 7:
            case 8:
            case 9:
            case 0xF:
            case 0x10:
                if (options->gameMode == MODE_TIMEATTACK) {
                    entity->type = 0;
                }
#if RETRO_USE_PLUS
                else if (options->gameMode == MODE_ENCORE) {
                    entity->type = 17;
                }
#endif
                else {
                    switch (player->characterID) {
                        case ID_SONIC: entity->type = 7; break;
                        case ID_TAILS: entity->type = 8; break;
                        case ID_KNUCKLES: entity->type = 9; break;
#if RETRO_USE_PLUS
                        case ID_MIGHTY: entity->type = 15; break;
                        case ID_RAY: entity->type = 16; break;
#endif
                        default: break;
                    }
                }
                entity->contentsData.frameID = entity->type;
                break;
            case 0xC:
            case 0xD:
#if RETRO_USE_PLUS
                if (options->gameMode == MODE_ENCORE || options->gameMode == MODE_COMPETITION)
#else
                if (options->gameMode == MODE_COMPETITION)
#endif
                    entity->contentsData.frameID = entity->type;
                else
                    RSDK.DestroyEntity(entity, 0, 0);
                return;
            default: entity->contentsData.frameID = entity->type; break;
        }
    }

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->direction *= FLIP_Y;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->visible       = 1;
        if (entity->planeFilter > 0 && ((byte)entity->planeFilter - 1) & 2)
            entity->drawOrder = Zone->drawOrderHigh;
        else
            entity->drawOrder = Zone->drawOrderLow;

        entity->alpha = 0xFF;
        if (entity->state == ItemBox_State_Broken) {
            RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 1, &entity->brokenData, true, entity->brokenData.frameID);
        }
        else if (entity->hidden) {
            entity->state = NULL;
        }
        else if (entity->isFalling) {
            entity->active = ACTIVE_NORMAL;
            entity->state  = ItemBox_State_Falling;
        }
        else if (LRZConvItem && entity->lrzConvPhys) {
            entity->active = ACTIVE_NORMAL;
            entity->state  = ItemBox_State_Conveyor;
        }
        else {
            entity->state = ItemBox_State_Normal;
        }
    }
}

void ItemBox_StageLoad()
{
    ItemBox->spriteIndex         = RSDK.LoadAnimation("Global/ItemBox.bin", SCOPE_STAGE);
    ItemBox->hitbox.left         = -15;
    ItemBox->hitbox.top          = -16;
    ItemBox->hitbox.right        = 15;
    ItemBox->hitbox.bottom       = 16;
    ItemBox->hiddenHitbox.left   = -15;
    ItemBox->hiddenHitbox.top    = -24;
    ItemBox->hiddenHitbox.right  = 15;
    ItemBox->hiddenHitbox.bottom = 16;

    if (DebugMode->itemCount < 256) {
        DebugMode->objectIDs[DebugMode->itemCount] = ItemBox->objectID;
        DebugMode->spawn[DebugMode->itemCount]     = ItemBox_DebugSpawn;
        DebugMode->draw[DebugMode->itemCount++]    = ItemBox_DebugDraw;
    }

    if (options->gameMode == MODE_COMPETITION) {
        if (options->itemMode == 1) {
            for (EntityItemBox *entity = 0; RSDK.GetObjects(ItemBox->objectID, (Entity **)&entity); entity->type = 13) {
            }
        }
        else if (options->itemMode == 2) {
            for (EntityItemBox *entity = 0; RSDK.GetObjects(ItemBox->objectID, (Entity **)&entity); entity->type = 12) {
            }
        }
    }

    ItemBox->sfx_Destroy   = RSDK.GetSFX("Global/Destroy.wav");
    ItemBox->sfx_Teleport  = RSDK.GetSFX("Global/Teleport.wav");
    ItemBox->sfx_HyperRing = RSDK.GetSFX("Global/HyperRing.wav");
    ItemBox->sfx_PowerDown = RSDK.GetSFX("Stage/PowerDown.wav");
    ItemBox->sfx_Revovery  = RSDK.GetSFX("Global/Recovery.wav");
}

void ItemBox_DebugDraw()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
#if RETRO_USE_PLUS
    DebugMode->subtypeCount = User.CheckDLC(DLC_PLUS) != 0 ? 18 : 15;
#else
    DebugMode->subtypeCount = 15;
#endif
    RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 0, &DebugMode->debugData, true, 0);
    RSDK.DrawSprite(&DebugMode->debugData, 0, 0);
    RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 2, &DebugMode->debugData, true, DebugMode->itemSubType);
    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y - 0x30000;
    RSDK.DrawSprite(&DebugMode->debugData, &drawPos, 0);
}
void ItemBox_DebugSpawn()
{
    EntityItemBox *entity =
        (EntityItemBox *)RSDK.SpawnEntity(ItemBox->objectID, 0, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y);
    entity->type                 = DebugMode->itemSubType;
    entity->contentsData.frameID = DebugMode->itemSubType;
}

void ItemBox_State_Broken()
{
    //EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    // if (LRZConvItem)
    //    LRZConvItem_Unknown2(entity);
    ItemBox_HandleFallingCollision();
}
void ItemBox_State_Contents()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    if (LRZConvItem && entity->lrzConvPhys) {
        // LRZConvItem_Unknown2(entity);
    }
    else {
        ItemBox_HandleFallingCollision();
    }

    if (entity->contentsSpeed < 0) {
        entity->contentsPos.y += entity->contentsSpeed;
        entity->contentsSpeed += 0x1800;
    }
    if (entity->contentsSpeed >= 0) {
        entity->contentsSpeed = 0;
        ItemBox_GivePowerup();
        RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 5, &entity->contentsData, true, 0);
        entity->state = ItemBox_State_Unknown;
    }
}

void ItemBox_State_Unknown()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    if (LRZConvItem && entity->lrzConvPhys) {
        // LRZConvItem_Unknown2(entity);
    }
    else {
        ItemBox_HandleFallingCollision();
    }
    RSDK.ProcessAnimation(&entity->contentsData);

    if (entity->contentsData.frameID == entity->contentsData.frameCount - 1) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->contentsData, true, 0);
        entity->state = ItemBox_State_Broken;
    }
}

void ItemBox_State_Normal()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    entity->contentsPos.x = RSDK_sceneInfo->entity->position.x;

    if (entity->direction == FLIP_NONE)
        entity->contentsPos.y = entity->position.y - 0x30000;
    else
        entity->contentsPos.y = entity->position.y + 0x30000;
    ItemBox_HandleObjectCollisions();
    ItemBox_CheckHit();
    RSDK.ProcessAnimation(&entity->overlayData);
    if (entity->type == 17) {
        RSDK.ProcessAnimation(&entity->contentsData);
#if RETRO_USE_PLUS
        if (!User.CheckDLC(DLC_PLUS) && entity->contentsData.frameID >= 3)
            entity->contentsData.frameID = 0;
#endif
    }

    if (entity->timer) {
        entity->timer--;
    }
    else {
        RSDK.ProcessAnimation(&entity->debrisData);
        if (!entity->debrisData.frameID) {
            entity->timer                 = RSDK.Rand(1, 15);
            entity->debrisData.frameDelay = RSDK.Rand(1, 32);
        }
    }
}
void ItemBox_State_Falling()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    if (ItemBox_HandleFallingCollision())
        entity->state = ItemBox_State_Normal;

    entity->contentsPos.x = entity->position.x;
    if (entity->direction == FLIP_NONE)
        entity->contentsPos.y = entity->position.y - 0x30000;
    else
        entity->contentsPos.y = entity->position.y + 0x30000;

    ItemBox_CheckHit();
    RSDK.ProcessAnimation(&entity->overlayData);
    if (entity->type == 17) {
        RSDK.ProcessAnimation(&entity->contentsData);
#if RETRO_USE_PLUS
        if (!User.CheckDLC(DLC_PLUS) && entity->contentsData.frameID >= 3)
            entity->contentsData.frameID = 0;
#endif
    }

    if (entity->timer) {
        entity->timer--;
    }
    else {
        RSDK.ProcessAnimation(&entity->debrisData);
        if (!entity->debrisData.frameID) {
            entity->timer                 = RSDK.Rand(1, 15);
            entity->debrisData.frameDelay = RSDK.Rand(1, 32);
        }
    }
}
void ItemBox_State_Conveyor()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;

    // entity->contentsPos.x = LRZConvItem_Unknown2(entity);
    if (entity->direction == FLIP_NONE)
        entity->contentsPos.y = entity->position.y - 0x30000;
    else
        entity->contentsPos.y = entity->position.y + 0x30000;

    ItemBox_CheckHit();
    RSDK.ProcessAnimation(&entity->overlayData);
    if (entity->type == 17) {
        RSDK.ProcessAnimation(&entity->contentsData);
#if RETRO_USE_PLUS
        if (!User.CheckDLC(DLC_PLUS) && entity->contentsData.frameID >= 3)
            entity->contentsData.frameID = 0;
#endif
    }

    if (entity->timer) {
        entity->timer--;
    }
    else {
        RSDK.ProcessAnimation(&entity->debrisData);
        if (!entity->debrisData.frameID) {
            entity->timer                 = RSDK.Rand(1, 15);
            entity->debrisData.frameDelay = RSDK.Rand(1, 32);
        }
    }
}

void ItemBox_CheckHit()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    EntityPlayer *player  = 0;
    while (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
        if (entity->planeFilter <= 0 || player->collisionPlane == (((byte)entity->planeFilter - 1) & 1)) {
#if RETRO_USE_PLUS
            if (player->characterID == ID_MIGHTY && player->jumpAbilityTimer > 1 && !entity->parent) {
                if (RSDK.CheckObjectCollisionTouchCircle(player, 0x1000000, entity, 0x100000)) {
                    if (entity->position.y - 0x800000 < player->position.y && entity->state != ItemBox_State_Falling) {
                        entity->active = ACTIVE_NORMAL;
                        if (!entity->lrzConvPhys)
                            entity->state = ItemBox_State_Falling;
                        entity->velocity.y = -0x20000;
                    }
                }
            }
#endif

            if (player->sidekick) {
                entity->position.x -= entity->unknownPos.x;
                entity->position.y -= entity->unknownPos.y;
                int px             = player->position.x;
                int py             = player->position.y;
                byte side          = Player_CheckCollisionBox(player, entity, &ItemBox->hitbox);
                player->position.x = px;
                player->position.y = py;
                entity->position.x += entity->unknownPos.x;
                entity->position.y += entity->unknownPos.y;

                if (side == 4) {
                    entity->active = ACTIVE_ALWAYS;
                    if (!entity->lrzConvPhys)
                        entity->state = ItemBox_State_Falling;
                    entity->velocity.y = -0x20000;
                    if (player->onGround == false)
                        player->velocity.y = 0x20000;
                }
                else if (side == 1) {
                    player->position.x += entity->unknownPos.x;
                    player->position.y += entity->unknownPos.y;
                }

                if (Player_CheckCollisionBox(player, entity, &ItemBox->hitbox) == 4) {
                    if (player->onGround) {
                        player->position.x = px;
                        player->position.y = py;
                    }
                }
            }
            else {
                int anim    = player->playerAnimData.animationID;
                bool32 flag = anim == ANI_JUMP
                              && (player->velocity.y >= 0 || player->onGround || entity->direction); //|| player->state == Ice_State_FrozenPlayer);
                switch (player->characterID) {
                    case ID_SONIC: flag |= anim == ANI_DROPDASH; break;
                    case ID_KNUCKLES: flag |= anim == ANI_FLY || anim == ANI_FLYLIFTTIRED; break;
#if RETRO_USE_PLUS
                    case ID_MIGHTY: flag |= anim == ANI_DROPDASH || player->jumpAbilityTimer > 1; break;
#endif
                }
                if (!flag) {
                    entity->position.x -= entity->unknownPos.x;
                    entity->position.y -= entity->unknownPos.y;
                    int px             = player->position.x;
                    int py             = player->position.y;
                    byte side          = Player_CheckCollisionBox(player, entity, &ItemBox->hitbox);
                    player->position.x = px;
                    player->position.y = py;
                    entity->position.x += entity->unknownPos.x;
                    entity->position.y += entity->unknownPos.y;

                    if (side == 4) {
                        entity->active = ACTIVE_ALWAYS;
                        if (!entity->lrzConvPhys)
                            entity->state = ItemBox_State_Falling;
                        entity->velocity.y = -0x20000;
                        if (player->onGround == false)
                            player->velocity.y = 0x20000;
                    }
                    else if (side == 1) {
                        player->position.x += entity->unknownPos.x;
                        player->position.y += entity->unknownPos.y;
                    }

                    if (Player_CheckCollisionBox(player, entity, &ItemBox->hitbox) == 4) {
                        if (player->onGround) {
                            player->position.x = px;
                            player->position.y = py;
                        }
                    }
                }
                else if (Player_CheckBadnikHit(player, entity, &ItemBox->hitbox)) {
                    ItemBox_Break(entity, player);
                    RSDK.NextForEachLoop();
                }
            }
        }
    }
}
void ItemBox_GivePowerup()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    EntityPlayer *player  = (EntityPlayer *)entity->storedEntity;
    while (true) {
        switch (entity->type) {
            case 0: Player_GiveRings(10, player, 1); return;
            case 1:
                player->shield = SHIELD_BLUE;
                Player_ApplyShieldEffect(player);
                RSDK.PlaySFX(Shield->sfx_BlueShield, 0, 255);
                return;
            case 2:
                player->shield = SHIELD_BUBBLE;
                Player_ApplyShieldEffect(player);
                RSDK.PlaySFX(Shield->sfx_BubbleShield, 0, 255);
                player->airTimer = 0;
                // Music_Unknown6(8, 0);
                return;
            case 3:
                player->shield = SHIELD_FIRE;
                Player_ApplyShieldEffect(player);
                RSDK.PlaySFX(Shield->sfx_FireShield, 0, 255);
                return;
            case 4:
                player->shield = SHIELD_LIGHTNING;
                Player_ApplyShieldEffect(player);
                RSDK.PlaySFX(Shield->sfx_LightningShield, 0, 255);
                return;
            case 5:
                if (!player->superState) {
                    Entity *shield = (Entity *)RSDK.GetEntityByID((ushort)(Player->playerCount + RSDK.GetEntityID(player)));
                    RSDK.DestroyEntity(shield, InvincibleStars->objectID, player);
                    player->invincibleTimer = 1260;
                    // Music_PlayMusicTrack(1);
                }
                return;
            case 6:
                player->speedShoesTimer = 1320;
                Player_ChangePhysicsState(player);
                if (!player->superState) {
                    // Music_PlayMusicTrack(2);
                    Entity *powerup = (Entity *)RSDK.GetEntityByID((ushort)(2 * Player->playerCount + RSDK.GetEntityID(player)));
                    RSDK.DestroyEntity(powerup, ImageTrail->objectID, player);
                }
                return;
            case 7:
            case 8:
            case 9:
            case 15:
            case 16: Player_GiveLife(player); return;
            case 10: Player_CheckHit(player, entity); return;
            case 11:
                RSDK.PlaySFX(ItemBox->sfx_HyperRing, 0, 255);
                player->hyperRing = true;
                return;
            case 12:
#if RETRO_USE_PLUS
                if (options->gameMode == MODE_ENCORE) {
                    if (!options->stock || player->playerAnimData.animationID == ANI_TRANSFORM) {
                        RSDK.PlaySFX(Player->sfx_SwapFail, 0, 255);
                        return;
                    }
                    int charID = player->characterID;
                    Player_ChangeCharacter(player, (byte)options->stock);
                    options->stock >>= 8;

                    if (options->stock) {
                        charID <<= 8;
                        if (options->stock & 0xFF00)
                            charID <<= 8;
                    }
                    options->stock |= charID;
                    EntityExplosion *explosion =
                        (EntityExplosion *)RSDK.SpawnEntity(Explosion->objectID, (void *)1, player->position.x, player->position.y);
                    explosion->drawOrder = Zone->drawOrderHigh;
                    RSDK.PlaySFX(ItemBox->sfx_PowerDown, 0, 255);
                    return;
                }
#endif
                if (options->gameMode != MODE_COMPETITION) {
                    RSDK.PlaySFX(Player->sfx_SwapFail, 0, 255);
                    return;
                }
                Zone_Unknown5();
                RSDK.PlaySFX(ItemBox->sfx_Teleport, 0, 255);
                return;
            case 13: {
                /*v23         = 0;
                int arr1[5] = { 0x0FF00000, 0x0FF00000, 0x0FF00000, 0x0FF00000 };
                arr1[4]     = 0xFF;
                if (player->playerAnimData.animationID == ANI_TRANSFORM) {
                    RSDK.PlaySFX(Player->sfx_SwapFail, 0, 255);
                    return;
                }
                EntityPlayer *player1 = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
                int id                = -1;
                for (int i = player1->characterID; i > 0; ++id) i >>= 1;
                arr1[0]               = id;
                EntityPlayer *player2 = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER2);
                id                    = -1;
                for (int i = player2->characterID; i > 0; ++id) i >>= 1;
                arr1[1] = id;
                if (id == -1) {
                    RSDK.PlaySFX(Player->sfx_SwapFail, 0, 255);
                    return;
                }
                v31   = 2;
                if (options->stock & 0xFF0000) {
                    id = -1;
                    int cnt = BYTE2(stock);
                    if (cnt) {
                        do {
                            cnt >>= 1;
                            ++id;
                        } while (cnt > 0);
                    }
                    LOWORD(stock) = options->stock;
                    v31           = 3;
                    arr1[2]       = id;
                }
                if (options->stock & 0xFF00) {
                    id = -1;
                    int cnt = BYTE1(stock);
                    if (cnt) {
                        do {
                            cnt >>= 1;
                            ++id;
                        } while (cnt > 0);
                    }
                    LOBYTE(stock) = v52;
                    arr1[v31++]   = id;
                }

                stock = (unsigned __int8)stock;
                if ((_BYTE)stock) {
                    v37 = -1;
                    if (stock) {
                        do {
                            stock >>= 1;
                            ++v37;
                        } while (stock > 0);
                    }
                    arr1[v31] = v37;
                }
                options->stock = 0;
                v38            = 0;
                do {
                    if (arr1[v38] == 255)
                        break;
                    v39      = RSDK.Rand(0, 5);
                    v53[v38] = v39;
                    if ((1 << v39) & options->characterFlags) // charFlags
                    {
                        while (1) {
                            v40 = v53[v38];
                            if (!((1 << v40) & v23)) {
                                if (v40 != arr1[v38])
                                    goto LABEL_94;
                                if (v38 == 4 || arr1[v38 + 1] == 255)
                                    break;
                            }
                            v41      = RSDK_Rand(0, 5);
                            v53[v38] = v41;
                            if (!((1 << v41) & options->characterFlags))
                                goto LABEL_95;
                        }
                        v42      = RSDK_Rand(0, v38);
                        v43      = v53[v42];
                        v44      = &v53[v42];
                        *v44     = v53[v38];
                        v53[v38] = v43;
                        v23 |= 1 << *(_BYTE *)v44;
                    LABEL_94:
                        v23 |= 1 << LOBYTE(v53[v38++]);
                    }
                LABEL_95:;
                } while (v38 < 5);
                v45 = 0;
                if (v38 > 0) {
                    while (v45) {
                        if (v45 == 1) {
                            v46 = (EntityPlayer *)RSDK_GetEntityByID(1);
                            v47 = v53[1];
                        LABEL_102:
                            Player_ChangeCharacter(v46, 1 << v47);
                            goto LABEL_103;
                        }
                        options->stock <<= 8;
                        options->stock |= 1 << LOBYTE(v53[v45]);
                    LABEL_103:
                        if (++v45 >= v38)
                            goto LABEL_104;
                    }
                    v46 = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
                    v47 = v53[0];
                    goto LABEL_102;
                }
            LABEL_104:*/
                EntityPlayer *player1 = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
                EntityExplosion *explosion =
                    (EntityExplosion *)RSDK.SpawnEntity(Explosion->objectID, (void *)1, player1->position.x, player1->position.y);
                explosion->drawOrder = Zone->drawOrderHigh;

                EntityPlayer *player2 = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER2);
                explosion             = (EntityExplosion *)RSDK.SpawnEntity(Explosion->objectID, (void *)1, player2->position.x, player2->position.y);
                explosion->drawOrder  = Zone->drawOrderHigh;

                RSDK.PlaySFX(ItemBox->sfx_PowerDown, 0, 255);
                return;
            }
            case 14:
                Player_GiveRings(50, player, 0);
                Player_CheckGoSuper(player, 127);
                return;
            case 17: {
                if (entity->contentsData.animationID == 7) {
#if RETRO_USE_PLUS
                    if (options->gameMode == MODE_ENCORE) {
                        if (!((1 << entity->contentsData.frameID) & options->characterFlags) && options->characterFlags != 31
                            && !(options->stock & 0xFF0000)) {
                            options->characterFlags |= (1 << entity->contentsData.frameID);
                            EntityPlayer *player2 = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER2);
                            if (player2->objectID) {
                                int id = 0;
                                if (options->stock) {
                                    do
                                        id += 8;
                                    while (options->stock >> id);
                                }
                                options->stock |= (1 << entity->contentsData.frameID << id);
                                //*(&HUD->field_2C + (v14 >> 3)) = 120;
                            }
                            else {
                                player2->objectID = Player->objectID;
                                Player->field_9E4 = 0;
                                EntityDust *dust =
                                    (EntityDust *)RSDK.SpawnEntity(Dust->objectID, (void *)1, player2->position.x, player2->position.y);

                                dust->visible         = 0;
                                dust->active          = ACTIVE_NEVER;
                                dust->field_3C        = 1;
                                dust->position.y      = (RSDK_screens->position.y - 128) << 16;
                                player2->playerID     = 1;
                                EntityPlayer *player1 = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
                                if (player1->state == Player_State_Die || player1->state == Player_State_Drown) {
                                    // player2->state           = Player_State_Unknown1;
                                    player2->velocity.x = 0;
                                    player2->velocity.y = 0;
                                    player2->position.x = -0x200000;
                                    player2->position.y = -0x200000;
                                    return;
                                }
                                else {
                                    Player_ChangeCharacter(player2, 1 << entity->contentsData.frameID);
                                    player2->velocity.x = 0;
                                    player2->velocity.y = 0;
                                    player2->groundVel  = 0;
                                    player2->position.x = -0x400000;
                                    player2->position.y = -0x400000;
                                    player2->angle      = 128;
                                    if (player2->characterID == ID_TAILS) {
                                        // player2->state = Player_State_FlyIn;
                                    }
                                    else {
                                        // player2->state         = Player_State_JumpIn;
                                        player2->maxGlideSpeed = ((RSDK_screens->position.y + RSDK_screens->height + 16) << 16) - player->position.y;
                                        player2->drawFX |= FX_SCALE;
                                        player2->scale.x = 1024;
                                        player2->scale.y = 1024;
                                        int spd          = player2->maxGlideSpeed / -12;
                                        if (spd >= -0x68000 || spd > -0xE0000) {
                                            if (spd < -0x68000)
                                                player2->velocity.y = player2->maxGlideSpeed / -12;
                                            else
                                                player2->velocity.y = -0x68000;
                                        }
                                        else {
                                            player2->velocity.y = -0xE0000;
                                        }
                                    }
                                    player2->field_214     = &dust->position.x;
                                    player2->maxGlideSpeed = 0;
                                    player2->nextAirState     = 0;
                                    player2->nextGroundState  = 0;
                                    // player2->inputState      = Player_FlyCarryInputState;
                                    player2->tileCollisions  = 0;
                                    player2->interaction     = 0;
                                    player2->drawOrder       = Zone->uiDrawLow;
                                    player2->airTimer        = 0;
                                    player2->active          = ACTIVE_NORMAL;
                                    player2->collisionPlane  = 0;
                                    player2->collisionMode   = CMODE_FLOOR;
                                    player2->collisionLayers = Zone->fgLayers;
                                    player2->controllerID    = 2;
                                    player2->sidekick        = 1;
                                    player2->drawFX          = 3;
                                    player2->visible         = 1;
                                    // HUD->field_28                       = 120;
                                }
                            }
                        }
                        RSDK.PlaySFX(ItemBox->sfx_Revovery, 0, 255);
                    }
                    else {
#endif
                        switch (entity->contentsData.frameID) {
                            case 0: Player_ChangeCharacter(player, ID_SONIC); break;
                            case 1: Player_ChangeCharacter(player, ID_TAILS); break;
                            case 2: Player_ChangeCharacter(player, ID_KNUCKLES); break;
#if RETRO_USE_PLUS
                            case 3: Player_ChangeCharacter(player, ID_MIGHTY); break;
                            case 4: Player_ChangeCharacter(player, ID_RAY); break;
#endif
                            default: break;
                        }
                        EntityExplosion *explosion =
                            (EntityExplosion *)RSDK.SpawnEntity(Explosion->objectID, (void *)1, player->position.x, player->position.y);
                        explosion->drawOrder = Zone->drawOrderHigh;
                        RSDK.PlaySFX(ItemBox->sfx_PowerDown, 0, 255);
#if RETRO_USE_PLUS
                    }
#endif
                }
                else {
                    switch (entity->contentsData.frameID) {
                        case 1: entity->type = 1; break;
                        case 2: entity->type = 2; break;
                        case 3: entity->type = 3; break;
                        case 4: entity->type = 4; break;
                        case 5: entity->type = 11; break;
                        case 6: entity->type = 12; break;
                        case 7: entity->type = 13; break;
                        default: entity->type = 0; break;
                    }
                    entity = (EntityItemBox *)RSDK_sceneInfo->entity;
                    entity = (EntityItemBox *)RSDK_sceneInfo->entity;
                    player = (EntityPlayer *)entity->parent;
                    if ((uint)entity->type <= 0x11)
                        continue;
                }
                return;
            }
            default: return;
        }
    }
}
void ItemBox_Break(EntityItemBox *itemBox, void *p)
{
    EntityPlayer *player = (EntityPlayer *)p;
    if (options->gameMode == MODE_COMPETITION) {
        ++options->competitionSession[RSDK.GetEntityID(player) + 55];
    }
    RSDK.SpawnEntity(0, 0, itemBox->position.x, itemBox->position.y);
#if RETRO_USE_PLUS
    if (player->characterID != ID_MIGHTY && player->playerAnimData.animationID != ANI_DROPDASH)
        player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);
    else
#endif
        player->velocity.y -= 0x10000;
    itemBox->storedEntity  = player;
    itemBox->alpha         = 256;
    itemBox->contentsSpeed = -0x30000;
    itemBox->active        = 2;
    itemBox->velocity.y    = -0x20000;
    itemBox->isContents    = true;
    itemBox->state         = ItemBox_State_Contents;
    RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 1, &itemBox->brokenData, true, 0);
    itemBox->brokenData.frameID = ItemBox->brokenFrame++;
    ItemBox->brokenFrame %= 3;
    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->overlayData, true, 0);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->debrisData, true, 0);

    EntityExplosion *explosion = (EntityExplosion *)RSDK.SpawnEntity(Explosion->objectID, 0, itemBox->position.x, itemBox->position.y - 0x100000);
    explosion->drawOrder       = Zone->drawOrderHigh;

    for (int d = 0; d < 6; ++d) {
        EntityDebris *debris = (EntityDebris *)RSDK.SpawnEntity(Debris->objectID, 0, itemBox->position.x + RSDK.Rand(-0x80000, 0x80000),
                                                                itemBox->position.y + RSDK.Rand(-0x80000, 0x80000));
        debris->state        = Debris_State_Fall;
        debris->gravity      = 0x4000;
        debris->velocity.x   = RSDK.Rand(0, 0x20000);
        if (debris->position.x < itemBox->position.x)
            debris->velocity.x = -debris->velocity.x;
        debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
        debris->drawFX     = 1;
        debris->direction  = d & 3;
        debris->drawOrder  = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 6, &debris->data, true, RSDK.Rand(0, 4));
    }

    RSDK.PlaySFX(ItemBox->sfx_Destroy, 0, 255);
    itemBox->active = ACTIVE_NORMAL;
    if (itemBox->type == 13) {
#if RETRO_USE_PLUS
        if (options->gameMode != MODE_ENCORE) {
#endif
            while (true) {
                itemBox->type = RSDK.Rand(0, 13);
                switch (itemBox->type) {
                    case 7:
                        if (options->gameMode == MODE_TIMEATTACK)
                            continue;
                        switch (player->characterID) {
                            case ID_SONIC:
                                itemBox->type                 = 7;
                                itemBox->contentsData.frameID = itemBox->type;
                                break;
                            case ID_TAILS:
                                itemBox->type                 = 8;
                                itemBox->contentsData.frameID = itemBox->type;
                                break;
                            case ID_KNUCKLES:
                                itemBox->type                 = 9;
                                itemBox->contentsData.frameID = itemBox->type;
                                break;
#if RETRO_USE_PLUS
                            case ID_MIGHTY:
                                itemBox->type                 = 15;
                                itemBox->contentsData.frameID = itemBox->type;
                                break;
                            case ID_RAY:
                                itemBox->type                 = 16;
                                itemBox->contentsData.frameID = itemBox->type;
#endif
                                break;
                            default: itemBox->contentsData.frameID = itemBox->type; break;
                        }
                        itemBox->contentsData.frameID = itemBox->type;
                        break;
                    case 8:
                    case 9:
                    case 15:
                    case 16: continue;
                    case 12:
                        if (options->gameMode != MODE_COMPETITION)
                            continue;
                        itemBox->contentsData.frameID = itemBox->type;
                        break;
                    default: itemBox->contentsData.frameID = itemBox->type; break;
                }
                break;
            }
#if RETRO_USE_PLUS
        }
#endif
    }
}
bool32 ItemBox_HandleFallingCollision()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    if (entity->direction)
        return false;

    entity->unknownPos.x = -entity->position.x;
    entity->unknownPos.y = -entity->position.y;
    if (entity->velocity.y)
        entity->parent = NULL;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    ItemBox_HandleObjectCollisions();
    if (!((entity->velocity.y < 0 || entity->direction != FLIP_Y || entity->brokenData.animationID != 1)
             ? RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)
             : RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, true))) {
        entity->unknownPos.x += entity->position.x;
        entity->unknownPos.y += entity->position.y;
        return false;
    }
    else {
        entity->velocity.y = 0;
        if (entity->state != ItemBox_State_Unknown && entity->state != ItemBox_State_Contents)
            entity->active = ACTIVE_BOUNDS;
        entity->unknownPos.x += entity->position.x;
        entity->unknownPos.y += entity->position.y;
        return true;
    }
}
bool32 ItemBox_HandlePlatformCollision(void *p)
{
    EntityPlatform *platform = (EntityPlatform *)p;
    EntityItemBox *entity    = (EntityItemBox *)RSDK_sceneInfo->entity;

    bool collided = false;
    if (platform->state != Platform_State_Falling && platform->state != Platform_State_OffScreenReset) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
        if (platform->collisionType) {
            if (platform->collisionType != 1) {
                if (platform->collisionType != 2 || !RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, entity, &ItemBox->hitbox)) {
                    platform->position.x = platform->centerPos.x;
                    platform->position.y = platform->centerPos.y;
                    return false;
                }
                if (entity->collisionLayers & Zone->moveID) {
                    TileLayer *move  = RSDK.GetSceneLayer(Zone->moveLayer);
                    move->position.x = -(platform->drawPos.x + platform->tileOrigin.x) >> 16;
                    move->position.y = -(platform->drawPos.y + platform->tileOrigin.y) >> 16;
                }
                if (entity->state == ItemBox_State_Normal || entity->velocity.y > 0x3800) {
                    platform->position.x = platform->centerPos.x;
                    platform->position.y = platform->centerPos.y;
                    return false;
                }
                else {
                }
                collided = true;
            }
            else {
                collided = RSDK.CheckObjectCollisionBox(platform, RSDK.GetHitbox(&platform->animData, 1), entity, &ItemBox->hitbox, 1);
            }
        }
        else {
            collided = RSDK.CheckObjectCollisionPlatform(platform, RSDK.GetHitbox(&platform->animData, 0), entity, &ItemBox->hitbox, 1);
        }

        if (!collided) {
            platform->position.x = platform->centerPos.x;
            platform->position.y = platform->centerPos.y;
            return false;
        }

        entity->parent        = platform;
        entity->scale.x       = (entity->position.x - platform->drawPos.x) & 0xFFFF0000;
        entity->scale.y       = (entity->position.y - platform->drawPos.y) & 0xFFFF0000;
        entity->updateRange.x = platform->updateRange.x;
        entity->updateRange.y = platform->updateRange.y;
        if (entity->state == ItemBox_State_Falling)
            entity->state = ItemBox_State_Normal;
        // if (platform->state == Platform_State_Collapsing && !platform->collapseDelay)
        //    platform->collapseDelay = 30;
        platform->stood      = true;
        entity->velocity.y   = 0;
        platform->position.x = platform->centerPos.x;
        platform->position.y = platform->centerPos.y;
        return collided;
    }
    return false;
}
void ItemBox_HandleObjectCollisions()
{
    bool flag             = false;
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    if (Platform) {
        if (entity->parent) {
            EntityPlatform *platform = (EntityPlatform *)entity->parent;
            if (platform->objectID == Platform->objectID) {
                /*platform[1].tileCollisions = 1;
                entity->position.x = entity->scale.x + ice[1].depth;
                entity->position.y = (entity->scale.y + *(_DWORD *)&platform[1].group) & 0xFFFF0000;
                entity->unknownPos.x       = platform[1].inBounds & 0xFFFF0000;
                entity->unknownPos.y    = platform[1].field_3C & 0xFFFF0000;
                entity->contentsPos.x += platform[1].inBounds;
                entity->contentsPos.y += platform[1].field_3C;
                entity->velocity.y = 0;
                flag               = true;*/
            }
        }
        else {
            //EntityPlatform *platform = NULL;
            /*while (RSDK.GetActiveObjects(Platform->objectID, (Entity**)&platform) == 1) {
                if (ItemBox_HandlePlatformCollision(platform))
                    flag = true;
            }*/
        }
    }

    if (TilePlatform) {
        if (entity->parent) {
            EntityTilePlatform *tilePlatform = (EntityTilePlatform *)entity->parent;
            if (tilePlatform->objectID == TilePlatform->objectID) {
                /*flag                           = true;
                tilePlatform[1].tileCollisions = 1;
                entity->position.x        = entity->scale.x + tilePlatform[1].depth;
                entity->position.y        = (entity->scale.y + *(_DWORD *)&tilePlatform[1].group) & 0xFFFF0000;
                entity->unknownPos.x           = tilePlatform[1].inBounds & 0xFFFF0000;
                entity->unknownPos.y           = tilePlatform[1].field_3C & 0xFFFF0000;
                entity->contentsPos.x += tilePlatform[1].inBounds;
                entity->contentsPos.y += tilePlatform[1].field_3C;
                entity->velocity.y = 0;*/
            }
        }
    }

    if (Crate) {
        if (entity->parent) {
            EntityCrate *crate = (EntityCrate *)entity->parent;
            if (crate->objectID == Crate->objectID) {
                /*crate[1].tileCollisions = 1;
                entity->position.x      = entity->scale.x + crate[1].depth;
                entity->position.y      = (entity->scale.y + *(_DWORD *)&crate[1].group) & 0xFFFF0000;
                v12                     = crate[1].inBounds;
                entity->unknownPos.x    = v12;
                v13                     = crate[1].field_3C & 0xFFFF0000;
                entity->unknownPos.x    = v12 & 0xFFFF0000;
                entity->unknownPos.y    = v13;
                entity->contentsPos.x += crate[1].inBounds;
                entity->contentsPos.y += crate[1].field_3C;
                entity->velocity.y = 0;*/
            }
            else {
                entity->parent = NULL;
            }
        }
        else {
            //EntityCrate *crate = 0;
            /*while (RSDK.GetActiveObjects(Crate->objectID, (Entity**)&crate)) {
                if (crate[2].updateRange.y != 1 && ItemBox_HandlePlatformCollision((EntityPlatform *)crate))
                    flag = true;
            }*/
        }
    }
    if (!flag)
        entity->parent = NULL;
    if (Ice) {
        //EntityIce *ice = 0;
        /*while (RSDK.GetActiveObjects(Ice->objectID, (Entity**)&ice)) {
            if (ice[1].position.y == Ice_Unknown18) {
                int storeX = ice->position.x;
                int storeY = ice->position.y;
                ice->position.x -= ice[2].alpha;
                ice->position.y -= ice[2].rotation;

                if (RSDK.CheckObjectCollisionBox(ice, &ice[2].inBounds, entity, &ItemBox->hitbox, 1) == 1) {
                    entity->position.x += ice[2].alpha;
                    entity->position.y += ice[2].rotation;
                    entity->position.y = entity->position.y >> 0x10 << 0x10;
                    entity->contentsPos.x += ice[2].alpha;
                    entity->contentsPos.y += ice[2].rotation;
                    entity->contentsPos.y         = entity->contentsPos.y >> 0x10 << 0x10;
                    entity->unknownPos.x     = ice[2].alpha;
                    entity->unknownPos.y          = ice[2].rotation;
                    entity->velocity.y       = 0;
                }
                ice->position.x = storeX;
                ice->position.y    = storeY;
            }
        }*/
    }
    for (EntitySpikes *spikes = 0; RSDK.GetActiveObjects(Spikes->objectID, (Entity **)&spikes);) {
        int storeX = spikes->position.x;
        int storeY = spikes->position.y;
        /*spikes->position.x -= spikes->unknownPos.x;
        spikes->position.y -= spikes->unknownPos.y;
        if (RSDK.CheckObjectCollisionBox(spikes, &spikes->hitbox, entity, &ItemBox->hitbox, 1) == 1) {
            entity->position.x += spikes->unknownPos.x;
            entity->position.y += spikes->unknownPos.y;
            LOWORD(entity->position.y) = 0;
            entity->contentsPos.x += spikes->unknownPos.x;
            entity->contentsPos.y += spikes->unknownPos.y;
            LOWORD(entity->contentsPos.y) = 0;
            entity->unknownPos.x          = spikes->unknownPos.x;
            entity->unknownPos.y          = spikes->unknownPos.y;
            entity->velocity.y       = 0;
        }*/
        spikes->position.x = storeX;
        spikes->position.y = storeY;
    }

    EntityItemBox *itemBox = 0;
    while (RSDK.GetActiveObjects(ItemBox->objectID, (Entity **)&itemBox)) {
        if (itemBox != entity) {
            if (entity->state == ItemBox_State_Normal || entity->state == ItemBox_State_Falling) {
                if (itemBox->state == ItemBox_State_Normal || itemBox->state == ItemBox_State_Falling) {
                    int storeX = itemBox->position.x;
                    int storeY = itemBox->position.y;
                    itemBox->position.x -= itemBox->unknownPos.x;
                    itemBox->position.y -= itemBox->unknownPos.y;
                    if (RSDK.CheckObjectCollisionBox(itemBox, &ItemBox->hitbox, entity, &ItemBox->hitbox, 1) == 1) {
                        entity->position.x += itemBox->unknownPos.x;
                        entity->position.y += itemBox->unknownPos.y;
                        entity->position.y = entity->position.y >> 0x10 << 0x10;
                        entity->contentsPos.x += itemBox->unknownPos.x;
                        entity->contentsPos.y += itemBox->unknownPos.y;
                        entity->contentsPos.y = entity->contentsPos.y >> 0x10 << 0x10;

                        entity->unknownPos.x = itemBox->unknownPos.x;
                        entity->unknownPos.y = itemBox->unknownPos.y;
                        entity->velocity.y   = 0;
                    }
                    itemBox->position.x = storeX;
                    itemBox->position.y = storeY;
                }
            }
        }
    }
}

void ItemBox_EditorDraw() {}

void ItemBox_EditorLoad() {}

void ItemBox_Serialize()
{
    RSDK_EDITABLE_VAR(ItemBox, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, isFalling);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, hidden);
    RSDK_EDITABLE_VAR(ItemBox, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(ItemBox, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, lrzConvPhys);
}
