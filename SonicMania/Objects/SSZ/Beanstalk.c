// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Beanstalk Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBeanstalk *Beanstalk;

void Beanstalk_Update(void)
{
    RSDK_THIS(Beanstalk);

    RSDK.ProcessAnimation(&self->nodeAnimator);

    if (self->type > BEANSTALK_NODE)
        RSDK.ProcessAnimation(&self->plantAnimator);

    StateMachine_Run(self->state);
}

void Beanstalk_LateUpdate(void) {}

void Beanstalk_StaticUpdate(void) {}

void Beanstalk_Draw(void)
{
    RSDK_THIS(Beanstalk);

    if (self->shown) {
        Beanstalk_DrawNodes();
        Beanstalk_DrawCreationNode();

        self->scale.x = self->size;
        self->scale.y = self->size;
        self->drawFX  = FX_SCALE | FX_FLIP;
        RSDK.DrawSprite(&self->nodeAnimator, NULL, false);

        if (self->type > BEANSTALK_NODE)
            RSDK.DrawSprite(&self->plantAnimator, NULL, false);

        self->drawFX  = FX_FLIP;
        self->scale.x = 0x200;
        self->scale.y = 0x200;
    }
}

void Beanstalk_Create(void *data)
{
    RSDK_THIS(Beanstalk);

    self->active       = ACTIVE_BOUNDS;
    self->visible      = true;
    self->drawFX       = FX_FLIP;
    self->drawGroup    = Zone->objectDrawGroup[0];
    self->bezCtrlAngle = self->bezCtrlAngle & 0xFF;

    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;
    self->curBezierPos  = -1;
    if (self->type == BEANSTALK_BEGIN) {
        self->size   = 512;
        self->active = ACTIVE_NORMAL;
    }
    else {
        self->size = 0;
    }

    self->startPos      = self->position;
    self->platformPos   = self->position;
    self->stoodAngle    = 0;
    self->timer         = 0;
    self->chomperOffset = 0;
    self->state         = Beanstalk_State_Init;
}

void Beanstalk_StageLoad(void)
{
    Beanstalk->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Beanstalk.bin", SCOPE_STAGE);

    Beanstalk->sfxImpact    = RSDK.GetSfx("Stage/Impact6.wav");
    Beanstalk->sfxBeanNode  = RSDK.GetSfx("SSZ1/BeanNode.wav");
    Beanstalk->sfxBeanChomp = RSDK.GetSfx("SSZ1/BeanChomp.wav");

    Beanstalk->hitboxSeed.left   = -1;
    Beanstalk->hitboxSeed.top    = -64;
    Beanstalk->hitboxSeed.right  = 1;
    Beanstalk->hitboxSeed.bottom = 1;

    Beanstalk->hitboxPlatform.left   = 0;
    Beanstalk->hitboxPlatform.top    = -3;
    Beanstalk->hitboxPlatform.right  = 46;
    Beanstalk->hitboxPlatform.bottom = 13;

    Beanstalk->hitboxPlant.left   = 8;
    Beanstalk->hitboxPlant.top    = -8;
    Beanstalk->hitboxPlant.right  = 44;
    Beanstalk->hitboxPlant.bottom = 8;
}

int32 Beanstalk_GetNextNodeDistance(void)
{
    RSDK_THIS(Beanstalk);

    EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntitySlot(self) + 1, Beanstalk);

    if (self->forceEnd || next->classID != Beanstalk->classID)
        return 0;

    int32 thisX = ((self->bezCtrlLength * RSDK.Cos256(self->bezCtrlAngle)) << 9) + self->position.x;
    int32 thisY = ((self->bezCtrlLength * RSDK.Sin256(self->bezCtrlAngle)) << 9) + self->position.y;

    uint8 angle = (next->bezCtrlAngle + 0x80);
    int32 nextX = ((next->bezCtrlLength * RSDK.Cos256(angle)) << 9) + next->position.x;
    int32 nextY = ((next->bezCtrlLength * RSDK.Sin256(angle)) << 9) + next->position.y;
    return MathHelpers_GetBezierCurveLength(self->position.x, self->position.y, thisX, thisY, nextX, nextY, next->position.x, next->position.y)
           / 0xA0000;
}

int32 Beanstalk_GetRemainingDistance(void)
{
    RSDK_THIS(Beanstalk);

    int32 len = 0;
    while (self->type) {
        EntityBeanstalk *prev = RSDK_GET_ENTITY(RSDK.GetEntitySlot(self) - 1, Beanstalk);
        if (prev->classID != Beanstalk->classID)
            break;

        int32 prevX = ((prev->bezCtrlLength * RSDK.Cos256(prev->bezCtrlAngle)) << 9) + prev->position.x;
        int32 prevY = ((prev->bezCtrlLength * RSDK.Sin256(prev->bezCtrlAngle)) << 9) + prev->position.y;

        uint8 angle = (self->bezCtrlAngle + 0x80);
        int32 thisX = ((self->bezCtrlLength * RSDK.Cos256(angle)) << 9) + self->position.x;
        int32 thisY = ((self->bezCtrlLength * RSDK.Sin256(angle)) << 9) + self->position.y;
        len += MathHelpers_GetBezierCurveLength(prev->position.x, prev->position.y, prevX, prevY, thisX, thisY, self->position.x, self->position.y);

        self = prev;
    }

    return len;
}

int32 Beanstalk_GetBezierInc(void)
{
    RSDK_THIS(Beanstalk);

    EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntitySlot(self) + 1, Beanstalk);

    if (self->forceEnd || next->classID != Beanstalk->classID)
        return 0;

    int32 thisX = ((self->bezCtrlLength * RSDK.Cos256(self->bezCtrlAngle)) << 9) + self->position.x;
    int32 thisY = ((self->bezCtrlLength * RSDK.Sin256(self->bezCtrlAngle)) << 9) + self->position.y;

    uint8 angle = (next->bezCtrlAngle + 0x80);
    int32 nextX = ((next->bezCtrlLength * RSDK.Cos256(angle)) << 9) + next->position.x;
    int32 nextY = ((next->bezCtrlLength * RSDK.Sin256(angle)) << 9) + next->position.y;
    return 0x10000
           / (MathHelpers_GetBezierCurveLength(self->position.x, self->position.y, thisX, thisY, nextX, nextY, next->position.x, next->position.y)
              / 0x32000);
}

void Beanstalk_DrawNodes(void)
{
    RSDK_THIS(Beanstalk);

    if (!self->forceEnd) {
        EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntitySlot(self) + 1, Beanstalk);

        if (next->classID == Beanstalk->classID) {
            if (SceneInfo->inEditor) {
                self->curBezierPos = Beanstalk_GetNextNodeDistance();
                self->bezierPos    = 0x10000;
            }

            if (self->curBezierPos == (uint16)-1)
                self->curBezierPos = Beanstalk_GetNextNodeDistance();

            if (self->curBezierPos) {
                if (!self->setupDecorNodeID || SceneInfo->inEditor) {
                    int32 distance         = Beanstalk_GetRemainingDistance();
                    self->setupDecorNodeID = true;
                    self->curDecorID       = (distance / 0xA0000 / 3) % 9;
                    self->curNodeID        = 3 * ((distance / 0xA0000 / 3) + 1) - distance / 0xA0000;
                }

                int32 thisX = self->position.x + ((self->bezCtrlLength * RSDK.Cos256(self->bezCtrlAngle + 0x00)) << 9);
                int32 thisY = self->position.y + ((self->bezCtrlLength * RSDK.Sin256(self->bezCtrlAngle + 0x00)) << 9);
                int32 nextX = next->position.x + ((next->bezCtrlLength * RSDK.Cos256(next->bezCtrlAngle + 0x80)) << 9);
                int32 nextY = next->position.y + ((next->bezCtrlLength * RSDK.Sin256(next->bezCtrlAngle + 0x80)) << 9);

                int32 bezInc = 0x10000 / self->curBezierPos;

                RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 1, &self->fillerAnimator, true, 0);
                for (int32 bezPos = (0x10000 / self->curBezierPos) >> 1; bezPos <= 0x10000; bezPos += bezInc) {
                    if (bezPos >= self->bezierPos)
                        break;

                    Vector2 drawPos = MathHelpers_GetBezierPoint(bezPos, self->position.x, self->position.y, thisX, thisY, nextX, nextY,
                                                                 next->position.x, next->position.y);

                    RSDK.DrawSprite(&self->fillerAnimator, &drawPos, false);
                }

                int32 nodeID  = self->curNodeID;
                int32 decorID = self->curDecorID;
                for (int32 bezPos = (0x10000 / self->curBezierPos) >> 1; bezPos <= 0x10000; ++nodeID) {
                    if (bezPos >= self->bezierPos)
                        break;

                    Vector2 drawPos = MathHelpers_GetBezierPoint(bezPos, self->position.x, self->position.y, thisX, thisY, nextX, nextY,
                                                                 next->position.x, next->position.y);

                    if (!(nodeID % 3)) {
                        RSDK.SetSpriteAnimation(Beanstalk->aniFrames, decorID + 5, &self->fillerAnimator, true, 0);
                        RSDK.DrawSprite(&self->fillerAnimator, &drawPos, false);

                        decorID = (decorID + 1) % 9;
                    }

                    bezPos += bezInc;
                }
            }
        }
    }
}

void Beanstalk_DrawCreationNode(void)
{
    RSDK_THIS(Beanstalk);

    // Draw the node that "walks" along the path and "creates" the beanstalk behind it
    if (!self->forceEnd) {
        EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntitySlot(self) + 1, Beanstalk);

        if (next->classID == Beanstalk->classID) {
            if (self->curBezierPos) {
                int32 thisX = self->position.x + ((self->bezCtrlLength * RSDK.Cos256(self->bezCtrlAngle + 0x00)) << 9);
                int32 thisY = self->position.y + ((self->bezCtrlLength * RSDK.Sin256(self->bezCtrlAngle + 0x00)) << 9);
                int32 nextX = next->position.x + ((next->bezCtrlLength * RSDK.Cos256(next->bezCtrlAngle + 0x80)) << 9);
                int32 nextY = next->position.y + ((next->bezCtrlLength * RSDK.Sin256(next->bezCtrlAngle + 0x80)) << 9);

                Vector2 drawPos = MathHelpers_GetBezierPoint(self->bezierPos, self->position.x, self->position.y, thisX, thisY, nextX, nextY,
                                                             next->position.x, next->position.y);
                RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 1, &self->fillerAnimator, true, 0);
                RSDK.DrawSprite(&self->fillerAnimator, &drawPos, false);
            }
        }
    }
}

void Beanstalk_HandleNodeMovement(void)
{
    RSDK_THIS(Beanstalk);

    if (!self->finished) {
        EntityBeanstalk *node = NULL;

        if (!self->forceEnd) {
            node = RSDK_GET_ENTITY(RSDK.GetEntitySlot(self) + 1, Beanstalk);
            if (node->classID != Beanstalk->classID)
                node = NULL;
        }

        if (self->forceEnd || !node)
            self->bezierPos = 0x10000;

        if (self->bezierPos < 0x10000) {
            if (!self->bezierInc)
                self->bezierInc = Beanstalk_GetBezierInc();

            self->bezierPos += self->bezierInc;
        }

        if (self->bezierPos > 0x10000)
            self->bezierPos = 0x10000;

        if (self->bezierPos == 0x10000) {
            self->finished = true;
            self->active   = ACTIVE_BOUNDS;

            if (node) {
                node->shown  = true;
                node->active = ACTIVE_NORMAL;
            }
        }
    }
}

void Beanstalk_HandleNodeAppear(void)
{
    RSDK_THIS(Beanstalk);

    self->size = (self->timer << 9) / 20;
    if (self->timer < 20) {
        if (!self->timer) {
            if (RSDK.CheckOnScreen(self, NULL))
                RSDK.PlaySfx(Beanstalk->sfxBeanNode, false, 255);
        }

        ++self->timer;
    }
}

void Beanstalk_CheckPlayerCollisions_Platform(void)
{
    RSDK_THIS(Beanstalk);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position   = self->platformPos;
    self->position.x = ((RSDK.Sin512(2 * self->moveAngle) << 9) + self->position.x) & 0xFFFF0000;
    self->position.y &= 0xFFFF0000;

    if (!self->activePlayers) {
        if (self->stoodAngle > 0)
            self->stoodAngle -= 4;
    }
    else {
        if (self->stoodAngle < 64)
            self->stoodAngle += 4;
    }

    self->activePlayers = 0;
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if (Player_CheckCollisionPlatform(player, self, &Beanstalk->hitboxPlatform)) {
            player->position.x += self->position.x - storeX;

            if (self->position.y - storeY > 0)
                player->position.y += self->position.y - storeY;

            player->position.y += 0x10000;
            player->position.y &= 0xFFFF0000;
            self->activePlayers |= 1 << playerID;
        }
    }

    self->platformPos.x = self->startPos.x;
    self->platformPos.y = self->startPos.y + (RSDK.Sin256(self->stoodAngle) << 10);
}

void Beanstalk_CheckPlayerCollisions_Chomper(void)
{
    RSDK_THIS(Beanstalk);

    int32 mult = 0;
    if (self->chomperOffset >= 15) {
        if (self->chomperOffset >= 60) {
            self->chomperOffset = 0;
        }
        else {
            mult = 0x15540 - 0x5B0 * self->chomperOffset;
            self->chomperOffset++;
        }
    }
    else {
        mult = 0x1111 * self->chomperOffset;
        self->chomperOffset++;
    }

    self->position = self->startPos;
    if (self->direction == FLIP_X)
        self->position.x += mult * -16;
    else
        self->position.x += mult * 16;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Beanstalk->hitboxPlant)) {
#if MANIA_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop) {
                CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
                RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
                self->state = Beanstalk_State_Node;
                RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->plantAnimator, true, 0);
            }
            else if (!Player_CheckMightyUnspin(player, 0x400, 2, &player->uncurlTimer)) {
#endif
                Player_Hurt(player, self);
#if MANIA_USE_PLUS
            }
#endif
        }
    }
}

void Beanstalk_State_Init(void)
{
    RSDK_THIS(Beanstalk);

    switch (self->type) {
        case BEANSTALK_BEGIN:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 4, &self->nodeAnimator, true, 9);
            self->state = Beanstalk_StateDirt_WaitForStart;
            break;

        case BEANSTALK_NODE:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->nodeAnimator, true, 0);
            self->state = Beanstalk_State_Node;
            break;

        case BEANSTALK_PLATFORM:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->nodeAnimator, true, 0);
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 2, &self->plantAnimator, true, 0);
            self->state = Beanstalk_State_Platform;
            break;

        case BEANSTALK_CHOMPER:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->nodeAnimator, true, 0);
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 3, &self->plantAnimator, true, 0);
            self->state = Beanstalk_State_Chomper;
            break;

        default: break;
    }
}

void Beanstalk_StateDirt_WaitForStart(void)
{
    RSDK_THIS(Beanstalk);

    if (self->startGrowth) {
        self->state = Beanstalk_StateDirt_GrowthDelay;
        self->timer = 15;
    }
}

void Beanstalk_StateDirt_GrowthDelay(void)
{
    RSDK_THIS(Beanstalk);

    if (self->timer <= 0) {
        RSDK.PlaySfx(Beanstalk->sfxImpact, false, 255);
        Camera_ShakeScreen(0, 0, 5);

        RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 4, &self->nodeAnimator, true, 0);
        self->shown  = true;
        self->active = ACTIVE_NORMAL;
        self->state  = Beanstalk_StateDirt_Grow;
    }
    else {
        self->timer--;
    }
}

void Beanstalk_StateDirt_Grow(void) { Beanstalk_HandleNodeMovement(); }

void Beanstalk_State_Node(void)
{
    RSDK_THIS(Beanstalk);

    if (self->shown) {
        Beanstalk_HandleNodeMovement();
        Beanstalk_HandleNodeAppear();

        self->position   = self->startPos;
        self->position.x = ((RSDK.Sin512(2 * self->moveAngle) << 9) + self->position.x) & 0xFFFF0000;
        self->position.y &= 0xFFFF0000;

        ++self->moveAngle;
    }
}

void Beanstalk_State_Platform(void)
{
    RSDK_THIS(Beanstalk);

    if (self->shown) {
        Beanstalk_HandleNodeMovement();
        Beanstalk_HandleNodeAppear();

        Beanstalk_CheckPlayerCollisions_Platform();

        ++self->moveAngle;
    }
}

void Beanstalk_State_Chomper(void)
{
    RSDK_THIS(Beanstalk);

    if (self->shown) {
        Beanstalk_HandleNodeMovement();
        Beanstalk_HandleNodeAppear();

        Beanstalk_CheckPlayerCollisions_Chomper();

        if (RSDK.CheckOnScreen(self, NULL)) {
            if (self->plantAnimator.frameID == 5 && self->plantAnimator.timer == 1)
                RSDK.PlaySfx(Beanstalk->sfxBeanChomp, false, 255);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void Beanstalk_EditorDraw(void)
{
    RSDK_THIS(Beanstalk);

    self->bezCtrlAngle &= 0xFF;
    self->curBezierPos  = -1;
    self->size          = 0x200;
    self->startPos      = self->position;
    self->platformPos   = self->position;
    self->stoodAngle    = 0;
    self->timer         = 0;
    self->chomperOffset = 0;

    switch (self->type) {
        case BEANSTALK_BEGIN: RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 4, &self->nodeAnimator, true, 9); break;

        case BEANSTALK_NODE: RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->nodeAnimator, true, 0); break;

        case BEANSTALK_PLATFORM:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->nodeAnimator, true, 0);
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 2, &self->plantAnimator, true, 0);
            break;

        case BEANSTALK_CHOMPER:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->nodeAnimator, true, 0);
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 3, &self->plantAnimator, true, 0);
            break;

        default: break;
    }

    RSDK_DRAWING_OVERLAY(true);
    Beanstalk_DrawNodes();
    RSDK_DRAWING_OVERLAY(false);

    self->scale.x = self->size;
    self->scale.y = self->size;
    self->drawFX  = FX_SCALE | FX_FLIP;
    RSDK.DrawSprite(&self->nodeAnimator, NULL, false);

    if (self->type > BEANSTALK_NODE)
        RSDK.DrawSprite(&self->plantAnimator, NULL, false);

    self->drawFX  = FX_FLIP;
    self->scale.x = 0x200;
    self->scale.y = 0x200;
}

void Beanstalk_EditorLoad(void)
{
    Beanstalk->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Beanstalk.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Beanstalk, type);
    RSDK_ENUM_VAR("Dirt Splash", BEANSTALK_BEGIN);
    RSDK_ENUM_VAR("Node", BEANSTALK_NODE);
    RSDK_ENUM_VAR("Platform", BEANSTALK_PLATFORM);
    RSDK_ENUM_VAR("Chomper", BEANSTALK_CHOMPER);

    RSDK_ACTIVE_VAR(Beanstalk, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void Beanstalk_Serialize(void)
{
    RSDK_EDITABLE_VAR(Beanstalk, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_INT32, bezCtrlAngle);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_INT32, bezCtrlLength);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_BOOL, forceEnd);
}
