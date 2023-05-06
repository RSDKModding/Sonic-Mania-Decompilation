// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ThoughtBubble Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectThoughtBubble *ThoughtBubble;

void ThoughtBubble_Update(void)
{
    RSDK_THIS(ThoughtBubble);

    StateMachine_Run(self->state);

    self->bubbleDotPos[0].x = self->position.x + 0x280000;
    self->bubbleDotPos[0].y = self->position.y + ((RSDK.Sin256(-4 * Zone->timer) + 0x1A00) << 9);

    self->bubbleDotPos[1].x = self->position.x + 0x180000;
    self->bubbleDotPos[1].y = self->position.y + ((RSDK.Sin256(4 * Zone->timer) + 0xB00) << 10);
}

void ThoughtBubble_LateUpdate(void) {}

void ThoughtBubble_StaticUpdate(void) {}

void ThoughtBubble_Draw(void)
{
    RSDK_THIS(ThoughtBubble);

    switch (self->type) {
        case THOUGHTBUBBLE_1DOT: RSDK.DrawSprite(&self->dotAnimator1, &self->bubbleDotPos[0], false); break;

        case THOUGHTBUBBLE_2DOTS:
            RSDK.DrawSprite(&self->dotAnimator1, &self->bubbleDotPos[0], false);
            RSDK.DrawSprite(&self->dotAnimator2, &self->bubbleDotPos[1], false);
            break;

        case THOUGHTBUBBLE_BUBBLE:
            RSDK.DrawSprite(&self->dotAnimator1, &self->bubbleDotPos[0], false);
            RSDK.DrawSprite(&self->dotAnimator2, &self->bubbleDotPos[1], false);
            RSDK.DrawSprite(&self->bubbleAnimator, NULL, false);
            break;

        case THOUGHTBUBBLE_THOUGHTS:
            RSDK.DrawSprite(&self->dotAnimator1, &self->bubbleDotPos[0], false);
            RSDK.DrawSprite(&self->dotAnimator2, &self->bubbleDotPos[1], false);
            RSDK.DrawSprite(&self->bubbleAnimator, NULL, false);
            RSDK.DrawSprite(&self->thoughtAnimator, NULL, false);
            break;

        default: break;
    }
}

void ThoughtBubble_Create(void *data)
{
    RSDK_THIS(ThoughtBubble);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->state         = ThoughtBubble_BubbleAppear;

        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &self->dotAnimator1, true, 0);
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &self->dotAnimator2, true, 3);
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 1, &self->bubbleAnimator, true, 0);
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 3, &self->thoughtAnimator, true, 0);
    }
}

void ThoughtBubble_StageLoad(void)
{
    ThoughtBubble->aniFrames  = RSDK.LoadSpriteAnimation("LRZ3/ThoughtBubble.bin", SCOPE_STAGE);
    ThoughtBubble->dustFrames = RSDK.LoadSpriteAnimation("Global/Dust.bin", SCOPE_STAGE);
}

void ThoughtBubble_BubbleAppear(void)
{
    RSDK_THIS(ThoughtBubble);

    RSDK.ProcessAnimation(&self->dotAnimator1);
    RSDK.ProcessAnimation(&self->dotAnimator2);

    if (++self->timer == 8) {
        self->timer = 0;
        if (++self->type == THOUGHTBUBBLE_BUBBLE)
            self->state = ThoughtBubble_HaveFirstThought;
    }
}
void ThoughtBubble_HaveFirstThought(void)
{
    RSDK_THIS(ThoughtBubble);

    RSDK.ProcessAnimation(&self->dotAnimator1);
    RSDK.ProcessAnimation(&self->dotAnimator2);
    RSDK.ProcessAnimation(&self->bubbleAnimator);

    if (self->bubbleAnimator.frameID == self->bubbleAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 2, &self->bubbleAnimator, true, 0);
        self->state = ThoughtBubble_HaveOtherThoughts;
        ++self->type;
    }
}
void ThoughtBubble_HaveOtherThoughts(void)
{
    RSDK_THIS(ThoughtBubble);

    RSDK.ProcessAnimation(&self->dotAnimator1);
    RSDK.ProcessAnimation(&self->dotAnimator2);
    RSDK.ProcessAnimation(&self->bubbleAnimator);
    RSDK.ProcessAnimation(&self->thoughtAnimator);

    if (++self->timer == 80) {
        self->timer = 0;
        if (self->thoughtAnimator.animationID >= 6) {
            self->state = ThoughtBubble_BubbleDisappear;
        }
        else {
            self->thoughtAnimator.animationID++;
            RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, self->thoughtAnimator.animationID, &self->thoughtAnimator, true, 0);
        }
    }
}
void ThoughtBubble_BubbleDisappear(void)
{
    RSDK_THIS(ThoughtBubble);

    self->type = THOUGHTBUBBLE_2DOTS;

    for (int32 i = 0; i < 8; ++i) {
        int32 x              = self->position.x + RSDK.Rand(-0x180000, 0x180000);
        int32 y              = self->position.y + RSDK.Rand(-0x100000, 0x100000);
        EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x, y);

        debris->state      = Debris_State_Move;
        debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
        debris->velocity.y = RSDK.Rand(-0x20000, 0x20000);
        debris->drawFX     = FX_SCALE | FX_FLIP;
        debris->direction  = i & 3;

        int32 scale       = RSDK.Rand(0x200, 0x400);
        debris->scale.x   = scale;
        debris->scale.y   = scale;
        debris->drawGroup = Zone->objectDrawGroup[1];

        RSDK.SetSpriteAnimation(ThoughtBubble->dustFrames, 0, &debris->animator, true, RSDK.Rand(0, 4));
    }

    self->state = ThoughtBubble_DestroyBubble;
}
void ThoughtBubble_DestroyBubble(void)
{
    RSDK_THIS(ThoughtBubble);

    if (++self->timer == 8) {
        self->timer = 0;

        if (--self->type < THOUGHTBUBBLE_1DOT)
            destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void ThoughtBubble_EditorDraw(void)
{
    RSDK_THIS(ThoughtBubble);

    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &self->dotAnimator1, true, 0);
    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &self->dotAnimator2, true, 3);
    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 1, &self->bubbleAnimator, true, 0);
    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 3, &self->thoughtAnimator, true, 0);

    self->type = THOUGHTBUBBLE_THOUGHTS;

    self->bubbleDotPos[0].x = self->position.x + 0x280000;
    self->bubbleDotPos[0].y = self->position.y + ((RSDK.Sin256(-4 * Zone->timer) + 0x1A00) << 9);

    self->bubbleDotPos[1].x = self->position.x + 0x180000;
    self->bubbleDotPos[1].y = self->position.y + ((RSDK.Sin256(4 * Zone->timer) + 0xB00) << 10);

    ThoughtBubble_Draw();
}

void ThoughtBubble_EditorLoad(void) { ThoughtBubble->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/ThoughtBubble.bin", SCOPE_STAGE); }
#endif

void ThoughtBubble_Serialize(void) {}
