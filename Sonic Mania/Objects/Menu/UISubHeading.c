#include "SonicMania.h"

ObjectUISubHeading *UISubHeading;

void UISubHeading_Update(void)
{
    RSDK_THIS(UISubHeading);

    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex || entity->storedListID != entity->listID || entity->storedFrameID != entity->frameID) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator, true, entity->frameID);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
        entity->storedListID    = entity->listID;
        entity->storedFrameID   = entity->frameID;
    }

    StateMachine_Run(entity->state);
}

void UISubHeading_LateUpdate(void) {}

void UISubHeading_StaticUpdate(void) {}

void UISubHeading_Draw(void)
{
    RSDK_THIS(UISubHeading);
    Vector2 drawPos;

    int32 size  = entity->size.y + entity->size.x;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    UIWidgets_Unknown7(entity->size.y, size >> 16, entity->shiftedY, 0, 0, 0, entity->position.x, entity->position.y);

    drawPos = entity->position;
    if (!entity->align) {
        drawPos.x += -0x60000 - (entity->size.x >> 1);
    }
    else if (entity->align == 2) {
        drawPos.x -= 0x60000;
        drawPos.x += entity->size.x >> 1;
    }
    drawPos.x += entity->offset;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
}

void UISubHeading_Create(void *data)
{
    RSDK_THIS(UISubHeading);
    if (!RSDK_sceneInfo->inEditor) {
        entity->offset <<= 16;
        entity->visible       = true;
        entity->drawOrder     = 2;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x400000;
        entity->shiftedY      = entity->size.y >> 16;
        entity->size.y        = abs(entity->size.y);
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator, true, entity->frameID);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    }
}

void UISubHeading_StageLoad(void) {}

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

void UISubHeading_Unknown2(void)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->secretsMenu;
    EntityUIButton *button   = control->buttons[1];
    button->disabled         = !GameProgress_CheckUnlock(5) && globals->superSecret;
    if (button->disabled)
        UIButton_Unknown1(button);

    button                  = control->buttons[2];
    EntityUIButton *option1 = UIButton_GetChoicePtr(button, 1);
    EntityUIButton *option2 = UIButton_GetChoicePtr(button, 2);
    int32 unlock              = GameProgress_CheckUnlock(2);
    button->disabled        = !unlock;
    if (button->disabled)
        UIButton_Unknown1(button);
    option1->disabled = !GameProgress_CheckUnlock(2);
    option2->disabled = !GameProgress_CheckUnlock(3);

    button           = control->buttons[3];
    unlock           = GameProgress_CheckUnlock(4);
    button->disabled = !unlock;
    if (button->disabled)
        UIButton_Unknown1(button);
}

void UISubHeading_Unknown3(void)
{
    foreach_all(UISaveSlot, slot) { slot->options2 = UISubHeading_StartNewSave; }

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
    saveSel->unknownCallback4 = UISubHeading_Unknown10;
    saveSel->yPressCB         = UISubHeading_Unknown11;

    EntityUIControl *saveSelEncore  = (EntityUIControl *)ManiaModeMenu->encoreSaveSelect;
    saveSelEncore->unknownCallback4 = UISubHeading_Unknown10;
}

void UISubHeading_Unknown4(int32 slot)
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
    UIWaitSpinner_Wait2();
    RSDK.LoadScene();
}

void UISubHeading_SecretsTransitionCB(void)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
    control->childHasFocus   = true;
    UIControl_MatchMenuTag("Secrets");
}

void UISubHeading_Unknown9(void)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
    if (ManiaModeMenu->field_24) {
        EntityUISaveSlot *slot = (EntityUISaveSlot *)control->buttons[control->field_D8];
        UISubHeading_Unknown4(slot->slotID);
        ManiaModeMenu->field_24 = 0;
    }
}

void UISubHeading_Unknown10(void)
{
    RSDK_THIS(UIControl);
    if (entity->active == ACTIVE_ALWAYS) {
        EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)ManiaModeMenu->prompt1;
        if (entity == (EntityUIControl *)ManiaModeMenu->encoreSaveSelect) {
            prompt = (EntityUIButtonPrompt *)ManiaModeMenu->prompt2;
        }
        else if (entity->field_D8 != ManiaModeMenu->field_28) {
            UISubHeading_Unknown9();
            ManiaModeMenu->field_28 = entity->field_D8;
        }

        bool32 flag  = false;
        bool32 flag2 = false;
        for (int32 i = 0; i < entity->buttonCount; ++i) {
            flag2 |= entity->buttons[i]->state == UISaveSlot_Unknown28;
            if (entity->field_D8 >= 0) {
                if (entity->buttons[i] == entity->buttons[entity->field_D8]) {
                    EntityUISaveSlot *slot = (EntityUISaveSlot *)entity->buttons[entity->field_D8];
                    if (!slot->isNewSave)
                        flag = true;
                }
            }
        }

        if (!flag2) {
            if ((entity == (EntityUIControl *)ManiaModeMenu->saveSelectMenu && entity->field_D8 == 8)
                || (entity == (EntityUIControl *)ManiaModeMenu->encoreSaveSelect && !entity->field_D8)) {
                prompt->visible = false;
            }
            else {
                prompt->visible = flag;
            }
        }
    }
}

void UISubHeading_Unknown11(void)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
    if (control->active == ACTIVE_ALWAYS) {
        if (!ManiaModeMenu->field_24) {
            UISubHeading_Unknown4(control->buttons[control->activeEntityID]->stopMusic);
            ManiaModeMenu->field_24 = 1;
        }
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIControl->inputLocked = true;

        UITransition_StartTransition(UISubHeading_SecretsTransitionCB, 0);
    }
}

void UISubHeading_StartNewSave(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    RSDK_THIS(UISaveSlot);
    EntityUIControl *control = (EntityUIControl *)entity->parent;

    EntitySaveGame *saveRAM = (EntitySaveGame *)SaveGame_GetDataPtr(entity->slotID, entity->encoreMode);
    TimeAttackData_ClearOptions();
    RSDK.GetCString(param->menuTag, &control->tag);
    param->selectionID = control->field_D8;
    param->field_168 = 0;
    globals->gameMode = entity->encoreMode != false;

    bool32 loadingSave = false;
    if (entity->type) {
        memset(globals->noSaveSlot, 0, 0x400);
        globals->continues  = 0;
        globals->saveSlotID = NO_SAVE_SLOT;
    }
    else {
        globals->saveSlotID = entity->slotID;
        globals->medalMods  = 0;
        if (entity->isNewSave) {
            int32 *saveData = SaveGame_GetDataPtr(entity->slotID % 8, entity->encoreMode);

            memset(saveData, 0, 0x400);
            if (globals->gameMode != MODE_ENCORE)
                saveRAM->saveState = 1;
            saveRAM->characterID   = entity->frameID;
            saveRAM->zoneID        = 0;
            saveRAM->lives         = 3;
            saveRAM->chaosEmeralds = entity->saveEmeralds;
            saveRAM->continues     = 0;
            UIWaitSpinner_Wait();
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

    if (entity->encoreMode) {
        globals->medalMods = getMod(MEDAL_NOTIMEOVER);
        saveRAM->medalMods = globals->medalMods;
    }
    else {
        globals->medalMods = UISubHeading_GetMedalMods();
        saveRAM->medalMods = globals->medalMods;
        switch (entity->frameID) {
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
        else if (!entity->frameID) {
            globals->playerID |= ID_TAILS_ASSIST;
        }
    }

    if (entity->type == 1 || entity->isNewSave) {
        if (entity->encoreMode) {
            globals->playerID          = ID_SONIC;
            globals->stock             = 0;
            globals->characterFlags    = 0;
            globals->enableIntro       = true;
            globals->suppressTitlecard = true;
            RSDK.SetScene("Cutscenes", "Angel Island Zone Encore");
        }
        else if (((globals->medalMods & getMod(MEDAL_DEBUGMODE)) && (RSDK_controller->keyC.down || RSDK_controller->keyX.down)) && entity->type == 1) {
            RSDK.SetScene("Presentation", "Level Select");
        }
        else {
            RSDK.SetScene("Cutscenes", "Angel Island Zone");
        }
    }
    else if (entity->encoreMode) {
        globals->playerID       = saveRAM->playerID;
        globals->stock          = saveRAM->stock;
        globals->characterFlags = saveRAM->characterFlags;
        RSDK.SetScene("Encore Mode", "");
        RSDK_sceneInfo->listPos += TimeAttackData_GetEncoreListPos(entity->saveZoneID, entity->frameID, 0);
    }
    else {
        RSDK.SetScene("Mania Mode", "");
        RSDK_sceneInfo->listPos += TimeAttackData_GetManiaListPos(entity->saveZoneID, entity->frameID, 0);
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

    if (entity->offset < 0x10000)
        entity->offset <<= 16;
    entity->drawOrder     = 2;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x400000;
    entity->shiftedY      = entity->size.y >> 16;
    entity->size.y        = abs(entity->size.y);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator, true, entity->frameID);
    entity->textSpriteIndex = UIWidgets->textSpriteIndex;

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
