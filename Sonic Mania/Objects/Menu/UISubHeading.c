// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UISubHeading Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectUISubHeading *UISubHeading;

void UISubHeading_Update(void)
{
    RSDK_THIS(UISubHeading);

    if (self->textFrames != UIWidgets->textFrames || self->storedListID != self->listID || self->storedFrameID != self->frameID) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator, true, self->frameID);
        self->textFrames = UIWidgets->textFrames;
        self->storedListID    = self->listID;
        self->storedFrameID   = self->frameID;
    }

    StateMachine_Run(self->state);
}

void UISubHeading_LateUpdate(void) {}

void UISubHeading_StaticUpdate(void) {}

void UISubHeading_Draw(void)
{
    RSDK_THIS(UISubHeading);
    Vector2 drawPos;

    int32 size  = self->size.y + self->size.x;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    UIWidgets_DrawParallelogram(self->size.y, size >> 16, self->shiftedY, 0, 0, 0, self->position.x, self->position.y);

    drawPos = self->position;
    if (!self->align) {
        drawPos.x += -0x60000 - (self->size.x >> 1);
    }
    else if (self->align == 2) {
        drawPos.x -= 0x60000;
        drawPos.x += self->size.x >> 1;
    }
    drawPos.x += self->offset;
    RSDK.DrawSprite(&self->animator, &drawPos, false);
}

void UISubHeading_Create(void *data)
{
    RSDK_THIS(UISubHeading);
    if (!SceneInfo->inEditor) {
        self->offset <<= 16;
        self->visible       = true;
        self->drawOrder     = 2;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x400000;
        self->shiftedY      = self->size.y >> 16;
        self->size.y        = abs(self->size.y);
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator, true, self->frameID);
        self->textFrames = UIWidgets->textFrames;
    }
}

void UISubHeading_StageLoad(void) {}

// ???
// Why is this all here
// Wouldn't it make more sense to make a "SaveSelectMenu" object like options, extras, time attack, competition and main menus have?

#if RETRO_USE_PLUS
void UISubHeading_Initialize(void)
{
    TextInfo tag;
    INIT_TEXTINFO(tag);

    foreach_all(UIControl, control)
    {
        RSDK.PrependText(&tag, "Save Select");
        if (RSDK.StringCompare(&tag, &control->tag, false))
            ManiaModeMenu->saveSelectMenu = (Entity *)control;

        RSDK.PrependText(&tag, "Encore Mode");
        if (RSDK.StringCompare(&tag, &control->tag, false))
            ManiaModeMenu->encoreSaveSelect = (Entity *)control;

        RSDK.PrependText(&tag, "No Save Mode");
        if (RSDK.StringCompare(&tag, &control->tag, false))
            ManiaModeMenu->noSaveMenu = (Entity *)control;

        RSDK.PrependText(&tag, "No Save Encore");
        if (RSDK.StringCompare(&tag, &control->tag, false))
            ManiaModeMenu->noSaveMenuEncore = (Entity *)control;

        RSDK.PrependText(&tag, "Secrets");
        if (RSDK.StringCompare(&tag, &control->tag, false))
            ManiaModeMenu->secretsMenu = (Entity *)control;
    }
}

void UISubHeading_HandleUnlocks(void)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->secretsMenu;
    EntityUIButton *button   = control->buttons[1];
    button->disabled         = !GameProgress_CheckUnlock(5) && globals->superSecret;
    if (button->disabled)
        UIButton_ManageChoices(button);

    button                  = control->buttons[2];
    EntityUIButton *option1 = UIButton_GetChoicePtr(button, 1);
    EntityUIButton *option2 = UIButton_GetChoicePtr(button, 2);
    int32 unlock              = GameProgress_CheckUnlock(2);
    button->disabled        = !unlock;
    if (button->disabled)
        UIButton_ManageChoices(button);
    option1->disabled = !GameProgress_CheckUnlock(2);
    option2->disabled = !GameProgress_CheckUnlock(3);

    button           = control->buttons[3];
    unlock           = GameProgress_CheckUnlock(4);
    button->disabled = !unlock;
    if (button->disabled)
        UIButton_ManageChoices(button);
}

void UISubHeading_SetupActions(void)
{
    foreach_all(UISaveSlot, slot) { slot->actionCB = UISubHeading_SaveButton_ActionCB; }

    foreach_all(UIButtonPrompt, prompt)
    {
        Hitbox hitbox;
        EntityUIControl *saveSel = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
        hitbox.right             = saveSel->size.x >> 17;
        hitbox.left              = -(saveSel->size.x >> 17);
        hitbox.bottom            = saveSel->size.y >> 17;
        hitbox.top               = -(saveSel->size.y >> 17);

        if (MathHelpers_PointInHitbox(FLIP_NONE, saveSel->startPos.x - saveSel->cameraOffset.x, saveSel->startPos.y - saveSel->cameraOffset.y,
                                      &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 2) {
            ManiaModeMenu->prompt1 = (Entity *)prompt;
        }
        else {
            saveSel = (EntityUIControl *)ManiaModeMenu->encoreSaveSelect;

            hitbox.right  = saveSel->size.x >> 17;
            hitbox.left   = -(saveSel->size.x >> 17);
            hitbox.bottom = saveSel->size.y >> 17;
            hitbox.top    = -(saveSel->size.y >> 17);
            if (MathHelpers_PointInHitbox(FLIP_NONE, saveSel->startPos.x - saveSel->cameraOffset.x, saveSel->startPos.y - saveSel->cameraOffset.y,
                                          &hitbox, prompt->position.x, prompt->position.y)
                && prompt->buttonID == 2)
                ManiaModeMenu->prompt2 = (Entity *)prompt;
        }
    }

    EntityUIControl *saveSel  = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
    saveSel->menuUpdateCB    = UISubHeading_SaveSel_MenuUpdateCB;
    saveSel->yPressCB         = UISubHeading_SaveSel_YPressCB;

    EntityUIControl *saveSelEncore  = (EntityUIControl *)ManiaModeMenu->encoreSaveSelect;
    saveSelEncore->menuUpdateCB    = UISubHeading_SaveSel_MenuUpdateCB;
}

void UISubHeading_HandleMenuReturn(int32 slot)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->secretsMenu;
    EntitySaveGame *saveGame  = (EntitySaveGame *)SaveGame_GetDataPtr(slot, false);

    UIButton_SetChoiceSelection(control->buttons[0], (saveGame->medalMods & getMod(MEDAL_NOTIMEOVER)) != 0);
    UIButton_SetChoiceSelection(control->buttons[1], (saveGame->medalMods & getMod(MEDAL_ANDKNUCKLES)) != 0);

    int32 medals = saveGame->medalMods;
    if (medals & getMod(MEDAL_NODROPDASH)) {
        if (medals & getMod(MEDAL_PEELOUT)) {
            UIButton_SetChoiceSelection(control->buttons[2], 1);
        }
        else if (medals & getMod(MEDAL_INSTASHIELD)) {
            UIButton_SetChoiceSelection(control->buttons[2], 2);
        }
    }
    else {
        UIButton_SetChoiceSelection(control->buttons[2], 0);
    }

    if (saveGame->medalMods & getMod(MEDAL_ANDKNUCKLES))
        UIButton_SetChoiceSelection(control->buttons[3], 1);
    else
        UIButton_SetChoiceSelection(control->buttons[3], 0);
}

int32 UISubHeading_GetMedalMods(void)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->secretsMenu;

    int32 mods = 0;
    if (control->buttons[0]->selection == 1)
        mods |= getMod(MEDAL_NOTIMEOVER);

    if (control->buttons[1]->selection == 1)
        mods |= getMod(MEDAL_DEBUGMODE);

    if (control->buttons[2]->selection == 1) {
        mods |= getMod(MEDAL_NODROPDASH);
        mods |= getMod(MEDAL_PEELOUT);
    }
    else if (control->buttons[2]->selection == 2) {
        mods |= getMod(MEDAL_NODROPDASH);
        mods |= getMod(MEDAL_INSTASHIELD);
    }

    if (control->buttons[3]->selection == 1)
        mods |= getMod(MEDAL_ANDKNUCKLES);

    return mods;
}

void UISubHeading_SaveFileCB(int32 status)
{
    UIWaitSpinner_FinishWait();
    RSDK.LoadScene();
}

void UISubHeading_SecretsTransitionCB(void)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
    control->childHasFocus   = true;
    UIControl_MatchMenuTag("Secrets");
}

void UISubHeading_LeaveSecretsMenu(void)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
    if (ManiaModeMenu->inSecretsMenu) {
        EntityUISaveSlot *slot = (EntityUISaveSlot *)control->buttons[control->lastButtonID];
        UISubHeading_HandleMenuReturn(slot->slotID);
        ManiaModeMenu->inSecretsMenu = false;
    }
}

void UISubHeading_SaveSel_MenuUpdateCB(void)
{
    RSDK_THIS(UIControl);
    if (self->active == ACTIVE_ALWAYS) {
        EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)ManiaModeMenu->prompt1;
        if (self == (EntityUIControl *)ManiaModeMenu->encoreSaveSelect) {
            prompt = (EntityUIButtonPrompt *)ManiaModeMenu->prompt2;
        }
        else if (self->lastButtonID != ManiaModeMenu->saveSelLastButtonID) {
            UISubHeading_LeaveSecretsMenu();
            ManiaModeMenu->saveSelLastButtonID = self->lastButtonID;
        }

        bool32 flag  = false;
        bool32 flag2 = false;
        for (int32 i = 0; i < self->buttonCount; ++i) {
            flag2 |= self->buttons[i]->state == UISaveSlot_State_Selected;
            if (self->lastButtonID >= 0) {
                if (self->buttons[i] == self->buttons[self->lastButtonID]) {
                    EntityUISaveSlot *slot = (EntityUISaveSlot *)self->buttons[self->lastButtonID];
                    if (!slot->isNewSave)
                        flag = true;
                }
            }
        }

        if (!flag2) {
            if ((self == (EntityUIControl *)ManiaModeMenu->saveSelectMenu && self->lastButtonID == 8)
                || (self == (EntityUIControl *)ManiaModeMenu->encoreSaveSelect && !self->lastButtonID)) {
                prompt->visible = false;
            }
            else {
                prompt->visible = flag;
            }
        }
    }
}

void UISubHeading_SaveSel_YPressCB(void)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
    if (control->active == ACTIVE_ALWAYS) {
        if (!ManiaModeMenu->inSecretsMenu) {
            UISubHeading_HandleMenuReturn(((EntityUISaveSlot*)control->buttons[control->buttonID])->slotID);
            ManiaModeMenu->inSecretsMenu = true;
        }
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIControl->inputLocked = true;

        UITransition_StartTransition(UISubHeading_SecretsTransitionCB, 0);
    }
}

void UISubHeading_SaveButton_ActionCB(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    RSDK_THIS(UISaveSlot);
    EntityUIControl *control = (EntityUIControl *)self->parent;

    EntitySaveGame *saveRAM = (EntitySaveGame *)SaveGame_GetDataPtr(self->slotID, self->encoreMode);
    TimeAttackData_Clear();
    RSDK.GetCString(param->menuTag, &control->tag);
    param->selectionID = control->lastButtonID;
    param->replayID    = 0;
    globals->gameMode = self->encoreMode != false;

    bool32 loadingSave = false;
    if (self->type) {
        memset(globals->noSaveSlot, 0, 0x400);
        globals->continues  = 0;
        globals->saveSlotID = NO_SAVE_SLOT;
    }
    else {
        globals->saveSlotID = self->slotID;
        globals->medalMods  = 0;
        if (self->isNewSave) {
            int32 *saveData = SaveGame_GetDataPtr(self->slotID % 8, self->encoreMode);

            memset(saveData, 0, 0x400);
            if (globals->gameMode != MODE_ENCORE)
                saveRAM->saveState = 1;
            saveRAM->characterID   = self->frameID;
            saveRAM->zoneID        = 0;
            saveRAM->lives         = 3;
            saveRAM->chaosEmeralds = self->saveEmeralds;
            saveRAM->continues     = 0;
            UIWaitSpinner_StartWait();
            loadingSave = true;
            SaveGame_SaveFile(UISubHeading_SaveFileCB);
        }
        else {
            if (saveRAM->saveState == 2) {
                saveRAM->collectedSpecialRings = 0;
                saveRAM->score                 = 0;
                saveRAM->score1UP              = 500000;
            }
            loadingSave = true;
            SaveGame_SaveFile(UISubHeading_SaveFileCB);
        }
    }

    if (self->encoreMode) {
        globals->medalMods = getMod(MEDAL_NOTIMEOVER);
        saveRAM->medalMods = globals->medalMods;
    }
    else {
        globals->medalMods = UISubHeading_GetMedalMods();
        saveRAM->medalMods = globals->medalMods;
        switch (self->frameID) {
            case 0:
            case 1: globals->playerID = ID_SONIC; break;
            case 2: globals->playerID = ID_TAILS; break;
            case 3: globals->playerID = ID_KNUCKLES; break;
            case 4: globals->playerID = ID_MIGHTY; break;
            case 5: globals->playerID = ID_RAY; break;
            default: break;
        }

        if ((globals->medalMods & getMod(MEDAL_ANDKNUCKLES))) {
            globals->playerID |= ID_KNUCKLES_ASSIST;
        }
        else if (!self->frameID) {
            globals->playerID |= ID_TAILS_ASSIST;
        }
    }

    if (self->type == 1 || self->isNewSave) {
        if (self->encoreMode) {
            globals->playerID          = ID_SONIC;
            globals->stock             = 0;
            globals->characterFlags    = 0;
            globals->enableIntro       = true;
            globals->suppressTitlecard = true;
            RSDK.SetScene("Cutscenes", "Angel Island Zone Encore");
        }
        else if (((globals->medalMods & getMod(MEDAL_DEBUGMODE)) && (ControllerInfo->keyC.down || ControllerInfo->keyX.down)) && self->type == 1) {
            RSDK.SetScene("Presentation", "Level Select");
        }
        else {
            RSDK.SetScene("Cutscenes", "Angel Island Zone");
        }
    }
    else if (self->encoreMode) {
        globals->playerID       = saveRAM->playerID;
        globals->stock          = saveRAM->stock;
        globals->characterFlags = saveRAM->characterFlags;
        RSDK.SetScene("Encore Mode", "");
        SceneInfo->listPos += TimeAttackData_GetEncoreListPos(self->saveZoneID, 0, self->frameID);
    }
    else {
        RSDK.SetScene("Mania Mode", "");
        SceneInfo->listPos += TimeAttackData_GetManiaListPos(self->saveZoneID, 0, self->frameID);
    }

    if (!loadingSave) {
        globals->initCoolBonus = false;
        RSDK.LoadScene();
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void UISubHeading_EditorDraw(void)
{
    RSDK_THIS(UISubHeading);

    if (self->offset < 0x10000)
        self->offset <<= 16;
    self->drawOrder     = 2;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->shiftedY      = self->size.y >> 16;
    self->size.y        = abs(self->size.y);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator, true, self->frameID);
    self->textFrames = UIWidgets->textFrames;

    UISubHeading_Draw();
}

void UISubHeading_EditorLoad(void) {}
#endif

void UISubHeading_Serialize(void)
{
    RSDK_EDITABLE_VAR(UISubHeading, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UISubHeading, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UISubHeading, VAR_ENUM, frameID);
    RSDK_EDITABLE_VAR(UISubHeading, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UISubHeading, VAR_ENUM, offset);
}
