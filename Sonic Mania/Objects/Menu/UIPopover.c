#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUIPopover *UIPopover;

void UIPopover_Update(void)
{
    RSDK_THIS(UIPopover);

    StateMachine_Run(entity->state);

    UIPopover_SetupButtonPositions();

    if (entity->timer2 > 0) {
        entity->timer2--;
        if (!entity->timer2)
            UIPopover_BackPressCB();
    }
}

void UIPopover_LateUpdate(void) {}

void UIPopover_StaticUpdate(void) {}

void UIPopover_Draw(void) { UIPopover_DrawSprites(); }

void UIPopover_Create(void *data)
{
    RSDK_THIS(UIPopover);
    entity->active      = ACTIVE_ALWAYS;
    entity->drawOrder   = 14;
    entity->visible     = true;
    entity->buttonCount = 0;
}

void UIPopover_StageLoad(void)
{
    UIPopover->storedControl = NULL;
    UIPopover->activeEntity  = NULL;
}

EntityUIPopover *UIPopover_CreatePopover(void)
{
    if (UIPopover->activeEntity) {
        LogHelpers_Print("EXCEPTION: Called CreatePopover when an activePopover already existed.");
        return NULL;
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_POPOVER, UIPopover)->objectID) {
            LogHelpers_Print("Can't create UIPopover (%d), entity already exists in slot (class ID: %d)", UIPopover->objectID,
                             RSDK_GET_ENTITY(SLOT_POPOVER, UIPopover)->objectID);
            return NULL;
        }
        else {
            RSDK.ResetEntitySlot(SLOT_POPOVER, UIPopover->objectID, NULL);
            EntityUIPopover *popover = RSDK.GetEntityByID(SLOT_POPOVER);
            popover->position.x      = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
            popover->position.y      = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
            UIPopover->activeEntity  = (Entity *)popover;
            return popover;
        }
    }
}

void UIPopover_AddButton(EntityUIPopover *popover, uint8 frameID, void (*callback)(void), bool32 flag)
{
    int32 id = popover->buttonCount;
    if (id < 4) {
        popover->frameIDs[id]  = frameID;
        popover->callbacks[id] = callback;
        popover->flags[id]     = flag;

        int32 slot = popover->buttonCount + SLOT_POPOVER_BUTTONS;
        RSDK.ResetEntitySlot(slot, UIButton->objectID, 0);
        EntityUIButton *button = RSDK_GET_ENTITY(slot, UIButton);

        button->position.x = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        button->position.y = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &button->animator, true, frameID);
        button->textSpriteIndex = UIWidgets->textSpriteIndex;
        button->options2        = UIPopover_Unknown8;
        button->size.x          = 0x380000;
        button->size.y          = 0x100000;
        button->dword138        = 16;
        button->align           = 1;
        button->active          = ACTIVE_ALWAYS;
        button->drawOrder       = popover->drawOrder;
        popover->buttons[id]    = button;
        ++popover->buttonCount;

        EntityUIControl *parent = popover->parent;
        if (parent) {
            button->parent      = (Entity *)parent;
            parent->buttons[id] = button;
            parent->buttonCount = popover->buttonCount;
        }
    }
}

void UIPopover_Setup(EntityUIPopover *popover, int32 posX, int32 posY)
{
    if (popover) {
        Vector2 size;
        size.x = RSDK_screens->width << 16;
        size.y = RSDK_screens->height << 16;

        bool32 controlFlag = false;
        foreach_all(UIControl, controlPtr)
        {
            if (controlPtr->active == ACTIVE_ALWAYS) {
                controlFlag                   = true;
                controlPtr->popoverHasFocus   = true;
                UIPopover->storedControl      = controlPtr;
                UIPopover->storedControlState = controlPtr->state;
                foreach_break;
            }
        }

        RSDK.ResetEntitySlot(SLOT_POPOVER_UICONTROL, UIControl->objectID, &size);
        EntityUIControl *control   = RSDK.GetEntityByID(SLOT_POPOVER_UICONTROL);
        control->dwordCC           = true;
        control->position.x        = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        control->position.y        = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
        control->rowCount          = popover->buttonCount;
        control->columnCount       = 1;
        control->activeEntityID    = 0;
        control->backPressCB       = UIPopover_BackPressCB;
        control->selectionDisabled = true;
        popover->parent            = control;
        if (!controlFlag) {
            UIPopover->storedControl      = NULL;
            UIPopover->storedControlState = StateMachine_None;
        }

        int32 b = 0;
        for (; b < 4; ++b) {
            if (!popover->buttons[b])
                break;
            EntityUIButton *button = popover->buttons[b];
            control->buttons[b]    = button;
            button->parent         = (Entity *)control;
        }

        control->buttonCount = b;
        int32 sizeY            = (0x180000 * b) + 0x200000;
        popover->position.x  = posX;
        popover->position.y  = posY;
        popover->size.y      = sizeY;
        popover->size.x      = 0x800000;
        popover->position.y += -0x80000 - (sizeY >> 1);
        popover->field_C8 = 1;
        popover->timer    = 0;
        popover->state    = UIPopover_Unknown9;
    }
}

void UIPopover_DrawSprites(void)
{
    RSDK_THIS(UIPopover);

    RSDK.DrawRect(entity->position.x + 0x30000 - (entity->size.x >> 1), entity->position.y + 0x30000 - (entity->size.y >> 1), entity->size.x,
                  entity->size.y, 0, 255, INK_BLEND, false);
    RSDK.DrawRect(entity->position.x - (entity->size.x >> 1), entity->position.y - (entity->size.y >> 1), entity->size.x, entity->size.y, 0x30A0F0,
                  255, INK_NONE, false);

    if (entity->field_C8 == 1) {
        int32 x = entity->position.x + 0x30000;
        int32 y = (entity->size.y >> 1) + entity->position.y + 0x30000;
        UIWidgets_Unknown6(1, 0, 0, 0, 1, x, y);
        UIWidgets_Unknown6(entity->field_C8, 48, 160, 240, 0, x - 0x30000, y - 0x30000);
    }
}

void UIPopover_SetupButtonPositions(void)
{
    RSDK_THIS(UIPopover);
    int32 offsets[] = { 0, 0, 24, 24, 24 };

    int32 posX    = entity->position.x;
    int32 offsetY = offsets[entity->buttonCount] << 16;

    int32 posY = entity->position.y - ((offsetY * maxVal(entity->buttonCount - 1, 0)) >> 1);
    for (int32 b = 0; b < 4; ++b) {
        if (!entity->buttons[b])
            break;
        EntityUIButton *button = entity->buttons[b];
        button->position.x     = posX;
        button->position.y     = posY;
        button->posUnknown2.x  = posX;
        button->posUnknown2.y  = posY;
        posY += offsetY;
    }
}

void UIPopover_Close(void)
{
    RSDK_THIS(UIPopover);

    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    if (parent) {
        UIControl_Unknown6(parent);
        destroyEntity(parent);
    }

    for (int32 i = 0; i < 3; ++i) {
        if (entity->buttons[i])
            destroyEntity(entity->buttons[i]);
    }

    EntityUIControl *control = UIPopover->storedControl;
    if (control) {
        UIControl_Unknown5(UIPopover->storedControl);
        control->state           = UIPopover->storedControlState;
        control->popoverHasFocus = false;
    }
    UIPopover->storedControl      = NULL;
    UIPopover->storedControlState = NULL;
    StateMachine_Run(entity->unknownCallback);
    UIPopover->activeEntity = NULL;
    destroyEntity(entity);
}

bool32 UIPopover_BackPressCB(void)
{
    EntityUIPopover *popover = (EntityUIPopover *)UIPopover->activeEntity;
    if (popover && popover->state != UIPopover_Unknown11) {
        popover->parent->selectionDisabled = true;
        popover->timer                     = 0;
        popover->state                     = UIPopover_Unknown11;
        popover->unknownCallback           = StateMachine_None;
    }
    return true;
}

void UIPopover_Unknown8(void)
{
    EntityUIPopover *popover = (EntityUIPopover *)UIPopover->activeEntity;
    EntityUIControl *control = (EntityUIControl *)popover->parent;

    if (control) {
        if (control->activeEntityID >= 0) {
            if (control->activeEntityID < control->buttonCount) {
                if (popover->flags[control->activeEntityID]) {
                    if (popover && popover->state != UIPopover_Unknown11) {
                        control                    = popover->parent;
                        control->selectionDisabled = true;
                        popover->timer             = 0;
                        popover->state             = UIPopover_Unknown11;
                        popover->unknownCallback   = popover->callbacks[control->activeEntityID];
                    }
                }
                else {
                    StateMachine_Run(popover->callbacks[control->activeEntityID]);
                }
            }
        }
    }
}

void UIPopover_Unknown9(void)
{
    RSDK_THIS(UIPopover);

    if (entity->timer == 1) {
        RSDK.PlaySfx(UIWidgets->sfx_Woosh, false, 255);
        EntityUIControl *control = (EntityUIControl *)entity->parent;
        UIControl_Unknown12((Entity *)control);
        control->selectionDisabled = false;
        entity->timer              = 0;
        entity->state              = UIPopover_Unknown10;
    }
    else {
        entity->timer++;
    }
}

void UIPopover_Unknown10(void) {}

void UIPopover_Unknown11(void) { UIPopover_Close(); }

#if RETRO_INCLUDE_EDITOR
void UIPopover_EditorDraw(void) {}

void UIPopover_EditorLoad(void) {}
#endif

void UIPopover_Serialize(void) {}
#endif
