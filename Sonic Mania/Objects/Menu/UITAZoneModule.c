#include "SonicMania.h"

ObjectUITAZoneModule *UITAZoneModule;

void UITAZoneModule_Update(void)
{
    RSDK_THIS(UITAZoneModule);
    UITAZoneModule_Setup();

    entity->active = ACTIVE_NORMAL;
    if (entity->disabled != entity->wasDisabled) {
        UITAZoneModule_SetupText();
        entity->wasDisabled = entity->disabled;
    }
    entity->drawPos = entity->position;

    StateMachine_Run(entity->state);

    entity->field_12C++;
    if (entity->field_12C >= 192)
        entity->field_12C -= 192;

    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    if (entity->state == UITAZoneModule_Unknown17
        && (parent->buttons[parent->field_D8] != (EntityUIButton *)entity || parent->state != UIControl_ProcessInputs)) {
        entity->flag  = false;
        entity->state = UITAZoneModule_Unknown16;
    }
}

void UITAZoneModule_LateUpdate(void) {}

void UITAZoneModule_StaticUpdate(void)
{
    if (UITAZoneModule->flag) {
        UITAZoneModule->flag = false;
    }

    if (UITAZoneModule->isEncoreMode != TimeAttackMenu->encoreMode) {
        UITAZoneModule->flag         = true;
        UITAZoneModule->isEncoreMode = TimeAttackMenu->encoreMode;
    }
}

void UITAZoneModule_Draw(void)
{
    UITAZoneModule_Unknown3();
    UITAZoneModule_Unknown6();
}

void UITAZoneModule_Create(void *data)
{
    RSDK_THIS(UITAZoneModule);
    entity->posUnknown2.x   = entity->position.x;
    entity->posUnknown2.y   = entity->position.y;
    entity->active          = ACTIVE_BOUNDS;
    entity->drawOrder       = 2;
    entity->visible         = true;
    entity->drawFX          = FX_FLIP;
    entity->updateRange.x   = 0x800000;
    entity->updateRange.y   = 0x300000;
    entity->processButtonCB = UIButton_Unknown6;
    entity->state           = UITAZoneModule_Unknown15;
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetText(&entity->text3, "", false);
        RSDK.SetText(&entity->text4, "", false);
        RSDK.CopyString(&entity->text3, &entity->text1);
        RSDK.CopyString(&entity->text4, &entity->text2);
    }
    UITAZoneModule_SetupText();
}

void UITAZoneModule_StageLoad(void)
{
    UITAZoneModule->isEncoreMode = false;
    UITAZoneModule->flag         = false;
    UITAZoneModule->aniFrames    = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
}

void UITAZoneModule_Setup(void)
{
    RSDK_THIS(UITAZoneModule);
    entity->touchPosEnd.x = 0;
    entity->touchPosEnd.y = 0;
    entity->options7      = UITAZoneModule_Options7CB;
    entity->options8      = UITAZoneModule_Options8CB;
    if (entity->processButtonCB == UIButton_Unknown6) {
        entity->touchPosStart.x = 0x1380000;
        entity->touchPosStart.y = 0x4E0000;
        entity->touchCB         = UIButton_ProcessTouch;
        entity->options3        = UITAZoneModule_Options3CB;
        entity->failCB          = UITAZoneModule_FailCB;
        entity->options5        = UITAZoneModule_Options5CB;
        entity->options6        = UITAZoneModule_Options6CB;
    }
    else {
        entity->touchPosStart.x = 0;
        entity->touchPosStart.y = 0;
        entity->touchPosCount   = 0;
        entity->options3        = StateMachine_None;
        entity->failCB          = StateMachine_None;
        entity->options5        = StateMachine_None;
        entity->options6        = StateMachine_None;
    }
}

void UITAZoneModule_SetupText(void)
{
    RSDK_THIS(UITAZoneModule);
    if (!RSDK_sceneInfo->inEditor) {
        if (!entity->text1.text)
            RSDK.SetText(&entity->text1, "", 0);
        if (!entity->text2.text)
            RSDK.SetText(&entity->text2, "", 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 3, &entity->animator5, false, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 3, &entity->animator6, false, 0);
        if (entity->disabled) {
            RSDK.PrependText(&entity->text1, "???");
            RSDK.PrependText(&entity->text2, "");
            entity->text2.textLength = 0;
        }
        else {
            RSDK.CopyString(&entity->text1, &entity->text3);
            RSDK.CopyString(&entity->text2, &entity->text4);
        }
        RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 3u, &entity->text1);
        RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 3u, &entity->text2);
    }
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 10, &entity->animator2, false, 0);
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 11, &entity->animator1, true, entity->zoneID);
}

void UITAZoneModule_Unknown3(void)
{
    RSDK_THIS(UITAZoneModule);

    uint colour = 0x5FA0B0;
    if (entity->isEncore)
        colour = 0xF26C4F;

    int drawY = entity->drawPos.y + 0x230000;
    UIWidgets_Unknown5(88, -71, 112, 224, entity->drawPos.x + 0x790000, drawY);

    RSDK.DrawRect(entity->drawPos.x + 0x790000, drawY - 0x480000, 0x200000, 0x480000, 0x5870E0, 255, INK_NONE, false);
    RSDK.DrawRect(entity->drawPos.x - 0x990000, entity->drawPos.y - 0x1C0000, 0x1320000, 0x2C0000, 0, 255, INK_NONE, false);

    UIWidgets_Unknown5((colour >> 16) & 0xFF, -71, (colour >> 8) & 0xFF, colour & 0xFF, entity->drawPos.x + 0x990000, entity->drawPos.y + 0x230000);

    if (!RSDK_sceneInfo->inEditor)
        UIWidgets_Unknown3(78, 312, entity->position.x + 0x30000, entity->position.y + 0x30000);

    if (entity->flag)
        UIWidgets_Unknown4(78, 312, entity->position.x, entity->position.y);
    else
        UIWidgets_Unknown2(78, 312, entity->position.x, entity->position.y);
}

void UITAZoneModule_Unknown4(void)
{
    RSDK_THIS(UITAZoneModule);
    Vector2 drawPos;
    drawPos     = entity->drawPos;
    uint colour = 0xF0F0F0;
    if (entity->isEncore)
        colour = 0xF26C4F;
    uint colour2 = 0xF0D808;

    drawPos.x = entity->drawPos.x - 0x390000;
    drawPos.y = entity->drawPos.y + 0x170000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x840000, 0xD0000, colour, 255, INK_NONE, false);

    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &entity->animator3, true, (entity->isEncore != 0) + 10);
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    drawPos = UIWidgets_Unknown10(colour, colour2, drawPos.x + 0x840000, drawPos.y);
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x400000, 0xD0000, colour2, 255, INK_NONE, false);
}

void UITAZoneModule_Unknown5(void)
{
    RSDK_THIS(UITAZoneModule);
    Vector2 drawPos;

    drawPos.x = entity->drawPos.x - 0x690000;
    drawPos.y = entity->drawPos.y;
    UIWidgets_Unknown2(72, 96, drawPos.x, drawPos.y);
    if (RSDK_sceneInfo->inEditor || !entity->flag || entity->disabled) {
        entity->direction = entity->field_138;
        entity->drawFX    = FX_FLIP;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->direction = FLIP_NONE;
        entity->drawFX    = 0;
    }
    else {
        SpriteFrame *frame = RSDK.GetFrame(UITAZoneModule->aniFrames, 11, entity->zoneID);
        frame->pivotX      = -45;
        frame->width       = 90;
        frame->sprX        = entity->field_12C;

        if (entity->field_12C <= 102) {
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        }
        else {
            int width    = entity->field_12C - 102;
            frame->width = 90 - width;
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);

            frame->pivotX += frame->width;
            frame->sprX  = 0;
            frame->width = width;
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        }
    }
}

void UITAZoneModule_Unknown6(void)
{
    RSDK_THIS(UITAZoneModule);
    Vector2 drawPos;

    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y;
    UITAZoneModule_Unknown4();
    UITAZoneModule_Unknown5();

    drawPos.x = entity->drawPos.x;
    drawPos.y = entity->drawPos.y;
    if (entity->state != UITAZoneModule_Unknown18 || !(entity->timer & 2)) {
        if (!RSDK_sceneInfo->inEditor) {
            drawPos.x = entity->drawPos.x - 0x370000;
            if (entity->text2.textLength) {
                drawPos.y = entity->drawPos.y - 0x160000;
                RSDK.DrawText(&entity->animator5, &drawPos, &entity->text1, 0, entity->text1.textLength, ALIGN_LEFT, 0, 2, 0, false);

                drawPos.y += 0x1C0000;
                drawPos.x += 0x200000;
                RSDK.DrawText(&entity->animator6, &drawPos, &entity->text2, 0, entity->text2.textLength, ALIGN_LEFT, 0, 2, 0, false);
            }
            else {
                drawPos.y = entity->drawPos.y - 0x60000;
                RSDK.DrawText(&entity->animator5, &drawPos, &entity->text1, 0, entity->text1.textLength, ALIGN_LEFT, 0, 2, 0, false);
            }
        }
    }
}

void UITAZoneModule_FailCB(void) { RSDK.PlaySFX(UIWidgets->sfx_Fail, false, 255); }

void UITAZoneModule_Options3CB(void)
{
    RSDK_THIS(UITAZoneModule);
    entity->timer = 0;
    entity->state = UITAZoneModule_Unknown18;
    RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 255);
    if (TimeAttackMenu->encoreMode)
        RSDK.CopyPalette(((entity->zoneID % 12) >> 3) + 4, (32 * (entity->zoneID % 12)), 0, 224, 32);
    else
        RSDK.CopyPalette(((entity->zoneID % 12) >> 3) + 1, (32 * (entity->zoneID % 12)), 0, 224, 32);
}

bool32 UITAZoneModule_Options7CB(void)
{
    RSDK_THIS(UITAZoneModule);
    return entity->state == UITAZoneModule_Unknown17;
}

bool32 UITAZoneModule_Options8CB(void)
{
    RSDK_THIS(UITAZoneModule);
    return entity->state == UITAZoneModule_Unknown18;
}

void UITAZoneModule_Options5CB(void)
{
    RSDK_THIS(UITAZoneModule);
    entity->flag  = true;
    entity->state = UITAZoneModule_Unknown17;
    if (TimeAttackMenu->encoreMode)
        RSDK.CopyPalette(((entity->zoneID % 12) >> 3) + 4, (32 * (entity->zoneID % 12)), 0, 224, 32);
    else
        RSDK.CopyPalette(((entity->zoneID % 12) >> 3) + 1, (32 * (entity->zoneID % 12)), 0, 224, 32);
}

void UITAZoneModule_Options6CB(void)
{
    RSDK_THIS(UITAZoneModule);
    entity->flag  = false;
    entity->state = UITAZoneModule_Unknown16;
}

void UITAZoneModule_TransitionCB(void) { UIControl_MatchMenuTag("Leaderboards"); }

void UITAZoneModule_Unknown14(void)
{
    UITransition_StartTransition(UITAZoneModule_TransitionCB, 0);
}

void UITAZoneModule_Unknown15(void)
{
    RSDK_THIS(UITAZoneModule);

    entity->state      = UITAZoneModule_Unknown16;
    entity->position.x = entity->posUnknown2.x;
    RSDK.ProcessAnimation(&entity->animator2);
    entity->touchCB   = UIButton_ProcessTouch;
    entity->field_138 = entity->animator2.frameID & 3;
}

void UITAZoneModule_Unknown16(void)
{
    RSDK_THIS(UITAZoneModule);
    entity->position.x = entity->posUnknown2.x;
    RSDK.ProcessAnimation(&entity->animator2);
    entity->touchCB   = UIButton_ProcessTouch;
    entity->field_138 = entity->animator2.frameID & 3;
}

void UITAZoneModule_Unknown17(void)
{
    RSDK_THIS(UITAZoneModule);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->touchCB   = UIButton_ProcessTouch;
    entity->field_138 = entity->animator2.frameID & 3;
    if (UITAZoneModule->flag) {
        if (TimeAttackMenu->encoreMode)
            RSDK.CopyPalette(((entity->zoneID % 12) >> 3) + 4, (32 * (entity->zoneID % 12)), 0, 224, 32);
        else
            RSDK.CopyPalette(((entity->zoneID % 12) >> 3) + 1, (32 * (entity->zoneID % 12)), 0, 224, 32);
    }
}

void UITAZoneModule_Unknown18(void)
{
    RSDK_THIS(UITAZoneModule);

    entity->flag = true;
    if (entity->timer >= 30) {
        entity->timer = 0;
        StateMachine_Run(entity->options2);
        entity->state = UITAZoneModule_Unknown17;
    }
    else {
        entity->timer++;
    }
}

void UITAZoneModule_EditorDraw(void) {}

void UITAZoneModule_EditorLoad(void) {}

void UITAZoneModule_Serialize(void)
{
    RSDK_EDITABLE_VAR(UITAZoneModule, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UITAZoneModule, VAR_UINT8, zoneID);
    RSDK_EDITABLE_VAR(UITAZoneModule, VAR_STRING, text1);
    RSDK_EDITABLE_VAR(UITAZoneModule, VAR_STRING, text2);
#if !RETRO_USE_PLUS
    RSDK_EDITABLE_VAR(UITAZoneModule, VAR_BOOL, debugExpand);
#endif
}
