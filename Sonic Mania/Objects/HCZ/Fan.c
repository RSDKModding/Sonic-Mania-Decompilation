#include "SonicMania.h"

ObjectFan *Fan;

void Fan_Update(void)
{
    RSDK_THIS(Fan);
    StateMachine_Run(self->state3);
    StateMachine_Run(self->state2);
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
                    Fan->hitbox1.top = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;
                    if (fan->state == Fan_HandlePlayerInteractions_Top) {
                        if (MathHelpers_PointInHitbox(fan->direction, fan->position.x, fan->position.y, &Fan->hitbox1, water->position.x,
                                                      water->position.y)) {
                            offsetV -= 0x20000;
                        }
                    }

                    Fan->hitbox2.top = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;
                    if (fan->state == Fan_HandlePlayerInteractions_Bottom) {
                        if (MathHelpers_PointInHitbox(fan->direction, fan->position.x, fan->position.y, &Fan->hitbox2, water->position.x,
                                                      water->position.y)) {
                            offsetV += 0x20000;
                        }
                    }

                    Fan->hitbox3.left = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;
                    if (fan->state == Fan_HandlePlayerInteractions_Left) {
                        if (MathHelpers_PointInHitbox(fan->direction, fan->position.x, fan->position.y, &Fan->hitbox3, water->position.x,
                                                      water->position.y)) {
                            offsetH -= 0x20000;
                        }
                    }
                    if (fan->state == Fan_HandlePlayerInteractions_Right) {
                        if (MathHelpers_PointInHitbox(fan->direction, fan->position.x, fan->position.y, &Fan->hitbox3, water->position.x,
                                                      water->position.y)) {
                            offsetH += 0x20000;
                        }
                    }
                }

                offsetH = clampVal(offsetH, -0x20000, 0x20000);
                offsetV = clampVal(offsetV, -0x20000, 0x20000);

                if (water->bubbleOffset.x < offsetH) {
                    int32 val = water->bubbleOffset.x + 0x800;
                    if (val < offsetH)
                        water->bubbleOffset.y = val;
                    else
                        water->bubbleOffset.y = offsetH;
                }

                if (water->bubbleOffset.x > offsetH) {
                    int32 val = water->bubbleOffset.x - 0x800;
                    if (val > offsetH)
                        water->bubbleOffset.y = val;
                    else
                        water->bubbleOffset.y = offsetH;
                }

                if (water->bubbleOffset.y < offsetV) {
                    int32 val = water->bubbleOffset.y + 0x800;
                    if (val < offsetV)
                        water->bubbleOffset.y = val;
                    else
                        water->bubbleOffset.y = offsetV;
                }

                if (water->bubbleOffset.y > offsetV) {
                    int32 val = water->bubbleOffset.y - 0x800;
                    if (val > offsetV)
                        water->bubbleOffset.y = val;
                    else
                        water->bubbleOffset.y = offsetV;
                }
            }
        }
    }
    if (HangConveyor) {
        foreach_active(HangConveyor, conveyor)
        {
            if (conveyor->activePlayers1) {
                foreach_active(Player, player)
                {
                    int32 playerID = RSDK.GetEntityID(player);
                    if (((1 << playerID) & conveyor->activePlayers1)) {
                        bool32 flag = false;
                        foreach_active(Fan, fan)
                        {
                            Fan->hitbox1.top = (RSDK.Sin256(2 * Zone->timer) >> 5) - fan->size;
                            if (fan->state == Fan_HandlePlayerInteractions_Top) {
                                if (Player_CheckCollisionTouch(player, fan, &Fan->hitbox1)) {
                                    if (conveyor->field_C4[playerID] < 12) {
                                        conveyor->field_C4[playerID]++;
                                        flag = true;
                                        foreach_break;
                                    }
                                }
                            }
                        }

                        if (conveyor->field_C4[playerID] > 0 && !flag) {
                            conveyor->field_C4[playerID]--;
                        }
                    }
                }
            }
        }
    }

    if (RSDK.CheckStageFolder("HCZ")) {
        Vector2 range;
        range.x = 0x400000;
        range.y = 0x400000;
        if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {
            int32 count = 0;
            foreach_active(Fan, fan)
            {
                uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, fan->position.x >> 20, fan->position.y >> 20);
                if (fan->state && fan->state != Fan_ProcessAnimationSpeed_Slow && tile == 0xFFFF && RSDK.CheckOnScreen(fan, &range))
                    ++count;
            }

            if (count) {
                if (!Fan->playingFanSFX) {
                    RSDK.PlaySfx(Fan->sfxFan, 37404, 0xFF);
                    Fan->playingFanSFX = true;
                }
                return;
            }
        }
        if (Fan->playingFanSFX) {
            RSDK.StopSFX(Fan->sfxFan);
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
        if (!self->type)
            self->direction *= FLIP_Y;
        if (!self->size)
            self->size = 5;
        self->size *= 16;
        RSDK.SetSpriteAnimation(Fan->aniFrames, self->type, &self->animator, true, 0);
        self->state                   = Fan_ProcessAnimationSpeed_Slow;
        self->animator.animationSpeed = 0;
        switch (self->activation) {
            case 0:
                Fan_Unknown11();
                self->animator.animationSpeed = 128;
                break;
            case 1: self->state3 = Fan_HandleDurationTimer; break;
            case 2: self->state3 = Fan_Unknown10; break;
            case 3:
                Fan_SetupTagLinks();
                self->state3 = Fan_Unknown8;
                break;
            default: break;
        }
        if (self->deactivation == 1) {
            Fan_SetupTagLinks();
            self->state2 = Fan_Unknown9;
        }
    }
}

void Fan_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2")) {
        Fan->aniFrames      = RSDK.LoadSpriteAnimation("OOZ/Fan.bin", SCOPE_STAGE);
        Fan->hitbox1.left   = -64;
        Fan->hitbox1.right  = 64;
        Fan->field_8        = 5;
        Fan->field_C        = -0x50000;
        Fan->hitbox2.left   = -16;
        Fan->hitbox2.right  = 16;
        Fan->hitbox2.top    = -160;
        Fan->hitbox2.bottom = 32;
        Fan->hitbox3.top    = -160;
        Fan->hitbox3.bottom = 112;
        Fan->hitbox3.left   = -112;
        Fan->hitbox3.right  = 32;
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Fan->aniFrames      = RSDK.LoadSpriteAnimation("HCZ/Fan.bin", SCOPE_STAGE);
        Fan->hitbox1.left   = -16;
        Fan->hitbox1.right  = 16;
        Fan->field_8        = 20;
        Fan->field_C        = -0x20000;
        Fan->hitbox2.left   = -16;
        Fan->hitbox2.right  = 16;
        Fan->hitbox2.top    = -160;
        Fan->hitbox2.bottom = 32;
        Fan->hitbox3.top    = -24;
        Fan->hitbox3.bottom = 24;
        Fan->hitbox3.left   = -112;
        Fan->hitbox3.right  = 32;
        Fan->sfxFan         = RSDK.GetSFX("HCZ/SmallFan.wav");
        Fan->active         = ACTIVE_ALWAYS;
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
    Fan->hitbox1.top    = (RSDK.Sin256(2 * Zone->timer) >> 5) - self->size;
    Fan->hitbox1.bottom = 48;

    int32 id = 1;
    foreach_active(Player, player)
    {
        if (player->state != Player_State_None) {
            int32 anim = player->animator.animationID;
            if (anim != ANI_HURT && anim != ANI_DIE && anim != ANI_DROWN
                && RSDK.CheckObjectCollisionTouchBox(self, &Fan->hitbox1, player, &Fan->playerHitbox)) {
                if (Water && player->position.y > Water->waterLevel)
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 1);
                else
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, false, 1);

                player->state          = Player_State_Air;
                player->tileCollisions = true;
                player->onGround       = false;

                int32 vel = maxVal((self->position.y + (Fan->hitbox1.top << 16) - player->position.y) >> 4, Fan->field_C);
                if (player->velocity.y <= vel) {
                    player->velocity.y = vel;
                }
                else {
                    player->velocity.y = player->velocity.y + (vel >> 2) + (vel >> 1);
                    if (player->velocity.y < vel)
                        player->velocity.y = vel;
                }

                if (!(id & Fan->activePlayers)) {
                    if (player->velocity.y > -0x40000 && player->velocity.y < 0) {
                        player->velocity.x += (32 * player->velocity.x / 31) >> 5;
                    }
                    Fan->activePlayers |= id;
                }
            }
        }
        id <<= 1;
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
            water->bubbleX   = water->position.x;
            water->velocity.y = -0x40000;
            water->childPtr   = NULL;
        }
    }
    Fan_ProcessAnimationSpeed_Fast();
}

void Fan_HandlePlayerInteractions_Bottom(void)
{
    RSDK_THIS(Fan);
    Fan->hitbox2.bottom = self->size - (RSDK.Sin256(2 * Zone->timer) >> 5);

    foreach_active(Player, player)
    {
        if (player->state != Player_State_None) {
            int32 anim = player->animator.animationID;
            if (anim != ANI_HURT && anim != ANI_DIE && anim != ANI_DROWN
                && RSDK.CheckObjectCollisionTouchBox(self, &Fan->hitbox2, player, &Fan->playerHitbox)) {
                int32 vel = player->velocity.y;
                int32 max = (self->position.y - player->position.y + 0xA00000) >> 4;
                if (vel < max)
                    player->velocity.y = vel + ((self->position.y - player->position.y + 0xA00000) >> 9);
            }
        }
    }

    Fan_ProcessAnimationSpeed_Fast();
}

void Fan_HandlePlayerInteractions_Left(void)
{
    RSDK_THIS(Fan);
    Fan->hitbox3.left = (RSDK.Sin256(2 * Zone->timer) >> 5) - self->size;

    foreach_active(Player, player)
    {
        if (player->state != Player_State_None) {
            int32 anim = player->animator.animationID;
            if (anim != ANI_HURT && anim != ANI_DIE && anim != ANI_DROWN && player->collisionMode != CMODE_LWALL
                && RSDK.CheckObjectCollisionTouchBox(self, &Fan->hitbox3, player, &Fan->playerHitbox)) {
                player->position.x += (self->position.x - player->position.x - 0xA00000) >> 4;
            }
        }
    }

    Fan_ProcessAnimationSpeed_Fast();
}

void Fan_HandlePlayerInteractions_Right(void)
{
    RSDK_THIS(Fan);
    Fan->hitbox3.left = (RSDK.Sin256(2 * Zone->timer) >> 5) - self->size;

    foreach_active(Player, player)
    {
        if (player->state != Player_State_None) {
            int32 anim = player->animator.animationID;
            if (anim != ANI_HURT && anim != ANI_DIE && anim != ANI_DROWN) {
                if (player->collisionMode != CMODE_LWALL && player->collisionMode != CMODE_RWALL
                    && RSDK.CheckObjectCollisionTouchBox(self, &Fan->hitbox3, player, &Fan->playerHitbox)) {
                    player->position.x += (self->position.x - player->position.x + 0xA00000) >> 4;
                }
            }
        }
    }

    Fan_ProcessAnimationSpeed_Fast();
}

void Fan_ProcessAnimationSpeed_Fast(void)
{
    RSDK_THIS(Fan);
    if (self->animator.animationSpeed < 0x80)
        self->animator.animationSpeed += 4;
}

void Fan_ProcessAnimationSpeed_Slow(void)
{
    RSDK_THIS(Fan);
    if (self->animator.animationSpeed)
        self->animator.animationSpeed -= 2;
}

void Fan_HandleDurationTimer(void)
{
    RSDK_THIS(Fan);
    if (self->durationStore) {
        self->durationStore--;
        if (!self->durationStore)
            self->state = Fan_ProcessAnimationSpeed_Slow;
    }
    else {
        if (!((Zone->timer + self->intervalOffset) % self->interval) && self->state == Fan_ProcessAnimationSpeed_Slow) {
            self->active = ACTIVE_NORMAL;
            Fan_Unknown11();
        }
    }
}

void Fan_Unknown8(void)
{
    RSDK_THIS(Fan);
    EntityButton *button = self->buttonPtr;
    if ((!button || button->activated) && self->state == Fan_ProcessAnimationSpeed_Slow) {
        self->active = ACTIVE_NORMAL;
        Fan_Unknown11();
    }
}

void Fan_Unknown9(void)
{
    RSDK_THIS(Fan);
    EntityButton *button = self->buttonPtr;
    if ((!button || button->activated) && self->state != Fan_ProcessAnimationSpeed_Slow) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Fan_ProcessAnimationSpeed_Slow;
    }
}

void Fan_Unknown10(void)
{
    RSDK_THIS(Fan);
    int32 slot                 = SceneInfo->entitySlot - 1;
    EntityPlatform *platform = RSDK_GET_ENTITY(slot, Platform);
    while (platform->objectID == Fan->objectID) {
        --slot;
        platform = RSDK_GET_ENTITY(slot, Platform);
    }

    if (platform->objectID == Platform->objectID) {
        if (platform->amplitude.y == platform->amplitude.x) {
            self->state = Fan_ProcessAnimationSpeed_Slow;
        }
        else if (self->state == Fan_ProcessAnimationSpeed_Slow) {
            Fan_Unknown11();
        }
    }
}

void Fan_Unknown11(void)
{
    RSDK_THIS(Fan);
    if (self->type) {
        if (self->direction == FLIP_NONE)
            self->state = Fan_HandlePlayerInteractions_Left;
        else
            self->state = Fan_HandlePlayerInteractions_Right;
        self->durationStore = self->duration;
    }
    else {
        if (self->direction == FLIP_NONE)
            self->state = Fan_HandlePlayerInteractions_Top;
        else
            self->state = Fan_HandlePlayerInteractions_Bottom;
        self->durationStore = self->duration;
    }
}

#if RETRO_INCLUDE_EDITOR
void Fan_EditorDraw(void)
{
    RSDK_THIS(Fan);
    RSDK.SetSpriteAnimation(Fan->aniFrames, self->type, &self->animator, true, 0);

    int dir = self->direction;
    if (!self->type)
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
