// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Fan Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFan *Fan;

void Fan_Update(void)
{
    RSDK_THIS(Fan);

    StateMachine_Run(self->stateActivate);
    StateMachine_Run(self->stateDeactivate);

    RSDK.ProcessAnimation(&self->animator);

    StateMachine_Run(self->state);
}

void Fan_LateUpdate(void) {}

void Fan_StaticUpdate(void)
{
    Fan->activePlayers = 0;

    if (Water) {
        int32 offsetH = 0;
        int32 offsetV = 0;

        foreach_active(Water, water)
        {
            if (water->state == Water_State_BigBubble && water->activePlayers) {
                foreach_active(Fan, fan)
                {
                    Fan->hitboxTop.top = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;
                    if (fan->state == Fan_HandlePlayerInteractions_Top) {
                        if (MathHelpers_PointInHitbox(fan->position.x, fan->position.y, water->position.x, water->position.y, fan->direction,
                                                      &Fan->hitboxTop)) {
                            offsetV -= 0x20000;
                        }
                    }

                    Fan->hitboxBottom.top = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;
                    if (fan->state == Fan_HandlePlayerInteractions_Bottom) {
                        if (MathHelpers_PointInHitbox(fan->position.x, fan->position.y, water->position.x, water->position.y, fan->direction,
                                                      &Fan->hitboxBottom)) {
                            offsetV += 0x20000;
                        }
                    }

                    Fan->hitboxSides.left = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;
                    if (fan->state == Fan_HandlePlayerInteractions_Left) {
                        if (MathHelpers_PointInHitbox(fan->position.x, fan->position.y, water->position.x, water->position.y, fan->direction,
                                                      &Fan->hitboxSides)) {
                            offsetH -= 0x20000;
                        }
                    }
                    if (fan->state == Fan_HandlePlayerInteractions_Right) {
                        if (MathHelpers_PointInHitbox(fan->position.x, fan->position.y, water->position.x, water->position.y, fan->direction,
                                                      &Fan->hitboxSides)) {
                            offsetH += 0x20000;
                        }
                    }
                }

                offsetH = CLAMP(offsetH, -0x20000, 0x20000);
                offsetV = CLAMP(offsetV, -0x20000, 0x20000);

                if (water->bubbleOffset.x < offsetH) {
                    water->bubbleOffset.x += 0x800;

                    if (water->bubbleOffset.x > offsetH)
                        water->bubbleOffset.x = offsetH;
                }

                if (water->bubbleOffset.x > offsetH) {
                    water->bubbleOffset.x -= 0x800;

                    if (water->bubbleOffset.x < offsetH)
                        water->bubbleOffset.x = offsetH;
                }

                if (water->bubbleOffset.y < offsetV) {
                    water->bubbleOffset.y += 0x800;

                    if (water->bubbleOffset.y > offsetV)
                        water->bubbleOffset.y = offsetV;
                }

                if (water->bubbleOffset.y > offsetV) {
                    water->bubbleOffset.y -= 0x800;

                    if (water->bubbleOffset.y < offsetV)
                        water->bubbleOffset.y = offsetV;
                }
            }
        }
    }

    if (HangConveyor) {
        foreach_active(HangConveyor, conveyor)
        {
            if (conveyor->movementActivePlayers) {
                foreach_active(Player, player)
                {
                    int32 playerID = RSDK.GetEntitySlot(player);

                    if (((1 << playerID) & conveyor->movementActivePlayers)) {
                        bool32 fanning = false;

                        foreach_active(Fan, fan)
                        {
                            Fan->hitboxTop.top = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;

                            if (fan->state == Fan_HandlePlayerInteractions_Top) {
                                if (Player_CheckCollisionTouch(player, fan, &Fan->hitboxTop) && conveyor->fanTimer[playerID] < 12) {
                                    conveyor->fanTimer[playerID]++;
                                    fanning = true;
                                    foreach_break;
                                }
                            }
                        }

                        if (conveyor->fanTimer[playerID] > 0 && !fanning)
                            conveyor->fanTimer[playerID]--;
                    }
                }
            }
        }
    }

    if (RSDK.CheckSceneFolder("HCZ")) {
        int32 count = 0;

        if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID != PauseMenu->classID) {
            Vector2 range = { 0x400000, 0x400000 };
            foreach_active(Fan, fan)
            {
                uint16 tile = RSDK.GetTile(Zone->fgLayer[1], fan->position.x >> 20, fan->position.y >> 20);
                if (fan->state && fan->state != Fan_State_Stopped && tile == 0xFFFF && RSDK.CheckOnScreen(fan, &range))
                    ++count;
            }

            if (count) {
                if (!Fan->playingFanSfx) {
                    RSDK.PlaySfx(Fan->sfxFan, 37404, 0xFF);
                    Fan->playingFanSfx = true;
                }
            }
        }

        if (!count && Fan->playingFanSfx) {
            RSDK.StopSfx(Fan->sfxFan);
            Fan->playingFanSfx = false;
        }
    }
}

void Fan_Draw(void)
{
    RSDK_THIS(Fan);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Fan_Create(void *data)
{
    RSDK_THIS(Fan);

    self->drawFX = FX_FLIP;

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0] + 1;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        if (self->type == FAN_V)
            self->direction *= FLIP_Y;

        if (!self->size)
            self->size = 5;

        self->size *= 16;
        RSDK.SetSpriteAnimation(Fan->aniFrames, self->type, &self->animator, true, 0);
        self->state          = Fan_State_Stopped;
        self->animator.speed = 0;

        switch (self->activation) {
            case FAN_ACTIVATE_NONE:
                Fan_Activate();
                self->animator.speed = 128;
                break;

            case FAN_ACTIVATE_INTERVAL: self->stateActivate = Fan_Activate_Interval; break;

            case FAN_ACTIVATE_PLATFORM: self->stateActivate = Fan_Activate_Platform; break;

            case FAN_ACTIVATE_BUTTON:
                Fan_SetupTagLink();
                self->stateActivate = Fan_Activate_Button;
                break;

            default: break;
        }

        if (self->deactivation == FAN_DEACTIVATE_BUTTON) {
            Fan_SetupTagLink();
            self->stateDeactivate = Fan_Deactivate_Button;
        }
    }
}

void Fan_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("OOZ1") || RSDK.CheckSceneFolder("OOZ2")) {
        Fan->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Fan.bin", SCOPE_STAGE);

        Fan->hitboxTop.left  = -64;
        Fan->hitboxTop.right = 64;

        Fan->unused      = 5;
        Fan->minVelocity = -0x50000;

        Fan->hitboxBottom.left   = -16;
        Fan->hitboxBottom.right  = 16;
        Fan->hitboxBottom.top    = -160;
        Fan->hitboxBottom.bottom = 32;

        Fan->hitboxSides.top    = -160;
        Fan->hitboxSides.bottom = 112;
        Fan->hitboxSides.left   = -112;
        Fan->hitboxSides.right  = 32;
    }
    else if (RSDK.CheckSceneFolder("HCZ")) {
        Fan->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Fan.bin", SCOPE_STAGE);

        Fan->hitboxTop.left  = -16;
        Fan->hitboxTop.right = 16;

        Fan->unused      = 20;
        Fan->minVelocity = -0x20000;

        Fan->hitboxBottom.left   = -16;
        Fan->hitboxBottom.right  = 16;
        Fan->hitboxBottom.top    = -160;
        Fan->hitboxBottom.bottom = 32;

        Fan->hitboxSides.top    = -24;
        Fan->hitboxSides.bottom = 24;
        Fan->hitboxSides.left   = -112;
        Fan->hitboxSides.right  = 32;

        Fan->sfxFan = RSDK.GetSfx("HCZ/SmallFan.wav");
        Fan->active = ACTIVE_ALWAYS;
    }

    Fan->playerHitbox.left   = -1;
    Fan->playerHitbox.top    = -1;
    Fan->playerHitbox.right  = 1;
    Fan->playerHitbox.bottom = 1;
}

void Fan_SetupTagLink(void)
{
    RSDK_THIS(Fan);

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
                }
            }
        }

        if (PullChain && !matchedTag) {
            foreach_all(PullChain, chain)
            {
                if (chain->tag == self->buttonTag) {
                    taggedButton = (EntityButton *)chain;
                    matchedTag   = true;
                }
            }
        }
    }

    if (taggedButton) {
        if ((Button && taggedButton->classID == Button->classID) || (PullChain && taggedButton->classID == PullChain->classID)) {
            int32 distX = abs(self->position.x - taggedButton->position.x) + 0x800000;
            int32 distY = abs(self->position.y - taggedButton->position.y) + 0x800000;

            if (self->updateRange.x < distX)
                self->updateRange.x = distX;

            if (self->updateRange.y < distY)
                self->updateRange.y = distY;

            self->taggedButton = taggedButton;
        }
    }
}

void Fan_HandlePlayerInteractions_Top(void)
{
    RSDK_THIS(Fan);

    Fan->hitboxTop.top    = (RSDK.Sin256(2 * Zone->timer) >> 5) - self->size;
    Fan->hitboxTop.bottom = 48;

    int32 playerID = 1;
    foreach_active(Player, player)
    {
        if (player->state != Player_State_Static) {
            int32 anim = player->animator.animationID;
            if (anim != ANI_HURT && anim != ANI_DIE && anim != ANI_DROWN
                && RSDK.CheckObjectCollisionTouchBox(self, &Fan->hitboxTop, player, &Fan->playerHitbox)) {
                if (Water && player->position.y > Water->waterLevel)
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 1);
                else
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_CS, &player->animator, false, 1);

                player->state          = Player_State_Air;
                player->tileCollisions = TILECOLLISION_DOWN;
                player->onGround       = false;

                int32 velocity = MAX((self->position.y + (Fan->hitboxTop.top << 16) - player->position.y) >> 4, Fan->minVelocity);
                if (player->velocity.y <= velocity) {
                    player->velocity.y = velocity;
                }
                else {
                    player->velocity.y = player->velocity.y + (velocity >> 2) + (velocity >> 1);
                    if (player->velocity.y < velocity)
                        player->velocity.y = velocity;
                }

                if (!(playerID & Fan->activePlayers)) {
                    if (player->velocity.y > -0x40000 && player->velocity.y < 0)
                        player->velocity.x += (32 * player->velocity.x / 31) >> 5;

                    Fan->activePlayers |= playerID;
                }
            }
        }

        playerID <<= 1;
    }

    if (Water) {
        if (self->position.y > Water->waterLevel && !(Zone->timer & 3)) {
            EntityWater *water = CREATE_ENTITY(Water, INT_TO_VOID(WATER_BUBBLE), self->position.x, self->position.y - 0x100000);

            water->isPermanent = false;
            water->position.x += ZONE_RAND(-6, 7) << 16;
            water->bubbleX    = water->position.x;
            water->velocity.y = -0x40000;
            water->childPtr   = NULL;
        }
    }

    Fan_State_Started();
}

void Fan_HandlePlayerInteractions_Bottom(void)
{
    RSDK_THIS(Fan);

    Fan->hitboxBottom.bottom = self->size - (RSDK.Sin256(2 * Zone->timer) >> 5);

    foreach_active(Player, player)
    {
        if (player->state != Player_State_Static) {
            int32 anim = player->animator.animationID;

            if (anim != ANI_HURT && anim != ANI_DIE && anim != ANI_DROWN
                && RSDK.CheckObjectCollisionTouchBox(self, &Fan->hitboxBottom, player, &Fan->playerHitbox)) {
                int32 max = (self->position.y - player->position.y + 0xA00000) >> 4;
                if (player->velocity.y < max)
                    player->velocity.y += ((self->position.y - player->position.y + 0xA00000) >> 9);
            }
        }
    }

    Fan_State_Started();
}

void Fan_HandlePlayerInteractions_Left(void)
{
    RSDK_THIS(Fan);

    Fan->hitboxSides.left = (RSDK.Sin256(2 * Zone->timer) >> 5) - self->size;

    foreach_active(Player, player)
    {
        if (player->state != Player_State_Static) {
            int32 anim = player->animator.animationID;

            if (anim != ANI_HURT && anim != ANI_DIE && anim != ANI_DROWN && player->collisionMode != CMODE_LWALL
                && RSDK.CheckObjectCollisionTouchBox(self, &Fan->hitboxSides, player, &Fan->playerHitbox)) {
                player->position.x += (self->position.x - player->position.x - 0xA00000) >> 4;
            }
        }
    }

    Fan_State_Started();
}

void Fan_HandlePlayerInteractions_Right(void)
{
    RSDK_THIS(Fan);

    Fan->hitboxSides.left = (RSDK.Sin256(2 * Zone->timer) >> 5) - self->size;

    foreach_active(Player, player)
    {
        if (player->state != Player_State_Static) {
            int32 anim = player->animator.animationID;

            if (anim != ANI_HURT && anim != ANI_DIE && anim != ANI_DROWN) {
                if (player->collisionMode != CMODE_LWALL && player->collisionMode != CMODE_RWALL
                    && RSDK.CheckObjectCollisionTouchBox(self, &Fan->hitboxSides, player, &Fan->playerHitbox)) {
                    player->position.x += (self->position.x - player->position.x + 0xA00000) >> 4;
                }
            }
        }
    }

    Fan_State_Started();
}

void Fan_State_Started(void)
{
    RSDK_THIS(Fan);

    if (self->animator.speed < 0x80)
        self->animator.speed += 4;
}

void Fan_State_Stopped(void)
{
    RSDK_THIS(Fan);

    if (self->animator.speed)
        self->animator.speed -= 2;
}

void Fan_Activate_Interval(void)
{
    RSDK_THIS(Fan);

    if (self->delay) {
        if (!--self->delay)
            self->state = Fan_State_Stopped;
    }
    else {
        if (!((Zone->timer + self->intervalOffset) % self->interval) && self->state == Fan_State_Stopped) {
            self->active = ACTIVE_NORMAL;
            Fan_Activate();
        }
    }
}

void Fan_Activate_Button(void)
{
    RSDK_THIS(Fan);

    EntityButton *button = self->taggedButton;
    if ((!button || button->activated) && self->state == Fan_State_Stopped) {
        self->active = ACTIVE_NORMAL;
        Fan_Activate();
    }
}

void Fan_Deactivate_Button(void)
{
    RSDK_THIS(Fan);

    EntityButton *button = self->taggedButton;
    if ((!button || button->activated) && self->state != Fan_State_Stopped) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Fan_State_Stopped;
    }
}

void Fan_Activate_Platform(void)
{
    RSDK_THIS(Fan);

    int32 slot               = SceneInfo->entitySlot - 1;
    EntityPlatform *platform = RSDK_GET_ENTITY(slot, Platform);
    while (platform->classID == Fan->classID) {
        --slot;
        platform = RSDK_GET_ENTITY(slot, Platform);
    }

    if (platform->classID == Platform->classID) {
        if (platform->amplitude.y == platform->amplitude.x)
            self->state = Fan_State_Stopped;
        else if (self->state == Fan_State_Stopped)
            Fan_Activate();
    }
}

void Fan_Activate(void)
{
    RSDK_THIS(Fan);

    if (self->type != FAN_V) {
        if (self->direction == FLIP_NONE)
            self->state = Fan_HandlePlayerInteractions_Left;
        else
            self->state = Fan_HandlePlayerInteractions_Right;

        self->delay = self->duration;
    }
    else {
        if (self->direction == FLIP_NONE)
            self->state = Fan_HandlePlayerInteractions_Top;
        else
            self->state = Fan_HandlePlayerInteractions_Bottom;

        self->delay = self->duration;
    }
}

#if GAME_INCLUDE_EDITOR
void Fan_EditorDraw(void)
{
    RSDK_THIS(Fan);

    RSDK.SetSpriteAnimation(Fan->aniFrames, self->type, &self->animator, true, 0);

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    int32 dir = self->direction;
    if (self->type == FAN_V)
        self->direction *= FLIP_Y;

    Fan_Draw();

    self->direction = dir;

    if (showGizmos()) {
        if (self->activation == FAN_ACTIVATE_BUTTON || self->deactivation == FAN_DEACTIVATE_BUTTON) {
            Fan_SetupTagLink();

            RSDK_DRAWING_OVERLAY(true);
            if (self->taggedButton) {
                DrawHelpers_DrawArrow(self->taggedButton->position.x, self->taggedButton->position.y, self->position.x, self->position.y, 0xFFFF00,
                                      INK_NONE, 0xFF);
            }
            RSDK_DRAWING_OVERLAY(false);
        }
    }
}

void Fan_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("OOZ1") || RSDK.CheckSceneFolder("OOZ2"))
        Fan->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Fan.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("HCZ"))
        Fan->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Fan.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Fan, type);
    RSDK_ENUM_VAR("Vertical", FAN_V);
    RSDK_ENUM_VAR("Horiozontal", FAN_H);

    RSDK_ACTIVE_VAR(Fan, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flipped", FLIP_X);

    RSDK_ACTIVE_VAR(Fan, activation);
    RSDK_ENUM_VAR("None", FAN_ACTIVATE_NONE);
    RSDK_ENUM_VAR("On Interval", FAN_ACTIVATE_INTERVAL);
    RSDK_ENUM_VAR("On Platform Moved", FAN_ACTIVATE_PLATFORM);
    RSDK_ENUM_VAR("On Button Press", FAN_ACTIVATE_BUTTON);

    RSDK_ACTIVE_VAR(Fan, deactivation);
    RSDK_ENUM_VAR("None", FAN_DEACTIVATE_NONE);
    RSDK_ENUM_VAR("On Button Press", FAN_DEACTIVATE_BUTTON);
}
#endif

void Fan_Serialize(void)
{
    RSDK_EDITABLE_VAR(Fan, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Fan, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Fan, VAR_UINT8, activation);
    RSDK_EDITABLE_VAR(Fan, VAR_UINT8, deactivation);
    RSDK_EDITABLE_VAR(Fan, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(Fan, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(Fan, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(Fan, VAR_UINT16, duration);
    RSDK_EDITABLE_VAR(Fan, VAR_ENUM, buttonTag);
}
