// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DAControl Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectDAControl *DAControl;

void DAControl_Update(void)
{
    RSDK_THIS(DAControl);
    StateMachine_Run(self->state);
}

void DAControl_LateUpdate(void) {}

void DAControl_StaticUpdate(void) {}

void DAControl_Draw(void)
{
    RSDK_THIS(DAControl);
    Vector2 drawPos;

    self->direction = FLIP_NONE;
    for (int32 i = 0; i < 2; ++i) {
        drawPos                   = self->position;
        self->animator1.frameID = 0;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);

        self->animator1.frameID = 1;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);

        drawPos.y += 0x80000;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);

        drawPos.y += 0x80000;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);

        drawPos.y += 0x80000;
        self->animator1.frameID = 2;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);

        drawPos.y -= 0x170000;
        self->animator1.frameID = 3;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);
        ++self->direction;
    }

    self->direction = FLIP_NONE;
    drawPos.y -= 0x130000;
    self->animator1.frameID = 4;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    drawPos.x = self->position.x - 0x700000;
    drawPos.y = self->position.y + 0x1E0000;
    for (int32 i = 0; i < 5; ++i) {
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
        RSDK.DrawSprite(&self->animator3[i], &drawPos, false);
        drawPos.x += 0x380000;
    }

    drawPos.x = self->position.x - 0x800000;
    drawPos.y = self->position.y;
    if (!SceneInfo->inEditor)
        RSDK.DrawText(&self->animator4, &drawPos, &self->text, 0, self->text.length, ALIGN_LEFT, 0, 0, 0, false);
}

void DAControl_Create(void *data)
{
    RSDK_THIS(DAControl);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = 2;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 4, &self->animator4, true, 0);
        for (int32 i = 0; i < 5; ++i) RSDK.SetSpriteAnimation(DAControl->aniFrames, 2, &self->animator3[i], true, i);

        RSDK.SetText(&self->text, "SELECT A TRACK...", 0);
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 4, &self->animator4, true, 0);
        RSDK.SetSpriteString(DAControl->aniFrames, 4, &self->text);
    }
}

void DAControl_StageLoad(void) { DAControl->aniFrames = RSDK.LoadSpriteAnimation("UI/DAGarden.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void DAControl_EditorDraw(void) {}

void DAControl_EditorLoad(void) {}
#endif

void DAControl_Serialize(void) {}
