// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIPopover Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectUIPopover *UIPopover;

void UIPopover_Update(void)
{
    RSDK_THIS(UIPopover);

    StateMachine_Run(self->state);

    UIPopover_SetupButtonPositions();

    if (self->closeDelay > 0) {
        self->closeDelay--;

        if (!self->closeDelay)
            UIPopover_BackPressCB();
    }
}

void UIPopover_LateUpdate(void) {}

void UIPopover_StaticUpdate(void) {}

void UIPopover_Draw(void) { UIPopover_DrawSprites(); }

void UIPopover_Create(void *data)
{
    RSDK_THIS(UIPopover);

    self->active      = ACTIVE_ALWAYS;
    self->drawGroup   = 14;
    self->visible     = true;
    self->buttonCount = 0;
}

void UIPopover_StageLoad(void)
{
    UIPopover->storedControl = NULL;
    UIPopover->activePopover = NULL;
}

EntityUIPopover *UIPopover_CreatePopover(void)
{
    if (UIPopover->activePopover) {
        LogHelpers_Print("EXCEPTION: Called CreatePopover when an activePopover already existed.");
        return NULL;
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_POPOVER, UIPopover)->classID) {
            LogHelpers_Print("Can't create UIPopover (%d), entity already exists in slot (class ID: %d)", UIPopover->classID,
                             RSDK_GET_ENTITY(SLOT_POPOVER, UIPopover)->classID);
            return NULL;
        }
        else {
            RSDK.ResetEntitySlot(SLOT_POPOVER, UIPopover->classID, NULL);

            EntityUIPopover *popover = RSDK_GET_ENTITY(SLOT_POPOVER, UIPopover);
            popover->position.x      = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
            popover->position.y      = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
            UIPopover->activePopover = popover;

            return popover;
        }
    }
}

void UIPopover_AddButton(EntityUIPopover *popover, uint8 frameID, void (*callback)(void), bool32 closeOnSelect)
{
    int32 id = popover->buttonCount;

    if (id < UIPOPOVER_OPTION_COUNT) {
        popover->frameIDs[id]      = frameID;
        popover->callbacks[id]     = callback;
        popover->closeOnSelect[id] = closeOnSelect;

        int32 slot = popover->buttonCount + SLOT_POPOVER_BUTTONS;
        RSDK.ResetEntitySlot(slot, UIButton->classID, 0);
        EntityUIButton *button = RSDK_GET_ENTITY(slot, UIButton);

        button->position.x = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
        button->position.y = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 16, &button->animator, true, frameID);

        button->textFrames = UIWidgets->textFrames;
        button->actionCB   = UIPopover_ButtonActionCB;
        button->size.x     = 0x380000;
        button->size.y     = 0x100000;
        button->bgEdgeSize = 16;
        button->align      = UIBUTTON_ALIGN_CENTER;
        button->active     = ACTIVE_ALWAYS;
        button->drawGroup  = popover->drawGroup;

        popover->buttons[id] = button;
        ++popover->buttonCount;

        EntityUIControl *parent = popover->parent;
        if (parent) {
            button->parent      = (Entity *)parent;
            parent->buttons[id] = button;
            parent->buttonCount = popover->buttonCount;
        }
    }
}

void UIPopover_Setup(EntityUIPopover *popover, int32 x, int32 y)
{
    if (popover) {
        Vector2 size;
        size.x = ScreenInfo->size.x << 16;
        size.y = ScreenInfo->size.y << 16;

        bool32 tookFocus = false;
        foreach_all(UIControl, controlPtr)
        {
            if (controlPtr->active == ACTIVE_ALWAYS) {
                tookFocus                     = true;
                controlPtr->popoverHasFocus   = true;
                UIPopover->storedControl      = controlPtr;
                UIPopover->storedControlState = controlPtr->state;
                foreach_break;
            }
        }

        RSDK.ResetEntitySlot(SLOT_POPOVER_UICONTROL, UIControl->classID, &size);

        EntityUIControl *control   = RSDK_GET_ENTITY(SLOT_POPOVER_UICONTROL, UIControl);
        control->menuWasSetup      = true;
        control->position.x        = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
        control->position.y        = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
        control->rowCount          = popover->buttonCount;
        control->columnCount       = 1;
        control->buttonID          = 0;
        control->backPressCB       = UIPopover_BackPressCB;
        control->selectionDisabled = true;

        popover->parent = control;
        if (!tookFocus) {
            UIPopover->storedControl      = NULL;
            UIPopover->storedControlState = StateMachine_None;
        }

        int32 b = 0;
        for (; b < UIPOPOVER_OPTION_COUNT; ++b) {
            if (!popover->buttons[b])
                break;

            EntityUIButton *button = popover->buttons[b];
            control->buttons[b]    = button;
            button->parent         = (Entity *)control;
        }

        control->buttonCount = b;
        int32 sizeY          = (0x180000 * b) + 0x200000;
        popover->position.x  = x;
        popover->position.y  = y;
        popover->size.y      = sizeY;
        popover->size.x      = 0x800000;
        popover->position.y += -0x80000 - (sizeY >> 1);
        popover->triangleMode = 1;
        popover->timer        = 0;
        popover->state        = UIPopover_State_Appear;
    }
}

void UIPopover_DrawSprites(void)
{
    RSDK_THIS(UIPopover);

    RSDK.DrawRect(self->position.x + 0x30000 - (self->size.x >> 1), self->position.y + 0x30000 - (self->size.y >> 1), self->size.x, self->size.y, 0,
                  0xFF, INK_BLEND, false);

    RSDK.DrawRect(self->position.x - (self->size.x >> 1), self->position.y - (self->size.y >> 1), self->size.x, self->size.y, 0x30A0F0, 0xFF,
                  INK_NONE, false);

    if (self->triangleMode == 1) {
        int32 x = self->position.x + 0x30000;
        int32 y = (self->size.y >> 1) + self->position.y + 0x30000;
        UIWidgets_DrawEquilateralTriangle(x, y, 8, true, 0x00, 0x00, 0x00, INK_BLEND);
        UIWidgets_DrawEquilateralTriangle(x - 0x30000, y - 0x30000, 8, self->triangleMode, 0x30, 0xA0, 0xE0, INK_NONE);
    }
}

void UIPopover_SetupButtonPositions(void)
{
    RSDK_THIS(UIPopover);

    int32 offsets[] = { 0, 0, 24, 24, 24 };

    int32 posX    = self->position.x;
    int32 offsetY = offsets[self->buttonCount] << 16;

    int32 posY = self->position.y - ((offsetY * MAX(self->buttonCount - 1, 0)) >> 1);
    for (int32 b = 0; b < UIPOPOVER_OPTION_COUNT; ++b) {
        if (!self->buttons[b])
            break;

        EntityUIButton *button = self->buttons[b];
        button->position.x     = posX;
        button->position.y     = posY;
        button->startPos.x     = posX;
        button->startPos.y     = posY;

        posY += offsetY;
    }
}

void UIPopover_Close(void)
{
    RSDK_THIS(UIPopover);

    EntityUIControl *parent = (EntityUIControl *)self->parent;

    if (parent) {
        UIControl_SetInactiveMenu(parent);
        destroyEntity(parent);
    }

    for (int32 i = UIPOPOVER_OPTION_COUNT - 1; i > 0; --i) {
        if (self->buttons[i])
            destroyEntity(self->buttons[i]);
    }

    EntityUIControl *control = UIPopover->storedControl;
    if (control) {
        UIControl_SetMenuLostFocus(UIPopover->storedControl);
        control->state           = UIPopover->storedControlState;
        control->popoverHasFocus = false;
    }

    UIPopover->storedControl      = NULL;
    UIPopover->storedControlState = NULL;

    StateMachine_Run(self->closeCB);

    UIPopover->activePopover = NULL;
    destroyEntity(self);
}

bool32 UIPopover_BackPressCB(void)
{
    EntityUIPopover *popover = UIPopover->activePopover;

    if (popover && popover->state != UIPopover_State_Close) {
        popover->parent->selectionDisabled = true;
        popover->timer                     = 0;
        popover->state                     = UIPopover_State_Close;
        popover->closeCB                   = StateMachine_None;
    }

    return true;
}

void UIPopover_ButtonActionCB(void)
{
    EntityUIPopover *popover = UIPopover->activePopover;
    EntityUIControl *control = (EntityUIControl *)popover->parent;

    if (control && control->buttonID >= 0 && control->buttonID < control->buttonCount) {
        if (popover->closeOnSelect[control->buttonID]) {
            if (popover && popover->state != UIPopover_State_Close) {
                control                    = popover->parent;
                control->selectionDisabled = true;
                popover->timer             = 0;
                popover->state             = UIPopover_State_Close;
                popover->closeCB           = popover->callbacks[control->buttonID];
            }
        }
        else {
            StateMachine_Run(popover->callbacks[control->buttonID]);
        }
    }
}

void UIPopover_State_Appear(void)
{
    RSDK_THIS(UIPopover);

    if (self->timer == 1) {
        RSDK.PlaySfx(UIWidgets->sfxWoosh, false, 255);

        EntityUIControl *control = (EntityUIControl *)self->parent;
        UIControl_HandleMenuLoseFocus(control);
        control->selectionDisabled = false;

        self->timer = 0;
        self->state = UIPopover_State_Idle;
    }
    else {
        self->timer++;
    }
}

void UIPopover_State_Idle(void) {}

void UIPopover_State_Close(void) { UIPopover_Close(); }

#if GAME_INCLUDE_EDITOR
void UIPopover_EditorDraw(void) { UIPopover_DrawSprites(); }

void UIPopover_EditorLoad(void) {}
#endif

void UIPopover_Serialize(void) {}
#endif
