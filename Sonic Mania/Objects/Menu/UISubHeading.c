#include "SonicMania.h"

ObjectUISubHeading *UISubHeading;

void UISubHeading_Update(void) {}

void UISubHeading_LateUpdate(void) {}

void UISubHeading_StaticUpdate(void) {}

void UISubHeading_Draw(void) {}

void UISubHeading_Create(void *data) {}

void UISubHeading_StageLoad(void) {}

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

int UISubHeading_GetMedalMods(void)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;

    int mods = 0;
    if (control->entities[0]->selection == 1)
        mods |= getMod(MEDAL_NOTIMEOVER);

    if (control->entities[1]->selection == 1)
        mods |= getMod(MEDAL_DEBUGMODE);

    if (control->entities[2]->selection == 1) {
        mods |= getMod(MEDAL_NODROPDASH);
        mods |= getMod(MEDAL_PEELOUT);
    }
    else if (control->entities[2]->selection == 2) {
        mods |= getMod(MEDAL_NODROPDASH);
        mods |= getMod(MEDAL_INSTASHIELD);
    }

    if (control->entities[3]->selection == 1)
        mods |= getMod(MEDAL_ANDKNUCKLES);

    return mods;
}

void UISubHeading_SaveFileCB(int status)
{
    UIWaitSpinner_Wait2();
    RSDK.InitSceneLoad();
}

void UISubHeading_SecretsTransitionCB(void)
{
    EntityUIControl *control = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
    control->childHasFocus   = true;
    UIControl_MatchMenuTag("Secrets");
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
            // UISubHeading_Unknown9();
            ManiaModeMenu->field_28 = entity->field_D8;
        }

        bool32 flag  = false;
        bool32 flag2 = false;
        for (int i = 0; i < entity->unknownCount1; ++i) {
            flag2 |= entity->entities[i]->state == UISaveSlot_Unknown28;
            if (entity->field_D8 >= 0) {
                if (entity->entities[i] == entity->entities[entity->field_D8]) {
                    EntityUISaveSlot *slot = (EntityUISaveSlot *)entity->entities[entity->field_D8];
                    if (!slot->flagUnknown)
                        flag = true;
                }
            }
        }

        if (!flag2) {
            if (entity == (EntityUIControl *)ManiaModeMenu->saveSelectMenu && entity->field_D8 == 8
                || entity == (EntityUIControl *)ManiaModeMenu->encoreSaveSelect && !entity->field_D8) {
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
            //UISubHeading_Unknown4(control->entities[control->activeEntityID]->stopMusic);
            ManiaModeMenu->field_24 = 1;
        }
        RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 255);
        UIControl->inputLocked = true;

        UITransition_StartTransition(UISubHeading_SecretsTransitionCB, 0);
    }
}

void UISubHeading_StartNewSave(void)
{
    RSDK_THIS(UISaveSlot);
    EntityUIControl *control = (EntityUIControl *)entity->parent;

    int *saveRAM = SaveGame_GetDataPtr(entity->slotID, entity->encoreMode);
    TimeAttackData_ClearOptions();
    char *tag = (char *)&((char *)&globals->menuParam)[90];
    RSDK.GetCString(tag, &control->tag);
    globals->menuParam[87] = control->field_D8;
    globals->menuParam[90] = 0;
    globals->gameMode      = entity->encoreMode != false;

    bool32 loadingSave = false;
    if (entity->type) {
        memset(globals->noSaveSlot, 0, 0x400);
        globals->continues  = 0;
        globals->saveSlotID = 0xFF;
#if !RETRO_USE_PLUS
        globals->gameMode = MODE_NOSAVE;
#endif
    }
    else {
        globals->saveSlotID = entity->slotID;
        globals->medalMods  = 0;
        if (entity->flagUnknown == 1) {
            int *saveData = SaveGame_GetDataPtr(entity->slotID % 8, entity->encoreMode);
            memset(saveData, 0, 0x400);
            if (globals->gameMode != MODE_ENCORE)
                saveRAM[22] = 1;
            saveRAM[0x17] = entity->frameID;
            saveRAM[0x18] = 0;
            saveRAM[0x19] = 3;
            saveRAM[0x1C] = entity->saveEmeralds;
            saveRAM[0x1D] = 0;
            UIWaitSpinner_Wait();
            loadingSave = true;
            SaveGame_SaveFile(UISubHeading_SaveFileCB);
        }
        else {
            if (saveRAM[22] == 2) {
                saveRAM[32] = 0;
                saveRAM[26] = 0;
                saveRAM[27] = 500000;
            }
            loadingSave = true;
            SaveGame_SaveFile(UISubHeading_SaveFileCB);
        }
    }

    if (entity->encoreMode) {
        globals->medalMods = getMod(MEDAL_NOTIMEOVER);
        saveRAM[33]        = globals->medalMods;
    }
    else {
        globals->medalMods = UISubHeading_GetMedalMods();
        saveRAM[33]        = globals->medalMods;
        switch (entity->frameID) {
            case 0:
            case 1: globals->playerID = ID_SONIC; break;
            case 2: globals->playerID = ID_TAILS; break;
            case 3: globals->playerID = ID_KNUCKLES; break;
#if RETRO_USE_PLUS
            case 4: globals->playerID = ID_MIGHTY; break;
            case 5: globals->playerID = ID_RAY; break;
#endif
            default: break;
        }

        if ((globals->medalMods & getMod(MEDAL_ANDKNUCKLES))) {
            globals->playerID |= ID_KNUCKLES_ASSIST;
        }
        else if (!entity->frameID) {
            globals->playerID |= ID_TAILS_ASSIST;
        }
    }

    if (entity->type == 1 || entity->flagUnknown) {
        if (entity->encoreMode) {
            globals->playerID          = ID_SONIC;
            globals->stock             = 0;
            globals->characterFlags    = 0;
            globals->enableIntro       = true;
            globals->suppressTitlecard = true;
            RSDK.LoadScene("Cutscenes", "Angel Island Zone Encore");
        }
        else if (((globals->medalMods & getMod(MEDAL_DEBUGMODE)) && (RSDK_controller->keyC.down || RSDK_controller->keyX.down))
                 && entity->type == 1) {
            RSDK.LoadScene("Presentation", "Level Select");
        }
        else {
            RSDK.LoadScene("Cutscenes", "Angel Island Zone");
        }
    }
    else if (entity->encoreMode) {
        globals->playerID       = saveRAM[68];
        globals->stock          = saveRAM[67];
        globals->characterFlags = saveRAM[66];
        RSDK.LoadScene("Encore Mode", "");
        RSDK_sceneInfo->listPos += TimeAttackData_GetEncoreListPos(entity->saveZoneID, entity->frameID, 0);
    }
    else {
        RSDK.LoadScene("Mania Mode", "");
        RSDK_sceneInfo->listPos += TimeAttackData_GetManiaListPos(entity->saveZoneID, entity->frameID, 0);
    }

    if (!loadingSave) {
        globals->initCoolBonus = false;
        RSDK.InitSceneLoad();
    }
}

void UISubHeading_EditorDraw(void) {}

void UISubHeading_EditorLoad(void) {}

void UISubHeading_Serialize(void) {}
