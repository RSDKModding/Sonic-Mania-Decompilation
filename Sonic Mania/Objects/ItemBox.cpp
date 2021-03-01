#include "../SonicMania.hpp"

ObjectItemBox *ItemBox;

void ItemBox_Update()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;

    if (entity->state)
        entity->state();

    if (entity->type == 17) {
        if (entity->contentsData.animationID == 2 || (uint)(entity->contentsData.animationID - 7) <= 1) {
            if (options->characterFlags != 0x1F || options->gameMode != MODE_ENCORE) {
                RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 7, &entity->contentsData, 0, 0);
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
            }
            else {
                RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 8, &entity->contentsData, 0, 0);
            }
        }
    }
}

void ItemBox_LateUpdate()
{

}

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

void ItemBox_Create(void* data)
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    if (data)
        entity->type = (int)data;
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
                else if (options->gameMode == MODE_ENCORE) {
                    entity->type = 17;
                }
                else {
                    switch (player->characterID) {
                        case ID_SONIC: entity->type = 7; break;
                        case ID_TAILS: entity->type = 8; break;
                        case ID_KNUCKLES: entity->type = 9; break;
                        case ID_MIGHTY: entity->type = 15; break;
                        case ID_RAY: entity->type = 16; break;
                        default: break;
                    }
                }
                entity->contentsData.frameID = entity->type;
                break;
            case 0xC:
            case 0xD:
                if (options->gameMode == MODE_ENCORE || options->gameMode == MODE_COMPETITION)
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
            // entity->state       = Itembox_State_Falling;
        }
        else if (LRZConvItem && entity->lrzConvPhys) {
            entity->active = ACTIVE_NORMAL;
            // entity->state  = Itembox_State_Conveyor;
        }
        else {
            entity->state = Itembox_State_Normal;
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
            for (EntityItemBox *entity = 0; RSDK.GetObjects(ItemBox->objectID, (Entity **)&entity) == 1; entity->type = 13) {
            }
        }
        else if (options->itemMode == 2) {
            for (EntityItemBox *entity = 0; RSDK.GetObjects(ItemBox->objectID, (Entity **)&entity) == 1; entity->type = 12) {
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
    EntityItemBox *entity   = (EntityItemBox *)RSDK_sceneInfo->entity;
    DebugMode->subtypeCount = User.CheckDLC(DLC_PLUS) != 0 ? 18 : 15;
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
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    //if (LRZConvItem)
    //    LRZConvItem_Unknown2();
    //ItemBox_HandeFallingCollision(entity);
}
void ItemBox_State_Contents()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    //if (LRZConvItem && entity->lrzConvPhys)
    //    LRZConvItem_Unknown2(entity);
    //else
    //    ItemBox_HandeFallingCollision();

    if (entity->contentsSpeed < 0) {
        entity->contentsPos.y += entity->contentsSpeed;
        entity->contentsSpeed += 0x1800;
    }
    if (entity->contentsSpeed >= 0) {
        entity->contentsSpeed = 0;
        //Itembox_GivePowerup();
        RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 5, &entity->contentsData, true, 0);
        entity->state = ItemBox_Unknown5;
    }
}

void ItemBox_Unknown5()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    //if (LRZConvItem && entity->lrzConvPhys)
    //    LRZConvItem_Unknown2(entity);
    //else
    //    ItemBox_HandeFallingCollision();
    RSDK.ProcessAnimation(&entity->contentsData);

    if (entity->contentsData.frameID == entity->contentsData.frameCount - 1) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->contentsData, true, 0);
        entity->state = ItemBox_State_Broken;
    }
}

void Itembox_State_Normal()
{
    EntityItemBox *entity = (EntityItemBox *)RSDK_sceneInfo->entity;
    entity->contentsPos.x = RSDK_sceneInfo->entity->position.x;

    if (entity->direction == FLIP_NONE)
        entity->contentsPos.y = entity->position.y - 0x30000;
    else
        entity->contentsPos.y = entity->position.y + 0x30000;
    // Itembox_HandleObjectCollisions();
    // Itembox_CheckHit();
    RSDK.ProcessAnimation(&entity->overlayData);
    if (entity->type == 17) {
        RSDK.ProcessAnimation(&entity->contentsData);
        if (!User.CheckDLC(DLC_PLUS) && entity->contentsData.frameID >= 3)
            entity->contentsData.frameID = 0;
    }

    if (entity->randomValue) {
        entity->randomValue--;
    }
    else {
        RSDK.ProcessAnimation(&entity->debrisData);
        if (!entity->debrisData.frameID) {
            entity->randomValue           = RSDK.Rand(1, 15);
            entity->debrisData.frameDelay = RSDK.Rand(1, 32);
        }
    }
}

void ItemBox_EditorDraw()
{

}

void ItemBox_EditorLoad()
{

}

void ItemBox_Serialize()
{
    RSDK_EDITABLE_VAR(ItemBox, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, isFalling);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, hidden);
    RSDK_EDITABLE_VAR(ItemBox, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(ItemBox, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, lrzConvPhys);
}

