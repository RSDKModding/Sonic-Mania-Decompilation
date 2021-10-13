#include "SonicMania.h"

ObjectUIVsZoneButton *UIVsZoneButton;

void UIVsZoneButton_Update(void)
{
    RSDK_THIS(UIVsZoneButton);

    entity->touchPosStart.x = 0x500000;
    entity->touchPosStart.y = 0x380000;
    entity->touchPosEnd.x   = 0;
    entity->touchPosEnd.y   = 0;

    entity->disabled = entity->obfuscate || entity->xOut;
    if (entity->obfuscate != entity->prevObfuscate) {
        UIVsZoneButton_SetNameText();
        entity->prevObfuscate = entity->obfuscate;
    }

    StateMachine_Run(entity->state);

    if (entity->flag) {
        entity->field_124 -= 0x600;
        entity->field_12C += entity->field_124;
        if (entity->field_12C <= 0x10000 && entity->field_124 < 0) {
            entity->field_12C = 0x10000;
            entity->field_124 = 0;
        }

        entity->field_130 -= 0x1800;
        entity->field_134 += entity->field_130;
        if (entity->field_134 <= 0x8000 && entity->field_130 < 0) {
            entity->field_134 = 0x8000;
            entity->field_130 = 0;
        }
    }
    else if (entity->state != UIVsZoneButton_Unknown15) {
        if (entity->field_12C > 0)
            entity->field_12C = entity->field_12C >> 1;
        if (entity->field_134 > 0)
            entity->field_134 = entity->field_134 >> 1;
    }
    if (entity->zoneID == 12 || entity->xOut)
        RSDK.ProcessAnimation(&entity->zoneAnimator);

    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    if (entity->state == UIVsZoneButton_Unknown14) {
        if (parent->state != UIControl_ProcessInputs) {
            entity->flag  = false;
            entity->flag  = false;
            entity->state = UIVsZoneButton_Unknown13;
        }
        else {
            int32 id = -1;
            for (int32 i = 0; i < parent->buttonCount; ++i) {
                if (entity == (EntityUIVsZoneButton *)parent->buttons[i]) {
                    id = i;
                    break;
                }
            }

            if (parent->activeEntityID != id) {
                entity->flag  = false;
                entity->flag  = false;
                entity->state = UIVsZoneButton_Unknown13;
            }
        }
    }
}

void UIVsZoneButton_LateUpdate(void) {}

void UIVsZoneButton_StaticUpdate(void) {}

void UIVsZoneButton_Draw(void)
{
    RSDK_THIS(UIVsZoneButton);
    UIVsZoneButton_SetupAnimators();
    RSDK.DrawRect(entity->position.x - 0x250000, entity->position.y - 0x190000, 0x500000, 0x380000, 0xFFFFFF, 127, INK_BLEND, false);
    UIVsZoneButton_Unknown4();
    UIVsZoneButton_Unknown3();
    UIVsZoneButton_Unknown6();
    if (entity->state != UIVsZoneButton_Unknown15 || !(entity->timer & 2))
        UIVsZoneButton_Unknown5();
}

void UIVsZoneButton_Create(void *data)
{
    RSDK_THIS(UIVsZoneButton);
    entity->active          = ACTIVE_BOUNDS;
    entity->drawOrder       = 2;
    entity->visible         = 1;
    entity->drawFX          = FX_FLIP;
    entity->updateRange.x   = 0x800000;
    entity->updateRange.y   = 0x300000;
    entity->processButtonCB = UIButton_Unknown6;
    entity->touchCB         = UIButton_ProcessTouch;
    entity->options3        = UIVsZoneButton_Options3CB;
    entity->failCB          = UIVsZoneButton_FailCB;
    entity->options5        = UIVsZoneButton_Options5CB;
    entity->options6        = UIVsZoneButton_Options6CB;
    entity->options7        = UIVsZoneButton_Options7CB;
    entity->options8        = UIVsZoneButton_Options8CB;
    entity->state           = UIVsZoneButton_Unknown13;
    UIVsZoneButton_SetupAnimators();
    UIVsZoneButton_SetNameText();
}

void UIVsZoneButton_StageLoad(void)
{
    UIVsZoneButton->aniFrames  = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
    UIVsZoneButton->textFrames = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE);
    UIVsZoneButton->sfxFail    = RSDK.GetSFX("Stage/Fail.wav");
}

void UIVsZoneButton_SetupAnimators(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (RSDK_sceneInfo->inEditor || entity->zoneID == 12 || entity->xOut)
        RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 16, &entity->zoneAnimator, false, 0);
    else
        RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 17, &entity->zoneAnimator, false, entity->zoneID);
    RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 14, &entity->animator1, false, 5);
    RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 14, &entity->animator2, false, 6);

    entity->field_118 = 0x300000;
    entity->field_11C = 0xD0000;
    entity->field_120 = 13;
}

void UIVsZoneButton_SetNameText(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->textAnimator, true, 0);
        if (entity->obfuscate) {
            RSDK.PrependText(&entity->nameText, "???");
            entity->nameText.textLength = 3;
        }
        else {
            Localization_SetZoneNameShort(&entity->nameText, entity->zoneID);
            if (entity->zoneID < 12) {
                char buffer[0x10];
                sprintf(buffer, " %d", entity->act + 1);
                RSDK.AppendText(&entity->nameText, buffer);
            }
        }
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->nameText);
    }
}

void UIVsZoneButton_Unknown3(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (!RSDK_sceneInfo->inEditor)
        UIWidgets_Unknown3(56, 80, RSDK_sceneInfo->entity->position.x + 0x30000, RSDK_sceneInfo->entity->position.y + 0x30000);
    if (entity->flag)
        UIWidgets_Unknown4(56, 80, entity->position.x, entity->position.y);
    else
        UIWidgets_Unknown2(56, 80, entity->position.x, entity->position.y);
}

void UIVsZoneButton_Unknown4(void)
{
    RSDK_THIS(UIVsZoneButton);
    UIWidgets_Unknown5(232, (23 * entity->field_12C) >> 16, 40, 88, entity->position.x - 0x250000, entity->position.y - 0x190000);
    UIWidgets_Unknown5(96, (-29 * entity->field_12C) >> 16, 160, 176, entity->position.x + 0x250000, entity->position.y + 0x180000);
    UIWidgets_Unknown5(88, (-17 * entity->field_12C) >> 16, 112, 224, entity->position.x + 0x250000, entity->position.y + 0x180000);
}

void UIVsZoneButton_Unknown5(void)
{
    RSDK_THIS(UIVsZoneButton);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;

    if (parent && (parent->active == ACTIVE_ALWAYS || parent->dialogHasFocus) && RSDK.CheckOnScreen(parent, &parent->updateRange)
        && !RSDK_sceneInfo->inEditor && entity->zoneID != 12 && !entity->xOut) {
        RSDK.CopyPalette(((entity->zoneID >> 3) + 1), (32 * entity->zoneID), 0, 224, 32);
    }
    RSDK.DrawRect(drawPos.x - 0x180000, drawPos.y - 0x140000, 0x300000, 0x280000, 0, 255, INK_BLEND, false);

    drawPos.x += entity->field_13C;
    drawPos.y += entity->field_13C;
    RSDK.DrawRect(drawPos.x - 0x180000, drawPos.y - 0x140000, 0x300000, 0x280000, 0xFFFFFF, 255, INK_NONE, false);
    if (entity->xOut) {
        int32 dir           = entity->direction;
        entity->direction = entity->zoneDirection;
        RSDK.DrawSprite(&entity->zoneAnimator, &drawPos, false);

        entity->direction = dir;
        drawPos.x += 0x10000;
        drawPos.y += 0x10000;
        entity->inkEffect = INK_BLEND;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->inkEffect = INK_NONE;
        drawPos.x -= 0x10000;
        drawPos.y -= 0x10000;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    }
    else if (entity->zoneID == 12) {
        int32 dir           = entity->direction;
        entity->direction = entity->zoneDirection;
        RSDK.DrawSprite(&entity->zoneAnimator, &drawPos, false);

        entity->direction = dir;
    }
    else {
        RSDK.DrawSprite(&entity->zoneAnimator, &drawPos, false);
    }
}

void UIVsZoneButton_Unknown6(void)
{
    RSDK_THIS(UIVsZoneButton);
    Vector2 drawPos;

    int32 size  = (entity->field_11C + entity->field_118) >> 16;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y + 0x280000;

    UIWidgets_Unknown7(entity->field_11C >> 16, size, entity->field_120, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);
    drawPos.x += entity->field_13C;
    drawPos.y += entity->field_13C;
    UIWidgets_Unknown7(entity->field_11C >> 16, size, entity->field_120, 0, 0, 0, drawPos.x, drawPos.y);

    if (!RSDK_sceneInfo->inEditor) {
        int32 width = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->nameText, 0, entity->nameText.textLength, 0);
        drawPos.y -= 0x10000;
        drawPos.x -= width << 15;
        RSDK.DrawText(&entity->textAnimator, &drawPos, &entity->nameText, 0, entity->nameText.textLength, ALIGN_LEFT, 0, 0, 0, false);
    }
}

bool32 UIVsZoneButton_Options7CB(void)
{
    RSDK_THIS(UIVsZoneButton);
    return entity->state == UIVsZoneButton_Unknown14;
}

bool32 UIVsZoneButton_Options8CB(void)
{
    RSDK_THIS(UIVsZoneButton);
    return entity->state == UIVsZoneButton_Unknown15;
}

void UIVsZoneButton_Options3CB(void)
{
    RSDK_THIS(UIVsZoneButton);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    EntityMenuParam *param  = (EntityMenuParam *)globals->menuParam;

    parent->state = StateMachine_None;
    RSDK.PlaySfx(UIWidgets->sfx_Accept, false, 255);
    RSDK.StopChannel(Music->channelID);
    TimeAttackData_ClearOptions();
    param->vsZoneID = entity->zoneID;
    param->vsActID  = entity->act;
    entity->state   = UIVsZoneButton_Unknown15;
}

void UIVsZoneButton_FailCB(void) { RSDK.PlaySfx(UIVsZoneButton->sfxFail, false, 255); }

void UIVsZoneButton_Options5CB(void)
{
    RSDK_THIS(UIVsZoneButton);

    entity->state = UIVsZoneButton_Unknown14;
    if (!entity->flag) {
        entity->field_13C = 0;
        entity->field_138 = -0x20000;
        entity->field_124 = 0x4000;
        entity->field_130 = 0x8000;
        entity->flag      = true;
    }
}

void UIVsZoneButton_Options6CB(void)
{
    RSDK_THIS(UIVsZoneButton);
    entity->flag  = false;
    entity->state = UIVsZoneButton_Unknown13;
}

void UIVsZoneButton_Unknown13(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (entity->zoneID == 12 || entity->xOut)
        entity->zoneDirection = entity->zoneAnimator.frameID & 3;

    if (entity->field_13C) {
        int32 dist = -(entity->field_13C / abs(entity->field_13C));
        entity->field_13C += dist << 16;
        if (dist < 0) {
            if (entity->field_13C < 0) {
                entity->field_13C = 0;
            }
            else if (dist > 0 && entity->field_13C > 0)
                entity->field_13C = 0;
        }
        else if (dist > 0 && entity->field_13C > 0)
            entity->field_13C = 0;
    }
}

void UIVsZoneButton_Unknown14(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (entity->zoneID == 12 || entity->xOut)
        entity->zoneDirection = entity->zoneAnimator.frameID & 3;

    entity->field_138 += 0x4800;
    entity->field_13C += entity->field_138;
    if (entity->field_13C >= -0x20000 && entity->field_138 > 0) {
        entity->field_13C = -0x20000;
        entity->field_138 = 0;
    }
}

void UIVsZoneButton_Unknown15(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (entity->zoneID == 12 || entity->xOut)
        entity->zoneDirection = entity->zoneAnimator.frameID & 3;

    if (entity->timer >= 30) {
        entity->flag            = false;
        entity->timer           = 0;
        entity->state           = UIVsZoneButton_Unknown13;
        entity->processButtonCB = UIButton_Unknown6;
        StateMachine_Run(entity->options2);
    }
    else {
        entity->timer++;
    }
}

void UIVsZoneButton_EditorDraw(void) {}

void UIVsZoneButton_EditorLoad(void) {}

void UIVsZoneButton_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsZoneButton, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIVsZoneButton, VAR_ENUM, zoneID);
    RSDK_EDITABLE_VAR(UIVsZoneButton, VAR_ENUM, act);
    RSDK_EDITABLE_VAR(UIVsZoneButton, VAR_BOOL, xOut);
    RSDK_EDITABLE_VAR(UIVsZoneButton, VAR_BOOL, obfuscate);
}
