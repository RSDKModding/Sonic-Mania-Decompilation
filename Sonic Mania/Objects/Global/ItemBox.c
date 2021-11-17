#include "SonicMania.h"

ObjectItemBox *ItemBox;

void ItemBox_Update(void)
{
    RSDK_THIS(ItemBox);
    StateMachine_Run(entity->state);

#if RETRO_USE_PLUS
    if (entity->type == ITEMBOX_STOCK) {
        if (entity->animatorContents.animationID == 2 || entity->animatorContents.animationID == 7 || entity->animatorContents.animationID == 8) {
            if (globals->characterFlags == 0x1F && globals->gameMode == MODE_ENCORE) {
                RSDK.SetSpriteAnimation(ItemBox->aniFrames, 8, &entity->animatorContents, false, 0);
            }
            else {
                RSDK.SetSpriteAnimation(ItemBox->aniFrames, 7, &entity->animatorContents, false, 0);
                if (globals->gameMode == MODE_ENCORE) {
                    int32 id = 0;
                    while ((1 << entity->animatorContents.frameID) & globals->characterFlags) {
                        if (++entity->animatorContents.frameID > 4)
                            entity->animatorContents.frameID = 0;
                        if (++id > 5) {
                            RSDK.SetSpriteAnimation(ItemBox->aniFrames, 8, &entity->animatorContents, false, 0);
                            RSDK.PrintLog(PRINT_NORMAL, "Bad Change Item State");
                        }
                    }
                }
            }
        }
    }
#endif
}

void ItemBox_LateUpdate(void) {}

void ItemBox_StaticUpdate(void)
{
    if (!(Zone->timer & 3)) {
        RSDK.RotatePalette(0, 60, 63, true);
        RSDK.RotatePalette(1, 60, 63, true);
    }
}

void ItemBox_Draw(void)
{
    RSDK_THIS(ItemBox);
    if (!entity->hidden) {
        if (entity->isContents) {
            if (RSDK_sceneInfo->currentDrawGroup == Zone->playerDrawHigh) {
                entity->drawFX = FX_NONE;
                RSDK.DrawSprite(&entity->animatorContents, &entity->contentsPos, false);
            }
            else {
                entity->drawFX    = FX_FLIP;
                entity->inkEffect = INK_NONE;
                RSDK.DrawSprite(&entity->animatorBox, NULL, false);
                RSDK.AddDrawListRef(Zone->playerDrawHigh, RSDK_sceneInfo->entitySlot);
            }
        }
        else {
            entity->inkEffect = INK_NONE;
            RSDK.DrawSprite(&entity->animatorBox, NULL, false);
            RSDK.DrawSprite(&entity->animatorContents, &entity->contentsPos, false);
            entity->inkEffect = INK_ADD;
            RSDK.DrawSprite(&entity->animatorOverlay, NULL, false);
            entity->inkEffect = INK_NONE;
            RSDK.DrawSprite(&entity->animatorDebris, NULL, false);
        }
    }
}

void ItemBox_Create(void *data)
{
    RSDK_THIS(ItemBox);
    if (data)
        entity->type = voidToInt(data);
    if (entity->state != ItemBox_State_Broken) {
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 0, &entity->animatorBox, true, 0);
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 2, &entity->animatorContents, true, 0);
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 3, &entity->animatorOverlay, true, 0);
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 4, &entity->animatorDebris, true, 0);

        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        switch (entity->type) {
            case ITEMBOX_1UP_SONIC:
            case ITEMBOX_1UP_TAILS:
            case ITEMBOX_1UP_KNUX:
#if RETRO_USE_PLUS
            case ITEMBOX_1UP_MIGHTY:
            case ITEMBOX_1UP_RAY:
#endif
                if (globals->gameMode == MODE_TIMEATTACK) {
                    entity->type = ITEMBOX_RING;
                }
#if RETRO_USE_PLUS
                else if (globals->gameMode == MODE_ENCORE) {
                    entity->type = ITEMBOX_STOCK;
                }
#endif
                else {
                    switch (player->characterID) {
                        case ID_SONIC: entity->type = ITEMBOX_1UP_SONIC; break;
                        case ID_TAILS: entity->type = ITEMBOX_1UP_TAILS; break;
                        case ID_KNUCKLES: entity->type = ITEMBOX_1UP_KNUX; break;
#if RETRO_USE_PLUS
                        case ID_MIGHTY: entity->type = ITEMBOX_1UP_MIGHTY; break;
                        case ID_RAY: entity->type = ITEMBOX_1UP_RAY; break;
#endif
                        default: break;
                    }
                }
                entity->animatorContents.frameID = entity->type;
                break;
#if RETRO_USE_PLUS
            case 12: // Swap
            case 13: // Random
                if (globals->gameMode == MODE_ENCORE || globals->gameMode == MODE_COMPETITION)
                    entity->animatorContents.frameID = entity->type;
                else
                    destroyEntity(entity);
#endif
                break;
            default: entity->animatorContents.frameID = entity->type; break;
        }
    }

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->direction *= FLIP_Y;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->visible       = true;
        if (entity->planeFilter > 0 && ((uint8)entity->planeFilter - 1) & 2)
            entity->drawOrder = Zone->drawOrderHigh;
        else
            entity->drawOrder = Zone->drawOrderLow;

        entity->alpha = 0xFF;
        if (entity->state == ItemBox_State_Broken) {
            RSDK.SetSpriteAnimation(ItemBox->aniFrames, 1, &entity->animatorBox, true, entity->animatorBox.frameID);
        }
        else if (entity->hidden) {
            entity->state = StateMachine_None;
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

void ItemBox_StageLoad(void)
{
    ItemBox->aniFrames         = RSDK.LoadSpriteAnimation("Global/ItemBox.bin", SCOPE_STAGE);
    ItemBox->hitbox.left         = -15;
    ItemBox->hitbox.top          = -16;
    ItemBox->hitbox.right        = 15;
    ItemBox->hitbox.bottom       = 16;
    ItemBox->hiddenHitbox.left   = -15;
    ItemBox->hiddenHitbox.top    = -24;
    ItemBox->hiddenHitbox.right  = 15;
    ItemBox->hiddenHitbox.bottom = 16;

    DEBUGMODE_ADD_OBJ(ItemBox);

    if (globals->gameMode == MODE_COMPETITION) {
        if (globals->itemMode == 1) {
            foreach_all(ItemBox, entity) { entity->type = ITEMBOX_RANDOM; }
        }
        else if (globals->itemMode == 2) {
            foreach_all(ItemBox, entity) { entity->type = ITEMBOX_SWAP; }
        }
    }

    ItemBox->sfxDestroy   = RSDK.GetSFX("Global/Destroy.wav");
    ItemBox->sfxTeleport  = RSDK.GetSFX("Global/Teleport.wav");
    ItemBox->sfxHyperRing = RSDK.GetSFX("Global/HyperRing.wav");
    ItemBox->sfxPowerDown = RSDK.GetSFX("Stage/PowerDown.wav");
    ItemBox->sfxRevovery  = RSDK.GetSFX("Global/Recovery.wav");
}

void ItemBox_DebugDraw(void)
{
    RSDK_THIS(ItemBox);
    DebugMode->subtypeCount = ITEMBOX_COUNT;
    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 2, &DebugMode->animator, true, DebugMode->itemSubType);
    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y - 0x30000;
    RSDK.DrawSprite(&DebugMode->animator, &drawPos, false);
}
void ItemBox_DebugSpawn(void)
{
    RSDK_THIS(ItemBox);
    EntityItemBox *itemBox        = CREATE_ENTITY(ItemBox, 0, entity->position.x, entity->position.y);
    itemBox->type                 = DebugMode->itemSubType;
    itemBox->animatorContents.frameID = DebugMode->itemSubType;
}

void ItemBox_State_Broken(void)
{
    RSDK_THIS(ItemBox);
    if (LRZConvItem)
       LRZConvItem_Unknown2(entity);
    ItemBox_HandleFallingCollision();
}
void ItemBox_State_Contents(void)
{
    RSDK_THIS(ItemBox);
    if (LRZConvItem && entity->lrzConvPhys) {
        LRZConvItem_Unknown2(entity);
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
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 5, &entity->animatorContents, true, 0);
        entity->state = ItemBox_State_Unknown;
    }
}

void ItemBox_State_Unknown(void)
{
    RSDK_THIS(ItemBox);
    if (LRZConvItem && entity->lrzConvPhys) {
        LRZConvItem_Unknown2(entity);
    }
    else {
        ItemBox_HandleFallingCollision();
    }
    RSDK.ProcessAnimation(&entity->animatorContents);

    if (entity->animatorContents.frameID == entity->animatorContents.frameCount - 1) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animatorContents, true, 0);
        entity->state = ItemBox_State_Broken;
    }
}

void ItemBox_State_Normal(void)
{
    RSDK_THIS(ItemBox);
    entity->contentsPos.x = entity->position.x;

    if (entity->direction == FLIP_NONE)
        entity->contentsPos.y = entity->position.y - 0x30000;
    else
        entity->contentsPos.y = entity->position.y + 0x30000;
    ItemBox_HandleObjectCollisions();
    ItemBox_CheckHit();
    RSDK.ProcessAnimation(&entity->animatorOverlay);
#if RETRO_USE_PLUS
    if (entity->type == ITEMBOX_STOCK) {
        RSDK.ProcessAnimation(&entity->animatorContents);
        if (!API.CheckDLC(DLC_PLUS) && entity->animatorContents.frameID >= 3)
            entity->animatorContents.frameID = 0;
    }
#endif

    if (entity->timer) {
        entity->timer--;
    }
    else {
        RSDK.ProcessAnimation(&entity->animatorDebris);
        if (!entity->animatorDebris.frameID) {
            entity->timer                 = RSDK.Rand(1, 15);
            entity->animatorDebris.frameDelay = RSDK.Rand(1, 32);
        }
    }
}
void ItemBox_State_Falling(void)
{
    RSDK_THIS(ItemBox);
    if (ItemBox_HandleFallingCollision())
        entity->state = ItemBox_State_Normal;

    entity->contentsPos.x = entity->position.x;
    if (entity->direction == FLIP_NONE)
        entity->contentsPos.y = entity->position.y - 0x30000;
    else
        entity->contentsPos.y = entity->position.y + 0x30000;

    ItemBox_CheckHit();
    RSDK.ProcessAnimation(&entity->animatorOverlay);
#if RETRO_USE_PLUS
    if (entity->type == ITEMBOX_STOCK) {
        RSDK.ProcessAnimation(&entity->animatorContents);
        if (!API.CheckDLC(DLC_PLUS) && entity->animatorContents.frameID >= 3)
            entity->animatorContents.frameID = 0;
    }
#endif

    if (entity->timer) {
        entity->timer--;
    }
    else {
        RSDK.ProcessAnimation(&entity->animatorDebris);
        if (!entity->animatorDebris.frameID) {
            entity->timer                 = RSDK.Rand(1, 15);
            entity->animatorDebris.frameDelay = RSDK.Rand(1, 32);
        }
    }
}
void ItemBox_State_Conveyor(void)
{
    RSDK_THIS(ItemBox);

    entity->moveOffset = LRZConvItem_Unknown2(entity);  
    entity->contentsPos.x = entity->position.x;
    if (entity->direction == FLIP_NONE)
        entity->contentsPos.y = entity->position.y - 0x30000;
    else
        entity->contentsPos.y = entity->position.y + 0x30000;

    ItemBox_CheckHit();
    RSDK.ProcessAnimation(&entity->animatorOverlay);
#if RETRO_USE_PLUS
    if (entity->type == ITEMBOX_STOCK) {
        RSDK.ProcessAnimation(&entity->animatorContents);
        if (!API.CheckDLC(DLC_PLUS) && entity->animatorContents.frameID >= 3)
            entity->animatorContents.frameID = 0;
    }
#endif

    if (entity->timer) {
        entity->timer--;
    }
    else {
        RSDK.ProcessAnimation(&entity->animatorDebris);
        if (!entity->animatorDebris.frameID) {
            entity->timer                 = RSDK.Rand(1, 15);
            entity->animatorDebris.frameDelay = RSDK.Rand(1, 32);
        }
    }
}

void ItemBox_CheckHit(void)
{
    RSDK_THIS(ItemBox);
    foreach_active(Player, player)
    {
        if (entity->planeFilter <= 0 || player->collisionPlane == (((uint8)entity->planeFilter - 1) & 1)) {
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
                entity->position.x -= entity->moveOffset.x;
                entity->position.y -= entity->moveOffset.y;
                int32 px             = player->position.x;
                int32 py             = player->position.y;
                uint8 side          = Player_CheckCollisionBox(player, entity, &ItemBox->hitbox);
                player->position.x = px;
                player->position.y = py;
                entity->position.x += entity->moveOffset.x;
                entity->position.y += entity->moveOffset.y;

                if (side == C_BOTTOM) {
                    entity->active = ACTIVE_ALWAYS;
                    if (!entity->lrzConvPhys)
                        entity->state = ItemBox_State_Falling;
                    entity->velocity.y = -0x20000;
                    if (player->onGround == false)
                        player->velocity.y = 0x20000;
                }
                else if (side == C_TOP) {
                    player->position.x += entity->moveOffset.x;
                    player->position.y += entity->moveOffset.y;
                }

                if (Player_CheckCollisionBox(player, entity, &ItemBox->hitbox) == C_BOTTOM) {
                    if (player->onGround) {
                        player->position.x = px;
                        player->position.y = py;
                    }
                }
            }
            else {
                int32 anim = player->playerAnimator.animationID;
                bool32 flag =
                    anim == ANI_JUMP && (player->velocity.y >= 0 || player->onGround || entity->direction || player->state == Ice_State_FrozenPlayer);
                switch (player->characterID) {
                    case ID_SONIC: flag |= anim == ANI_DROPDASH; break;
                    case ID_KNUCKLES: flag |= anim == ANI_FLY || anim == ANI_FLYLIFTTIRED; break;
#if RETRO_USE_PLUS
                    case ID_MIGHTY: flag |= anim == ANI_DROPDASH || player->jumpAbilityTimer > 1; break;
#endif
                }
                if (!flag) {
                    entity->position.x -= entity->moveOffset.x;
                    entity->position.y -= entity->moveOffset.y;
                    int32 px             = player->position.x;
                    int32 py             = player->position.y;
                    uint8 side          = Player_CheckCollisionBox(player, entity, &ItemBox->hitbox);
                    player->position.x = px;
                    player->position.y = py;
                    entity->position.x += entity->moveOffset.x;
                    entity->position.y += entity->moveOffset.y;

                    if (side == C_BOTTOM) {
                        entity->active = ACTIVE_ALWAYS;
                        if (!entity->lrzConvPhys)
                            entity->state = ItemBox_State_Falling;
                        if (!player->onGround)
                            player->velocity.y = 0x20000;
                        else
                            entity->velocity.y = -0x20000;
                    }
                    else if (side == C_TOP) {
                        player->position.x += entity->moveOffset.x;
                        player->position.y += entity->moveOffset.y;
                    }

                    if (Player_CheckCollisionBox(player, entity, &ItemBox->hitbox) == C_BOTTOM) {
                        if (player->onGround) {
                            player->position.x = px;
                            player->position.y = py;
                        }
                    }
                }
                else if (Player_CheckBadnikTouch(player, entity, &ItemBox->hitbox)) {
                    ItemBox_Break(entity, player);
                    foreach_break;
                }
            }
        }
    }
}
void ItemBox_GivePowerup(void)
{
    RSDK_THIS(ItemBox);
    EntityPlayer *player = (EntityPlayer *)entity->storedEntity;
    while (true) {
        switch (entity->type) {
            case ITEMBOX_RING: Player_GiveRings(10, player, 1); return;
            case ITEMBOX_BLUESHIELD:
                player->shield = SHIELD_BLUE;
                Player_ApplyShieldEffect(player);
                RSDK.PlaySfx(Shield->sfxBlueShield, false, 255);
                return;
            case ITEMBOX_BUBBLESHIELD:
                player->shield = SHIELD_BUBBLE;
                Player_ApplyShieldEffect(player);
                RSDK.PlaySfx(Shield->sfxBubbleShield, false, 255);
                player->airTimer = 0;
                Music_ResumePrevTrack(TRACK_DROWNING, false);
                return;
            case ITEMBOX_FIRESHIELD:
                player->shield = SHIELD_FIRE;
                Player_ApplyShieldEffect(player);
                RSDK.PlaySfx(Shield->sfxFireShield, false, 255);
                return;
            case ITEMBOX_LIGHTNINGSHIELD:
                player->shield = SHIELD_LIGHTNING;
                Player_ApplyShieldEffect(player);
                RSDK.PlaySfx(Shield->sfxLightningShield, false, 255);
                return;
            case ITEMBOX_INVINCIBLE:
                if (player->superState == SUPERSTATE_NONE) {
                    EntityInvincibleStars *invincibleStars = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), InvincibleStars);
                    RSDK.ResetEntityPtr(invincibleStars, InvincibleStars->objectID, player);
                    player->invincibleTimer = 1260;
                    Music_PlayMusicTrack(TRACK_INVINCIBLE);
                }
                return;
            case ITEMBOX_SNEAKERS:
                player->speedShoesTimer = 1320;
                Player_ChangePhysicsState(player);
                if (player->superState == SUPERSTATE_NONE) {
                    Music_PlayMusicTrack(TRACK_SNEAKERS);
                    EntityImageTrail *powerup = RSDK_GET_ENTITY(2 * Player->playerCount + RSDK.GetEntityID(player), ImageTrail);
                    RSDK.ResetEntityPtr(powerup, ImageTrail->objectID, player);
                }
                return;
            case ITEMBOX_1UP_SONIC:
            case ITEMBOX_1UP_TAILS:
            case ITEMBOX_1UP_KNUX:
#if RETRO_USE_PLUS
            case ITEMBOX_1UP_MIGHTY:
            case ITEMBOX_1UP_RAY:
#endif
                Player_GiveLife(player);
                return;
            case ITEMBOX_EGGMAN: Player_CheckHit(player, entity); return;
            case ITEMBOX_HYPERRING:
                RSDK.PlaySfx(ItemBox->sfxHyperRing, 0, 255);
                player->hyperRing = true;
                return;
            case ITEMBOX_SWAP:
#if RETRO_USE_PLUS
                if (globals->gameMode == MODE_ENCORE) {
                    if (!globals->stock || player->playerAnimator.animationID == ANI_TRANSFORM) {
                        RSDK.PlaySfx(Player->sfxSwapFail, 0, 255);
                        return;
                    }
                    int32 charID = player->characterID;
                    Player_ChangeCharacter(player, (uint8)globals->stock);
                    globals->stock >>= 8;

                    if (globals->stock) {
                        charID <<= 8;
                        if (globals->stock & 0xFF00)
                            charID <<= 8;
                    }
                    globals->stock |= charID;
                    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(1), player->position.x, player->position.y);
                    explosion->drawOrder       = Zone->drawOrderHigh;
                    RSDK.PlaySfx(ItemBox->sfxPowerDown, 0, 255);
                }
                else if (globals->gameMode != MODE_COMPETITION) {
                    RSDK.PlaySfx(Player->sfxSwapFail, 0, 255);
                }
                else {
#endif
                    Zone_StartTeleportAction();
                    RSDK.PlaySfx(ItemBox->sfxTeleport, 0, 255);
#if RETRO_USE_PLUS
                }
#endif
                return;
#if RETRO_USE_PLUS
            case ITEMBOX_RANDOM: {
                uint8 playerIDs[5]    = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
                uint8 newPlayerIDs[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
                if (player->playerAnimator.animationID == ANI_TRANSFORM) {
                    RSDK.PlaySfx(Player->sfxSwapFail, false, 255);
                }
                else {
                    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

                    int32 charID = -1;
                    for (int32 i = player1->characterID; i > 0; ++charID) i >>= 1;
                    playerIDs[0] = charID;

                    charID = -1;
                    for (int32 i = player2->characterID; i > 0; ++charID) i >>= 1;
                    playerIDs[1] = charID;

                    if (playerIDs[1] == 0xFF) {
                        RSDK.PlaySfx(Player->sfxSwapFail, false, 255);
                    }
                    else {
                        for (int32 i = 0; i < 3; ++i) {
                            if (globals->stock & (0xFF << (8 * i))) {
                                int32 characterID = (globals->stock >> (8 * i)) & 0xFF;

                                playerIDs[i + 2] = -1;
                                for (int32 c = characterID; c > 0; ++playerIDs[i + 2]) c >>= 1;
                            }
                        }

                        globals->stock = 0;

                        int32 tempStock = 0;
                        int32 p         = 0;
                        for (; p < 5;) {
                            bool32 inc = true;
                            if (playerIDs[p] == 0xFF)
                                break;

                            newPlayerIDs[p] = RSDK.Rand(0, 5);
                            if ((1 << newPlayerIDs[p]) & globals->characterFlags) {
                                while (true) {
                                    if (!((1 << newPlayerIDs[p]) & tempStock)) {
                                        if (newPlayerIDs[p] != playerIDs[p]) {
                                            tempStock |= 1 << newPlayerIDs[p];
                                            break;
                                        }
                                        else if (p == 4 || playerIDs[p + 1] == 0xFF) {
                                            int32 slot           = RSDK.Rand(0, p);
                                            int32 id             = newPlayerIDs[slot];
                                            newPlayerIDs[slot] = newPlayerIDs[p];
                                            newPlayerIDs[p]    = id;
                                            tempStock |= 1 << newPlayerIDs[slot];
                                            tempStock |= 1 << newPlayerIDs[p];
                                            break;
                                        }
                                        else {
                                            newPlayerIDs[p] = RSDK.Rand(0, 5);
                                            if (!((1 << newPlayerIDs[p]) & globals->characterFlags)) {
                                                inc = false;
                                                break;
                                            }
                                        }
                                    }
                                    else {
                                        newPlayerIDs[p] = RSDK.Rand(0, 5);
                                        if (!((1 << newPlayerIDs[p]) & globals->characterFlags)) {
                                            inc = false;
                                            break;
                                        }
                                    }
                                }
                                if (inc)
                                    ++p;
                            }
                        }

                        for (int32 i = 0; i < p; ++i) {
                            switch (i) {
                                case 0: Player_ChangeCharacter(player1, 1 << newPlayerIDs[0]); break;
                                case 1: Player_ChangeCharacter(player2, 1 << newPlayerIDs[1]); break;
                                default:
                                    globals->stock <<= 8;
                                    globals->stock |= 1 << newPlayerIDs[i];
                                    break;
                            }
                        }

                        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(1), player1->position.x, player1->position.y);
                        explosion->drawOrder       = Zone->drawOrderHigh;

                        explosion            = CREATE_ENTITY(Explosion, intToVoid(1), player2->position.x, player2->position.y);
                        explosion->drawOrder = Zone->drawOrderHigh;

                        RSDK.PlaySfx(ItemBox->sfxPowerDown, 0, 255);
                    }
                }
                return;
            }
#endif
            case ITEMBOX_SUPER:
                Player_GiveRings(50, player, 0);
                Player_CheckGoSuper(player, 127);
                return;
#if RETRO_USE_PLUS
            case ITEMBOX_STOCK: {
                if (entity->animatorContents.animationID == 7) {
                    if (globals->gameMode == MODE_ENCORE) {
                        if (!((1 << entity->animatorContents.frameID) & globals->characterFlags) && globals->characterFlags != 31
                            && !(globals->stock & 0xFF0000)) {
                            globals->characterFlags |= (1 << entity->animatorContents.frameID);
                            EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                            if (player2->objectID) {
                                int32 id = 0;
                                while ((globals->stock >> id) & 0xFF) id += 8;
                                globals->stock |= (1 << entity->animatorContents.frameID << id);
                                HUD->stockFlashTimers[(id >> 3) + 1] = 120;
                            }
                            else {
                                player2->objectID   = Player->objectID;
                                Player->jumpInDelay = 0;
                                EntityDust *dust    = CREATE_ENTITY(Dust, intToVoid(1), player2->position.x, player2->position.y);

                                dust->visible         = 0;
                                dust->active          = ACTIVE_NEVER;
                                dust->isPermanent     = true;
                                dust->position.y      = (RSDK_screens->position.y - 128) << 16;
                                player2->playerID     = 1;
                                EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                                if (player1->state == Player_State_Die || player1->state == Player_State_Drown) {
                                    player2->state      = Player_State_EncoreRespawn;
                                    player2->velocity.x = 0;
                                    player2->velocity.y = 0;
                                    player2->position.x = -0x200000;
                                    player2->position.y = -0x200000;
                                    return;
                                }
                                else {
                                    Player_ChangeCharacter(player2, 1 << entity->animatorContents.frameID);
                                    player2->velocity.x = 0;
                                    player2->velocity.y = 0;
                                    player2->groundVel  = 0;
                                    player2->position.x = -0x400000;
                                    player2->position.y = -0x400000;
                                    player2->angle      = 128;
                                    if (player2->characterID == ID_TAILS) {
                                        player2->state = Player_State_FlyIn;
                                    }
                                    else {
                                        player2->state         = Player_State_JumpIn;
                                        player2->abilityValues[0] = ((RSDK_screens->position.y + RSDK_screens->height + 16) << 16) - player->position.y;
                                        player2->drawFX |= FX_SCALE;
                                        player2->scale.x = 1024;
                                        player2->scale.y = 1024;
                                        int32 spd          = player2->abilityValues[0] / -12;
                                        if (spd >= -0x68000 || spd > -0xE0000) {
                                            if (spd < -0x68000)
                                                player2->velocity.y = player2->abilityValues[0] / -12;
                                            else
                                                player2->velocity.y = -0x68000;
                                        }
                                        else {
                                            player2->velocity.y = -0xE0000;
                                        }
                                    }
                                    player2->abilityPtrs[0]   = dust;
                                    player2->abilityValues[0] = 0;
                                    player2->nextAirState     = StateMachine_None;
                                    player2->nextGroundState  = StateMachine_None;
                                    player2->stateInput      = Player_ProcessP2InputLag;
                                    player2->tileCollisions  = false;
                                    player2->interaction     = false;
                                    player2->drawOrder       = Zone->playerDrawHigh;
                                    player2->airTimer        = 0;
                                    player2->active          = ACTIVE_NORMAL;
                                    player2->collisionPlane  = 0;
                                    player2->collisionMode   = CMODE_FLOOR;
                                    player2->collisionLayers = Zone->fgLayers;
                                    player2->controllerID    = CONT_P2;
                                    player2->sidekick        = true;
                                    player2->drawFX          = FX_FLIP | FX_ROTATE;
                                    player2->visible         = true;
                                    HUD->stockFlashTimers[0] = 120;
                                }
                            }
                        }
                        RSDK.PlaySfx(ItemBox->sfxRevovery, 0, 255);
                    }
                    else {
                        switch (entity->animatorContents.frameID) {
                            case 0: Player_ChangeCharacter(player, ID_SONIC); break;
                            case 1: Player_ChangeCharacter(player, ID_TAILS); break;
                            case 2: Player_ChangeCharacter(player, ID_KNUCKLES); break;
                            case 3: Player_ChangeCharacter(player, ID_MIGHTY); break;
                            case 4: Player_ChangeCharacter(player, ID_RAY); break;
                            default: break;
                        }
                        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(1), player->position.x, player->position.y);
                        explosion->drawOrder       = Zone->drawOrderHigh;
                        RSDK.PlaySfx(ItemBox->sfxPowerDown, 0, 255);
                    }
                }
                else {
                    switch (entity->animatorContents.frameID) {
                        case 1: entity->type = ITEMBOX_BLUESHIELD; break;
                        case 2: entity->type = ITEMBOX_BUBBLESHIELD; break;
                        case 3: entity->type = ITEMBOX_FIRESHIELD; break;
                        case 4: entity->type = ITEMBOX_LIGHTNINGSHIELD; break;
                        case 5: entity->type = ITEMBOX_HYPERRING; break;
                        case 6: entity->type = ITEMBOX_SWAP; break;
                        case 7: entity->type = ITEMBOX_RANDOM; break;
                        default: entity->type = ITEMBOX_RING; break;
                    }
                    player = (EntityPlayer *)entity->parent;
                    if ((uint32)entity->type <= ITEMBOX_STOCK)
                        continue;
                }
                return;
            }
#endif
            default: return;
        }
    }
}
void ItemBox_Break(EntityItemBox *itemBox, void *p)
{
    EntityPlayer *player = (EntityPlayer *)p;
    if (globals->gameMode == MODE_COMPETITION) {
        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
        ++session->items[RSDK.GetEntityID(player)];
    }
    RSDK.CreateEntity(TYPE_BLANK, 0, itemBox->position.x, itemBox->position.y);
#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_DROPDASH)
        player->velocity.y -= 0x10000;
    else
#endif
        player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);
    itemBox->storedEntity  = (Entity *)player;
    itemBox->alpha         = 256;
    itemBox->contentsSpeed = -0x30000;
    itemBox->active        = ACTIVE_NORMAL;
    itemBox->velocity.y    = -0x20000;
    itemBox->isContents    = true;
    itemBox->state         = ItemBox_State_Contents;
    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 1, &itemBox->animatorBox, true, 0);
    itemBox->animatorBox.frameID = ItemBox->brokenFrame++;
    ItemBox->brokenFrame %= 3;
    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->animatorOverlay, true, 0);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->animatorDebris, true, 0);

    EntityExplosion *explosion = (EntityExplosion *)RSDK.CreateEntity(Explosion->objectID, 0, itemBox->position.x, itemBox->position.y - 0x100000);
    explosion->drawOrder       = Zone->drawOrderHigh;

    for (int32 d = 0; d < 6; ++d) {
        EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, 0, itemBox->position.x + RSDK.Rand(-0x80000, 0x80000),
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
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 6, &debris->animator, true, RSDK.Rand(0, 4));
    }

    RSDK.PlaySfx(ItemBox->sfxDestroy, 0, 255);
    itemBox->active = ACTIVE_NORMAL;
    if (itemBox->type == ITEMBOX_RANDOM) {
#if RETRO_USE_PLUS
        if (globals->gameMode != MODE_ENCORE) {
#endif
            while (true) {
                itemBox->type = RSDK.Rand(0, 13);
                switch (itemBox->type) {
                    case ITEMBOX_1UP_SONIC:
                        if (globals->gameMode == MODE_TIMEATTACK)
                            continue;
                        switch (player->characterID) {
                            case ID_SONIC: itemBox->type = ITEMBOX_1UP_SONIC; break;
                            case ID_TAILS: itemBox->type = ITEMBOX_1UP_TAILS; break;
                            case ID_KNUCKLES: itemBox->type = ITEMBOX_1UP_KNUX; break;
#if RETRO_USE_PLUS
                            case ID_MIGHTY: itemBox->type = ITEMBOX_1UP_MIGHTY; break;
                            case ID_RAY: itemBox->type = ITEMBOX_1UP_RAY;
#endif
                                break;
                            default: break;
                        }
                        itemBox->animatorContents.frameID = itemBox->type;
                        break;
                    case ITEMBOX_1UP_TAILS:
                    case ITEMBOX_1UP_KNUX:
#if RETRO_USE_PLUS
                    case ITEMBOX_1UP_MIGHTY:
                    case ITEMBOX_1UP_RAY:
#endif
                        continue;
                    case ITEMBOX_SWAP:
                        if (globals->gameMode != MODE_COMPETITION)
                            continue;
                        itemBox->animatorContents.frameID = itemBox->type;
                        break;
                    default: itemBox->animatorContents.frameID = itemBox->type; break;
                }
                break;
            }
#if RETRO_USE_PLUS
        }
#endif
    }
}
bool32 ItemBox_HandleFallingCollision(void)
{
    RSDK_THIS(ItemBox);
    if (entity->direction)
        return false;

    entity->moveOffset.x = -entity->position.x;
    entity->moveOffset.y = -entity->position.y;
    if (entity->velocity.y)
        entity->parent = NULL;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    ItemBox_HandleObjectCollisions();

    bool32 flag = entity->velocity.y < 0;

    if (flag
        || (entity->direction != FLIP_Y || entity->animatorBox.animationID != 1
                ? !RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)
                : !RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, true))) {
        entity->moveOffset.x += entity->position.x;
        entity->moveOffset.y += entity->position.y;
        return false;
    }
    else {
        entity->velocity.y = 0;
        if (entity->state != ItemBox_State_Unknown && entity->state != ItemBox_State_Contents)
            entity->active = ACTIVE_BOUNDS;
        entity->moveOffset.x += entity->position.x;
        entity->moveOffset.y += entity->position.y;
        return true;
    }
}
bool32 ItemBox_HandlePlatformCollision(void *p)
{
    EntityPlatform *platform = (EntityPlatform *)p;
    RSDK_THIS(ItemBox);

    bool32 collided = false;
    if (platform->state != Platform_State_Falling && platform->state != Platform_State_OffScreenReset) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
        if (platform->collision) {
            if (platform->collision != 1) {
                if (platform->collision != 2 || !RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, entity, &ItemBox->hitbox)) {
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
                collided = RSDK.CheckObjectCollisionBox(platform, RSDK.GetHitbox(&platform->animator, 1), entity, &ItemBox->hitbox, 1);
            }
        }
        else {
            collided = RSDK.CheckObjectCollisionPlatform(platform, RSDK.GetHitbox(&platform->animator, 0), entity, &ItemBox->hitbox, 1);
        }

        if (!collided) {
            platform->position.x = platform->centerPos.x;
            platform->position.y = platform->centerPos.y;
            return false;
        }

        entity->parent        = (Entity *)platform;
        entity->scale.x       = (entity->position.x - platform->drawPos.x) & 0xFFFF0000;
        entity->scale.y       = (entity->position.y - platform->drawPos.y) & 0xFFFF0000;
        entity->updateRange.x = platform->updateRange.x;
        entity->updateRange.y = platform->updateRange.y;
        if (entity->state == ItemBox_State_Falling)
            entity->state = ItemBox_State_Normal;
        if (platform->state == Platform_State_Collapsing && !platform->collapseDelay)
            platform->collapseDelay = 30;
        platform->stood      = true;
        entity->velocity.y   = 0;
        platform->position.x = platform->centerPos.x;
        platform->position.y = platform->centerPos.y;
        return collided;
    }
    return false;
}
void ItemBox_HandleObjectCollisions(void)
{
    bool32 flag = false;
    RSDK_THIS(ItemBox);
    if (Platform) {
        if (entity->parent) {
            EntityPlatform *platform = (EntityPlatform *)entity->parent;
            if (platform->objectID == Platform->objectID) {
                platform->stood      = true;
                entity->position.x   = entity->scale.x + platform->drawPos.x;
                entity->position.y   = (entity->scale.y + platform->drawPos.y) & 0xFFFF0000;
                entity->moveOffset.x = platform->collisionOffset.x & 0xFFFF0000;
                entity->moveOffset.y = platform->collisionOffset.y & 0xFFFF0000;
                entity->contentsPos.x += platform->collisionOffset.x;
                entity->contentsPos.y += platform->collisionOffset.y;
                entity->velocity.y = 0;
                flag               = true;
            }
        }
        else {
            foreach_active(Platform, platform)
            {
                if (ItemBox_HandlePlatformCollision(platform))
                    flag = true;
            }
        }
    }

    if (TilePlatform) {
        if (entity->parent) {
            EntityTilePlatform *tilePlatform = (EntityTilePlatform *)entity->parent;
            if (tilePlatform->objectID == TilePlatform->objectID) {
                flag                 = true;
                tilePlatform->stood  = true;
                entity->position.x   = entity->scale.x + tilePlatform->drawPos.x;
                entity->position.y   = (entity->scale.y + tilePlatform->drawPos.y) & 0xFFFF0000;
                entity->moveOffset.x = tilePlatform->collisionOffset.x & 0xFFFF0000;
                entity->moveOffset.y = tilePlatform->collisionOffset.y & 0xFFFF0000;
                entity->contentsPos.x += tilePlatform->collisionOffset.x;
                entity->contentsPos.y += tilePlatform->collisionOffset.y;
                entity->velocity.y = 0;
            }
        }
    }

    if (Crate) {
        if (entity->parent) {
            EntityCrate *crate = (EntityCrate *)entity->parent;
            if (crate->objectID == Crate->objectID) {
                crate->stood         = true;
                entity->position.x   = entity->scale.x + crate->drawPos.x;
                entity->position.y   = (entity->scale.y + crate->drawPos.x) & 0xFFFF0000;
                entity->moveOffset.x = crate->collisionOffset.x & 0xFFFF0000;
                entity->moveOffset.y = crate->collisionOffset.y & 0xFFFF0000;
                entity->contentsPos.x += crate->collisionOffset.x;
                entity->contentsPos.y += crate->collisionOffset.y;
                entity->velocity.y = 0;
            }
            else {
                entity->parent = NULL;
            }
        }
        else {
            foreach_active(Crate, crate)
            {
                if (!crate->ignoreItemBox && ItemBox_HandlePlatformCollision((EntityPlatform *)crate))
                    flag = true;
            }
        }
    }
    if (!flag)
        entity->parent = NULL;
    if (Ice) {
        foreach_active(Ice, ice)
        {
            if (ice->stateDraw == Ice_StateDraw_Unknown1) {
                int32 storeX = ice->position.x;
                int32 storeY = ice->position.y;
                ice->position.x -= ice->playerPos.x;
                ice->position.y -= ice->playerPos.y;

                if (RSDK.CheckObjectCollisionBox(ice, &ice->hitbox1, entity, &ItemBox->hitbox, true)) {
                    entity->position.x += ice->playerPos.x;
                    entity->position.y += ice->playerPos.y;
                    entity->position.y = entity->position.y >> 0x10 << 0x10;
                    entity->contentsPos.x += ice->playerPos.x;
                    entity->contentsPos.y += ice->playerPos.y;
                    entity->contentsPos.y = entity->contentsPos.y >> 0x10 << 0x10;
                    entity->moveOffset.x  = ice->playerPos.x;
                    entity->moveOffset.y  = ice->playerPos.y;
                    entity->velocity.y    = 0;
                }
                ice->position.x = storeX;
                ice->position.y = storeY;
            }
        }
    }
    foreach_active(Spikes, spikes)
    {
        int32 storeX = spikes->position.x;
        int32 storeY = spikes->position.y;
        spikes->position.x -= spikes->offset.x;
        spikes->position.y -= spikes->offset.y;
        if (RSDK.CheckObjectCollisionBox(spikes, &spikes->hitbox, entity, &ItemBox->hitbox, true)) {
            entity->position.x += spikes->offset.x;
            entity->position.y += spikes->offset.y;
            entity->position.y &= 0xFFFF0000;

            entity->contentsPos.x += spikes->offset.x;
            entity->contentsPos.y += spikes->offset.y;
            entity->contentsPos.y &= 0xFFFF0000;

            entity->moveOffset.x = spikes->offset.x;
            entity->moveOffset.y = spikes->offset.y;
            entity->velocity.y   = 0;
        }
        spikes->position.x = storeX;
        spikes->position.y = storeY;
    }

    foreach_active(ItemBox, itemBox)
    {
        if (itemBox != entity) {
            if (entity->state == ItemBox_State_Normal || entity->state == ItemBox_State_Falling) {
                if (itemBox->state == ItemBox_State_Normal || itemBox->state == ItemBox_State_Falling) {
                    int32 storeX = itemBox->position.x;
                    int32 storeY = itemBox->position.y;
                    itemBox->position.x -= itemBox->moveOffset.x;
                    itemBox->position.y -= itemBox->moveOffset.y;
                    if (RSDK.CheckObjectCollisionBox(itemBox, &ItemBox->hitbox, entity, &ItemBox->hitbox, 1) == 1) {
                        entity->position.x += itemBox->moveOffset.x;
                        entity->position.y += itemBox->moveOffset.y;
                        entity->position.y = entity->position.y >> 0x10 << 0x10;
                        entity->contentsPos.x += itemBox->moveOffset.x;
                        entity->contentsPos.y += itemBox->moveOffset.y;
                        entity->contentsPos.y = entity->contentsPos.y >> 0x10 << 0x10;

                        entity->moveOffset.x = itemBox->moveOffset.x;
                        entity->moveOffset.y = itemBox->moveOffset.y;
                        entity->velocity.y   = 0;
                    }
                    itemBox->position.x = storeX;
                    itemBox->position.y = storeY;
                }
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void ItemBox_EditorDraw(void)
{
    RSDK_THIS(ItemBox);
    int dir = entity->direction;
    entity->direction *= FLIP_Y;

    entity->contentsPos.x = entity->position.x;
    if (entity->direction == FLIP_NONE)
        entity->contentsPos.y = entity->position.y - 0x30000;
    else
        entity->contentsPos.y = entity->position.y + 0x30000;

    RSDK.DrawSprite(&entity->animatorBox, NULL, false);
    RSDK.DrawSprite(&entity->animatorContents, &entity->contentsPos, false);

    entity->direction = dir;
}

void ItemBox_EditorLoad(void)
{
    ItemBox->aniFrames = RSDK.LoadSpriteAnimation("Global/ItemBox.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(ItemBox, type);
    RSDK_ENUM_VAR("Rings", ITEMBOX_RING);
    RSDK_ENUM_VAR("Blue Shield", ITEMBOX_BLUESHIELD);
    RSDK_ENUM_VAR("Bubble Shield", ITEMBOX_BUBBLESHIELD);
    RSDK_ENUM_VAR("Fire Shield", ITEMBOX_FIRESHIELD);
    RSDK_ENUM_VAR("Lightning Shield", ITEMBOX_LIGHTNINGSHIELD);
    RSDK_ENUM_VAR("Invincibility", ITEMBOX_INVINCIBLE);
    RSDK_ENUM_VAR("Speed Shoes", ITEMBOX_SNEAKERS);
    RSDK_ENUM_VAR("1UP (Sonic)", ITEMBOX_1UP_SONIC);
    RSDK_ENUM_VAR("1UP (Tails)", ITEMBOX_1UP_TAILS);
    RSDK_ENUM_VAR("1UP (Knux)", ITEMBOX_1UP_KNUX);
    RSDK_ENUM_VAR("Eggman", ITEMBOX_EGGMAN);
    RSDK_ENUM_VAR("Hyper Ring", ITEMBOX_HYPERRING);
    RSDK_ENUM_VAR("Swap", ITEMBOX_SWAP);
    RSDK_ENUM_VAR("Random", ITEMBOX_RANDOM);
    RSDK_ENUM_VAR("Super", ITEMBOX_SUPER);
#if RETRO_USE_PLUS
    RSDK_ENUM_VAR("1UP (Mighty)", ITEMBOX_1UP_MIGHTY);
    RSDK_ENUM_VAR("1UP (Ray)", ITEMBOX_1UP_RAY);
    RSDK_ENUM_VAR("Extra Stock", ITEMBOX_STOCK);
#endif

    RSDK_ACTIVE_VAR(ItemBox, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
}
#endif

void ItemBox_Serialize(void)
{
    RSDK_EDITABLE_VAR(ItemBox, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, isFalling);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, hidden);
    RSDK_EDITABLE_VAR(ItemBox, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(ItemBox, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, lrzConvPhys);
}
