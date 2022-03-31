// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Fan Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
            if (water->state == Water_State_HCZBubble && water->activePlayers) {
                foreach_active(Fan, fan)
                {
                    Fan->hitboxTop.top = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;
                    if (fan->state == Fan_HandlePlayerInteractions_Top) {
                        if (MathHelpers_PointInHitbox(fan->direction, fan->position.x, fan->position.y, &Fan->hitboxTop, water->position.x,
                                                      water->position.y)) {
                            offsetV -= 0x20000;
                        }
                    }

                    Fan->hitboxBottom.top = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;
                    if (fan->state == Fan_HandlePlayerInteractions_Bottom) {
                        if (MathHelpers_PointInHitbox(fan->direction, fan->position.x, fan->position.y, &Fan->hitboxBottom, water->position.x,
                                                      water->position.y)) {
                            offsetV += 0x20000;
                        }
                    }

                    Fan->hitboxSides.left = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;
                    if (fan->state == Fan_HandlePlayerInteractions_Left) {
                        if (MathHelpers_PointInHitbox(fan->direction, fan->position.x, fan->position.y, &Fan->hitboxSides, water->position.x,
                                                      water->position.y)) {
                            offsetH -= 0x20000;
                        }
                    }
                    if (fan->state == Fan_HandlePlayerInteractions_Right) {
                        if (MathHelpers_PointInHitbox(fan->direction, fan->position.x, fan->position.y, &Fan->hitboxSides, water->position.x,
                                                      water->position.y)) {
                            offsetH += 0x20000;
                        }
                    }
                }

                offsetH = clampVal(offsetH, -0x20000, 0x20000);
                offsetV = clampVal(offsetV, -0x20000, 0x20000);

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
                    int32 playerID = RSDK.GetEntityID(player);
                    if (((1 << playerID) & conveyor->movementActivePlayers)) {
                        bool32 fanning = false;
                        foreach_active(Fan, fan)
                        {
                            Fan->hitboxTop.top = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;
                            if (fan->state == Fan_HandlePlayerInteractions_Top) {
                                if (Player_CheckCollisionTouch(player, fan, &Fan->hitboxTop)) {
                                    if (conveyor->fanTimer[playerID] < 12) {
                                        conveyor->fanTimer[playerID]++;
                                        fanning = true;
                                        foreach_break;
                                    }
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

    if (RSDK.CheckStageFolder("HCZ")) {
        int32 count = 0;
        if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {
            Vector2 range;
            range.x = 0x400000;
            range.y = 0x400000;
            foreach_active(Fan, fan)
            {
                uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, fan->position.x >> 20, fan->position.y >> 20);
                if (fan->state && fan->state != Fan_State_Stopped && tile == 0xFFFF && RSDK.CheckOnScreen(fan, &range))
                    ++count;
            }

            if (count) {
                if (!Fan->playingFanSFX) {
                    RSDK.PlaySfx(Fan->sfxFan, 37404, 0xFF);
                    Fan->playingFanSFX = true;
                }
            }
        }

        if (!count && Fan->playingFanSFX) {
            RSDK.StopSfx(Fan->sfxFan);
            Fan->playingFanSFX = false;
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
        self->drawOrder     = Zone->drawOrderLow + 1;
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
                Fan_SetupTagLinks();
                self->stateActivate = Fan_Activate_Button;
                break;

            default: break;
        }

        if (self->deactivation == FAN_DEACTIVATE_BUTTON) {
            Fan_SetupTagLinks();
            self->stateDeactivate = Fan_Deactivate_Button;
        }
    }
}

void Fan_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2")) {
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
    else if (RSDK.CheckStageFolder("HCZ")) {
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

void Fan_SetupTagLinks(void)
{
    RSDK_THIS(Fan);
    EntityButton *buttonPtr = RSDK.GetEntityByID(RSDK.GetEntityID(self) - 1);
    if (self->buttonTag > 0) {
        bool32 flag = false;
        if (Button) {
            foreach_all(Button, button)
            {
                if (button->tag == self->buttonTag) {
                    buttonPtr = button;
                }
            }
        }

        if (PullChain && !flag) {
            foreach_all(PullChain, chain)
            {
                if (chain->tag == self->buttonTag) {
                    buttonPtr = (EntityButton *)chain;
                }
            }
        }
    }

    if ((Button && buttonPtr->objectID == Button->objectID) || (PullChain && buttonPtr->objectID == PullChain->objectID)) {
        if (self->updateRange.y < 0x800000 + abs(self->position.y - buttonPtr->position.y))
            self->updateRange.y = 0x800000 + abs(self->position.y - buttonPtr->position.y);
        if (self->updateRange.y < 0x800000 + abs(self->position.y - buttonPtr->position.y))
            self->updateRange.y = 0x800000 + abs(self->position.y - buttonPtr->position.y);

        self->buttonPtr = buttonPtr;
    }
    else {
        self->buttonPtr = NULL;
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
        if (player->state != Player_State_None) {
            int32 anim = player->animator.animationID;
            if (anim != ANI_HURT && anim != ANI_DIE && anim != ANI_DROWN
                && RSDK.CheckObjectCollisionTouchBox(self, &Fan->hitboxTop, player, &Fan->playerHitbox)) {
                if (Water && player->position.y > Water->waterLevel)
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 1);
                else
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, false, 1);

                player->state          = Player_State_Air;
                player->tileCollisions = true;
                player->onGround       = false;

                int32 velocity = maxVal((self->position.y + (Fan->hitboxTop.top << 16) - player->position.y) >> 4, Fan->minVelocity);
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
            EntityWater *water = CREATE_ENTITY(Water, intToVoid(7), self->position.x, self->position.y - 0x100000);
            water->isPermanent = false;
#if RETRO_USE_PLUS
            water->position.x += RSDK.RandSeeded(-6, 7, &Zone->randSeed) << 16;
#else
            water->position.x += RSDK.Rand(-6, 7) << 16;
#endif
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
        if (player->state != Player_State_None) {
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
        if (player->state != Player_State_None) {
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
        if (player->state != Player_State_None) {
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

    EntityButton *button = self->buttonPtr;
    if ((!button || button->activated) && self->state == Fan_State_Stopped) {
        self->active = ACTIVE_NORMAL;
        Fan_Activate();
    }
}

void Fan_Deactivate_Button(void)
{
    RSDK_THIS(Fan);
    EntityButton *button = self->buttonPtr;
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
    while (platform->objectID == Fan->objectID) {
        --slot;
        platform = RSDK_GET_ENTITY(slot, Platform);
    }

    if (platform->objectID == Platform->objectID) {
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

#if RETRO_INCLUDE_EDITOR
void Fan_EditorDraw(void)
{
    RSDK_THIS(Fan);
    RSDK.SetSpriteAnimation(Fan->aniFrames, self->type, &self->animator, true, 0);

    int32 dir = self->direction;
    if (self->type == FAN_V)
        self->direction *= FLIP_Y;

    Fan_Draw();

    self->direction = dir;
}

void Fan_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        Fan->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Fan.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("HCZ"))
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
