#include "SonicMania.h"

ObjectSizeLaser *SizeLaser;

void SizeLaser_Update(void)
{
    RSDK_THIS(SizeLaser);
    entity->animator1.frameID = (Zone->timer >> 2) & 1;
    entity->animator2.frameID = (Zone->timer >> 2) & 1;
    StateMachine_Run(entity->state);
}

void SizeLaser_LateUpdate(void)
{

}

void SizeLaser_StaticUpdate(void)
{

}

void SizeLaser_Draw(void)
{
    RSDK_THIS(SizeLaser);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    if (entity->state == SizeLaser_Unknown10)
        RSDK.DrawSprite(&entity->animator2, &entity->storedPos, false);
}

void SizeLaser_Create(void* data)
{
    RSDK_THIS(SizeLaser);
    
    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        int32 type              = (int32)(size_t)data;
        entity->visible       = true;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (!type) {
            entity->active    = ACTIVE_BOUNDS;
            entity->drawOrder = Zone->drawOrderLow + 1;

            entity->state = SizeLaser_Unknown8;
            switch (entity->orientation) {
                case 0:
                    entity->direction = FLIP_NONE;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type, &entity->animator1, true, 0);
                    break;
                case 1:
                    entity->direction = FLIP_NONE;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 1, &entity->animator1, true, 0);
                    break;
                case 2:
                    entity->direction = FLIP_X;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 1, &entity->animator1, true, 0);
                    break;
            }
        }
        else {
            entity->active      = ACTIVE_NORMAL;
            entity->drawOrder   = Zone->drawOrderLow;
            entity->orientation = (type - 1) >> 2;
            entity->type        = (type - 1) & 1;

            entity->state = SizeLaser_Unknown9;
            switch (entity->orientation) {
                default:
                case 0:
                    entity->velocity.y = 0x40000;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * (entity->type + 2), &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * (entity->type + 4), &entity->animator2, true, 0);
                    break;
                case 1:
                    entity->velocity.x = 0x40000;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 5, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 9, &entity->animator2, true, 0);
                    break;
                case 2:
                    entity->direction  = FLIP_X;
                    entity->velocity.x = -0x40000;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 5, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 9, &entity->animator2, true, 0);
                    break;
            }
        }
    }
}

void SizeLaser_StageLoad(void)
{
    SizeLaser->spriteIndex = RSDK.LoadSpriteAnimation("MMZ/SizeLaser.bin", SCOPE_STAGE);
    switch (globals->playerID & 0xFF) {
        case ID_SONIC: SizeLaser->sonicIndex = RSDK.LoadSpriteAnimation("Players/ChibiSonic.bin", SCOPE_STAGE); break;
        case ID_TAILS:
            SizeLaser->tailsIndex      = RSDK.LoadSpriteAnimation("Players/ChibiTails.bin", SCOPE_STAGE);
            SizeLaser->tailSpriteIndex = RSDK.LoadSpriteAnimation("Players/CTailSprite.bin", SCOPE_STAGE);
            break;
        case ID_KNUCKLES: SizeLaser->knuxIndex = RSDK.LoadSpriteAnimation("Players/ChibiKnux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
        case ID_MIGHTY: SizeLaser->mightyIndex = RSDK.LoadSpriteAnimation("Players/ChibiMighty.bin", SCOPE_STAGE); break;
        case ID_RAY: SizeLaser->rayIndex = RSDK.LoadSpriteAnimation("Players/ChibiRay.bin", SCOPE_STAGE); break;
#endif
        default: break;
    }

    switch ((globals->playerID >> 8) & 0xFF) {
        case ID_SONIC: SizeLaser->sonicIndex = RSDK.LoadSpriteAnimation("Players/ChibiSonic.bin", SCOPE_STAGE); break;
        case ID_TAILS:
            SizeLaser->tailsIndex      = RSDK.LoadSpriteAnimation("Players/ChibiTails.bin", SCOPE_STAGE);
            SizeLaser->tailSpriteIndex = RSDK.LoadSpriteAnimation("Players/CTailSprite.bin", SCOPE_STAGE);
            break;
        case ID_KNUCKLES: SizeLaser->knuxIndex = RSDK.LoadSpriteAnimation("Players/ChibiKnux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
        case ID_MIGHTY: SizeLaser->mightyIndex = RSDK.LoadSpriteAnimation("Players/ChibiMighty.bin", SCOPE_STAGE); break;
        case ID_RAY: SizeLaser->rayIndex = RSDK.LoadSpriteAnimation("Players/ChibiRay.bin", SCOPE_STAGE); break;
#endif
        default: break;
    }

    SizeLaser->hitbox.left   = -4;
    SizeLaser->hitbox.top    = -4;
    SizeLaser->hitbox.right  = 4;
    SizeLaser->hitbox.bottom = 4;
    SizeLaser->sfxShrink2   = RSDK.GetSFX("MMZ/Shrink2.wav");
    SizeLaser->sfxGrow2     = RSDK.GetSFX("MMZ/Grow2.wav");

    Soundboard_LoadSFX("MMZ/SizeLaser.wav", true, SizeLaser_SizeChangeSFXCheck, NULL);
}

bool32 SizeLaser_SizeChangeSFXCheck(void)
{
    int32 cnt = 0;
    foreach_active(SizeLaser, entity)
    {
        if (entity->activeScreens)
            ++cnt;
    }
    return cnt > 0;
}

void SizeLaser_SetP2State(EntityPlayer *player, bool32 chibiFlag)
{
    if (chibiFlag) {
        switch (player->characterID) {
            case ID_TAILS:
                player->spriteIndex     = SizeLaser->tailsIndex;
                player->tailSpriteIndex = SizeLaser->tailSpriteIndex;
                player->isChibi         = chibiFlag;
                break;
            case ID_KNUCKLES:
                player->spriteIndex     = SizeLaser->knuxIndex;
                player->tailSpriteIndex = -1;
                player->isChibi         = chibiFlag;
                break;
#if RETRO_USE_PLUS
            case ID_MIGHTY:
                player->spriteIndex     = SizeLaser->mightyIndex;
                player->tailSpriteIndex = -1;
                player->isChibi         = chibiFlag;
                break;
            case ID_RAY:
                player->spriteIndex     = SizeLaser->rayIndex;
                player->tailSpriteIndex = -1;
                player->isChibi         = chibiFlag;
                break;
#endif
            default:
                player->spriteIndex     = SizeLaser->sonicIndex;
                player->tailSpriteIndex = -1;
                player->isChibi         = chibiFlag;
                break;
        }
    }
    else {
        switch (player->characterID) {
            case ID_TAILS:
                player->spriteIndex     = Player->tailsSpriteIndex;
                player->tailSpriteIndex = Player->tailsSpriteIndex;
                player->isChibi         = false;
                break;
            case ID_KNUCKLES:
                player->spriteIndex     = Player->knuxSpriteIndex;
                player->tailSpriteIndex = -1;
                player->isChibi         = chibiFlag;
                break;
#if RETRO_USE_PLUS
            case ID_MIGHTY:
                player->spriteIndex     = Player->mightySpriteIndex;
                player->tailSpriteIndex = -1;
                player->isChibi         = chibiFlag;
                break;
            case ID_RAY:
                player->spriteIndex     = Player->raySpriteIndex;
                player->tailSpriteIndex = -1;
                player->isChibi         = chibiFlag;
                break;
#endif
            default:
                player->spriteIndex     = Player->sonicSpriteIndex;
                player->tailSpriteIndex = -1;
                player->isChibi         = chibiFlag;
                break;
        }
    }
}

void SizeLaser_P2JumpInResize(void)
{
    EntityPlayer *entity  = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot, Player);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->position.x = player1->position.x;
    entity->position.y = player1->position.y;

    entity->position.x += entity->abilityValues[0];
    entity->position.y += entity->abilityValues[1];
    if (entity->abilityValues[0] <= 0) {
        if (entity->abilityValues[0] < 0) {
            entity->abilityValues[0] -= (entity->abilityValues[0] >> 4);
            if (entity->abilityValues[0] > 0)
                entity->abilityValues[0] = 0;
        }
    }
    else {
        entity->abilityValues[0] -= (entity->abilityValues[0] >> 4);
        if (entity->abilityValues[0] < 0)
            entity->abilityValues[0] = 0;
    }

    if (entity->abilityValues[1] <= 0) {
        if (entity->abilityValues[1] < 0) {
            entity->abilityValues[1] -= (entity->abilityValues[0] >> 4);
            if (entity->abilityValues[1] > 0)
                entity->abilityValues[1] = 0;
        }
    }
    else {
        entity->abilityValues[1] -= (entity->abilityValues[0] >> 4);
        if (entity->abilityValues[1] < 0)
            entity->abilityValues[1] = 0;
    }

    if (player1->state == SizeLaser_P2JumpInShrink) {
        if (player1->state == SizeLaser_P2JumpInGrow || !player1->isChibi) {
            entity->state       = SizeLaser_P2JumpInGrow;
            entity->interaction = false;
        }
    }
    else if (player1->isChibi) {
        entity->state       = SizeLaser_P2JumpInShrink;
        entity->interaction = false;
    }
}

void SizeLaser_P2JumpInGrow(void)
{
    EntityPlayer *entity = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot, Player);
    StateMachine(state)  = entity->abilityPtrs[0];
    StateMachine_Run(state);

    if (entity->scale.x >= 0x200) {
        entity->drawFX &= ~FX_SCALE;
        entity->isChibi     = false;
        entity->scale.x     = 0x200;
        entity->scale.y     = 0x200;
        entity->interaction = true;
        entity->state       = Player_State_Air;
        Player_ChangePhysicsState(entity);
        if (entity->characterID == ID_TAILS)
            entity->cameraOffset = 0;
        else
            entity->cameraOffset = 0x50000;
    }
    else {
        entity->state        = SizeLaser_P2JumpInGrow;
        entity->scale.x += 6;
        entity->scale.y = entity->scale.x;
    }
}

void SizeLaser_P2JumpInShrink(void)
{
    EntityPlayer *entity = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot, Player);
    StateMachine(state)  = entity->abilityPtrs[0];
    StateMachine_Run(state);

    if (entity->scale.x <= 0x140) {
        switch (entity->characterID) {
            case ID_TAILS:
                entity->spriteIndex = SizeLaser->tailsIndex;
                entity->tailSpriteIndex = SizeLaser->tailSpriteIndex;
                break;
            case ID_KNUCKLES:
                entity->spriteIndex = SizeLaser->knuxIndex;
                entity->tailSpriteIndex = -1;
                break;
#if RETRO_USE_PLUS
            case ID_MIGHTY:
                entity->spriteIndex = SizeLaser->mightyIndex;
                entity->tailSpriteIndex = -1;
                break;
            case ID_RAY:
                entity->spriteIndex     = SizeLaser->rayIndex;
                entity->tailSpriteIndex = -1;
                break;
#endif
            default:
                entity->spriteIndex = SizeLaser->sonicIndex;
                entity->tailSpriteIndex = -1;
                break;
        }
        entity->cameraOffset = 0x40000;
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_HURT, &entity->playerAnimator, false, 0);
        entity->isChibi = true;
        entity->drawFX &= ~FX_SCALE;
        entity->scale.x     = 0x200;
        entity->scale.y     = 0x200;
        entity->interaction = true;
        entity->state       = Player_State_Air;
        Player_ChangePhysicsState(entity);
    }
    else {
        entity->state        = SizeLaser_P2JumpInShrink;
        entity->scale.x -= 6;
        entity->scale.y = entity->scale.x;
    }
}

void SizeLaser_PlayerState_Grow(void)
{
    EntityPlayer *entity = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot, Player);

    StateMachine(state) = entity->abilityPtrs[0];
    StateMachine_Run(state);

    if (entity->scale.x >= 0x300) {
        entity->scale.x     = 0x300;
        entity->scale.y     = 0x300;
        entity->interaction = true;
        entity->state            = Player_State_Air;
        Player_ChangePhysicsState(entity);
    }
    else {
        entity->state = SizeLaser_PlayerState_Grow;
        entity->scale.x += 8;
        entity->scale.y = entity->scale.x;
    }
}

void SizeLaser_Unknown3(void)
{
    RSDK_THIS(SizeLaser);
    int32 entityX = entity->position.x;
    int32 entityY = entity->position.y;

    int32 tx1[2];
    int32 tx2[2];
    int32 ty1[2];
    int32 ty2[2];

    switch (entity->orientation) {
        case 0:
            tx1[0] = entityX - 0x80000;
            tx1[1] = entityX + 0x80000;
            tx2[0] = entityY - 0x200000;
            tx2[1] = entityY - 0x200000;
            ty1[0] = entityX - 0x80000;
            ty1[1] = entityX + 0x80000;
            ty2[1] = (entity->extend << 16) + entityY;
            ty2[0] = (entity->extend << 16) + entityY;
            break;
        case 1:
            tx1[0] = entityX - 0x200000;
            tx1[1] = entityX - 0x200000;
            tx2[0] = entityY - 0x80000;
            tx2[1] = entityY + 0x80000;
            ty1[0] = (entity->extend << 16) + entityX;
            ty1[1] = (entity->extend << 16) + entityX;
            ty2[1] = entityY + 0x80000;
            ty2[0] = entityY - 0x80000;
            break;
        case 2:
            tx1[0] = entityX + 0x200000;
            tx1[1] = entityX;
            tx2[0] = entityY - 0x80000;
            tx2[1] = entityY + 0x80000;
            ty1[0] = entityX - 0x80000;
            ty1[1] = entityX - (entity->extend << 16);
            ty2[1] = entityY + 0x80000;
            ty2[0] = entityY - 0x80000;
            break;
        default: // what
            tx1[0] = entityX;
            tx1[1] = entityX;
            tx2[0] = entityY;
            tx2[1] = entityX;
            ty1[0] = entityX;
            ty1[1] = entityX;
            ty2[0] = entityY;
            ty2[1] = entityX;
            break;
    }

    foreach_active(Player, player) {
        int32 pID = RSDK.GetEntityID(player);
        if (MathHelpers_Unknown12(SizeLaser->playerPositions[pID].x, SizeLaser->playerPositions[pID].y, player->position.x, player->position.y,
                                  tx1[0], tx2[0], ty1[0], ty2[0])
            || MathHelpers_Unknown12(SizeLaser->playerPositions[pID].x, SizeLaser->playerPositions[pID].y, player->position.x, player->position.y,
                                     tx1[1], tx2[1], ty1[1], ty2[1])) {

            if (entity->type) {
                if (player->state == SizeLaser_P2JumpInShrink || player->state == SizeLaser_P2JumpInGrow || !player->isChibi) {
                    if (player->state != SizeLaser_PlayerState_Grow && player->scale.x > 0x400) {
                        player->onGround        = false;
                        player->interaction     = false;
                        player->velocity.y      = -0x40000;
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        if (!player->direction)
                            player->velocity.x = -0x20000;
                        else
                            player->velocity.x = 0x20000;
                        player->drawFX |= FX_SCALE;
                        player->scale.x        = 0x200;
                        player->scale.y        = 0x200;
                        player->tileCollisions = true;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, false, 0);
                        player->abilityPtrs[0] = Player_State_Hit;
                        player->state          = SizeLaser_PlayerState_Grow;
                    }
                }
                else {
                    player->onGround        = false;
                    player->interaction     = false;
                    player->velocity.y      = -0x40000;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    if (!player->direction)
                        player->velocity.x = -0x20000;
                    else
                        player->velocity.x = 0x20000;
                    player->drawFX |= FX_SCALE;
                    player->scale.x = 0x140;
                    player->scale.y = 0x140;
                    switch (player->characterID) {
                        case ID_TAILS:
                            player->spriteIndex     = Player->tailsSpriteIndex;
                            player->tailSpriteIndex = Player->tailsTailsSpriteIndex;
                            break;
                        case ID_KNUCKLES:
                            player->spriteIndex     = Player->knuxSpriteIndex;
                            player->tailSpriteIndex = -1;
                            break;
#if RETRO_USE_PLUS
                        case ID_MIGHTY:
                            player->spriteIndex     = Player->mightySpriteIndex;
                            player->tailSpriteIndex = -1;
                            break;
                        case ID_RAY:
                            player->spriteIndex     = Player->raySpriteIndex;
                            player->tailSpriteIndex = -1;
                            break;
#endif
                        default:
                            if (player->superState == SUPERSTATE_SUPER)
                                player->spriteIndex = Player->superSpriteIndex;
                            else
                                player->spriteIndex = Player->sonicSpriteIndex;
                            player->tailSpriteIndex = -1;
                            break;
                    }
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, false, 0);
                    RSDK.PlaySfx(SizeLaser->sfxGrow2, false, 255);
                    player->tileCollisions = true;
                    player->abilityPtrs[0] = Player_State_Hit;
                    player->state          = SizeLaser_P2JumpInGrow;
                }
            }
            else if (player->state != SizeLaser_P2JumpInShrink && player->state != SizeLaser_P2JumpInGrow && !player->isChibi) {
                player->onGround        = false;
                player->interaction     = false;
                player->velocity.y      = -0x40000;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
                if (!player->direction)
                    player->velocity.x = -0x20000;
                else
                    player->velocity.x = 0x20000;
                player->drawFX |= FX_SCALE;
                player->scale.x = 0x200;
                player->scale.y = 0x200;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, false, 0);
                RSDK.PlaySfx(SizeLaser->sfxShrink2, 0, 255);
                player->tileCollisions = true;
                player->abilityPtrs[0] = Player_State_Hit;
                player->state          = SizeLaser_P2JumpInShrink;
            }
        }
    }
}

void SizeLaser_Unknown8(void)
{
    RSDK_THIS(SizeLaser);

    if (entity->timer < 24 && !(entity->timer & 1))
        RSDK.CreateEntity(SizeLaser->objectID, intToVoid(entity->type + 4 * entity->orientation + 1), entity->position.x,
                          entity->position.y);
    SizeLaser_Unknown3();
    entity->timer = (entity->timer + 1) & 0x1F;
}

void SizeLaser_Unknown9(void)
{
    RSDK_THIS(SizeLaser);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    bool32 collided = false;
    switch (entity->orientation) {
        case 0: collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x40000, false); break;
        case 1: collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x40000, 0, false); break;
        case 2: collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x40000, 0, false); break;
    }

    if (collided) {
        entity->storedPos.x = entity->position.x;
        entity->storedPos.y = entity->position.y;
        entity->state       = SizeLaser_Unknown10;
    }
}


void SizeLaser_Unknown10(void)
{
    RSDK_THIS(SizeLaser);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (++entity->timer == 4)
        destroyEntity(entity);
}

void SizeLaser_EditorDraw(void)
{

}

void SizeLaser_EditorLoad(void)
{

}

void SizeLaser_Serialize(void)
{
    RSDK_EDITABLE_VAR(SizeLaser, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(SizeLaser, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(SizeLaser, VAR_ENUM, extend);
}

