// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Current Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCurrent *Current;

void Current_Update(void)
{
    RSDK_THIS(Current);

    EntityButton *button = self->taggedButton;
    if (!button || button->activated) {
        self->activated = true;
        StateMachine_Run(self->state);
    }
}

void Current_LateUpdate(void) {}

void Current_StaticUpdate(void)
{
    if (Water) {
        int32 offsetH = 0;
        int32 offsetV = 0;

        foreach_active(Water, water)
        {
            if (water->state == Water_State_BigBubble && water->activePlayers) {
                foreach_active(Current, current)
                {
                    if (current->planeFilter <= 0 || water->collisionPlane == ((uint8)(current->planeFilter - 1) & 1)) {
                        int32 strength = current->strength << 15;
                        if (MathHelpers_PointInHitbox(current->position.x, current->position.y, water->position.x, water->position.y,
                                                      current->direction, &current->hitbox)) {
                            if (current->state == Current_State_WaterUp)
                                offsetV -= strength;

                            if (current->state == Current_State_WaterDown)
                                offsetV += strength;

                            if (current->state == Current_State_WaterLeft)
                                offsetH -= strength;

                            if (current->state == Current_State_WaterRight)
                                offsetH += strength;
                        }
                    }
                }

                if (water->bubbleVelocity.x < offsetH) {
                    water->bubbleVelocity.x += 0x6000;

                    if (water->bubbleVelocity.x > offsetH)
                        water->bubbleVelocity.x = offsetH;
                }

                if (water->bubbleVelocity.x > offsetH) {
                    water->bubbleVelocity.x -= 0x6000;

                    if (water->bubbleVelocity.x < offsetH)
                        water->bubbleVelocity.x = offsetH;
                }

                if (water->bubbleVelocity.y < offsetV) {
                    water->bubbleVelocity.y += 0x6000;

                    if (water->bubbleVelocity.y > offsetV)
                        water->bubbleVelocity.y = offsetV;
                }

                if (water->bubbleVelocity.y > offsetV) {
                    water->bubbleVelocity.y -= 0x6000;

                    if (water->bubbleVelocity.y < offsetV)
                        water->bubbleVelocity.y = offsetV;
                }
            }
        }
    }

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        bool32 active = false;
        foreach_active(Current, current)
        {
            if (current->activated) {
                if (current->type < CURRENT_W_LEFT) {
                    if ((current->planeFilter <= 0 || player->collisionPlane == ((uint8)(current->planeFilter - 1) & 1))
                        && (!current->waterOnly || player->underwater)) {
                        if (Player_CheckCollisionTouch(player, current, &current->hitbox)) {
                            Current->activePlayers |= 1 << playerID;
                            active = true;
                            foreach_break;
                        }
                    }
                }
            }
        }

        if (!active)
            Current->activePlayers &= ~(1 << playerID);
    }
}

void Current_Draw(void)
{
    RSDK_THIS(Current);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Current_Create(void *data)
{
    RSDK_THIS(Current);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        if (data) {
            self->active        = ACTIVE_NORMAL;
            self->inkEffect     = INK_ADD;
            self->visible       = true;
            self->updateRange.x = TO_FIXED(512);
            self->updateRange.y = TO_FIXED(256);

            if (VOID_TO_INT(data) == CURRENT_CHILD_WIND) {
                self->drawFX  = FX_SCALE | FX_FLIP;
                self->scale.x = 0x400;
                self->scale.y = 0x100;
                RSDK.SetSpriteAnimation(Current->aniFrames, 1, &self->animator, true, ZONE_RAND(0, 4));
            }
            else {
                RSDK.SetSpriteAnimation(Water->aniFrames, 5, &self->animator, true, 0);
                self->alpha = 0x200;
            }

            self->state = Current_State_Child;
        }
        else {
            self->visible       = false;
            self->updateRange.x = self->size.x >> 1;
            self->updateRange.y = self->size.y >> 1;
            self->active        = ACTIVE_BOUNDS;

            self->hitbox.left   = -(self->size.x >> 17);
            self->hitbox.right  = self->size.x >> 17;
            self->hitbox.top    = -(self->size.y >> 17);
            self->hitbox.bottom = self->size.y >> 17;

            Current_SetupTagLink();
            switch (self->type) {
                case CURRENT_C_LEFT: self->state = Current_State_WaterLeft; break;
                case CURRENT_C_RIGHT: self->state = Current_State_WaterRight; break;
                case CURRENT_C_UP: self->state = Current_State_WaterUp; break;
                case CURRENT_C_DOWN: self->state = Current_State_WaterDown; break;
                case CURRENT_W_LEFT: self->state = Current_State_PushLeft; break;
                case CURRENT_W_RIGHT: self->state = Current_State_PushRight; break;
                case CURRENT_W_UP: self->state = Current_State_PushUp; break;
                case CURRENT_W_DOWN: self->state = Current_State_PushDown; break;
                default: break;
            }
        }
    }
}

void Current_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("FBZ"))
        Current->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Current.bin", SCOPE_STAGE);
    else
        Current->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(Current->aniFrames, 0, &Current->animator, true, 0);

    Current->sfxRush = RSDK.GetSfx("Stage/Rush.wav");
}

void Current_SetupTagLink(void)
{
    RSDK_THIS(Current);

    self->taggedButton         = NULL;
    EntityButton *taggedButton = RSDK_GET_ENTITY(RSDK.GetEntitySlot(self) - 1, Button);

    if (self->buttonTag > 0) {
        bool32 matchedTag = false;
        if (Button) {
            foreach_all(Button, button)
            {
                if (button->tag == self->buttonTag) {
                    taggedButton = button;
                    matchedTag   = true;
                    foreach_break;
                }
            }
        }

        if (PullChain && !matchedTag) {
            foreach_all(PullChain, chain)
            {
                if (chain->tag == self->buttonTag) {
                    taggedButton = (EntityButton *)chain;
                    matchedTag   = true;
                    foreach_break;
                }
            }
        }
    }

    if (taggedButton) {
        if ((Button && taggedButton->classID == Button->classID) || (PullChain && taggedButton->classID == PullChain->classID)) {
            int32 distX = abs(self->position.x - taggedButton->position.x) + TO_FIXED(128);
            int32 distY = abs(self->position.y - taggedButton->position.y) + TO_FIXED(128);

            if (self->updateRange.x < distX)
                self->updateRange.x = distX;

            if (self->updateRange.y < distY)
                self->updateRange.y = distY;

            self->taggedButton = taggedButton;
        }
    }
}

Vector2 Current_GetBubbleSpawnPosHorizontal(uint8 right)
{
    RSDK_THIS(Current);

    int32 x = 0;
    if (right) {
        x = self->position.x - (self->size.x >> 1) - 0x200000;
        if ((ScreenInfo->position.x - 64) << 16 > x)
            x = (ScreenInfo->position.x - 64) << 16;
    }
    else {
        x = (self->size.x >> 1) + self->position.x + 0x200000;
        if ((ScreenInfo->position.x + 64 + ScreenInfo->size.x) << 16 < x)
            x = (ScreenInfo->position.x + 64 + ScreenInfo->size.x) << 16;
    }

    int32 maxY = self->position.y - (self->size.y >> 1) - 0x200000;
    if ((ScreenInfo->position.y - 64) << 16 > maxY)
        maxY = (ScreenInfo->position.y - 64) << 16;

    int32 screenY = (ScreenInfo->position.y + 64 + ScreenInfo->size.y) << 16;
    int32 minY    = (self->size.y >> 1) + self->position.y + 0x200000;
    if (screenY < minY)
        minY = screenY;

    int32 randMax = (minY - maxY) >> 20;
    if (!randMax)
        randMax = 1;

    Vector2 bubblePos;
    bubblePos.x = x;
    if (randMax <= 0)
        bubblePos.y = maxY;
    else
        bubblePos.y = maxY + (ZONE_RAND(0, randMax) << 20);
    return bubblePos;
}

Vector2 Current_GetBubbleSpawnPosVertical(uint8 down)
{
    RSDK_THIS(Current);

    int32 y = 0;
    if (down) {
        y = self->position.y - (self->size.y >> 1) - 0x200000;
        if ((ScreenInfo->position.y - 64) << 16 > y)
            y = (ScreenInfo->position.y - 64) << 16;
    }
    else {
        y = (self->size.y >> 1) + self->position.y + 0x200000;
        if ((ScreenInfo->position.y + 64 + ScreenInfo->size.y) << 16 < y)
            y = (ScreenInfo->position.y + 64 + ScreenInfo->size.y) << 16;
    }

    int32 maxX = self->position.x - (self->size.x >> 1) - 0x200000;
    if ((ScreenInfo->position.x - 64) << 16 > maxX)
        maxX = (ScreenInfo->position.x - 64) << 16;

    int32 screenX = (ScreenInfo->position.x + 64 + ScreenInfo->size.x) << 16;
    int32 minX    = (self->size.x >> 1) + self->position.x + 0x200000;
    if (screenX < minX)
        minX = screenX;

    int32 max = (minX - maxX) >> 20;
    if (!max)
        max = 1;

    Vector2 bubblePos;
    if (max <= 0) {
        bubblePos.x = maxX;
        bubblePos.y = y;
    }
    else {
        bubblePos.x = maxX + (RSDK.Rand(0, max) << 20);
        bubblePos.y = y;
    }
    return bubblePos;
}

void Current_State_WaterLeft(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) && (!self->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & self->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, self, &self->hitbox) || player->state == Player_State_HoldRespawn) {
                        if (player->state != Player_State_HoldRespawn)
                            player->state = Player_State_Air;

                        int32 x = self->position.x + ((self->hitbox.right - 32) << 16);
                        self->activePlayers &= ~(1 << playerID);
                        if (RSDK.CheckSceneFolder("FBZ") && !player->sidekick && self->fbzAchievement && self->playerPositions[playerID].x >= x
                            && player->position.x < self->position.x) {
                            API_UnlockAchievement(&achievementList[ACH_FBZ]);
                        }
                        self->playerPositions[playerID].x = 0;
                        self->playerPositions[playerID].y = 0;
                    }
                }
                else if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    self->playerPositions[playerID].x = player->position.x;
                    self->playerPositions[playerID].y = player->position.y;
                    self->activePlayers |= 1 << playerID;
                }

                if ((1 << playerID) & self->activePlayers) {
                    bool32 inBubble = false;
                    if (Water)
                        inBubble = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hurt && player->state != Player_State_HoldRespawn) {
                        int32 anim = player->animator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFT_SWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.x      = -(self->strength << 15);
                            player->groundVel       = player->velocity.x;

                            if (!inBubble) {
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
                                player->tileCollisions  = TILECOLLISION_DOWN;
                                player->nextAirState    = StateMachine_None;
                                player->nextGroundState = StateMachine_None;
                                player->state           = Current_PlayerState_Left;
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
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_GetBubbleSpawnPosHorizontal(false);
            EntityCurrent *child = CREATE_ENTITY(Current, INT_TO_VOID(CURRENT_CHILD_BUBBLE), pos.x, pos.y);

            child->drawGroup = Zone->objectDrawGroup[1];
            child->strength  = (self->strength + (self->strength >> 3)) >> 1;
            child->size.x    = self->position.x - (self->size.x >> 1) + 0x400000;
            child->drawGroup = Zone->playerDrawGroup[0];
            if (child->position.x < (self->position.x + (self->size.x >> 1)))
                child->alpha = 0xF0;
        }
    }
}

void Current_State_WaterRight(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) && (!self->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & self->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, self, &self->hitbox) || player->state == Player_State_HoldRespawn) {
                        if (player->state != Player_State_HoldRespawn)
                            player->state = Player_State_Air;
                        self->activePlayers &= ~(1 << playerID);
                    }
                }
                else if (Player_CheckCollisionTouch(player, self, &self->hitbox))
                    self->activePlayers |= 1 << playerID;

                if ((1 << playerID) & self->activePlayers) {
                    bool32 inBubble = false;
                    if (Water)
                        inBubble = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hurt && player->state != Player_State_HoldRespawn) {
                        int32 anim = player->animator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFT_SWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.x      = self->strength << 15;
                            player->groundVel       = player->velocity.x;
                            if (!inBubble) {
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
                                player->tileCollisions  = TILECOLLISION_DOWN;
                                player->nextAirState    = StateMachine_None;
                                player->nextGroundState = StateMachine_None;
                                player->state           = Current_PlayerState_Right;
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
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_GetBubbleSpawnPosHorizontal(true);
            EntityCurrent *child = CREATE_ENTITY(Current, INT_TO_VOID(CURRENT_CHILD_BUBBLE), pos.x, pos.y);

            child->drawGroup = Zone->objectDrawGroup[1];
            child->strength  = (self->strength + (self->strength >> 3)) >> 1;
            child->size.x    = (self->size.x >> 1) + self->position.x - 0x400000;
            child->type      = CURRENT_C_RIGHT;
            child->drawGroup = Zone->playerDrawGroup[0];
            if (child->position.x > (self->position.x - (self->size.x >> 1)))
                child->alpha = 0xF0;
        }
    }
}

void Current_State_WaterUp(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) && (!self->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & self->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, self, &self->hitbox) || player->state == Player_State_HoldRespawn) {
                        if (player->state != Player_State_HoldRespawn)
                            player->state = Player_State_Air;
                        self->activePlayers &= ~(1 << playerID);
                    }
                }
                else if (Player_CheckCollisionTouch(player, self, &self->hitbox))
                    self->activePlayers |= 1 << playerID;

                if ((1 << playerID) & self->activePlayers) {
                    bool32 inBubble = false;
                    if (Water)
                        inBubble = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hurt && player->state != Player_State_HoldRespawn) {
                        int32 anim = player->animator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFT_SWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.y      = -(self->strength << 15);
                            if (!inBubble) {
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
                                player->tileCollisions  = TILECOLLISION_DOWN;
                                player->nextAirState    = StateMachine_None;
                                player->nextGroundState = StateMachine_None;
                                player->state           = Current_PlayerState_Up;
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
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_GetBubbleSpawnPosVertical(false);
            EntityCurrent *child = CREATE_ENTITY(Current, INT_TO_VOID(CURRENT_CHILD_BUBBLE), pos.x, pos.y);

            child->drawGroup = Zone->objectDrawGroup[1];
            child->strength  = (self->strength + (self->strength >> 3)) >> 1;
            child->size.y    = (self->position.y - (self->size.y >> 1)) + 0x400000;
            child->type      = CURRENT_C_UP;
            child->drawGroup = Zone->playerDrawGroup[0];
            if (child->position.y < (self->position.y + (self->size.y >> 1)))
                child->alpha = 0xF0;
        }
    }
}

void Current_State_WaterDown(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) && (!self->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & self->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, self, &self->hitbox) || player->state == Player_State_HoldRespawn) {
                        if (player->state != Player_State_HoldRespawn)
                            player->state = Player_State_Air;

                        self->activePlayers &= ~(1 << playerID);
                    }
                }
                else if (Player_CheckCollisionTouch(player, self, &self->hitbox))
                    self->activePlayers |= 1 << playerID;

                if ((1 << playerID) & self->activePlayers) {
                    bool32 inBubble = false;
                    if (Water)
                        inBubble = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hurt && player->state != Player_State_HoldRespawn) {
                        int32 anim = player->animator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFT_SWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.y      = self->strength << 15;

                            if (!inBubble) {
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
                                player->tileCollisions  = TILECOLLISION_DOWN;
                                player->nextAirState    = StateMachine_None;
                                player->nextGroundState = StateMachine_None;
                                player->state           = Current_PlayerState_Down;
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
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_GetBubbleSpawnPosVertical(true);
            EntityCurrent *child = CREATE_ENTITY(Current, INT_TO_VOID(CURRENT_CHILD_BUBBLE), pos.x, pos.y);

            child->drawGroup = Zone->objectDrawGroup[1];
            child->strength  = (self->strength + (self->strength >> 3)) >> 1;
            child->size.y    = (self->size.y >> 1) + self->position.y - 0x400000;
            child->type      = CURRENT_C_DOWN;
            child->drawGroup = Zone->playerDrawGroup[0];
            if (child->position.y > (self->position.y - (self->size.y >> 1)))
                child->alpha = 0xF0;
        }
    }
}

void Current_State_PushLeft(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) && (!self->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                int32 anim = player->animator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFT_SWING)
                    player->position.x += -(self->strength << 15);
            }
        }
    }

    if (!(Zone->timer & 7)) {
        Vector2 pos          = Current_GetBubbleSpawnPosHorizontal(0);
        EntityCurrent *child = CREATE_ENTITY(Current, INT_TO_VOID(CURRENT_CHILD_WIND), pos.x, pos.y);

        child->drawGroup = Zone->objectDrawGroup[1];
        child->strength  = 4 * self->strength + RSDK.Rand(1 - self->strength, 5);
        child->size.x    = self->position.x - (self->size.x >> 1) + 0x400000;
        if (child->position.x < (self->position.x + (self->size.x >> 1)))
            child->alpha = 0xF0;
    }
}

void Current_State_PushRight(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) && (!self->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                int32 anim = player->animator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFT_SWING)
                    player->position.x += self->strength << 15;
            }
        }
    }
}

void Current_State_PushUp(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) && (!self->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                int32 anim = player->animator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFT_SWING && !player->onGround)
                    player->position.y += -(self->strength << 15);
            }
        }
    }
}

void Current_State_PushDown(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) && (!self->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                int32 anim = player->animator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFT_SWING && !player->onGround)
                    player->position.y += self->strength << 15;
            }
        }
    }
}

void Current_State_Child(void)
{
    RSDK_THIS(Current);
    RSDK.ProcessAnimation(&self->animator);

    switch (self->type) {
        case CURRENT_C_LEFT:
            self->position.x -= self->strength << 17;
            if (self->position.x < self->size.x) {
                self->alpha -= 8;
            }
            else {
                if (self->alpha < 0xF0)
                    self->alpha += 8;
            }
            break;

        case CURRENT_C_RIGHT:
            self->position.x += self->strength << 17;
            if (self->position.x >= self->size.x) {
                if (self->alpha < 0xF0)
                    self->alpha += 8;
            }
            else {
                self->alpha -= 8;
            }
            break;

        case CURRENT_C_UP:
            self->position.y -= self->strength << 17;
            if (self->position.y >= self->size.y) {
                if (self->alpha < 0xF0)
                    self->alpha += 8;
            }
            else {
                self->alpha -= 8;
            }
            break;

        case CURRENT_C_DOWN:
            self->position.y += self->strength << 17;
            if (self->position.y >= self->size.y) {
                if (self->alpha < 0xF0)
                    self->alpha += 8;
            }
            else {
                self->alpha -= 8;
            }
            break;

        default: break;
    }

    if ((Water && self->position.y < Water->waterLevel) || !RSDK.CheckOnScreen(self, NULL) || !self->alpha)
        destroyEntity(self);
}

void Current_PlayerState_Left(void)
{
    RSDK_THIS(Player);

    Hitbox *hitbox = Player_GetHitbox(self);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_ROOF, self->collisionPlane, hitbox->left << 16, hitbox->top << 16, true);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_FLOOR, self->collisionPlane, hitbox->left << 16, hitbox->bottom << 16, true);
}

void Current_PlayerState_Right(void)
{
    RSDK_THIS(Player);

    Hitbox *hitbox = Player_GetHitbox(self);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_ROOF, self->collisionPlane, hitbox->right << 16, hitbox->top << 16, true);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_FLOOR, self->collisionPlane, hitbox->right << 16, hitbox->bottom << 16, true);
}

void Current_PlayerState_Up(void)
{
    RSDK_THIS(Player);

    Hitbox *hitbox = Player_GetHitbox(self);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_RWALL, self->collisionPlane, hitbox->left << 16, hitbox->top << 16, true);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_LWALL, self->collisionPlane, hitbox->right << 16, hitbox->top << 16, true);
}

void Current_PlayerState_Down(void)
{
    RSDK_THIS(Player);

    Hitbox *hitbox = Player_GetHitbox(self);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_RWALL, self->collisionPlane, hitbox->left << 16, hitbox->bottom << 16, true);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_LWALL, self->collisionPlane, hitbox->right << 16, hitbox->bottom << 16, true);
}

#if GAME_INCLUDE_EDITOR
void Current_EditorDraw(void)
{
    RSDK_THIS(Current);

    self->updateRange.x = self->size.x >> 1;
    self->updateRange.y = self->size.y >> 1;

    RSDK.SetSpriteAnimation(Current->aniFrames, 1, &Current->animator, true, 0);
    self->direction = FLIP_NONE;

    RSDK.DrawSprite(&Current->animator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);
        DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);

        RSDK.SetSpriteAnimation(Current->aniFrames, 0, &Current->animator, true, 0);
        Vector2 drawPos;
        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        drawPos.x -= self->size.x >> 1;
        drawPos.y -= self->size.y >> 1;

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&Current->animator, &drawPos, false);

        drawPos.x += self->size.x;
        self->direction = FLIP_X;
        RSDK.DrawSprite(&Current->animator, &drawPos, false);

        drawPos.y += self->size.y;
        self->direction = FLIP_XY;
        RSDK.DrawSprite(&Current->animator, &drawPos, false);

        drawPos.x -= self->size.x;
        self->direction = FLIP_Y;
        RSDK.DrawSprite(&Current->animator, &drawPos, false);
        RSDK_DRAWING_OVERLAY(false);

        Current_SetupTagLink();

        RSDK_DRAWING_OVERLAY(true);
        if (self->taggedButton) {
            DrawHelpers_DrawArrow(self->taggedButton->position.x, self->taggedButton->position.y, self->position.x, self->position.y, 0xFFFF00,
                                  INK_NONE, 0xFF);
        }
        RSDK_DRAWING_OVERLAY(false);
    }
}

void Current_EditorLoad(void)
{
    Current->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Current.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(Current->aniFrames, 1, &Current->animator, true, 0);

    RSDK_ACTIVE_VAR(Current, type);
    RSDK_ENUM_VAR("Current - Left", CURRENT_C_LEFT);
    RSDK_ENUM_VAR("Current - Right", CURRENT_C_RIGHT);
    RSDK_ENUM_VAR("Current - Up", CURRENT_C_UP);
    RSDK_ENUM_VAR("Current - Down", CURRENT_C_DOWN);
    RSDK_ENUM_VAR("Wind - Left", CURRENT_W_LEFT);
    RSDK_ENUM_VAR("Wind - Right", CURRENT_W_RIGHT);
    RSDK_ENUM_VAR("Wind - Up", CURRENT_W_UP);
    RSDK_ENUM_VAR("Wind - Down", CURRENT_W_DOWN);

    RSDK_ACTIVE_VAR(Current, planeFilter);
    RSDK_ENUM_VAR("None", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("AL", PLANEFILTER_AL);
    RSDK_ENUM_VAR("BL", PLANEFILTER_BL);
    RSDK_ENUM_VAR("AH", PLANEFILTER_AH);
    RSDK_ENUM_VAR("BH", PLANEFILTER_BH);
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
