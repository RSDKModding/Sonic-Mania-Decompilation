// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ERZRider Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
}

void ERZRider_Create(void *data)
{
    RSDK_THIS(ERZRider);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->isJimmy       = voidToInt(data);
        if (!voidToInt(data)) {
            self->drawFX = FX_FLIP;
            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 4, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 5, &self->animator3, true, 0);
            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 6, &self->animator4, true, 0);
            self->startPos  = self->position;
            self->stateDraw = ERZRider_StateDraw_Unknown1;
            self->state     = StateMachine_None;
        }
    }
}

void ERZRider_StageLoad(void) { ERZRider->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomRider.bin", SCOPE_STAGE); }

void ERZRider_StateDraw_Unknown1(void)
{
    // thats all folks!
}

#if RETRO_INCLUDE_EDITOR
void ERZRider_EditorDraw(void)
{
    RSDK_THIS(ERZRider);
    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(ERZRider->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(ERZRider->aniFrames, 4, &self->animator2, false, 0);
    RSDK.SetSpriteAnimation(ERZRider->aniFrames, 5, &self->animator3, false, 0);
    RSDK.SetSpriteAnimation(ERZRider->aniFrames, 6, &self->animator4, false, 0);

    // ???
}

void ERZRider_EditorLoad(void) { ERZRider->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomRider.bin", SCOPE_STAGE); }
#endif

void ERZRider_Serialize(void) {}
