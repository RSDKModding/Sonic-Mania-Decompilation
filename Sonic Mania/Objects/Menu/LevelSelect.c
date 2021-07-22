#include "SonicMania.h"

ObjectLevelSelect *LevelSelect;

void LevelSelect_Update(void)
{
    RSDK_THIS(LevelSelect);
#if RETRO_USE_PLUS
    EntityUIText *text = (EntityUIText *)entity->storedTextEntity;
    if (text)
        text->visible = API.CheckDLC(DLC_PLUS);
#endif
    StateMachine_Run(entity->state);
    RSDK_screens->position.x = 256 - RSDK_screens->centerX;
}

void LevelSelect_LateUpdate(void)
{

}

void LevelSelect_StaticUpdate(void)
{
    if (--LevelSelect->bgScrollPos < 1) {
        ++LevelSelect->bgScrollPosID;
        LevelSelect->bgScrollPosID &= 3;
        LevelSelect->bgScrollPos = LevelSelect->bgScrollPositions[LevelSelect->bgScrollPosID];
        TileLayer *bg            = RSDK.GetSceneLayer(0);
        bg->scrollPos            = (bg->scrollPos + 0x1000000) & 0x7FF0000;
    }
}

void LevelSelect_Draw(void)
{
    RSDK_THIS(LevelSelect);
    StateMachine_Run(entity->stateDraw);
}

void LevelSelect_Create(void* data)
{
    RSDK_THIS(LevelSelect);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_ALWAYS;
        entity->visible   = true;
        entity->drawOrder = 12;
        entity->state          = LevelSelect_State_SetupEntities;
        entity->stateDraw      = LevelSelect_StateDraw_Fade;
        entity->timer          = 640;
    }
}

void LevelSelect_StageLoad(void)
{
    LevelSelect->sfxFail     = RSDK.GetSFX("Stage/Fail.wav");
    LevelSelect->sfxRing     = RSDK.GetSFX("Global/Ring.wav");
    LevelSelect->sfxEmerald  = RSDK.GetSFX("Special/Emerald.wav");
    LevelSelect->sfxContinue = RSDK.GetSFX("Special/Continue.wav");
    LevelSelect->sfxMedalGot = RSDK.GetSFX("Special/MedalCaught.wav");
    RSDK.ResetEntitySlot(0, LevelSelect->objectID, NULL);
    UIPicture->spriteIndex    = RSDK.LoadSpriteAnimation("LSelect/Icons.bin", SCOPE_STAGE);
    UIText->spriteIndex       = RSDK.LoadSpriteAnimation("LSelect/Text.bin", SCOPE_STAGE);
    LevelSelect->dword11C     = 0;
    LevelSelect->soundTestMax = 0;
    foreach_all(Music, music)
    {
        if (!LevelSelect->dword11C) {
            LevelSelect->dword11C = RSDK.GetEntityID(music);
        }
        ++LevelSelect->soundTestMax;
    }
    ++LevelSelect->dword11C;
    --LevelSelect->soundTestMax;
    globals->medalMods |= MEDAL_DEBUGMODE;
    LevelSelect->cheatCodePtrs[0]       = LevelSelect->cheat_RickyMode;
    LevelSelect->cheatCodePtrs[1]       = LevelSelect->cheat_AllEmeralds;
    LevelSelect->cheatCodePtrs[2]       = LevelSelect->cheat_MaxContinues;
    LevelSelect->cheatCodePtrs[3]       = LevelSelect->cheat_SwapGameMode;
    LevelSelect->cheatCodePtrs[4]       = LevelSelect->cheat_UnlockAllMedals;
    LevelSelect->cheatCodePtrs[5]       = LevelSelect->cheat_SuperDash;
    LevelSelect->cheatCodePtrs[6]       = LevelSelect->cheat_MaxControl;
    LevelSelect->cheatCodePtrs[7]       = &LevelSelect->cheat_DisableSuperMusic[4];
    LevelSelect->checkCheatActivated[0] = LevelSelect_CheatActivated_RickyMode;
    LevelSelect->checkCheatActivated[1] = LevelSelect_CheatActivated_AllEmeralds;
    LevelSelect->checkCheatActivated[2] = LevelSelect_CheatActivated_MaxContinues;
    LevelSelect->checkCheatActivated[3] = LevelSelect_CheatActivated_SwapGameMode;
    LevelSelect->checkCheatActivated[4] = LevelSelect_CheatActivated_UnlockAllMedals;
    LevelSelect->checkCheatActivated[5] = LevelSelect_CheatActivated_SuperDash;
    LevelSelect->checkCheatActivated[6] = LevelSelect_CheatActivated_MaxControl;
    LevelSelect->checkCheatActivated[7] = LevelSelect_CheatActivated_ChangeSuperMusicFlag;
    LevelSelect->cheatUnknown[0]        = 0;
    LevelSelect->cheatUnknown[1]        = 0;
    LevelSelect->cheatUnknown[2]        = 0;
    LevelSelect->cheatUnknown[3]        = 0;
    LevelSelect->cheatUnknown[4]        = 0;
    LevelSelect->cheatUnknown[5]        = 0;
    LevelSelect->cheatUnknown[6]        = 0;
    LevelSelect->cheatUnknown[7]        = 0;
}

//TODO: were these actually in preplus?
void LevelSelect_CheatActivated_AllEmeralds(void)
{
    Music_FadeOut(0.125);
    RSDK.PlaySFX(LevelSelect->sfxEmerald, 0, 255);
    SaveGame->saveRAM[28] = 0x7F;
}

void LevelSelect_CheatActivated_ChangeSuperMusicFlag(void)
{
    RSDK.PlaySFX(LevelSelect->sfxRing, 0, 255);
#if RETRO_USE_PLUS
    globals->superMusicEnabled ^= 1;
#endif
}

void LevelSelect_CheatActivated_MaxContinues(void)
{
    RSDK.PlaySFX(LevelSelect->sfxContinue, 0, 255);
    SaveGame->saveRAM[29] = 14;
}

void LevelSelect_CheatActivated_MaxControl(void)
{
    RSDK.PlaySFX(LevelSelect->sfxRing, 0, 255);
    globals->medalMods &= ~getMod(MEDAL_NODROPDASH);
    globals->medalMods |= getMod(MEDAL_INSTASHIELD);
    globals->medalMods |= getMod(MEDAL_PEELOUT);
}

void LevelSelect_CheatActivated_RickyMode(void)
{
    RSDK.PlaySFX(LevelSelect->sfxRing, 0, 255);
#if RETRO_USE_PLUS
    globals->secrets ^= SECRET_RICKYMODE;
#endif
}

void LevelSelect_CheatActivated_SuperDash(void)
{
    RSDK.PlaySFX(LevelSelect->sfxRing, 0, 255);
#if RETRO_USE_PLUS
    globals->secrets ^= SECRET_SUPERDASH;
#endif
}

void LevelSelect_CheatActivated_SwapGameMode(void)
{
#if RETRO_USE_PLUS
    if (API.CheckDLC(DLC_PLUS) == 1) {
        RSDK.PlaySFX(LevelSelect->sfxRing, 0, 255);
        if (globals->gameMode == MODE_ENCORE) {
            globals->gameMode = MODE_MANIA;
        }
        else {
            globals->gameMode = MODE_ENCORE;
            if ((globals->medalMods & getMod(MEDAL_ANDKNUCKLES))) {
                globals->medalMods &= -getMod(MEDAL_ANDKNUCKLES);
            }
        }
    }
#endif
}

void LevelSelect_CheatActivated_UnlockAllMedals(void)
{
#if RETRO_USE_PLUS
    if (globals->superSecret && (globals->secrets & getMod(SECRET_RICKYMODE))) {
        RSDK.PlaySFX(LevelSelect->sfxMedalGot, 0, 255);
        SaveGame_UnlockAllMedals();
        if (RSDK_sceneInfo->inEditor || API.GetUserStorageNoSave() || globals->saveLoaded != STATUS_OK) {
            LogHelpers_Print("WARNING GameProgress Attempted to unlock all before loading SaveGame file");
        }
        else {
            globals->saveRAM[2373] = 0;
        }
    }
    else {
        RSDK.PlaySFX(LevelSelect->sfxRing, 0, 255);
    }
#endif
}

void LevelSelect_State_SetupEntities(void)
{
    RSDK_THIS(LevelSelect);

    int labelPos[32];
    int lastY = 0;
    foreach_all(UIText, label) {
        if (label->position.x < 0x1000000 && label->position.y > lastY) {
            lastY                          = label->position.y;
            labelPos[entity->labelCount++] = label->position.y;
        }
    }

    foreach_all(UIText, label2)
    {
        if (label2->position.x < 0x1000000) {
            for (int i = 0; i < entity->labelCount; ++i) {
                if (label2->position.y == labelPos[i]) {
                    if (label2->align == 2) {
                        entity->labelPtrs[i] = (Entity *)label2;
                    }
                    else if (label2->align == 0) {
                        entity->labelPtrs2[i] = (Entity *)label2;
                    }
                }
            }
        }
    }

    if (entity->labelCount > 0)
        memset(labelPos, 0, sizeof(int) * entity->labelCount);
    lastY = 0;

    foreach_all(UIText, label3) {
        if (label3->position.x > 0x1000000 && label3->position.y > lastY) {
            lastY                          = label3->position.y;
            labelPos[entity->labelCount++] = label3->position.y;
        }
    }

    foreach_all(UIText, label4) {
        if (label4->position.x > 0x1000000) {
            if (entity->labelCount > 0) {
                for (int i = 0; i < entity->labelCount; ++i) {
                    if (label4->position.y == labelPos[i]) {
                        switch (label4->align) {
                            case 0: entity->labelPtrs2[i] = (Entity *)label4; break;
                            case 1: break;
                            case 2:
                                entity->labelPtrs[i] = (Entity*)label4;
                                if (!label4->data0 && label4->data1 == 15) {
                                    entity->storedTextEntity = (Entity *)label;
                                }
                                break;
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < entity->labelCount; ++i) {
        if (!entity->labelPtrs2[i]) {
            for (int v = i; i >= 0; --i) {
                if (entity->labelPtrs[v]) {
                    entity->labelPtrs2[i] = entity->labelPtrs2[v];
                    break;
                }
            }
        }
    }


    if (entity->labelPtrs2[entity->labelID])
        ((EntityUIText *)entity->labelPtrs2[entity->labelID])->highlighted = true;

    if (entity->labelPtrs[entity->labelID])
        ((EntityUIText *)entity->labelPtrs[entity->labelID])->highlighted = true;

    foreach_all(UIText, label5)
    {
        if (label5->align == 1) {
            entity->soundTestLabel = (Entity *)label5;
            label5->align          = 0;
        }
    }

    int id = 0;
    for (int i = globals->playerID & 0xFF; i > 0; ++id) i >>= 1;
    entity->playerID = id;

    id          = 0;
    for (int i = globals->playerID >> 8; i > 0; ++id) i >>= 1;
    entity->player2ID = id;

    foreach_all(UIPicture, picture) {
        if (picture->listID == 1) {
            entity->zoneIcon      = (Entity *)picture;
            picture->animator.frameID   = entity->playerID;
        }
        else if (picture->listID == 3) {
            if (picture->frameID)
                entity->player2Icon = (Entity*)picture;
            else
                entity->player1Icon = (Entity *)picture;
        }
    }

    entity->labelPtrs2[entity->labelCount + 31] = entity->soundTestLabel;
    LevelSelect_ManagePlayerIcon();
    entity->state = LevelSelect_Unknown1;
}

void LevelSelect_ManagePlayerIcon(void)
{
    RSDK_THIS(LevelSelect);
    EntityUIPicture *player1 = (EntityUIPicture *)entity->player1Icon;
    EntityUIPicture *player2 = (EntityUIPicture *)entity->player2Icon;
    switch (entity->playerID) {
        case 1:
        case 2:
            if (entity->player2ID == 2)
                entity->playerID = 3;
            player1->animator.frameID = entity->playerID;
            break;
        case 3:
            player1->animator.frameID = entity->playerID;
            break;
#if RETRO_USE_PLUS
        case 4:
        case 5:
            if (!API.CheckDLC(DLC_PLUS))
                entity->playerID = 1;
            player1->animator.frameID = entity->playerID;
            break;
#endif
        default:
            entity->playerID          = 1;
            player1->animator.frameID = 1;
            break;
    }

    if (entity->player2ID == 1) {
        entity->player2ID = 2;
    }
    else {
        if (entity->player2ID != 2) {
            entity->player2ID     = 0;
            player2->animator.frameID = 0;
        }
        else {
            player2->animator.frameID = entity->player2ID;
            if (entity->playerID != 1) {
                entity->player2ID     = 0;
                player2->animator.frameID = 0;
            }
        }
    }
}

void LevelSelect_StateDraw_Fade(void)
{
    RSDK_THIS(LevelSelect);
    RSDK.FillScreen(0x000000, entity->timer, entity->timer - 128, entity->timer - 256);
}

void LevelSelect_Unknown1(void)
{
    RSDK_THIS(LevelSelect);

    if (entity->timer <= 0) {
        entity->timer     = 0;
        entity->state     = LevelSelect_Unknown2;
        entity->stateDraw = StateMachine_None;
    }
    else {
        entity->timer -= 16;
    }
}

void LevelSelect_Unknown2(void)
{
    RSDK_THIS(LevelSelect);

    bool32 confirmPress = false;
#if RETRO_USE_PLUS
    if (API.GetConfirmButtonFlip())
#else
    if (APICallback_GetConfirmButtonFlip())
#endif
        confirmPress = RSDK_controller->keyB.press;
    else
        confirmPress = RSDK_controller->keyA.press;

    if (RSDK_controller->keyUp.down || RSDK_stickL->keyUp.down) {
        entity->timer = (entity->timer + 1) & 0xF;
        if (entity->timer == 1) {
            EntityUIText *label = (EntityUIText*)entity->labelPtrs2[entity->labelID];
            if (label)
                label->highlighted = false;
            EntityUIText *label2 = (EntityUIText *)entity->labelPtrs[entity->labelID];
            if (label2)
                label2->highlighted = false;

#if RETRO_USE_PLUS
            if (--entity->labelID == 28 && !API.CheckDLC(DLC_PLUS))
#else
            if (--entity->labelID == 28)
#endif
                --entity->labelID;
            if (entity->labelID < 0)
                entity->labelID = entity->labelCount - 1;

            label = (EntityUIText*)entity->labelPtrs2[entity->labelID];
            if (label)
                label->highlighted = true;
            label = (EntityUIText*)entity->labelPtrs[entity->labelID];
            if (label)
                label->highlighted = true;
        }
    }
    else {
        if (RSDK_controller->keyDown.down || RSDK_stickL->keyDown.down) {
            entity->timer = (entity->timer + 1) & 0xF;
            if (entity->timer == 1) {
                LevelSelect_Unknown5(false);
#if RETRO_USE_PLUS 
                if (++entity->labelID == 28 && !API.CheckDLC(DLC_PLUS))
#else
                    if (++entity->labelID == 28)
#endif
                    ++entity->labelID;
                if (entity->labelID == entity->labelCount)
                    entity->labelID = 0;
                LevelSelect_Unknown5(true);
            }
        }
        else {
            if (RSDK_stickL->keyLeft.press || RSDK_controller->keyLeft.press || RSDK_controller->keyRight.press || RSDK_stickL->keyRight.press) {
                entity->timer = 0;
                if (entity->labelID >= entity->labelCount - 1) {
                    if (RSDK_stickL->keyLeft.press || RSDK_controller->keyLeft.press) {
                        if (--entity->soundTestID < 0)
                            entity->soundTestID = LevelSelect->soundTestMax - 1;
                    }
                    else if (++entity->soundTestID >= LevelSelect->soundTestMax) {
                        entity->soundTestID = 0;
                    }
                    EntityUIText *soundTest = (EntityUIText *)entity->soundTestLabel;
                    *soundTest->text.text   = entity->soundTestID >> 4;
                    soundTest->text.text[1] = entity->soundTestID & 0xF;
                }
                else {
                    LevelSelect_Unknown6();
                }
            }
            else {
                if (confirmPress || RSDK_controller->keyStart.press) {
                    if (entity->labelID < entity->labelCount - 1 || RSDK_controller->keyStart.press) {
#if RETRO_USE_PLUS 
                        if (entity->labelID != 28 || API.CheckDLC(DLC_PLUS))
#else
                        if (entity->labelID != 28)
#endif
                            LevelSelect_Unknown7();
                        else
                            RSDK.PlaySFX(LevelSelect->sfxFail, 0, 255);
                    }
                    else {
                        EntityMusic* mus = RSDK_GET_ENTITY(entity->soundTestID + LevelSelect->dword11C, Music);
                        Music_PlayTrackPtr(mus);
                        entity->field_18C = entity->soundTestID % 14;
                        entity->field_190 = entity->soundTestID & 0x1F;

                        for (int i = 0; i < 8; ++i) {
                            if (entity->soundTestID != LevelSelect->cheatCodePtrs[i][LevelSelect->cheatUnknown[i]]) {
                                LevelSelect->cheatUnknown[i] = 0;
                            }
                            else {
                                LevelSelect->cheatUnknown[i]++;
                                if (LevelSelect->cheatCodePtrs[i][LevelSelect->cheatUnknown[i]] == 255) {
                                    LevelSelect->checkCheatActivated[i]();
                                    LevelSelect->cheatUnknown[i] = 0;
                                }
                            }
                        }
                    }
                }
                else {
                    entity->timer = 0;
                }
            }
        }
    }

    if (RSDK_controller->keyX.press) {
        ++entity->playerID;
        LevelSelect_ManagePlayerIcon();
    }
    if (RSDK_controller->keyY.press) {
        ++entity->player2ID;
        LevelSelect_ManagePlayerIcon();
    }

    EntityUIPicture *zoneIcon = (EntityUIPicture*)entity->zoneIcon;
    if (entity->labelID >= entity->labelCount - 1)
        RSDK.SetSpriteAnimation(UIPicture->spriteIndex, 2, &zoneIcon->animator, true, entity->playerID);
    else
        RSDK.SetSpriteAnimation(UIPicture->spriteIndex, 1, &zoneIcon->animator, true, ((EntityUIText*)entity->labelPtrs[entity->labelID])->data1);
}

void LevelSelect_Unknown3(void)
{
    RSDK_THIS(LevelSelect);
    if (entity->timer >= 1024) {
        RSDK.InitSceneLoad();
    }
    else {
        entity->timer += 16;
    }
}

void LevelSelect_Unknown5(bool32 highlight)
{
    RSDK_THIS(LevelSelect);
    EntityUIText *label = (EntityUIText*)entity->labelPtrs2[entity->labelID];
    if (label)
        label->highlighted = highlight;
    EntityUIText *label2 = (EntityUIText *)entity->labelPtrs[entity->labelID];
    if (label2)
        label2->highlighted = highlight;
}   

void LevelSelect_Unknown6(void)
{
    RSDK_THIS(LevelSelect);
    EntityUIText *curLabel = (EntityUIText*)entity->labelPtrs[entity->labelID];
    if (!curLabel)
        curLabel = (EntityUIText *)entity->labelPtrs2[entity->labelID];

    int val        = 0x1000000;
    EntityUIText *labelPtr = NULL;
    if (curLabel->position.x < 0x1000000) {
        foreach_active(UIText, label)
        {
            if (label->position.x > 0x1000000) {
                int pos = abs(label->position.y - curLabel->position.y);
                if (pos < val) {
                    val = pos;
                    labelPtr = label;
                }
            }
        }
    }
    else {
        foreach_active(UIText, label) {
            if (label->position.x < 0x1000000) {
                int pos = abs(label->position.y - curLabel->position.y);
                if (pos < val) {
                    val      = pos;
                    labelPtr = label;
                }
            }
        }
    }

    for (int i = 0; i < entity->labelCount; ++i) {
        if (entity->labelPtrs[i] == (Entity *)labelPtr && entity->labelPtrs[i]->position.x == labelPtr->position.x) {
            EntityUIText *label = (EntityUIText *)entity->labelPtrs2[entity->labelID];
            if (label)
                label->highlighted = false;
            EntityUIText *label2 = (EntityUIText *)entity->labelPtrs[entity->labelID];
            if (label2)
                label2->highlighted = false;

#if RETRO_USE_PLUS 
            if (--entity->labelID == 28 && !API.CheckDLC(DLC_PLUS))
#else
            if (--entity->labelID == 28)
#endif
                --entity->labelID;
            if (entity->labelID < 0)
                entity->labelID = entity->labelCount - 1;

            label = (EntityUIText *)entity->labelPtrs2[entity->labelID];
            if (label)
                label->highlighted = true;
            label = (EntityUIText *)entity->labelPtrs[entity->labelID];
            if (label)
                label->highlighted = true;
        }
    }
}

void LevelSelect_Unknown7(void)
{
    RSDK_THIS(LevelSelect);
    EntityUIText *curLabel = (EntityUIText *)entity->labelPtrs[entity->labelID];
    if (!curLabel)
        curLabel = (EntityUIText *)entity->labelPtrs2[entity->labelID];
    if (curLabel->selectable) {
        char buffer[32];
        RSDK.GetCString(buffer, &curLabel->tag);
        RSDK.LoadScene(buffer, "");
        RSDK_sceneInfo->listPos += curLabel->data0;

        if (entity->labelID == entity->labelCount - 4) {
            RSDK_sceneInfo->listPos += entity->field_18C;
        }
        else if (entity->labelID == entity->labelCount - 3) {
            RSDK_sceneInfo->listPos += entity->field_190;
        }
#if RETRO_USE_PLUS 
        else if (globals->gameMode == MODE_ENCORE) {
            RSDK_sceneInfo->listPos = Zone_GetEncoreStageID();
        }
#endif

        int p1ID = 0;
        if (entity->playerID > 0)
            p1ID = 1 << (entity->playerID - 1);

        int p2ID = 0;
        if (entity->player2ID > 0)
            p2ID = 1 << (entity->player2ID - 1);
        globals->playerID = p1ID | (p2ID << 8);

        if ((globals->playerID & 0xFF) == ID_KNUCKLES && curLabel->data0 == 15)
            ++RSDK_sceneInfo->listPos;
        entity->timer     = 0;
        entity->state     = LevelSelect_Unknown3;
        entity->stateDraw = LevelSelect_StateDraw_Fade;
        Music_FadeOut(0.1);
    }
    else {
        RSDK.PlaySFX(LevelSelect->sfxFail, 0, 255);
    }
}

void LevelSelect_EditorDraw(void)
{

}

void LevelSelect_EditorLoad(void)
{

}

void LevelSelect_Serialize(void)
{

}

