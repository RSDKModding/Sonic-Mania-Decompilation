#include "SonicMania.h"

ObjectUIChoice *UIChoice;

void UIChoice_Update(void)
{
    RSDK_THIS(UIChoice);
    EntityUIButton *parent  = (EntityUIButton *)self->parent;
    bool32 disabled         = self->disabled;
    self->touchPosStart.x = self->size.x;
    self->touchPosStart.y = self->size.y;
    self->touchPosEnd.y   = 0;
    self->touchPosStart.x = (self->touchPosStart.x + 3 * self->size.y) >> 1;
    self->touchPosStart.y = self->size.y + 0x60000;
    self->touchPosEnd.x   = -(self->touchPosStart.x >> 1);
    if (parent && (disabled || parent->disabled))
        disabled = true;

    if (self->aniFrames != UIWidgets->textSpriteIndex || self->isDisabled != disabled) {
        if (disabled)
            RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 7, &self->animator1, true, 0);
        else
            RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, self->listID, &self->animator1, true, self->frameID);
        self->aniFrames = UIWidgets->textSpriteIndex;
        self->isDisabled  = disabled;
    }

    StateMachine_Run(self->state);

    if (parent) {
        if (parent->state == UIButton_Unknown16) {
            self->field_134.x = 0;
            self->field_134.y = 0;
            self->flag        = false;
            self->state       = UIChoice_Unknown6;
        }
    }
}

void UIChoice_LateUpdate(void) {}

void UIChoice_StaticUpdate(void) {}

void UIChoice_Draw(void)
{
    RSDK_THIS(UIChoice);
    Vector2 drawPos;

    int32 size  = self->size.y + self->size.x;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->field_134.y;
    drawPos.y -= self->field_134.y;
    size >>= 16;
#if RETRO_USE_PLUS
    UIWidgets_DrawParallelogram(self->size.y >> 16, size, self->sizeIY, (UIWidgets->buttonColour >> 16) & 0xFF, (UIWidgets->buttonColour >> 8) & 0xFF,
                       (UIWidgets->buttonColour) & 0xFF, drawPos.x, drawPos.y);
#else
    UIWidgets_DrawParallelogram(self->size.y >> 16, size, self->sizeIY, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);
#endif

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += self->field_134.y;
    drawPos.y += self->field_134.y;
    UIWidgets_DrawParallelogram(self->size.y >> 16, size, self->sizeIY, 0, 0, 0, drawPos.x, drawPos.y);

    EntityUIButton *parent = (EntityUIButton *)self->parent;
    if (self->arrowWidth > 0 && self->flag && !(self->disabled || parent->disabled)) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        drawPos.y += self->field_134.x;
        drawPos.x -= self->arrowWidth << 15;
        drawPos.x += self->field_134.y;
        drawPos.y += self->field_134.y;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        drawPos.y += self->field_134.x;
        drawPos.x += self->arrowWidth << 15;
        drawPos.x += self->field_134.y;
        drawPos.y += self->field_134.y;
        RSDK.DrawSprite(&self->animator4, &drawPos, false);
    }

    if (self->textFlag) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        drawPos.y = self->field_134.x + self->position.y;
        drawPos.x = self->field_134.y + self->position.x;
        drawPos.y += self->field_134.y;
        if (self->align) {
            if (self->align == 2)
                drawPos.x += (self->size.x >> 1) - 0x60000;
        }
        else {
            drawPos.x += -0x60000 - (self->size.x >> 1);
        }
        if (self->auxIcon)
            RSDK.DrawSprite(&self->animator2, &drawPos, false);

        if (!self->noText)
            RSDK.DrawSprite(&self->animator1, &drawPos, false);
    }
}

void UIChoice_Create(void *data)
{
    RSDK_THIS(UIChoice);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = 2;
        self->active        = ACTIVE_NEVER;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x400000;
        self->sizeIY        = self->size.y >> 16;
        self->size.y        = abs(self->size.y);
        self->textFlag      = true;
        self->touchCB       = UIChoice_CheckTouch;
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, self->listID, &self->animator1, true, self->frameID);
        self->aniFrames = UIWidgets->textSpriteIndex;
        RSDK.SetSpriteAnimation(UIChoice->aniFrames, self->auxListID, &self->animator2, true, self->auxFrameID);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator4, true, 1);
    }
}

void UIChoice_StageLoad(void) { UIChoice->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIChoice_Unknown1(EntityUIChoice *entity)
{
    if (entity) {
        EntityUIButton *parent = (EntityUIButton *)entity->parent;
        entity->active         = ACTIVE_BOUNDS;
        if (!parent->disabled)
            entity->visible = true;
        entity->field_134.x = 0;
        entity->field_134.y = 0;
        entity->field_13C   = -0x20000;
        entity->field_140   = -0x20000;
        entity->flag        = true;
        entity->state       = UIChoice_Unknown7;
    }
}

void UIChoice_Unknown2(EntityUIButton *entity)
{
    if (entity) {
        entity->isDisabled = false;
        entity->dword138   = 0;
        entity->flag       = false;
        entity->state      = UIChoice_Unknown6;
    }
}

void UIChoice_TouchedCB_Left(void)
{
    RSDK_THIS(UIChoice);
    EntityUIButton *parent = (EntityUIButton *)self->parent;
    int32 id                 = parent->selection;
    EntityUIButton *ent    = NULL;

    do {
        if (--id < 0) {
            do
                id += parent->choiceCount;
            while (id < 0);
        }
        if (parent->choiceCount > 0) {
            Entity *ent = RSDK.GetEntityByID(id % parent->choiceCount - parent->choiceCount + RSDK.GetEntityID(parent));
            if (ent->objectID == UIChoice->objectID || ent->objectID == UIVsRoundPicker->objectID || ent->objectID == UIResPicker->objectID
                || ent->objectID == UIWinSize->objectID)
                continue;
        }
        ent = 0;
    } while ((ent && ent->disabled) && id != parent->selection);

    if (id != parent->selection) {
        UIButton_SetChoiceSelectionWithCB(parent, id);
        RSDK.PlaySfx(UIWidgets->sfxBleep, 0, 255);
    }
}

void UIChoice_TouchedCB_Right(void)
{
    RSDK_THIS(UIChoice);
    EntityUIButton *parent = (EntityUIButton *)self->parent;
    int32 id                 = parent->selection;
    EntityUIButton *ent    = NULL;
    do {
        id = (id + 1) % parent->choiceCount;
        if (parent->choiceCount > 0) {
            Entity *ent = RSDK.GetEntityByID(id % parent->choiceCount - parent->choiceCount + RSDK.GetEntityID(parent));
            if (ent->objectID == UIChoice->objectID || ent->objectID == UIVsRoundPicker->objectID || ent->objectID == UIResPicker->objectID
                || ent->objectID == UIWinSize->objectID)
                continue;
        }
        ent = NULL;
    } while ((ent && ent->disabled) && id != parent->selection);

    if (id != parent->selection) {
        UIButton_SetChoiceSelectionWithCB(parent, id);
        RSDK.PlaySfx(UIWidgets->sfxBleep, 0, 255);
    }
}

bool32 UIChoice_CheckTouch(void)
{
    void (*callbacks[2])(void);
    Vector2 touchStart[2];
    Vector2 touchEnd[2];

    RSDK_THIS(UIChoice);
    callbacks[0]  = UIChoice_TouchedCB_Left;
    callbacks[1] = UIChoice_TouchedCB_Right;

    touchStart[0].x = self->touchPosStart.x;
    touchStart[0].y = self->touchPosStart.y;
    touchStart[1].x = self->touchPosStart.x;
    touchStart[1].y = self->touchPosStart.y;

    touchEnd[0].x = self->touchPosEnd.x;
    touchEnd[0].y = self->touchPosEnd.y;
    touchEnd[1].x = -self->touchPosEnd.x;
    touchEnd[1].y = self->touchPosEnd.y;

    bool32 pressed = false;
    for (int32 i = 0; i < 2; ++i) {
        if (TouchInfo->count) {
            int32 sizeX = touchStart[i].x >> 1;
            int32 sizeY = touchStart[i].y >> 1;
            for (int32 t = 0; t < TouchInfo->count; ++t) {
                int32 x = (ScreenInfo->position.x << 16) - ((TouchInfo->x[t] * ScreenInfo->width) * -65536.0f);
                int32 y = (ScreenInfo->position.y << 16) - ((TouchInfo->y[t] * ScreenInfo->height) * -65536.0f);

                int32 touchX = abs(touchEnd[i].x + self->position.x - x);
                int32 touchY = abs(touchEnd[i].y + self->position.y - y);
                if (touchX < sizeX && touchY < sizeY) {
                    self->touchID = i;
                    pressed         = true;
                }
            }
        }
        else if (self->touchPressed && self->touchID == i && !self->disabled) {
            callbacks[i]();
        }
    }

    self->touchPressed = pressed;
    return pressed;
}

void UIChoice_Unknown6(void)
{
    RSDK_THIS(UIChoice);

    if (self->field_134.x) {
        int32 val = -(self->field_134.x / abs(self->field_134.x));
        self->field_134.x += val << 15;
        if (val < 0 && self->field_134.x < 0) {
            self->field_134.x = 0;
        }
        else if (val > 0 && self->field_134.x > 0) {
            self->field_134.x = 0;
        }
    }

    if (self->field_134.y) {
        int32 val = -(self->field_134.y / abs(self->field_134.y));
        self->field_134.y += val << 16;
        if (val < 0 && self->field_134.y < 0) {
            self->field_134.y = 0;
        }
        else if (val > 0 && self->field_134.y > 0) {
            self->field_134.y = 0;
        }
    }
}

void UIChoice_Unknown7(void)
{
    RSDK_THIS(UIChoice);
    self->field_13C += 0x4000;
    self->field_134.x += self->field_13C;
    if (self->field_134.x >= 0 && self->field_13C > 0) {
        self->field_134.x = 0;
        self->field_13C   = 0;
    }

    self->field_140 += 0x4800;
    self->field_134.y += self->field_140;
    if (self->field_134.y >= -0x20000 && self->field_140 > 0) {
        self->field_134.y = -0x20000;
        self->field_140   = 0;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIChoice_EditorDraw(void)
{
    RSDK_THIS(UIChoice);
    self->drawOrder     = 2;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->sizeIY        = self->size.y >> 16;
    self->size.y        = abs(self->size.y);
    self->textFlag      = true;
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, self->listID, &self->animator1, true, self->frameID);
    self->aniFrames = UIWidgets->textSpriteIndex;
    RSDK.SetSpriteAnimation(UIChoice->aniFrames, self->auxListID, &self->animator2, true, self->auxFrameID);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator4, true, 1);

    UIChoice_Draw();
}

void UIChoice_EditorLoad(void) { UIChoice->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }
#endif

void UIChoice_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIChoice, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIChoice, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, frameID);
    RSDK_EDITABLE_VAR(UIChoice, VAR_BOOL, noText);
    RSDK_EDITABLE_VAR(UIChoice, VAR_BOOL, auxIcon);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, auxListID);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, auxFrameID);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, arrowWidth);
}
