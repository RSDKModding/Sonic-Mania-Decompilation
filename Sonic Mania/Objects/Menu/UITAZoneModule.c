#include "SonicMania.h"

ObjectUITAZoneModule *UITAZoneModule;

void UITAZoneModule_Update(void)
{
    RSDK_THIS(UITAZoneModule);
    UITAZoneModule_Setup();

    self->active = ACTIVE_NORMAL;
    if (self->disabled != self->wasDisabled) {
        UITAZoneModule_SetupText();
        self->wasDisabled = self->disabled;
    }
    self->drawPos = self->position;

#if !RETRO_USE_PLUS
    if (self->id != self->field_138_2)
        self->field_138_2 = self->id;
    if (self->value) {
        self->field_140++;
        self->position.y = self->posUnknown2.y + (self->id >> 2);
    }

    if (self->announceTimer) {
        --self->announceTimer;
        if (self->announceTimer == 121) {
            RSDK.PlaySfx(UIWidgets->sfxEvent, false, 0xFF);
        }
        if (self->announceTimer == 30) {
            if (self->field_138 == 1) {
                RSDK.PlaySfx(Announcer->sfxNewRecordTop, false, 0xFF);
            }
            else if (self->field_138 <= 3) {
                RSDK.PlaySfx(Announcer->sfxNewRecordMid, false, 0xFF);
            }
        }
    }

    self->drawPos.y = self->position.y - (self->field_138_2 >> 1);
#endif

    StateMachine_Run(self->state);

    self->field_12C++;
    if (self->field_12C >= 192)
        self->field_12C -= 192;

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    if (self->state == UITAZoneModule_Unknown17
        && (parent->buttons[parent->field_D8] != (EntityUIButton *)self || parent->state != UIControl_ProcessInputs)) {
        self->flag  = false;
        self->state = UITAZoneModule_Unknown16;
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
    
    if (self->field_138_2 >= 1) {
        RSDK.SetClipBounds(SceneInfo->currentScreenID, ((self->drawPos.x - 0x990000) >> 16) - ScreenInfo->position.x,
                           ((self->drawPos.y + 0x240000) >> 16) - 16 - ScreenInfo->position.y,
                           ((self->drawPos.x - 0x990000) >> 16) + 306 - ScreenInfo->position.x,
                           ((self->drawPos.y + 0x240000 + self->field_138_2) >> 16) - ScreenInfo->position.y);
        UITAZoneModule_Unknown7();
        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo->width, ScreenInfo->height);
    }
#endif
}

void UITAZoneModule_Create(void *data)
{
    RSDK_THIS(UITAZoneModule);
    self->posUnknown2.x   = self->position.x;
    self->posUnknown2.y   = self->position.y;
    self->active          = ACTIVE_BOUNDS;
    self->drawOrder       = 2;
    self->visible         = true;
    self->drawFX          = FX_FLIP;
    self->updateRange.x   = 0x800000;
    self->updateRange.y   = 0x300000;
    self->processButtonCB = UIButton_Unknown6;
    self->state           = UITAZoneModule_Unknown15;
    if (!SceneInfo->inEditor) {
        RSDK.SetText(&self->text3, "", false);
        RSDK.SetText(&self->text4, "", false);
        RSDK.CopyString(&self->text3, &self->text1);
        RSDK.CopyString(&self->text4, &self->text2);
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
    self->touchPosEnd.x = 0;
    self->touchPosEnd.y = 0;
    self->options7      = UITAZoneModule_Options7CB;
    self->options8      = UITAZoneModule_Options8CB;
    if (self->processButtonCB == UIButton_Unknown6) {
        self->touchPosStart.x = 0x1380000;
        self->touchPosStart.y = 0x4E0000;
        self->touchCB         = UIButton_ProcessTouch;
        self->options3        = UITAZoneModule_Options3CB;
        self->failCB          = UITAZoneModule_FailCB;
        self->options5        = UITAZoneModule_Options5CB;
        self->options6        = UITAZoneModule_Options6CB;
    }
#if !RETRO_USE_PLUS
    else if (self->processButtonCB == UITAZoneModule_Unknown24) {
        self->touchPos1[0].x       = 0x9C0000;
        self->touchPos1[0].y       = 0x2C0000;
        self->touchPos1[1].x       = 0x9C0000;
        self->touchPos1[1].y       = 0x2C0000;
        self->touchPos2[0].x       = -0x4E0000;
        self->touchPos2[1].x       = 0x4E0000;
        self->touchPosCallbacks[0] = UITAZoneModule_Unknown22;
        self->touchPosCallbacks[1] = UITAZoneModule_Unknown23;
        self->touchPosCount        = 2;
        self->options5             = StateMachine_None;
        self->options6             = StateMachine_None;
        self->touchCB              = UIButton_TouchCB_Alt;
        self->options3             = StateMachine_None;
        self->failCB               = StateMachine_None;
    }
#endif
    else {
        self->touchPosStart.x = 0;
        self->touchPosStart.y = 0;
        self->touchPosCount   = 0;
        self->options3        = StateMachine_None;
        self->failCB          = StateMachine_None;
        self->options5        = StateMachine_None;
        self->options6        = StateMachine_None;
    }
}

void UITAZoneModule_SetupText(void)
{
    RSDK_THIS(UITAZoneModule);
    if (!SceneInfo->inEditor) {
        if (!self->text1.text)
            RSDK.SetText(&self->text1, "", 0);
        if (!self->text2.text)
            RSDK.SetText(&self->text2, "", 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 3, &self->animator5, false, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 3, &self->animator6, false, 0);
        if (self->disabled) {
            RSDK.PrependText(&self->text1, "???");
            RSDK.PrependText(&self->text2, "");
            self->text2.textLength = 0;
        }
        else {
            RSDK.CopyString(&self->text1, &self->text3);
            RSDK.CopyString(&self->text2, &self->text4);
        }
        RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 3, &self->text1);
        RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 3, &self->text2);
    }
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 10, &self->animator2, false, 0);
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 11, &self->animator1, true, self->zoneID);
}

void UITAZoneModule_Unknown3(void)
{
    RSDK_THIS(UITAZoneModule);

    uint32 colour = 0x5FA0B0;
#if RETRO_USE_PLUS
    if (self->isEncore)
        colour = 0xF26C4F;
#endif

    int32 drawY = self->drawPos.y + 0x230000;
    UIWidgets_DrawRightTriangle(self->drawPos.x + 0x790000, drawY, -71, 88, 112, 224);

    RSDK.DrawRect(self->drawPos.x + 0x790000, drawY - 0x480000, 0x200000, 0x480000, 0x5870E0, 255, INK_NONE, false);
    RSDK.DrawRect(self->drawPos.x - 0x990000, self->drawPos.y - 0x1C0000, 0x1320000, 0x2C0000, 0, 255, INK_NONE, false);

    UIWidgets_DrawRightTriangle(self->drawPos.x + 0x990000, self->drawPos.y + 0x230000, -71, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, colour & 0xFF);

#if RETRO_USE_PLUS
    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(78, 312, self->position.x + 0x30000, self->position.y + 0x30000);

    if (self->flag)
        UIWidgets_DrawRectOutline_Flash(78, 312, self->position.x, self->position.y);
    else
        UIWidgets_DrawRectOutline_Black(78, 312, self->position.x, self->position.y);
#else
    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(78 + (self->field_138_2 >> 16), 312, self->position.x + 0x30000, self->position.y + 0x30000);

    if (self->flag || self->value)
        UIWidgets_DrawRectOutline_Flash(78 + (self->field_138_2 >> 16), 312, self->position.x, self->position.y);
    else
        UIWidgets_DrawRectOutline_Black(78 + (self->field_138_2 >> 16), 312, self->position.x, self->position.y);
#endif
}

void UITAZoneModule_Unknown4(void)
{
    RSDK_THIS(UITAZoneModule);
    Vector2 drawPos;
    drawPos     = self->drawPos;
    uint32 colour = 0xF0F0F0;
#if RETRO_USE_PLUS
    if (self->isEncore)
        colour = 0xF26C4F;
#endif
    drawPos.x -= 0x390000;
    drawPos.y += 0x170000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x840000, 0xD0000, colour, 255, INK_NONE, false);

#if RETRO_USE_PLUS
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->animator3, true, (self->isEncore != 0) + 10);
#else
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->animator3, true, 10);
#endif
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

#if RETRO_USE_PLUS
    drawPos = UIWidgets_DrawTriJoinRect(drawPos.x + 0x840000, drawPos.y, colour, 0xF0D808);
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x400000, 0xD0000, 0xF0D808, 255, INK_NONE, false);
#else
    drawPos.x += 0x5C0000;
    UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, 12, 0xF0, 0xF0, 0xF0);

    drawPos.x += 0xE0000;
    drawPos.y += 0xC0000;
    UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, -12, 0xE8, 0x28, 0x58);

    drawPos.y -= 0xC0000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x680000, 0xD0000, 0xE82858, 0xFF, INK_NONE, false);

    drawPos.x = self->drawPos.x + 0x430000;
    drawPos.y = (self->drawPos.y + 0xD0000) + 0x100000;
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->animator3, true, 9);
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

    drawPos.x += 0x100000;
    drawPos.y -= 0x20000;
    if (SceneInfo->inEditor || !self->characterID) {
        UITAZoneModule_DrawTime(drawPos.x, drawPos.y, 0, 0, 0);
    }
    else {
        uint16 *records1 = TimeAttackData_GetRecordedTime(self->zoneID, 0, self->characterID, 1);
        uint16 *records2 = TimeAttackData_GetRecordedTime(self->zoneID, 1, self->characterID, 1);

        int32 time = records1[0] + records2[0];

        if (records1[0] && records2[0])
            UITAZoneModule_DrawTime(drawPos.x, drawPos.y, time / 6000, time % 6000 / 100, time % 100);
        else if (self->characterID != 3 && records1[0] && self->zoneID == 7)
            UITAZoneModule_DrawTime(drawPos.x, drawPos.y, time / 6000, time % 6000 / 100, time % 100);
        else
            UITAZoneModule_DrawTime(drawPos.x, drawPos.y, 0, 0, 0);
    }
#endif
}

void UITAZoneModule_Unknown5(void)
{
    RSDK_THIS(UITAZoneModule);
    Vector2 drawPos;

    drawPos.x = self->drawPos.x - 0x690000;
    drawPos.y = self->drawPos.y;
    UIWidgets_DrawRectOutline_Black(72, 96, drawPos.x, drawPos.y);
    if (SceneInfo->inEditor || !self->flag || self->disabled) {
        self->direction = self->field_138;
        self->drawFX    = FX_FLIP;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->direction = FLIP_NONE;
        self->drawFX    = FX_NONE;
    }
    else {
        SpriteFrame *frame = RSDK.GetFrame(UITAZoneModule->aniFrames, 11, self->zoneID);
        frame->pivotX      = -45;
        frame->width       = 90;
        frame->sprX        = self->field_12C;

        if (self->field_12C <= 102) {
            RSDK.DrawSprite(&self->animator1, &drawPos, false);
        }
        else {
            int32 width    = self->field_12C - 102;
            frame->width = 90 - width;
            RSDK.DrawSprite(&self->animator1, &drawPos, false);

            frame->pivotX += frame->width;
            frame->sprX  = 0;
            frame->width = width;
            RSDK.DrawSprite(&self->animator1, &drawPos, false);
        }
    }
}

void UITAZoneModule_Unknown6(void)
{
    RSDK_THIS(UITAZoneModule);
    Vector2 drawPos;

    drawPos = self->position;
    UITAZoneModule_Unknown4();
    UITAZoneModule_Unknown5();

    drawPos.x = self->drawPos.x;
    drawPos.y = self->drawPos.y;
    if (self->state != UITAZoneModule_Unknown18 || !(self->timer & 2)) {
        if (!SceneInfo->inEditor) {
            drawPos.x = self->drawPos.x - 0x370000;
            if (self->text2.textLength) {
                drawPos.y = self->drawPos.y - 0x160000;
                RSDK.DrawText(&self->animator5, &drawPos, &self->text1, 0, self->text1.textLength, ALIGN_LEFT, 0, 2, 0, false);

                drawPos.y += 0x1C0000;
                drawPos.x += 0x200000;
                RSDK.DrawText(&self->animator6, &drawPos, &self->text2, 0, self->text2.textLength, ALIGN_LEFT, 0, 2, 0, false);
            }
            else {
                drawPos.y = self->drawPos.y - 0x60000;
                RSDK.DrawText(&self->animator5, &drawPos, &self->text1, 0, self->text1.textLength, ALIGN_LEFT, 0, 2, 0, false);
            }
        }
    }
}

void UITAZoneModule_FailCB(void) { RSDK.PlaySfx(UIWidgets->sfxFail, false, 255); }

void UITAZoneModule_Options3CB(void)
{
    RSDK_THIS(UITAZoneModule);
    self->timer = 0;
    self->state = UITAZoneModule_Unknown18;
#if !RETRO_USE_PLUS
    EntityUIControl *parent = (EntityUIControl *)self->parent;
    parent->childHasFocus   = true;
    foreach_all(UITAZoneModule, module)
    {
        if (module != self)
            module->state = UITAZoneModule_Unknown29;
    }
#endif
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
#if RETRO_USE_PLUS
    if (TimeAttackMenu->encoreMode)
        RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 4, (32 * (self->zoneID % 12)), 0, 224, 32);
    else
#endif
        RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 1, (32 * (self->zoneID % 12)), 0, 224, 32);
}

bool32 UITAZoneModule_Options7CB(void)
{
    RSDK_THIS(UITAZoneModule);
    return self->state == UITAZoneModule_Unknown17;
}

bool32 UITAZoneModule_Options8CB(void)
{
    RSDK_THIS(UITAZoneModule);
    return self->state == UITAZoneModule_Unknown18;
}

void UITAZoneModule_Options5CB(void)
{
    RSDK_THIS(UITAZoneModule);
    self->flag  = true;
    self->state = UITAZoneModule_Unknown17;
#if !RETRO_USE_PLUS
    self->rank = 0;
#endif
#if RETRO_USE_PLUS
    if (TimeAttackMenu->encoreMode)
        RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 4, (32 * (self->zoneID % 12)), 0, 224, 32);
    else
#endif
        RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 1, (32 * (self->zoneID % 12)), 0, 224, 32);
}

void UITAZoneModule_Options6CB(void)
{
    RSDK_THIS(UITAZoneModule);
    self->flag  = false;
    self->state = UITAZoneModule_Unknown16;
}

void UITAZoneModule_TransitionCB(void) { UIControl_MatchMenuTag("Leaderboards"); }

void UITAZoneModule_Unknown14(void) { UITransition_StartTransition(UITAZoneModule_TransitionCB, 0); }

void UITAZoneModule_Unknown15(void)
{
    RSDK_THIS(UITAZoneModule);
#if !RETRO_USE_PLUS
    EntityUIControl *parent = (EntityUIControl *)self->parent;
    Hitbox hitbox;
    foreach_active(UIButtonPrompt, prompt)
    {
        int32 posX      = parent->startPos.x - parent->cameraOffset.x;
        int32 posY      = parent->startPos.y - parent->cameraOffset.y;
        hitbox.right  = parent->size.x >> 17;
        hitbox.left   = -(parent->size.x >> 17);
        hitbox.bottom = parent->size.y >> 17;
        hitbox.top    = -(parent->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, prompt->position.x, prompt->position.y) && prompt->buttonID == 3)
            UITAZoneModule->entityPtr = (Entity *)prompt;
    }
#endif

    self->state = UITAZoneModule_Unknown16;
    UITAZoneModule_Unknown16();
}

void UITAZoneModule_Unknown16(void)
{
    RSDK_THIS(UITAZoneModule);
    self->position.x = self->posUnknown2.x;
    RSDK.ProcessAnimation(&self->animator2);
    self->touchCB   = UIButton_ProcessTouch;
    self->field_138 = self->animator2.frameID & 3;
}

void UITAZoneModule_Unknown17(void)
{
    RSDK_THIS(UITAZoneModule);
    RSDK.ProcessAnimation(&self->animator2);
    self->field_138 = self->animator2.frameID & 3;
#if RETRO_USE_PLUS
    if (UITAZoneModule->flag) {
        if (TimeAttackMenu->encoreMode)
            RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 4, (32 * (self->zoneID % 12)), 0, 224, 32);
        else
            RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 1, (32 * (self->zoneID % 12)), 0, 224, 32);
    }
#endif
}

void UITAZoneModule_Unknown18(void)
{
    RSDK_THIS(UITAZoneModule);

    self->flag = true;
#if !RETRO_USE_PLUS
    self->touchCB = StateMachine_None;
#endif
    if (self->timer >= 30) {
#if !RETRO_USE_PLUS
        self->value        = 1;
        self->field_140    = 0;
        self->timer        = 0;
        self->rank         = 0;
        self->actID        = self->zoneID == 7 && self->characterID != 3;
        UITAZoneModule->flag = true;
        self->state        = UITAZoneModule_Unknown27;
#else
        self->timer = 0;
        StateMachine_Run(self->options2);
        self->state = UITAZoneModule_Unknown17;
#endif
    }
    else {
        self->timer++;
    }
}

#if !RETRO_USE_PLUS
Vector2 UITAZoneModule_DrawTime(int32 x, int32 y, int32 minutes, int32 seconds, int32 milliseconds)
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

    Vector2 drawPos;
    drawPos.x = x;
    drawPos.y = y;
    for (int32 i = 0; i < 8; ++i) {
        if (!strBuf[i])
            break;
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 8, &self->animator3, true, (uint8)(strBuf[i] - '0'));
        RSDK.DrawSprite(&self->animator3, &drawPos, false);
        drawPos.x += 0x80000;
    }
    return drawPos;
}

void UITAZoneModule_Unknown8(void)
{
    RSDK_THIS(UITAZoneModule);
    Vector2 drawPos = self->drawPos;

    uint32 colour1 = 0;
    if (self->actID)
        colour1 = 0xE82858;

    uint32 colour2 = 0;
    if (self->actID != 1)
        colour2 = 0xE82858;

    RSDK.DrawRect(drawPos.x - 0x990000, drawPos.y + 0x240000, 0x860000, 0x150000, colour1, 255, INK_NONE, false);

    int32 offset = 0;
    if (RSDK.Sin256(8 * minVal((uint8)self->field_140, 16)) >= 0)
        offset = RSDK.Sin256(8 * minVal((uint8)self->field_140, 16));
    else
        offset = -RSDK.Sin256(8 * minVal((uint8)self->field_140, 16));
    offset >>= 6;

    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &self->animator7, true, 0);
    drawPos.x -= 0x630000;
    drawPos.y += 0x2E0000;

    bool32 flag = true;
    if (!self->actID) {
        drawPos.x -= (offset << 16);
        drawPos.y -= (offset << 16);
        if (self->state == UITAZoneModule_Unknown21 && (self->timer & 2)) {
            drawPos.x += offset << 16;
            drawPos.y += offset << 16;
            flag = false;
        }
    }
    if (flag) {
        if (self->zoneID != 7 || self->characterID == 3) {
            RSDK.DrawSprite(&self->animator7, &drawPos, false);
            if (!self->actID) {
                drawPos.x += offset << 16;
                drawPos.y += offset << 16;
            }
        }
    }

    Vector2 drawPos2, drawPos3;
    drawPos2.x = self->drawPos.x - 0x1A0000;
    drawPos2.y = self->drawPos.y + 0x2E0000;

    drawPos.x = self->drawPos.x - 0x130000;
    drawPos.y = self->drawPos.y + 0x240000;
    UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, 20, (colour1 >> 16) & 0xFF, (colour1 >> 8) & 0xFF, colour1 & 0xFF);

    drawPos.x += 0x140000;
    drawPos.y += 0x140000;
    UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, -20, (colour2 >> 16) & 0xFF, (colour2 >> 8) & 0xFF, colour2 & 0xFF);

    drawPos3.x = drawPos.x + 0x40000;
    drawPos3.y = drawPos.y - 0xA0000;
    drawPos.y -= 0x140000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x980000, 0x150000, colour2, 255, INK_NONE, false);

    drawPos.x += 0x4B0000;
    drawPos.y += 0xA0000;

    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &self->animator7, true, 1);
    
    flag = true;
    if (self->actID == 1) {
        drawPos.x -= (offset << 16);
        drawPos.y -= (offset << 16);
        if (self->state == UITAZoneModule_Unknown21 && (self->timer & 2)) {
            drawPos.x += offset << 16;
            drawPos.y += offset << 16;
            flag = false;
        }
    }
    if (flag) {
        RSDK.DrawSprite(&self->animator7, &drawPos, false);
        if (self->actID == 1) {
            drawPos.x += offset << 16;
            drawPos.y += offset << 16;
        }
    }

    self->drawFX    = FX_FLIP;
    self->direction = FLIP_X;
    if (self->actID == 1)
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->animator7, true, 3);
    else
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->animator7, true, 2);
    if (!self->actID) {
        if (RSDK.Sin256(2 * (uint8)self->field_140) >= 0)
            drawPos2.x -= RSDK.Sin256(2 * (uint8)self->field_140) << 10;
        else
            drawPos2.x -= -RSDK.Sin256(2 * (uint8)self->field_140) << 10;
    }

    if (self->zoneID != 7 || self->characterID == 3)
        RSDK.DrawSprite(&self->animator7, &drawPos2, false);

    self->direction = FLIP_NONE;
    self->drawFX    = FX_NONE;
    if (self->actID == 1)
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->animator7, true, 2);
    else
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->animator7, true, 3);
    if (self->actID == 1) {
        if (RSDK.Sin256(2 * (uint8)self->field_140) >= 0)
            drawPos3.x -= RSDK.Sin256(2 * (uint8)self->field_140) << 10;
        else
            drawPos3.x -= -RSDK.Sin256(2 * (uint8)self->field_140) << 10;
    }

    RSDK.DrawSprite(&self->animator7, &drawPos3, false);
}

void UITAZoneModule_Unknown7(void)
{
    RSDK_THIS(UITAZoneModule);
    int32 drawX = self->drawPos.x;
    int32 drawY = self->drawPos.y;
    Vector2 drawPos2 = self->drawPos;

    uint16 *records = TimeAttackData_GetRecordedTime(self->zoneID, self->actID, self->characterID, 1);

    RSDK.DrawRect(drawX - 0x990000, drawY + 0x240000, 0x1320000, 0x4D0000, 0x000000, 255, INK_NONE, false);
    UITAZoneModule_Unknown8();
    UIWidgets_DrawRhombus(0x17, 0xE0, 0x17, 0xD9, 0xAD, 0x4, self->position.x - 0x40000, drawY + 0x448000);

    drawPos2.x = drawX - 0x390000;
    drawPos2.y = drawY + 0x450000;
    if (self->rank != 1 || (UIControl->timer & 4)) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &self->animator7, true, 2);
        RSDK.DrawSprite(&self->animator7, &drawPos2, false);

        int32 time = 0;
        if (records)
            time = records[0];

        Vector2 pos;
        pos.x = drawX - 0x10000;
        pos.y = drawY + 0x420000;
        pos   = UITAZoneModule_DrawTime(pos.x, pos.y, time / 6000, time % 6000 / 100, time % 100);
    }
    drawY += 0x140000;

    UIWidgets_DrawRhombus(0x0F, 0xE0, 0x0F, 0x98, 0xC0, 0xC8, self->position.x + 0x80000, drawY + 0x448000);
    drawPos2.x = drawX - 0x290000;
    drawPos2.y = drawY + 0x460000;
    if (self->rank != 2 || (UIControl->timer & 4)) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &self->animator7, true, 3);
        RSDK.DrawSprite(&self->animator7, &drawPos2, 0);

        int32 time = 0;
        if (records)
            time = records[1];

        Vector2 pos;
        pos.x = drawX + 0x140000;
        pos.y = drawY + 0x420000;
        pos   = UITAZoneModule_DrawTime(pos.x, pos.y, time / 6000, time % 6000 / 100, time % 100);
    }

    UIWidgets_DrawRhombus(0xF, 0xE0, 0xF, 0xC0, 0x58, 0x01, self->position.x + 0x140000, drawY + 0x548000);
    drawPos2.x = drawX - 0x1A0000;
    drawPos2.y = drawY + 0x560000;
    if (self->rank != 3 || (UIControl->timer & 4)) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &self->animator7, true, 4);
        RSDK.DrawSprite(&self->animator7, &drawPos2, false);

        int32 time = 0;
        if (records)
            time = records[2];

        Vector2 pos;
        pos.x = drawX + 0x230000;
        pos.y = drawY + 0x520000;
        pos   = UITAZoneModule_DrawTime(pos.x, pos.y, time / 6000, time % 6000 / 100, time % 100);
    }
}

void UITAZoneModule_Unknown19(void) {}

void UITAZoneModule_Unknown21(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    self->flag = true;
    if (self->timer >= 30) {
        TimeAttackData_ClearOptions();
        param->zoneID = self->zoneID;
        param->actID  = self->actID;
        self->timer = 0;
        StateMachine_Run(self->options2);
        self->state           = UITAZoneModule_Unknown19;
        self->processButtonCB = StateMachine_None;
    }
    else {
        self->timer++;
    }
}

void UITAZoneModule_Unknown22(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    if (self->zoneID != 7 || self->characterID == 3) {
        self->actID = 0;

        int32 id = API_MostRecentActiveControllerID(0);
        API_ResetControllerAssignments();
        API_AssignControllerID(1, id);

        parent->state = StateMachine_None;
        self->timer = 0;
        self->state = UITAZoneModule_Unknown21;
        RSDK.StopChannel(Music->channelID);
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
    }
}

void UITAZoneModule_Unknown23(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    int32 id = API_MostRecentActiveControllerID(0);
    API_ResetControllerAssignments();
    API_AssignControllerID(1, id);

    parent->state = StateMachine_None;
    self->timer = 0;
    self->state = UITAZoneModule_Unknown21;
    RSDK.StopChannel(Music->channelID);
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
}

void UITAZoneModule_Unknown24(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    if (UIControl->keyLeft && self->actID == 1 && (self->zoneID != 7 || self->characterID == 3)) {
        self->actID = 0;
        self->rank  = 0;
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
        self->field_140 = 0;
    }
    if (UIControl->keyRight && !self->actID && (self->zoneID != 7 || self->characterID == 3)) {
        self->actID     = 1;
        self->rank  = 0;
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
        self->field_140 = 0;
    }
    else if (UIControl->keyConfirm) {
        int32 id = API_MostRecentActiveControllerID(0);
        API_ResetControllerAssignments();
        API_AssignControllerID(1, id);

        parent->state = StateMachine_None;
        self->timer = 0;
        self->state = UITAZoneModule_Unknown21;
        RSDK.StopChannel(Music->channelID);
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
    }
    else {
        if (MenuSetup && UIControl->keyY) {
            UITAZoneModule_Unknown25(self->characterID, self->zoneID, self->actID, 0, UITAZoneModule_Unknown14);
            RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
        }
        if (UIControl->keyBack) {
            self->timer           = 0;
            self->state           = UITAZoneModule_Unknown28;
            self->processButtonCB = StateMachine_None;
        }
    }
}

void UITAZoneModule_Unknown25(int32 player, int32 zone, int32 act, int32 a4, void (*callback)(void))
{
    TextInfo info;
    INIT_TEXTINFO(info);

    Localization_GetString(&info, STR_CONNECTING);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        UIDialog_Setup(dialog);
        MenuSetup->dialog = (Entity *)dialog;
        MenuSetup_StartTransitionLB(callback, 120);

        uint16 *records                  = TimeAttackData_GetRecordedTime(zone, act, player, 1);
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

void UITAZoneModule_Unknown26(EntityUIControl *control, uint8 characterID, uint32 zoneID, uint8 actID, int32 score)
{
    control->childHasFocus       = true;
    control->activeEntityID      = zoneID;
    EntityUITAZoneModule *module = (EntityUITAZoneModule *)control->buttons[zoneID];

    control->position.x = module->posUnknown2.x;
    control->position.y = module->posUnknown2.y;
    control->posUnknown = module->posUnknown2;

    ScreenInfo->position.x = (module->posUnknown2.x >> 16) - ScreenInfo->centerX;
    ScreenInfo->position.y = (control->position.y >> 16) - ScreenInfo->centerY;
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

    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)control->buttons[i];
        if (button != module) {
            button->state      = UITAZoneModule_Unknown30;
            button->value      = 0;
            button->flag       = false;
            button->position.x = button->posUnknown2.x + (ScreenInfo->width << 16);
        }
    }

    EntityUIControl *zoneControl = (EntityUIControl *)MenuSetup->timeAttackZones;
    for (int32 i = 0; i < zoneControl->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)zoneControl->buttons[i];
        button->characterID          = characterID;
    }
}

void UITAZoneModule_Unknown27(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityUIControl *parent         = (EntityUIControl *)self->parent;
    EntityUIHeading *heading        = (EntityUIHeading *)parent->heading;
    EntityUIButtonPrompt *promptPtr = (EntityUIButtonPrompt *)UITAZoneModule->entityPtr;

    if (self->timer > 15) {
        self->id = 0x4D0000;

        foreach_all(UIButtonPrompt, prompt)
        {
            Hitbox hitbox;
            foreach_all(UIButtonPrompt, prompt)
            {
                int32 posX      = parent->startPos.x - parent->cameraOffset.x;
                int32 posY      = parent->startPos.y - parent->cameraOffset.y;
                hitbox.left   = -(parent->size.x) >> 17;
                hitbox.top    = -(parent->size.x) >> 17;
                hitbox.right  = (parent->size.x) >> 17;
                hitbox.bottom = (parent->size.y) >> 17;

                if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, prompt->position.x, prompt->position.y)) {
                    if (prompt->startPos.y >> 16 < ScreenInfo->position.y) {
                        prompt->position.y = (ScreenInfo->position.y << 16) + 0x180000;
                        if (prompt == promptPtr)
                            promptPtr->position.y += 0x180000;
                    }
                }
            }
        }

        heading->position.y     = (ScreenInfo->position.y << 16) + 0x1C0000;
        self->timer           = 0;
        self->state           = UITAZoneModule_Unknown19;
        self->processButtonCB = UITAZoneModule_Unknown24;
        self->touchCB         = UIButton_TouchCB_Alt;
    }
    else {
        if (heading->startPos.y >> 16 < ScreenInfo->position.y) {
            int32 pos = (ScreenInfo->position.y << 16) - 0x1C0000;
            if (self->timer >= 1) {
                int32 pos = (ScreenInfo->position.y << 16) + 0x1C0000;
                if (16 * self->timer <= 255)
                    pos += 0x3800 * (RSDK.Sin1024(16 * self->timer) >> 2);
                else
                    pos = (ScreenInfo->position.y << 16) + 0x1C0000;
            }
            heading->position.y = pos;
        }

        Hitbox hitbox;
        foreach_all(UIButtonPrompt, prompt)
        {
            int32 posX      = parent->startPos.x - parent->cameraOffset.x;
            int32 posY      = parent->startPos.y - parent->cameraOffset.y;
            hitbox.left   = -(parent->size.x) >> 17;
            hitbox.top    = -(parent->size.x) >> 17;
            hitbox.right  = (parent->size.x) >> 17;
            hitbox.bottom = (parent->size.y) >> 17;

            if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, prompt->position.x, prompt->position.y)) {
                if (!prompt->buttonID)
                    prompt->promptID = 4;
                if (prompt->startPos.y >> 16 < ScreenInfo->position.y) {
                    int32 offset1, offset2;
                    if (prompt == promptPtr)
                        offset1 = 0x300000;
                    else
                        offset1 = 0x180000;
                    if (prompt == promptPtr)
                        offset2 = -0x180000;
                    else
                        offset2 = -0x300000;

                    int32 pos = (ScreenInfo->position.y << 16) + offset2;
                    if (self->timer >= 1) {
                        if (16 * self->timer <= 255)
                            pos += (RSDK.Sin1024(16 * self->timer) >> 2) * ((((ScreenInfo->position.y) << 16) + offset1 - pos) >> 8);
                        else
                            pos = (ScreenInfo->position.y) << 16 + offset1;
                    }
                    prompt->position.y = pos;
                }
            }
        }

        if (self->timer < 1) {
            self->id = 0;
        }
        else {
            if ((16 * self->timer) < 0x100) {
                self->id = 0x4D00 * (RSDK.Sin1024(16 * self->timer) >> 2);
            }
            else {
                self->id = 0x4D0000;
            }
        }
        self->timer++;
    }
}

void UITAZoneModule_Unknown28(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityUIControl *parent         = (EntityUIControl *)self->parent;
    EntityUIHeading *heading        = (EntityUIHeading *)parent->heading;
    EntityUIButtonPrompt *promptPtr = (EntityUIButtonPrompt *)UITAZoneModule->entityPtr;

    self->processButtonCB = StateMachine_None;
    if (self->timer > 15) {
        if (self->timer == 16) {
            foreach_all(UITAZoneModule, module)
            {
                if (module != self)
                    module->state = UITAZoneModule_Unknown31;
            }

            Hitbox hitbox;
            foreach_all(UIButtonPrompt, prompt)
            {
                int32 posX      = parent->startPos.x - parent->cameraOffset.x;
                int32 posY      = parent->startPos.y - parent->cameraOffset.y;
                hitbox.left   = -(parent->size.x) >> 17;
                hitbox.top    = -(parent->size.x) >> 17;
                hitbox.right  = (parent->size.x) >> 17;
                hitbox.bottom = (parent->size.y) >> 17;

                if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, prompt->position.x, prompt->position.y)) {
                    if (!prompt->buttonID)
                        prompt->promptID = 0;
                    if (prompt->startPos.y >> 16 < ScreenInfo->position.y)
                        prompt->position.y = prompt->startPos.y;
                }
            }

            UITAZoneModule->flag = false;
            heading->position.y  = heading->startPos.y;
            self->position     = self->posUnknown2;
            self->id           = 0;
            self->value        = 0;
            self->timer++;
        }
        else if (self->timer > 45) {
            parent->childHasFocus   = false;
            self->timer           = 0;
            self->state           = UITAZoneModule_Unknown17;
            self->processButtonCB = UIButton_Unknown6;
        }
        else {
            self->timer++;
        }
    }
    else {
        if (heading->startPos.y >> 16 < ScreenInfo->position.y) {
            int32 pos = (ScreenInfo->position.y << 16) + 0x1C0000;
            if (self->timer >= 1) {
                if (16 * self->timer <= 255)
                    pos -= 0x3800 * (RSDK.Sin1024(16 * self->timer) >> 2);
                else
                    pos = (ScreenInfo->position.y << 16) - 0x1C0000;
            }
            heading->position.y = pos;
        }

        Hitbox hitbox;
        foreach_all(UIButtonPrompt, prompt)
        {
            int32 posX      = parent->startPos.x - parent->cameraOffset.x;
            int32 posY      = parent->startPos.y - parent->cameraOffset.y;
            hitbox.left   = -(parent->size.x) >> 17;
            hitbox.top    = -(parent->size.x) >> 17;
            hitbox.right  = (parent->size.x) >> 17;
            hitbox.bottom = (parent->size.y) >> 17;

            if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, prompt->position.x, prompt->position.y)) {
                if (prompt->startPos.y >> 16 < ScreenInfo->position.y) {
                    int32 offset1, offset2;
                    if (prompt == promptPtr)
                        offset1 = -0x180000;
                    else
                        offset1 = -0x300000;
                    if (prompt == promptPtr)
                        offset2 = 0x300000;
                    else
                        offset2 = 0x180000;
                    if (self->timer >= 1) {
                        int32 pos = (ScreenInfo->position.y << 16) + offset2;
                        if (16 * self->timer <= 255)
                            pos += (RSDK.Sin1024(16 * self->timer) >> 2) * (((ScreenInfo->position.y) << 16 + offset1 - pos) >> 8);
                        else
                            pos = (ScreenInfo->position.y) << 16 + offset1;
                        prompt->position.y = pos;
                    }
                    else {
                        prompt->position.y = (ScreenInfo->position.y) << 16 + offset2;
                    }
                }
            }
        }

        if (self->timer >= 1) {
            if (16 * self->timer < 0x100)
                self->id = 0x4D0000 - 0x4D00 * (RSDK.Sin1024(16 * self->timer) >> 2);
            else
                self->id = 0;
        }
        else {
            self->id = 0x4D0000;
        }
        self->timer++;
    }
}

void UITAZoneModule_Unknown29(void)
{
    RSDK_THIS(UITAZoneModule);
    RSDK.ProcessAnimation(&self->animator2);
    self->field_138  = self->animator2.frameID & 3;
    self->velocity.x = 0x200000;
    if ((self->position.x - self->posUnknown2.x) >> 16 < ScreenInfo->width) {
        self->position.x += 0x200000;
        self->position.y += self->velocity.y;
    }
    if ((self->position.x - self->posUnknown2.x) >> 16 >= ScreenInfo->width) {
        self->position.x = self->posUnknown2.x + (ScreenInfo->width << 16);
        self->state      = UITAZoneModule_Unknown30;
    }
}

void UITAZoneModule_Unknown30(void)
{
    RSDK_THIS(UITAZoneModule);
    self->position.x = self->posUnknown2.x + (ScreenInfo->width << 16);
}

void UITAZoneModule_Unknown31(void)
{
    RSDK_THIS(UITAZoneModule);
    RSDK.ProcessAnimation(&self->animator2);
    self->velocity.x = -0x200000;
    self->field_138  = self->animator2.frameID & 3;

    if (self->position.x > self->posUnknown2.x) {
        self->position.x -= 0x200000;
        self->position.y += self->velocity.y;
    }
    if (self->position.x <= self->posUnknown2.x) {
        self->position.x = self->posUnknown2.x;
        self->state      = UITAZoneModule_Unknown16;
    }
}

#endif

#if RETRO_INCLUDE_EDITOR
void UITAZoneModule_EditorDraw(void)
{
    RSDK_THIS(UITAZoneModule);

    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 3, &self->animator5, false, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 3, &self->animator6, false, 0);
    UITAZoneModule_SetupText();

    self->inkEffect = self->disabled ? INK_BLEND : INK_NONE;

    UITAZoneModule_Draw();
}

void UITAZoneModule_EditorLoad(void) { UITAZoneModule->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }
#endif

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
