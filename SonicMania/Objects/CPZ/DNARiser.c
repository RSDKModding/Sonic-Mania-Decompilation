// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DNARiser Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDNARiser *DNARiser;

void DNARiser_Update(void)
{
    RSDK_THIS(DNARiser);

    if (self->scaleTimer)
        self->scaleTimer--;

    if (self->sfxTimer > 0)
        self->sfxTimer--;

    StateMachine_Run(self->state);

    if (self->popped) {
        if (self->popAnimator.frameID == self->popAnimator.loopIndex)
            self->popped = false;
        else
            RSDK.ProcessAnimation(&self->popAnimator);
    }
}

void DNARiser_LateUpdate(void) {}

void DNARiser_StaticUpdate(void)
{
    foreach_active(DNARiser, entity) { RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(entity)); }
}

void DNARiser_Draw(void)
{
    RSDK_THIS(DNARiser);

    StateMachine_Run(self->stateDraw);
}

void DNARiser_Create(void *data)
{
    RSDK_THIS(DNARiser);
    self->visible   = true;
    self->active    = ACTIVE_BOUNDS;
    self->drawGroup = Zone->objectDrawGroup[0] + 1;
    self->amplitude = 0x2E0000;

    if (SceneInfo->inEditor) {
        self->speed.x = 0;
        if (!self->speed.y)
            self->speed.y = 0x60000;
        if (!self->height)
            self->height = 128;
    }

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = DNARiser_State_Init;
}

void DNARiser_StageLoad(void)
{
    DNARiser->aniFrames = RSDK.LoadSpriteAnimation("CPZ/DNARiser.bin", SCOPE_STAGE);

    DNARiser->hitbox.top    = -16;
    DNARiser->hitbox.bottom = 16;
    DNARiser->hitbox.left   = -16;
    DNARiser->hitbox.right  = 16;

    DNARiser->active = ACTIVE_ALWAYS;

    DNARiser->sfxBurst   = RSDK.GetSfx("CPZ/DNABurst.wav");
    DNARiser->sfxGrab    = RSDK.GetSfx("CPZ/DNAGrab.wav");
    DNARiser->sfxScan    = RSDK.GetSfx("CPZ/DNAScan.wav");
    DNARiser->sfxTiny[0] = RSDK.GetSfx("CPZ/DNATiny0.wav");
    DNARiser->sfxTiny[1] = RSDK.GetSfx("CPZ/DNATiny1.wav");
    DNARiser->sfxTiny[2] = RSDK.GetSfx("CPZ/DNATiny2.wav");
    DNARiser->sfxTiny[3] = RSDK.GetSfx("CPZ/DNATiny3.wav");
    DNARiser->sfxTiny[4] = RSDK.GetSfx("CPZ/DNATiny4.wav");
    DNARiser->sfxTiny[5] = RSDK.GetSfx("CPZ/DNATiny5.wav");
}

void DNARiser_State_BubbleBurst(void)
{
    RSDK_THIS(DNARiser);

    RSDK.PlaySfx(DNARiser->sfxBurst, false, 255);

    self->popPos = self->position;
    self->popped = true;
    RSDK.SetSpriteAnimation(DNARiser->aniFrames, 5, &self->popAnimator, true, 0);
}

void DNARiser_SetupBurst(void)
{
    RSDK_THIS(DNARiser);

    DNARiser_State_BubbleBurst();

    self->maxSpeed        = self->speed;
    self->noBubbleScaling = true;
    self->velocity.x      = 0;
    self->velocity.y      = 0;
    self->state           = DNARiser_State_HelixBurst;
    self->timer           = 0;
}

Vector2 DNARiser_CalculateScale(Vector2 *scalePtr)
{
    RSDK_THIS(DNARiser);

    Vector2 scale;
    int32 x = 0x200;
    int32 y = 0x200;
    if (scalePtr) {
        x = scalePtr->x;
        y = scalePtr->y;
    }

    int32 timer = (30 - self->scaleTimer);
    int32 sine  = RSDK.Sin1024(((timer * (0x40000000 / ((10983 * timer + 286520) >> 7))) >> 11) & 0x3FF);
    scale.x     = (((timer - 30) * (sine << 6) / 100 + 0x10000) * x) >> 16;
    scale.y     = (((30 - timer) * (sine << 6) / 100 + 0x10000) * y) >> 16;
    return scale;
}

void DNARiser_State_Init(void)
{
    RSDK_THIS(DNARiser);

    self->startPos        = self->position;
    self->active          = ACTIVE_BOUNDS;
    self->speed.x         = 0;
    self->velocity.x      = 0;
    self->speed.y         = -abs(self->speed.y);
    self->timer           = 0;
    self->risePos         = 0;
    self->canDestroySelf  = false;
    self->firstChild      = NULL;
    self->child           = 0;
    self->helixPos        = 0;
    self->activePlayers   = 0;
    self->noBubbleScaling = false;
    self->lastSfxID       = 0;
    self->sfxTimer        = 0;
    self->scanSfxDir      = 0;
    self->velocity.y      = 0;
    self->stateDraw       = DNARiser_Draw_Main;
    self->state           = DNARiser_State_HandleInteractions;
    self->bubbleScale     = 0x200;
    RSDK.SetSpriteAnimation(DNARiser->aniFrames, 0, &self->bubbleAnimator, true, 0);
    DNARiser_State_HandleInteractions();
}

void DNARiser_State_HandleInteractions(void)
{
    RSDK_THIS(DNARiser);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (!((1 << playerID) & self->activePlayers)) {
            if (Player_CheckCollisionTouch(player, self, &DNARiser->hitbox)) {
                RSDK.PlaySfx(DNARiser->sfxGrab, false, 255);
                self->scaleTimer = 30;
                self->sfxTimer   = 30;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                player->groundVel        = 0;
                player->drawGroup        = 1;
                player->jumpAbilityState = 1;
                player->onGround         = false;
                player->tileCollisions   = TILECOLLISION_NONE;
                player->state            = Player_State_Static;
                player->nextAirState     = StateMachine_None;
                player->nextGroundState  = StateMachine_None;
                self->activePlayers |= 1 << playerID;
            }
        }

        if (((1 << playerID) & self->activePlayers)) {
            if (player->state == Player_State_Static) {
                player->position.x = self->position.x;
                player->position.y = self->position.y;
                player->velocity.x = 0;
                player->velocity.y = 0;
                if (!player->sidekick) {
                    self->risePos = 0;
                    self->active  = ACTIVE_NORMAL;
                    self->state   = DNARiser_State_HelixRise;
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void DNARiser_State_HelixRise(void)
{
    RSDK_THIS(DNARiser);
    if (self->risePos >= self->height << 16) {
        DNARiser_SetupBurst();
    }
    else {
        if (self->velocity.y >= abs(self->speed.y))
            self->velocity.y = abs(self->speed.y);
        else
            self->velocity.y += 0x1200;

        self->risePos += self->velocity.y;
        if (self->height << 16 < self->risePos)
            self->risePos = self->height << 16;

        int32 angle = (((0x57262 * (self->risePos >> 16)) >> 16) + 250) & 0x3FF;
        if (angle >= 0x200 && !self->scanSfxDir) {
            self->scanSfxDir = 1;
            RSDK.PlaySfx(DNARiser->sfxScan, false, 255);
        }
        else if (angle <= 0x1FF && self->scanSfxDir) {
            self->scanSfxDir = 0;
            RSDK.PlaySfx(DNARiser->sfxScan, false, 255);
        }

        self->position.y = self->startPos.y - self->risePos;
        for (; self->helixPos <= (self->risePos >> 18) + 1; ++self->helixPos) {
            if (!(self->helixPos & 1) || !(self->helixPos % 5)) {
                if (!self->sfxTimer) {
                    int32 sfxID = 0;
                    if (!self->lastSfxID) {
                        while (sfxID == self->lastSfxID) sfxID = ZONE_RAND(0, 5);
                    }

                    RSDK.PlaySfx(DNARiser->sfxTiny[sfxID], false, 255);
                    self->lastSfxID = sfxID;
                    self->sfxTimer  = ZONE_RAND(2, 8);
                }

                EntityDNARiser *child = CREATE_ENTITY(DNARiser, self, self->startPos.x, self->startPos.y - (self->helixPos << 18));
                child->state          = DNARiser_State_OrbSetup;
                child->active         = ACTIVE_NORMAL;
                child->startHelixPos  = self->helixPos;
                child->parent         = self;
                child->angle          = (((0x57262 * (4 * self->helixPos)) >> 16) + 100) & 0x3FF;
                child->amplitude      = 0x2E0000;
                child->drawGroup      = self->drawGroup - 1;
                if (!self->firstChild)
                    self->firstChild = child;

                if (self->child) {
                    child->lastChild     = self->child;
                    self->child->sibling = child;
                }
                self->child = child;
            }
        }
    }

    bool32 popped     = false;
    self->bubbleScale = (0x10000 - 85 * ((self->risePos / self->height) >> 8)) >> 7;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if (!((1 << playerID) & self->activePlayers)) {
            if (Player_CheckCollisionTouch(player, self, &DNARiser->hitbox)) {
                RSDK.PlaySfx(DNARiser->sfxGrab, false, 255);
                self->sfxTimer   = 30;
                self->scaleTimer = 30;
                self->activePlayers |= (1 << playerID);
            }
        }

        if ((1 << playerID) & self->activePlayers) {
            bool32 playerActive = false;
            if (!Player_CheckValidState(player)) {
                self->activePlayers &= ~(1 << playerID);
                if (self->activePlayers)
                    playerActive = true;
                else
                    DNARiser_SetupBurst();
            }

            if (!playerActive) {
                if (player->jumpPress) {
                    player->velocity.y = 0;
                    player->velocity.x = 0;
                    Player_Action_Jump(player);
                    DNARiser_SetupBurst();
                }

                if (self->state == DNARiser_State_HelixBurst) {
                    popped = true;
                }
                else {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->groundVel        = 0;
                    player->drawGroup        = 1;
                    player->jumpAbilityState = 1;
                    player->onGround         = false;
                    player->tileCollisions   = TILECOLLISION_NONE;
                    player->state            = Player_State_Static;
                    player->nextAirState     = StateMachine_None;
                    player->nextGroundState  = StateMachine_None;
                    player->velocity.x       = 0;
                    player->velocity.y       = -self->velocity.y;
                    player->position.x       = self->position.x;
                    player->position.y       = self->position.y;
                }
            }
        }
    }

    if (popped) {
        foreach_active(Player, playerPtr)
        {
            int32 playerID = RSDK.GetEntitySlot(playerPtr);
            if (((1 << playerID) & self->activePlayers)) {
                playerPtr->drawGroup      = Zone->playerDrawGroup[0];
                playerPtr->tileCollisions = TILECOLLISION_DOWN;
                playerPtr->state          = Player_State_Air;
                playerPtr->velocity.x     = self->speed.x;
                playerPtr->velocity.y     = self->speed.y;
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void DNARiser_State_HelixBurst(void)
{
    RSDK_THIS(DNARiser);

    if (self->timer <= 0) {
        if (!self->timer) {
            if (!self->canDestroySelf) {
                EntityDNARiser *child = self->child;
                if (child) {
                    self->canDestroySelf = true;
                    child->timer         = 0;
                    child->velocity.x    = 0;
                    child->state         = DNARiser_State_OrbFall;
                    child->velocity.y    = self->maxSpeed.y >> 1;
                    child->maxSpeed.y    = self->maxSpeed.y >> 1;
                }
            }
            if (self->position.y < self->startPos.y) {
                self->velocity.y += 0x38000;
                self->position.y += self->velocity.y;
            }

            int32 posY = self->startPos.y;
            if (self->position.y < self->startPos.y)
                posY = self->position.y;

            self->position.y = posY;
            self->risePos    = self->startPos.y - posY;
            if (posY == self->startPos.y) {
                self->state = DNARiser_State_ResetRiser;
                self->timer = 60;
            }
        }
    }
    else {
        self->timer--;
    }
}

void DNARiser_State_ResetRiser(void)
{
    RSDK_THIS(DNARiser);

    if (!self->firstChild) {
        if (self->timer <= 0) {
            if (RSDK.CheckOnScreen(self, &self->updateRange)) {
                DNARiser_State_BubbleBurst();
                DNARiser_State_Init();
                self->scaleTimer = 30;
            }
            else {
                self->state = DNARiser_State_Init;
            }
        }
        else {
            self->timer--;
        }
    }
}

void DNARiser_State_OrbSetup(void)
{
    RSDK_THIS(DNARiser);

    self->active    = ACTIVE_NORMAL;
    self->stateDraw = DNARiser_Draw_Helix;
    self->state     = DNARiser_State_OrbIdle;
}

void DNARiser_State_OrbIdle(void)
{
    // 20220614 rdc said i could come back and comment these >:]
}

void DNARiser_State_OrbFall(void)
{
    RSDK_THIS(DNARiser);
    EntityDNARiser *parent = self->parent;

    if (self->timer <= 0) {
        if (self->canDestroySelf) {
            if (!RSDK.CheckOnScreen(self, &self->updateRange))
                destroyEntity(self);
        }
        else {
            EntityDNARiser *child = self->lastChild;
            self->canDestroySelf  = true;
            if (child) {
                while (true) {
                    if (RSDK.CheckOnScreen(child, &child->updateRange)) {
                        child->velocity.y = 0;
                        child->timer      = 1;
                        child->state      = DNARiser_State_OrbFall;
                        child->velocity.x = 0;
                        child->velocity.y = (self->maxSpeed.y >> 1) + (self->maxSpeed.y >> 2);
                        child->maxSpeed.y = (self->maxSpeed.y >> 1) + (self->maxSpeed.y >> 2);
                        break;
                    }
                    else {
                        EntityDNARiser *grandChild = child->lastChild;

                        if (child == parent->child)
                            parent->child = grandChild;
                        destroyEntity(child);

                        child = grandChild;
                        if (!grandChild)
                            break;
                    }
                }
            }

            if (!child)
                parent->firstChild = NULL;

            if (self->canDestroySelf) {
                if (!RSDK.CheckOnScreen(self, &self->updateRange))
                    destroyEntity(self);
            }
        }
    }
    else {
        self->timer--;
    }

    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;
    self->amplitude += 0x20000;
}

void DNARiser_Draw_Main(void)
{
    RSDK_THIS(DNARiser);

    bool32 isYellowBig = false;
    int32 angle        = ((0x57262 * (self->risePos >> 16)) >> 16) + 100;

    Animator orbAnimator;
    memset(&orbAnimator, 0, sizeof(orbAnimator));
    if ((uint32)((angle & 0x3FF) - 0x100) <= 0x200)
        isYellowBig = true;

    int32 sineOff   = (RSDK.Sin1024(angle & 0x3FF) << 6) * (self->amplitude >> 16);
    self->alpha     = 0x100;
    self->inkEffect = INK_NONE;

    Vector2 drawPos;
    drawPos = self->position;

    self->scale.x = 0x200;
    self->scale.y = 0x200;
    self->drawFX  = FX_NONE;
    if (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[1]) {
        if (isYellowBig) {
            drawPos.x = sineOff + self->position.x;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 4, &orbAnimator, true, 0);
        }
        else {
            drawPos.x = self->position.x - sineOff;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 2, &orbAnimator, true, 0);
        }

        RSDK.DrawSprite(&orbAnimator, &drawPos, false);
        if (!self->noBubbleScaling) {
            drawPos         = self->position;
            self->drawFX    = FX_SCALE;
            self->inkEffect = INK_BLEND;
            self->alpha     = 0x80;
            Vector2 scale;
            scale.x     = self->bubbleScale;
            scale.y     = self->bubbleScale;
            self->scale = DNARiser_CalculateScale(&scale);
            RSDK.DrawSprite(&self->bubbleAnimator, &drawPos, false);
        }

        if (self->popped) {
            drawPos         = self->popPos;
            self->scale.x   = 0x200;
            self->scale.y   = 0x200;
            self->drawFX    = FX_NONE;
            self->inkEffect = INK_NONE;
            RSDK.DrawSprite(&self->popAnimator, &drawPos, false);
        }
    }
    else {
        if (isYellowBig) {
            drawPos.y = self->position.y;
            drawPos.x = self->position.x - sineOff;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 2, &orbAnimator, true, 0);
        }
        else {
            drawPos.x = sineOff + self->position.x;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 4, &orbAnimator, true, 0);
        }
        RSDK.DrawSprite(&orbAnimator, &drawPos, false);
    }
}

void DNARiser_Draw_Helix(void)
{
    RSDK_THIS(DNARiser);

    bool32 isYellowBig     = false;
    EntityDNARiser *parent = self->parent;

    Animator orbAnimator;
    memset(&orbAnimator, 0, sizeof(orbAnimator));
    if ((uint32)(self->angle - 0x100) <= 0x200)
        isYellowBig = true;

    int32 sineOff   = (RSDK.Sin1024(self->angle) << 6) * (self->amplitude >> 16);
    self->alpha     = 0x100;
    self->inkEffect = INK_NONE;

    Vector2 drawPos;
    drawPos = self->position;

    self->scale.x = 0x200;
    self->scale.y = 0x200;
    self->drawFX  = FX_NONE;
    self->drawFX  = INK_NONE;
    if (SceneInfo->currentDrawGroup != Zone->objectDrawGroup[1] && !(self->startHelixPos & 1)) {
        if (isYellowBig) {
            drawPos.x = self->position.x - sineOff;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 2, &orbAnimator, true, 0);
        }
        else {
            drawPos.x = self->position.x + sineOff;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 4, &orbAnimator, true, 0);
        }
        RSDK.DrawSprite(&orbAnimator, &drawPos, false);
    }

    if (!(self->startHelixPos % 5)) {
        int32 distance = (parent->helixPos - self->startHelixPos >= 8) ? 4 : ((parent->helixPos - self->startHelixPos) / 2);
        int32 sine     = (RSDK.Sin1024(self->angle) << 6) * ((self->amplitude >> 16) - 12);
        drawPos.y      = self->position.y;
        if (isYellowBig)
            drawPos.x = self->position.x - sine;
        else
            drawPos.x = self->position.x + sine;
        int32 startX = drawPos.x;
        sine         = 2 * sine / 7;

        bool32 drawFlags[8];
        drawFlags[0] = distance >= 1;
        drawFlags[1] = distance >= 2;
        drawFlags[2] = distance >= 3;
        drawFlags[3] = distance >= 4;
        drawFlags[4] = drawFlags[3];
        drawFlags[5] = drawFlags[2];
        drawFlags[6] = drawFlags[1];
        drawFlags[7] = drawFlags[0];

        for (int32 i = 0; i < 8; ++i) {
            bool32 isYellowSmall = (isYellowBig && i <= 3) || (!isYellowBig && i >= 4);
            bool32 canDraw       = SceneInfo->currentDrawGroup == Zone->objectDrawGroup[1]
                                       ? ((isYellowBig && !isYellowSmall) || (!isYellowBig && isYellowSmall))
                                       : ((isYellowBig && isYellowSmall) || (!isYellowBig && !isYellowSmall));

            if (drawFlags[i] && canDraw) {
                if (!isYellowSmall)
                    RSDK.SetSpriteAnimation(DNARiser->aniFrames, 3, &orbAnimator, true, 0);
                else
                    RSDK.SetSpriteAnimation(DNARiser->aniFrames, 1, &orbAnimator, true, 0);
                drawPos.x = startX + sine * i * (2 * isYellowBig - 1);
                RSDK.DrawSprite(&orbAnimator, &drawPos, false);
            }
        }
    }

    if (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[1] && !(self->startHelixPos & 1)) {
        if (isYellowBig) {
            drawPos.x = self->position.x + sineOff;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 4, &orbAnimator, true, 0);
        }
        else {
            drawPos.x = self->position.x - sineOff;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 2, &orbAnimator, true, 0);
        }
        RSDK.DrawSprite(&orbAnimator, &drawPos, false);
    }
}

#if GAME_INCLUDE_EDITOR
void DNARiser_EditorDraw(void)
{
    RSDK_THIS(DNARiser);

    self->startPos        = self->position;
    self->active          = ACTIVE_BOUNDS;
    self->speed.x         = 0;
    self->velocity.x      = 0;
    self->speed.y         = -abs(self->speed.y);
    self->timer           = 0;
    self->risePos         = self->height << 16;
    self->canDestroySelf  = false;
    self->firstChild      = NULL;
    self->child           = 0;
    self->helixPos        = 0;
    self->activePlayers   = 0;
    self->noBubbleScaling = false;
    self->lastSfxID       = 0;
    self->sfxTimer        = 0;
    self->scanSfxDir      = 0;
    self->velocity.y      = 0;
    self->bubbleScale     = 0x200;
    RSDK.SetSpriteAnimation(DNARiser->aniFrames, 0, &self->bubbleAnimator, true, 0);

    int32 group = SceneInfo->currentDrawGroup;

    SceneInfo->currentDrawGroup = -1;
    DNARiser_Draw_Main();

    SceneInfo->currentDrawGroup = Zone->objectDrawGroup[1];
    DNARiser_Draw_Main();

    SceneInfo->currentDrawGroup = group;

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        Vector2 drawPos;
        drawPos.x = self->position.x;
        drawPos.y = self->position.y - self->risePos;

        RSDK.SetSpriteAnimation(DNARiser->aniFrames, 5, &self->bubbleAnimator, true, 0);
        RSDK.DrawSprite(&self->bubbleAnimator, &drawPos, false);

        DrawHelpers_DrawArrow(self->position.x, self->position.y, drawPos.x, drawPos.y, 0xFFFF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void DNARiser_EditorLoad(void) { DNARiser->aniFrames = RSDK.LoadSpriteAnimation("CPZ/DNARiser.bin", SCOPE_STAGE); }
#endif

void DNARiser_Serialize(void)
{
    RSDK_EDITABLE_VAR(DNARiser, VAR_UINT32, height);
    RSDK_EDITABLE_VAR(DNARiser, VAR_VECTOR2, speed);
}
