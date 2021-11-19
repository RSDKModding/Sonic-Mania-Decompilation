#include "SonicMania.h"

ObjectCurrent *Current;

void Current_Update(void)
{
    RSDK_THIS(Current);
    EntityButton *button = self->taggedButton;
    if (!button || button->field_70) {
        self->activated = true;
        StateMachine_Run(self->state);
    }
}

void Current_LateUpdate(void) {}

void Current_StaticUpdate(void) {}

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
            self->updateRange.x = 0x2000000;
            self->updateRange.y = 0x1000000;
            if (voidToInt(data) == 1) {
                self->drawFX  = FX_SCALE | FX_FLIP;
                self->scale.x = 0x400;
                self->scale.y = 0x100;
#if RETRO_USE_PLUS
                RSDK.SetSpriteAnimation(Current->aniFrames, 1, &self->animator, true, RSDK.RandSeeded(0, 4, &Zone->randSeed));
#else
                RSDK.SetSpriteAnimation(Current->aniFrames, 1, &self->animator, true, RSDK.Rand(0, 4));
#endif
            }
            else {
                RSDK.SetSpriteAnimation(Water->aniFrames, 5, &self->animator, true, 0);
                self->alpha = 512;
            }
            self->state = Current_State_LaundoMobile;
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
            Current_GetTaggedButton();
            switch (self->type) {
                case 0: self->state = Current_State_Type0; break;
                case 1: self->state = Current_State_Type1; break;
                case 2: self->state = Current_State_Type2; break;
                case 3: self->state = Current_State_Type3; break;
                case 4: self->state = Current_State_Type4; break;
                case 5: self->state = Current_State_Type5; break;
                case 6: self->state = Current_State_Type6; break;
                case 7: self->state = Current_State_Type7; break;
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

    self->taggedButton       = NULL;
    EntityButton *taggedEntity = RSDK.GetEntityByID(RSDK.GetEntityID(self) - 1);
    bool32 tagged              = false;

    if (self->buttonTag > 0) {
        if (Button) {
            foreach_all(Button, button)
            {
                if (button->tag == self->buttonTag) {
                    taggedEntity = button;
                    tagged       = true;
                    foreach_break;
                }
            }
        }

        if (PullChain && !tagged) {
            foreach_all(PullChain, chain)
            {
                if (chain->tag == self->buttonTag) {
                    taggedEntity = (EntityButton *)chain;
                    tagged       = true;
                    foreach_break;
                }
            }
        }
    }

    if (taggedEntity) {
        if ((Button && taggedEntity->objectID == Button->objectID) || (PullChain && taggedEntity->objectID == PullChain->objectID)) {
            if (self) {
                int32 distX = abs(self->position.x - taggedEntity->position.x);
                int32 distY = abs(self->position.y - taggedEntity->position.y);

                if (self->updateRange.x < 0x800000 + distX)
                    self->updateRange.x = 0x800000 + distX;

                if (self->updateRange.y < 0x800000 + distY)
                    self->updateRange.y = 0x800000 + distY;
            }
            self->taggedButton = taggedEntity;
        }
    }
}

Vector2 Current_Unknown2(uint8 a1)
{
    RSDK_THIS(Current);
    Vector2 result;

    int x = 0;
    if (a1) {
        x = self->position.x - (self->size.x >> 1) - 0x200000;
        if ((ScreenInfo->position.x - 64) << 16 > x)
            x = (ScreenInfo->position.x - 64) << 16;
    }
    else {
        x = (self->size.x >> 1) + self->position.x + 0x200000;
        if ((ScreenInfo->position.x + 64 + ScreenInfo->width) << 16 < x)
            x = (ScreenInfo->position.x + 64 + ScreenInfo->width) << 16;
    }

    int startY = self->size.y >> 1;

    int finalY = self->position.y - startY - 0x200000;
    if ((ScreenInfo->position.y - 64) << 16 > finalY)
        finalY = (ScreenInfo->position.y - 64) << 16;

    int screenY = (ScreenInfo->position.y + 64 + ScreenInfo->height) << 16;
    int minY    = startY + self->position.y + 0x200000;
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
        result.y = (RSDK.RandSeeded(0, max, &Zone->randSeed) << 20) + finalY;
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
        y = self->position.y - (self->size.y >> 1) - 0x200000;
        if ((ScreenInfo->position.y - 64) << 16 > y)
            y = (ScreenInfo->position.y - 64) << 16;
    }
    else {
        y = (self->size.y >> 1) + self->position.y + 0x200000;
        if ((ScreenInfo->position.y + 64 + ScreenInfo->height) << 16 < y)
            y = (ScreenInfo->position.y + 64 + ScreenInfo->height) << 16;
    }

    int startX = self->size.x >> 1;
    int finalX = self->position.x - startX - 0x200000;
    if ((ScreenInfo->position.x - 64) << 16 > finalX)
        finalX = (ScreenInfo->position.x - 64) << 16;

    int screenX = (ScreenInfo->position.x + 64 + ScreenInfo->width) << 16;
    int minX    = startX + self->position.x + 0x200000;
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
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
            && (!self->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & self->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, self, &self->hitbox) || player->state == Player_State_StartJumpIn) {
                        if (player->state != Player_State_StartJumpIn)
                            player->state = Player_State_Air;

                        int x = self->position.x + ((self->hitbox.right - 32) << 16);
                        self->activePlayers &= ~(1 << playerID);
                        if (RSDK.CheckStageFolder("FBZ") && !player->sidekick && self->fbzAchievement && self->playerPositions[playerID].x >= x
                            && player->position.x < self->position.x) {
                            API_UnlockAchievement("ACH_FBZ");
                        }
                        self->playerPositions[playerID].x = 0;
                        self->playerPositions[playerID].y = 0;
                    }
                }
                else if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    self->playerPositions[playerID].x = player->position.x;
                    self->playerPositions[playerID].y = player->position.y;
                    self->activePlayers |= (1 << playerID);
                }

                if ((1 << playerID) & self->activePlayers) {
                    bool32 flag = false;
                    if (Water)
                        flag = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hit && player->state != Player_State_StartJumpIn) {
                        int anim = player->animator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.x      = -0x8000 * self->strength;
                            player->groundVel       = player->velocity.x;
                            if (!flag) {
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
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
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_Unknown2(0);
            EntityCurrent *child = CREATE_ENTITY(Current, intToVoid(2), pos.x, pos.y);
            child->drawOrder     = Zone->drawOrderHigh;
            child->strength      = (self->strength + (self->strength >> 3)) >> 1;
            child->size.x        = self->position.x - (self->size.x >> 1) + 0x400000;
            child->drawOrder     = Zone->playerDrawLow;
            if (child->position.x < (self->position.x + (self->size.x >> 1)))
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
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
            && (!self->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & self->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, self, &self->hitbox) || player->state == Player_State_StartJumpIn) {
                        if (player->state != Player_State_StartJumpIn)
                            player->state = Player_State_Air;
                        self->activePlayers &= ~(1 << playerID);
                    }
                }
                else if (Player_CheckCollisionTouch(player, self, &self->hitbox))
                    self->activePlayers |= (1 << playerID);

                if ((1 << playerID) & self->activePlayers) {
                    bool32 flag = false;
                    if (Water)
                        flag = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hit && player->state != Player_State_StartJumpIn) {
                        int anim = player->animator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.x      = self->strength << 15;
                            player->groundVel       = player->velocity.x;
                            if (!flag) {
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
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
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_Unknown2(1);
            EntityCurrent *child = CREATE_ENTITY(Current, intToVoid(2), pos.x, pos.y);
            child->drawOrder     = Zone->drawOrderHigh;
            child->strength      = (self->strength + (self->strength >> 3)) >> 1;
            child->size.x        = (self->size.x >> 1) + self->position.x - 0x400000;
            child->type          = 1;
            child->drawOrder     = Zone->playerDrawLow;
            if (child->position.x > (self->position.x - (self->size.x >> 1)))
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
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
            && (!self->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & self->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, self, &self->hitbox) || player->state == Player_State_StartJumpIn) {
                        if (player->state != Player_State_StartJumpIn)
                            player->state = Player_State_Air;
                        self->activePlayers &= ~(1 << playerID);
                    }
                }
                else if (Player_CheckCollisionTouch(player, self, &self->hitbox))
                    self->activePlayers |= (1 << playerID);

                if ((1 << playerID) & self->activePlayers) {
                    bool32 flag = false;
                    if (Water)
                        flag = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hit && player->state != Player_State_StartJumpIn) {
                        int anim = player->animator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.y      = -0x8000 * self->strength;
                            if (!flag) {
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
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
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_Unknown3(0);
            EntityCurrent *child = CREATE_ENTITY(Current, intToVoid(2), pos.x, pos.y);
            child->drawOrder     = Zone->drawOrderHigh;
            child->strength      = (self->strength + (self->strength >> 3)) >> 1;
            child->size.y        = (self->position.y - (self->size.y >> 1)) + 0x400000;
            child->type          = 2;
            child->drawOrder     = Zone->playerDrawLow;
            if (child->position.y < (self->position.y + (self->size.y >> 1)))
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
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
            && (!self->waterOnly || player->underwater)) {

            if (Player_CheckValidState(player)) {
                if ((1 << playerID) & self->activePlayers) {
                    if (!Player_CheckCollisionTouch(player, self, &self->hitbox) || player->state == Player_State_StartJumpIn) {
                        if (player->state != Player_State_StartJumpIn)
                            player->state = Player_State_Air;
                        self->activePlayers &= ~(1 << playerID);
                    }
                }
                else if (Player_CheckCollisionTouch(player, self, &self->hitbox))
                    self->activePlayers |= (1 << playerID);

                if ((1 << playerID) & self->activePlayers) {
                    bool32 flag = false;
                    if (Water)
                        flag = Water_GetPlayerBubble(player) != NULL;

                    if (player->state != Player_State_Hit && player->state != Player_State_StartJumpIn) {
                        int anim = player->animator.animationID;
                        if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                            player->onGround        = false;
                            player->nextGroundState = StateMachine_None;
                            player->nextAirState    = StateMachine_None;
                            player->velocity.y      = self->strength << 15;
                            if (!flag) {
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
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
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (Water) {
        if (!(Zone->timer & 7)) {
            Vector2 pos          = Current_Unknown3(1);
            EntityCurrent *child = CREATE_ENTITY(Current, intToVoid(2), pos.x, pos.y);
            child->drawOrder     = Zone->drawOrderHigh;
            child->strength      = (self->strength + (self->strength >> 3)) >> 1;
            child->size.y        = (self->size.y >> 1) + self->position.y - 0x400000;
            child->type          = 3;
            child->drawOrder     = Zone->playerDrawLow;
            if (child->position.y > (self->position.y - (self->size.y >> 1)))
                child->alpha = 240;
        }
    }
}

void Current_State_Type4(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
            && (!self->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                int anim = player->animator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING)
                    player->position.x += -0x8000 * self->strength;
            }
        }
    }

    if (!(Zone->timer & 7)) {
        Vector2 pos          = Current_Unknown2(0);
        EntityCurrent *child = CREATE_ENTITY(Current, intToVoid(1), pos.x, pos.y);
        child->drawOrder     = Zone->drawOrderHigh;
        child->strength      = 4 * self->strength + RSDK.Rand(1 - self->strength, 5);
        child->size.x        = self->position.x - (self->size.x >> 1) + 0x400000;
        if (child->position.x < (self->position.x + (self->size.x >> 1)))
            child->alpha = 240;
    }
}

void Current_State_Type5(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
            && (!self->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                int anim = player->animator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING)
                    player->position.x += self->strength << 15;
            }
        }
    }
}

void Current_State_Type6(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
            && (!self->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                int anim = player->animator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING && !player->onGround)
                    player->position.y += -0x8000 * self->strength;
            }
        }
    }
}

void Current_State_Type7(void)
{
    RSDK_THIS(Current);

    foreach_active(Player, player)
    {
        if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
            && (!self->waterOnly || player->underwater)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                int anim = player->animator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING && !player->onGround)
                    player->position.y += self->strength << 15;
            }
        }
    }
}

void Current_State_LaundoMobile(void)
{
    RSDK_THIS(Current);
    RSDK.ProcessAnimation(&self->animator);
    switch (self->type) {
        case 0:
            self->position.x -= self->strength << 17;
            if (self->position.x < self->size.x) {
                self->alpha -= 8;
            }
            else {
                if (self->alpha < 240)
                    self->alpha += 8;
            }
            break;
        case 1:
            self->position.x += self->strength << 17;
            if (self->position.x >= self->size.x) {
                if (self->alpha < 240)
                    self->alpha += 8;
            }
            else {
                self->alpha -= 8;
            }
            break;
        case 2:
            self->position.y -= self->strength << 17;
            if (self->position.y >= self->size.y) {
                if (self->alpha < 240)
                    self->alpha += 8;
            }
            else {
                self->alpha -= 8;
            }
            break;
        case 3:
            self->position.y += self->strength << 17;
            if (self->position.y >= self->size.y) {
                if (self->alpha < 240)
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

void Current_Player_State_Type0(void)
{
    RSDK_THIS(Player);
    Hitbox *hitbox = Player_GetHitbox(self);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_ROOF, self->collisionPlane, hitbox->left << 16, hitbox->top << 16, true);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_FLOOR, self->collisionPlane, hitbox->left << 16, hitbox->bottom << 16, true);
}

void Current_Player_State_Type1(void)
{
    RSDK_THIS(Player);
    Hitbox *hitbox = Player_GetHitbox(self);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_ROOF, self->collisionPlane, hitbox->right << 16, hitbox->top << 16, true);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_FLOOR, self->collisionPlane, hitbox->right << 16, hitbox->bottom << 16, true);
}

void Current_Player_State_Type2(void)
{
    RSDK_THIS(Player);
    Hitbox *hitbox = Player_GetHitbox(self);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_RWALL, self->collisionPlane, hitbox->left << 16, hitbox->top << 16, true);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_LWALL, self->collisionPlane, hitbox->right << 16, hitbox->top << 16, true);
}

void Current_Player_State_Type3(void)
{
    RSDK_THIS(Player);
    Hitbox *hitbox = Player_GetHitbox(self);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_RWALL, self->collisionPlane, hitbox->left << 16, hitbox->bottom << 16, true);
    RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_LWALL, self->collisionPlane, hitbox->right << 16, hitbox->bottom << 16, true);
}

#if RETRO_INCLUDE_EDITOR
void Current_EditorDraw(void)
{
    RSDK_THIS(Current);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->size.x >> 1;
    drawPos.y -= self->size.y >> 1;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + self->size.x, drawPos.y - 0x10000, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, self->size.y + drawPos.y, drawPos.x + self->size.x, self->size.y + drawPos.y, 0xFFFF00, 0, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + self->size.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + self->size.x, drawPos.y - 0x10000, drawPos.x + self->size.x, drawPos.y + self->size.y, 0xFFFF00, 0, INK_NONE,
                  false);

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
