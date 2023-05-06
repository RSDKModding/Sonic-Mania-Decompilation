// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UICreditsText Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUICreditsText *UICreditsText;

void UICreditsText_Update(void)
{
    RSDK_THIS(UICreditsText);

    StateMachine_Run(self->state);
}

void UICreditsText_LateUpdate(void) {}

void UICreditsText_StaticUpdate(void) {}

void UICreditsText_Draw(void)
{
    RSDK_THIS(UICreditsText);

    Vector2 drawPos;
    int32 width = RSDK.GetStringWidth(UICreditsText->aniFrames, self->listID, &self->text, 0, self->text.length, 0);

    drawPos.x = self->position.x;
    drawPos.y = self->position.y + 0x40000;
    if (self->hasShape)
        UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, (self->scaleTimer * (width + 16)) >> 8, self->scaleTimer >> 4, 16, 0x00, 0x00, 0x00);

    drawPos.x -= width << 15;
    drawPos.y = self->position.y;
    if (self->animator.animationID >= 7) {
        RSDK.DrawSprite(&self->animator, 0, false);
    }
    else {
        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo->size.x,
                           (self->position.y >> 16) + self->clipY2 - ScreenInfo[SceneInfo->currentScreenID].position.y);

        RSDK.DrawText(&self->animator, &drawPos, &self->text, 0, 0, ALIGN_LEFT, 0, NULL, self->charPositions, false);

        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);
    }
}

void UICreditsText_Create(void *data)
{
    RSDK_THIS(UICreditsText);

    RSDK.SetSpriteAnimation(UICreditsText->aniFrames, self->listID, &self->animator, true, 0);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawGroup     = 2;
        self->state         = UICreditsText_State_Init;

        if (!self->text.chars)
            RSDK.InitString(&self->text, "UNTITLED", 0);

        bool32 isHeading = self->isHeading;
        UICreditsText_SetText(self->animator.animationID, self, &self->text);
        self->isHeading = isHeading;
    }
}

void UICreditsText_StageLoad(void) { UICreditsText->aniFrames = RSDK.LoadSpriteAnimation("UI/CreditsText.bin", SCOPE_STAGE); }

void UICreditsText_SetText(int32 animID, EntityUICreditsText *label, String *text)
{
    RSDK.SetSpriteAnimation(UICreditsText->aniFrames, animID, &label->animator, true, 0);

    label->listID    = animID;
    label->isHeading = animID <= 4;
    RSDK.CopyString(&label->text, text);

    RSDK.SetSpriteString(UICreditsText->aniFrames, label->listID, &label->text);

    SpriteFrame *frame = RSDK.GetFrame(UICreditsText->aniFrames, animID, 0);
    if (frame)
        label->clipY2 = frame->pivotY + frame->height + 2;
}

void UICreditsText_State_Init(void)
{
    RSDK_THIS(UICreditsText);

    if ((self->position.y >> 16) - ScreenInfo->position.y - ScreenInfo->center.y < ScreenInfo->center.y - 16) {
        self->active  = ACTIVE_NORMAL;
        self->visible = true;

        if (self->animator.animationID == 7) {
            self->animator.frameID = self->text.chars[1];
            self->scaleSpeed       = 64;
            self->drawFX           = FX_SCALE;
            self->state            = UICreditsText_State_ScaleIn;
        }
        else if (self->isHeading) {
            UICreditsText_State_SetupCharPos();
        }
        else {
            self->inkEffect = INK_ALPHA;
            self->state     = UICreditsText_State_FadeIn;
        }
    }
}

void UICreditsText_State_SetupCharPos(void)
{
    RSDK_THIS(UICreditsText);

    self->visible = true;

    int32 yOffset = 0x280000;
    for (int32 c = 0; c < self->text.length; ++c) {
        self->charPositions[c].y = yOffset;
        self->charOffsets[c]     = -0x80000;
        yOffset += 0x100000;
    }

    self->scaleTimer = 0;
    self->scaleSpeed = 0x20;
    self->state      = UICreditsText_State_MoveChars;
}

void UICreditsText_State_MoveChars(void)
{
    RSDK_THIS(UICreditsText);

    for (int32 c = 0; c < self->text.length; ++c) {
        if (self->charPositions[c].y < 0)
            self->charOffsets[c] += 0x28000;

        self->charPositions[c].y += self->charOffsets[c];

        if (self->charPositions[c].y > 0 && self->charOffsets[c] > 0)
            self->charPositions[c].y = 0;
    }

    if (self->scaleTimer > 0)
        self->scaleSpeed -= 2;

    self->scaleTimer += self->scaleSpeed;

    if (self->scaleTimer < 256 && self->scaleSpeed < 0)
        self->scaleTimer = 256;

    if (++self->timer == 120) {
        self->timer = 0;
        self->state = UICreditsText_SetupIdleDelay;
    }
}

void UICreditsText_State_ScaleIn(void)
{
    RSDK_THIS(UICreditsText);

    if (self->scale.y > 0)
        self->scaleSpeed -= 4;

    self->scale.y += self->scaleSpeed;

    if (self->scale.y < 0x200 && self->scaleSpeed < 0)
        self->scale.y = 0x200;

    self->scale.x = self->scale.y;

    if (++self->timer == 120) {
        self->timer  = 0;
        self->drawFX = FX_NONE;
        self->state  = UICreditsText_SetupIdleDelay;
    }
}

void UICreditsText_State_FadeIn(void)
{
    RSDK_THIS(UICreditsText);

    if (self->alpha >= 0x100)
        self->state = UICreditsText_SetupIdleDelay;
    else
        self->alpha += 8;
}

void UICreditsText_SetupIdleDelay(void)
{
    RSDK_THIS(UICreditsText);

    if ((self->position.y >> 16) - ScreenInfo->position.y < 32) {
        if (self->animator.animationID == 7) {
            self->drawFX = FX_SCALE;
            self->state  = UICreditsText_State_ScaleOut;
        }
        else if (self->isHeading) {
            for (int32 c = 0; c < self->text.length; ++c) {
                self->charTimers[c]  = 2 * (self->text.length - c - 1);
                self->charOffsets[c] = -0x80000;
            }

            self->state = UICreditsText_State_Idle;
        }
        else {
            self->state = UICreditsText_State_FadeOut;
        }
    }
}

void UICreditsText_State_Idle(void)
{
    RSDK_THIS(UICreditsText);

    for (int32 c = 0; c < self->text.length; ++c) {
        if (self->charTimers[c] <= 0) {
            self->charOffsets[c] += 0x28000;
            self->charPositions[c].y += self->charOffsets[c];
        }
        else {
            --self->charTimers[c];
        }
    }

    if (self->timer > 12 && self->scaleTimer > 0)
        self->scaleTimer -= 16;

    if (++self->timer == 60)
        destroyEntity(self);
}

void UICreditsText_State_ScaleOut(void)
{
    RSDK_THIS(UICreditsText);

    if (self->scale.y > 0)
        self->scale.y -= 0x20;
    self->scale.x = self->scale.y;

    if (++self->timer == 60)
        destroyEntity(self);
}

void UICreditsText_State_FadeOut(void)
{
    RSDK_THIS(UICreditsText);

    if (self->alpha <= 0)
        self->state = UICreditsText_SetupIdleDelay;
    else
        self->alpha -= 8;
}

#if GAME_INCLUDE_EDITOR
void UICreditsText_EditorDraw(void)
{
    RSDK_THIS(UICreditsText);

    RSDK.SetSpriteAnimation(UICreditsText->aniFrames, self->listID, &self->animator, true, 0);

    UICreditsText_Draw();
}

void UICreditsText_EditorLoad(void) { UICreditsText->aniFrames = RSDK.LoadSpriteAnimation("UI/CreditsText.bin", SCOPE_STAGE); }
#endif

void UICreditsText_Serialize(void)
{
    RSDK_EDITABLE_VAR(UICreditsText, VAR_STRING, text);
    RSDK_EDITABLE_VAR(UICreditsText, VAR_STRING, tag);
    RSDK_EDITABLE_VAR(UICreditsText, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UICreditsText, VAR_BOOL, isHeading);
    RSDK_EDITABLE_VAR(UICreditsText, VAR_BOOL, hasShape);
}
