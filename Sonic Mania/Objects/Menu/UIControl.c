#include "SonicMania.h"

ObjectUIControl *UIControl;

void UIControl_Update(void)
{
    RSDK_THIS(UIControl);

    if (self->activeEntityID >= 0 && self->activeEntityID != self->field_D8)
        self->field_D8 = self->activeEntityID;
    if (!UIControl->field_C8 && self->activeEntityID == -1)
        self->activeEntityID = self->field_D8;
    StateMachine_Run(self->state);
    if (self->backoutTimer > 0)
        self->backoutTimer--;
    StateMachine_Run(self->unknownCallback4);
}

void UIControl_LateUpdate(void) {}

void UIControl_StaticUpdate(void)
{
    if (UIControl->field_C) {
        UIControl->field_C     = 0;
        UIControl->inputLocked = true;
    }
    else {
        UIControl->inputLocked = 0;
    }
    UIControl->flagA = 0;
    ++UIControl->timer;
    UIControl->timer &= 0x7FFF;
}

void UIControl_Draw(void)
{
    RSDK_THIS(UIControl);
    ScreenInfo->position.x = (self->position.x >> 0x10) - ScreenInfo->centerX;
    ScreenInfo->position.y = (self->position.y >> 0x10) - ScreenInfo->centerY;
}

void UIControl_Create(void *data)
{
    RSDK_THIS(UIControl);
    if (!SceneInfo->inEditor) {
        if (data) {
            Vector2 *size  = (Vector2 *)data;
            self->size.x = size->x;
            self->size.y = size->y;
        }
        self->updateRange.x = self->size.x >> 1;
        self->updateRange.y = self->size.y >> 1;
#if RETRO_USE_PLUS
        self->promptCount = 0;
#endif
        if (self->rowCount <= 1)
            self->rowCount = 1;

        if (self->columnCount <= 1)
            self->columnCount = 1;

        if (!self->dwordC4)
            self->activeEntityID = self->startingID;
        else
            self->activeEntityID = self->storedEntityID;

        self->position.x += self->cameraOffset.x;
        self->position.y += self->cameraOffset.y;
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
#if RETRO_USE_PLUS
        int32 entityID       = RSDK.GetEntityID(self);
        if (UIButtonPrompt) {
            if (entityID != SLOT_DIALOG_UICONTROL) {
                foreach_all(UIButtonPrompt, prompt)
                {
                    if (self->promptCount < 4) {
                        Hitbox hitbox;
                        hitbox.right  = self->size.x >> 17;
                        hitbox.left   = -(self->size.x >> 17);
                        hitbox.bottom = self->size.y >> 17;
                        hitbox.top    = -(self->size.y >> 17);
                        if (MathHelpers_PointInHitbox(FLIP_NONE, self->startPos.x - self->cameraOffset.x,
                                                      self->startPos.y - self->cameraOffset.y, &hitbox, prompt->position.x, prompt->position.y)) {
                            prompt->parent                     = (Entity *)self;
                            self->prompts[self->promptCount++] = prompt;
                        }
                    }
                }
            }
        }
#endif

        AnalogStickInfoL[1].deadzone = 0.75f;
        AnalogStickInfoL[2].deadzone = 0.75f;
        AnalogStickInfoL[3].deadzone = 0.75f;
        AnalogStickInfoL[4].deadzone = 0.75f;
        UIControl_Unknown7();
        if (self->noWidgets) {
            self->active  = ACTIVE_NORMAL;
            self->visible = true;
        }
        else {
            self->dwordCC = 0;
            if (self->activeOnLoad) {
                UIControl_Unknown4(self);
            }
            else {
                self->active = ACTIVE_NEVER;
            }
        }
    }
}

void UIControl_StageLoad(void)
{
    UIControl->inputLocked = false;
    UIControl->field_C     = 0;
    UIControl->active      = ACTIVE_ALWAYS;
    UIControl->field_4     = 0;
}

EntityUIControl *UIControl_GetUIControl(void)
{
    foreach_all(UIControl, control)
    {
        if (control->active == ACTIVE_ALWAYS) {
            foreach_return control;
        }
    }
    return NULL;
}

void UIControl_ClearInputs(char id)
{
    for (int32 i = 0; i < 4; ++i) {
        UIControl->upPress[i]      = false;
        UIControl->downPress[i]    = false;
        UIControl->leftPress[i]    = false;
        UIControl->rightPress[i]   = false;
        UIControl->yPress[i]       = false;
        UIControl->xPress[i]       = false;
        UIControl->backPress[i]    = false;
        UIControl->confirmPress[i] = false;
#if RETRO_USE_PLUS
        UIControl->startPress[i] = false;
#endif
    }

    UIControl->keyUp    = false;
    UIControl->keyDown  = false;
    UIControl->keyLeft  = false;
    UIControl->keyRight = false;
    UIControl->keyY     = id == 3;
    UIControl->keyX     = id == 2;
#if RETRO_USE_PLUS
    UIControl->keyStart = id == 5;
#endif

    if (API_GetConfirmButtonFlip()) {
        UIControl->keyConfirm = id == 1;
        UIControl->keyBack    = id == 0;
        UIControl->flagA      = id == 0;
    }
    else {
        UIControl->keyConfirm = id == 0;
        UIControl->keyBack    = id == 1;
        UIControl->flagA      = id == 1;
    }
    UIControl->field_C     = true;
    UIControl->inputLocked = true;
}

void UIControl_ProcessInputs(void)
{
    RSDK_THIS(UIControl);
    UIControl_Unknown16();

    if (!UIControl->inputLocked) {
        for (int32 i = 0; i < 4; ++i) {
            UIControl->upPress[i]    = ControllerInfo[i + 1].keyUp.press || AnalogStickInfoL[i + 1].keyUp.press;
            UIControl->downPress[i]  = ControllerInfo[i + 1].keyDown.press || AnalogStickInfoL[i + 1].keyDown.press;
            UIControl->leftPress[i]  = ControllerInfo[i + 1].keyLeft.press || AnalogStickInfoL[i + 1].keyLeft.press;
            UIControl->rightPress[i] = ControllerInfo[i + 1].keyRight.press || AnalogStickInfoL[i + 1].keyRight.press;

            if (UIControl->upPress[i] && UIControl->downPress[i]) 
            {
                UIControl->upPress[i]   = false;
                UIControl->downPress[i] = false;
            }
            if (UIControl->leftPress[i] && UIControl->rightPress[i]) 
            {
                UIControl->leftPress[i]  = false;
                UIControl->rightPress[i] = false;
            }

            UIControl->yPress[i] = ControllerInfo[i + 1].keyY.press;
            UIControl->xPress[i] = ControllerInfo[i + 1].keyX.press;
#if RETRO_USE_PLUS
            UIControl->startPress[i] = ControllerInfo[i + 1].keyStart.press;
#endif
            if (API_GetConfirmButtonFlip()) {
                UIControl->confirmPress[i] = ControllerInfo[i + 1].keyStart.press || ControllerInfo[i + 1].keyB.press;
                UIControl->backPress[i]    = ControllerInfo[i + 1].keyA.press;
            }
            else {
                UIControl->confirmPress[i] = ControllerInfo[i + 1].keyStart.press || ControllerInfo[i + 1].keyA.press;
                UIControl->backPress[i]    = ControllerInfo[i + 1].keyB.press;
            }
        }

        UIControl->keyUp    = ControllerInfo->keyUp.press || AnalogStickInfoL->keyUp.press;
        UIControl->keyDown  = ControllerInfo->keyDown.press || AnalogStickInfoL->keyDown.press;
        UIControl->keyLeft  = ControllerInfo->keyLeft.press || AnalogStickInfoL->keyLeft.press;
        UIControl->keyRight = ControllerInfo->keyRight.press || AnalogStickInfoL->keyRight.press;
        UIControl->keyY     = ControllerInfo->keyY.press;
        UIControl->keyX     = ControllerInfo->keyX.press;
#if RETRO_USE_PLUS
        UIControl->keyStart = ControllerInfo->keyStart.press;
#endif
        if (API_GetConfirmButtonFlip()) {
            UIControl->keyConfirm    = ControllerInfo->keyStart.press || ControllerInfo->keyB.press;
            UIControl->keyBack = ControllerInfo->keyA.press;
        }
        else {
            UIControl->keyConfirm    = ControllerInfo->keyStart.press || ControllerInfo->keyA.press;
            UIControl->keyBack = ControllerInfo->keyB.press;
        }
#if RETRO_USE_PLUS
        UIControl->keyBack |= UnknownInfo->field_10;
#else

#endif
        UIControl->keyBack |= UIControl->flagA;

        if (UIControl->keyBack) {
            UIControl->keyConfirm = false;
            UIControl->keyY    = false;
        }
        if (UIControl->keyConfirm) {
            UIControl->keyY = false;
        }
        UIControl->inputLocked = true;
    }

    if (!self->selectionDisabled) {
        bool32 flag = false;
        if (UIControl->keyBack) {
            if (!self->childHasFocus && !self->dialogHasFocus
#if RETRO_USE_PLUS
                && !self->popoverHasFocus 
#endif
                && self->backoutTimer <= 0) {
                if (self->backPressCB) {
                    flag = self->backPressCB();
                    if (!flag) {
                        UIControl->keyBack = false;
                    }
                    else {
                        if (self->buttons[self->activeEntityID])
                            self->buttons[self->activeEntityID]->flag = false;
                    }
                }
                else {
                    if (self->parentTag.textLength <= 0) {
                        UIControl->keyBack = false;
                    }
                    else {
                        self->selectionDisabled = true;
                        UITransition_StartTransition(UIControl_Unknown13, 0);
                        flag = false;

                        if (self->buttons[self->activeEntityID])
                            self->buttons[self->activeEntityID]->flag = false;
                    }
                }

                if (flag)
                    return;
            }
#if RETRO_USE_PLUS
            else {
                LogHelpers_Print("Backout prevented");
                LogHelpers_Print("childHasFocus = %d", self->childHasFocus);
                LogHelpers_Print("dialogHasFocus = %d", self->dialogHasFocus);
                LogHelpers_Print("popoverHasFocus = %d", self->popoverHasFocus);
                LogHelpers_Print("backoutTimer = %d", self->backoutTimer);
            }
#endif
        }

        if (self->processButtonInputCB)
            self->processButtonInputCB();
        else
            UIControl_ProcessButtonInput();

        if (!self->selectionDisabled) {
            if (UIControl->keyY) {
                if (!self->childHasFocus
                    && !self->dialogHasFocus
#if RETRO_USE_PLUS
                    && !self->popoverHasFocus
#endif
                    && self->backoutTimer <= 0) {
                    StateMachine_Run(self->yPressCB);
                }
                UIControl->keyY = false;
            }
            if (UIControl->keyX) {
                if (!self->childHasFocus 
                    && !self->dialogHasFocus
#if RETRO_USE_PLUS
                    && !self->popoverHasFocus 
#endif
                    && self->backoutTimer <= 0) {
                    StateMachine_Run(self->xPressCB);
                }
                UIControl->keyX = false;
            }
        }
    }
}

int32 UIControl_Unknown1(EntityUIControl *control, EntityUIButton *entity)
{
    for (int32 i = 0; i < control->buttonCount; ++i) {
        if (entity == control->buttons[i])
            return i;
    }
    return -1;
}

void UIControl_Unknown2(EntityUIControl *control)
{
    Entity *storeEntity = SceneInfo->entity;
    for (int32 i = 0; i < SCENEENTITY_COUNT; ++i) {
        EntityUIButton *entity = RSDK.GetEntityByID(i);
        if (entity) {
            int32 h          = ScreenInfo->height;
            int32 centerX    = -ScreenInfo->width >> 1;
            int32 centerX2   = ScreenInfo->width >> 1;
            int32 centerY    = h >> 1;
            int32 negCenterY = -h >> 1;

            int32 x = centerX2;
            int32 y = centerY;
            if (centerX < centerX2)
                x = centerX;
            if (centerX2 > centerX)
                centerX = centerX2;
            if (negCenterY < centerY)
                y = negCenterY;
            if (centerY > negCenterY)
                negCenterY = centerY;
            if (entity->position.x >= control->position.x + (x << 16) && entity->position.x <= control->position.x + (centerX << 16)) {
                if (entity->position.y >= control->position.y + (y << 16) && entity->position.y <= control->position.y + (negCenterY << 16)) {
                    int32 id          = RSDK.GetEntityID(entity);
                    SceneInfo->entity = (Entity *)entity;
                    if (UIButton && entity->objectID == UIButton->objectID) {
                        UIButton_ManageChoices(entity);
                        UIButton_Update();
                    }
                    else if (UIChoice && entity->objectID == UIChoice->objectID) {
                        UIChoice_Update();
                    }
                    else if (UITAZoneModule && entity->objectID == UITAZoneModule->objectID) {
                        UITAZoneModule_Update();
                    }
#if RETRO_USE_PLUS
                    else if (UIReplayCarousel && entity->objectID == UIReplayCarousel->objectID) {
                        UIReplayCarousel_Update();
                    }
#endif
                    else if (UIModeButton && entity->objectID == UIModeButton->objectID) {
#if RETRO_USE_PLUS
                        UIModeButton_Update();
#else
                        EntityUIModeButton *modeButton = (EntityUIModeButton *)SceneInfo->entity;
                        modeButton->touchPosStart.x    = 0xB80000;
                        modeButton->touchPosStart.y    = 0x3E0000;
                        modeButton->touchPosEnd.x      = 0;
                        modeButton->touchPosEnd.y      = -0x120000;
                        if (modeButton->textSpriteIndex != UIWidgets->textSpriteIndex || modeButton->wasDisabled != modeButton->disabled) {
                            UIModeButton_Unknown1();
                            modeButton->textSpriteIndex = UIWidgets->textSpriteIndex;
                            modeButton->wasDisabled     = modeButton->disabled;
                        }
#endif
                    }
                    else if (UIVsZoneButton && entity->objectID == UIVsZoneButton->objectID) {
                        UIVsZoneButton_Update();
                    }
                    else if (UIHeading && entity->objectID == UIHeading->objectID) {
                        EntityUIHeading *heading = (EntityUIHeading *)entity;
                        if (heading->textSpriteIndex != UIHeading->textSpriteIndex) {
                            RSDK.SetSpriteAnimation(UIHeading->textSpriteIndex, heading->headingID, &heading->animator, true, 0);
                            heading->textSpriteIndex = UIHeading->textSpriteIndex;
                        }
                        StateMachine_Run(heading->state);
                    }
                    if (entity->visible)
                        RSDK.AddDrawListRef(entity->drawOrder, id);
                    SceneInfo->entity = storeEntity;
                }
            }
        }
    }
}

#if RETRO_USE_PLUS
void UIControl_Unknown3(EntityUIControl *entity)
{
    for (int32 i = 0; i < entity->promptCount; ++i) {
        entity->prompts[i]->active = ACTIVE_NORMAL;
    }
}
#endif

void UIControl_Unknown4(EntityUIControl *entity)
{
#if RETRO_USE_PLUS
    RSDK.PrintText(PRINT_NORMAL, &entity->tag);
#endif
    entity->active  = ACTIVE_ALWAYS;
    entity->visible = true;
    if (entity->dwordC4) {
        entity->activeEntityID = entity->storedEntityID;
        entity->storedEntityID = 0;
        entity->dwordC4        = 0;
    }
    else if (entity->resetSelection) {
        entity->activeEntityID = entity->startingID;
    }

    RSDK.ClearCameras();
    RSDK.AddCamera(&entity->position, ScreenInfo->width << 16, ScreenInfo->height << 16, true);
    UIControl_Unknown2(entity);
    if (!entity->childHasFocus && (entity->resetSelection || !entity->dwordCC)) {
        entity->position.x   = entity->startPos.x;
        entity->position.y   = entity->startPos.y;
        entity->posUnknown.x = entity->startPos.x;
        entity->posUnknown.y = entity->startPos.y;
    }
    entity->state         = UIControl_ProcessInputs;
    entity->childHasFocus = false;
    entity->dwordCC       = 1;

#if RETRO_USE_PLUS
    for (int32 i = 0; i < entity->promptCount; ++i) {
        entity->prompts[i]->active = ACTIVE_NORMAL;
    }
#endif
    if (entity->unknownCallback3) {
        Entity *storeEntity    = SceneInfo->entity;
        SceneInfo->entity = (Entity *)entity;
        entity->unknownCallback3();
        SceneInfo->entity = storeEntity;
    }
}

void UIControl_Unknown5(EntityUIControl *entity)
{
    entity->active  = ACTIVE_ALWAYS;
    entity->visible = true;
    if (!entity->dialogHasFocus
#if RETRO_USE_PLUS
        && !entity->popoverHasFocus
#endif
        && !entity->childHasFocus) {
        if (entity->dwordC4) {
            entity->activeEntityID = entity->storedEntityID;
            entity->storedEntityID = 0;
            entity->dwordC4        = 0;
        }
        else if (entity->resetSelection) {
            entity->activeEntityID = entity->startingID;
#if !RETRO_USE_PLUS
            entity->position.x   = entity->startPos.x;
            entity->position.y   = entity->startPos.y;
            entity->posUnknown.x = entity->startPos.x;
            entity->posUnknown.y = entity->startPos.y;
            entity->state        = UIControl_ProcessInputs;
#endif
        }

#if RETRO_USE_PLUS
        if (entity->resetSelection || !entity->dwordCC) {
            entity->position.x   = entity->startPos.x;
            entity->position.y   = entity->startPos.y;
            entity->posUnknown.x = entity->startPos.x;
            entity->posUnknown.y = entity->startPos.y;
        }
        entity->dwordCC = 1;
        UIControl_Unknown3(entity);
#endif
        entity->state = UIControl_ProcessInputs;
    }
}

void UIControl_Unknown6(EntityUIControl *control)
{
    RSDK_THIS(UIControl);
    UIControl->field_C8 = 0;
    control->active     = ACTIVE_NEVER;
    control->visible    = false;
    control->state      = StateMachine_None;
#if RETRO_USE_PLUS
    if (self->promptCount) {
        for (int32 i = 0; i < control->promptCount; ++i) {
            control->prompts[i]->active = ACTIVE_BOUNDS;
        }
    }
#endif
}

void UIControl_Unknown7(void)
{
    RSDK_THIS(UIControl);
    int32 slotID = RSDK.GetEntityID(self);
    Hitbox bounds;

    if (UIHeading && slotID != SLOT_DIALOG_UICONTROL) {
        foreach_all(UIHeading, heading)
        {
            int32 x         = self->startPos.x - self->cameraOffset.x;
            int32 y         = self->startPos.y - self->cameraOffset.y;
            bounds.right  = self->size.x >> 17;
            bounds.left   = -(self->size.x >> 17);
            bounds.bottom = self->size.y >> 17;
            bounds.top    = -(self->size.y >> 17);
            if (MathHelpers_PointInHitbox(FLIP_NONE, x, y, &bounds, heading->position.x, heading->position.y))
                self->heading = (Entity *)heading;
        }
    }

#if RETRO_USE_PLUS
    if (UIShifter && slotID != SLOT_DIALOG_UICONTROL) {
        foreach_all(UIShifter, shifter)
        {
            int32 x         = self->startPos.x - self->cameraOffset.x;
            int32 y         = self->startPos.y - self->cameraOffset.y;
            bounds.right  = self->size.x >> 17;
            bounds.left   = -(self->size.x >> 17);
            bounds.bottom = (self->size.y >> 17);
            bounds.top    = -(self->size.y >> 17);
            if (MathHelpers_PointInHitbox(FLIP_NONE, x, y, &bounds, shifter->position.x, shifter->position.y)) {
                self->shifter = (Entity *)shifter;
                shifter->parent = self;
            }
        }
    }

    if (UICarousel && slotID != SLOT_DIALOG_UICONTROL) {
        foreach_all(UICarousel, carousel)
        {
            int32 x         = self->startPos.x - self->cameraOffset.x;
            int32 y         = self->startPos.y - self->cameraOffset.y;
            bounds.right  = self->size.x >> 17;
            bounds.left   = -(self->size.x >> 17);
            bounds.bottom = self->size.y >> 17;
            bounds.top    = -(self->size.y >> 17);
            if (MathHelpers_PointInHitbox(FLIP_NONE, x, y, &bounds, carousel->position.x, carousel->position.y)) {
                self->carousel = carousel;
                carousel->parent = (Entity *)self;
            }
        }
    }
#endif

    for (int32 i = 0; i < SCENEENTITY_COUNT; ++i) {
        EntityUIButton *button = RSDK_GET_ENTITY(i, UIButton);
        if (button) {
            int32 id = button->objectID;
            if (id != UIButton->objectID && (!UIModeButton || id != UIModeButton->objectID) && (!UISaveSlot || id != UISaveSlot->objectID)
                && (!UICharButton || id != UICharButton->objectID) && (!UITAZoneModule || id != UITAZoneModule->objectID)
#if RETRO_USE_PLUS
                && (!UIRankButton || id != UIRankButton->objectID) && (!UIReplayCarousel || id != UIReplayCarousel->objectID)
#endif
                && (!UILeaderboard || id != UILeaderboard->objectID) && (!UIVsCharSelector || id != UIVsCharSelector->objectID)
                && (!UIVsZoneButton || id != UIVsZoneButton->objectID) && (!UIVsResults || id != UIVsResults->objectID)
                && (!UISlider || id != UISlider->objectID) && (!UIKeyBinder || id != UIKeyBinder->objectID)) {
            }
            else {
                int32 x         = self->startPos.x - self->cameraOffset.x;
                int32 y         = self->startPos.y - self->cameraOffset.y;
                bounds.left   = -(self->size.x >> 17);
                bounds.top    = -(self->size.y >> 17);
                bounds.right  = self->size.x >> 17;
                bounds.bottom = self->size.y >> 17;
                if (MathHelpers_PointInHitbox(FLIP_NONE, x, y, &bounds, button->position.x, button->position.y)) {
                    if (self->buttonCount < 64) {
                        if (!button->parent)
                            button->parent = (Entity *)self;
                        self->buttons[self->buttonCount++] = button;
                    }
                }
            }
        }
    }
}

bool32 UIControl_Unknown9(EntityUIControl *entity)
{
    if (entity->scrollSpeed.x && entity->scrollSpeed.y) {
        return entity->position.x != entity->posUnknown.x || entity->position.y != entity->posUnknown.y;
    }
    else if (entity->scrollSpeed.x) {
        return entity->position.x != entity->posUnknown.x;
    }
    else if (entity->scrollSpeed.y) {
        return entity->position.y != entity->posUnknown.y;
    }
    return false;
}

void UIControl_MatchMenuTag(const char *text)
{
    TextInfo info;
    info.text       = NULL;
    info.length     = 0;
    info.textLength = 0;
    RSDK.PrependText(&info, text);
    foreach_all(UIControl, entity)
    {
        if (entity->active == ACTIVE_ALWAYS) {
            UIControl_Unknown6(entity);
        }
        else if (RSDK.StringCompare(&info, &entity->tag, false)) {
            UIControl_Unknown4(entity);
        }
        else {
            UIControl_Unknown6(entity);
        }
    }
}

void UIControl_Unknown11(TextInfo *info)
{
    if (info->textLength) {
        foreach_all(UIControl, entity)
        {
            if (entity->active == ACTIVE_ALWAYS || !RSDK.StringCompare(info, &entity->tag, false))
                UIControl_Unknown6(entity);
            else
                UIControl_Unknown4(entity);
        }
    }
}

void UIControl_Unknown12(Entity *control)
{
    foreach_all(UIControl, entity)
    {
        if (entity->active == ACTIVE_ALWAYS) {
            UIControl_Unknown6(entity);
        }
        else if (entity == (EntityUIControl *)control) {
            UIControl_Unknown5(entity);
        }
        else {
            UIControl_Unknown6(entity);
        }
    }
}

void UIControl_Unknown13(void)
{
    EntityUIControl *entity   = UIControl_GetUIControl();
    entity->selectionDisabled = false;
    UIControl_Unknown11(&entity->parentTag);
}

void UIControl_Unknown15(EntityUIControl *entity, int32 x, int32 y)
{
    int32 finalX = x;
    if (!x) {
        finalX = entity->position.x;
        x      = entity->position.x;
    }
    int32 finalY = y;
    if (!y) {
        finalY = entity->position.y;
        y      = entity->position.y;
    }

#if RETRO_USE_PLUS
    if (!entity->noClamp) {
#endif
        int32 startX = entity->startPos.x - entity->cameraOffset.x;
        int32 startY = entity->startPos.y - entity->cameraOffset.y;
        int32 x1     = startX + (ScreenInfo->width << 15) - (entity->size.x >> 1);
        int32 x2     = startX + (entity->size.x >> 1) - (ScreenInfo->width << 15);
        int32 y1     = startY + (ScreenInfo->height << 15) - (entity->size.y >> 1);
        int32 y2     = startY + (entity->size.y >> 1) - (ScreenInfo->height << 15);
        if (x < x2)
            x2 = x;
        finalX = x2;
        if (x1 > x2)
            finalX = x1;

        if (y < y2)
            y2 = y;
        finalY = y2;
        if (y1 > y2)
            finalY = y1;
#if RETRO_USE_PLUS
    }
#endif
    entity->posUnknown.x = finalX;
    entity->posUnknown.y = finalY;
}

void UIControl_Unknown16(void)
{
    RSDK_THIS(UIControl);
    if (self->position.x < self->posUnknown.x) {
        self->position.x += self->scrollSpeed.x;
        if (self->position.x > self->posUnknown.x)
            self->position.x = self->posUnknown.x;
    }
    if (self->position.x > self->posUnknown.x) {
        self->position.x -= self->scrollSpeed.x;
        if (self->position.x < self->posUnknown.x)
            self->position.x = self->posUnknown.x;
    }

    if (self->position.y < self->posUnknown.y) {
        self->position.y += self->scrollSpeed.y;
        if (self->position.y > self->posUnknown.y)
            self->position.y = self->posUnknown.y;
    }
    if (self->position.y > self->posUnknown.y) {
        self->position.y -= self->scrollSpeed.y;
        if (self->position.y < self->posUnknown.y)
            self->position.y = self->posUnknown.y;
    }
}

void UIControl_ProcessButtonInput(void)
{
    RSDK_THIS(UIControl);

    bool32 flag = false;
    if (TouchInfo->count || UIControl->field_C8) {
        EntityUIButton *activeButton = 0;
        UIControl->field_C8          = TouchInfo->count != 0;
        UIControl->field_4           = 1;

        for (int32 i = 0; i < self->buttonCount; ++i) {
            if (self->buttons[i]) {
                EntityUIButton *button = self->buttons[i];

                Entity *storeEntity    = SceneInfo->entity;
                SceneInfo->entity = (Entity *)button;
                if (button->touchCB && !self->dialogHasFocus
#if RETRO_USE_PLUS
                    && !self->popoverHasFocus
#endif
                    ) {
                    if (!button->options8 || !button->options8()) {
                        if (flag || button->touchCB()) {
                            flag = true;
                            if (button->touchCB && !activeButton)
                                activeButton = button;
                        }
                        else {
                            flag = false;
                        }
                    }
                }
                SceneInfo->entity = storeEntity;
            }
        }

        if (TouchInfo->count) {
            if (flag) {
                int32 id = -1;
                for (int32 i = 0; i < self->buttonCount; ++i) {
                    if (activeButton == self->buttons[i]) {
                        id = i;
                        break;
                    }
                }

                self->activeEntityID = id;
                if (activeButton->flag) {
                    Entity *storeEntity    = SceneInfo->entity;
                    SceneInfo->entity = (Entity *)activeButton;
                    activeButton->flag     = true;
                    StateMachine_Run(activeButton->options5);
                    SceneInfo->entity = storeEntity;
                }
            }
            else {
                self->activeEntityID = -1;
            }
        }

        UIControl->field_4 = 0;
    }

    if (self->activeEntityID >= 0) {
        if (self->activeEntityID < self->buttonCount) {
            EntityUIButton *button = self->buttons[self->activeEntityID];
            if (button) {
                Entity *storeEntity    = SceneInfo->entity;
                SceneInfo->entity = (Entity *)button;
                if (button->processButtonCB) {
                    if (!button->options8 || !button->options8())
                        button->processButtonCB();
                }
                SceneInfo->entity = storeEntity;
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void UIControl_EditorDraw(void)
{
    RSDK_THIS(UIControl);
    self->updateRange.x = self->size.x >> 1;
    self->updateRange.y = self->size.y >> 1;

    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->size.x >> 1;
    drawPos.y -= self->size.y >> 1;
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x + self->size.x, drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, self->size.y + drawPos.y, drawPos.x + self->size.x, self->size.y + drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x, drawPos.y + self->size.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + self->size.x, drawPos.y, drawPos.x + self->size.x, drawPos.y + self->size.y, 0xFFFF00, 0, INK_NONE, false);

    RSDK.SetSpriteAnimation(UIControl->aniFrames, 0, &self->animator, false, 7);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void UIControl_EditorLoad(void) { UIControl->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void UIControl_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIControl, VAR_STRING, tag);
    RSDK_EDITABLE_VAR(UIControl, VAR_STRING, parentTag);
    RSDK_EDITABLE_VAR(UIControl, VAR_BOOL, activeOnLoad);
    RSDK_EDITABLE_VAR(UIControl, VAR_BOOL, noWidgets);
    RSDK_EDITABLE_VAR(UIControl, VAR_BOOL, resetSelection);
    RSDK_EDITABLE_VAR(UIControl, VAR_UINT8, rowCount);
    RSDK_EDITABLE_VAR(UIControl, VAR_UINT8, columnCount);
    RSDK_EDITABLE_VAR(UIControl, VAR_UINT8, startingID);
    RSDK_EDITABLE_VAR(UIControl, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIControl, VAR_VECTOR2, cameraOffset);
    RSDK_EDITABLE_VAR(UIControl, VAR_VECTOR2, scrollSpeed);
#if RETRO_USE_PLUS
    RSDK_EDITABLE_VAR(UIControl, VAR_BOOL, noClamp);
    RSDK_EDITABLE_VAR(UIControl, VAR_BOOL, noWrap);
#endif
}
