#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectReplayRecorder *ReplayRecorder;

void ReplayRecorder_Update(void)
{
    RSDK_THIS(ReplayRecorder);
    if (!self->paused) {
        StateMachine_Run(self->state);
    }
}

void ReplayRecorder_LateUpdate(void)
{
    RSDK_THIS(ReplayRecorder);
    if (!self->paused) {
        StateMachine_Run(self->stateLate);

        int32 *buffer = NULL;
        if (RSDK.GetEntityID(self) == SLOT_REPLAYRECORDER_W)
            buffer = ReplayRecorder->writeBuffer;
        else
            buffer = ReplayRecorder->readBuffer;
        if (self->replayFrame > buffer[REPLAY_HDR_FRAMECOUNT2] && self->state) {
            ReplayRecorder_Stop(self);
        }

        if (ReplayRecorder->reachedGoal && !self->player->playerID && self->state == ReplayRecorder_None_Record) {
            if (self->replayStopDelay <= 0) {
                if (!self->replayStopDelay) {
                    ReplayRecorder_Stop(self);
                    ReplayRecorder_Rewind(self);
                    self->replayStopDelay = -1;
                }
            }
            else {
                self->replayStopDelay--;
            }
        }
    }
}

void ReplayRecorder_StaticUpdate(void)
{
    if (globals->gameMode == MODE_TIMEATTACK) {
        if (SceneInfo->state & ENGINESTATE_REGULAR)
            ++ReplayRecorder->frameCounter;

        if (Zone) {
            if (!ReplayRecorder->initialized) {
                TitleCard->finishedCB       = ReplayRecorder_TitleCardCB;
                TimeAttackGate->startCB     = ReplayRecorder_StartCB;
                TimeAttackGate->endCB       = ReplayRecorder_FinishCB;
                ActClear->bufferMove_CB     = ReplayRecorder_Buffer_Move;
                ActClear->saveReplay_CB     = ReplayRecorder_SaveReplay;
                ReplayRecorder->initialized = true;
            }

            if (ReplayRecorder->startRecording) {
                EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                ReplayRecorder_StartRecording(player);
                ReplayRecorder->startRecording   = false;
                ReplayRecorder->startedRecording = true;
            }

            if (ReplayRecorder->startPlayback) {
                EntityPlayer *player = NULL;
                if (Player->playerCount <= 1) {
                    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

                    player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                    API.SetAchievementStatus(false);
                    API.SetStatsStatus(false);
                    TimeAttackGate->disableRecords = true;
                    TimeAttackData->personalRank   = param->replayRankID;
                }
                else {
                    player = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                }

                EntityReplayRecorder *recorder2 = (EntityReplayRecorder *)ReplayRecorder->recorder_w;
                ReplayRecorder_Rewind(recorder2);
                ReplayRecorder_Play(player);
                ReplayRecorder->startPlayback = false;
                ReplayRecorder->isReplaying   = true;
            }

            EntityReplayRecorder *recorder1 = (EntityReplayRecorder *)ReplayRecorder->recorder_r;
            EntityPlayer *playerR1          = recorder1->player;
            if (playerR1) {
                recorder1->storedState = playerR1->state;
                recorder1->storedAnim  = playerR1->animator.animationID;
                recorder1->storedFrame = playerR1->animator.frameID;
            }

            EntityReplayRecorder *recorder2 = (EntityReplayRecorder *)ReplayRecorder->recorder_w;
            EntityPlayer *playerR2          = recorder2->player;
            if (recorder2->playing && playerR2)
                playerR2->state = ReplayRecorder_PlayerState;

            if ((ControllerInfo->keyStart.press || Unknown_pausePress) && SceneInfo->state == ENGINESTATE_REGULAR) {
                EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                bool32 flag                = true;
                if (ActClear && ActClear->actClearActive)
                    flag = false;
                if (!RSDK.GetEntityCount(TitleCard->objectID, false) && !pauseMenu->objectID && flag) {
                    RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                    pauseMenu->triggerPlayer = RSDK.GetEntityID(SceneInfo->entity);
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
    if (self->playing)
        ReplayRecorder_DrawGhostDisplay();
}

void ReplayRecorder_Create(void *data)
{
    RSDK_THIS(ReplayRecorder);
    self->active          = ACTIVE_NEVER;
    self->drawOrder       = 13;
    self->inkEffect       = INK_NONE;
    self->replayStopDelay = -1;
    self->drawFX          = FX_FLIP;
    self->alphaStore      = 256;
    self->visible         = globals->gameMode == MODE_TIMEATTACK;
}

void ReplayRecorder_StageLoad(void)
{
    ReplayRecorder->replayID    = 0;
    ReplayRecorder->replayRowID = -1;
    if (globals->gameMode == MODE_TIMEATTACK) {
        ReplayRecorder->active            = ACTIVE_ALWAYS;
        ReplayRecorder->frameCounter      = -1;
        ReplayRecorder->savedReplay       = 0;
        ReplayRecorder->initialized       = false;
        ReplayRecorder->startedRecording  = false;
        ReplayRecorder->isReplaying       = false;
        ReplayRecorder->hasSetupGhostView = false;
        ReplayRecorder->hasSetupGhostVS   = false;
        ReplayRecorder->passedStartLine   = false;
        ReplayRecorder->reachedGoal       = false;
        ReplayRecorder->packedStartFrame  = false;
        ReplayRecorder->loadCallback      = NULL;
        ReplayRecorder->buffer            = NULL;
        memset(ReplayRecorder->filename, 0, sizeof(ReplayRecorder->filename));
        ReplayRecorder->writeBuffer   = globals->replayWriteBuffer;
        ReplayRecorder->frameBuffer_w = (ReplayFrame *)&globals->replayWriteBuffer[REPLAY_HDR_SIZE];
        ReplayRecorder->readBuffer    = globals->replayReadBuffer;
        ReplayRecorder->frameBuffer_r = (ReplayFrame *)&globals->replayReadBuffer[REPLAY_HDR_SIZE];
        if (Zone) {
            if (!SceneInfo->inEditor)
                ReplayRecorder_SetupActions();
            RSDK.SetRandSeed(1624633040);

            RSDK.ResetEntitySlot(SLOT_REPLAYRECORDER_W, ReplayRecorder->objectID, NULL);
            EntityReplayRecorder *recorder_r = RSDK_GET_ENTITY(SLOT_REPLAYRECORDER_W, ReplayRecorder);
            recorder_r->maxFrameCount        = 37447;
            ReplayRecorder->recorder_r       = (Entity *)recorder_r;

            RSDK.ResetEntitySlot(SLOT_REPLAYRECORDER_R, ReplayRecorder->objectID, NULL);
            EntityReplayRecorder *recorder_w = RSDK_GET_ENTITY(SLOT_REPLAYRECORDER_R, ReplayRecorder);
            recorder_w->maxFrameCount        = 37447;
            ReplayRecorder->recorder_w       = (Entity *)recorder_w;

            ReplayRecorder->startRecording = false;
            ReplayRecorder->startPlayback  = false;
            int32 *buffer                  = NULL;
            if (RSDK.GetEntityID(ReplayRecorder->recorder_w) == SLOT_REPLAYRECORDER_W)
                buffer = ReplayRecorder->writeBuffer;
            else
                buffer = ReplayRecorder->readBuffer;

            EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
            if (param->viewReplay && buffer[REPLAY_HDR_PACKED]) {
                if (param->showGhost) {
                    globals->playerID        = (globals->playerID & 0xFF) | ((globals->playerID & 0xFF) << 8);
                    Player->configureGhostCB = ReplayRecorder_ConfigureGhost_CB;
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
    int32 *buffer = NULL;
    if (RSDK.GetEntityID(ReplayRecorder->recorder_w) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;

    if (Player->playerCount != 1 || !buffer[REPLAY_HDR_PACKED])
        ReplayRecorder->startRecording = true;
    if (buffer[REPLAY_HDR_PACKED])
        ReplayRecorder->startPlayback = true;
}

void ReplayRecorder_Resume(EntityReplayRecorder *recorder)
{
    LogHelpers_Print("ReplayRecorder_Resume()");
    recorder->paused          = false;
    recorder->player->visible = true;
}

void ReplayRecorder_StartCB(void)
{
    if (ReplayRecorder->startedRecording) {
        ((EntityReplayRecorder *)ReplayRecorder->recorder_r)->changeFlags = 2;
    }
    EntityReplayRecorder *recorder = (EntityReplayRecorder *)ReplayRecorder->recorder_w;
    if (ReplayRecorder->isReplaying && recorder->playing) {
        if (ReplayRecorder->packedStartFrame)
            ReplayRecorder_Resume(recorder);
        else
            ReplayRecorder_SeekFunc(recorder);
    }
    ReplayRecorder->passedStartLine = true;
}

void ReplayRecorder_FinishCB(void)
{
    EntityReplayRecorder *recorder = (EntityReplayRecorder *)ReplayRecorder->recorder_r;
    if (ReplayRecorder->startedRecording) {
        recorder->replayStopDelay = 120;
    }
    ReplayRecorder->reachedGoal = true;
}

void ReplayRecorder_Buffer_Move(void)
{
    int32 *buffer = NULL;
    if (RSDK.GetEntityID(ReplayRecorder->recorder_r) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;
    if (buffer[REPLAY_HDR_PACKED]) {
        if (buffer[REPLAY_HDR_FRAMECOUNT2] < ((EntityReplayRecorder *)ReplayRecorder->recorder_r)->maxFrameCount - 1) {
            memset(globals->replayTempWBuffer, 0, Replay_BufferSize);
            LogHelpers_Print("Buffer_Move(0x%08x, 0x%08x)", globals->replayTempWBuffer, buffer);
            memcpy(globals->replayTempWBuffer, buffer, Replay_BufferSize);
            memset(buffer, 0, Replay_BufferSize);
            ReplayRecorder_Buffer_PackInPlace(globals->replayTempWBuffer);
            HUD->replaySaveEnabled = true;
        }
    }
}

void ReplayRecorder_SaveReplayDLG_NoCB(void)
{
    HUD->replaySaveEnabled        = true;
    ActClear->hasSavedReplay      = false;
    ActClear->disableResultsInput = false;
}

void ReplayRecorder_SaveReplayDLG_YesCB(void)
{
    ReplayRecorder->replayID    = 0;
    ReplayRecorder->replayRowID = -1;
    int32 mins                  = SceneInfo->minutes;
    int32 secs                  = SceneInfo->seconds;
    int32 millisecds            = SceneInfo->milliseconds;
    LogHelpers_Print("Bout to create ReplayDB entry...");
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    int32 rowID            = ReplayRecorder_AddReplayID(param->actID, param->zoneID, param->characterID, (millisecds + 100 * (secs + 60 * mins)),
                                             SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE));
    if (rowID == -1) {
        LogHelpers_Print("Table row ID invalid! %d", -1);
        ReplayRecorder_SavedReplay(false);
    }
    else {
        ReplayRecorder->replayID    = API.GetUserDBRowUUID(globals->replayTableID, rowID);
        ReplayRecorder->replayRowID = rowID;
        char fileName[0x20];
        sprintf(fileName, "Replay_%08X.bin", ReplayRecorder->replayID);
        LogHelpers_Print("Replay Filename: %s", fileName);
        UIWaitSpinner_StartWait();
        ReplayRecorder->savedReplay = true;
        ReplayRecorder_Buffer_SaveFile(fileName, globals->replayTempWBuffer);
        HUD->replaySaveEnabled = false;
    }
}

void ReplayRecorder_Unknown6(void)
{
    TextInfo message;
    INIT_TEXTINFO(message);
    Localization_GetString(&message, STR_SAVEREPLAY);
    UIDialog_CreateDialogYesNo(&message, ReplayRecorder_SaveReplayDLG_YesCB, ReplayRecorder_SaveReplayDLG_NoCB, true, true);
}

void ReplayRecorder_SaveReplay(void)
{
    if (globals->replayTempWBuffer[REPLAY_HDR_PACKED]) {
        LogHelpers_Print("Saving replay...");
        foreach_all(HUD, hud) { foreach_break; }
        RSDK.SetSpriteAnimation(HUD->aniFrames, 11, &hud->taAnimator2, true, 0);
        RSDK.PlaySfx(HUD->sfxClick, 0, 255);
        EntityDialogRunner *runner = CREATE_ENTITY(DialogRunner, DialogRunner_HandleCallback, 0, 0);
        runner->callback           = ReplayRecorder_Unknown6;
        runner->timer              = 45;
        runner->isPermanent        = true;
    }
    else {
        LogHelpers_Print("Can't save replay! No data available");
        ActClear->disableResultsInput = false;
    }
}

void ReplayRecorder_SavedReplay(bool32 status)
{
    if (status) {
        LogHelpers_Print("Replay save successful!");
        ReplayRecorder_SaveReplayDB(ReplayRecorder_ReplaySave_CB);
    }
    else {
        if (ReplayRecorder->replayRowID != -1)
            API.RemoveDBRow(globals->replayTableID, ReplayRecorder->replayRowID);
        TextInfo message;
        INIT_TEXTINFO(message);
        Localization_GetString(&message, STR_NOREPLAYSPACE);
        UIDialog_CreateDialogOk(&message, NULL, true);
        UIWaitSpinner_FinishWait();
        ActClear->disableResultsInput = false;
        ActClear->hasSavedReplay      = false;
        HUD->replaySaveEnabled        = true;
    }
}

void ReplayRecorder_ReplaySave_CB(bool32 flag)
{
    if (flag) {
        if (TimeAttackData->rowID == -1) {
            UIWaitSpinner_FinishWait();
            ActClear->disableResultsInput = false;
            foreach_all(HUD, hud)
            {
                RSDK.SetSpriteAnimation(HUD->aniFrames, 10, &hud->taAnimator2, true, 0);
                foreach_break;
            }
            RSDK.PlaySfx(HUD->sfxStarpost, true, 255);
            HUD->replaySaveEnabled = false;
        }
        else {
            API.SetUserDBValue(globals->taTableID, TimeAttackData->rowID, DBVAR_UINT32, "replayID", &ReplayRecorder->replayID);
            TimeAttackData_SaveTimeAttackDB(ReplayRecorder_SaveTimeAttackDB_CB);
        }
    }
    else {
        TextInfo message;
        INIT_TEXTINFO(message);
        char fileName[0x20];
        sprintf(fileName, "Replay_%08X.bin", ReplayRecorder->replayID);
        if (ReplayRecorder->replayRowID != -1)
            API.RemoveDBRow(globals->replayTableID, ReplayRecorder->replayRowID);
        API.DeleteUserFile(fileName, NULL);
        Localization_GetString(&message, STR_NOREPLAYSPACE);
        UIDialog_CreateDialogOk(&message, NULL, true);
        UIWaitSpinner_FinishWait();
        ActClear->disableResultsInput = false;
        ActClear->hasSavedReplay      = false;
        HUD->replaySaveEnabled        = true;
    }
}

void ReplayRecorder_SaveTimeAttackDB_CB(int32 status)
{
    UIWaitSpinner_FinishWait();
    ActClear->disableResultsInput = false;
    if (!status) {
        ActClear->hasSavedReplay = false;
        HUD->replaySaveEnabled   = true;
    }
    else {
        foreach_all(HUD, hud)
        {
            RSDK.SetSpriteAnimation(HUD->aniFrames, 10, &hud->taAnimator2, true, 0);
            foreach_break;
        }
        RSDK.PlaySfx(HUD->sfxStarpost, false, 255);
        HUD->replaySaveEnabled = false;
    }
}

void ReplayRecorder_Buffer_PackInPlace(int32 *tempWriteBuffer)
{
    LogHelpers_Print("Buffer_PackInPlace(%08x)", tempWriteBuffer);

    if (*tempWriteBuffer == Replay_Signature) {
        if (tempWriteBuffer[REPLAY_HDR_PACKED2]) {
            LogHelpers_Print("Buffer_Ppack ERROR: Buffer is already packed");
        }
        else {
            int32 compSize         = REPLAY_HDR_SIZE * sizeof(int);
            int32 uncompressedSize = sizeof(ReplayFrame) * (tempWriteBuffer[REPLAY_HDR_FRAMECOUNT2] + 2);

            ReplayFrame *framePtr = (ReplayFrame *)&tempWriteBuffer[REPLAY_HDR_SIZE];
            uint8 *compressedData = (uint8 *)&tempWriteBuffer[REPLAY_HDR_SIZE];
            for (int32 f = 0; f < tempWriteBuffer[REPLAY_HDR_FRAMECOUNT2]; ++f) {
                ReplayFrame uncompressedData;
                memcpy(&uncompressedData, framePtr, sizeof(ReplayFrame));

                memset(framePtr, 0, sizeof(ReplayFrame));

                int32 size = ReplayDB_Buffer_PackEntry(compressedData, &uncompressedData);
                compressedData += size;
                compSize += size;
                framePtr++;
            }
            LogHelpers_Print("Packed %d frames: %luB -> %luB", tempWriteBuffer[REPLAY_HDR_FRAMECOUNT2], uncompressedSize, compSize);
            tempWriteBuffer[REPLAY_HDR_COMPSIZE] = compSize;
            tempWriteBuffer[REPLAY_HDR_PACKED2]  = true;
        }
    }
    else {
        LogHelpers_Print("Buffer_Pack ERROR: Signature does not match");
    }
}

void ReplayRecorder_Buffer_Unpack(int32 *readBuffer, int32 *tempReadBuffer)
{
    LogHelpers_Print("Buffer_Unpack(0x%08x, 0x%08x)", readBuffer, tempReadBuffer);

    uint8 *compressedData = (uint8 *)&tempReadBuffer[REPLAY_HDR_SIZE];
    if (*tempReadBuffer == Replay_Signature) {
        if (tempReadBuffer[REPLAY_HDR_PACKED2]) {
            int32 compSize                     = tempReadBuffer[REPLAY_HDR_COMPSIZE];
            readBuffer[REPLAY_HDR_SIG]         = tempReadBuffer[REPLAY_HDR_SIG];
            readBuffer[REPLAY_HDR_VER]         = tempReadBuffer[REPLAY_HDR_VER];
            readBuffer[REPLAY_HDR_PACKED2]     = tempReadBuffer[REPLAY_HDR_PACKED2];
            readBuffer[REPLAY_HDR_PACKED]      = tempReadBuffer[REPLAY_HDR_PACKED];
            readBuffer[REPLAY_HDR_FRAMECOUNT2] = tempReadBuffer[REPLAY_HDR_FRAMECOUNT2];
            readBuffer[REPLAY_HDR_FRAMECOUNT]  = tempReadBuffer[REPLAY_HDR_FRAMECOUNT];
            int32 frameSize                    = sizeof(ReplayFrame) * (tempReadBuffer[REPLAY_HDR_FRAMECOUNT2] + 2);
            ReplayFrame *uncompressedBuffer    = (ReplayFrame *)&readBuffer[REPLAY_HDR_SIZE];

            for (int32 i = 0; i < tempReadBuffer[REPLAY_HDR_FRAMECOUNT2]; ++i) {
                int32 size = ReplayDB_Buffer_UnpackEntry(uncompressedBuffer, compressedData);
                compressedData += size;
                uncompressedBuffer++;
            }
            LogHelpers_Print("Unpacked %d frames: %luB -> %luB", tempReadBuffer[REPLAY_HDR_FRAMECOUNT2], compSize, frameSize);
            readBuffer[REPLAY_HDR_PACKED2]  = false;
            readBuffer[REPLAY_HDR_COMPSIZE] = frameSize;
            memset(tempReadBuffer, 0, Replay_BufferSize);
        }
        else {
            LogHelpers_Print("Buffer_Unpack ERROR: Buffer is not packed");
        }
    }
    else {
        LogHelpers_Print("Buffer_Unpack ERROR: Signature does not match");
    }
}

void ReplayRecorder_Buffer_SaveFile(const char *fileName, int32 *buffer)
{
    LogHelpers_Print("Buffer_SaveFile(%s, %08x)", fileName, buffer);
    if (buffer[REPLAY_HDR_PACKED]) {
        ReplayRecorder->saveFinishPtr = ReplayRecorder_SavedReplay;
        API_SaveUserFile(fileName, buffer, buffer[REPLAY_HDR_COMPSIZE], ReplayRecorder_SetReplayStatus, true);
    }
    else {
        LogHelpers_Print("Attempted to save an empty replay buffer");
        ReplayRecorder_SavedReplay(false);
    }
}

void ReplayRecorder_SetReplayStatus(int32 status)
{
    if (ReplayRecorder->saveFinishPtr)
        ReplayRecorder->saveFinishPtr(status == STATUS_OK);
    ReplayRecorder->saveFinishPtr = NULL;
}

void ReplayRecorder_Buffer_LoadFile(const char *fileName, void *buffer, void (*callback)(bool32))
{
    LogHelpers_Print("Buffer_LoadFile(%s, %08x)", fileName, buffer);
    memset(buffer, 0, Replay_BufferSize);
    ReplayRecorder->buffer       = buffer;
    ReplayRecorder->loadCallback = callback;
    strcpy(ReplayRecorder->filename, fileName);
    API_LoadUserFile(fileName, buffer, Replay_BufferSize, ReplayRecorder_Load_CB);
}

void ReplayRecorder_Load_CB(int32 status)
{
    if (ReplayRecorder->loadCallback)
        ReplayRecorder->loadCallback(status == STATUS_OK);
    ReplayRecorder->loadCallback = NULL;
    ReplayRecorder->buffer       = NULL;
    memset(ReplayRecorder->filename, 0, sizeof(ReplayRecorder->filename));
}

void ReplayRecorder_ConfigureGhost_CB(void)
{
    RSDK_THIS(Player);
    LogHelpers_Print("ConfigureGhost_CB()");
    LogHelpers_Print("Ghost Slot %d", self->playerID);
    RSDK.PrintVector2(0, "Ghost pos ", self->position.x, self->position.y);
    self->isGhost        = true;
    self->stateInput     = StateMachine_None;
    self->state          = ReplayRecorder_PlayerState;
    self->sidekick       = false;
    self->interaction    = false;
    self->tileCollisions = false;
    self->visible        = true;
    self->alpha          = 255;
}

void ReplayRecorder_SetupActions(void)
{
    for (int32 i = 0; i < 64; ++i) {
        ReplayRecorder->actions[i] = StateMachine_None;
    }
    ReplayRecorder->actions[3]  = Current_Player_State_CurrentDown;
    ReplayRecorder->actions[4]  = Current_Player_State_CurrentLeft;
    ReplayRecorder->actions[5]  = Current_Player_State_CurrentRight;
    ReplayRecorder->actions[6]  = Current_Player_State_CurrentUp;
    ReplayRecorder->actions[7]  = Cylinder_Player_State_InkRoller_Stand;
    ReplayRecorder->actions[8]  = Cylinder_Player_State_InkRoller_Roll;
    ReplayRecorder->actions[9]  = Cylinder_Player_State_Pillar;
    ReplayRecorder->actions[10] = Cylinder_Player_State_Spiral;
    ReplayRecorder->actions[13] = GymBar_PlayerState_Hang;
    ReplayRecorder->actions[14] = GymBar_PlayerState_SwingV;
    ReplayRecorder->actions[15] = GymBar_PlayerState_SwingH;
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
    ReplayRecorder->actions[47] = Player_ForceSuperTransform;
    ReplayRecorder->actions[48] = Player_State_None;
    ReplayRecorder->actions[49] = Player_State_Transform;
    ReplayRecorder->actions[50] = Player_State_TransportTube;
    ReplayRecorder->actions[51] = Player_State_ForceRoll_Air;
    ReplayRecorder->actions[52] = Player_State_ForceRoll_Ground;
    ReplayRecorder->actions[53] = Player_State_Victory;
    ReplayRecorder->actions[54] = SizeLaser_P2JumpInShrink;
    ReplayRecorder->actions[55] = SizeLaser_PlayerState_Grow;
    ReplayRecorder->actions[56] = SizeLaser_P2JumpInGrow;
}

void ReplayRecorder_SetupWriteBuffer(void)
{
    EntityMenuParam *param     = (EntityMenuParam *)globals->menuParam;
    int32 *buffer              = ReplayRecorder->writeBuffer;
    buffer[REPLAY_HDR_SIG]     = Replay_Signature; // signature
    buffer[REPLAY_HDR_VER]     = RETRO_GAMEVER;    // game version
    buffer[REPLAY_HDR_PACKED2] = 0;                // ???
    buffer[REPLAY_HDR_PACKED]  = true;             // isPacked
    // buffer[REPLAY_HDR_FRAMECOUNT2] = ???
    buffer[REPLAY_HDR_FRAMECOUNT]   = ReplayRecorder->frameCounter;                       // frameCount
    buffer[REPLAY_HDR_ZONEID]       = param->zoneID;                                      // zoneID
    buffer[REPLAY_HDR_ACTID]        = param->actID;                                       // actID
    buffer[REPLAY_HDR_CHARID]       = param->characterID;                                 // characterID
    buffer[REPLAY_HDR_ISPLUSLAYOUT] = SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE); // isEncore
    buffer[REPLAY_HDR_OSC]          = Zone->timer;                                        // oscillation
    buffer[REPLAY_HDR_COMPSIZE]     = REPLAY_HDR_SIZE * sizeof(int32);                    // header size
    LogHelpers_Print("characterID = %d", buffer[REPLAY_HDR_CHARID]);
    LogHelpers_Print("zoneID = %d", buffer[REPLAY_HDR_ZONEID]);
    LogHelpers_Print("act = %d", buffer[REPLAY_HDR_ACTID]);
    LogHelpers_Print("isPlusLayout = %d", buffer[REPLAY_HDR_ISPLUSLAYOUT]);
    LogHelpers_Print("oscillation = %d", buffer[REPLAY_HDR_OSC]);
}

void ReplayRecorder_DrawGhostDisplay(void)
{
    RSDK_THIS(ReplayRecorder);
    EntityPlayer *player = self->player;
    if (!self->state) {
        if (self->alphaStore > 0)
            self->alphaStore -= 4;
    }
    if (!SceneInfo->currentScreenID && self->alphaStore) {
        self->inkEffect = INK_NONE;
        int32 screenX   = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
        int32 screenY   = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;

        Hitbox hitbox;
        hitbox.left   = -(ScreenInfo->width >> 1);
        hitbox.right  = ScreenInfo->width >> 1;
        hitbox.top    = -(ScreenInfo->height >> 1);
        hitbox.bottom = ScreenInfo->height >> 1;
        if (!MathHelpers_PointInHitbox(FLIP_NONE, screenX, screenY, &hitbox, player->position.x, player->position.y)) {
            Vector2 drawPos;
            drawPos.x = 0;
            drawPos.y = 0;

            int32 distX = abs(player->position.x - screenX);
            int32 distY = abs(player->position.y - screenY);
            int32 rad   = MathHelpers_SquareRoot((distX >> 16) * (distX >> 16) + (distY >> 16) * (distY >> 16));
            rad         = clampVal(rad, 100, 2000);

            int32 val     = 12 - 4 * (3 * rad - 300) / 2000;
            hitbox.right  = hitbox.right - 8 - (val + 24);
            hitbox.bottom = hitbox.bottom - 8 - (val + 24);
            hitbox.top += val + 24 + 8;
            hitbox.left += val + 24 + 8;

            Vector2 screenPos;
            screenPos.x = screenX;
            screenPos.y = screenY;
            if (MathHelpers_ConstrainToBox(&drawPos, player->position.x, player->position.y, screenPos, hitbox)) {
                int32 angle = RSDK.ATan2(player->position.x - drawPos.x, player->position.y - drawPos.y);
                int32 x     = ((val + 18) * RSDK.Cos256(angle) << 8) + drawPos.x;
                int32 y     = ((val + 18) * RSDK.Sin256(angle) << 8) + drawPos.y;
                DrawHelpers_DrawIsocelesTriangle(x, y, (RSDK.Cos256(angle) << 10) + x, (RSDK.Sin256(angle) << 10) + y, 4, 0xC0E0E0, INK_ALPHA,
                                                 self->alphaStore >> 1);

                self->alpha     = self->alphaStore;
                self->inkEffect = INK_ALPHA;
                self->drawFX    = FX_FLIP | FX_ROTATE | FX_SCALE;
                self->scale.x   = 0x100;
                self->scale.y   = 0x100;
                if (player->tailSpriteIndex != 0xFFFF) {
                    self->rotation   = player->tailRotation;
                    self->direction  = player->tailDirection;
                    self->velocity.x = player->velocity.x;
                    self->velocity.y = player->velocity.y;
                    RSDK.DrawSprite(&player->tailAnimator, &drawPos, false);
                }
                self->rotation  = player->rotation;
                self->direction = player->direction;
                RSDK.DrawSprite(&player->animator, &drawPos, false);

                self->drawFX     = FX_NONE;
                self->alpha      = 255;
                self->velocity.x = 0;
                self->velocity.y = 0;
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
    recorder->storedAnim  = player->animator.animationID;
    recorder->storedFrame = player->animator.frameID;
}

void ReplayRecorder_StartRecording(EntityPlayer *player)
{
    EntityReplayRecorder *recorder = (EntityReplayRecorder *)ReplayRecorder->recorder_r;
    LogHelpers_Print("ReplayRecorder_StartRecording()");
    recorder->active = ACTIVE_NORMAL;
    memset(globals->replayTempWBuffer, 0, sizeof(globals->replayTempWBuffer));
    memset(globals->replayWriteBuffer, 0, sizeof(globals->replayWriteBuffer));
    ReplayRecorder_Rewind(recorder);
    ReplayRecorder_SetupWriteBuffer();
    ReplayRecorder_Record(recorder, player);
}

void ReplayRecorder_Play(EntityPlayer *player)
{
    LogHelpers_Print("ReplayRecorder_Play()");
    EntityReplayRecorder *recorder = (EntityReplayRecorder *)ReplayRecorder->recorder_w;

    int32 *buffer = NULL;
    if (RSDK.GetEntityID(&ReplayRecorder->recorder_w) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;

    if (buffer[REPLAY_HDR_PACKED]) {
        recorder->active = ACTIVE_NORMAL;
        if (player) {
            recorder->player      = player;
            recorder->playerState = player->state;
        }
        recorder->playing = false;
        if (player->playerID) {
            recorder->playing                 = true;
            ReplayRecorder->hasSetupGhostView = true;
        }
        else {
            Zone->timer              = buffer[REPLAY_HDR_OSC];
            player->stateInputReplay = ReplayRecorder_PlayBackInput;
            player->controllerID     = 2;
            RSDK.AssignControllerID(2, CONT_UNASSIGNED);
            ReplayRecorder->hasSetupGhostVS = true;
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

void ReplayRecorder_Seek(EntityReplayRecorder *recorder, uint32 frame)
{
    LogHelpers_Print("ReplayRecorder_Seek(%u)", frame);
    recorder->replayFrame = frame;

    ReplayFrame *frameBuffer = NULL;
    if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
        frameBuffer = ReplayRecorder->frameBuffer_w;
    else
        frameBuffer = ReplayRecorder->frameBuffer_r;

    int32 newFrame = frame;

    ReplayFrame *framePtr = &frameBuffer[frame];
    while (framePtr->info != REPLAY_INFO_STATECHANGE) {
        if (framePtr->info == REPLAY_INFO_PASSEDGATE)
            break;
        if (newFrame > frame)
            break;
        framePtr--;
        --newFrame;
    }

    if (newFrame <= frame) {
        ReplayRecorder_ForceApplyFramePtr(recorder, framePtr);
        if (newFrame < frame) {
            int32 count      = frame - newFrame;
            ReplayFrame *ptr = &frameBuffer[frame];
            for (int32 i = 0; i < count; ++i) {
                ptr++;
                ReplayRecorder_ApplyFramePtr(recorder, ptr);
            }
        }
    }
}

void ReplayRecorder_SeekFunc(EntityReplayRecorder *recorder)
{
    ReplayFrame *framePtr = NULL;
    if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
        framePtr = ReplayRecorder->frameBuffer_w;
    else
        framePtr = ReplayRecorder->frameBuffer_r;

    for (int32 f = 0; f < recorder->maxFrameCount; ++f) {
        if (framePtr[f].info == REPLAY_INFO_PASSEDGATE) {
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
                            player->aniFrames       = SizeLaser->tailsIndex;
                            player->tailSpriteIndex = SizeLaser->tailSpriteIndex;
                            break;
                        case ID_KNUCKLES: player->aniFrames = SizeLaser->knuxIndex; break;
                        case ID_MIGHTY: player->aniFrames = SizeLaser->mightyIndex; break;
                        case ID_RAY: player->aniFrames = SizeLaser->rayIndex; break;
                        default: player->aniFrames = SizeLaser->sonicIndex; break;
                    }
                }
                else if (RSDK.CheckStageFolder("PSZ2")) {
                    player->aniFrames = Ice->aniFrames;
                }
            }
            else {
                switch (player->characterID) {
                    case ID_TAILS:
                        player->aniFrames       = Player->tailsSpriteIndex;
                        player->tailSpriteIndex = Player->tailsTailsSpriteIndex;
                        break;
                    case ID_KNUCKLES: player->aniFrames = Player->knuxSpriteIndex; break;
                    case ID_MIGHTY: player->aniFrames = Player->mightySpriteIndex; break;
                    case ID_RAY: player->aniFrames = Player->raySpriteIndex; break;
                    default: player->aniFrames = Player->sonicSpriteIndex; break;
                }
            }
        }
    }
}

void ReplayRecorder_ForceApplyFramePtr(EntityReplayRecorder *recorder, ReplayFrame *framePtr)
{
    EntityPlayer *player = recorder->player;
    if (player) {
        player->position.x = framePtr->position.x & 0xFFFF0000;
        player->position.y = framePtr->position.y & 0xFFFF0000;
        player->velocity.x = framePtr->velocity.x & 0xFFFF0000;
        player->velocity.y = framePtr->velocity.y & 0xFFFF0000;
        player->direction  = framePtr->dir;
        player->rotation   = framePtr->rotation;
        ReplayRecorder_SetGimmickState(recorder, (framePtr->flags & REPLAY_FLAG_GIMMICK) != 0);
        RSDK.SetSpriteAnimation(player->aniFrames, framePtr->anim, &player->animator, true, framePtr->frame);
        player->animator.speed = 0;
    }
}

void ReplayRecorder_ApplyFramePtr(EntityReplayRecorder *recorder, ReplayFrame *framePtr)
{
    EntityPlayer *player = recorder->player;
    if (player) {
        if (framePtr->flags & REPLAY_FLAG_POS) {
            player->position.x = framePtr->position.x & 0xFFFF0000;
            player->position.y = framePtr->position.y & 0xFFFF0000;
        }
        if (framePtr->flags & REPLAY_FLAG_VEL) {
            player->velocity.x = framePtr->velocity.y & 0xFFFF0000;
            player->velocity.y = framePtr->velocity.y & 0xFFFF0000;
        }

        if (framePtr->flags & REPLAY_FLAG_ROT)
            player->rotation = framePtr->rotation;

        if (framePtr->flags & REPLAY_FLAG_DIR)
            player->direction = framePtr->dir;

        ReplayRecorder_SetGimmickState(recorder, (framePtr->flags & REPLAY_FLAG_GIMMICK) != 0);

        if (framePtr->flags & REPLAY_FLAG_ANIM) {
            RSDK.SetSpriteAnimation(player->aniFrames, framePtr->anim, &player->animator, true, framePtr->frame);
        }
        else if (framePtr->flags & REPLAY_FLAG_FRAME) {
            RSDK.SetSpriteAnimation(player->aniFrames, player->animator.animationID, &player->animator, true, framePtr->frame);
        }
        player->animator.speed = 0;
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

void ReplayRecorder_PackFrame(ReplayFrame *recording)
{
    RSDK_THIS(ReplayRecorder);

    uint8 buffer[sizeof(ReplayFrame)];
    memset(&buffer, 0, sizeof(ReplayFrame));

    int32 *writeBuffer       = ReplayRecorder->writeBuffer;
    ReplayFrame *frameBuffer = ReplayRecorder->frameBuffer_w;
    int32 size               = ReplayDB_Buffer_PackEntry(buffer, recording);
    memcpy(&frameBuffer[self->replayFrame], recording, sizeof(ReplayFrame));

    float *writeBufferF = (float *)writeBuffer;
    if (writeBuffer[REPLAY_HDR_FRAMECOUNT2]) {
        uint32 frameCount           = writeBuffer[REPLAY_HDR_FRAMECOUNT2];
        float val12                 = writeBufferF[REPLAY_HDR_12];
        float sizef                 = size;
        writeBufferF[REPLAY_HDR_12] = ((val12 * frameCount) + sizef) / (frameCount + 1);
    }
    else {
        writeBufferF[REPLAY_HDR_12] = size;
    }
    writeBuffer[REPLAY_HDR_COMPSIZE] += size;
    ++self->replayFrame;
    ++writeBuffer[REPLAY_HDR_FRAMECOUNT2];
}

void ReplayRecorder_PlayBackInput(void)
{
    RSDK_THIS(Player);
    EntityReplayRecorder *recorder = (EntityReplayRecorder *)ReplayRecorder->recorder_w;

    int32 *buffer = NULL;
    if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;

    if (ReplayRecorder->frameCounter >= buffer[REPLAY_HDR_FRAMECOUNT] && self == recorder->player) {
        ReplayFrame *frameBuffer = NULL;
        if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
            frameBuffer = ReplayRecorder->frameBuffer_w;
        else
            frameBuffer = ReplayRecorder->frameBuffer_r;
        ReplayFrame *framePtr = &frameBuffer[recorder->replayFrame];

        bool32 setPos = false;
        if (framePtr->info) {
            bool32 changeFlag = framePtr->info == REPLAY_INFO_STATECHANGE || framePtr->info == REPLAY_INFO_PASSEDGATE;

            setPos          = changeFlag || (framePtr->info == REPLAY_INFO_USEFLAGS && (framePtr->flags & REPLAY_FLAG_POS));
            bool32 setInput = changeFlag || (framePtr->info == REPLAY_INFO_USEFLAGS && (framePtr->flags & REPLAY_FLAG_INPUT));
            if (setInput) {
                int32 inputs                                     = framePtr->inputs;
                ControllerInfo[self->controllerID].keyUp.down    = (inputs & 0x01) != 0;
                ControllerInfo[self->controllerID].keyDown.down  = (inputs & 0x02) != 0;
                ControllerInfo[self->controllerID].keyLeft.down  = (inputs & 0x04) != 0;
                ControllerInfo[self->controllerID].keyRight.down = (inputs & 0x08) != 0;
                ControllerInfo[self->controllerID].keyA.press    = (inputs & 0x10) != 0;
                ControllerInfo[self->controllerID].keyA.down     = (inputs & 0x20) != 0;
            }
        }

        RSDKControllerState *controller = &ControllerInfo[self->controllerID];
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
            recorder->storedPos.x = framePtr->position.x;
            recorder->storedPos.y = framePtr->position.y;
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
        self->animator.speed = 0;

        ReplayFrame *buffer = NULL;
        if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
            buffer = ReplayRecorder->frameBuffer_w;
        else
            buffer = ReplayRecorder->frameBuffer_r;

        ReplayFrame *framePtr = &buffer[recorder->replayFrame];
        if (recorder->state) {
            if (framePtr->info == REPLAY_INFO_USEFLAGS) {
                ReplayRecorder_ApplyFramePtr(recorder, framePtr);
            }
            else if (framePtr->info == REPLAY_INFO_STATECHANGE || framePtr->info == REPLAY_INFO_PASSEDGATE) {
                ReplayRecorder_ForceApplyFramePtr(recorder, framePtr);
                if (framePtr->info == REPLAY_INFO_PASSEDGATE && !ReplayRecorder->packedStartFrame) {
                    ReplayRecorder->packedStartFrame = true;
                    if (!ReplayRecorder->passedStartLine) {
                        ReplayRecorder_Pause(recorder);
                    }
                }
            }
        }
        else {
            self->velocity.x = 0;
            self->velocity.y = 0;
        }

        if (FarPlane) {
            int32 screenX = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
            int32 screenY = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;

            self->scale.x   = 0x200;
            self->scale.y   = 0x200;
            self->drawOrder = Zone->playerDrawLow;
            foreach_all(FarPlane, farPlane)
            {
                Hitbox hitbox;
                hitbox.left   = -(farPlane->size.x >> 16);
                hitbox.top    = -(farPlane->size.y >> 16);
                hitbox.right  = (farPlane->size.x >> 16);
                hitbox.bottom = (farPlane->size.y >> 16);
                if (MathHelpers_PointInHitbox(FLIP_NONE, farPlane->origin.x, farPlane->origin.y, &hitbox, self->position.x, self->position.y)) {
                    self->position.x =
                        (farPlane->position.x - ((farPlane->position.x - screenX) >> 1) + 0x8000) + ((self->position.x - farPlane->origin.x) >> 1);
                    self->position.y =
                        (farPlane->position.y - ((farPlane->position.y - screenY) >> 1) + 0x8000) + ((self->position.y - farPlane->origin.y) >> 1);
                    self->drawFX |= FX_SCALE;
                    self->scale.x   = 0x100;
                    self->scale.y   = 0x100;
                    self->drawOrder = Zone->drawOrderLow;
                    foreach_break;
                }
            }
        }

        if (ReplayRecorder->packedStartFrame && !ReplayRecorder->passedStartLine) {
            self->visible = ~(self->timer >> 2) & 1;
            self->timer++;
        }
    }
}

void ReplayRecorder_StatePlay(void)
{
    RSDK_THIS(ReplayRecorder);
    int32 *buffer = NULL;
    if (RSDK.GetEntityID(self) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;

    ReplayFrame *frameBuffer = NULL;
    if (RSDK.GetEntityID(self) == SLOT_REPLAYRECORDER_W)
        frameBuffer = ReplayRecorder->frameBuffer_w;
    else
        frameBuffer = ReplayRecorder->frameBuffer_r;

    if (ReplayRecorder->frameCounter >= buffer[REPLAY_HDR_FRAMECOUNT]) {
        if (ReplayRecorder->frameCounter != buffer[REPLAY_HDR_FRAMECOUNT]) {
            if (!self->playing)
                ReplayRecorder_Seek(self, ReplayRecorder->frameCounter - buffer[REPLAY_HDR_FRAMECOUNT]);
            self->replayFrame = ReplayRecorder->frameCounter - buffer[REPLAY_HDR_FRAMECOUNT];
        }
        self->stateLate = ReplayRecorder_StateLate_Replay;
        self->state     = ReplayRecorder_None_Replay;
    }
    else if (ReplayRecorder->frameCounter < buffer[REPLAY_HDR_FRAMECOUNT]) {
        if (self->playing) {
            ReplayRecorder_ForceApplyFramePtr(self, frameBuffer);
        }
    }
}

void ReplayRecorder_None_Replay(void) {}

void ReplayRecorder_StateLate_Replay(void)
{
    RSDK_THIS(ReplayRecorder);
    EntityPlayer *player = self->player;
    if (!self->replayFrame) {
        EntityCamera *camera = player->camera;
        if (camera) {
            camera->position.x                      = player->position.x;
            camera->position.y                      = player->position.y;
            ScreenInfo[camera->screenID].position.x = (camera->position.x >> 16);
            ScreenInfo[camera->screenID].position.y = (camera->position.y >> 16);
            if (WarpDoor)
                WarpDoor_SetupBoundaries(-1, NULL);
            else
                WarpDoor_CheckAllBounds();
            camera->state       = Camera_State_Follow;
            player->scrollDelay = 0;
        }
    }
    if (self->playing) {
        if (player->state != ReplayRecorder_PlayerState)
            self->playerState = player->state;
    }

    ReplayFrame *frameBuffer = NULL;
    if (RSDK.GetEntityID(self) == SLOT_REPLAYRECORDER_W)
        frameBuffer = ReplayRecorder->frameBuffer_w;
    else
        frameBuffer = ReplayRecorder->frameBuffer_r;

    ReplayFrame *framePtr = &frameBuffer[self->replayFrame];

    if (!self->playing) {
        if (framePtr->info) {
            if ((framePtr->info == REPLAY_INFO_STATECHANGE || framePtr->info == REPLAY_INFO_PASSEDGATE)
                || (framePtr->info == REPLAY_INFO_USEFLAGS && (framePtr->flags & REPLAY_FLAG_POS))) {
                int32 distX = abs(player->position.x - framePtr->position.x);
                int32 distY = abs(player->position.y - framePtr->position.y);
                if (MathHelpers_SquareRoot((distX >> 16) * (distX >> 16) + (distY >> 16) * (distY >> 16)) << 16 >= 0x20000) {
                    player->position.x += (framePtr->position.x - player->position.x) >> 1;
                    player->position.y += (framePtr->position.y - player->position.y) >> 1;
                }
            }
        }
    }
    ++self->replayFrame;
}

void ReplayRecorder_None_Record(void) {}

void ReplayRecorder_RecordFrameData(void)
{
    ReplayFrame frame;

    RSDK_THIS(ReplayRecorder);
    if (self->replayFrame < self->maxFrameCount - 1) {
        EntityPlayer *player = self->player;
        int32 inputState     = 0;
        memset(&frame, 0, sizeof(frame));

        RSDKControllerState *controller = &ControllerInfo[player->controllerID];
        if (controller->keyUp.down || AnalogStickInfoL[player->controllerID].keyUp.down)
            inputState = 0x01;
        if (controller->keyDown.down || AnalogStickInfoL[player->controllerID].keyDown.down)
            inputState |= 0x02;
        if (controller->keyLeft.down || AnalogStickInfoL[player->controllerID].keyLeft.down)
            inputState |= 0x04;
        if (controller->keyRight.down || AnalogStickInfoL[player->controllerID].keyRight.down)
            inputState |= 0x08;
        if (controller->keyA.press || controller->keyB.press || controller->keyC.press || controller->keyX.press)
            inputState |= 0x10;
        if (controller->keyA.down || controller->keyB.down || controller->keyC.down || controller->keyX.down)
            inputState |= 0x20;

        bool32 isGimmickState = ReplayRecorder_CheckPlayerGimmickState(self);
        self->stateStore      = player->state;

        EntityIce *ice     = player->abilityPtrs[1];
        Animator *animator = &player->animator;
        if (isGimmickState && RSDK.CheckStageFolder("PSZ2") && player->state == Ice_State_FrozenPlayer && ice->objectID == Ice->objectID) {
            animator = &ice->animator2;
        }
        void *storedState = self->storedState;
        self->animID      = animator->animationID;
        self->frameID     = animator->frameID;

        for (int32 i = 0; i < 64; ++i) {
            if (storedState == ReplayRecorder->actions[i])
                break;
        }

        for (int32 i = 0; i < 64; ++i) {
            if (self->stateStore == ReplayRecorder->actions[i])
                break;
        }

        // byte 0 info:
        // 0 = none
        // 1 = state change (first frame is always a state change)
        // 2 = some values changed (see byte 1 for info)
        // 3 = passed TA Gate

        // byte 1 info:
        // its flags for what vals changed

        if (self->replayFrame == 0 || self->changeFlags > 0 || self->storedState != self->stateStore) {
            frame.info       = self->changeFlags == 2 ? REPLAY_INFO_PASSEDGATE : REPLAY_INFO_STATECHANGE;
            frame.flags      = REPLAY_FLAG_NONE;
            frame.inputs     = inputState;
            frame.dir        = player->direction;
            frame.rotation   = player->rotation;
            frame.position.x = player->position.x;
            frame.position.y = player->position.y;
            frame.velocity.x = player->velocity.x;
            frame.velocity.y = player->velocity.y;
            frame.anim       = self->animID;
            frame.frame      = self->frameID;
            if (isGimmickState)
                frame.flags = REPLAY_FLAG_GIMMICK;
            self->changeFlags = 0;
        }
        else {
            int32 changedVals = 0;
            if (inputState != self->storedInputs) {
                frame.inputs      = inputState;
                changedVals  = REPLAY_FLAG_INPUT;
            }
            if (player->direction != self->storedDirection) {
                frame.dir= player->direction;
                changedVals |= REPLAY_FLAG_DIR;
            }
            if (player->position.x != self->storedPos.x || player->position.y != self->storedPos.y) {
                changedVals |= REPLAY_FLAG_POS;
                frame.position.x = player->position.x;
                frame.position.y = player->position.y;
            }
            if (player->velocity.x != self->storedVel.x || player->velocity.y != self->storedVel.y) {
                changedVals |= REPLAY_FLAG_VEL;
                frame.velocity.x = player->velocity.x;
                frame.velocity.y = player->velocity.y;
            }
            //what ??? ??? ??? this is in mania, not my thing
            if (player->groundVel != self->storedRotation) {
                changedVals |= REPLAY_FLAG_ROT;
                frame.rotation = player->rotation;
            }
            if (animator->animationID != self->storedAnim) {
                changedVals |= (REPLAY_FLAG_ANIM | REPLAY_FLAG_FRAME);
                frame.anim      = self->animID;
                frame.frame        = self->frameID;
            }
            if (animator->frameID != self->storedFrame) {
                changedVals |= REPLAY_FLAG_FRAME;
                frame.frame = self->frameID;
            }

            if (isGimmickState)
                changedVals |= REPLAY_FLAG_GIMMICK;

            if (changedVals) {
                frame.info       = REPLAY_INFO_USEFLAGS;
                frame.flags     = changedVals;
            }
            else {
                frame.info = REPLAY_INFO_NONE;
            }
        }

        ReplayRecorder_PackFrame(&frame);
        self->storedPos.x     = player->position.x;
        self->storedPos.y     = player->position.y;
        self->storedVel.x     = player->velocity.x;
        self->storedVel.y     = player->velocity.y;
        self->storedDirection = player->direction;
        self->storedSpeed     = player->groundVel;
        self->storedRotation  = player->rotation;
        self->storedInputs    = inputState;
        self->storedAnim      = animator->animationID;
        self->storedFrame     = animator->frameID;
    }
    else {
        self->state     = StateMachine_None;
        self->stateLate = StateMachine_None;
    }
}

void ReplayRecorder_LoadReplayDB(void (*callback)(bool32))
{
    if ((globals->replayTableID != -1 && globals->replayTableLoaded == STATUS_OK) || globals->replayTableLoaded == STATUS_CONTINUE)
        return;
    LogHelpers_Print("Loading Replay DB");
    globals->replayTableLoaded = STATUS_CONTINUE;
    ReplayDB->loadEntity       = SceneInfo->entity;
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
        ReplayDB->saveEntity   = SceneInfo->entity;
        ReplayDB->saveCallback = callback;
        API.SaveUserDB(globals->replayTableID, ReplayRecorder_ReplaySaveFinish);
    }
}

void ReplayRecorder_CreateReplayDB(void)
{
    globals->replayTableID = API.InitUserDB("ReplayDB.bin", DBVAR_UINT32, "score", DBVAR_UINT8, "zoneID", DBVAR_UINT8, "act", DBVAR_UINT8,
                                            "characterID", DBVAR_UINT8, "encore", DBVAR_UINT32, "zoneSortVal", NULL);

    if (globals->replayTableID == -1)
        globals->replayTableLoaded = STATUS_ERROR;
    else
        globals->replayTableLoaded = STATUS_OK;
}

uint32 ReplayRecorder_AddReplayID(uint8 actID, char zone, int32 charID, int32 score, char mode)
{
    if (globals->replayTableLoaded == STATUS_OK) {
        uint32 rowID       = API.AddUserDBRow(globals->replayTableID);
        int32 zoneStortVal = score & 0x3FFFFFF | ((actID & 1 | 2 * (mode & 1 | 2 * zone)) << 26);
        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT32, "score", &score);
        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT8, "zoneID", &zone);
        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT8, "act", &actID);
        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT8, "characterID", &charID);
        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT8, "encore", &actID);
        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT32, "zoneSortVal", &zoneStortVal);
        uint32 UUID = API.GetUserDBRowUUID(globals->replayTableID, rowID);
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

void ReplayRecorder_DeleteTimeAttackRow(int32 row, void (*callback)(bool32), bool32 useAltCB)
{
    int32 id                 = API.GetUserDBRowUUID(globals->replayTableID, row);
    int32 value              = 0;
    ReplayDB->deleteEntity   = SceneInfo->entity;
    ReplayDB->deleteCallback = callback;
    API.RemoveDBRow(globals->replayTableID, row);
    TimeAttackData->loaded = false;
    API.SetupUserDBRowSorting(globals->taTableID);
    API.AddRowSortFilter(globals->taTableID, DBVAR_UINT32, "replayID", &id);
    int32 count = API.GetSortedUserDBRowCount(globals->taTableID);
    for (int32 i = 0; i < count; ++i) {
        uint32 uuid = API.GetSortedUserDBRowID(globals->taTableID, i);
        LogHelpers_Print("Deleting Time Attack replay from row #%d", uuid);
        API.SetUserDBValue(globals->taTableID, uuid, DBVAR_UINT32, "replayID", &value);
    }

    char filename[0x20];
    sprintf(filename, "Replay_%08X.bin", id);
    if (!useAltCB)
        API.DeleteUserFile(filename, ReplayRecorder_DeleteReplayCB);
    else
        API.DeleteUserFile(filename, ReplayRecorder_DeleteReplaySave2CB);
}

void ReplayRecorder_DeleteReplayCB(int32 status)
{
    LogHelpers_Print("DeleteReplay_CB(%d)", status);
    API.SaveUserDB(globals->replayTableID, ReplayRecorder_DeleteReplaySaveCB);
}

void ReplayRecorder_DeleteReplaySaveCB(int32 status)
{
    LogHelpers_Print("DeleteReplaySave_CB(%d)", status);
    API.SaveUserDB(globals->taTableID, ReplayRecorder_DeleteReplaySave2CB);
}

void ReplayRecorder_DeleteReplaySave2CB(int32 status)
{
    LogHelpers_Print("DeleteReplaySave2_CB(%d)", status);
    if (ReplayDB->deleteCallback) {
        Entity *store = SceneInfo->entity;
        if (ReplayDB->deleteEntity)
            SceneInfo->entity = ReplayDB->deleteEntity;
        ReplayDB->deleteCallback(status == STATUS_OK);
        SceneInfo->entity        = store;
        ReplayDB->deleteCallback = NULL;
        ReplayDB->deleteEntity   = NULL;
    }
}

int32 ReplayRecorder_SetStatus(int32 status)
{
    if (status == STATUS_OK) {
        globals->replayTableLoaded = STATUS_OK;
        API.SetupUserDBRowSorting(globals->replayTableID);
        LogHelpers_Print("Load Succeeded! Replay count: %d", API.GetSortedUserDBRowCount(globals->replayTableID));
    }
    else {
        LogHelpers_Print("Load Failed! Creating new Replay DB");
        ReplayRecorder_CreateReplayDB();
    }
    LogHelpers_Print("Replay DB Slot => %d, Load Status => %d", globals->replayTableID, globals->replayTableLoaded);

    if (ReplayDB->loadCallback) {
        Entity *store = SceneInfo->entity;
        if (ReplayDB->loadEntity)
            SceneInfo->entity = ReplayDB->loadEntity;
        ReplayDB->loadCallback(status == STATUS_OK);
        SceneInfo->entity      = store;
        ReplayDB->loadCallback = NULL;
        ReplayDB->loadEntity   = NULL;
    }
    return 1;
}

int32 ReplayRecorder_ReplaySaveFinish(int32 status)
{
    if (ReplayDB->saveCallback) {
        Entity *store = SceneInfo->entity;
        if (ReplayDB->saveEntity)
            SceneInfo->entity = ReplayDB->saveEntity;
        ReplayDB->saveCallback(status == STATUS_OK);
        SceneInfo->entity      = store;
        ReplayDB->saveCallback = NULL;
        ReplayDB->saveEntity   = NULL;
    }
    return 1;
}

#if RETRO_INCLUDE_EDITOR
void ReplayRecorder_EditorDraw(void) {}

void ReplayRecorder_EditorLoad(void) {}
#endif

void ReplayRecorder_Serialize(void) {}
#endif
