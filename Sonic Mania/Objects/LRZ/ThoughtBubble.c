// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ThoughtBubble Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectThoughtBubble *ThoughtBubble;

void ThoughtBubble_Update(void)
{
    RSDK_THIS(ThoughtBubble);
    StateMachine_Run(self->state);
    self->drawPos1.x = self->position.x + 0x280000;
    self->drawPos1.y = ((RSDK.Sin256(-4 * Zone->timer) + 0x1A00) << 9) + self->position.y;
    self->drawPos2.x = self->position.x + 0x180000;
    self->drawPos2.y = ((RSDK.Sin256(4 * Zone->timer) + 0xB00) << 10) + self->position.y;
}

void ThoughtBubble_LateUpdate(void) {}

void ThoughtBubble_StaticUpdate(void) {}

void ThoughtBubble_Draw(void)
{
    RSDK_THIS(ThoughtBubble);
    switch (self->type) {
        case 0: RSDK.DrawSprite(&self->animator3, &self->drawPos1, false); break;
        case 1:
            RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);
            RSDK.DrawSprite(&self->animator4, &self->drawPos2, false);
            break;
        case 2:
            RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);
            RSDK.DrawSprite(&self->animator4, &self->drawPos2, false);
            RSDK.DrawSprite(&self->animator1, NULL, false);
            break;
        case 3:
            RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);
            RSDK.DrawSprite(&self->animator4, &self->drawPos2, false);
            RSDK.DrawSprite(&self->animator1, NULL, false);
            RSDK.DrawSprite(&self->animator2, NULL, false);
            break;
        default: break;
    }
}

void ThoughtBubble_Create(void *data)
{
    RSDK_THIS(ThoughtBubble);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderHigh;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->state         = ThoughtBubble_Unknown1;
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &self->animator4, true, 3);
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 1, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 3, &self->animator2, true, 0);
    }
}

void ThoughtBubble_StageLoad(void)
{
    ThoughtBubble->aniFrames  = RSDK.LoadSpriteAnimation("LRZ3/ThoughtBubble.bin", SCOPE_STAGE);
    ThoughtBubble->dustFrames = RSDK.LoadSpriteAnimation("Global/Dust.bin", SCOPE_STAGE);
}

void ThoughtBubble_Unknown1(void)
{
    RSDK_THIS(ThoughtBubble);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    if (++self->timer == 8) {
        self->timer = 0;
        if (++self->type == 2)
            self->state = ThoughtBubble_Unknown2;
    }
}
void ThoughtBubble_Unknown2(void)
{
    RSDK_THIS(ThoughtBubble);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 2, &self->animator1, true, 0);
        self->state = ThoughtBubble_Unknown3;
        ++self->type;
    }
}
void ThoughtBubble_Unknown3(void)
{
    RSDK_THIS(ThoughtBubble);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    if (++self->timer == 80) {
        self->timer = 0;
        if (self->animator2.animationID >= 6) {
            self->state = ThoughtBubble_Unknown4;
        }
        else {
            self->animator2.animationID++;
            RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, self->animator2.animationID, &self->animator2, true, 0);
        }
    }
}
void ThoughtBubble_Unknown4(void)
{
    RSDK_THIS(ThoughtBubble);
    self->type = 1;
    for (int32 i = 0; i < 8; ++i) {
        EntityDebris *debris =
            CREATE_ENTITY(Debris, NULL, self->position.x + RSDK.Rand(-0x180000, 0x180000), self->position.y + RSDK.Rand(-0x100000, 0x100000));
        debris->state        = Debris_State_Move;
        debris->velocity.x   = RSDK.Rand(-0x20000, 0x20000);
        debris->velocity.y   = RSDK.Rand(-0x20000, 0x20000);
        debris->drawFX       = FX_SCALE | FX_FLIP;
        debris->direction    = i & 3;
        int32 scale            = RSDK.Rand(0x200, 0x400);
        debris->scale.x      = scale;
        debris->scale.y      = scale;
        debris->drawOrder    = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(ThoughtBubble->dustFrames, 0, &debris->animator, true, RSDK.Rand(0, 4));
    }
    self->state = ThoughtBubble_Unknown5;
}
void ThoughtBubble_Unknown5(void)
{
    RSDK_THIS(ThoughtBubble);
    if (++self->timer == 8) {
        self->timer = 0;
        if (--self->type < 0)
            destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void ThoughtBubble_EditorDraw(void)
{
    RSDK_THIS(ThoughtBubble);
    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &self->animator4, true, 3);
    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 1, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 3, &self->animator2, true, 0);

    self->type = 3;

    self->drawPos1.x = self->position.x + 0x280000;
    self->drawPos1.y = ((RSDK.Sin256(-4) + 0x1A00) << 9) + self->position.y;
    self->drawPos2.x = self->position.x + 0x180000;
    self->drawPos2.y = ((RSDK.Sin256(4) + 0xB00) << 10) + self->position.y;

    ThoughtBubble_Draw();
}

void ThoughtBubble_EditorLoad(void) {}
#endif

void ThoughtBubble_Serialize(void) {}
