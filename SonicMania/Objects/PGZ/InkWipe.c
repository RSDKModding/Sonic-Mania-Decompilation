// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: InkWipe Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectInkWipe *InkWipe;

// This object doesn't look like it was ever fully finished, a bummer indeed

void InkWipe_Update(void)
{
    RSDK_THIS(InkWipe);

    RSDK.ProcessAnimation(&self->animator);
}

void InkWipe_LateUpdate(void) {}

void InkWipe_StaticUpdate(void) {}

void InkWipe_Draw(void)
{
    RSDK_THIS(InkWipe);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void InkWipe_Create(void *data)
{
    RSDK_THIS(InkWipe);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(InkWipe->aniFrames, 0, &self->animator, true, 0);
    }
}

void InkWipe_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        InkWipe->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/InkWipe.bin", SCOPE_STAGE);
}

#if GAME_INCLUDE_EDITOR
void InkWipe_EditorDraw(void)
{
    RSDK_THIS(InkWipe);

    self->drawGroup     = Zone->objectDrawGroup[1];
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(InkWipe->aniFrames, 0, &self->animator, true, 0);

    InkWipe_Draw();
}

void InkWipe_EditorLoad(void) { InkWipe->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/InkWipe.bin", SCOPE_STAGE); }
#endif

void InkWipe_Serialize(void) {}
