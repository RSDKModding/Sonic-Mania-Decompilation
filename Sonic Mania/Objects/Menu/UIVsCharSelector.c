#include "SonicMania.h"

ObjectUIVsCharSelector *UIVsCharSelector;

void UIVsCharSelector_Update(void)
{
    RSDK_THIS(UIVsCharSelector);

    entity->prevFlag = entity->flag;
    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex || entity->prevFrameID != entity->frameID) {
        UIVsCharSelector_SetupText();
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
        entity->prevFrameID     = entity->frameID;
    }

    StateMachine_Run(entity->state);

    if (entity->flag) {
        entity->field_110 -= 0x600;
        entity->field_114 += entity->field_110;
        if (entity->field_114 <= 0x10000 && entity->field_110 < 0) {
            entity->field_114 = 0x10000;
            entity->field_110 = 0;
        }

        entity->field_118 -= 0x1800;
        entity->field_11C += entity->field_118;
        if (entity->field_11C <= 0x8000 && entity->field_118 < 0) {
            entity->field_11C = 0x8000;
            entity->field_118 = 0;
        }
    }
    else if (entity->state != UIVsCharSelector_Unknown11) {
        if (entity->field_114 > 0) {
            entity->field_114 -= 0x2000;
            if (entity->field_114 < 0)
                entity->field_114 = 0;
        }

        if (entity->field_11C > 0) {
            entity->field_11C -= 0x2000;
            if (entity->field_11C < 0)
                entity->field_11C = 0;
        }
    }

    entity->field_114 = minVal(entity->field_114, 0x13600);
    entity->field_11C = minVal(entity->field_11C, 0x11800);

    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    if ((entity->state == UIVsCharSelector_Unknown10 || entity->state == UIVsCharSelector_Unknown11)
        && (parent->state != UIControl_ProcessInputs || parent->active != ACTIVE_ALWAYS || parent->selectionDisabled)) {
        entity->flag      = false;
        entity->ready = false;
        entity->state     = UIVsCharSelector_Unknown8;
    }
}

void UIVsCharSelector_LateUpdate(void) {}

void UIVsCharSelector_StaticUpdate(void) {}

void UIVsCharSelector_Draw(void)
{
    RSDK_THIS(UIVsCharSelector);
    RSDK.DrawRect(entity->position.x - 0x2D0000, entity->position.y - 0x2D0000, 0x600000, 0x600000, 0xFFFFFF, 127, INK_BLEND, false);
    UIVsCharSelector_Unknown3();
    UIVsCharSelector_Unknown2();
    UIVsCharSelector_Unknown4();
}

void UIVsCharSelector_Create(void *data)
{
    RSDK_THIS(UIVsCharSelector);
    entity->frameID         = entity->playerID;
    entity->active          = ACTIVE_BOUNDS;
    entity->drawOrder       = 2;
    entity->visible         = true;
    entity->drawFX          = FX_FLIP;
    entity->updateRange.x   = 0x800000;
    entity->updateRange.y   = 0x300000;
    entity->processButtonCB = UIVsCharSelector_ProcessButtonCB;
    entity->state           = UIVsCharSelector_Unknown8;
    UIVsCharSelector_SetupText();
    entity->textSpriteIndex = UIWidgets->textSpriteIndex;
}

void UIVsCharSelector_StageLoad(void) { UIVsCharSelector->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIVsCharSelector_SetupText(void)
{
    RSDK_THIS(UIVsCharSelector);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 12, &entity->animator5, true, entity->playerID + 8);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 12, &entity->animator6, true, 7);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 8, &entity->animator3, true, entity->frameID);
}

void UIVsCharSelector_Unknown2(void)
{
    RSDK_THIS(UIVsCharSelector);
    if (!RSDK_sceneInfo->inEditor)
        UIWidgets_Unknown3(96, 96, RSDK_sceneInfo->entity->position.x + 0x30000, RSDK_sceneInfo->entity->position.y + 0x30000);
    if (entity->flag)
        UIWidgets_Unknown4(96, 96, entity->position.x, entity->position.y);
    else
        UIWidgets_Unknown2(96, 96, entity->position.x, entity->position.y);
}

void UIVsCharSelector_Unknown3(void)
{
    RSDK_THIS(UIVsCharSelector);
    UIWidgets_Unknown5(232, (entity->field_114 >> 11), 40, 88, entity->position.x - 0x2D0000, entity->position.y - 0x2D0000);
    UIWidgets_Unknown5(96, (-64 * entity->field_114) >> 16, 160, 176, entity->position.x + 0x2D0000, entity->position.y + 0x2C0000);
    UIWidgets_Unknown5(88, (-44 * entity->field_114) >> 16, 112, 224, entity->position.x + 0x2D0000, entity->position.y + 0x2C0000);
}

void UIVsCharSelector_Unknown4(void)
{
    RSDK_THIS(UIVsCharSelector);
    Vector2 drawPos;

    drawPos.x = entity->position.x - 0x2D0000;
    drawPos.y = entity->position.y + 0x180000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x5A0000, 0x100000, 0, 255, INK_NONE, false);
    RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 14, &entity->animator4, true, 1);

    drawPos.x = entity->position.x + 0x2D0000;
    drawPos.y = entity->position.y - 0x2D0000;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    drawPos.y += 0x80000;
    drawPos.x -= 0xA0000;
    RSDK.DrawSprite(&entity->animator5, &drawPos, false);

    if (entity->state == UIVsCharSelector_Unknown9) {
        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 14, &entity->animator1, true, 7);
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y - 0x80000;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        RSDK.DrawSprite(&entity->animator6, &drawPos, false);
    }
    else if (entity->state != UIVsCharSelector_Unknown11 || !(entity->timer & 2)) {
        int frameID = entity->frameID;
        if (entity->frameID > 2)
            frameID++;
        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 1, &entity->animator1, true, frameID);
        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 2, &entity->animator2, true, frameID);
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y - 0x80000;
        drawPos.x += 4 * entity->field_11C;
        drawPos.y += 4 * entity->field_11C;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        drawPos.x -= 8 * entity->field_11C;
        drawPos.y -= 8 * entity->field_11C;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y + 0x200000;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);
    }

    if (!entity->ready && entity->state != UIVsCharSelector_Unknown9) {
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y + 0x200000;
        if (RSDK.Sin256(2 * UIControl->timer) < 0)
            UIWidgets_Unknown9((-RSDK.Sin256(2 * UIControl->timer) + 0x880) << 11, drawPos.x, drawPos.y);
        else
            UIWidgets_Unknown9((RSDK.Sin256(2 * UIControl->timer) + 0x880) << 11, drawPos.x, drawPos.y);
    }
}

void UIVsCharSelector_ProcessButtonCB(void)
{
    RSDK_THIS(UIVsCharSelector);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    if (parent->active == ACTIVE_ALWAYS) {
        int storedFrame = entity->frameID;
        int inc         = 1;
        if (UIControl->leftPress[entity->playerID]) {
            --entity->frameID;
            inc               = -1;
            entity->field_11C = 0;
            entity->field_118 = 0x8000;
            RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
        }
        else if (UIControl->rightPress[entity->playerID]) {
            ++entity->frameID;
            entity->field_11C = 0;
            entity->field_118 = 0x8000;
            RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
        }

#if RETRO_USE_PLUS
        int max = 3 + (API.CheckDLC(DLC_PLUS) ? 2 : 0);
#else
        int max = 3;
#endif
        while (entity->frameID < 0) {
            entity->frameID += max;
        }

        while (entity->frameID >= max) {
            entity->frameID -= max;
        }

        int activePlayers = 0;
        for (int i = 0; i < parent->buttonCount; ++i) {
            EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)parent->buttons[i];
            if (button->state == UIVsCharSelector_Unknown11 || button->processButtonCB == UIVsCharSelector_ProcessButtonCB_Alt)
                activePlayers |= (1 << button->frameID);
        }

        int frame = entity->frameID;
        while ((1 << frame) & activePlayers) {
            frame += inc;
            if (frame < 0)
                frame += max;
            if (frame >= max)
                frame -= max;
            entity->frameID = frame;
        }

        if (entity->frameID != frame) {
            entity->field_11C = 0;
            entity->field_118 = 0x8000;
        }
        if (storedFrame != entity->frameID)
            UIVsCharSelector_SetupText();

        bool32 pressed = false;
        if (API.GetConfirmButtonFlip())
            pressed = RSDK_controller[entity->playerID + 1].keyB.press;
        else
            pressed = RSDK_controller[entity->playerID + 1].keyA.press;
        if (pressed) {
            entity->timer           = 0;
            entity->state           = UIVsCharSelector_Unknown11;
            entity->processButtonCB = StateMachine_None;
            entity->ready           = true;
            RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 255);
        }
        else if (!entity->flag && !entity->ready) {
            if (parent->active == ACTIVE_ALWAYS)
                UIVsCharSelector_Unknown7();
        }
    }
}

void UIVsCharSelector_ProcessButtonCB_Alt(void)
{
    RSDK_THIS(UIVsCharSelector);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    if (parent->active == ACTIVE_ALWAYS) {
        parent->posUnknown.x = entity->position.x;

        bool32 pressed = false;
        if (API.GetConfirmButtonFlip())
            pressed = RSDK_controller[entity->playerID + 1].keyB.press;
        else
            pressed = RSDK_controller[entity->playerID + 1].keyA.press;
        if (pressed) {
            entity->timer = 0;
            UIVsCharSelector_Unknown7();
            entity->processButtonCB = UIVsCharSelector_ProcessButtonCB;
            entity->ready           = false;
        }
    }
}

void UIVsCharSelector_Unknown7(void)
{
    RSDK_THIS(UIVsCharSelector);

    entity->state = UIVsCharSelector_Unknown10;
    if (!entity->flag) {
        entity->field_110 = 0x4000;
        entity->field_11C = 0;
        entity->field_118 = 0x8000;
        entity->flag      = true;
    }
}

void UIVsCharSelector_Unknown8(void)
{
    RSDK_THIS(UIVsCharSelector);

    int id = RSDK.ControllerIDForInputID(entity->playerID + 1);
    if (!id || id == CONT_AUTOASSIGN) {
        entity->state = UIVsCharSelector_Unknown9;
        UIVsCharSelector_Unknown9();
    }
}

void UIVsCharSelector_Unknown9(void)
{
    RSDK_THIS(UIVsCharSelector);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    entity->processButtonCB = StateMachine_None;
    entity->flag            = false;
    entity->ready           = false;

    int id       = RSDK.ControllerIDForInputID(entity->playerID + 1);
    int assigned = RSDK.GetAssignedControllerID(id);
    if (parent->active == ACTIVE_ALWAYS) {
        if (!id) {
            RSDK.AssignControllerID(entity->playerID + 1, CONT_AUTOASSIGN);
        }
        else if (!assigned && id != CONT_AUTOASSIGN) {
            RSDK.AssignControllerID(entity->playerID + 1, CONT_AUTOASSIGN);
        }
        else if (id != CONT_AUTOASSIGN) {
            entity->field_11C = 0;
            entity->field_118 = 0x8000;
            entity->timer     = 4;
            entity->state     = UIVsCharSelector_Unknown10;
        }
    }
}

void UIVsCharSelector_Unknown10(void)
{
    RSDK_THIS(UIVsCharSelector);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    entity->flag = true;
    int id       = RSDK.ControllerIDForInputID(entity->playerID + 1);
    int assigned = RSDK.GetAssignedControllerID(id);

    if (parent->active == ACTIVE_ALWAYS) {
        if (id && id != CONT_AUTOASSIGN && assigned) {
            if (entity->timer > 0) {
                if (entity->timer == 1)
                    entity->processButtonCB = UIVsCharSelector_ProcessButtonCB;
                entity->timer--;
            }
        }
        else {
            entity->state = UIVsCharSelector_Unknown9;
        }
    }
}

void UIVsCharSelector_Unknown11(void)
{
    RSDK_THIS(UIVsCharSelector);

    if (entity->timer >= 30) {
        if (entity->parent->active == ACTIVE_ALWAYS) {
            entity->flag            = false;
            entity->timer           = 0;
            entity->state           = UIVsCharSelector_Unknown8;
            entity->processButtonCB = UIVsCharSelector_ProcessButtonCB_Alt;
            StateMachine_Run(entity->options2);
        }
    }
    else {
        if (entity->timer == 2) {
            switch (entity->frameID) {
                case 0: RSDK.PlaySFX(Announcer->sfx_Sonic, false, 255); break;
                case 1: RSDK.PlaySFX(Announcer->sfx_Tails, false, 255); break;
                case 2: RSDK.PlaySFX(Announcer->sfx_Knuckles, false, 255); break;
#if RETRO_USE_PLUS
                case 3: RSDK.PlaySFX(Announcer->sfx_Mighty, false, 255); break;
                case 4: RSDK.PlaySFX(Announcer->sfx_Ray, false, 255); break;
#endif
                default: break;
            }
        }
        ++entity->timer;
    }
}

void UIVsCharSelector_EditorDraw(void) {}

void UIVsCharSelector_EditorLoad(void) {}

void UIVsCharSelector_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsCharSelector, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIVsCharSelector, VAR_UINT8, playerID);
}
