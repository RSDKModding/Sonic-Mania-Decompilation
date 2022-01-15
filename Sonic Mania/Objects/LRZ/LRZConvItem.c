// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZConvItem Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectLRZConvItem *LRZConvItem;

void LRZConvItem_Update(void)
{
    RSDK_THIS(LRZConvItem);
    StateMachine_Run(self->state);
    RSDK.ProcessAnimation(&self->animator);
}

void LRZConvItem_LateUpdate(void) {}

void LRZConvItem_StaticUpdate(void) {}

void LRZConvItem_Draw(void)
{
    RSDK_THIS(LRZConvItem);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void LRZConvItem_Create(void *data)
{
    RSDK_THIS(LRZConvItem);

    if (data)
        self->type = voidToInt(data);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(LRZConvItem->aniFrames, self->type, &self->animator, true, 0);

    if (!SceneInfo->inEditor) {
        switch (self->type) {
            case LRZCONVITEM_ROCK: self->state = LRZConvItem_State_Rock; break;
            case LRZCONVITEM_SPIKEBALL: self->state = LRZConvItem_State_SpikeBall; break;
        }
    }
}

void LRZConvItem_StageLoad(void)
{
    LRZConvItem->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConvItem.bin", SCOPE_STAGE);
    LRZConvItem_SetupHitboxes();
    LRZConvItem->sfxSizzle = RSDK.GetSfx("LRZ/Sizzle.wav");
}

void LRZConvItem_SetupHitboxes(void)
{
    LRZConvItem->hitboxRock.left   = -16;
    LRZConvItem->hitboxRock.top    = -16;
    LRZConvItem->hitboxRock.right  = 16;
    LRZConvItem->hitboxRock.bottom = 16;

    LRZConvItem->hitboxSpikeball.left   = -10;
    LRZConvItem->hitboxSpikeball.top    = -10;
    LRZConvItem->hitboxSpikeball.right  = 10;
    LRZConvItem->hitboxSpikeball.bottom = 10;
}

Vector2 LRZConvItem_GetMoveOffset(void *e)
{
    EntityLRZConvItem *entity = (EntityLRZConvItem *)e;

    Vector2 movePos;
    movePos.x = 0;
    movePos.y = 0;
    if (RSDK.CheckStageFolder("LRZ2")) {
        int storeX = entity->position.x;
        int storeY = entity->position.y;

        Hitbox hitbox;
        hitbox.left   = 0;
        hitbox.top    = 0;
        hitbox.right  = 0;
        hitbox.bottom = 0;
        if (entity->objectID == LRZConvItem->objectID) 
            hitbox = LRZConvItem->hitboxRock;
        else if (entity->objectID == ItemBox->objectID) 
            hitbox = ItemBox->hitbox;
        else if (entity->objectID == Iwamodoki->objectID) 
            hitbox = Iwamodoki->hitbox1;

        bool32 conveyorCollided = false;
        if (LRZConveyor) {
            foreach_active(LRZConveyor, conveyor)
            {
                int moveX = (conveyor->speed << 14) * (2 * (conveyor->direction == FLIP_X) - 1);
                if (LRZConveyor_HandlePlayerCollisions(conveyor, entity, &hitbox) == C_TOP) {
                    conveyorCollided = true;
                    if (conveyor->off) {
                        entity->velocity.x = 0;
                    }
                    else {
                        entity->position.y += 0x10000;
                        entity->active     = ACTIVE_NORMAL;
                        entity->velocity.x = moveX;
                    }
                }
            }
        }

        int prevY           = entity->position.y;
        bool32 tileCollided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, hitbox.bottom << 16, 4);
        if (!tileCollided) {
            if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, (hitbox.right << 16) - 0x10000, hitbox.bottom << 16, 4)
                || !RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, (hitbox.right << 16) - 0x10000, hitbox.bottom << 16, 4)) {
                if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, (hitbox.left + 1) << 16, hitbox.bottom << 16, 4)) {
                    tileCollided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, (hitbox.left + 1) << 16, hitbox.bottom << 16, 4);
                }
            }
        }

        bool32 prevOnGround = entity->onGround;
        entity->onGround    = false;
        if (tileCollided || conveyorCollided)
            entity->onGround = true;

        int32 tileInfo = 0;
        uint8 behaviour = 0;

        LRZ2Setup_GetTileInfo(&tileInfo, entity->collisionPlane, entity->position.x, (hitbox.bottom << 16) + entity->position.y, 0, 0, &behaviour);
        if (!behaviour) {
            LRZ2Setup_GetTileInfo(&tileInfo, entity->collisionPlane, (hitbox.right << 16) + entity->position.x,
                                  (hitbox.bottom << 16) + entity->position.y, 0, 0, &behaviour);
            if (!behaviour) {
                LRZ2Setup_GetTileInfo(&tileInfo, entity->collisionPlane, (hitbox.left << 16) + entity->position.x,
                                      (hitbox.bottom << 16) + entity->position.y, 0, 0, &behaviour);
                if (!behaviour) {
                    LRZ2Setup_GetTileInfo(&tileInfo, entity->collisionPlane, entity->position.x, (hitbox.bottom << 16) + entity->position.y, 0, 0,
                                          &behaviour);
                }
            }
        }

        bool32 lavaCollided = false;
        bool32 tileConveyorCollided = false;
        switch (behaviour) {
            case LRZ2_TBEHAVE_NORMAL: break;
            case LRZ2_TBEHAVE_LAVA:
                entity->active = ACTIVE_NORMAL;
                if (prevY > entity->position.y)
                    entity->position.y = prevY;
                lavaCollided = true;
                break;
            case LRZ2_TBEHAVE_CONVEYOR_L:
                tileConveyorCollided = true;
                entity->active = ACTIVE_NORMAL;
                if (entity->onGround) {
                    if (LRZ2Setup->conveyorOff)
                        entity->velocity.x = 0;
                    else
                        entity->velocity.x = (2 * ((((tileInfo & 0x400) != 0) ^ LRZ2Setup->conveyorDir) != 0) - 1) << 17;
                }
                break;
            case LRZ2_TBEHAVE_CONVEYOR_R:
                tileConveyorCollided       = true;
                entity->active = ACTIVE_NORMAL;
                if (entity->onGround) {
                    if (LRZ2Setup->conveyorOff)
                        entity->velocity.x = 0;
                    else
                        entity->velocity.x = (2 * ((((tileInfo & 0x400) != 0) ^ LRZ2Setup->conveyorDir) != 1) - 1) << 17;
                }
                break;
        }

        if (tileCollided) {
            if ((lavaCollided ^ 1) && (abs(entity->velocity.x) > 0) && (tileConveyorCollided ^ 1))
                entity->velocity.x = 0;
        }

        if (entity->onGround)
            entity->velocity.y = 0;
        else
            entity->velocity.y += 0x3800;

        bool32 wallCollided = false;
        if (entity->velocity.x < 0)
            wallCollided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, hitbox.left << 16, hitbox.bottom << 16, false);
        else if (entity->velocity.x > 0)
            wallCollided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, hitbox.right << 16, hitbox.bottom << 16, false);

        if (lavaCollided) {
            if (!prevOnGround && entity->onGround) {
                RSDK.PlaySfx(LRZConvItem->sfxSizzle, false, 0xFF);
                entity->velocity.x >>= 1;
            }
            if (entity->objectID != LRZConvItem->objectID || entity->type) {
                entity->velocity.y = 0x4000;
            }
            else {
                if (!entity->velocity.x)
                    entity->velocity.y = 0x4000;
                else
                    entity->velocity.y = 0;
            }
        }
        if (wallCollided)
            entity->velocity.x = 0;

        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        movePos.x = entity->position.x - storeX;
        movePos.y = entity->position.y - storeY;
        if (lavaCollided && !RSDK.CheckOnScreen(entity, &entity->updateRange))
            destroyEntity(entity);
    }
    return movePos;
}

void LRZConvItem_State_Rock(void)
{
    RSDK_THIS(LRZConvItem);

    int x              = self->position.x;
    int y              = self->position.y;
    Vector2 moveOffset = LRZConvItem_GetMoveOffset(self);

    int storeX         = self->position.x;
    int storeY         = self->position.y;
    self->position.x = x;
    self->position.y = y;

    foreach_active(Player, player)
    {
        switch (Player_CheckCollisionBox(player, self, &LRZConvItem->hitboxRock)) {
            case C_TOP:
                player->position.x += moveOffset.x;
                player->position.y += moveOffset.y;
                if (moveOffset.y <= 0)
                    player->collisionFlagV |= 1;
                break;
            case C_LEFT: player->collisionFlagH |= 1; break;
            case C_RIGHT: player->collisionFlagH |= 2; break;
            case C_BOTTOM:
                if (moveOffset.y >= 0)
                    player->collisionFlagV |= 2;
                break;
            default: break;
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void LRZConvItem_State_SpikeBall(void)
{
    RSDK_THIS(LRZConvItem);

    LRZConvItem_GetMoveOffset(self);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &LRZConvItem->hitboxSpikeball)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x400, player, true, &player->uncurlTimer))
#endif
                Player_CheckHit(player, self);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void LRZConvItem_EditorDraw(void)
{
    RSDK_THIS(LRZConvItem);
    RSDK.SetSpriteAnimation(LRZConvItem->aniFrames, self->type, &self->animator, true, 0);

    LRZConvItem_Draw();
}

void LRZConvItem_EditorLoad(void)
{
    LRZConvItem->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConvItem.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(LRZConvItem, type);
    RSDK_ENUM_VAR("Rock", LRZCONVITEM_ROCK);
    RSDK_ENUM_VAR("Spike Ball", LRZCONVITEM_SPIKEBALL);
}
#endif

void LRZConvItem_Serialize(void) { RSDK_EDITABLE_VAR(LRZConvItem, VAR_UINT8, type); }
