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
            case 0: self->state = LRZConvItem_State_Unknown1; break;
            case 1: self->state = LRZConvItem_State_Unknown2; break;
        }
    }
}

void LRZConvItem_StageLoad(void)
{
    LRZConvItem->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConvItem.bin", SCOPE_STAGE);
    LRZConvItem_SetupHitboxes();
    LRZConvItem->sfxSizzle = RSDK.GetSFX("LRZ/Sizzle.wav");
}

void LRZConvItem_SetupHitboxes(void)
{
    LRZConvItem->hitbox1.left   = -16;
    LRZConvItem->hitbox1.top    = -16;
    LRZConvItem->hitbox1.right  = 16;
    LRZConvItem->hitbox1.bottom = 16;
    LRZConvItem->hitbox2.left   = -10;
    LRZConvItem->hitbox2.top    = -10;
    LRZConvItem->hitbox2.right  = 10;
    LRZConvItem->hitbox2.bottom = 10;
}

Vector2 LRZConvItem_Unknown2(void *e)
{
    EntityLRZConvItem *entity = (EntityLRZConvItem *)e;

    Vector2 movePos;
    movePos.x = 0;
    movePos.y = 0;
    if (RSDK.CheckStageFolder("LRZ2")) {
        int storeX = entity->position.x;
        int storeY = entity->position.y;

        Hitbox hitbox;
        if (entity->objectID == LRZConvItem->objectID) {
            hitbox = LRZConvItem->hitbox1;
        }
        else if (entity->objectID == ItemBox->objectID) {
            hitbox = ItemBox->hitbox;
        }
        else if (entity->objectID == Iwamodoki->objectID) {
            hitbox = Iwamodoki->hitbox1;
        }
        else {
            hitbox.left   = 0;
            hitbox.top    = 0;
            hitbox.right  = 0;
            hitbox.bottom = 0;
        }

        bool32 flag1 = false;
        if (LRZConveyor) {
            foreach_active(LRZConveyor, conveyor)
            {
                int moveX = (conveyor->speed << 14) * (2 * (conveyor->direction == FLIP_X) - 1);
                if (LRZConveyor_HandlePlayerCollisions(conveyor, entity, &hitbox) == C_TOP) {
                    flag1 = true;
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
        if (tileCollided || flag1)
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

        bool32 flag2 = false;
        bool32 flag3 = false;
        switch (behaviour) {
            case 0: break;
            case 1:
                entity->active = ACTIVE_NORMAL;
                if (prevY > entity->position.y)
                    entity->position.y = prevY;
                flag2 = true;
                break;
            case 2:
                flag3          = true;
                entity->active = ACTIVE_NORMAL;
                if (entity->onGround) {
                    if (LRZ2Setup->conveyorOff)
                        entity->velocity.x = 0;
                    else
                        entity->velocity.x = (2 * ((((tileInfo & 0x400) != 0) ^ LRZ2Setup->conveyorDir) != 0) - 1) << 17;
                }
                break;
            case 3:
                flag3          = true;
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
            if ((flag2 ^ 1) && (abs(entity->velocity.x) > 0) && (flag3 ^ 1))
                entity->velocity.x = 0;
        }

        if (entity->onGround)
            entity->velocity.y = 0;
        else
            entity->velocity.y += 0x3800;

        flag1 = false;
        if (entity->velocity.x >= 0) {
            if (entity->velocity.x <= 0)
                flag1 = false;
            else
                flag1 = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, hitbox.right << 16, (hitbox.bottom << 16) << 16, false);
        }
        else {
            flag1 = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, hitbox.left << 16, (hitbox.bottom << 16) << 16, false);
        }

        if (flag2) {
            if (!prevOnGround && entity->onGround) {
                RSDK.PlaySfx(LRZConvItem->sfxSizzle, false, 255);
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
        if (flag1)
            entity->velocity.x = 0;

        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        movePos.x = entity->position.x - storeX;
        movePos.y = entity->position.y - storeY;
        if (flag2 && !RSDK.CheckOnScreen(entity, &entity->updateRange))
            destroyEntity(entity);
    }
    return movePos;
}

void LRZConvItem_State_Unknown1(void)
{
    RSDK_THIS(LRZConvItem);

    int x              = self->position.x;
    int y              = self->position.y;
    Vector2 moveOffset = LRZConvItem_Unknown2(self);

    int storeX         = self->position.x;
    int storeY         = self->position.y;
    self->position.x = x;
    self->position.y = y;

    foreach_active(Player, player)
    {
        switch (Player_CheckCollisionBox(player, self, &LRZConvItem->hitbox1)) {
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

void LRZConvItem_State_Unknown2(void)
{
    RSDK_THIS(LRZConvItem);

    LRZConvItem_Unknown2(self);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &LRZConvItem->hitbox2)) {
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
