// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZConvItem Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        self->type = VOID_TO_INT(data);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
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

Vector2 LRZConvItem_HandleLRZConvPhys(void *e)
{
    EntityLRZConvItem *entity = (EntityLRZConvItem *)e;

    Vector2 moveOffset;
    moveOffset.x = 0;
    moveOffset.y = 0;
    if (RSDK.CheckSceneFolder("LRZ2")) {
        int32 storeX = entity->position.x;
        int32 storeY = entity->position.y;

        Hitbox hitbox;
        hitbox.left   = 0;
        hitbox.top    = 0;
        hitbox.right  = 0;
        hitbox.bottom = 0;

        if (entity->classID == LRZConvItem->classID)
            hitbox = LRZConvItem->hitboxRock;
        else if (entity->classID == ItemBox->classID)
            hitbox = ItemBox->hitboxItemBox;
        else if (entity->classID == Iwamodoki->classID)
            hitbox = Iwamodoki->hitboxBadnik;

        // Handle Object-Based Conveyor interactions
        bool32 conveyorCollided = false;
        if (LRZConveyor) {
            foreach_active(LRZConveyor, conveyor)
            {
                int32 moveX = (conveyor->speed << 14) * (2 * (conveyor->direction == FLIP_X) - 1);
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

        // Try to collide with the floor
        int32 prevY         = entity->position.y;
        bool32 tileCollided = RSDK.ObjectTileGrip(entity, Zone->collisionLayers, CMODE_FLOOR, 0, 0, hitbox.bottom << 16, 4);
        if (!tileCollided) {
            if (!RSDK.ObjectTileGrip(entity, Zone->collisionLayers, CMODE_FLOOR, 0, (hitbox.right << 16) - 0x10000, hitbox.bottom << 16, 4)
                || !RSDK.ObjectTileGrip(entity, Zone->collisionLayers, CMODE_FLOOR, 0, (hitbox.right << 16) - 0x10000, hitbox.bottom << 16, 4)) {
                if (RSDK.ObjectTileGrip(entity, Zone->collisionLayers, CMODE_FLOOR, 0, (hitbox.left + 1) << 16, hitbox.bottom << 16, 4)) {
                    tileCollided =
                        RSDK.ObjectTileGrip(entity, Zone->collisionLayers, CMODE_FLOOR, 0, (hitbox.left + 1) << 16, hitbox.bottom << 16, 4);
                }
            }
        }

        bool32 prevOnGround = entity->onGround;
        entity->onGround    = false;
        if (tileCollided || conveyorCollided)
            entity->onGround = true;

        int32 tileInfo  = 0;
        uint8 behaviour = LRZ2_TFLAGS_NORMAL;

        // try to grab tile info & behaviour for tile-based conveyor interactions
        LRZ2Setup_GetTileInfo(entity->position.x, entity->position.y + (hitbox.bottom << 16), 0, 0, entity->collisionPlane, &tileInfo, &behaviour);
        if (behaviour == LRZ2_TFLAGS_NORMAL) {
            LRZ2Setup_GetTileInfo(entity->position.x + (hitbox.right << 16), entity->position.y + (hitbox.bottom << 16), 0, 0, entity->collisionPlane,
                                  &tileInfo, &behaviour);
        }
        if (behaviour == LRZ2_TFLAGS_NORMAL) {
            LRZ2Setup_GetTileInfo(entity->position.x + (hitbox.left << 16), entity->position.y + (hitbox.bottom << 16), 0, 0, entity->collisionPlane,
                                  &tileInfo, &behaviour);
        }
        if (behaviour == LRZ2_TFLAGS_NORMAL) {
            LRZ2Setup_GetTileInfo(entity->position.x, entity->position.y + (hitbox.bottom << 16), 0, 0, entity->collisionPlane, &tileInfo,
                                  &behaviour);
        }

        bool32 lavaCollided         = false;
        bool32 tileConveyorCollided = false;
        switch (behaviour) {
            case LRZ2_TFLAGS_NORMAL: break;

            case LRZ2_TFLAGS_LAVA:
                entity->active = ACTIVE_NORMAL;

                if (prevY > entity->position.y)
                    entity->position.y = prevY;

                lavaCollided = true;
                break;

            case LRZ2_TFLAGS_CONVEYOR_L:
                tileConveyorCollided = true;
                entity->active       = ACTIVE_NORMAL;

                if (entity->onGround) {
                    if (LRZ2Setup->conveyorOff)
                        entity->velocity.x = 0;
                    else
                        entity->velocity.x = (2 * ((((tileInfo & 0x400) != 0) ^ LRZ2Setup->conveyorDir) != 0) - 1) << 17;
                }
                break;

            case LRZ2_TFLAGS_CONVEYOR_R:
                tileConveyorCollided = true;
                entity->active       = ACTIVE_NORMAL;

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

        // Apply Gravity
        if (entity->onGround)
            entity->velocity.y = 0;
        else
            entity->velocity.y += 0x3800;

        bool32 wallCollided = false;
        if (entity->velocity.x < 0)
            wallCollided = RSDK.ObjectTileCollision(entity, Zone->collisionLayers, CMODE_FLOOR, 0, hitbox.left << 16, hitbox.top << 16, false);
        else if (entity->velocity.x > 0)
            wallCollided = RSDK.ObjectTileCollision(entity, Zone->collisionLayers, CMODE_FLOOR, 0, hitbox.right << 16, hitbox.top << 16, false);

        // Apply Lava Physics
        if (lavaCollided) {
            if (!prevOnGround && entity->onGround) {
                RSDK.PlaySfx(LRZConvItem->sfxSizzle, false, 0xFF);
                entity->velocity.x >>= 1;
            }

            if (entity->classID != LRZConvItem->classID || entity->type) {
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
        moveOffset.x = entity->position.x - storeX;
        moveOffset.y = entity->position.y - storeY;

        if (lavaCollided && !RSDK.CheckOnScreen(entity, &entity->updateRange))
            destroyEntity(entity);
    }
    return moveOffset;
}

void LRZConvItem_State_Rock(void)
{
    RSDK_THIS(LRZConvItem);

    int32 x            = self->position.x;
    int32 y            = self->position.y;
    Vector2 moveOffset = LRZConvItem_HandleLRZConvPhys(self);

    int32 storeX     = self->position.x;
    int32 storeY     = self->position.y;
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

    LRZConvItem_HandleLRZConvPhys(self);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &LRZConvItem->hitboxSpikeball)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x400, true, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }
}

#if GAME_INCLUDE_EDITOR
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
