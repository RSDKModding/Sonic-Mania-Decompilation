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

#if !RETRO_USE_PLUS
    if (entity->id != entity->field_138_2)
        entity->field_138_2 = entity->id;
    if (entity->value) {
        entity->field_140++;
        entity->position.y = entity->posUnknown2.y + (entity->id >> 2);
    }

    if (entity->announceTimer) {
        --entity->announceTimer;
        if (entity->announceTimer == 121) {
            RSDK.PlaySFX(UIWidgets->sfx_Event, false, 0xFF);
        }
        if (entity->announceTimer == 30) {
            if (entity->field_138 == 1) {
                RSDK.PlaySFX(Announcer->sfx_NewRecordTop, false, 0xFF);
            }
            else if (entity->field_138 <= 3) {
                RSDK.PlaySFX(Announcer->sfx_NewRecordMid, false, 0xFF);
            }
        }
    }

    entity->drawPos.y = entity->position.y - (entity->field_138_2 >> 1);
#endif

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
#if RETRO_USE_PLUS
    if (UITAZoneModule->flag) {
        UITAZoneModule->flag = false;
    }

    if (UITAZoneModule->isEncoreMode != TimeAttackMenu->encoreMode) {
        UITAZoneModule->flag         = true;
        UITAZoneModule->isEncoreMode = TimeAttackMenu->encoreMode;
    }
#else
    Entity *entity = UITAZoneModule->entityPtr;
    if (entity) {
        entity->active  = ACTIVE_ALWAYS;
        entity->visible = UITAZoneModule->flag;
    }
#endif
}

void UITAZoneModule_Draw(void)
{
    UITAZoneModule_Unknown3();
    UITAZoneModule_Unknown6();
#if !RETRO_USE_PLUS
    RSDK_THIS(UITAZoneModule);
    
    if (entity->field_138_2 >= 1) {
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, ((entity->drawPos.x - 0x990000) >> 16) - RSDK_screens->position.x,
                           ((entity->drawPos.y + 0x240000) >> 16) - 16 - RSDK_screens->position.y,
                           ((entity->drawPos.x - 0x990000) >> 16) + 306 - RSDK_screens->position.x,
                           ((entity->drawPos.y + 0x240000 + entity->field_138_2) >> 16) - RSDK_screens->position.y);
        UITAZoneModule_Unknown7();
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, RSDK_screens->width, RSDK_screens->height);
    }
#endif
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
#if RETRO_USE_PLUS
    UITAZoneModule->isEncoreMode = false;
    UITAZoneModule->flag         = false;
#endif
    UITAZoneModule->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
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
#if !RETRO_USE_PLUS
    else if (entity->processButtonCB == UITAZoneModule_Unknown24) {
        entity->touchPos1[0].x       = 0x9C0000;
        entity->touchPos1[0].y       = 0x2C0000;
        entity->touchPos1[1].x       = 0x9C0000;
        entity->touchPos1[1].y       = 0x2C0000;
        entity->touchPos2[0].x       = -0x4E0000;
        entity->touchPos2[1].x       = 0x4E0000;
        entity->touchPosCallbacks[0] = UITAZoneModule_Unknown22;
        entity->touchPosCallbacks[1] = UITAZoneModule_Unknown23;
        entity->touchPosCount        = 2;
        entity->options5             = StateMachine_None;
        entity->options6             = StateMachine_None;
        entity->touchCB              = UIButton_TouchCB_Alt;
        entity->options3             = StateMachine_None;
        entity->failCB               = StateMachine_None;
    }
#endif
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
        RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 3, &entity->text1);
        RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 3, &entity->text2);
    }
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 10, &entity->animator2, false, 0);
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 11, &entity->animator1, true, entity->zoneID);
}

void UITAZoneModule_Unknown3(void)
{
    RSDK_THIS(UITAZoneModule);

    uint colour = 0x5FA0B0;
#if RETRO_USE_PLUS
    if (entity->isEncore)
        colour = 0xF26C4F;
#endif

    int drawY = entity->drawPos.y + 0x230000;
    UIWidgets_Unknown5(88, -71, 112, 224, entity->drawPos.x + 0x790000, drawY);

    RSDK.DrawRect(entity->drawPos.x + 0x790000, drawY - 0x480000, 0x200000, 0x480000, 0x5870E0, 255, INK_NONE, false);
    RSDK.DrawRect(entity->drawPos.x - 0x990000, entity->drawPos.y - 0x1C0000, 0x1320000, 0x2C0000, 0, 255, INK_NONE, false);

    UIWidgets_Unknown5((colour >> 16) & 0xFF, -71, (colour >> 8) & 0xFF, colour & 0xFF, entity->drawPos.x + 0x990000, entity->drawPos.y + 0x230000);

#if RETRO_USE_PLUS
    if (!RSDK_sceneInfo->inEditor)
        UIWidgets_Unknown3(78, 312, entity->position.x + 0x30000, entity->position.y + 0x30000);

    if (entity->flag)
        UIWidgets_Unknown4(78, 312, entity->position.x, entity->position.y);
    else
        UIWidgets_Unknown2(78, 312, entity->position.x, entity->position.y);
#else
    if (!RSDK_sceneInfo->inEditor)
        UIWidgets_Unknown3(78 + (entity->field_138_2 >> 16), 312, entity->position.x + 0x30000, entity->position.y + 0x30000);

    if (entity->flag || entity->value)
        UIWidgets_Unknown4(78 + (entity->field_138_2 >> 16), 312, entity->position.x, entity->position.y);
    else
        UIWidgets_Unknown2(78 + (entity->field_138_2 >> 16), 312, entity->position.x, entity->position.y);
#endif
}

void UITAZoneModule_Unknown4(void)
{
    RSDK_THIS(UITAZoneModule);
    Vector2 drawPos;
    drawPos     = entity->drawPos;
    uint colour = 0xF0F0F0;
#if RETRO_USE_PLUS
    if (entity->isEncore)
        colour = 0xF26C4F;
#endif
    drawPos.x -= 0x390000;
    drawPos.y += 0x170000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x840000, 0xD0000, colour, 255, INK_NONE, false);

#if RETRO_USE_PLUS
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &entity->animator3, true, (entity->isEncore != 0) + 10);
#else
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &entity->animator3, true, 10);
#endif
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

#if RETRO_USE_PLUS
    drawPos = UIWidgets_Unknown10(colour, 0xF0D808, drawPos.x + 0x840000, drawPos.y);
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x400000, 0xD0000, 0xF0D808, 255, INK_NONE, false);
#else
    drawPos.x += 0x5C0000;
    UIWidgets_Unknown5(0xF0, 12, 0xF0, 0xF0, drawPos.x, drawPos.y);

    drawPos.x += 0xE0000;
    drawPos.y += 0xC0000;
    UIWidgets_Unknown5(0xE8, -12, 0x28, 0x58, drawPos.x, drawPos.y);

    drawPos.y -= 0xC0000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x680000, 0xD0000, 0xE82858, 0xFF, INK_NONE, false);

    drawPos.x = entity->drawPos.x + 0x430000;
    drawPos.y = (entity->drawPos.y + 0xD0000) + 0x100000;
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &entity->animator3, true, 9);
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    drawPos.x += 0x100000;
    drawPos.y -= 0x20000;
    if (RSDK_sceneInfo->inEditor || !entity->characterID) {
        UITAZoneModule_DrawTime(&drawPos, 0, 0, 0);
    }
    else {
        ushort *records1 = TimeAttackData_GetRecordedTime(entity->zoneID, 0, entity->characterID, 1);
        ushort *records2 = TimeAttackData_GetRecordedTime(entity->zoneID, 1, entity->characterID, 1);

        int time = records1[0] + records2[0];

        if (records1[0] && records2[0])
            UITAZoneModule_DrawTime(&drawPos, time / 6000, time % 6000 / 100, time % 100);
        else if (entity->characterID != 3 && records1[0] && entity->zoneID == 7)
            UITAZoneModule_DrawTime(&drawPos, time / 6000, time % 6000 / 100, time % 100);
        else
            UITAZoneModule_DrawTime(&drawPos, 0, 0, 0);
    }
#endif
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
        entity->drawFX    = FX_NONE;
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

    drawPos = entity->position;
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
#if !RETRO_USE_PLUS
    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    parent->childHasFocus   = true;
    foreach_all(UITAZoneModule, module)
    {
        if (module != entity)
            module->state = UITAZoneModule_Unknown29;
    }
#endif
    RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 255);
#if RETRO_USE_PLUS
    if (TimeAttackMenu->encoreMode)
        RSDK.CopyPalette(((entity->zoneID % 12) >> 3) + 4, (32 * (entity->zoneID % 12)), 0, 224, 32);
    else
#endif
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
#if !RETRO_USE_PLUS
    entity->rank = 0;
#endif
#if RETRO_USE_PLUS
    if (TimeAttackMenu->encoreMode)
        RSDK.CopyPalette(((entity->zoneID % 12) >> 3) + 4, (32 * (entity->zoneID % 12)), 0, 224, 32);
    else
#endif
        RSDK.CopyPalette(((entity->zoneID % 12) >> 3) + 1, (32 * (entity->zoneID % 12)), 0, 224, 32);
}

void UITAZoneModule_Options6CB(void)
{
    RSDK_THIS(UITAZoneModule);
    entity->flag  = false;
    entity->state = UITAZoneModule_Unknown16;
}

void UITAZoneModule_TransitionCB(void) { UIControl_MatchMenuTag("Leaderboards"); }

void UITAZoneModule_Unknown14(void) { UITransition_StartTransition(UITAZoneModule_TransitionCB, 0); }

void UITAZoneModule_Unknown15(void)
{
    RSDK_THIS(UITAZoneModule);
#if !RETRO_USE_PLUS
    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    Hitbox hitbox;
    foreach_active(UIButtonPrompt, prompt)
    {
        int posX      = parent->startPos.x - parent->cameraOffset.x;
        int posY      = parent->startPos.y - parent->cameraOffset.y;
        hitbox.right  = parent->size.x >> 17;
        hitbox.left   = -(parent->size.x >> 17);
        hitbox.bottom = parent->size.y >> 17;
        hitbox.top    = -(parent->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, prompt->position.x, prompt->position.y) && prompt->buttonID == 3)
            UITAZoneModule->entityPtr = (Entity *)prompt;
    }
#endif

    entity->state = UITAZoneModule_Unknown16;
    UITAZoneModule_Unknown16();
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
    entity->field_138 = entity->animator2.frameID & 3;
#if RETRO_USE_PLUS
    if (UITAZoneModule->flag) {
        if (TimeAttackMenu->encoreMode)
            RSDK.CopyPalette(((entity->zoneID % 12) >> 3) + 4, (32 * (entity->zoneID % 12)), 0, 224, 32);
        else
            RSDK.CopyPalette(((entity->zoneID % 12) >> 3) + 1, (32 * (entity->zoneID % 12)), 0, 224, 32);
    }
#endif
}

void UITAZoneModule_Unknown18(void)
{
    RSDK_THIS(UITAZoneModule);

    entity->flag = true;
#if !RETRO_USE_PLUS
    entity->touchCB = StateMachine_None;
#endif
    if (entity->timer >= 30) {
#if !RETRO_USE_PLUS
        entity->value        = 1;
        entity->field_140    = 0;
        entity->timer        = 0;
        entity->rank         = 0;
        entity->actID        = entity->zoneID == 7 && entity->characterID != 3;
        UITAZoneModule->flag = true;
        entity->state        = UITAZoneModule_Unknown27;
#else
        entity->timer = 0;
        StateMachine_Run(entity->options2);
        entity->state = UITAZoneModule_Unknown17;
#endif
    }
    else {
        entity->timer++;
    }
}

#if !RETRO_USE_PLUS
void UITAZoneModule_DrawTime(Vector2 *drawPos, int minutes, int seconds, int milliseconds)
{
    RSDK_THIS(UITAZoneModule);
    char strBuf[16];

    if (minutes) {
        if (minutes != 99 || seconds != 99 || milliseconds != 99) {
            sprintf(strBuf, "%02d:%02d;%02d", minutes, seconds, milliseconds);
        }
        else {
            sprintf(strBuf, "<<:<<;<<");
        }
    }
    else if (!seconds && !milliseconds) {
        sprintf(strBuf, "<<:<<;<<");
    }
    else {
        sprintf(strBuf, "%02d:%02d;%02d", minutes, seconds, milliseconds);
    }

    for (int i = 0; i < 8; ++i) {
        if (!strBuf[i])
            break;
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 8, &entity->animator3, true, (byte)(strBuf[i] - '0'));
        RSDK.DrawSprite(&entity->animator3, drawPos, 0);
        drawPos->x += 0x80000;
    }
}

void UITAZoneModule_Unknown8(void)
{
    RSDK_THIS(UITAZoneModule);
    Vector2 drawPos = entity->drawPos;

    uint colour1 = 0;
    if (entity->actID)
        colour1 = 0xE82858;

    uint colour2 = 0;
    if (entity->actID != 1)
        colour2 = 0xE82858;

    RSDK.DrawRect(drawPos.x - 0x990000, drawPos.y + 0x240000, 0x860000, 0x150000, colour1, 255, INK_NONE, false);

    int offset = 0;
    if (RSDK.Sin256(8 * minVal((byte)entity->field_140, 16)) >= 0)
        offset = RSDK.Sin256(8 * minVal((byte)entity->field_140, 16));
    else
        offset = -RSDK.Sin256(8 * minVal((byte)entity->field_140, 16));
    offset >>= 6;

    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &entity->animator7, true, 0);
    drawPos.x -= 0x630000;
    drawPos.y += 0x2E0000;

    bool32 flag = true;
    if (!entity->actID) {
        drawPos.x -= (offset << 16);
        drawPos.y -= (offset << 16);
        if (entity->state == UITAZoneModule_Unknown21 && (entity->timer & 2)) {
            drawPos.x += offset << 16;
            drawPos.y += offset << 16;
            flag = false;
        }
    }
    if (flag) {
        if (entity->zoneID != 7 || entity->characterID == 3) {
            RSDK.DrawSprite(&entity->animator7, &drawPos, false);
            if (!entity->actID) {
                drawPos.x += offset << 16;
                drawPos.y += offset << 16;
            }
        }
    }

    Vector2 drawPos2, drawPos3;
    drawPos2.x = entity->drawPos.x - 0x1A0000;
    drawPos2.y = entity->drawPos.y + 0x2E0000;

    drawPos.x = entity->drawPos.x - 0x130000;
    drawPos.y = entity->drawPos.y + 0x240000;
    UIWidgets_Unknown5((colour1 >> 16) & 0xFF, 20, (colour1 >> 8) & 0xFF, colour1 & 0xFF, drawPos.x, drawPos.y);

    drawPos.x += 0x140000;
    drawPos.y += 0x140000;
    UIWidgets_Unknown5((colour2 >> 16) & 0xFF, -20, (colour2 >> 8) & 0xFF, colour2 & 0xFF, drawPos.x, drawPos.y);

    drawPos3.x = drawPos.x + 0x40000;
    drawPos3.y = drawPos.y - 0xA0000;
    drawPos.y -= 0x140000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x980000, 0x150000, colour2, 255, INK_NONE, false);

    drawPos.x += 0x4B0000;
    drawPos.y += 0xA0000;

    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &entity->animator7, true, 1);
    
    flag = true;
    if (entity->actID == 1) {
        drawPos.x -= (offset << 16);
        drawPos.y -= (offset << 16);
        if (entity->state == UITAZoneModule_Unknown21 && (entity->timer & 2)) {
            drawPos.x += offset << 16;
            drawPos.y += offset << 16;
            flag = false;
        }
    }
    if (flag) {
        RSDK.DrawSprite(&entity->animator7, &drawPos, false);
        if (entity->actID == 1) {
            drawPos.x += offset << 16;
            drawPos.y += offset << 16;
        }
    }

    entity->drawFX    = FX_FLIP;
    entity->direction = FLIP_X;
    if (entity->actID == 1)
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &entity->animator7, true, 3);
    else
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &entity->animator7, true, 2);
    if (!entity->actID) {
        if (RSDK.Sin256(2 * (byte)entity->field_140) >= 0)
            drawPos2.x -= RSDK.Sin256(2 * (byte)entity->field_140) << 10;
        else
            drawPos2.x -= -RSDK.Sin256(2 * (byte)entity->field_140) << 10;
    }

    if (entity->zoneID != 7 || entity->characterID == 3)
        RSDK.DrawSprite(&entity->animator7, &drawPos2, false);

    entity->direction = FLIP_NONE;
    entity->drawFX    = FX_NONE;
    if (entity->actID == 1)
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &entity->animator7, true, 2);
    else
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &entity->animator7, true, 3);
    if (entity->actID == 1) {
        if (RSDK.Sin256(2 * (byte)entity->field_140) >= 0)
            drawPos3.x -= RSDK.Sin256(2 * (byte)entity->field_140) << 10;
        else
            drawPos3.x -= -RSDK.Sin256(2 * (byte)entity->field_140) << 10;
    }

    RSDK.DrawSprite(&entity->animator7, &drawPos3, false);
}

void UITAZoneModule_Unknown7(void)
{
    RSDK_THIS(UITAZoneModule);
    int drawX = entity->drawPos.x;
    int drawY = entity->drawPos.y;
    Vector2 drawPos2 = entity->drawPos;

    ushort *records = TimeAttackData_GetRecordedTime(entity->zoneID, entity->actID, entity->characterID, 1);

    RSDK.DrawRect(drawX - 0x990000, drawY + 0x240000, 0x1320000, 0x4D0000, 0x000000, 255, INK_NONE, false);
    UITAZoneModule_Unknown8();
    UIWidgets_Unknown7(0x17, 0xE0, 0x17, 0xD9, 0xAD, 0x4, entity->position.x - 0x40000, drawY + 0x448000);

    drawPos2.x = drawX - 0x390000;
    drawPos2.y = drawY + 0x450000;
    if (entity->rank != 1 || (UIControl->timer & 4)) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &entity->animator7, true, 2);
        RSDK.DrawSprite(&entity->animator7, &drawPos2, false);

        int time = 0;
        if (records)
            time = records[0];

        Vector2 pos;
        pos.x = drawX - 0x10000;
        pos.y = drawY + 0x420000;
        UITAZoneModule_DrawTime(&pos, time / 6000, time % 6000 / 100, time % 100);
    }
    drawY += 0x140000;

    UIWidgets_Unknown7(0x0F, 0xE0, 0x0F, 0x98, 0xC0, 0xC8, entity->position.x + 0x80000, drawY + 0x448000);
    drawPos2.x = drawX - 0x290000;
    drawPos2.y = drawY + 0x460000;
    if (entity->rank != 2 || (UIControl->timer & 4)) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &entity->animator7, true, 3);
        RSDK.DrawSprite(&entity->animator7, &drawPos2, 0);

        int time = 0;
        if (records)
            time = records[1];

        Vector2 pos;
        pos.x = drawX + 0x140000;
        pos.y = drawY + 0x420000;
        UITAZoneModule_DrawTime(&pos, time / 6000, time % 6000 / 100, time % 100);
    }

    UIWidgets_Unknown7(0xF, 0xE0, 0xF, 0xC0, 0x58, 0x01, entity->position.x + 0x140000, drawY + 0x548000);
    drawPos2.x = drawX - 0x1A0000;
    drawPos2.y = drawY + 0x560000;
    if (entity->rank != 3 || (UIControl->timer & 4)) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &entity->animator7, true, 4);
        RSDK.DrawSprite(&entity->animator7, &drawPos2, false);

        int time = 0;
        if (records)
            time = records[2];

        Vector2 pos;
        pos.x = drawX + 0x230000;
        pos.y = drawY + 0x520000;
        UITAZoneModule_DrawTime(&pos, time / 6000, time % 6000 / 100, time % 100);
    }
}

void UITAZoneModule_Unknown19(void) {}

void UITAZoneModule_Unknown21(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    entity->flag = true;
    if (entity->timer >= 30) {
        TimeAttackData_ClearOptions();
        param->zoneID = entity->zoneID;
        param->actID  = entity->actID;
        entity->timer = 0;
        StateMachine_Run(entity->options2);
        entity->state           = UITAZoneModule_Unknown19;
        entity->processButtonCB = StateMachine_None;
    }
    else {
        entity->timer++;
    }
}

void UITAZoneModule_Unknown22(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    if (entity->zoneID != 7 || entity->characterID == 3) {
        entity->actID = 0;

        int id = API_MostRecentActiveControllerID(0);
        API_ResetControllerAssignments();
        API_AssignControllerID(1, id);

        parent->state = StateMachine_None;
        entity->timer = 0;
        entity->state = UITAZoneModule_Unknown21;
        RSDK.StopChannel(Music->channelID);
        RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 0xFF);
    }
}

void UITAZoneModule_Unknown23(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    int id = API_MostRecentActiveControllerID(0);
    API_ResetControllerAssignments();
    API_AssignControllerID(1, id);

    parent->state = StateMachine_None;
    entity->timer = 0;
    entity->state = UITAZoneModule_Unknown21;
    RSDK.StopChannel(Music->channelID);
    RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 0xFF);
}

void UITAZoneModule_Unknown24(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    if (UIControl->keyLeft && entity->actID == 1 && (entity->zoneID != 7 || entity->characterID == 3)) {
        entity->actID = 0;
        entity->rank  = 0;
        RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 0xFF);
        entity->field_140 = 0;
    }
    if (UIControl->keyRight && !entity->actID && (entity->zoneID != 7 || entity->characterID == 3)) {
        entity->actID     = 1;
        entity->rank  = 0;
        RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 0xFF);
        entity->field_140 = 0;
    }
    else if (UIControl->keyConfirm) {
        int id = API_MostRecentActiveControllerID(0);
        API_ResetControllerAssignments();
        API_AssignControllerID(1, id);

        parent->state = StateMachine_None;
        entity->timer = 0;
        entity->state = UITAZoneModule_Unknown21;
        RSDK.StopChannel(Music->channelID);
        RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 0xFF);
    }
    else {
        if (MenuSetup && UIControl->keyY) {
            UITAZoneModule_Unknown25(entity->characterID, entity->zoneID, entity->actID, 0, UITAZoneModule_Unknown14);
            RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 0xFF);
        }
        if (UIControl->keyBack) {
            entity->timer           = 0;
            entity->state           = UITAZoneModule_Unknown28;
            entity->processButtonCB = StateMachine_None;
        }
    }
}

void UITAZoneModule_Unknown25(int player, int zone, int act, int a4, void (*callback)(void))
{
    TextInfo info;
    INIT_TEXTINFO(info);

    Localization_GetString(&info, STR_CONNECTING);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        UIDialog_Setup(dialog);
        MenuSetup->dialog = (Entity *)dialog;
        MenuSetup_StartTransitionLB(callback, 120);

        ushort *records                  = TimeAttackData_GetRecordedTime(zone, act, player, 1);
        EntityUILeaderboard *leaderboard = (EntityUILeaderboard *)MenuSetup->leaderboardWidget;

        bool32 isUser = false;
        if (!a4)
            isUser = *records;
        leaderboard->taRecord    = *records;
        leaderboard->entryIsUser = isUser;
        APICallback_FetchLeaderboardData(zone, act, player, 0, 100, isUser);
        UILeaderboard_SetupLeaderboard(leaderboard, player, zone, act);
    }
}

void UITAZoneModule_Unknown26(EntityUIControl *control, char characterID, unsigned int zoneID, char actID, int score)
{
    control->childHasFocus       = true;
    control->activeEntityID      = zoneID;
    EntityUITAZoneModule *module = (EntityUITAZoneModule *)control->buttons[zoneID];

    control->position.x = module->posUnknown2.x;
    control->position.y = module->posUnknown2.y;
    control->posUnknown = module->posUnknown2;

    RSDK_screens->position.x = (module->posUnknown2.x >> 16) - RSDK_screens->centerX;
    RSDK_screens->position.y = (control->position.y >> 16) - RSDK_screens->centerY;
    RSDK.CopyPalette((zoneID >> 3) + 1, 32 * zoneID, 0LL, 0xE0, 0x20);
    module->actID        = actID;
    module->state        = UITAZoneModule_Unknown27;
    module->timer        = 16;
    module->value        = 1;
    module->flag         = true;
    UITAZoneModule->flag = true;
    if (score >= 1) {
        module->announceTimer = 150;
        module->rank          = score;
    }

    for (int i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)control->buttons[i];
        if (button != module) {
            button->state      = UITAZoneModule_Unknown30;
            button->value      = 0;
            button->flag       = false;
            button->position.x = button->posUnknown2.x + (RSDK_screens->width << 16);
        }
    }

    EntityUIControl *zoneControl = (EntityUIControl *)MenuSetup->timeAttackZones;
    for (int i = 0; i < zoneControl->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)zoneControl->buttons[i];
        button->characterID          = characterID;
    }
}

void UITAZoneModule_Unknown27(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityUIControl *parent         = (EntityUIControl *)entity->parent;
    EntityUIHeading *heading        = (EntityUIHeading *)parent->heading;
    EntityUIButtonPrompt *promptPtr = (EntityUIButtonPrompt *)UITAZoneModule->entityPtr;

    if (entity->timer > 15) {
        entity->id = 0x4D0000;

        foreach_all(UIButtonPrompt, prompt)
        {
            Hitbox hitbox;
            foreach_all(UIButtonPrompt, prompt)
            {
                int posX      = parent->startPos.x - parent->cameraOffset.x;
                int posY      = parent->startPos.y - parent->cameraOffset.y;
                hitbox.left   = -(parent->size.x) >> 17;
                hitbox.top    = -(parent->size.x) >> 17;
                hitbox.right  = (parent->size.x) >> 17;
                hitbox.bottom = (parent->size.y) >> 17;

                if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, prompt->position.x, prompt->position.y)) {
                    if (prompt->startPos.y >> 16 < RSDK_screens->position.y) {
                        prompt->position.y = (RSDK_screens->position.y << 16) + 0x180000;
                        if (prompt == promptPtr)
                            promptPtr->position.y += 0x180000;
                    }
                }
            }
        }

        heading->position.y     = (RSDK_screens->position.y << 16) + 0x1C0000;
        entity->timer           = 0;
        entity->state           = UITAZoneModule_Unknown19;
        entity->processButtonCB = UITAZoneModule_Unknown24;
        entity->touchCB         = UIButton_TouchCB_Alt;
    }
    else {
        if (heading->startPos.y >> 16 < RSDK_screens->position.y) {
            int pos = (RSDK_screens->position.y << 16) - 0x1C0000;
            if (entity->timer >= 1) {
                int pos = (RSDK_screens->position.y << 16) + 0x1C0000;
                if (16 * entity->timer <= 255)
                    pos += 0x3800 * (RSDK.Sin1024(16 * entity->timer) >> 2);
                else
                    pos = (RSDK_screens->position.y << 16) + 0x1C0000;
            }
            heading->position.y = pos;
        }

        Hitbox hitbox;
        foreach_all(UIButtonPrompt, prompt)
        {
            int posX      = parent->startPos.x - parent->cameraOffset.x;
            int posY      = parent->startPos.y - parent->cameraOffset.y;
            hitbox.left   = -(parent->size.x) >> 17;
            hitbox.top    = -(parent->size.x) >> 17;
            hitbox.right  = (parent->size.x) >> 17;
            hitbox.bottom = (parent->size.y) >> 17;

            if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, prompt->position.x, prompt->position.y)) {
                if (!prompt->buttonID)
                    prompt->promptID = 4;
                if (prompt->startPos.y >> 16 < RSDK_screens->position.y) {
                    int offset1, offset2;
                    if (prompt == promptPtr)
                        offset1 = 0x300000;
                    else
                        offset1 = 0x180000;
                    if (prompt == promptPtr)
                        offset2 = -0x180000;
                    else
                        offset2 = -0x300000;

                    int pos = (RSDK_screens->position.y << 16) + offset2;
                    if (entity->timer >= 1) {
                        if (16 * entity->timer <= 255)
                            pos += (RSDK.Sin1024(16 * entity->timer) >> 2) * ((((RSDK_screens->position.y) << 16) + offset1 - pos) >> 8);
                        else
                            pos = (RSDK_screens->position.y) << 16 + offset1;
                    }
                    prompt->position.y = pos;
                }
            }
        }

        if (entity->timer < 1) {
            entity->id = 0;
        }
        else {
            if ((16 * entity->timer) < 0x100) {
                entity->id = 0x4D00 * (RSDK.Sin1024(16 * entity->timer) >> 2);
            }
            else {
                entity->id = 0x4D0000;
            }
        }
        entity->timer++;
    }
}

void UITAZoneModule_Unknown28(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityUIControl *parent         = (EntityUIControl *)entity->parent;
    EntityUIHeading *heading        = (EntityUIHeading *)parent->heading;
    EntityUIButtonPrompt *promptPtr = (EntityUIButtonPrompt *)UITAZoneModule->entityPtr;

    entity->processButtonCB = StateMachine_None;
    if (entity->timer > 15) {
        if (entity->timer == 16) {
            foreach_all(UITAZoneModule, module)
            {
                if (module != entity)
                    module->state = UITAZoneModule_Unknown31;
            }

            Hitbox hitbox;
            foreach_all(UIButtonPrompt, prompt)
            {
                int posX      = parent->startPos.x - parent->cameraOffset.x;
                int posY      = parent->startPos.y - parent->cameraOffset.y;
                hitbox.left   = -(parent->size.x) >> 17;
                hitbox.top    = -(parent->size.x) >> 17;
                hitbox.right  = (parent->size.x) >> 17;
                hitbox.bottom = (parent->size.y) >> 17;

                if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, prompt->position.x, prompt->position.y)) {
                    if (!prompt->buttonID)
                        prompt->promptID = 0;
                    if (prompt->startPos.y >> 16 < RSDK_screens->position.y)
                        prompt->position.y = prompt->startPos.y;
                }
            }

            UITAZoneModule->flag = false;
            heading->position.y  = heading->startPos.y;
            entity->position     = entity->posUnknown2;
            entity->id           = 0;
            entity->value        = 0;
            entity->timer++;
        }
        else if (entity->timer > 45) {
            parent->childHasFocus   = false;
            entity->timer           = 0;
            entity->state           = UITAZoneModule_Unknown17;
            entity->processButtonCB = UIButton_Unknown6;
        }
        else {
            entity->timer++;
        }
    }
    else {
        if (heading->startPos.y >> 16 < RSDK_screens->position.y) {
            int pos = (RSDK_screens->position.y << 16) + 0x1C0000;
            if (entity->timer >= 1) {
                if (16 * entity->timer <= 255)
                    pos -= 0x3800 * (RSDK.Sin1024(16 * entity->timer) >> 2);
                else
                    pos = (RSDK_screens->position.y << 16) - 0x1C0000;
            }
            heading->position.y = pos;
        }

        Hitbox hitbox;
        foreach_all(UIButtonPrompt, prompt)
        {
            int posX      = parent->startPos.x - parent->cameraOffset.x;
            int posY      = parent->startPos.y - parent->cameraOffset.y;
            hitbox.left   = -(parent->size.x) >> 17;
            hitbox.top    = -(parent->size.x) >> 17;
            hitbox.right  = (parent->size.x) >> 17;
            hitbox.bottom = (parent->size.y) >> 17;

            if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, prompt->position.x, prompt->position.y)) {
                if (prompt->startPos.y >> 16 < RSDK_screens->position.y) {
                    int offset1, offset2;
                    if (prompt == promptPtr)
                        offset1 = -0x180000;
                    else
                        offset1 = -0x300000;
                    if (prompt == promptPtr)
                        offset2 = 0x300000;
                    else
                        offset2 = 0x180000;
                    if (entity->timer >= 1) {
                        int pos = (RSDK_screens->position.y << 16) + offset2;
                        if (16 * entity->timer <= 255)
                            pos += (RSDK.Sin1024(16 * entity->timer) >> 2) * (((RSDK_screens->position.y) << 16 + offset1 - pos) >> 8);
                        else
                            pos = (RSDK_screens->position.y) << 16 + offset1;
                        prompt->position.y = pos;
                    }
                    else {
                        prompt->position.y = (RSDK_screens->position.y) << 16 + offset2;
                    }
                }
            }
        }

        if (entity->timer >= 1) {
            if (16 * entity->timer < 0x100)
                entity->id = 0x4D0000 - 0x4D00 * (RSDK.Sin1024(16 * entity->timer) >> 2);
            else
                entity->id = 0;
        }
        else {
            entity->id = 0x4D0000;
        }
        entity->timer++;
    }
}

void UITAZoneModule_Unknown29(void)
{
    RSDK_THIS(UITAZoneModule);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->field_138  = entity->animator2.frameID & 3;
    entity->velocity.x = 0x200000;
    if ((entity->position.x - entity->posUnknown2.x) >> 16 < RSDK_screens->width) {
        entity->position.x += 0x200000;
        entity->position.y += entity->velocity.y;
    }
    if ((entity->position.x - entity->posUnknown2.x) >> 16 >= RSDK_screens->width) {
        entity->position.x = entity->posUnknown2.x + (RSDK_screens->width << 16);
        entity->state      = UITAZoneModule_Unknown30;
    }
}

void UITAZoneModule_Unknown30(void)
{
    RSDK_THIS(UITAZoneModule);
    entity->position.x = entity->posUnknown2.x + (RSDK_screens->width << 16);
}

void UITAZoneModule_Unknown31(void)
{
    RSDK_THIS(UITAZoneModule);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->velocity.x = -0x200000;
    entity->field_138  = entity->animator2.frameID & 3;

    if (entity->position.x > entity->posUnknown2.x) {
        entity->position.x -= 0x200000;
        entity->position.y += entity->velocity.y;
    }
    if (entity->position.x <= entity->posUnknown2.x) {
        entity->position.x = entity->posUnknown2.x;
        entity->state      = UITAZoneModule_Unknown16;
    }
}

#endif

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
