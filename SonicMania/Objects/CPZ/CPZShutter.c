// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CPZShutter Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCPZShutter *CPZShutter;

void CPZShutter_Update(void)
{
    RSDK_THIS(CPZShutter);

    StateMachine_Run(self->state);
}

void CPZShutter_LateUpdate(void) {}

void CPZShutter_StaticUpdate(void) {}

void CPZShutter_Draw(void)
{
    RSDK_THIS(CPZShutter);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void CPZShutter_Create(void *data)
{
    RSDK_THIS(CPZShutter);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->active        = ACTIVE_BOUNDS;
        self->drawGroup     = Zone->objectDrawGroup[1] - 2;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(CPZShutter->aniFrames, 0, &self->animator, true, 0);
    }
}

void CPZShutter_StageLoad(void) { CPZShutter->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Shutter.bin", SCOPE_STAGE); }

void CPZShutter_State_Open(void)
{
    RSDK_THIS(CPZShutter);

    self->position.y -= 0x20000;
    if (++self->timer == 24) {
        self->timer = 0;
        self->state = StateMachine_None;
    }
}

void CPZShutter_State_Close(void)
{
    RSDK_THIS(CPZShutter);

    self->position.y += 0x20000;
    if (++self->timer == 24) {
        self->timer  = 0;
        self->active = ACTIVE_BOUNDS;
        self->state  = StateMachine_None;
    }
}

#if GAME_INCLUDE_EDITOR
void CPZShutter_EditorDraw(void)
{
    RSDK_THIS(CPZShutter);

    RSDK.SetSpriteAnimation(CPZShutter->aniFrames, 0, &self->animator, true, 0);

    CPZShutter_Draw();
}

void CPZShutter_EditorLoad(void) { CPZShutter->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Shutter.bin", SCOPE_STAGE); }
#endif

void CPZShutter_Serialize(void) {}
