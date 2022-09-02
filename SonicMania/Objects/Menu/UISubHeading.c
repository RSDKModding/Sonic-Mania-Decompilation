// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UISubHeading Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUISubHeading *UISubHeading;

void UISubHeading_Update(void)
{
    RSDK_THIS(UISubHeading);

    if (self->textFrames != UIWidgets->textFrames || self->storedListID != self->listID || self->storedFrameID != self->frameID) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator, true, self->frameID);
        self->textFrames    = UIWidgets->textFrames;
        self->storedListID  = self->listID;
        self->storedFrameID = self->frameID;
    }

    StateMachine_Run(self->state);
}

void UISubHeading_LateUpdate(void) {}

void UISubHeading_StaticUpdate(void) {}

void UISubHeading_Draw(void)
{
    RSDK_THIS(UISubHeading);
    Vector2 drawPos;

    int32 size = (self->size.x + self->size.y) >> 16;
    drawPos.x  = self->position.x;
    drawPos.y  = self->position.y;
    UIWidgets_DrawParallelogram(self->position.x, self->position.y, size, self->size.y >> 16, self->bgEdgeSize, 0x00, 0x00, 0x00);

    drawPos = self->position;
    switch (self->align) {
        case UIBUTTON_ALIGN_LEFT: drawPos.x += -0x60000 - (self->size.x >> 1); break;

        default:
        case UIBUTTON_ALIGN_CENTER: break;

        case UIBUTTON_ALIGN_RIGHT:
            drawPos.x -= 0x60000;
            drawPos.x += self->size.x >> 1;
            break;
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
        self->drawGroup     = 2;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x400000;
        self->bgEdgeSize    = self->size.y >> 16;
        self->size.y        = abs(self->size.y);

        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator, true, self->frameID);
        self->textFrames = UIWidgets->textFrames;
    }
}

void UISubHeading_StageLoad(void) {}

// ???
// Why is this all here
// Wouldn't it make more sense to make a "SaveSelectMenu" object like options, extras, time attack, competition and main menus have?

#if MANIA_USE_PLUS
void UISubHeading_Initialize(void)
{
    String tag;
    INIT_STRING(tag);

    foreach_all(UIControl, control)
    {
        RSDK.SetString(&tag, "Save Select");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            ManiaModeMenu->saveSelectMenu = control;

        RSDK.SetString(&tag, "Encore Mode");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            ManiaModeMenu->encoreSaveSelect = control;

        RSDK.SetString(&tag, "No Save Mode");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            ManiaModeMenu->noSaveMenu = control;

        RSDK.SetString(&tag, "No Save Encore");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            ManiaModeMenu->noSaveMenuEncore = control;

        RSDK.SetString(&tag, "Secrets");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            ManiaModeMenu->secretsMenu = control;
    }
}

void UISubHeading_HandleUnlocks(void)
{
    EntityUIControl *control  = ManiaModeMenu->secretsMenu;
    EntityUIButton *debugMode = control->buttons[1];
    debugMode->disabled       = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_DEBUGMODE) && !globals->superSecret;
    if (debugMode->disabled)
        UIButton_ManageChoices(debugMode);

    EntityUIButton *sonicAbility      = control->buttons[2];
    EntityUIButton *peeloutChoice     = UIButton_GetChoicePtr(sonicAbility, 1);
    EntityUIButton *instaShieldChoice = UIButton_GetChoicePtr(sonicAbility, 2);
    sonicAbility->disabled            = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_PEELOUT);
    if (sonicAbility->disabled)
        UIButton_ManageChoices(sonicAbility);

    peeloutChoice->disabled     = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_PEELOUT);
    instaShieldChoice->disabled = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_INSTASHIELD);

    EntityUIButton *andKnux = control->buttons[3];
    andKnux->disabled       = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_ANDKNUX);
    if (andKnux->disabled)
        UIButton_ManageChoices(andKnux);
}

void UISubHeading_SetupActions(void)
{
    foreach_all(UISaveSlot, slot) { slot->actionCB = UISubHeading_SaveButton_ActionCB; }

    foreach_all(UIButtonPrompt, prompt)
    {
        Hitbox hitbox;
        EntityUIControl *saveSel = ManiaModeMenu->saveSelectMenu;
        hitbox.right             = saveSel->size.x >> 17;
        hitbox.left              = -(saveSel->size.x >> 17);
        hitbox.bottom            = saveSel->size.y >> 17;
        hitbox.top               = -(saveSel->size.y >> 17);

        if (MathHelpers_PointInHitbox(saveSel->startPos.x - saveSel->cameraOffset.x, saveSel->startPos.y - saveSel->cameraOffset.y,
                                      prompt->position.x, prompt->position.y, FLIP_NONE, &hitbox)
            && prompt->buttonID == 2) {
            ManiaModeMenu->delSavePrompt = prompt;
        }
        else {
            saveSel = ManiaModeMenu->encoreSaveSelect;

            hitbox.right  = saveSel->size.x >> 17;
            hitbox.left   = -(saveSel->size.x >> 17);
            hitbox.bottom = saveSel->size.y >> 17;
            hitbox.top    = -(saveSel->size.y >> 17);
            if (MathHelpers_PointInHitbox(saveSel->startPos.x - saveSel->cameraOffset.x, saveSel->startPos.y - saveSel->cameraOffset.y,
                                          prompt->position.x, prompt->position.y, FLIP_NONE, &hitbox)
                && prompt->buttonID == 2)
                ManiaModeMenu->delSavePrompt_Encore = prompt;
        }
    }

    EntityUIControl *saveSel = ManiaModeMenu->saveSelectMenu;
    saveSel->menuUpdateCB    = UISubHeading_SaveSel_MenuUpdateCB;
    saveSel->yPressCB        = UISubHeading_SaveSel_YPressCB;

    EntityUIControl *saveSelEncore = ManiaModeMenu->encoreSaveSelect;
    saveSelEncore->menuUpdateCB    = UISubHeading_SaveSel_MenuUpdateCB;
}

void UISubHeading_HandleMenuReturn(int32 slot)
{
    EntityUIControl *control = ManiaModeMenu->secretsMenu;
    SaveRAM *saveGame        = (SaveRAM *)SaveGame_GetDataPtr(slot, false);

    UIButton_SetChoiceSelection(control->buttons[0], (saveGame->medalMods & MEDAL_NOTIMEOVER) != 0);
    UIButton_SetChoiceSelection(control->buttons[1], (saveGame->medalMods & MEDAL_ANDKNUCKLES) != 0);

    if (saveGame->medalMods & MEDAL_NODROPDASH) {
        if (saveGame->medalMods & MEDAL_PEELOUT)
            UIButton_SetChoiceSelection(control->buttons[2], 1);
        else if (saveGame->medalMods & MEDAL_INSTASHIELD)
            UIButton_SetChoiceSelection(control->buttons[2], 2);
    }
    else {
        UIButton_SetChoiceSelection(control->buttons[2], 0);
    }

    if (saveGame->medalMods & MEDAL_ANDKNUCKLES)
        UIButton_SetChoiceSelection(control->buttons[3], 1);
    else
        UIButton_SetChoiceSelection(control->buttons[3], 0);
}

int32 UISubHeading_GetMedalMods(void)
{
    EntityUIControl *control = ManiaModeMenu->secretsMenu;

    int32 mods = 0;
    if (control->buttons[0]->selection == 1)
        mods |= MEDAL_NOTIMEOVER;

    if (control->buttons[1]->selection == 1)
        mods |= MEDAL_DEBUGMODE;

    if (control->buttons[2]->selection == 1) {
        mods |= MEDAL_NODROPDASH;
        mods |= MEDAL_PEELOUT;
    }
    else if (control->buttons[2]->selection == 2) {
        mods |= MEDAL_NODROPDASH;
        mods |= MEDAL_INSTASHIELD;
    }

    if (control->buttons[3]->selection == 1)
        mods |= MEDAL_ANDKNUCKLES;

    return mods;
}

void UISubHeading_SaveFileCB(bool32 success)
{
    UIWaitSpinner_FinishWait();

    RSDK.LoadScene();
}

void UISubHeading_SecretsTransitionCB(void)
{
    EntityUIControl *control = ManiaModeMenu->saveSelectMenu;
    control->childHasFocus   = true;

    UIControl_MatchMenuTag("Secrets");
}

void UISubHeading_LeaveSecretsMenu(void)
{
    EntityUIControl *control = ManiaModeMenu->saveSelectMenu;
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
        EntityUIButtonPrompt *prompt = ManiaModeMenu->delSavePrompt;

        if (self == ManiaModeMenu->encoreSaveSelect) {
            prompt = ManiaModeMenu->delSavePrompt_Encore;
        }
        else if (self->lastButtonID != ManiaModeMenu->saveSelLastButtonID) {
            UISubHeading_LeaveSecretsMenu();
            ManiaModeMenu->saveSelLastButtonID = self->lastButtonID;
        }

        bool32 canDeleteSave = false;
        bool32 showPrompt    = false;
        for (int32 i = 0; i < self->buttonCount; ++i) {
            showPrompt |= self->buttons[i]->state == UISaveSlot_State_Selected;

            if (self->lastButtonID >= 0) {
                if (self->buttons[i] == self->buttons[self->lastButtonID]) {
                    EntityUISaveSlot *slot = (EntityUISaveSlot *)self->buttons[self->lastButtonID];
                    if (!slot->isNewSave)
                        canDeleteSave = true;
                }
            }
        }

        if (!showPrompt) {
            if ((self == ManiaModeMenu->saveSelectMenu && self->lastButtonID == 8)
                || (self == ManiaModeMenu->encoreSaveSelect && !self->lastButtonID)) {
                prompt->visible = false;
            }
            else {
                prompt->visible = canDeleteSave;
            }
        }
    }
}

void UISubHeading_SaveSel_YPressCB(void)
{
    EntityUIControl *control = ManiaModeMenu->saveSelectMenu;

    if (control->active == ACTIVE_ALWAYS) {
        if (!ManiaModeMenu->inSecretsMenu) {
            UISubHeading_HandleMenuReturn(((EntityUISaveSlot *)control->buttons[control->buttonID])->slotID);
            ManiaModeMenu->inSecretsMenu = true;
        }

        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIControl->inputLocked = true;

        UITransition_StartTransition(UISubHeading_SecretsTransitionCB, 0);
    }
}

void UISubHeading_SaveButton_ActionCB(void)
{
    RSDK_THIS(UISaveSlot);

    EntityMenuParam *param   = MenuParam_GetParam();
    EntityUIControl *control = (EntityUIControl *)self->parent;

    SaveRAM *saveRAM = (SaveRAM *)SaveGame_GetDataPtr(self->slotID, self->encoreMode);
    TimeAttackData_Clear();

    RSDK.GetCString(param->menuTag, &control->tag);
    param->menuSelection = control->lastButtonID;
    param->replayID      = 0;
    globals->gameMode    = self->encoreMode != false;

    bool32 loadingSave = false;
    if (self->type) {
        // Bug Details(?):
        // sizeof(globals->noSaveSlot) and sizeof(saveData) is 4096 (sizeof(int32) * 0x400)
        // but the memset size is only 1024 (sizeof(uint8) * 0x400)
        // so only about 1/4th of the save slot is cleared, though nothin uses the extra space so it's not a big deal

        memset(globals->noSaveSlot, 0, 0x400);
        globals->continues  = 0;
        globals->saveSlotID = NO_SAVE_SLOT;
    }
    else {
        globals->saveSlotID = self->slotID;
        globals->medalMods  = 0;

        if (self->isNewSave) {
            int32 *saveData = SaveGame_GetDataPtr(self->slotID % 8, self->encoreMode);

            // Bug Details(?):
            // see above

            memset(saveData, 0, 0x400);
            if (globals->gameMode != MODE_ENCORE)
                saveRAM->saveState = 1;

            saveRAM->characterID   = self->frameID;
            saveRAM->zoneID        = 0;
            saveRAM->lives         = 3;
            saveRAM->collectedEmeralds = self->saveEmeralds;
            saveRAM->continues     = 0;
            UIWaitSpinner_StartWait();
            loadingSave = true;
            SaveGame_SaveFile(UISubHeading_SaveFileCB);
        }
        else {
            if (saveRAM->saveState == SAVEGAME_COMPLETE) {
                SaveGame_ClearCollectedSpecialRings();
                saveRAM->score                 = 0;
                saveRAM->score1UP              = 500000;
            }

            loadingSave = true;
            SaveGame_SaveFile(UISubHeading_SaveFileCB);
        }
    }

    if (self->encoreMode) {
        globals->medalMods = MEDAL_NOTIMEOVER;
        saveRAM->medalMods = globals->medalMods;
    }
    else {
        globals->medalMods = UISubHeading_GetMedalMods();
        saveRAM->medalMods = globals->medalMods;

        switch (self->frameID) {
            case 0: // Sonic & Tails
            case 1: globals->playerID = ID_SONIC; break;
            case 2: globals->playerID = ID_TAILS; break;
            case 3: globals->playerID = ID_KNUCKLES; break;
            case 4: globals->playerID = ID_MIGHTY; break;
            case 5: globals->playerID = ID_RAY; break;
            default: break;
        }

        if ((globals->medalMods & MEDAL_ANDKNUCKLES))
            globals->playerID |= ID_KNUCKLES_ASSIST;
        else if (!self->frameID)
            globals->playerID |= ID_TAILS_ASSIST;
    }

    if (self->type == UISAVESLOT_NOSAVE || self->isNewSave) {
        if (self->encoreMode) {
            globals->playerID          = ID_SONIC;
            globals->stock             = ID_NONE;
            globals->characterFlags    = ID_NONE;
            globals->enableIntro       = true;
            globals->suppressTitlecard = true;
            RSDK.SetScene("Cutscenes", "Angel Island Zone Encore");
        }
        else if (((globals->medalMods & MEDAL_DEBUGMODE) && (ControllerInfo->keyC.down || ControllerInfo->keyX.down))
                 && self->type == UISAVESLOT_NOSAVE) {
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
        SceneInfo->listPos += TimeAttackData_GetEncoreListPos(self->saveZoneID, ACT_1, self->frameID);
    }
    else {
        RSDK.SetScene("Mania Mode", "");
        SceneInfo->listPos += TimeAttackData_GetManiaListPos(self->saveZoneID, ACT_1, self->frameID);
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

    self->drawGroup     = 2;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->bgEdgeSize    = self->size.y >> 16;
    self->size.y        = abs(self->size.y);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator, true, self->frameID);
    self->textFrames = UIWidgets->textFrames;

    UISubHeading_Draw();
}

void UISubHeading_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(UISubHeading, align);
    RSDK_ENUM_VAR("Left", UIBUTTON_ALIGN_LEFT);
    RSDK_ENUM_VAR("Center", UIBUTTON_ALIGN_CENTER);
    RSDK_ENUM_VAR("Right", UIBUTTON_ALIGN_RIGHT);
}
#endif

void UISubHeading_Serialize(void)
{
    RSDK_EDITABLE_VAR(UISubHeading, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UISubHeading, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UISubHeading, VAR_ENUM, frameID);
    RSDK_EDITABLE_VAR(UISubHeading, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UISubHeading, VAR_ENUM, offset);
}
