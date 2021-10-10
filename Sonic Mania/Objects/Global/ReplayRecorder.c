#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectReplayRecorder *ReplayRecorder;

void ReplayRecorder_Update(void)
{
    RSDK_THIS(ReplayRecorder);
    if (!entity->paused) {
        StateMachine_Run(entity->state);
    }
}

void ReplayRecorder_LateUpdate(void)
{
    RSDK_THIS(ReplayRecorder);
    if (!entity->paused) {
        StateMachine_Run(entity->stateLate);

        int *buffer = NULL;
        if (RSDK.GetEntityID(entity) == SLOT_REPLAYRECORDER_W)
            buffer = ReplayRecorder->writeBuffer;
        else
            buffer = ReplayRecorder->readBuffer;
        if (entity->replayFrame > buffer[4] && entity->state) {
            ReplayRecorder_Stop(entity);
        }

        if (ReplayRecorder->dword144 && !entity->player->playerID && entity->state == ReplayRecorder_None_Record) {
            if (entity->field_88 <= 0) {
                if (!entity->field_88) {
                    ReplayRecorder_Stop(entity);
                    ReplayRecorder_Rewind(entity);
                    entity->field_88 = -1;
                }
            }
            else {
                entity->field_88--;
            }
        }
    }
}

void ReplayRecorder_StaticUpdate(void)
{
    if (globals->gameMode == MODE_TIMEATTACK) {
        if (RSDK_sceneInfo->state & ENGINESTATE_REGULAR)
            ++ReplayRecorder->frameCounter;

        if (Zone) {
            if (!ReplayRecorder->initialized) {
                TitleCard->finishedCB       = ReplayRecorder_TitleCardCB;
                TimeAttackGate->startCB     = ReplayRecorder_ResumeStartCB;
                TimeAttackGate->endCB       = ReplayRecorder_FinishCB;
                ActClear->bufferMove_CB     = ReplayRecorder_Buffer_Move;
                ActClear->saveReplay_CB     = ReplayRecorder_SaveReplay;
                ReplayRecorder->initialized = true;
            }

            if (ReplayRecorder->dword124) {
                EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                ReplayRecorder_StartRecording(player);
                ReplayRecorder->dword124 = false;
                ReplayRecorder->dword130 = true;
            }

            if (ReplayRecorder->dword128) {
                EntityPlayer *player = NULL;
                if (Player->playerCount <= 1) {
                    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

                    player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                    API.SetAchievementStatus(false);
                    API.SetStatsStatus(false);
                    TimeAttackGate->debugEnabled = true;
                    TimeAttackData->dbRank       = param->replayRank;
                }
                else {
                    player = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                }

                EntityReplayRecorder *recorder2 = (EntityReplayRecorder *)ReplayRecorder->recorder_w;
                ReplayRecorder_Rewind(recorder2);
                ReplayRecorder_Play(player);
                ReplayRecorder->dword128 = false;
                ReplayRecorder->dword134 = true;
            }

            EntityReplayRecorder *recorder1 = (EntityReplayRecorder *)ReplayRecorder->recorder_r;
            EntityPlayer *playerR1          = recorder1->player;
            if (playerR1) {
                recorder1->storedState = playerR1->state;
                recorder1->storedAnim  = playerR1->playerAnimator.animationID;
                recorder1->storedFrame = playerR1->playerAnimator.frameID;
            }

            EntityReplayRecorder *recorder2 = (EntityReplayRecorder *)ReplayRecorder->recorder_w;
            EntityPlayer *playerR2          = recorder2->player;
            if (recorder2->playing && playerR2)
                playerR2->state = ReplayRecorder_PlayerState;

            if ((RSDK_controller->keyStart.press || RSDK_unknown->field_10) && RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
                EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                bool32 flag                = true;
                if (ActClear && ActClear->dword34)
                    flag = false;
                if (!RSDK.GetEntityCount(TitleCard->objectID, 0) && !pauseMenu->objectID && flag) {
                    RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                    pauseMenu->triggerPlayer = RSDK.GetEntityID(RSDK_sceneInfo->entity);
                    if (globals->gameMode == MODE_COMPETITION)
                        pauseMenu->disableRestart = true;
                }
            }
        }
    }
}

void ReplayRecorder_Draw(void)
{
    RSDK_THIS(ReplayRecorder);
    if (entity->playing)
        ReplayRecorder_DrawGhostDisplay();
}

void ReplayRecorder_Create(void *data)
{
    RSDK_THIS(ReplayRecorder);
    entity->active     = ACTIVE_NEVER;
    entity->drawOrder  = 13;
    entity->inkEffect  = INK_NONE;
    entity->field_88   = -1;
    entity->drawFX     = FX_FLIP;
    entity->alphaStore = 256;
    entity->visible    = globals->gameMode == MODE_TIMEATTACK;
}

void ReplayRecorder_StageLoad(void)
{
    ReplayRecorder->replayID    = 0;
    ReplayRecorder->replayRowID = -1;
    if (globals->gameMode == MODE_TIMEATTACK) {
        ReplayRecorder->active       = ACTIVE_ALWAYS;
        ReplayRecorder->frameCounter = -1;
        ReplayRecorder->dword12C     = 0;
        ReplayRecorder->initialized  = false;
        ReplayRecorder->dword130     = 0;
        ReplayRecorder->dword134     = 0;
        ReplayRecorder->dword138     = 0;
        ReplayRecorder->dword13C     = 0;
        ReplayRecorder->dword140     = 0;
        ReplayRecorder->dword144     = 0;
        ReplayRecorder->dword148     = 0;
        ReplayRecorder->loadCallback = NULL;
        ReplayRecorder->buffer       = NULL;
        memset(ReplayRecorder->filename, 0, 0x100);
        ReplayRecorder->writeBuffer   = globals->replayWriteBuffer;
        ReplayRecorder->frameBuffer_w = &globals->replayWriteBuffer[14];
        ReplayRecorder->readBuffer    = globals->replayReadBuffer;
        ReplayRecorder->frameBuffer_r = &globals->replayReadBuffer[14];
        if (Zone) {
            if (!RSDK_sceneInfo->inEditor)
                ReplayRecorder_SetupActions();
            RSDK.SetRandKey(1624633040);

            RSDK.ResetEntitySlot(SLOT_REPLAYRECORDER_W, ReplayRecorder->objectID, NULL);
            EntityReplayRecorder *recorder_r = RSDK_GET_ENTITY(SLOT_REPLAYRECORDER_W, ReplayRecorder);
            recorder_r->field_84             = 0x9247;
            ReplayRecorder->recorder_r       = (Entity *)recorder_r;

            RSDK.ResetEntitySlot(SLOT_REPLAYRECORDER_R, ReplayRecorder->objectID, NULL);
            EntityReplayRecorder *recorder_w = RSDK_GET_ENTITY(SLOT_REPLAYRECORDER_R, ReplayRecorder);
            recorder_w->field_84             = 0x9247;
            ReplayRecorder->recorder_w       = (Entity *)recorder_w;

            ReplayRecorder->dword124 = false;
            ReplayRecorder->dword128 = false;
            int *buffer              = NULL;
            if (RSDK.GetEntityID(ReplayRecorder->recorder_w) == SLOT_REPLAYRECORDER_W)
                buffer = ReplayRecorder->writeBuffer;
            else
                buffer = ReplayRecorder->readBuffer;

            EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
            if (param->viewReplay && buffer[3]) {
                if (param->showGhost) {
                    globals->playerID         = (globals->playerID & 0xFF) | ((globals->playerID & 0xFF) << 8);
                    Player->configureGhost_CB = ReplayRecorder_ConfigureGhost_CB;
                }
                else {
                    globals->playerID &= 0xFF;
                }
            }
            LogHelpers_Print("CharID: %08x", globals->playerID);
        }
    }
}

void ReplayRecorder_TitleCardCB(void)
{
    int *buffer = NULL;
    if (RSDK.GetEntityID(ReplayRecorder->recorder_w) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;

    if (Player->playerCount != 1 || !buffer[3])
        ReplayRecorder->dword124 = true;
    if (buffer[3] == 1)
        ReplayRecorder->dword128 = true;
}

void ReplayRecorder_Resume(EntityReplayRecorder *recorder)
{
    LogHelpers_Print("ReplayRecorder_Resume()");
    recorder->paused          = false;
    recorder->player->visible = true;
}

void ReplayRecorder_ResumeStartCB(void)
{
    if (ReplayRecorder->dword130) {
        ((EntityReplayRecorder *)ReplayRecorder->recorder_r)->field_78 = 2;
    }
    EntityReplayRecorder *recorder = (EntityReplayRecorder *)ReplayRecorder->recorder_w;
    if (ReplayRecorder->dword134 && recorder->playing) {
        if (ReplayRecorder->dword148) {
            ReplayRecorder_Resume(recorder);
            ReplayRecorder->dword140 = 1;
            return;
        }
        ReplayRecorder_SeekFunc(recorder);
    }
    ReplayRecorder->dword140 = 1;
}

void ReplayRecorder_FinishCB(void)
{
    EntityReplayRecorder *entity = (EntityReplayRecorder *)ReplayRecorder->recorder_r;
    if (ReplayRecorder->dword130) {
        entity->field_88 = 120;
    }
    ReplayRecorder->dword144 = 1;
}

void ReplayRecorder_Buffer_Move(void)
{
    int *buffer = NULL;
    if (RSDK.GetEntityID(ReplayRecorder->recorder_w) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;
    if (buffer[3]) {
        if (buffer[4] < (((EntityReplayRecorder *)ReplayRecorder->recorder_r)->field_84 - 1)) {
            memset(globals->replayTempWBuffer, 0, 0x100000);
            LogHelpers_Print("Buffer_Move(0x%08x, 0x%08x)", globals->replayTempWBuffer, buffer);
            memcpy(globals->replayTempWBuffer, buffer, 0x100000);
            memset(buffer, 0, 0x100000);
            ReplayRecorder_Buffer_PackInPlace(globals->replayTempWBuffer);
            HUD->replaySaveEnabled = true;
        }
    }
}

void ReplayRecorder_SaveReplayDLG_NoCB(void)
{
    HUD->replaySaveEnabled = true;
    ActClear->field_2C     = 0;
    ActClear->field_14     = 0;
}

void ReplayRecorder_SaveReplayDLG_YesCB(void)
{
    ReplayRecorder->replayID    = 0;
    ReplayRecorder->replayRowID = -1;
    int mins                    = RSDK_sceneInfo->minutes;
    int secs                    = RSDK_sceneInfo->seconds;
    int millisecds              = RSDK_sceneInfo->milliseconds;
    LogHelpers_Print("Bout to create ReplayDB entry...");
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    int rowID              = ReplayRecorder_AddReplayID(param->actID, param->zoneID, param->characterID, (millisecds + 100 * (secs + 60 * mins)),
                                           RSDK_sceneInfo->filter == SCN_FILTER_ENCORE);
    if (rowID == -1) {
        LogHelpers_Print("Table row ID invalid! %d", -1);
        ReplayRecorder_SavedReplay(false);
    }
    else {
        uint uuid                   = API.GetUserDBRowUUID(globals->replayTableID, rowID);
        ReplayRecorder->replayRowID = rowID;
        ReplayRecorder->replayID    = uuid;
        char fileName[0x20];
        sprintf(fileName, "Replay_%08X.bin", uuid);
        LogHelpers_Print("Replay Filename: %s", fileName);
        UIWaitSpinner_Wait();
        ReplayRecorder->dword12C = 1;
        ReplayRecorder_Buffer_SaveFile(fileName, globals->replayTempWBuffer);
        HUD->replaySaveEnabled = 0;
    }
}

void ReplayRecorder_Unknown6(void)
{
    TextInfo buffer;
    INIT_TEXTINFO(buffer);
    Localization_GetString(&buffer, STR_SAVEREPLAY);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&buffer);
    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, ReplayRecorder_SaveReplayDLG_NoCB, true);
        UIDialog_AddButton(DIALOG_YES, dialog, ReplayRecorder_SaveReplayDLG_YesCB, true);
        UIDialog_Setup(dialog);
    }
}

void ReplayRecorder_SaveReplay(void)
{
    if (globals->replayTempWBuffer[3]) {
        LogHelpers_Print("Saving replay...");
        foreach_all(HUD, hud) { foreach_break; }
        RSDK.SetSpriteAnimation(HUD->hudMappings, 11, &hud->taData2, true, 0);
        RSDK.PlaySfx(HUD->sfxClick, 0, 255);
        EntityDialogRunner *runner = (EntityDialogRunner *)RSDK.CreateEntity(DialogRunner->objectID, DialogRunner_HandleCallback, 0, 0);
        runner->callback           = ReplayRecorder_Unknown6;
        runner->timer              = 45;
        runner->isPermanent        = true;
    }
    else {
        LogHelpers_Print("Can't save replay! No data available");
        ActClear->field_14 = 0;
    }
}

void ReplayRecorder_SavedReplay(bool32 status)
{
    if (status) {
        LogHelpers_Print("Replay save successful!");
        ReplayRecorder_SaveReplayDB(ReplayRecorder_WaitWhileReplaySaves);
    }
    else {
        if (ReplayRecorder->replayRowID != -1)
            API.RemoveDBRow(globals->replayTableID, ReplayRecorder->replayRowID);
        TextInfo buffer;
        INIT_TEXTINFO(buffer);
        Localization_GetString(&buffer, STR_NOREPLAYSPACE);
        EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&buffer);
        if (dialog) {
            UIDialog_AddButton(2, dialog, 0, 1);
            UIDialog_Setup(dialog);
        }
        UIWaitSpinner_Wait2();
        ActClear->field_14     = 0;
        ActClear->field_2C     = 0;
        HUD->replaySaveEnabled = 1;
    }
}

void ReplayRecorder_WaitWhileReplaySaves(bool32 a1)
{
    if (a1) {
        if (TimeAttackData->rowID == -1) {
            UIWaitSpinner_Wait2();
            ActClear->field_14 = 0;
            foreach_all(HUD, hud) { foreach_break; }
            RSDK.SetSpriteAnimation(HUD->hudMappings, 10, &hud->taData2, true, 0);
            RSDK.PlaySfx(HUD->sfxStarpost, 0, 255);
            HUD->replaySaveEnabled = 0;
        }
        else {
            API.SetUserDBValue(globals->taTableID, TimeAttackData->rowID, 4, "replayID", &ReplayRecorder->replayID);
            TimeAttackData_SaveTimeAttackDB(ReplayRecorder_Unknown10);
        }
    }
    else {
        TextInfo buffer;
        INIT_TEXTINFO(buffer);
        char fileName[0x20];
        sprintf(fileName, "Replay_%08X.bin", ReplayRecorder->replayID);
        if (ReplayRecorder->replayRowID != -1)
            API.RemoveDBRow(globals->replayTableID, ReplayRecorder->replayRowID);
        API.DeleteUserFile(fileName, 0);
        Localization_GetString(&buffer, STR_NOREPLAYSPACE);
        EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&buffer);
        if (dialog) {
            UIDialog_AddButton(2, dialog, 0, 1);
            UIDialog_Setup(dialog);
        }
        UIWaitSpinner_Wait2();
        ActClear->field_14     = 0;
        ActClear->field_2C     = 0;
        HUD->replaySaveEnabled = 1;
    }
}

void ReplayRecorder_Unknown10(int status)
{
    UIWaitSpinner_Wait2();
    ActClear->field_14 = 0;
    if (!status) {
        ActClear->field_2C     = 0;
        HUD->replaySaveEnabled = true;
    }
    else {
        foreach_all(HUD, hud) { foreach_break; }
        RSDK.SetSpriteAnimation(HUD->hudMappings, 10, &hud->taData2, true, 0);
        RSDK.PlaySfx(HUD->sfxStarpost, 0, 255);
        HUD->replaySaveEnabled = 0;
    }
}

void ReplayRecorder_Buffer_PackInPlace(int *tempWriteBuffer)
{
    LogHelpers_Print("Buffer_PackInPlace(%08x)", tempWriteBuffer);

    if (*tempWriteBuffer == 0xF6057BED) {
        if (tempWriteBuffer[2] == 1) {
            LogHelpers_Print("Buffer_Ppack ERROR: Buffer is already packed");
        }
        else {
            int frameSize        = 56;
            int uncompressedSize = 28 * (tempWriteBuffer[4] + 2);

            byte *framePtr       = (byte *)&tempWriteBuffer[14];
            byte *compressedData = (byte *)&tempWriteBuffer[14];
            for (int f = 0; f < tempWriteBuffer[4]; ++f) {
                byte uncompressedData[28];
                memcpy(uncompressedData, framePtr, 28 * sizeof(byte));
                int *dataPtr = (int *)uncompressedData;

                // set velocity to zero
                dataPtr[2] = 0;
                dataPtr[3] = 0;

                int size = ReplayDB_Buffer_PackEntry(compressedData, uncompressedData);
                framePtr += 28;
                compressedData += size;
                frameSize += size;
            }
            LogHelpers_Print("Packed %d frames: %luB -> %luB", tempWriteBuffer[4], uncompressedSize, frameSize);
            tempWriteBuffer[11] = frameSize;
            tempWriteBuffer[2]  = 1;
        }
    }
    else {
        LogHelpers_Print("Buffer_Pack ERROR: Signature does not match");
    }
}

void ReplayRecorder_Buffer_Unpack(int *readBuffer, int *tempReadBuffer)
{
    LogHelpers_Print("Buffer_Unpack(0x%08x, 0x%08x)", readBuffer, tempReadBuffer);

    byte *compressedData = (byte *)&tempReadBuffer[14];
    if (*tempReadBuffer == 0xF6057BED) {
        if (tempReadBuffer[2] == 1) {
            int compSize             = tempReadBuffer[11];
            readBuffer[0]            = tempReadBuffer[0];
            readBuffer[1]            = tempReadBuffer[1];
            readBuffer[2]            = tempReadBuffer[2];
            readBuffer[3]            = tempReadBuffer[3];
            readBuffer[4]            = tempReadBuffer[4];
            readBuffer[5]            = tempReadBuffer[5];
            int frameSize            = 28 * (tempReadBuffer[4] + 2);
            byte *uncompressedBuffer = (byte *)&readBuffer[14];
            for (int i = 0; i < tempReadBuffer[4]; ++i) {
                int size = ReplayDB_Buffer_UnpackEntry(uncompressedBuffer, compressedData);
                compressedData += size;
                uncompressedBuffer += 28;
            }
            LogHelpers_Print("Unpacked %d frames: %luB -> %luB", tempReadBuffer[4], compSize, frameSize);
            readBuffer[2]  = 0;
            readBuffer[11] = frameSize;
            memset(tempReadBuffer, 0, 0x100000);
        }
        else {
            LogHelpers_Print("Buffer_Unpack ERROR: Buffer is not packed");
        }
    }
    else {
        LogHelpers_Print("Buffer_Unpack ERROR: Signature does not match");
    }
}

void ReplayRecorder_Buffer_SaveFile(const char *fileName, int *buffer)
{
    LogHelpers_Print("Buffer_SaveFile(%s, %08x)", fileName, buffer);
    if (buffer[3]) {
        ReplayRecorder->saveFinishPtr = ReplayRecorder_SavedReplay;
        API_SaveUserFile(fileName, buffer, buffer[11], ReplayRecorder_SetReplayStatus, true);
    }
    else {
        LogHelpers_Print("Attempted to save an empty replay buffer");
        ReplayRecorder_SavedReplay(0);
    }
}

void ReplayRecorder_SetReplayStatus(int status)
{
    if (ReplayRecorder->saveFinishPtr)
        ReplayRecorder->saveFinishPtr((status == STATUS_OK));
    ReplayRecorder->saveFinishPtr = 0;
}

void ReplayRecorder_Buffer_LoadFile(const char *fileName, void *buffer, void (*callback)(bool32))
{
    LogHelpers_Print("Buffer_LoadFile(%s, %08x)", fileName, buffer);
    memset(buffer, 0, 0x100000);
    ReplayRecorder->buffer       = buffer;
    ReplayRecorder->loadCallback = callback;
    strcpy(ReplayRecorder->filename, fileName);
    API_LoadUserFile(fileName, buffer, 0x100000, ReplayRecorder_Load_CB);
}

void ReplayRecorder_Load_CB(int status)
{
    if (ReplayRecorder->loadCallback)
        ReplayRecorder->loadCallback(status == STATUS_OK);
    ReplayRecorder->loadCallback = NULL;
    ReplayRecorder->buffer       = NULL;
    memset(ReplayRecorder->filename, 0, 0x100);
}

void ReplayRecorder_ConfigureGhost_CB(void)
{
    RSDK_THIS(Player);
    LogHelpers_Print("ConfigureGhost_CB()");
    LogHelpers_Print("Ghost Slot %d", entity->playerID);
    RSDK.PrintVector2(0, "Ghost pos ", entity->position.x, entity->position.y);
    entity->isGhost        = true;
    entity->stateInput     = StateMachine_None;
    entity->state          = ReplayRecorder_PlayerState;
    entity->sidekick       = false;
    entity->interaction    = false;
    entity->tileCollisions = false;
    entity->visible        = true;
    entity->alpha          = 255;
}

void ReplayRecorder_SetupActions(void)
{
    for (int i = 0; i < 64; ++i) {
        ReplayRecorder->actions[i] = StateMachine_None;
    }
    ReplayRecorder->actions[3]  = StateMachine_None; // Current_Unknown16;
    ReplayRecorder->actions[4]  = StateMachine_None; // Current_Unknown13;
    ReplayRecorder->actions[5]  = StateMachine_None; // Current_Unknown14;
    ReplayRecorder->actions[6]  = StateMachine_None; // Current_Unknown15;
    ReplayRecorder->actions[7]  = Cylinder_Player_State_Unknown2;
    ReplayRecorder->actions[8]  = Cylinder_Player_State_Unknown3;
    ReplayRecorder->actions[9]  = Cylinder_Player_State_Unknown4;
    ReplayRecorder->actions[10] = Cylinder_Player_State_Unknown1;
    ReplayRecorder->actions[13] = StateMachine_None; // GymBar_Unknown6;
    ReplayRecorder->actions[14] = StateMachine_None; // GymBar_Unknown5;
    ReplayRecorder->actions[15] = StateMachine_None; // GymBar_Unknown7;
    ReplayRecorder->actions[16] = Ice_State_FrozenPlayer;
    ReplayRecorder->actions[17] = OOZSetup_PlayerState_OilFall;
    ReplayRecorder->actions[18] = OOZSetup_PlayerState_OilPool;
    ReplayRecorder->actions[19] = OOZSetup_PlayerState_OilSlide;
    ReplayRecorder->actions[20] = OOZSetup_PlayerState_OilStrip;
    ReplayRecorder->actions[21] = PlaneSeeSaw_PlayerState_ToBG;
    ReplayRecorder->actions[22] = PlaneSeeSaw_PlayerState_ToFG;
    ReplayRecorder->actions[23] = Player_State_Air;
    ReplayRecorder->actions[24] = Player_State_BubbleBounce;
    ReplayRecorder->actions[25] = Player_State_FlyCarried;
    ReplayRecorder->actions[26] = Player_State_KnuxWallClimb;
    ReplayRecorder->actions[27] = Player_State_Crouch;
    ReplayRecorder->actions[28] = Player_State_Die;
    ReplayRecorder->actions[29] = Player_State_MightyHammerDrop;
    ReplayRecorder->actions[30] = Player_State_DropDash;
    ReplayRecorder->actions[31] = Player_State_Drown;
    ReplayRecorder->actions[32] = Player_State_TailsFlight;
    ReplayRecorder->actions[33] = Player_State_FlyIn;
    ReplayRecorder->actions[34] = Player_State_KnuxGlideDrop;
    ReplayRecorder->actions[35] = Player_State_KnuxGlideLeft;
    ReplayRecorder->actions[36] = Player_State_KnuxGlideRight;
    ReplayRecorder->actions[37] = Player_State_GlideSlide;
    ReplayRecorder->actions[38] = Player_State_Ground;
    ReplayRecorder->actions[39] = Player_State_StartJumpIn;
    ReplayRecorder->actions[40] = Player_State_Hit;
    ReplayRecorder->actions[41] = Player_State_KnuxLedgePullUp;
    ReplayRecorder->actions[42] = Player_State_LookUp;
    ReplayRecorder->actions[43] = Player_State_OuttaHere;
    ReplayRecorder->actions[44] = Player_State_Peelout;
    ReplayRecorder->actions[45] = Player_State_Roll;
    ReplayRecorder->actions[46] = Player_State_Spindash;
    ReplayRecorder->actions[47] = Player_Unknown9;
    ReplayRecorder->actions[48] = Player_State_None;
    ReplayRecorder->actions[49] = Player_State_Transform;
    ReplayRecorder->actions[50] = Player_Unknown7;
    ReplayRecorder->actions[51] = Player_State_ForceRoll_Air;
    ReplayRecorder->actions[52] = Player_State_ForceRoll_Ground;
    ReplayRecorder->actions[53] = Player_State_Victory;
    ReplayRecorder->actions[54] = SizeLaser_P2JumpInShrink;
    ReplayRecorder->actions[55] = SizeLaser_PlayerState_Grow;
    ReplayRecorder->actions[56] = SizeLaser_P2JumpInGrow;
}

void ReplayRecorder_SetupWriteBuffer(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    int *buffer            = ReplayRecorder->writeBuffer;
    buffer[0]              = 0xF6057BED;    // signature
    buffer[1]              = RETRO_GAMEVER; // game version
    buffer[2]              = 0;
    buffer[8]              = param->characterID;
    buffer[6]              = param->zoneID;
    buffer[7]              = param->actID;
    buffer[9]              = RSDK_sceneInfo->filter == SCN_FILTER_ENCORE;
    buffer[10]             = Zone->timer;
    buffer[11]             = 56;
    buffer[5]              = ReplayRecorder->frameCounter;
    LogHelpers_Print("characterID = %d", buffer[8]);
    LogHelpers_Print("zoneID = %d", buffer[6]);
    LogHelpers_Print("act = %d", buffer[7]);
    LogHelpers_Print("isPlusLayout = %d", buffer[9]);
    LogHelpers_Print("oscillation = %d", buffer[10]);
    buffer[3] = true;
}

void ReplayRecorder_DrawGhostDisplay(void)
{
    RSDK_THIS(ReplayRecorder);
    EntityPlayer *player = entity->player;
    if (!entity->state) {
        if (entity->alphaStore > 0)
            entity->alphaStore -= 4;
    }
    if (!RSDK_sceneInfo->currentScreenID && entity->alphaStore) {
        entity->inkEffect = INK_NONE;
        int screenX       = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        int screenY       = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;

        Hitbox hitbox;
        hitbox.left   = -(RSDK_screens->width >> 1);
        hitbox.right  = RSDK_screens->width >> 1;
        hitbox.top    = -(RSDK_screens->height >> 1);
        hitbox.bottom = RSDK_screens->height >> 1;
        if (!MathHelpers_PointInHitbox(FLIP_NONE, screenX, screenY, &hitbox, player->position.x, player->position.y)) {
            Vector2 drawPos;
            drawPos.x = 0;
            drawPos.y = 0;

            int distX = abs(player->position.x - screenX);
            int distY = abs(player->position.y - screenY);
            int rad   = MathHelpers_Unknown6((distX >> 16) * (distX >> 16) + (distY >> 16) * (distY >> 16));
            rad       = clampVal(rad, 100, 2000);

            int val       = 12 - 4 * (3 * rad - 300) / 2000;
            hitbox.right  = hitbox.right - 8 - (val + 24);
            hitbox.bottom = hitbox.bottom - 8 - (val + 24);
            hitbox.top += val + 24 + 8;
            hitbox.left += val + 24 + 8;

            Vector2 screenPos;
            screenPos.x = screenX;
            screenPos.y = screenY;
            if (MathHelpers_Unknown14(&drawPos, player->position.x, player->position.y, screenPos, hitbox)) {
                int angle = RSDK.ATan2(player->position.x - drawPos.x, player->position.y - drawPos.y);
                int x     = ((val + 18) * RSDK.Cos256(angle) << 8) + drawPos.x;
                int y     = ((val + 18) * RSDK.Sin256(angle) << 8) + drawPos.y;
                DrawHelpers_DrawDebug4(0xC0E0E0, 2, entity->alphaStore >> 1, x, y, (RSDK.Cos256(angle) << 10) + x, (RSDK.Sin256(angle) << 10) + y);
                RSDK.DrawCircle(drawPos.x, drawPos.y, val + 16, 0xC0E0E0, entity->alphaStore >> 1, INK_ALPHA, 0);

                entity->alpha     = entity->alphaStore;
                entity->inkEffect = INK_ADD;
                entity->drawFX    = FX_FLIP | FX_ROTATE | FX_SCALE;
                entity->scale.x   = 0x100;
                entity->scale.y   = 0x100;
                if (player->tailSpriteIndex != 0xFFFF) {
                    entity->rotation   = player->tailRotation;
                    entity->direction  = player->tailDirection;
                    entity->velocity.x = player->velocity.x;
                    entity->velocity.y = player->velocity.y;
                    RSDK.DrawSprite(&player->tailAnimator, &drawPos, false);
                }
                entity->rotation  = player->rotation;
                entity->direction = player->direction;
                RSDK.DrawSprite(&player->playerAnimator, &drawPos, false);

                entity->drawFX     = FX_NONE;
                entity->alpha      = 255;
                entity->velocity.x = 0;
                entity->velocity.y = 0;
            }
        }
    }
}

void ReplayRecorder_Record(EntityReplayRecorder *recorder, EntityPlayer *player)
{
    LogHelpers_Print("ReplayRecorder_Record()");
    if (player)
        recorder->player = player;
    recorder->storedState = player->state;
    recorder->state       = ReplayRecorder_None_Record;
    recorder->stateLate   = ReplayRecorder_RecordFrameData;
    recorder->storedAnim  = player->playerAnimator.animationID;
    recorder->storedFrame = player->playerAnimator.frameID;
}

void ReplayRecorder_StartRecording(EntityPlayer *player)
{
    EntityReplayRecorder *recorder = (EntityReplayRecorder *)ReplayRecorder->recorder_r;
    LogHelpers_Print("ReplayRecorder_StartRecording()");
    recorder->active = ACTIVE_NORMAL;
    memset(globals->replayTempWBuffer, 0, 0x100000);
    memset(globals->replayWriteBuffer, 0, 0x100000);
    ReplayRecorder_Rewind(recorder);
    ReplayRecorder_SetupWriteBuffer();
    ReplayRecorder_Record(recorder, player);
}

void ReplayRecorder_Play(EntityPlayer *player)
{
    LogHelpers_Print("ReplayRecorder_Play()");
    EntityReplayRecorder *recorder = (EntityReplayRecorder *)ReplayRecorder->recorder_w;

    int *buffer = NULL;
    if (RSDK.GetEntityID(&ReplayRecorder->recorder_w) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;

    if (buffer[3]) {
        recorder->active = ACTIVE_NORMAL;
        if (player) {
            recorder->player   = player;
            recorder->field_60 = player->state;
        }
        recorder->playing = false;
        if (player->playerID) {
            recorder->playing        = true;
            ReplayRecorder->dword138 = true;
        }
        else {
            Zone->timer              = buffer[10];
            player->stateInputReplay = ReplayRecorder_PlayBackInput;
            player->controllerID     = 2;
            RSDK.AssignControllerID(2, CONT_UNASSIGNED);
            ReplayRecorder->dword13C = 1;
        }
        recorder->state     = ReplayRecorder_StatePlay;
        recorder->stateLate = StateMachine_None;
        // Set the dim timer to dim after 15 mins instead of the usual 5
        RSDK.SetSettingsValue(SETTINGS_DIMTIMER, 15 * 60 * 60);
    }
    else {
        LogHelpers_Print("No replay to play");
    }
}

void ReplayRecorder_Rewind(EntityReplayRecorder *recorder)
{
    LogHelpers_Print("ReplayRecorder_Rewind()");
    recorder->replayFrame = 0;
}

void ReplayRecorder_Seek(EntityReplayRecorder *recorder, uint frame)
{
    LogHelpers_Print("ReplayRecorder_Seek(%u)", frame);
    recorder->replayFrame = frame;

    int *frameBuffer = NULL;
    if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
        frameBuffer = ReplayRecorder->frameBuffer_w;
    else
        frameBuffer = ReplayRecorder->frameBuffer_r;
    byte *byteBuf = (byte *)frameBuffer;
    int newFrame  = frame;

    byte *framePtr = &byteBuf[28 * frame];
    while (framePtr[0] != 1) {
        if (framePtr[0] == 3)
            break;
        if (newFrame > frame)
            break;
        framePtr -= 28;
        --newFrame;
    }

    if (newFrame <= frame) {
        ReplayRecorder_ApplyFrameData(recorder, framePtr);
        if (newFrame < frame) {
            int count = frame - newFrame;
            byte *ptr = &byteBuf[28 * newFrame];
            for (int i = 0; i < count; ++i) {
                ptr += 28;
                ReplayRecorder_Unknown19(recorder, ptr);
            }
        }
    }
}

void ReplayRecorder_SeekFunc(EntityReplayRecorder *recorder)
{
    int *buffer2 = NULL;
    if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
        buffer2 = ReplayRecorder->frameBuffer_w;
    else
        buffer2 = ReplayRecorder->frameBuffer_r;
    byte *byteBuf = (byte *)buffer2;

    for (int f = 0; f < recorder->field_84; ++f) {
        if (byteBuf[f * 28] == 3) {
            ReplayRecorder_Seek(recorder, f);
            break;
        }
    }
}

void ReplayRecorder_Stop(EntityReplayRecorder *recorder)
{
    LogHelpers_Print("ReplayRecorder_Stop()");
    EntityPlayer *player = recorder->player;
    recorder->state      = StateMachine_None;
    recorder->stateLate  = StateMachine_None;
    if (player) {
        if (player->stateInputReplay == ReplayRecorder_PlayBackInput)
            player->stateInputReplay = StateMachine_None;
    }
}

void ReplayRecorder_SetGimmickState(EntityReplayRecorder *recorder, bool32 flag)
{
    EntityPlayer *player = recorder->player;
    if (player) {
        player->tailSpriteIndex = -1;
        if (RSDK.CheckStageFolder("MMZ") || RSDK.CheckStageFolder("PSZ2")) {
            if (flag) {
                if (RSDK.CheckStageFolder("MMZ")) {
                    switch (player->characterID) {
                        case ID_TAILS:
                            player->spriteIndex     = SizeLaser->tailsIndex;
                            player->tailSpriteIndex = SizeLaser->tailSpriteIndex;
                            break;
                        case ID_KNUCKLES: player->spriteIndex = SizeLaser->knuxIndex; break;
                        case ID_MIGHTY: player->spriteIndex = SizeLaser->mightyIndex; break;
                        case ID_RAY: player->spriteIndex = SizeLaser->rayIndex; break;
                        default: player->spriteIndex = SizeLaser->sonicIndex; break;
                    }
                }
                else if (RSDK.CheckStageFolder("PSZ2")) {
                    player->spriteIndex = Ice->aniFrames;
                }
            }
            else {
                switch (player->characterID) {
                    case ID_TAILS:
                        player->spriteIndex     = Player->tailsSpriteIndex;
                        player->tailSpriteIndex = Player->tailsTailsSpriteIndex;
                        break;
                    case ID_KNUCKLES: player->spriteIndex = Player->knuxSpriteIndex; break;
                    case ID_MIGHTY: player->spriteIndex = Player->mightySpriteIndex; break;
                    case ID_RAY: player->spriteIndex = Player->raySpriteIndex; break;
                    default: player->spriteIndex = Player->sonicSpriteIndex; break;
                }
            }
        }
    }
}

void ReplayRecorder_ApplyFrameData(EntityReplayRecorder *recorder, byte *buffer)
{
    EntityPlayer *player = recorder->player;
    int *bufferPtr       = (int *)buffer;
    if (player) {
        player->position.x = bufferPtr[1] & 0xFFFF0000;
        player->position.y = bufferPtr[2] & 0xFFFF0000;
        player->velocity.x = bufferPtr[3] & 0xFFFF0000;
        player->velocity.y = bufferPtr[4] & 0xFFFF0000;
        player->direction  = buffer[3];
        player->rotation   = bufferPtr[5];
        ReplayRecorder_SetGimmickState(recorder, ((buffer[1] & 8) > 0));
        RSDK.SetSpriteAnimation(player->spriteIndex, buffer[24], &player->playerAnimator, true, buffer[25]);
        player->playerAnimator.animationSpeed = 0;
    }
}

void ReplayRecorder_Unknown19(EntityReplayRecorder *recorder, byte *buffer)
{
    EntityPlayer *player = recorder->player;
    int *bufferPtr       = (int *)buffer;
    if (player) {
        if (buffer[1] & 2) {
            player->position.x = bufferPtr[1] & 0xFFFF0000;
            player->position.y = bufferPtr[2] & 0xFFFF0000;
        }
        if (buffer[1] & 4) {
            player->velocity.x = bufferPtr[3] & 0xFFFF0000;
            player->velocity.y = bufferPtr[4] & 0xFFFF0000;
        }
        if (buffer[1] & 0x20)
            player->rotation = bufferPtr[5];
        if (buffer[1] & 0x10)
            player->direction = buffer[3];
        ReplayRecorder_SetGimmickState(recorder, ((buffer[1] & 8) > 0));

        if (buffer[1] & 0x40) {
            RSDK.SetSpriteAnimation(player->spriteIndex, buffer[24], &player->playerAnimator, true, buffer[25]);
        }
        else if ((buffer[1] & 0x80)) {
            RSDK.SetSpriteAnimation(player->spriteIndex, player->playerAnimator.animationID, &player->playerAnimator, true, buffer[25]);
        }
        player->playerAnimator.animationSpeed = 0;
    }
}

bool32 ReplayRecorder_CheckPlayerGimmickState(EntityReplayRecorder *recorder)
{
    EntityPlayer *player = recorder->player;
    if (!player || (!RSDK.CheckStageFolder("MMZ") && !RSDK.CheckStageFolder("PSZ2")))
        return false;
    if (RSDK.CheckStageFolder("MMZ"))
        return player->isChibi;
    if (RSDK.CheckStageFolder("PSZ2"))
        return player->state == Ice_State_FrozenPlayer;
    return false;
}

void ReplayRecorder_PackFrame(byte *recording)
{
    RSDK_THIS(ReplayRecorder);

    byte frameBuf[28];
    memset(frameBuf, 0, sizeof(frameBuf));
    frameBuf[0]       = 0;
    int *writeBuffer  = ReplayRecorder->writeBuffer;
    int *writeBuffer2 = ReplayRecorder->frameBuffer_w;
    int size          = ReplayDB_Buffer_PackEntry(frameBuf, recording);
    memcpy(&writeBuffer2[7 * entity->replayFrame], recording, 0x28 * sizeof(byte));

    if (writeBuffer[4]) {
        writeBuffer[4] = ((float)(writeBuffer[4] * writeBuffer[4]) + size) / (writeBuffer[4] + 1);
    }
    else {
        writeBuffer[4] = size;
    }
    writeBuffer[11] += size;
    ++entity->replayFrame;
    ++writeBuffer[4];
}

void ReplayRecorder_PlayBackInput(void)
{
    RSDK_THIS(Player);
    EntityReplayRecorder *recorder = (EntityReplayRecorder *)ReplayRecorder->recorder_w;

    int *buffer = NULL;
    if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;

    if (ReplayRecorder->frameCounter >= buffer[5] && entity == recorder->player) {
        int *frameBuffer = NULL;
        if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
            frameBuffer = ReplayRecorder->frameBuffer_w;
        else
            frameBuffer = ReplayRecorder->frameBuffer_r;
        int *playbackBuf = &frameBuffer[7 * recorder->replayFrame];
        byte *bufBytes   = (byte *)playbackBuf;

        bool32 setPos = false;
        if (bufBytes[0]) {
            setPos = (bufBytes[0] == 1 || bufBytes[0] == 3) || (bufBytes[0] == 2 && (bufBytes[1] & 2));
            if ((bufBytes[0] == 1 || bufBytes[0] == 3) || (bufBytes[0] == 2 && (bufBytes[1] & 1))) {
                int inputs                                          = bufBytes[2];
                RSDK_controller[entity->controllerID].keyUp.down    = (inputs & 0x01) > 0;
                RSDK_controller[entity->controllerID].keyDown.down  = (inputs & 0x02) > 0;
                RSDK_controller[entity->controllerID].keyLeft.down  = (inputs & 0x04) > 0;
                RSDK_controller[entity->controllerID].keyRight.down = (inputs & 0x08) > 0;
                RSDK_controller[entity->controllerID].keyA.press    = (inputs & 0x10) > 0;
                RSDK_controller[entity->controllerID].keyA.down     = (inputs & 0x20) > 0;
            }
        }

        ControllerState *controller = &RSDK_controller[entity->controllerID];
        if (controller->keyUp.down)
            controller->keyUp.press = !controller->keyUp.press;
        else
            controller->keyUp.press = false;
        if (controller->keyDown.down)
            controller->keyDown.press = !controller->keyDown.press;
        else
            controller->keyDown.press = false;
        if (controller->keyLeft.down)
            controller->keyLeft.press = !controller->keyLeft.press;
        else
            controller->keyLeft.press = false;
        if (controller->keyRight.down)
            controller->keyRight.press = !controller->keyRight.press;
        else
            controller->keyRight.press = false;

        if (setPos) {
            recorder->storedPos.x = playbackBuf[1];
            recorder->storedPos.y = playbackBuf[2];
        }
    }
}

void ReplayRecorder_Pause(EntityReplayRecorder *recorder)
{
    LogHelpers_Print("ReplayRecorder_Pause()");
    recorder->paused = true;
}

void ReplayRecorder_PlayerState(void)
{
    RSDK_THIS(Player);
    EntityReplayRecorder *recorder = (EntityReplayRecorder *)ReplayRecorder->recorder_w;

    if (recorder->playing) {
        entity->playerAnimator.animationSpeed = 0;

        int *buffer = NULL;
        if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
            buffer = ReplayRecorder->frameBuffer_w;
        else
            buffer = ReplayRecorder->frameBuffer_r;

        byte *frameData = (byte *)&buffer[7 * recorder->replayFrame];
        if (recorder->state) {
            if (frameData[0] == 2) {
                ReplayRecorder_Unknown19(recorder, frameData);
            }
            else if (frameData[0] == 1 || frameData[0] == 3) {
                ReplayRecorder_ApplyFrameData(recorder, frameData);
                if (frameData[0] == 3 && !ReplayRecorder->dword148) {
                    ReplayRecorder->dword148 = 1;
                    if (!ReplayRecorder->dword140) {
                        ReplayRecorder_Pause(recorder);
                    }
                }
            }
        }
        else {
            entity->velocity.x = 0;
            entity->velocity.y = 0;
        }

        if (FarPlane) {
            int screenX = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
            int screenY = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;

            entity->scale.x   = 0x200;
            entity->scale.y   = 0x200;
            entity->drawOrder = Zone->playerDrawLow;
            foreach_all(FarPlane, farPlane)
            {
                Hitbox hitbox;
                hitbox.left   = -(farPlane->size.x >> 16);
                hitbox.top    = -(farPlane->size.y >> 16);
                hitbox.right  = (farPlane->size.x >> 16);
                hitbox.bottom = (farPlane->size.y >> 16);
                if (MathHelpers_PointInHitbox(FLIP_NONE, farPlane->origin.x, farPlane->origin.y, &hitbox, entity->position.x, entity->position.y)) {
                    entity->position.x =
                        (farPlane->position.x - ((farPlane->position.x - screenX) >> 1) + 0x8000) + ((entity->position.x - farPlane->origin.x) >> 1);
                    entity->position.y =
                        (farPlane->position.y - ((farPlane->position.y - screenY) >> 1) + 0x8000) + ((entity->position.y - farPlane->origin.y) >> 1);
                    entity->drawFX |= FX_SCALE;
                    entity->scale.x   = 0x100;
                    entity->scale.y   = 0x100;
                    entity->drawOrder = Zone->drawOrderLow;
                    foreach_break;
                }
            }
        }

        if (ReplayRecorder->dword148 && !ReplayRecorder->dword140) {
            entity->visible = ~(entity->timer >> 2) & 1;
            entity->timer++;
        }
    }
}

void ReplayRecorder_StatePlay(void)
{
    RSDK_THIS(ReplayRecorder);
    int *buffer      = NULL;
    int *frameBuffer = NULL;
    if (RSDK.GetEntityID(entity) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;

    if (RSDK.GetEntityID(entity) == SLOT_REPLAYRECORDER_W)
        frameBuffer = ReplayRecorder->frameBuffer_w;
    else
        frameBuffer = ReplayRecorder->frameBuffer_r;

    if (ReplayRecorder->frameCounter >= buffer[5]) {
        if (ReplayRecorder->frameCounter != buffer[5]) {
            if (!entity->playing)
                ReplayRecorder_Seek(entity, ReplayRecorder->frameCounter - buffer[5]);
            entity->replayFrame = ReplayRecorder->frameCounter - buffer[5];
        }
        entity->stateLate = ReplayRecorder_StateLate_Replay;
        entity->state     = ReplayRecorder_None_Replay;
    }
    else if (ReplayRecorder->frameCounter < buffer[5]) {
        if (entity->playing) {
            ReplayRecorder_ApplyFrameData(entity, (byte *)frameBuffer);
        }
    }
}

void ReplayRecorder_None_Replay(void) {}

void ReplayRecorder_StateLate_Replay(void)
{
    RSDK_THIS(ReplayRecorder);
    EntityPlayer *player = entity->player;
    if (!entity->replayFrame) {
        EntityCamera *camera = player->camera;
        if (camera) {
            camera->position.x                        = player->position.x;
            camera->position.y                        = player->position.y;
            RSDK_screens[camera->screenID].position.x = (camera->position.x >> 16);
            RSDK_screens[camera->screenID].position.y = (camera->position.y >> 16);
            if (WarpDoor)
                WarpDoor_Unknown5(-1, 0);
            else
                WarpDoor_Unknown4();
            camera->state       = Camera_State_Follow;
            player->scrollDelay = 0;
        }
    }
    if (entity->playing) {
        if (player->state != ReplayRecorder_PlayerState)
            entity->field_60 = player->state;
    }

    int *frameBuffer = NULL;
    if (RSDK.GetEntityID(entity) == SLOT_REPLAYRECORDER_W)
        frameBuffer = ReplayRecorder->frameBuffer_w;
    else
        frameBuffer = ReplayRecorder->frameBuffer_r;
    int *framePtr  = &frameBuffer[7 * entity->replayFrame];
    byte *bufBytes = (byte *)framePtr;
    if (!entity->playing) {
        if (bufBytes[0]) {
            if ((bufBytes[0] == 1 || bufBytes[0] == 3) || (bufBytes[0] == 2 && (bufBytes[1] & 2))) {
                int distX = abs(player->position.x - framePtr[1]);
                int distY = abs(player->position.y - framePtr[2]);
                if (MathHelpers_Unknown6((distX >> 16) * (distX >> 16) + (distY >> 16) * (distY >> 16)) << 16 >= 0x20000) {
                    player->position.x += (framePtr[1] - player->position.x) >> 1;
                    player->position.y += (framePtr[2] - player->position.y) >> 1;
                }
            }
        }
    }
    ++entity->replayFrame;
}

void ReplayRecorder_None_Record(void) {}

void ReplayRecorder_RecordFrameData(void)
{
    byte recording[28];
    int *recordingPtr = (int *)recording;

    RSDK_THIS(ReplayRecorder);
    if (entity->replayFrame < entity->field_84 - 1) {
        EntityPlayer *player = entity->player;
        int inputState       = 0;
        memset(recording, 0, sizeof(recording));

        ControllerState *controller = &RSDK_controller[player->controllerID];
        if (controller->keyUp.down || RSDK_stickL[player->controllerID].keyUp.down)
            inputState = 0x01;
        if (controller->keyDown.down || RSDK_stickL[player->controllerID].keyDown.down)
            inputState |= 0x02;
        if (controller->keyLeft.down || RSDK_stickL[player->controllerID].keyLeft.down)
            inputState |= 0x04;
        if (controller->keyRight.down || RSDK_stickL[player->controllerID].keyRight.down)
            inputState |= 0x08;
        if (controller->keyA.press || controller->keyB.press || controller->keyC.press || controller->keyX.press)
            inputState |= 0x10;
        if (controller->keyA.down || controller->keyB.down || controller->keyC.down || controller->keyX.down)
            inputState |= 0x20;

        bool32 isGimmickState = ReplayRecorder_CheckPlayerGimmickState(entity);
        entity->stateStore    = player->state;

        EntityIce *entPtr  = player->field_218;
        Animator *animator = &player->playerAnimator;
        if (isGimmickState && RSDK.CheckStageFolder("PSZ2") && player->state == Ice_State_FrozenPlayer && entPtr->objectID == Ice->objectID) {
            animator = &entPtr->animator2;
        }
        void *storedState = entity->storedState;
        entity->animID    = animator->animationID;
        entity->frameID   = animator->frameID;

        for (int i = 0; i < 64; ++i) {
            if (storedState == ReplayRecorder->actions[i])
                break;
        }

        for (int i = 0; i < 64; ++i) {
            if (entity->stateStore == ReplayRecorder->actions[i])
                break;
        }

        if (entity->replayFrame == 0 || entity->field_78 > 0 || entity->storedState != entity->stateStore) {
            entity->field_78 = 0;
            recording[2]     = inputState;
            recording[1]     = 0;
            recording[0]     = 2 * (entity->field_78 == 2) + 1;
            recording[3]     = player->direction;
            recordingPtr[5]  = player->rotation;
            recordingPtr[1]  = player->position.x;
            recordingPtr[2]  = player->position.y;
            recordingPtr[3]  = player->velocity.x;
            recordingPtr[4]  = player->velocity.y;
            recording[24]    = entity->animID;
            recording[25]    = entity->frameID;
            if (isGimmickState)
                recording[1] = 8;
        }
        else {
            int changedVals = 0;
            if (inputState != entity->storedInputs) {
                recording[2] = inputState;
                changedVals  = 1;
            }
            if (player->direction != entity->storedDirection) {
                recording[3] = player->direction;
                changedVals |= 0x10;
            }
            if (player->position.x != entity->storedPos.x || player->position.y != entity->storedPos.y) {
                changedVals |= 2;
                recordingPtr[1] = player->position.x;
                recordingPtr[2] = player->position.y;
            }
            if (player->velocity.x != entity->storedVel.x || player->velocity.y != entity->storedVel.y) {
                changedVals |= 4;
                recordingPtr[2] = player->velocity.x;
                recordingPtr[3] = player->velocity.y;
            }
            if (player->groundVel != entity->storedRotation) {
                changedVals |= 0x20;
                recordingPtr[5] = player->rotation;
            }
            if (animator->animationID == entity->storedAnim) {
                if (animator->frameID == entity->storedFrame) {
                    if (isGimmickState)
                        changedVals |= 8;
                    if (changedVals) {
                        recording[0] = 2;
                        recording[1] = changedVals;
                    }
                    else {
                        recording[0] = 0;
                    }
                    ReplayRecorder_PackFrame(recording);
                    entity->storedPos.x     = player->position.x;
                    entity->storedPos.y     = player->position.y;
                    entity->storedVel.x     = player->velocity.x;
                    entity->storedVel.y     = player->velocity.y;
                    entity->storedDirection = player->direction;
                    entity->storedSpeed     = player->groundVel;
                    entity->storedRotation  = player->rotation;
                    entity->storedInputs    = inputState;
                    entity->storedAnim      = animator->animationID;
                    entity->storedFrame     = animator->frameID;
                    return;
                }
                changedVals |= 0x80;
            }
            else {
                changedVals |= 0xC0;
                recording[24] = entity->animID;
            }
            recording[25] = entity->frameID;

            if (isGimmickState)
                changedVals |= 8;
            if (changedVals) {
                recording[0] = 2;
                recording[1] = changedVals;
            }
            else {
                recording[0] = 0;
            }
        }
        ReplayRecorder_PackFrame(recording);
        entity->storedPos.x     = player->position.x;
        entity->storedPos.y     = player->position.y;
        entity->storedVel.x     = player->velocity.x;
        entity->storedVel.y     = player->velocity.y;
        entity->storedDirection = player->direction;
        entity->storedSpeed     = player->groundVel;
        entity->storedRotation  = player->rotation;
        entity->storedInputs    = inputState;
        entity->storedAnim      = animator->animationID;
        entity->storedFrame     = animator->frameID;
    }
    else {
        entity->state     = StateMachine_None;
        entity->stateLate = StateMachine_None;
    }
}

void ReplayRecorder_LoadReplayDB(void (*callback)(bool32))
{
    if ((globals->replayTableID != -1 && globals->replayTableLoaded == STATUS_OK) || globals->replayTableLoaded == STATUS_CONTINUE)
        return;
    LogHelpers_Print("Loading Replay DB");
    globals->replayTableLoaded = STATUS_CONTINUE;
    ReplayDB->loadEntity       = RSDK_sceneInfo->entity;
    ReplayDB->loadCallback     = NULL;
    globals->replayTableID     = API.LoadUserDB("ReplayDB.bin", ReplayRecorder_SetStatus); //, callback
    if (globals->replayTableID == -1) {
        LogHelpers_Print("Couldn't claim a slot for loading %s", "ReplayDB.bin");
        globals->replayTableLoaded = STATUS_ERROR;
    }
}

void ReplayRecorder_SaveReplayDB(void (*callback)(bool32))
{
    if (API.GetUserStorageNoSave() || globals->replayTableID == 0xFFFF || globals->replayTableLoaded != STATUS_OK) {
        if (callback)
            callback(false);
    }
    else {
        LogHelpers_Print("Saving Replay DB");
        ReplayDB->saveEntity   = RSDK_sceneInfo->entity;
        ReplayDB->saveCallback = callback;
        API.SaveUserDB(globals->replayTableID, ReplayRecorder_ReplaySaveFinish);
    }
}

void ReplayRecorder_CreateReplayDB(void)
{
    globals->replayTableID = API.InitUserDB("ReplayDB.bin", 4, "score", 2, "zoneID", 2, "act", 2, "characterID", 2, "encore", 4, "zoneSortVal", 0);

    if (globals->replayTableID == -1)
        globals->replayTableLoaded = STATUS_ERROR;
    else
        globals->replayTableLoaded = STATUS_OK;
}

uint ReplayRecorder_AddReplayID(byte actID, char zone, int charID, int score, char mode)
{
    if (globals->replayTableLoaded == STATUS_OK) {
        uint rowID       = API.AddUserDBRow(globals->replayTableID);
        int zoneStortVal = score & 0x3FFFFFF | ((actID & 1 | 2 * (mode & 1 | 2 * zone)) << 26);
        API.SetUserDBValue(globals->replayTableID, rowID, 4, "score", &score);
        API.SetUserDBValue(globals->replayTableID, rowID, 2, "zoneID", &zone);
        API.SetUserDBValue(globals->replayTableID, rowID, 2, "act", &actID);
        API.SetUserDBValue(globals->replayTableID, rowID, 2, "characterID", &charID);
        API.SetUserDBValue(globals->replayTableID, rowID, 2, "encore", &actID);
        API.SetUserDBValue(globals->replayTableID, rowID, 4, "zoneSortVal", &zoneStortVal);
        uint UUID = API.GetUserDBRowUUID(globals->replayTableID, rowID);
        char createTime[24];
        sprintf(createTime, "");
        API.GetUserDBCreationTime(globals->replayTableID, rowID, createTime, 23, "%Y/%m/%d %H:%M:%S");
        LogHelpers_Print("Replay DB Added Entry");
        LogHelpers_Print("Created at %s", createTime);
        LogHelpers_Print("Row ID: %d", rowID);
        LogHelpers_Print("UUID: %08X", UUID);
        return rowID;
    }
    return -1;
}

void ReplayRecorder_DeleteTimeAttackRow(int a1, void (*callback)(bool32), int a3)
{
    int id                   = API.GetUserDBRowUUID(globals->replayTableID, a1);
    int value                = 0;
    ReplayDB->deleteEntity   = RSDK_sceneInfo->entity;
    ReplayDB->deleteCallback = callback;
    API.RemoveDBRow(globals->replayTableID, a1);
    TimeAttackData->status = 0;
    API.SetupSortedUserDBRowIDs(globals->taTableID);
    API.Unknown33(globals->taTableID, 4, "replayID", &id);
    int count = API.GetSortedUserDBRowCount(globals->taTableID);
    for (int i = 0; i < count; ++i) {
        uint uuid = API.GetSortedUserDBRowID(globals->taTableID, i);
        LogHelpers_Print("Deleting Time Attack replay from row #%d", uuid);
        API.SetUserDBValue(globals->taTableID, uuid, 4, "replayID", &value);
    }

    char filename[0x20];
    sprintf(filename, "Replay_%08X.bin", id);
    if (!a3)
        API.DeleteUserFile(filename, ReplayRecorder_DeleteReplayCB);
    else
        API.DeleteUserFile(filename, ReplayRecorder_DeleteReplaySave2CB);
}

void ReplayRecorder_DeleteReplayCB(int status)
{
    LogHelpers_Print("DeleteReplay_CB(%d)", status);
    API.SaveUserDB(globals->replayTableID, ReplayRecorder_DeleteReplaySaveCB);
}

void ReplayRecorder_DeleteReplaySaveCB(int status)
{
    LogHelpers_Print("DeleteReplaySave_CB(%d)", status);
    API.SaveUserDB(globals->taTableID, ReplayRecorder_DeleteReplaySave2CB);
}

void ReplayRecorder_DeleteReplaySave2CB(int status)
{
    LogHelpers_Print("DeleteReplaySave2_CB(%d)", status);
    if (ReplayDB->deleteCallback) {
        Entity *store = RSDK_sceneInfo->entity;
        if (ReplayDB->deleteEntity)
            RSDK_sceneInfo->entity = ReplayDB->deleteEntity;
        ReplayDB->deleteCallback(status == STATUS_OK);
        RSDK_sceneInfo->entity   = store;
        ReplayDB->deleteCallback = NULL;
        ReplayDB->deleteEntity   = NULL;
    }
}

int ReplayRecorder_SetStatus(int status)
{
    if (status == STATUS_OK) {
        globals->replayTableLoaded = STATUS_OK;
        API.SetupSortedUserDBRowIDs(globals->replayTableID);
        LogHelpers_Print("Load Succeeded! Replay count: %d", API.GetSortedUserDBRowCount(globals->replayTableID));
    }
    else {
        LogHelpers_Print("Load Failed! Creating new Replay DB");
        ReplayRecorder_CreateReplayDB();
    }
    LogHelpers_Print("Replay DB Slot => %d, Load Status => %d", globals->replayTableID, globals->replayTableLoaded);

    if (ReplayDB->loadCallback) {
        Entity *store = RSDK_sceneInfo->entity;
        if (ReplayDB->loadEntity)
            RSDK_sceneInfo->entity = ReplayDB->loadEntity;
        ReplayDB->loadCallback(status == STATUS_OK);
        RSDK_sceneInfo->entity = store;
        ReplayDB->loadCallback = NULL;
        ReplayDB->loadEntity   = NULL;
    }
    return 1;
}

int ReplayRecorder_ReplaySaveFinish(int status)
{
    if (ReplayDB->saveCallback) {
        Entity *store = RSDK_sceneInfo->entity;
        if (ReplayDB->saveEntity)
            RSDK_sceneInfo->entity = ReplayDB->saveEntity;
        ReplayDB->saveCallback((status == STATUS_OK));
        RSDK_sceneInfo->entity = store;
        ReplayDB->saveCallback = NULL;
        ReplayDB->saveEntity   = NULL;
    }
    return 1;
}

void ReplayRecorder_EditorDraw(void) {}

void ReplayRecorder_EditorLoad(void) {}

void ReplayRecorder_Serialize(void) {}
#endif
