// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SizeLaser Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSizeLaser *SizeLaser;

void SizeLaser_Update(void)
{
    RSDK_THIS(SizeLaser);

    self->mainAnimator.frameID   = (Zone->timer >> 2) & 1;
    self->impactAnimator.frameID = (Zone->timer >> 2) & 1;

    StateMachine_Run(self->state);
}

void SizeLaser_LateUpdate(void) {}

void SizeLaser_StaticUpdate(void)
{
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        SizeLaser->playerPositions[playerID].x = player->position.x;
        SizeLaser->playerPositions[playerID].y = player->position.y;

        if (player->scale.x > 0x200) {

            Hitbox *playerHitbox                       = Player_GetHitbox(player);
            SizeLaser->playerOuterBox[playerID].left   = (player->scale.x * playerHitbox->left) >> 9;
            SizeLaser->playerOuterBox[playerID].top    = (player->scale.x * playerHitbox->top) >> 9;
            SizeLaser->playerOuterBox[playerID].right  = (player->scale.x * playerHitbox->right) >> 9;
            SizeLaser->playerOuterBox[playerID].bottom = (player->scale.x * playerHitbox->bottom) >> 9;

            playerHitbox                               = Player_GetAltHitbox(player);
            SizeLaser->playerInnerBox[playerID].left   = (player->scale.x * playerHitbox->left) >> 9;
            SizeLaser->playerInnerBox[playerID].top    = (player->scale.x * playerHitbox->top) >> 9;
            SizeLaser->playerInnerBox[playerID].right  = (player->scale.x * playerHitbox->right) >> 9;
            SizeLaser->playerInnerBox[playerID].bottom = (player->scale.x * playerHitbox->bottom) >> 9;

            player->outerbox = &SizeLaser->playerOuterBox[playerID];
            player->innerbox = &SizeLaser->playerInnerBox[playerID];
        }
    }
}

void SizeLaser_Draw(void)
{
    RSDK_THIS(SizeLaser);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    if (self->state == SizeLaser_State_Impact)
        RSDK.DrawSprite(&self->impactAnimator, &self->impactPos, false);
}

void SizeLaser_Create(void *data)
{
    RSDK_THIS(SizeLaser);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        int32 type          = VOID_TO_INT(data);
        self->visible       = true;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        if (!type) {
            self->active    = ACTIVE_BOUNDS;
            self->drawGroup = Zone->objectDrawGroup[0] + 1;

            self->state = SizeLaser_State_Emitter;
            switch (self->orientation) {
                case SIZELASER_ORIENTATION_DOWN:
                    self->direction = FLIP_NONE;
                    RSDK.SetSpriteAnimation(SizeLaser->aniFrames, 2 * self->type, &self->mainAnimator, true, 0);
                    break;

                case SIZELASER_ORIENTATION_RIGHT:
                case SIZELASER_ORIENTATION_LEFT:
                    self->direction = self->orientation == SIZELASER_ORIENTATION_LEFT ? FLIP_X : FLIP_NONE;
                    RSDK.SetSpriteAnimation(SizeLaser->aniFrames, 2 * self->type + 1, &self->mainAnimator, true, 0);
                    break;
            }
        }
        else {
            type--;
            self->active      = ACTIVE_NORMAL;
            self->drawGroup   = Zone->objectDrawGroup[0];
            self->orientation = type >> 2;
            self->type        = type & 1;

            self->state = SizeLaser_State_Laser;
            switch (self->orientation) {
                default:
                case SIZELASER_ORIENTATION_DOWN:
                    self->velocity.y = 0x40000;

                    RSDK.SetSpriteAnimation(SizeLaser->aniFrames, 2 * (self->type + 2), &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(SizeLaser->aniFrames, 2 * (self->type + 4), &self->impactAnimator, true, 0);
                    break;

                case SIZELASER_ORIENTATION_RIGHT:
                case SIZELASER_ORIENTATION_LEFT:
                    self->direction  = self->orientation == SIZELASER_ORIENTATION_LEFT ? FLIP_X : FLIP_NONE;
                    self->velocity.x = self->orientation == SIZELASER_ORIENTATION_LEFT ? -0x40000 : 0x40000;

                    RSDK.SetSpriteAnimation(SizeLaser->aniFrames, 2 * self->type + 5, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(SizeLaser->aniFrames, 2 * self->type + 9, &self->impactAnimator, true, 0);
                    break;
            }
        }
    }
}

void SizeLaser_StageLoad(void)
{
    SizeLaser->aniFrames = RSDK.LoadSpriteAnimation("MMZ/SizeLaser.bin", SCOPE_STAGE);

    switch (GET_CHARACTER_ID(1)) {
        case ID_SONIC: SizeLaser->sonicFrames = RSDK.LoadSpriteAnimation("Players/ChibiSonic.bin", SCOPE_STAGE); break;

        case ID_TAILS:
            SizeLaser->tailsFrames = RSDK.LoadSpriteAnimation("Players/ChibiTails.bin", SCOPE_STAGE);
            SizeLaser->tailFrames  = RSDK.LoadSpriteAnimation("Players/CTailSprite.bin", SCOPE_STAGE);
            break;

        case ID_KNUCKLES: SizeLaser->knuxFrames = RSDK.LoadSpriteAnimation("Players/ChibiKnux.bin", SCOPE_STAGE); break;

#if MANIA_USE_PLUS
        case ID_MIGHTY: SizeLaser->mightyFrames = RSDK.LoadSpriteAnimation("Players/ChibiMighty.bin", SCOPE_STAGE); break;

        case ID_RAY: SizeLaser->rayFrames = RSDK.LoadSpriteAnimation("Players/ChibiRay.bin", SCOPE_STAGE); break;
#endif

        default: break;
    }

    switch (GET_CHARACTER_ID(2)) {
        case ID_SONIC: SizeLaser->sonicFrames = RSDK.LoadSpriteAnimation("Players/ChibiSonic.bin", SCOPE_STAGE); break;

        case ID_TAILS:
            SizeLaser->tailsFrames = RSDK.LoadSpriteAnimation("Players/ChibiTails.bin", SCOPE_STAGE);
            SizeLaser->tailFrames  = RSDK.LoadSpriteAnimation("Players/CTailSprite.bin", SCOPE_STAGE);
            break;

        case ID_KNUCKLES: SizeLaser->knuxFrames = RSDK.LoadSpriteAnimation("Players/ChibiKnux.bin", SCOPE_STAGE); break;

#if MANIA_USE_PLUS
        case ID_MIGHTY: SizeLaser->mightyFrames = RSDK.LoadSpriteAnimation("Players/ChibiMighty.bin", SCOPE_STAGE); break;

        case ID_RAY: SizeLaser->rayFrames = RSDK.LoadSpriteAnimation("Players/ChibiRay.bin", SCOPE_STAGE); break;
#endif

        default: break;
    }

    SizeLaser->hitbox.left   = -4;
    SizeLaser->hitbox.top    = -4;
    SizeLaser->hitbox.right  = 4;
    SizeLaser->hitbox.bottom = 4;

    SizeLaser->sfxShrink2 = RSDK.GetSfx("MMZ/Shrink2.wav");
    SizeLaser->sfxGrow2   = RSDK.GetSfx("MMZ/Grow2.wav");

    Soundboard_LoadSfx("MMZ/SizeLaser.wav", true, SizeLaser_SfxCheck_SizeLaser, StateMachine_None);
}

bool32 SizeLaser_SfxCheck_SizeLaser(void)
{
    int32 activeCount = 0;

    foreach_active(SizeLaser, entity)
    {
        if (entity->onScreen)
            ++activeCount;
    }

    return activeCount > 0;
}

void SizeLaser_SetPlayerSize(EntityPlayer *player, bool32 isChibi)
{
    if (isChibi) {
        switch (player->characterID) {
            default:
            case ID_SONIC:
                player->aniFrames  = SizeLaser->sonicFrames;
                player->tailFrames = -1;
                player->isChibi    = isChibi;
                break;

            case ID_TAILS:
                player->aniFrames  = SizeLaser->tailsFrames;
                player->tailFrames = SizeLaser->tailFrames;
                player->isChibi    = isChibi;
                break;

            case ID_KNUCKLES:
                player->aniFrames  = SizeLaser->knuxFrames;
                player->tailFrames = -1;
                player->isChibi    = isChibi;
                break;

#if MANIA_USE_PLUS
            case ID_MIGHTY:
                player->aniFrames  = SizeLaser->mightyFrames;
                player->tailFrames = -1;
                player->isChibi    = isChibi;
                break;

            case ID_RAY:
                player->aniFrames  = SizeLaser->rayFrames;
                player->tailFrames = -1;
                player->isChibi    = isChibi;
                break;
#endif
        }
    }
    else {
        switch (player->characterID) {
            default:
            case ID_SONIC:
                player->aniFrames  = Player->sonicFrames;
                player->tailFrames = -1;
                player->isChibi    = isChibi;
                break;

            case ID_TAILS:
                player->aniFrames  = Player->tailsFrames;
                player->tailFrames = Player->tailsTailsFrames;
                player->isChibi    = false;
                break;

            case ID_KNUCKLES:
                player->aniFrames  = Player->knuxFrames;
                player->tailFrames = -1;
                player->isChibi    = isChibi;
                break;

#if MANIA_USE_PLUS
            case ID_MIGHTY:
                player->aniFrames  = Player->mightyFrames;
                player->tailFrames = -1;
                player->isChibi    = isChibi;
                break;

            case ID_RAY:
                player->aniFrames  = Player->rayFrames;
                player->tailFrames = -1;
                player->isChibi    = isChibi;
                break;
#endif
        }
    }
}

// used to have a sidekick resize to match the leader's size
void SizeLaser_PlayerState_Resize(void)
{
    // ???
    EntityPlayer *self = RSDK_GET_ENTITY(SceneInfo->entitySlot, Player);

    EntityPlayer *leader = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x = leader->position.x;
    self->position.y = leader->position.y;

    self->position.x += self->abilityValues[0];
    self->position.y += self->abilityValues[1];

    if (self->abilityValues[0] <= 0) {
        if (self->abilityValues[0] < 0) {
            self->abilityValues[0] -= self->abilityValues[0] >> 4;

            if (self->abilityValues[0] > 0)
                self->abilityValues[0] = 0;
        }
    }
    else {
        self->abilityValues[0] -= self->abilityValues[0] >> 4;

        if (self->abilityValues[0] < 0)
            self->abilityValues[0] = 0;
    }

    if (self->abilityValues[1] <= 0) {
        if (self->abilityValues[1] < 0) {
            self->abilityValues[1] -= self->abilityValues[0] >> 4;

            if (self->abilityValues[1] > 0)
                self->abilityValues[1] = 0;
        }
    }
    else {
        self->abilityValues[1] -= self->abilityValues[0] >> 4;

        if (self->abilityValues[1] < 0)
            self->abilityValues[1] = 0;
    }

    if (leader->state == SizeLaser_PlayerState_ShrinkChibi) {
        if (leader->state == SizeLaser_PlayerState_GrowNormal || !leader->isChibi) {
            self->state       = SizeLaser_PlayerState_GrowNormal;
            self->interaction = false;
        }
    }
    else if (leader->isChibi) {
        self->state       = SizeLaser_PlayerState_ShrinkChibi;
        self->interaction = false;
    }
}

void SizeLaser_PlayerState_GrowNormal(void)
{
    // ???
    EntityPlayer *self = RSDK_GET_ENTITY(SceneInfo->entitySlot, Player);

    StateMachine(state) = self->abilityPtrs[0];
    StateMachine_Run(state);

    if (self->scale.x >= 0x200) {
        self->drawFX &= ~FX_SCALE;
        self->isChibi     = false;
        self->scale.x     = 0x200;
        self->scale.y     = 0x200;
        self->interaction = true;
        self->state       = Player_State_Air;
        Player_UpdatePhysicsState(self);
        self->jumpOffset = self->characterID == ID_TAILS ? 0 : 0x50000;
    }
    else {
        self->state = SizeLaser_PlayerState_GrowNormal;
        self->scale.x += 6;
        self->scale.y = self->scale.x;
    }
}

void SizeLaser_PlayerState_ShrinkChibi(void)
{
    // ???
    EntityPlayer *self = RSDK_GET_ENTITY(SceneInfo->entitySlot, Player);

    StateMachine(state) = self->abilityPtrs[0];
    StateMachine_Run(state);

    if (self->scale.x <= 0x140) {
        switch (self->characterID) {
            default:
            case ID_SONIC:
                self->aniFrames  = SizeLaser->sonicFrames;
                self->tailFrames = -1;
                break;

            case ID_TAILS:
                self->aniFrames  = SizeLaser->tailsFrames;
                self->tailFrames = SizeLaser->tailFrames;
                break;

            case ID_KNUCKLES:
                self->aniFrames  = SizeLaser->knuxFrames;
                self->tailFrames = -1;
                break;

#if MANIA_USE_PLUS
            case ID_MIGHTY:
                self->aniFrames  = SizeLaser->mightyFrames;
                self->tailFrames = -1;
                break;

            case ID_RAY:
                self->aniFrames  = SizeLaser->rayFrames;
                self->tailFrames = -1;
                break;
#endif
        }

        self->jumpOffset = 0x40000;
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_HURT, &self->animator, false, 0);
        self->isChibi = true;
        self->drawFX &= ~FX_SCALE;
        self->scale.x     = 0x200;
        self->scale.y     = 0x200;
        self->interaction = true;

        self->state = Player_State_Air;
        Player_UpdatePhysicsState(self);
    }
    else {
        self->state = SizeLaser_PlayerState_ShrinkChibi;
        self->scale.x -= 6;
        self->scale.y = self->scale.x;
    }
}

void SizeLaser_PlayerState_GrowGiant(void)
{
    EntityPlayer *self = RSDK_GET_ENTITY(SceneInfo->entitySlot, Player);

    StateMachine(state) = self->abilityPtrs[0];
    StateMachine_Run(state);

    if (self->scale.x >= 0x300) {
        self->scale.x     = 0x300;
        self->scale.y     = 0x300;
        self->interaction = true;
        self->state       = Player_State_Air;
        Player_UpdatePhysicsState(self);
    }
    else {
        self->state = SizeLaser_PlayerState_GrowGiant;
        self->scale.x += 8;
        self->scale.y = self->scale.x;
    }
}

void SizeLaser_CheckPlayerCollisions(void)
{
    RSDK_THIS(SizeLaser);

    int32 entityX = self->position.x;
    int32 entityY = self->position.y;

    int32 extendX1[2];
    int32 extendY1[2];
    int32 extendX2[2];
    int32 extendY2[2];

    switch (self->orientation) {
        default: break;

        case SIZELASER_ORIENTATION_DOWN:
            extendX1[0] = entityX - 0x80000;
            extendX1[1] = entityX + 0x80000;

            extendY1[0] = entityY - 0x200000;
            extendY1[1] = entityY - 0x200000;

            extendX2[0] = entityX - 0x80000;
            extendX2[1] = entityX + 0x80000;

            extendY2[0] = entityY + (self->extend << 16);
            extendY2[1] = entityY + (self->extend << 16);
            break;

        case SIZELASER_ORIENTATION_RIGHT:
            extendX1[0] = entityX - 0x200000;
            extendX1[1] = entityX - 0x200000;

            extendY1[0] = entityY - 0x80000;
            extendY1[1] = entityY + 0x80000;

            extendX2[0] = entityX + (self->extend << 16);
            extendX2[1] = entityX + (self->extend << 16);

            extendY2[1] = entityY + 0x80000;
            extendY2[0] = entityY - 0x80000;
            break;

        case SIZELASER_ORIENTATION_LEFT:
            extendX1[0] = entityX - 0x200000;
            extendX1[1] = entityX;

            extendY1[0] = entityY - 0x80000;
            extendY1[1] = entityY + 0x80000;

            extendX2[0] = entityX + 0x200000;
            extendX2[1] = entityX - (self->extend << 16);

            extendY2[0] = entityY + 0x80000;
            extendY2[1] = entityY - 0x80000;
            break;
    }

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if (MathHelpers_CheckPositionOverlap(SizeLaser->playerPositions[playerID].x, SizeLaser->playerPositions[playerID].y, player->position.x,
                                             player->position.y, extendX1[0], extendY1[0], extendX2[0], extendY2[0])
            || MathHelpers_CheckPositionOverlap(SizeLaser->playerPositions[playerID].x, SizeLaser->playerPositions[playerID].y, player->position.x,
                                                player->position.y, extendX1[1], extendY1[1], extendX2[1], extendY2[1])) {

            if (self->type) {
                if (player->state == SizeLaser_PlayerState_ShrinkChibi || player->state == SizeLaser_PlayerState_GrowNormal || !player->isChibi) {
                    if (player->state != SizeLaser_PlayerState_GrowGiant && player->scale.x > 0x400) {
                        player->onGround        = false;
                        player->interaction     = false;
                        player->velocity.y      = -0x40000;
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        player->velocity.x      = player->direction ? 0x20000 : -0x20000;
                        player->drawFX |= FX_SCALE;
                        player->scale.x        = 0x200;
                        player->scale.y        = 0x200;
                        player->tileCollisions = TILECOLLISION_DOWN;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                        player->abilityPtrs[0] = Player_State_Hurt;
                        player->state          = SizeLaser_PlayerState_GrowGiant;
                    }
                }
                else {
                    player->onGround        = false;
                    player->interaction     = false;
                    player->velocity.y      = -0x40000;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->velocity.x      = player->direction ? 0x20000 : -0x20000;
                    player->drawFX |= FX_SCALE;
                    player->scale.x = 0x140;
                    player->scale.y = 0x140;
                    switch (player->characterID) {
                        default:
                        case ID_SONIC:
                            if (player->superState == SUPERSTATE_SUPER)
                                player->aniFrames = Player->superFrames;
                            else
                                player->aniFrames = Player->sonicFrames;
                            player->tailFrames = -1;
                            break;

                        case ID_TAILS:
                            player->aniFrames  = Player->tailsFrames;
                            player->tailFrames = Player->tailsTailsFrames;
                            break;

                        case ID_KNUCKLES:
                            player->aniFrames  = Player->knuxFrames;
                            player->tailFrames = -1;
                            break;

#if MANIA_USE_PLUS
                        case ID_MIGHTY:
                            player->aniFrames  = Player->mightyFrames;
                            player->tailFrames = -1;
                            break;

                        case ID_RAY:
                            player->aniFrames  = Player->rayFrames;
                            player->tailFrames = -1;
                            break;
#endif
                    }

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                    RSDK.PlaySfx(SizeLaser->sfxGrow2, false, 255);
                    player->tileCollisions = TILECOLLISION_DOWN;
                    player->abilityPtrs[0] = Player_State_Hurt;
                    player->state          = SizeLaser_PlayerState_GrowNormal;
                }
            }
            else if (player->state != SizeLaser_PlayerState_ShrinkChibi && player->state != SizeLaser_PlayerState_GrowNormal && !player->isChibi) {
                player->onGround        = false;
                player->interaction     = false;
                player->velocity.y      = -0x40000;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
                player->velocity.x      = player->direction ? 0x20000 : -0x20000;
                player->drawFX |= FX_SCALE;
                player->scale.x = 0x200;
                player->scale.y = 0x200;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                RSDK.PlaySfx(SizeLaser->sfxShrink2, false, 0xFF);
                player->tileCollisions = TILECOLLISION_DOWN;
                player->abilityPtrs[0] = Player_State_Hurt;
                player->state          = SizeLaser_PlayerState_ShrinkChibi;
            }
        }
    }
}

void SizeLaser_State_Emitter(void)
{
    RSDK_THIS(SizeLaser);

    if (self->timer < 24 && !(self->timer & 1))
        CREATE_ENTITY(SizeLaser, INT_TO_VOID(self->type + (self->orientation << 2) + 1), self->position.x, self->position.y);
    SizeLaser_CheckPlayerCollisions();

    self->timer = (self->timer + 1) & 0x1F;
}

void SizeLaser_State_Laser(void)
{
    RSDK_THIS(SizeLaser);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    bool32 collided = false;
    switch (self->orientation) {
        case SIZELASER_ORIENTATION_DOWN: collided = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x40000, false); break;
        case SIZELASER_ORIENTATION_RIGHT: collided = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x40000, 0, false); break;
        case SIZELASER_ORIENTATION_LEFT: collided = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x40000, 0, false); break;
    }

    if (collided) {
        self->impactPos.x = self->position.x;
        self->impactPos.y = self->position.y;
        self->state       = SizeLaser_State_Impact;
    }
}

void SizeLaser_State_Impact(void)
{
    RSDK_THIS(SizeLaser);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (++self->timer == 4)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void SizeLaser_EditorDraw(void)
{
    RSDK_THIS(SizeLaser);

    switch (self->orientation) {
        case SIZELASER_ORIENTATION_DOWN:
            self->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(SizeLaser->aniFrames, 2 * self->type, &self->mainAnimator, true, 0);
            break;

        case SIZELASER_ORIENTATION_RIGHT:
        case SIZELASER_ORIENTATION_LEFT:
            self->direction = self->orientation == SIZELASER_ORIENTATION_LEFT ? FLIP_X : FLIP_NONE;
            RSDK.SetSpriteAnimation(SizeLaser->aniFrames, 2 * self->type + 1, &self->mainAnimator, true, 0);
            break;
    }

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        int32 extendX = 0, extendY = 0;
        switch (self->orientation) {
            case SIZELASER_ORIENTATION_DOWN: extendY = (self->extend << 16); break;

            case SIZELASER_ORIENTATION_RIGHT: extendX = (self->extend << 16); break;

            case SIZELASER_ORIENTATION_LEFT: extendX = -(self->extend << 16); break;
        }

        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x + extendX, self->position.y + extendY, 0x00FF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void SizeLaser_EditorLoad(void)
{
    SizeLaser->aniFrames = RSDK.LoadSpriteAnimation("MMZ/SizeLaser.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SizeLaser, type);
    RSDK_ENUM_VAR("Shrink", SIZELASER_SHRINK);
    RSDK_ENUM_VAR("Grow", SIZELASER_GROW);

    RSDK_ACTIVE_VAR(SizeLaser, orientation);
    RSDK_ENUM_VAR("Down", SIZELASER_ORIENTATION_DOWN);
    RSDK_ENUM_VAR("Right", SIZELASER_ORIENTATION_RIGHT);
    RSDK_ENUM_VAR("Left", SIZELASER_ORIENTATION_LEFT);
}
#endif

void SizeLaser_Serialize(void)
{
    RSDK_EDITABLE_VAR(SizeLaser, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(SizeLaser, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(SizeLaser, VAR_ENUM, extend);
}
