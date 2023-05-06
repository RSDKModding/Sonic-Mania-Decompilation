// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ERZRider Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectERZRider *ERZRider;

void ERZRider_Update(void)
{
    RSDK_THIS(ERZRider);

    StateMachine_Run(self->state);
}

void ERZRider_LateUpdate(void) {}

void ERZRider_StaticUpdate(void) {}

void ERZRider_Draw(void)
{
    RSDK_THIS(ERZRider);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    }
}

void ERZRider_Create(void *data)
{
    RSDK_THIS(ERZRider);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->isJimmy       = VOID_TO_INT(data);

        if (!VOID_TO_INT(data)) {
            self->drawFX = FX_FLIP;

            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 0, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 4, &self->jimmyAnimator, true, 0);
            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 5, &self->wheelAnimator, true, 0);
            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 6, &self->thrustAnimator, true, 0);

            self->startPos  = self->position;
            self->stateDraw = ERZRider_Draw_Rider;
            self->state     = StateMachine_None;
        }
    }
}

void ERZRider_StageLoad(void) { ERZRider->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomRider.bin", SCOPE_STAGE); }

void ERZRider_Draw_Rider(void)
{
    // thats all folks!
}

#if GAME_INCLUDE_EDITOR
void ERZRider_EditorDraw(void)
{
    RSDK_THIS(ERZRider);

    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(ERZRider->aniFrames, 0, &self->mainAnimator, false, 0);
    RSDK.SetSpriteAnimation(ERZRider->aniFrames, 4, &self->jimmyAnimator, false, 0);
    RSDK.SetSpriteAnimation(ERZRider->aniFrames, 5, &self->wheelAnimator, false, 0);
    RSDK.SetSpriteAnimation(ERZRider->aniFrames, 6, &self->thrustAnimator, false, 0);
}

void ERZRider_EditorLoad(void) { ERZRider->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomRider.bin", SCOPE_STAGE); }
#endif

void ERZRider_Serialize(void) {}
