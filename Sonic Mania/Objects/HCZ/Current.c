#include "SonicMania.h"

ObjectCurrent *Current;

void Current_Update(void)
{
    RSDK_THIS(Current);
    EntityButton *button = entity->taggedButton;
    if (!button || button->field_70) {
        entity->activated = true;
        StateMachine_Run(entity->state);
    }
}

void Current_LateUpdate(void) {}

void Current_StaticUpdate(void) {}

void Current_Draw(void)
{
    RSDK_THIS(Current);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Current_Create(void *data)
{
    RSDK_THIS(Current);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        if (data) {
            entity->active        = ACTIVE_NORMAL;
            entity->inkEffect     = INK_ADD;
            entity->visible       = true;
            entity->updateRange.x = 0x2000000;
            entity->updateRange.y = 0x1000000;
            if (voidToInt(data) == 1) {
                entity->drawFX  = FX_SCALE | FX_FLIP;
                entity->scale.x = 0x400;
                entity->scale.y = 0x100;
#if RETRO_USE_PLUS
                RSDK.SetSpriteAnimation(Current->aniFrames, 1, &entity->animator, true, RSDK.Random(0, 4, &Zone->randKey));
#else
                RSDK.SetSpriteAnimation(Current->aniFrames, 1, &entity->animator, true, RSDK.Rand(0, 4));
#endif
            }
            else {
                RSDK.SetSpriteAnimation(Water->spriteIndex, 5, &entity->animator, true, 0);
                entity->alpha = 512;
            }
            entity->state = Current_State_LaundoMobile;
        }
        else {
            entity->visible       = false;
            entity->updateRange.x = entity->size.x >> 1;
            entity->updateRange.y = entity->size.y >> 1;
            entity->active        = ACTIVE_BOUNDS;
            entity->hitbox.left   = -(entity->size.x >> 17);
            entity->hitbox.right  = entity->size.x >> 17;
            entity->hitbox.top    = -(entity->size.y >> 17);
            entity->hitbox.bottom = entity->size.y >> 17;
            Current_GetTaggedButton();
            switch (entity->type) {
                case 0: entity->state = Current_State_Type0; break;
                case 1: entity->state = Current_State_Type1; break;
                case 2: entity->state = Current_State_Type2; break;
                case 3: entity->state = Current_State_Type3; break;
                case 4: entity->state = Current_State_Type4; break;
                case 5: entity->state = Current_State_Type5; break;
                case 6: entity->state = Current_State_Type6; break;
                case 7: entity->state = Current_State_Type7; break;
                default: break;
            }
        }
    }
}

void Current_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        Current->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Current.bin", SCOPE_STAGE);
    else
        Current->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(Current->aniFrames, 0, &Current->animator, true, 0);
    Current->sfxRush = RSDK.GetSFX("Stage/Rush.wav");
}

void Current_GetTaggedButton(void)
{
    RSDK_THIS(Current);

    entity->taggedButton       = NULL;
    EntityButton *taggedEntity = RSDK.GetEntityByID(RSDK.GetEntityID(entity) - 1);
    bool32 tagged              = false;

    if (entity->buttonTag > 0) {
        if (Button) {
            foreach_all(Button, button)
            {
                if (button->tag == entity->buttonTag) {
                    taggedEntity = button;
                    tagged       = true;
                    foreach_break;
                }
            }
        }

        if (PullChain && !tagged) {
            foreach_all(PullChain, chain)
            {
                if (chain->tag == entity->buttonTag) {
                    taggedEntity = (EntityButton *)chain;
                    tagged       = true;
                    foreach_break;
                }
            }
        }
    }

    if (taggedEntity) {
        if ((Button && taggedEntity->objectID == Button->objectID) || (PullChain && taggedEntity->objectID == PullChain->objectID)) {
            if (entity) {
                int32 distX = abs(entity->position.x - taggedEntity->position.x);
                int32 distY = abs(entity->position.y - taggedEntity->position.y);

                if (entity->updateRange.x < 0x800000 + distX)
                    entity->updateRange.x = 0x800000 + distX;

                if (entity->updateRange.y < 0x800000 + distY)
                    entity->updateRange.y = 0x800000 + distY;
            }
            entity->taggedButton = taggedEntity;
        }
    }
}

Vector2 Current_Unknown2(uint8 a1)
{
    RSDK_THIS(Current);
    Vector2 result;

    int x = 0;
    if (a1) {
        x = entity->position.x - (entity->size.x >> 1) - 0x200000;
        if ((RSDK_screens->position.x - 64) << 16 > x)
            x = (RSDK_screens->position.x - 64) << 16;
    }
    else {
        x = (entity->size.x >> 1) + entity->position.x + 0x200000;
        if ((RSDK_screens->position.x + 64 + RSDK_screens->width) << 16 < x)
            x = (RSDK_screens->position.x + 64 + RSDK_screens->width) << 16;
    }

    int startY = entity->size.y >> 1;

    int finalY = entity->position.y - startY - 0x200000;
    if ((RSDK_screens->position.y - 64) << 16 > finalY)
        finalY = (RSDK_screens->position.y - 64) << 16;

    int screenY = (RSDK_screens->position.y + 64 + RSDK_screens->height) << 16;
    int minY    = startY + entity->position.y + 0x200000;
    if (screenY < minY)
        minY = screenY;

    int max = (minY - finalY) >> 20;
    if (!max)
        max = 1;

    result.x = x;
    if (max <= 0)
        result.y = finalY;
    else
#if RETRO_USE_PLUS
        result.y = (RSDK.Random(0, max, &Zone->randKey) << 20) + finalY;
#else
        result.y = (RSDK.Rand(0, max) << 20) + finalY;
#endif
    return result;
}

Vector2 Current_Unknown3(uint8 a1)
{
    RSDK_THIS(Current);
    Vector2 result;

    int y = 0;
    if (a1) {
        y = entity->position.y - (entity->size.y >> 1) - 0x200000;
        if ((RSDK_screens->position.y - 64) << 16 > y)
            y = (RSDK_screens->position.y - 64) << 16;
    }
    else {
        y = (entity->size.y >> 1) + entity->position.y + 0x200000;
        if ((RSDK_screens->position.y + 64 + RSDK_screens->height) << 16 < y)
            y = (RSDK_screens->position.y + 64 + RSDK_screens->height) << 16;
    }

    int startX = entity->size.x >> 1;
    int finalX = entity->position.x - startX - 0x200000;
    if ((RSDK_screens->position.x - 64) << 16 > finalX)
        finalX = (RSDK_screens->position.x - 64) << 16;

    int screenX = (RSDK_screens->position.x + 64 + RSDK_screens->width) << 16;
    int minX    = startX + entity->position.x + 0x200000;
    if (screenX < minX)
        minX = screenX;

    int max = (minX - finalX) >> 20;
    if (!max)
        max = 1;
    if (max <= 0) {
        result.x = finalX;
        result.y = y;
    }
    else {
        result.x = finalX + (RSDK.Rand(0, max) << 20);
        result.y = y;
    }
    return result;
}

void Current_State_Type0(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if ((entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1))
            && (!entity->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & entity->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox) || player->state == Player_State_StartJumpIn) {
                        if (player->state != Player_State_StartJumpIn)
                            player->state = Player_State_Air;

                        int x = entity->position.x + ((entity->hitbox.right - 32) << 16);
                        entity->activePlayers &= ~(1 << playerID);
                        if (RSDK.CheckStageFolder("FBZ") && !player->sidekick && entity->fbzAchievement && entity->playerPositions[playerID].x >= x
                            && player->position.x < entity->position.x) {
                            API_UnlockAchievement("ACH_FBZ");
                        }
                        entity->playerPositions[playerID].x = 0;
                        entity->playerPositions[playerID].y = 0;
                    }
                }
                else if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                    entity->playerPositions[playerID].x = player->position.x;
                    entity->playerPositions[playerID].y = player->position.y;
                    entity->activePlayers |= (1 << playerID);
                }

                if ((1 << playerID) & entity->activePlayers) {
                    bool32 flag = false;
                    if (Water)
                        flag = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hit && player->state != Player_State_StartJumpIn) {
                        int anim = player->playerAnimator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.x      = -0x8000 * entity->strength;
                            player->groundVel       = player->velocity.x;
                            if (!flag) {
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
                                player->tileCollisions  = true;
                                player->nextAirState    = StateMachine_None;
                                player->nextGroundState = StateMachine_None;
                                player->state           = Current_Player_State_Type0;
                            }
                        }

                        if (player->up)
                            player->velocity.y = -0x10000;
                        else if (player->down)
                            player->velocity.y = 0x10000;
                        else
                            player->velocity.y = 0;
                    }
                }
            }
            else {
                entity->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_Unknown2(0);
            EntityCurrent *child = CREATE_ENTITY(Current, intToVoid(2), pos.x, pos.y);
            child->drawOrder     = Zone->drawOrderHigh;
            child->strength      = (entity->strength + (entity->strength >> 3)) >> 1;
            child->size.x        = entity->position.x - (entity->size.x >> 1) + 0x400000;
            child->drawOrder     = Zone->playerDrawLow;
            if (child->position.x < (entity->position.x + (entity->size.x >> 1)))
                child->alpha = 240;
        }
    }
}

void Current_State_Type1(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if ((entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1))
            && (!entity->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & entity->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox) || player->state == Player_State_StartJumpIn) {
                        if (player->state != Player_State_StartJumpIn)
                            player->state = Player_State_Air;
                        entity->activePlayers &= ~(1 << playerID);
                    }
                }
                else if (Player_CheckCollisionTouch(player, entity, &entity->hitbox))
                    entity->activePlayers |= (1 << playerID);

                if ((1 << playerID) & entity->activePlayers) {
                    bool32 flag = false;
                    if (Water)
                        flag = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hit && player->state != Player_State_StartJumpIn) {
                        int anim = player->playerAnimator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.x      = entity->strength << 15;
                            player->groundVel       = player->velocity.x;
                            if (!flag) {
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
                                player->tileCollisions  = true;
                                player->nextAirState    = StateMachine_None;
                                player->nextGroundState = StateMachine_None;
                                player->state           = Current_Player_State_Type1;
                            }
                        }

                        if (player->up)
                            player->velocity.y = -0x10000;
                        else if (player->down)
                            player->velocity.y = 0x10000;
                        else
                            player->velocity.y = 0;
                    }
                }
            }
            else {
                entity->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_Unknown2(1);
            EntityCurrent *child = CREATE_ENTITY(Current, intToVoid(2), pos.x, pos.y);
            child->drawOrder     = Zone->drawOrderHigh;
            child->strength      = (entity->strength + (entity->strength >> 3)) >> 1;
            child->size.x        = (entity->size.x >> 1) + entity->position.x - 0x400000;
            child->type          = 1;
            child->drawOrder     = Zone->playerDrawLow;
            if (child->position.x > (entity->position.x - (entity->size.x >> 1)))
                child->alpha = 240;
        }
    }
}

void Current_State_Type2(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if ((entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1))
            && (!entity->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & entity->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox) || player->state == Player_State_StartJumpIn) {
                        if (player->state != Player_State_StartJumpIn)
                            player->state = Player_State_Air;
                        entity->activePlayers &= ~(1 << playerID);
                    }
                }
                else if (Player_CheckCollisionTouch(player, entity, &entity->hitbox))
                    entity->activePlayers |= (1 << playerID);

                if ((1 << playerID) & entity->activePlayers) {
                    bool32 flag = false;
                    if (Water)
                        flag = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hit && player->state != Player_State_StartJumpIn) {
                        int anim = player->playerAnimator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.y      = -0x8000 * entity->strength;
                            if (!flag) {
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
                                player->tileCollisions  = true;
                                player->nextAirState    = StateMachine_None;
                                player->nextGroundState = StateMachine_None;
                                player->state           = Current_Player_State_Type2;
                            }
                        }
                        if (player->left) {
                            player->velocity.x = -0x10000;
                            player->groundVel  = -0x10000;
                        }
                        else if (player->right) {
                            player->velocity.x = 0x10000;
                            player->groundVel  = 0x10000;
                        }
                        else {
                            player->velocity.x = 0;
                        }
                    }
                }
            }
            else {
                entity->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_Unknown3(0);
            EntityCurrent *child = CREATE_ENTITY(Current, intToVoid(2), pos.x, pos.y);
            child->drawOrder     = Zone->drawOrderHigh;
            child->strength      = (entity->strength + (entity->strength >> 3)) >> 1;
            child->size.y        = (entity->position.y - (entity->size.y >> 1)) + 0x400000;
            child->type          = 2;
            child->drawOrder     = Zone->playerDrawLow;
            if (child->position.y < (entity->position.y + (entity->size.y >> 1)))
                child->alpha = 240;
        }
    }
}

void Current_State_Type3(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if ((entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1))
            && (!entity->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & entity->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox) || player->state == Player_State_StartJumpIn) {
                        if (player->state != Player_State_StartJumpIn)
                            player->state = Player_State_Air;
                        entity->activePlayers &= ~(1 << playerID);
                    }
                }
                else if (Player_CheckCollisionTouch(player, entity, &entity->hitbox))
                    entity->activePlayers |= (1 << playerID);

                if ((1 << playerID) & entity->activePlayers) {
                    bool32 flag = false;
                    if (Water)
                        flag = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hit && player->state != Player_State_StartJumpIn) {
                        int anim = player->playerAnimator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.y      = entity->strength << 15;
                            if (!flag) {
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
                                player->tileCollisions  = true;
                                player->nextAirState    = StateMachine_None;
                                player->nextGroundState = StateMachine_None;
                                player->state           = Current_Player_State_Type3;
                            }
                        }
                        if (player->left) {
                            player->velocity.x = -0x10000;
                            player->groundVel  = -0x10000;
                        }
                        else if (player->right) {
                            player->velocity.x = 0x10000;
                            player->groundVel  = 0x10000;
                        }
                        else {
                            player->velocity.x = 0;
                        }
                    }
                }
            }
            else {
                entity->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_Unknown3(1);
            EntityCurrent *child = CREATE_ENTITY(Current, intToVoid(2), pos.x, pos.y);
            child->drawOrder     = Zone->drawOrderHigh;
            child->strength      = (entity->strength + (entity->strength >> 3)) >> 1;
            child->size.y        = (entity->size.y >> 1) + entity->position.y - 0x400000;
            child->type          = 3;
            child->drawOrder     = Zone->playerDrawLow;
            if (child->position.y > (entity->position.y - (entity->size.y >> 1)))
                child->alpha = 240;
        }
    }
}

void Current_State_Type4(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1))
            && (!entity->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                int anim = player->playerAnimator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING)
                    player->position.x += -0x8000 * entity->strength;
            }
        }
    }

    if (!(Zone->timer & 7)) {
        Vector2 pos          = Current_Unknown2(0);
        EntityCurrent *child = CREATE_ENTITY(Current, intToVoid(1), pos.x, pos.y);
        child->drawOrder     = Zone->drawOrderHigh;
        child->strength      = 4 * entity->strength + RSDK.Rand(1 - entity->strength, 5);
        child->size.x        = entity->position.x - (entity->size.x >> 1) + 0x400000;
        if (child->position.x < (entity->position.x + (entity->size.x >> 1)))
            child->alpha = 240;
    }
}

void Current_State_Type5(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1))
            && (!entity->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                int anim = player->playerAnimator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING)
                    player->position.x += entity->strength << 15;
            }
        }
    }
}

void Current_State_Type6(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1))
            && (!entity->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                int anim = player->playerAnimator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING && !player->onGround)
                    player->position.y += -0x8000 * entity->strength;
            }
        }
    }
}

void Current_State_Type7(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1))
            && (!entity->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                int anim = player->playerAnimator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING && !player->onGround)
                    player->position.y += entity->strength << 15;
            }
        }
    }
}

void Current_State_LaundoMobile(void)
{
    RSDK_THIS(Current);
    RSDK.ProcessAnimation(&entity->animator);
    switch (entity->type) {
        case 0:
            entity->position.x -= entity->strength << 17;
            if (entity->position.x < entity->size.x) {
                entity->alpha -= 8;
            }
            else {
                if (entity->alpha < 240)
                    entity->alpha += 8;
            }
            break;
        case 1:
            entity->position.x += entity->strength << 17;
            if (entity->position.x >= entity->size.x) {
                if (entity->alpha < 240)
                    entity->alpha += 8;
            }
            else {
                entity->alpha -= 8;
            }
            break;
        case 2:
            entity->position.y -= entity->strength << 17;
            if (entity->position.y >= entity->size.y) {
                if (entity->alpha < 240)
                    entity->alpha += 8;
            }
            else {
                entity->alpha -= 8;
            }
            break;
        case 3:
            entity->position.y += entity->strength << 17;
            if (entity->position.y >= entity->size.y) {
                if (entity->alpha < 240)
                    entity->alpha += 8;
            }
            else {
                entity->alpha -= 8;
            }
            break;
        default: break;
    }
    if (Water && entity->position.y < Water->waterLevel)
        destroyEntity(entity);
    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
    if (!entity->alpha)
        destroyEntity(entity);
}

void Current_Player_State_Type0(void)
{
    RSDK_THIS(Player);
    Hitbox *hitbox = Player_GetHitbox(entity);
    RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_ROOF, entity->collisionPlane, hitbox->left << 16, hitbox->top << 16, true);
    RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, hitbox->left << 16, hitbox->bottom << 16, true);
}

void Current_Player_State_Type1(void)
{
    RSDK_THIS(Player);
    Hitbox *hitbox = Player_GetHitbox(entity);
    RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_ROOF, entity->collisionPlane, hitbox->right << 16, hitbox->top << 16, true);
    RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, hitbox->right << 16, hitbox->bottom << 16, true);
}

void Current_Player_State_Type2(void)
{
    RSDK_THIS(Player);
    Hitbox *hitbox = Player_GetHitbox(entity);
    RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_RWALL, entity->collisionPlane, hitbox->left << 16, hitbox->top << 16, true);
    RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_LWALL, entity->collisionPlane, hitbox->right << 16, hitbox->top << 16, true);
}

void Current_Player_State_Type3(void)
{
    RSDK_THIS(Player);
    Hitbox *hitbox = Player_GetHitbox(entity);
    RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_RWALL, entity->collisionPlane, hitbox->left << 16, hitbox->bottom << 16, true);
    RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_LWALL, entity->collisionPlane, hitbox->right << 16, hitbox->bottom << 16, true);
}

#if RETRO_INCLUDE_EDITOR
void Current_EditorDraw(void)
{
    RSDK_THIS(Current);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x >> 1;
    drawPos.y -= entity->size.y >> 1;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + entity->size.x, drawPos.y - 0x10000, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, entity->size.y + drawPos.y, drawPos.x + entity->size.x, entity->size.y + drawPos.y, 0xFFFF00, 0, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + entity->size.x, drawPos.y - 0x10000, drawPos.x + entity->size.x, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE,
                  false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&Current->animator, &drawPos, false);

    drawPos.x += entity->size.x;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&Current->animator, &drawPos, false);

    drawPos.y += entity->size.y;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&Current->animator, &drawPos, false);

    drawPos.x -= entity->size.x;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&Current->animator, &drawPos, false);
}

void Current_EditorLoad(void)
{
    Current->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(Current->aniFrames, 0, &Current->animator, true, 0);

    RSDK_ACTIVE_VAR(Current, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
}
#endif

void Current_Serialize(void)
{
    RSDK_EDITABLE_VAR(Current, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Current, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(Current, VAR_UINT8, strength);
    RSDK_EDITABLE_VAR(Current, VAR_ENUM, buttonTag);
    RSDK_EDITABLE_VAR(Current, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(Current, VAR_BOOL, waterOnly);
    RSDK_EDITABLE_VAR(Current, VAR_BOOL, fbzAchievement);
}
