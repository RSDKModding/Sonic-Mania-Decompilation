// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: AIZEggRobo Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectAIZEggRobo *AIZEggRobo;

void AIZEggRobo_Update(void)
{
    RSDK_THIS(AIZEggRobo);

    if (self->oscillate)
        self->position.y += RSDK.Sin256(4 * (self->oscillateOffset + Zone->timer)) << 7;

    if (self->movePos.x != self->position.x) {
        int32 distance = self->position.x - self->movePos.x;
        if (distance < 0)
            self->direction = FLIP_X;
        else if (distance > 0)
            self->direction = FLIP_NONE;
    }

    self->movePos = self->position;
    RSDK.ProcessAnimation(&self->animatorBody);
    RSDK.ProcessAnimation(&self->animatorLegs);
    RSDK.ProcessAnimation(&self->animatorArm);
}

void AIZEggRobo_LateUpdate(void) {}

void AIZEggRobo_StaticUpdate(void) {}

void AIZEggRobo_Draw(void)
{
    RSDK_THIS(AIZEggRobo);

    RSDK.DrawSprite(&self->animatorLegs, NULL, false);
    RSDK.DrawSprite(&self->animatorBody, NULL, false);
    RSDK.DrawSprite(&self->animatorArm, NULL, false);
}

void AIZEggRobo_Create(void *data)
{
    RSDK_THIS(AIZEggRobo);
    self->active          = ACTIVE_BOUNDS;
    self->drawGroup       = Zone->objectDrawGroup[0];
    self->rotation        = self->angle;
    self->startPos        = self->position;
    self->movePos.x       = self->position.x;
    self->movePos.y       = self->position.y;
    self->visible         = true;
    self->drawFX          = FX_ROTATE | FX_FLIP;
    self->updateRange.x   = 0x800000;
    self->updateRange.y   = 0x800000;
    self->oscillateOffset = RSDK.Rand(0, 256);

    RSDK.SetSpriteAnimation(AIZEggRobo->aniFrames, 0, &self->animatorBody, true, 0);
    RSDK.SetSpriteAnimation(AIZEggRobo->aniFrames, 1, &self->animatorArm, true, 0);
    RSDK.SetSpriteAnimation(AIZEggRobo->aniFrames, 2, &self->animatorLegs, true, 0);
}

void AIZEggRobo_StageLoad(void) { AIZEggRobo->aniFrames = RSDK.LoadSpriteAnimation("AIZ/AIZEggRobo.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void AIZEggRobo_EditorDraw(void) { AIZEggRobo_Draw(); }

void AIZEggRobo_EditorLoad(void)
{
    AIZEggRobo->aniFrames = RSDK.LoadSpriteAnimation("AIZ/AIZEggRobo.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(AIZEggRobo, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void AIZEggRobo_Serialize(void)
{
    RSDK_EDITABLE_VAR(AIZEggRobo, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(AIZEggRobo, VAR_BOOL, oscillate);
    RSDK_EDITABLE_VAR(AIZEggRobo, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(AIZEggRobo, VAR_BOOL, forKnux);
}
