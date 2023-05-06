// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DAControl Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        drawPos                         = self->position;
        self->backPlateAnimator.frameID = 0;
        RSDK.DrawSprite(&self->backPlateAnimator, &drawPos, false);

        self->backPlateAnimator.frameID = 1;
        RSDK.DrawSprite(&self->backPlateAnimator, &drawPos, false);

        drawPos.y += 0x80000;
        RSDK.DrawSprite(&self->backPlateAnimator, &drawPos, false);

        drawPos.y += 0x80000;
        RSDK.DrawSprite(&self->backPlateAnimator, &drawPos, false);

        drawPos.y += 0x80000;
        self->backPlateAnimator.frameID = 2;
        RSDK.DrawSprite(&self->backPlateAnimator, &drawPos, false);

        drawPos.y -= 0x170000;
        self->backPlateAnimator.frameID = 3;
        RSDK.DrawSprite(&self->backPlateAnimator, &drawPos, false);
        ++self->direction;
    }

    self->direction = FLIP_NONE;
    drawPos.y -= 0x130000;
    self->backPlateAnimator.frameID = 4;
    RSDK.DrawSprite(&self->backPlateAnimator, &drawPos, false);

    drawPos.x = self->position.x - 0x700000;
    drawPos.y = self->position.y + 0x1E0000;
    for (int32 i = 0; i < 5; ++i) {
        RSDK.DrawSprite(&self->buttonAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->optionsAnimator[i], &drawPos, false);
        drawPos.x += 0x380000;
    }

    drawPos.x = self->position.x - 0x800000;
    drawPos.y = self->position.y;
    if (!SceneInfo->inEditor)
        RSDK.DrawText(&self->textAnimator, &drawPos, &self->text, 0, self->text.length, ALIGN_LEFT, 0, 0, 0, false);
}

void DAControl_Create(void *data)
{
    RSDK_THIS(DAControl);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = 2;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 0, &self->backPlateAnimator, true, 0);
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 1, &self->buttonAnimator, true, 0);
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 4, &self->textAnimator, true, 0);
        for (int32 i = 0; i < 5; ++i) RSDK.SetSpriteAnimation(DAControl->aniFrames, 2, &self->optionsAnimator[i], true, i);

        RSDK.InitString(&self->text, "SELECT A TRACK...", 0);
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 4, &self->textAnimator, true, 0);
        RSDK.SetSpriteString(DAControl->aniFrames, 4, &self->text);
    }
}

void DAControl_StageLoad(void) { DAControl->aniFrames = RSDK.LoadSpriteAnimation("UI/DAGarden.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void DAControl_EditorDraw(void)
{
    RSDK_THIS(DAControl);

    self->visible       = true;
    self->drawGroup     = 2;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(DAControl->aniFrames, 0, &self->backPlateAnimator, true, 0);
    RSDK.SetSpriteAnimation(DAControl->aniFrames, 1, &self->buttonAnimator, true, 0);
    RSDK.SetSpriteAnimation(DAControl->aniFrames, 4, &self->textAnimator, true, 0);
    for (int32 i = 0; i < 5; ++i) RSDK.SetSpriteAnimation(DAControl->aniFrames, 2, &self->optionsAnimator[i], true, i);

    DAControl_Draw();
}

void DAControl_EditorLoad(void) { DAControl->aniFrames = RSDK.LoadSpriteAnimation("UI/DAGarden.bin", SCOPE_STAGE); }
#endif

void DAControl_Serialize(void) {}
