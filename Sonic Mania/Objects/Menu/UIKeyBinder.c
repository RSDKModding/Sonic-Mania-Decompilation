#include "SonicMania.h"

ObjectUIKeyBinder *UIKeyBinder;

void UIKeyBinder_Update(void)
{
    RSDK_THIS(UIKeyBinder);
    entity->touchPosStart.x = entity->field_110;
    entity->touchPosEnd.x   = 0;
    entity->touchPosEnd.y   = 0;
    entity->touchPosStart.x += 3 * entity->field_114;
    entity->touchPosStart.y = entity->field_114 + 0x60000;

    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator1, true, entity->frameID);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    }

    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    int32 input               = entity->inputID + 1;
    int32 mappings            = UIKeyBinder_GetMappings(input, entity->type);

    TextInfo info;
    INIT_TEXTINFO(info);
    bool32 flag = true;

    int32 frameID = -1;
    if (entity->field_16C == mappings) {
        flag = false;
    }
    else if (mappings == -1) {
        RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &entity->animator2, true, 0);
        entity->field_16C = -1;
        flag              = false;
    }
    else {
        frameID = UIButtonPrompt_MappingsToFrame(mappings);
    }

    for (int32 b = 0; b <= 8 && flag; ++b) {
        for (int32 i = 1; i <= 2 && flag; ++i) {
            if ((b != entity->type || i != input) && mappings) {
                if (UIKeyBinder_GetMappings(i, b) != mappings)
                    continue;
                if (entity->state != UIKeyBinder_Unknown14)
                    break;

                UIKeyBinder->curInputID  = i;
                UIKeyBinder->curMappings = b;

                int32 str = -1;
                if (i == input) {
                    str = STR_KEYALREADYBOUND;
                }
                else if (i == 1) {
                    str = STR_KEYALREADYBOUNDP1;
                }
                else if (i == 2) {
                    str = STR_KEYALREADYBOUNDP2;
                }
                if (str != -1)
                    Localization_GetString(&info, str);

                UIKeyBinder_SetMappings(entity->type, input, 0);
                entity->field_16C      = 0;
                EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
                if (dialog) {
                    UIDialog_AddButton(DIALOG_NO, dialog, UIKeyBinder_Unknown15, true);
                    UIDialog_AddButton(DIALOG_YES, dialog, UIKeyBinder_Unknown16, true);
                    UIDialog_Setup(dialog);
                }
                flag = false;
            }
        }
    }

    if (flag) {
        if (frameID || entity->state != UIKeyBinder_Unknown14) {
            RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &entity->animator2, true, frameID);
            entity->field_16C = mappings;
            if (entity->state == UIKeyBinder_Unknown14) {
                LogHelpers_Print("bind = %d 0x%02x", mappings, mappings);
                UIKeyBinder->flag         = false;
                parent->selectionDisabled = false;
                entity->processButtonCB   = UIButton_Unknown6;
                entity->state             = UIKeyBinder_Unknown13;
                UIKeyBinder->activeBinder = 0;
                parent->childHasFocus     = false;
                RSDK.SetSettingsValue(SETTINGS_CHANGED, 1);
                RSDK.PlaySfx(UIWidgets->sfx_Accept, false, 255);
            }
        }
        else {
            LogHelpers_Print("bind = %d 0x%02x", mappings, mappings);
            int32 frame = UIButtonPrompt_MappingsToFrame(entity->field_16C);
            RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &entity->animator2, true, frame);
            UIKeyBinder_SetMappings(input, entity->type, -1);
            RSDK.PlaySfx(UIKeyBinder->sfxFail, false, 255);
        }
    }

    StateMachine_Run(entity->state);

    int32 id = -1;
    for (int32 i = 0; i < parent->buttonCount; ++i) {
        if (entity == (EntityUIKeyBinder *)parent->buttons[i]) {
            id = i;
            break;
        }
    }

    if (entity->state == UIKeyBinder_Unknown13 && (parent->state != UIControl_ProcessInputs || parent->activeEntityID != id)) {
        entity->flag  = false;
        entity->state = UIKeyBinder_Unknown12;
    }
}

void UIKeyBinder_LateUpdate(void) {}

void UIKeyBinder_StaticUpdate(void) {}

void UIKeyBinder_Draw(void) { UIKeyBinder_DrawSprites(); }

void UIKeyBinder_Create(void *data)
{
    RSDK_THIS(UIKeyBinder);
    entity->visible         = true;
    entity->drawOrder       = 2;
    entity->active          = ACTIVE_BOUNDS;
    entity->updateRange.x   = 0x800000;
    entity->updateRange.y   = 0x400000;
    entity->field_110       = 0x400000;
    entity->field_118       = 12;
    entity->field_114       = 0xC0000;
    entity->processButtonCB = UIButton_Unknown6;
    entity->touchCB         = UIButton_ProcessTouch;
    entity->options2        = UIKeyBinder_Options2CB;
    entity->options3        = UIKeyBinder_Options3CB;
    entity->failCB          = 0;
    entity->options5        = UIKeyBinder_Options5CB;
    entity->options6        = UIKeyBinder_Options6CB;
    entity->options7        = UIKeyBinder_Options7CB;
    entity->options8        = UIKeyBinder_Options8CB;
    entity->textFlag        = true;
    entity->field_12C       = 512;
    entity->listID          = 0;
    entity->frameID         = UIKeyBinder_Unknown4(entity->type);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator1, true, entity->frameID);
    entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    if (!RSDK_sceneInfo->inEditor) {
        int32 mappings = UIKeyBinder_GetMappings(entity->inputID + 1, entity->type);
        int32 frame    = UIButtonPrompt_MappingsToFrame(mappings);
        RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &entity->animator2, true, frame);
    }
}

void UIKeyBinder_StageLoad(void)
{
    UIKeyBinder->aniFrames = RSDK.LoadSpriteAnimation("UI/Buttons.bin", SCOPE_STAGE);
    UIKeyBinder->sfxFail   = RSDK.GetSFX("Stage/Fail.wav");
}

int32 UIKeyBinder_GetButtonListID(void)
{
    switch (Localization->language) {
        case LANGUAGE_FR: return 9; break;
        case LANGUAGE_IT: return 10; break;
        case LANGUAGE_GE: return 11; break;
        case LANGUAGE_SP: return 12; break;
        default: break;
    }
    return 1;
}

int32 UIKeyBinder_GetMappings(int32 input, int32 button)
{
    switch (button) {
        case 0: return RSDK_controller[input].keyUp.keyMap; break;
        case 1: return RSDK_controller[input].keyDown.keyMap; break;
        case 2: return RSDK_controller[input].keyLeft.keyMap; break;
        case 3: return RSDK_controller[input].keyRight.keyMap; break;
        case 4: return RSDK_controller[input].keyA.keyMap; break;
        case 5: return RSDK_controller[input].keyB.keyMap; break;
        case 6: return RSDK_controller[input].keyX.keyMap; break;
        case 7: return RSDK_controller[input].keyY.keyMap; break;
        case 8: return RSDK_controller[input].keyStart.keyMap; break;
        default: break;
    }
    return 0;
}

void UIKeyBinder_SetMappings(int32 input, int32 button, int32 mappings)
{
    switch (button) {
        case 0: RSDK_controller[input].keyUp.keyMap = mappings; break;
        case 1: RSDK_controller[input].keyDown.keyMap = mappings; break;
        case 2: RSDK_controller[input].keyLeft.keyMap = mappings; break;
        case 3: RSDK_controller[input].keyRight.keyMap = mappings; break;
        case 4: RSDK_controller[input].keyA.keyMap = mappings; break;
        case 5: RSDK_controller[input].keyB.keyMap = mappings; break;
        case 6: RSDK_controller[input].keyX.keyMap = mappings; break;
        case 7: RSDK_controller[input].keyY.keyMap = mappings; break;
        case 8: RSDK_controller[input].keyStart.keyMap = mappings; break;
        default: break;
    }
}

int32 UIKeyBinder_Unknown4(int32 id)
{
    switch (id) {
        case 0: return 7;
        case 1: return 8;
        case 2: return 9;
        case 3: return 10;
        case 4: return 13;
        case 5: return 1;
        case 6: return 3;
        case 7: return 11;
        case 8: return 12;
        default: break;
    }
    return 0;
}

void UIKeyBinder_DrawSprites(void)
{
    RSDK_THIS(UIKeyBinder);
    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->field_120;
    drawPos.y -= entity->field_120;
    UIWidgets_Unknown7(entity->field_114 >> 16, entity->field_110 >> 16, entity->field_118, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);

    drawPos.x += entity->field_120;
    drawPos.y += entity->field_120;
    drawPos.x += entity->field_120;
    drawPos.y += entity->field_120;
    UIWidgets_Unknown7(entity->field_114 >> 16, entity->field_110 >> 16, entity->field_118, 0, 0, 0, drawPos.x, drawPos.y);

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x += entity->field_120;
    drawPos.y += entity->field_120;
    drawPos.y += entity->field_11C;
    drawPos.x += 0xB0000 - (entity->field_110 >> 1);
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    if (entity->textFlag) {
        drawPos.x += 0x60000;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    }
}

void UIKeyBinder_Options2CB(void) {}

bool32 UIKeyBinder_Options7CB(void)
{
    RSDK_THIS(UIKeyBinder);
    return entity->state == UIKeyBinder_Unknown13;
}

bool32 UIKeyBinder_Options8CB(void)
{
    RSDK_THIS(UIKeyBinder);
    return entity->state == UIKeyBinder_Unknown14;
}

void UIKeyBinder_Options5CB(void)
{
    RSDK_THIS(UIKeyBinder);
    if (entity->state != UIKeyBinder_Unknown13) {
        entity->field_11C = 0;
        entity->field_120 = 0;
        entity->field_124 = -0x20000;
        entity->field_128 = -0x20000;
        entity->state     = UIKeyBinder_Unknown13;
    }
}

void UIKeyBinder_Options6CB(void)
{
    RSDK_THIS(UIKeyBinder);
    entity->state = UIKeyBinder_Unknown12;
}

void UIKeyBinder_Options3CB(void)
{
    RSDK_THIS(UIKeyBinder);
    if (!UIKeyBinder->flag) {
        UIKeyBinder->flag         = true;
        EntityUIControl *parent   = (EntityUIControl *)entity->parent;
        parent->childHasFocus     = true;
        parent->selectionDisabled = true;
        UIKeyBinder->activeBinder = (Entity *)entity;
        entity->state             = UIKeyBinder_Unknown14;
        RSDK.PlaySfx(UIWidgets->sfx_Accept, false, 255);
        UIKeyBinder_SetMappings(entity->inputID + 1, entity->type, -1);
    }
}

void UIKeyBinder_Unknown12(void)
{
    RSDK_THIS(UIKeyBinder);

    entity->textFlag = true;
    if (entity->field_11C) {
        int32 dist = -(entity->field_11C / abs(entity->field_11C));
        entity->field_11C += dist << 15;
        if (dist < 0) {
            if (entity->field_11C < 0) {
                entity->field_11C = 0;
            }
            else if (dist > 0 && entity->field_11C > 0)
                entity->field_11C = 0;
        }
        else if (dist > 0 && entity->field_11C > 0)
            entity->field_11C = 0;
    }

    if (entity->field_120) {
        int32 dist = -(entity->field_120 / abs(entity->field_120));
        entity->field_120 += dist << 16;
        if (dist < 0) {
            if (entity->field_120 < 0) {
                entity->field_120 = 0;
            }
            else if (dist > 0 && entity->field_120 > 0)
                entity->field_120 = 0;
        }
        else if (dist > 0 && entity->field_120 > 0)
            entity->field_120 = 0;
    }
}

void UIKeyBinder_Unknown13(void)
{
    RSDK_THIS(UIKeyBinder);

    entity->field_124 += 0x4000;
    entity->field_11C += entity->field_124;
    entity->textFlag = true;
    if (entity->field_11C >= 0 && entity->field_124 > 0) {
        entity->field_11C = 0;
        entity->field_124 = 0;
    }

    entity->field_128 += 0x4800;
    entity->field_120 += entity->field_128;
    if (entity->field_120 >= -0x20000 && entity->field_128 > 0) {
        entity->field_120 = -0x20000;
        entity->field_128 = 0;
    }
}

void UIKeyBinder_Unknown14(void)
{
    RSDK_THIS(UIKeyBinder);
    UIKeyBinder_Unknown13();

    entity->processButtonCB = UIKeyBinder_Options2CB;
    entity->textFlag        = !((entity->timer >> 1) & 1);
    entity->timer++;
}

void UIKeyBinder_Unknown15(void)
{
    EntityUIKeyBinder *binder = (EntityUIKeyBinder *)UIKeyBinder->activeBinder;
    if (binder->state == UIKeyBinder_Unknown14) {
        UIKeyBinder_SetMappings(binder->inputID + 1, binder->type, -1);
        UIKeyBinder->curInputID  = -1;
        UIKeyBinder->curMappings = -1;
    }
}

void UIKeyBinder_Unknown16(void)
{
    EntityUIKeyBinder *binder = (EntityUIKeyBinder *)UIKeyBinder->activeBinder;
    if (binder->state == UIKeyBinder_Unknown14) {
        int32 mappings = UIKeyBinder_GetMappings(UIKeyBinder->curInputID, UIKeyBinder->curMappings);
        // TODO: what is v3??
        // UIKeyBinder_SetMappings(UIKeyBinder->curInputID, v3, 0);
        UIKeyBinder_SetMappings(binder->inputID + 1, binder->type, mappings);

        EntityUIControl *parent   = (EntityUIControl *)binder->parent;
        UIKeyBinder->flag         = false;
        parent->selectionDisabled = false;
        binder->processButtonCB   = UIButton_Unknown6;
        binder->state             = UIKeyBinder_Unknown13;
        UIKeyBinder->activeBinder = NULL;
        parent->childHasFocus     = false;
        RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
        UIKeyBinder->curInputID  = -1;
        UIKeyBinder->curMappings = -1;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIKeyBinder_EditorDraw(void)
{
    RSDK_THIS(UIKeyBinder);
    entity->inkEffect = entity->disabled ? INK_BLEND : INK_NONE;
    UIKeyBinder_DrawSprites();
}

void UIKeyBinder_EditorLoad(void) { UIKeyBinder->aniFrames = RSDK.LoadSpriteAnimation("UI/Buttons.bin", SCOPE_STAGE); }
#endif

void UIKeyBinder_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIKeyBinder, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIKeyBinder, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(UIKeyBinder, VAR_UINT8, inputID);
}
