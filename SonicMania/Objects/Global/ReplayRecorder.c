// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ReplayRecorder Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
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

        Replay *replayPtr = NULL;
        if (RSDK.GetEntitySlot(self) == SLOT_REPLAYRECORDER_RECORD)
            replayPtr = ReplayRecorder->recordBuffer;
        else
            replayPtr = ReplayRecorder->playbackBuffer;

        if (self->replayFrame > replayPtr->header.frameCount && self->state)
            ReplayRecorder_Stop(self);

        if (ReplayRecorder->reachedGoal && !self->player->playerID && self->state == ReplayRecorder_State_Record) {
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
                    EntityMenuParam *param = MenuParam_GetParam();

                    player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                    API_SetAchievementsEnabled(false);
                    API.SetStatsEnabled(false);
                    TimeAttackGate->disableRecords = true;
                    TimeAttackData->personalRank   = param->replayRankID;
                }
                else {
                    player = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                }

                EntityReplayRecorder *playbackManager = ReplayRecorder->playbackManager;
                ReplayRecorder_Rewind(playbackManager);
                ReplayRecorder_Play(player);
                ReplayRecorder->startPlayback = false;
                ReplayRecorder->isReplaying   = true;
            }

            EntityReplayRecorder *recordingManager = ReplayRecorder->recordingManager;
            EntityPlayer *recordingPlayer          = recordingManager->player;
            if (recordingPlayer) {
                recordingManager->prevPlayerState = recordingPlayer->state;
                recordingManager->storedAnim      = recordingPlayer->animator.animationID;
                recordingManager->storedFrame     = recordingPlayer->animator.frameID;
            }

            EntityReplayRecorder *playbackManager = ReplayRecorder->playbackManager;
            EntityPlayer *playbackPlayer          = playbackManager->player;
            if (playbackManager->isGhostPlayback && playbackPlayer)
                playbackPlayer->state = ReplayRecorder_PlayerState_PlaybackReplay;

            if (!playbackManager->isGhostPlayback) {
                if ((ControllerInfo->keyStart.press || Unknown_pausePress) && SceneInfo->state == ENGINESTATE_REGULAR) {
                    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

                    bool32 allowPause = true;
                    if (ActClear && ActClear->actClearActive)
                        allowPause = false;

                    if (!RSDK.GetEntityCount(TitleCard->classID, false) && !pauseMenu->classID && allowPause) {
                        RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
                        pauseMenu->triggerPlayer = RSDK.GetEntitySlot(SceneInfo->entity);
                        if (globals->gameMode == MODE_COMPETITION)
                            pauseMenu->disableRestart = true;
                    }
                }
            }
        }
    }
}

void ReplayRecorder_Draw(void)
{
    RSDK_THIS(ReplayRecorder);

    if (self->isGhostPlayback)
        ReplayRecorder_DrawGhostDisplay();
}

void ReplayRecorder_Create(void *data)
{
    RSDK_THIS(ReplayRecorder);

    self->active          = ACTIVE_NEVER;
    self->drawGroup       = 13;
    self->inkEffect       = INK_NONE;
    self->replayStopDelay = -1;
    self->drawFX          = FX_FLIP;
    self->ghostAlpha      = 0x100;
    self->visible         = globals->gameMode == MODE_TIMEATTACK;
}

void ReplayRecorder_StageLoad(void)
{
    ReplayRecorder->replayID    = 0;
    ReplayRecorder->replayRowID = -1;

    if (globals->gameMode == MODE_TIMEATTACK) {
        ReplayRecorder->active = ACTIVE_ALWAYS;

        ReplayRecorder->frameCounter      = -1;
        ReplayRecorder->savedReplay       = 0;
        ReplayRecorder->initialized       = false;
        ReplayRecorder->startedRecording  = false;
        ReplayRecorder->isReplaying       = false;
        ReplayRecorder->hasSetupGhostVS   = false;
        ReplayRecorder->hasSetupGhostView = false;
        ReplayRecorder->passedStartLine   = false;
        ReplayRecorder->reachedGoal       = false;
        ReplayRecorder->packedStartFrame  = false;
        ReplayRecorder->loadCallback      = NULL;
        ReplayRecorder->fileBuffer        = NULL;

        memset(ReplayRecorder->filename, 0, sizeof(ReplayRecorder->filename));
        ReplayRecorder->recordBuffer    = (Replay *)globals->replayWriteBuffer;
        ReplayRecorder->recordingFrames = ReplayRecorder->recordBuffer->frames;
        ReplayRecorder->playbackBuffer  = (Replay *)globals->replayReadBuffer;
        ReplayRecorder->playbackFrames  = ReplayRecorder->playbackBuffer->frames;

        if (Zone) {
            if (!SceneInfo->inEditor)
                ReplayRecorder_SetupActions();

            RSDK.SetRandSeed(1624633040);

            RSDK.ResetEntitySlot(SLOT_REPLAYRECORDER_RECORD, ReplayRecorder->classID, NULL);
            EntityReplayRecorder *recordingManager = RSDK_GET_ENTITY(SLOT_REPLAYRECORDER_RECORD, ReplayRecorder);
            recordingManager->maxFrameCount        = REPLAY_MAX_FRAMECOUNT;
            ReplayRecorder->recordingManager       = recordingManager;

            RSDK.ResetEntitySlot(SLOT_REPLAYRECORDER_PLAYBACK, ReplayRecorder->classID, NULL);
            EntityReplayRecorder *playbackManager = RSDK_GET_ENTITY(SLOT_REPLAYRECORDER_PLAYBACK, ReplayRecorder);
            playbackManager->maxFrameCount        = REPLAY_MAX_FRAMECOUNT;
            ReplayRecorder->playbackManager       = playbackManager;

            ReplayRecorder->startRecording = false;
            ReplayRecorder->startPlayback  = false;

            Replay *replayPtr = NULL;
            if (RSDK.GetEntitySlot(ReplayRecorder->playbackManager) == SLOT_REPLAYRECORDER_RECORD)
                replayPtr = ReplayRecorder->recordBuffer;
            else
                replayPtr = ReplayRecorder->playbackBuffer;

            EntityMenuParam *param = MenuParam_GetParam();
            if (param->viewReplay && replayPtr->header.isNotEmpty) {
                if (param->showGhost) {
                    globals->playerID        = GET_CHARACTER_ID(1) | (GET_CHARACTER_ID(1) << 8);
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
    Replay *buffer = NULL;
    if (RSDK.GetEntitySlot(ReplayRecorder->playbackManager) == SLOT_REPLAYRECORDER_RECORD)
        buffer = ReplayRecorder->recordBuffer;
    else
        buffer = ReplayRecorder->playbackBuffer;

    if (Player->playerCount != 1 || !buffer->header.isNotEmpty)
        ReplayRecorder->startRecording = true;
    if (buffer->header.isNotEmpty)
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
    if (ReplayRecorder->startedRecording)
        ReplayRecorder->recordingManager->changeFlags = 2; // Passed Gate

    EntityReplayRecorder *recorder = ReplayRecorder->playbackManager;
    if (ReplayRecorder->isReplaying && recorder->isGhostPlayback) {
        if (ReplayRecorder->packedStartFrame)
            ReplayRecorder_Resume(recorder);
        else
            ReplayRecorder_SeekFunc(recorder);
    }

    ReplayRecorder->passedStartLine = true;
}

void ReplayRecorder_FinishCB(void)
{
    EntityReplayRecorder *recorder = ReplayRecorder->recordingManager;
    if (ReplayRecorder->startedRecording)
        recorder->replayStopDelay = 120;

    ReplayRecorder->reachedGoal = true;
}

void ReplayRecorder_Buffer_Move(void)
{
    Replay *replayPtr = NULL;
    if (RSDK.GetEntitySlot(ReplayRecorder->recordingManager) == SLOT_REPLAYRECORDER_RECORD)
        replayPtr = ReplayRecorder->recordBuffer;
    else
        replayPtr = ReplayRecorder->playbackBuffer;

    if (replayPtr->header.isNotEmpty) {
        if (replayPtr->header.frameCount < ReplayRecorder->recordingManager->maxFrameCount - 1) {
            memset(globals->replayTempWBuffer, 0, sizeof(globals->replayTempWBuffer));
            LogHelpers_Print("Buffer_Move(0x%08x, 0x%08x)", globals->replayTempWBuffer, replayPtr);
            memcpy(globals->replayTempWBuffer, replayPtr, sizeof(globals->replayTempWBuffer));
            memset(replayPtr, 0, sizeof(globals->replayWriteBuffer));
            ReplayRecorder_Buffer_PackInPlace(globals->replayTempWBuffer);
            HUD->replaySaveEnabled = true;
        }
    }
}

void ReplayRecorder_SaveReplayDLG_NoCB(void)
{
    HUD->replaySaveEnabled = true;

    ActClear->hasSavedReplay      = false;
    ActClear->disableResultsInput = false;
}

void ReplayRecorder_SaveReplayDLG_YesCB(void)
{
    ReplayRecorder->replayID    = 0;
    ReplayRecorder->replayRowID = -1;

    int32 mins      = SceneInfo->minutes;
    int32 secs      = SceneInfo->seconds;
    int32 millisecs = SceneInfo->milliseconds;
    LogHelpers_Print("Bout to create ReplayDB entry...");

    EntityMenuParam *param = MenuParam_GetParam();
    int32 rowID            = ReplayDB_AddReplay(param->zoneID, param->actID, param->characterID, millisecs + 100 * (secs + 60 * mins),
                                             SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE));
    if (rowID == -1) {
        LogHelpers_Print("Table row ID invalid! %d", -1);
        ReplayRecorder_SaveFile_Replay(false);
    }
    else {
        ReplayRecorder->replayID    = API.GetUserDBRowUUID(globals->replayTableID, rowID);
        ReplayRecorder->replayRowID = rowID;

        char fileName[0x20];
        sprintf_s(fileName, (int32)sizeof(fileName), "Replay_%08X.bin", ReplayRecorder->replayID);
        LogHelpers_Print("Replay Filename: %s", fileName);
        UIWaitSpinner_StartWait();

        ReplayRecorder->savedReplay = true;
        ReplayRecorder_Buffer_SaveFile(fileName, globals->replayTempWBuffer, ReplayRecorder_SaveFile_Replay);
        HUD->replaySaveEnabled = false;
    }
}

void ReplayRecorder_SaveReplayDLG_CB(void)
{
    String message;
    INIT_STRING(message);

    Localization_GetString(&message, STR_SAVEREPLAY);
    UIDialog_CreateDialogYesNo(&message, ReplayRecorder_SaveReplayDLG_YesCB, ReplayRecorder_SaveReplayDLG_NoCB, true, true);
}

void ReplayRecorder_SaveReplay(void)
{
    Replay *replayPtr = (Replay *)globals->replayTempWBuffer;

    if (replayPtr->header.isNotEmpty) {
        LogHelpers_Print("Saving replay...");

        foreach_all(HUD, hud) { foreach_break; }
        RSDK.SetSpriteAnimation(HUD->aniFrames, 11, &hud->replayClapAnimator, true, 0);
        RSDK.PlaySfx(HUD->sfxClick, false, 0xFF);

        EntityDialogRunner *callback = CREATE_ENTITY(DialogRunner, DialogRunner_HandleCallback, 0, 0);
        callback->callback           = ReplayRecorder_SaveReplayDLG_CB;
        callback->timer              = 45;
        callback->isPermanent        = true;
    }
    else {
        LogHelpers_Print("Can't save replay! No data available");
        ActClear->disableResultsInput = false;
    }
}

void ReplayRecorder_SaveFile_Replay(bool32 success)
{
    if (success) {
        LogHelpers_Print("Replay save successful!");
        ReplayDB_SaveDB(ReplayRecorder_SaveCallback_ReplayDB);
    }
    else {
        if (ReplayRecorder->replayRowID != -1)
            API.RemoveDBRow(globals->replayTableID, ReplayRecorder->replayRowID);

        String message;
        INIT_STRING(message);
        Localization_GetString(&message, STR_NOREPLAYSPACE);
        UIDialog_CreateDialogOk(&message, NULL, true);

        UIWaitSpinner_FinishWait();

        ActClear->disableResultsInput = false;
        ActClear->hasSavedReplay      = false;
        HUD->replaySaveEnabled        = true;
    }
}

void ReplayRecorder_SaveCallback_ReplayDB(bool32 success)
{
    if (success) {
        if (TimeAttackData->rowID == -1) {
            UIWaitSpinner_FinishWait();
            ActClear->disableResultsInput = false;
            foreach_all(HUD, hud)
            {
                RSDK.SetSpriteAnimation(HUD->aniFrames, 10, &hud->replayClapAnimator, true, 0);
                foreach_break;
            }
            RSDK.PlaySfx(HUD->sfxStarpost, true, 255);
            HUD->replaySaveEnabled = false;
        }
        else {
            API.SetUserDBValue(globals->taTableID, TimeAttackData->rowID, DBVAR_UINT32, "replayID", &ReplayRecorder->replayID);
            TimeAttackData_SaveDB(ReplayRecorder_SaveCallback_TimeAttackDB);
        }
    }
    else {
        String message;
        INIT_STRING(message);

        char fileName[0x20];
        sprintf_s(fileName, (int32)sizeof(fileName), "Replay_%08X.bin", ReplayRecorder->replayID);

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

void ReplayRecorder_SaveCallback_TimeAttackDB(bool32 success)
{
    UIWaitSpinner_FinishWait();

    ActClear->disableResultsInput = false;
    if (!success) {
        ActClear->hasSavedReplay = false;
        HUD->replaySaveEnabled   = true;
    }
    else {
        foreach_all(HUD, hud)
        {
            RSDK.SetSpriteAnimation(HUD->aniFrames, 10, &hud->replayClapAnimator, true, 0);
            foreach_break;
        }
        RSDK.PlaySfx(HUD->sfxStarpost, false, 255);

        HUD->replaySaveEnabled = false;
    }
}

void ReplayRecorder_Buffer_PackInPlace(int32 *tempWriteBuffer)
{
    Replay *replayPtr = (Replay *)tempWriteBuffer;

    LogHelpers_Print("Buffer_PackInPlace(%08x)", tempWriteBuffer);

    if (replayPtr->header.signature == REPLAY_SIGNATURE) {
        if (replayPtr->header.isPacked) {
            LogHelpers_Print("Buffer_Ppack ERROR: Buffer is already packed");
        }
        else {
            int32 compressedSize   = sizeof(ReplayHeader);
            int32 uncompressedSize = sizeof(ReplayFrame) * (replayPtr->header.frameCount + 2);

            ReplayFrame *framePtr   = replayPtr->frames;
            uint8 *compressedFrames = (uint8 *)replayPtr->frames;
            for (int32 f = 0; f < replayPtr->header.frameCount; ++f) {
                ReplayFrame uncompressedFrame;
                memcpy(&uncompressedFrame, framePtr, sizeof(ReplayFrame));

                memset(framePtr, 0, sizeof(ReplayFrame));

                int32 size = ReplayDB_Buffer_PackEntry(compressedFrames, &uncompressedFrame);
                compressedFrames += size;
                compressedSize += size;
                framePtr++;
            }
            LogHelpers_Print("Packed %d frames: %luB -> %luB", replayPtr->header.frameCount, uncompressedSize, compressedSize);

            replayPtr->header.bufferSize = compressedSize;
            replayPtr->header.isPacked   = true;
        }
    }
    else {
        LogHelpers_Print("Buffer_Pack ERROR: Signature does not match");
    }
}

void ReplayRecorder_Buffer_Unpack(int32 *readBuffer, int32 *tempReadBuffer)
{
    LogHelpers_Print("Buffer_Unpack(0x%08x, 0x%08x)", readBuffer, tempReadBuffer);
    Replay *replayPtr     = (Replay *)readBuffer;
    Replay *tempReplayPtr = (Replay *)tempReadBuffer;

    uint8 *compressedFrames = (uint8 *)tempReplayPtr->frames;
    if ((uint32)*tempReadBuffer == REPLAY_SIGNATURE) {
        if (tempReplayPtr->header.isPacked) {
            int32 compressedSize            = tempReplayPtr->header.bufferSize;
            replayPtr->header.signature     = tempReplayPtr->header.signature;
            replayPtr->header.version       = tempReplayPtr->header.version;
            replayPtr->header.isPacked      = tempReplayPtr->header.isPacked;
            replayPtr->header.isNotEmpty    = tempReplayPtr->header.isNotEmpty;
            replayPtr->header.frameCount    = tempReplayPtr->header.frameCount;
            replayPtr->header.startingFrame = tempReplayPtr->header.startingFrame;
            int32 uncompressedSize          = sizeof(ReplayFrame) * (tempReplayPtr->header.frameCount + 2);
            ReplayFrame *uncompressedBuffer = replayPtr->frames;

            for (int32 i = 0; i < tempReplayPtr->header.frameCount; ++i) {
                int32 size = ReplayDB_Buffer_UnpackEntry(uncompressedBuffer, compressedFrames);
                compressedFrames += size;
                uncompressedBuffer++;
            }
            LogHelpers_Print("Unpacked %d frames: %luB -> %luB", tempReplayPtr->header.frameCount, compressedSize, uncompressedSize);

            replayPtr->header.isPacked   = false;
            replayPtr->header.bufferSize = uncompressedSize;
            memset(tempReadBuffer, 0, sizeof(globals->replayTempRBuffer));
        }
        else {
            LogHelpers_Print("Buffer_Unpack ERROR: Buffer is not packed");
        }
    }
    else {
        LogHelpers_Print("Buffer_Unpack ERROR: Signature does not match");
    }
}

void ReplayRecorder_Buffer_SaveFile(const char *fileName, int32 *buffer, void (*callback)(bool32 success))
{
    LogHelpers_Print("Buffer_SaveFile(%s, %08x)", fileName, buffer);

    Replay *replayPtr = (Replay *)buffer;
    if (replayPtr->header.isNotEmpty) {
        ReplayRecorder->saveCallback = callback;
        API_SaveUserFile(fileName, buffer, replayPtr->header.bufferSize, ReplayRecorder_SaveReplayCallback, true);
    }
    else {
        LogHelpers_Print("Attempted to save an empty replay buffer");
        if (callback)
            callback(false);
    }
}

void ReplayRecorder_SaveReplayCallback(int32 status)
{
    if (ReplayRecorder->saveCallback)
        ReplayRecorder->saveCallback(status == STATUS_OK);

    ReplayRecorder->saveCallback = NULL;
}

void ReplayRecorder_Buffer_LoadFile(const char *fileName, void *buffer, void (*callback)(bool32 success))
{
    LogHelpers_Print("Buffer_LoadFile(%s, %08x)", fileName, buffer);

    memset(buffer, 0, sizeof(globals->replayReadBuffer));
    ReplayRecorder->fileBuffer   = buffer;
    ReplayRecorder->loadCallback = callback;
    strcpy(ReplayRecorder->filename, fileName);

    API_LoadUserFile(fileName, buffer, sizeof(globals->replayReadBuffer), ReplayRecorder_LoadReplayCallback);
}

void ReplayRecorder_LoadReplayCallback(int32 status)
{
    if (ReplayRecorder->loadCallback)
        ReplayRecorder->loadCallback(status == STATUS_OK);

    ReplayRecorder->loadCallback = NULL;
    ReplayRecorder->fileBuffer   = NULL;
    memset(ReplayRecorder->filename, 0, sizeof(ReplayRecorder->filename));
}

void ReplayRecorder_ConfigureGhost_CB(void)
{
    RSDK_THIS(Player);

    LogHelpers_Print("ConfigureGhost_CB()");
    LogHelpers_Print("Ghost Slot %d", self->playerID);

    LogHelpers_PrintVector2("Ghost pos ", self->position);
    self->isGhost        = true;
    self->stateInput     = StateMachine_None;
    self->state          = ReplayRecorder_PlayerState_PlaybackReplay;
    self->sidekick       = false;
    self->interaction    = false;
    self->tileCollisions = TILECOLLISION_NONE;
    self->visible        = true;
    self->alpha          = 0xFF;
}

void ReplayRecorder_SetupActions(void)
{
    for (int32 i = 0; i < 64; ++i) ReplayRecorder->actions[i] = StateMachine_None;

    ReplayRecorder->actions[3] = Current_PlayerState_Down;
    ReplayRecorder->actions[4] = Current_PlayerState_Left;
    ReplayRecorder->actions[5] = Current_PlayerState_Right;
    ReplayRecorder->actions[6] = Current_PlayerState_Up;

    ReplayRecorder->actions[7]  = Cylinder_PlayerState_InkRoller_Stand;
    ReplayRecorder->actions[8]  = Cylinder_PlayerState_InkRoller_Roll;
    ReplayRecorder->actions[9]  = Cylinder_PlayerState_Pillar;
    ReplayRecorder->actions[10] = Cylinder_PlayerState_Spiral;

    ReplayRecorder->actions[13] = GymBar_PlayerState_Hang;
    ReplayRecorder->actions[14] = GymBar_PlayerState_SwingV;
    ReplayRecorder->actions[15] = GymBar_PlayerState_SwingH;

    ReplayRecorder->actions[16] = Ice_PlayerState_Frozen;

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
    ReplayRecorder->actions[28] = Player_State_Death;
    ReplayRecorder->actions[29] = Player_State_MightyHammerDrop;
    ReplayRecorder->actions[30] = Player_State_DropDash;
    ReplayRecorder->actions[31] = Player_State_Drown;
    ReplayRecorder->actions[32] = Player_State_TailsFlight;
    ReplayRecorder->actions[33] = Player_State_FlyToPlayer;
    ReplayRecorder->actions[34] = Player_State_KnuxGlideDrop;
    ReplayRecorder->actions[35] = Player_State_KnuxGlideLeft;
    ReplayRecorder->actions[36] = Player_State_KnuxGlideRight;
    ReplayRecorder->actions[37] = Player_State_KnuxGlideSlide;
    ReplayRecorder->actions[38] = Player_State_Ground;
    ReplayRecorder->actions[39] = Player_State_HoldRespawn;
    ReplayRecorder->actions[40] = Player_State_Hurt;
    ReplayRecorder->actions[41] = Player_State_KnuxLedgePullUp;
    ReplayRecorder->actions[42] = Player_State_LookUp;
    ReplayRecorder->actions[43] = Player_State_OuttaHere;
    ReplayRecorder->actions[44] = Player_State_Peelout;
    ReplayRecorder->actions[45] = Player_State_Roll;
    ReplayRecorder->actions[46] = Player_State_Spindash;
    ReplayRecorder->actions[47] = Player_State_StartSuper;
    ReplayRecorder->actions[48] = Player_State_Static;
    ReplayRecorder->actions[49] = Player_State_Transform;
    ReplayRecorder->actions[50] = Player_State_TransportTube;
    ReplayRecorder->actions[51] = Player_State_TubeAirRoll;
    ReplayRecorder->actions[52] = Player_State_TubeRoll;
    ReplayRecorder->actions[53] = Player_State_Victory;

    ReplayRecorder->actions[54] = SizeLaser_PlayerState_ShrinkChibi;
    ReplayRecorder->actions[55] = SizeLaser_PlayerState_GrowGiant;
    ReplayRecorder->actions[56] = SizeLaser_PlayerState_GrowNormal;
}

void ReplayRecorder_SetupWriteBuffer(void)
{
    EntityMenuParam *param = MenuParam_GetParam();
    Replay *replayPtr      = ReplayRecorder->recordBuffer;

    replayPtr->header.signature     = REPLAY_SIGNATURE;
    replayPtr->header.version       = GAME_VERSION;
    replayPtr->header.isPacked      = false;
    replayPtr->header.isNotEmpty    = true;
    replayPtr->header.startingFrame = ReplayRecorder->frameCounter;
    replayPtr->header.zoneID        = param->zoneID;
    replayPtr->header.act           = param->actID;
    replayPtr->header.characterID   = param->characterID;
    replayPtr->header.isPlusLayout  = SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE); // isEncore
    replayPtr->header.oscillation   = Zone->timer;
    replayPtr->header.bufferSize    = sizeof(ReplayHeader);

    LogHelpers_Print("characterID = %d", replayPtr->header.characterID);
    LogHelpers_Print("zoneID = %d", replayPtr->header.zoneID);
    LogHelpers_Print("act = %d", replayPtr->header.act);
    LogHelpers_Print("isPlusLayout = %d", replayPtr->header.isPlusLayout);
    LogHelpers_Print("oscillation = %d", replayPtr->header.oscillation);
}

void ReplayRecorder_DrawGhostDisplay(void)
{
    RSDK_THIS(ReplayRecorder);

    EntityPlayer *player = self->player;

    if (!self->state) {
        if (self->ghostAlpha > 0)
            self->ghostAlpha -= 4;
    }

    if (!SceneInfo->currentScreenID && self->ghostAlpha) {
        self->inkEffect = INK_NONE;
        int32 screenX   = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
        int32 screenY   = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;

        Hitbox hitbox;
        hitbox.left   = -(ScreenInfo->size.x >> 1);
        hitbox.top    = -(ScreenInfo->size.y >> 1);
        hitbox.right  = ScreenInfo->size.x >> 1;
        hitbox.bottom = ScreenInfo->size.y >> 1;

        if (!MathHelpers_PointInHitbox(screenX, screenY, player->position.x, player->position.y, FLIP_NONE, &hitbox)) {
            // Draw Player Preview (when ghost is off screen)
            Vector2 drawPos;
            drawPos.x = 0;
            drawPos.y = 0;

            int32 dist = CLAMP(MathHelpers_Distance(screenX, screenY, player->position.x, player->position.y) >> 16, 100, 2000);

            int32 size    = 12 - 4 * (3 * dist - 300) / 2000;
            hitbox.right  = hitbox.right - 8 - (size + 24);
            hitbox.bottom = hitbox.bottom - 8 - (size + 24);
            hitbox.top += size + 24 + 8;
            hitbox.left += size + 24 + 8;

            Vector2 screenPos;
            screenPos.x = screenX;
            screenPos.y = screenY;
            if (MathHelpers_ConstrainToBox(&drawPos, player->position.x, player->position.y, screenPos, hitbox)) {
                int32 angle = RSDK.ATan2(player->position.x - drawPos.x, player->position.y - drawPos.y);
                int32 x     = ((size + 18) * RSDK.Cos256(angle) << 8) + drawPos.x;
                int32 y     = ((size + 18) * RSDK.Sin256(angle) << 8) + drawPos.y;
                // Draw Direction Arrow
                DrawHelpers_DrawIsocelesTriangle(x, y, (RSDK.Cos256(angle) << 10) + x, (RSDK.Sin256(angle) << 10) + y, 4, 0xC0E0E0, INK_ALPHA,
                                                 self->ghostAlpha >> 1);

                self->alpha     = self->ghostAlpha;
                self->inkEffect = INK_ALPHA;
                self->drawFX    = FX_FLIP | FX_ROTATE | FX_SCALE;
                self->scale.x   = 0x100;
                self->scale.y   = 0x100;
                if (player->tailFrames != (uint16)-1) {
                    // Draw Tail Preview
                    self->rotation   = player->tailRotation;
                    self->direction  = player->tailDirection;
                    self->velocity.x = player->velocity.x;
                    self->velocity.y = player->velocity.y;
                    RSDK.DrawSprite(&player->tailAnimator, &drawPos, false);
                }

                // Draw Player Preview
                self->rotation  = player->rotation;
                self->direction = player->direction;
                RSDK.DrawSprite(&player->animator, &drawPos, false);

                self->drawFX     = FX_NONE;
                self->alpha      = 0xFF;
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

    recorder->prevPlayerState = player->state;
    recorder->state           = ReplayRecorder_State_Record;
    recorder->stateLate       = ReplayRecorder_Late_RecordFrames;
    recorder->storedAnim      = player->animator.animationID;
    recorder->storedFrame     = player->animator.frameID;
}

void ReplayRecorder_StartRecording(EntityPlayer *player)
{
    EntityReplayRecorder *recorder = ReplayRecorder->recordingManager;
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
    EntityReplayRecorder *recorder = ReplayRecorder->playbackManager;

    Replay *replayPtr = NULL;
    if (RSDK.GetEntitySlot(recorder) == SLOT_REPLAYRECORDER_RECORD)
        replayPtr = ReplayRecorder->recordBuffer;
    else
        replayPtr = ReplayRecorder->playbackBuffer;

    if (replayPtr->header.isNotEmpty) {
        recorder->active = ACTIVE_NORMAL;
        if (player) {
            recorder->player           = player;
            recorder->ghostPlayerState = player->state;
        }

        recorder->isGhostPlayback = false;
        if (player->playerID) {
            recorder->isGhostPlayback       = true;
            ReplayRecorder->hasSetupGhostVS = true;
        }
        else {
            Zone->timer              = replayPtr->header.oscillation;
            player->stateInputReplay = ReplayRecorder_PlayBackInput;
            player->controllerID     = CONT_P2;
            API_AssignInputSlotToDevice(CONT_P2, INPUT_UNASSIGNED);
            ReplayRecorder->hasSetupGhostView = true;
        }

        recorder->state     = ReplayRecorder_State_SetupPlayback;
        recorder->stateLate = StateMachine_None;
        // Set the dim timer to dim after 15 mins instead of the usual 5
        RSDK.SetVideoSetting(VIDEOSETTING_DIMTIMER, 15 * 60 * 60);
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
    if (RSDK.GetEntitySlot(recorder) == SLOT_REPLAYRECORDER_RECORD)
        frameBuffer = ReplayRecorder->recordingFrames;
    else
        frameBuffer = ReplayRecorder->playbackFrames;

    int32 newFrame = frame;

    ReplayFrame *framePtr = &frameBuffer[frame];
    while (framePtr->info != REPLAY_INFO_STATECHANGE) {
        if (framePtr->info == REPLAY_INFO_PASSEDGATE)
            break;
        if (newFrame > (int32)frame)
            break;
        framePtr--;
        --newFrame;
    }

    if (newFrame <= (int32)frame) {
        ReplayRecorder_ForceApplyFramePtr(recorder, framePtr);
        if (newFrame < (int32)frame) {
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
    if (RSDK.GetEntitySlot(recorder) == SLOT_REPLAYRECORDER_RECORD)
        framePtr = ReplayRecorder->recordingFrames;
    else
        framePtr = ReplayRecorder->playbackFrames;

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

    recorder->state     = StateMachine_None;
    recorder->stateLate = StateMachine_None;

    EntityPlayer *player = recorder->player;
    if (player) {
        if (player->stateInputReplay == ReplayRecorder_PlayBackInput)
            player->stateInputReplay = StateMachine_None;
    }
}

void ReplayRecorder_SetGimmickState(EntityReplayRecorder *recorder, bool32 allowSpriteChanges)
{
    EntityPlayer *player = recorder->player;
    if (player) {
        player->tailFrames = -1;

        if (RSDK.CheckSceneFolder("MMZ") || RSDK.CheckSceneFolder("PSZ2")) {
            if (allowSpriteChanges) {
                if (RSDK.CheckSceneFolder("MMZ")) {
                    switch (player->characterID) {
                        default:
                        case ID_SONIC: player->aniFrames = SizeLaser->sonicFrames; break;

                        case ID_TAILS:
                            player->aniFrames  = SizeLaser->tailsFrames;
                            player->tailFrames = SizeLaser->tailFrames;
                            break;

                        case ID_KNUCKLES: player->aniFrames = SizeLaser->knuxFrames; break;
                        case ID_MIGHTY: player->aniFrames = SizeLaser->mightyFrames; break;
                        case ID_RAY: player->aniFrames = SizeLaser->rayFrames; break;
                    }
                }
                else if (RSDK.CheckSceneFolder("PSZ2")) {
                    player->aniFrames = Ice->aniFrames;
                }
            }
            else {
                switch (player->characterID) {
                    default:
                    case ID_SONIC: player->aniFrames = Player->sonicFrames; break;

                    case ID_TAILS:
                        player->aniFrames  = Player->tailsFrames;
                        player->tailFrames = Player->tailsTailsFrames;
                        break;

                    case ID_KNUCKLES: player->aniFrames = Player->knuxFrames; break;
                    case ID_MIGHTY: player->aniFrames = Player->mightyFrames; break;
                    case ID_RAY: player->aniFrames = Player->rayFrames; break;
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
        player->direction  = framePtr->direction;
        player->rotation   = framePtr->rotation;

        ReplayRecorder_SetGimmickState(recorder, (framePtr->changedValues & REPLAY_CHANGED_GIMMICK) != 0);
        RSDK.SetSpriteAnimation(player->aniFrames, framePtr->anim, &player->animator, true, framePtr->frame);
        player->animator.speed = 0;
    }
}

void ReplayRecorder_ApplyFramePtr(EntityReplayRecorder *recorder, ReplayFrame *framePtr)
{
    EntityPlayer *player = recorder->player;
    if (player) {
        if (framePtr->changedValues & REPLAY_CHANGED_POS) {
            player->position.x = framePtr->position.x & 0xFFFF0000;
            player->position.y = framePtr->position.y & 0xFFFF0000;
        }

        if (framePtr->changedValues & REPLAY_CHANGED_VEL) {
            player->velocity.x = framePtr->velocity.y & 0xFFFF0000;
            player->velocity.y = framePtr->velocity.y & 0xFFFF0000;
        }

        if (framePtr->changedValues & REPLAY_CHANGED_ROT)
            player->rotation = framePtr->rotation;

        if (framePtr->changedValues & REPLAY_CHANGED_DIR)
            player->direction = framePtr->direction;

        ReplayRecorder_SetGimmickState(recorder, (framePtr->changedValues & REPLAY_CHANGED_GIMMICK) != 0);

        if (framePtr->changedValues & REPLAY_CHANGED_ANIM)
            RSDK.SetSpriteAnimation(player->aniFrames, framePtr->anim, &player->animator, true, framePtr->frame);
        else if (framePtr->changedValues & REPLAY_CHANGED_FRAME)
            RSDK.SetSpriteAnimation(player->aniFrames, player->animator.animationID, &player->animator, true, framePtr->frame);

        player->animator.speed = 0;
    }
}

bool32 ReplayRecorder_CheckPlayerGimmickState(EntityReplayRecorder *recorder)
{
    EntityPlayer *player = recorder->player;

    if (!player || (!RSDK.CheckSceneFolder("MMZ") && !RSDK.CheckSceneFolder("PSZ2")))
        return false;

    if (RSDK.CheckSceneFolder("MMZ"))
        return player->isChibi;

    if (RSDK.CheckSceneFolder("PSZ2"))
        return player->state == Ice_PlayerState_Frozen;

    return false;
}

void ReplayRecorder_PackFrame(ReplayFrame *recording)
{
    RSDK_THIS(ReplayRecorder);

    uint8 buffer[sizeof(ReplayFrame)];
    memset(&buffer, 0, sizeof(ReplayFrame));

    Replay *replayPtr        = ReplayRecorder->recordBuffer;
    ReplayFrame *frameBuffer = ReplayRecorder->recordingFrames;

    int32 size = ReplayDB_Buffer_PackEntry(buffer, recording);
    memcpy(&frameBuffer[self->replayFrame], recording, sizeof(ReplayFrame));

    if (replayPtr->header.frameCount) {
        uint32 frameCount                  = replayPtr->header.frameCount;
        float avg                          = replayPtr->header.averageFrameSize;
        float sizef                        = size;
        replayPtr->header.averageFrameSize = ((avg * frameCount) + sizef) / (frameCount + 1);
    }
    else {
        replayPtr->header.averageFrameSize = size;
    }

    replayPtr->header.bufferSize += size;

    ++self->replayFrame;
    ++replayPtr->header.frameCount;
}

void ReplayRecorder_PlayBackInput(void)
{
    RSDK_THIS(Player);

    EntityReplayRecorder *recorder = ReplayRecorder->playbackManager;

    Replay *replayPtr = NULL;
    if (RSDK.GetEntitySlot(recorder) == SLOT_REPLAYRECORDER_RECORD)
        replayPtr = ReplayRecorder->recordBuffer;
    else
        replayPtr = ReplayRecorder->playbackBuffer;

    if (ReplayRecorder->frameCounter >= replayPtr->header.startingFrame && self == recorder->player) {
        ReplayFrame *frameBuffer = NULL;
        if (RSDK.GetEntitySlot(recorder) == SLOT_REPLAYRECORDER_RECORD)
            frameBuffer = ReplayRecorder->recordingFrames;
        else
            frameBuffer = ReplayRecorder->playbackFrames;
        ReplayFrame *framePtr = &frameBuffer[recorder->replayFrame];

        bool32 setPos = false;
        if (framePtr->info) {
            bool32 forceChange = framePtr->info == REPLAY_INFO_STATECHANGE || framePtr->info == REPLAY_INFO_PASSEDGATE;

            setPos          = forceChange || (framePtr->info == REPLAY_INFO_USEFLAGS && (framePtr->changedValues & REPLAY_CHANGED_POS));
            bool32 setInput = forceChange || (framePtr->info == REPLAY_INFO_USEFLAGS && (framePtr->changedValues & REPLAY_CHANGED_INPUT));
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

void ReplayRecorder_PlayerState_PlaybackReplay(void)
{
    RSDK_THIS(Player);

    EntityReplayRecorder *recorder = ReplayRecorder->playbackManager;
    if (recorder->isGhostPlayback) {
        self->animator.speed = 0;

        ReplayFrame *frameBuffer = NULL;
        if (RSDK.GetEntitySlot(recorder) == SLOT_REPLAYRECORDER_RECORD)
            frameBuffer = ReplayRecorder->recordingFrames;
        else
            frameBuffer = ReplayRecorder->playbackFrames;

        ReplayFrame *framePtr = &frameBuffer[recorder->replayFrame];
        if (recorder->state) {
            if (framePtr->info == REPLAY_INFO_USEFLAGS) {
                ReplayRecorder_ApplyFramePtr(recorder, framePtr);
            }
            else if (framePtr->info == REPLAY_INFO_STATECHANGE || framePtr->info == REPLAY_INFO_PASSEDGATE) {
                ReplayRecorder_ForceApplyFramePtr(recorder, framePtr);
                if (framePtr->info == REPLAY_INFO_PASSEDGATE && !ReplayRecorder->packedStartFrame) {
                    ReplayRecorder->packedStartFrame = true;
                    if (!ReplayRecorder->passedStartLine)
                        ReplayRecorder_Pause(recorder);
                }
            }
        }
        else {
            self->velocity.x = 0;
            self->velocity.y = 0;
        }

        if (FarPlane) {
            int32 screenX = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
            int32 screenY = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;

            self->scale.x   = 0x200;
            self->scale.y   = 0x200;
            self->drawGroup = Zone->playerDrawGroup[0];
            foreach_all(FarPlane, farPlane)
            {
                Hitbox hitbox;
                hitbox.left   = -(farPlane->size.x >> 16);
                hitbox.top    = -(farPlane->size.y >> 16);
                hitbox.right  = (farPlane->size.x >> 16);
                hitbox.bottom = (farPlane->size.y >> 16);

                if (MathHelpers_PointInHitbox(farPlane->origin.x, farPlane->origin.y, self->position.x, self->position.y, FLIP_NONE, &hitbox)) {
                    self->position.x =
                        (farPlane->position.x - ((farPlane->position.x - screenX) >> 1) + 0x8000) + ((self->position.x - farPlane->origin.x) >> 1);
                    self->position.y =
                        (farPlane->position.y - ((farPlane->position.y - screenY) >> 1) + 0x8000) + ((self->position.y - farPlane->origin.y) >> 1);
                    self->drawFX |= FX_SCALE;
                    self->scale.x   = 0x100;
                    self->scale.y   = 0x100;
                    self->drawGroup = Zone->objectDrawGroup[0];
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

void ReplayRecorder_State_SetupPlayback(void)
{
    RSDK_THIS(ReplayRecorder);

    Replay *replayPtr = NULL;
    if (RSDK.GetEntitySlot(self) == SLOT_REPLAYRECORDER_RECORD)
        replayPtr = ReplayRecorder->recordBuffer;
    else
        replayPtr = ReplayRecorder->playbackBuffer;

    ReplayFrame *frameBuffer = NULL;
    if (RSDK.GetEntitySlot(self) == SLOT_REPLAYRECORDER_RECORD)
        frameBuffer = ReplayRecorder->recordingFrames;
    else
        frameBuffer = ReplayRecorder->playbackFrames;

    if (ReplayRecorder->frameCounter >= replayPtr->header.startingFrame) {
        if (ReplayRecorder->frameCounter != replayPtr->header.startingFrame) {
            if (!self->isGhostPlayback)
                ReplayRecorder_Seek(self, ReplayRecorder->frameCounter - replayPtr->header.startingFrame);
            self->replayFrame = ReplayRecorder->frameCounter - replayPtr->header.startingFrame;
        }

        self->state     = ReplayRecorder_State_Playback;
        self->stateLate = ReplayRecorder_Late_Playback;
    }
    else if (self->isGhostPlayback) {
        ReplayRecorder_ForceApplyFramePtr(self, frameBuffer);
    }
}

void ReplayRecorder_State_Playback(void) {}

void ReplayRecorder_State_Record(void) {}

void ReplayRecorder_Late_Playback(void)
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

            camera->state       = Camera_State_FollowXY;
            player->scrollDelay = 0;
        }
    }

    if (self->isGhostPlayback) {
        if (player->state != ReplayRecorder_PlayerState_PlaybackReplay)
            self->ghostPlayerState = player->state;
    }

    ReplayFrame *frameBuffer = NULL;
    if (RSDK.GetEntitySlot(self) == SLOT_REPLAYRECORDER_RECORD)
        frameBuffer = ReplayRecorder->recordingFrames;
    else
        frameBuffer = ReplayRecorder->playbackFrames;

    ReplayFrame *framePtr = &frameBuffer[self->replayFrame];

    if (!self->isGhostPlayback && framePtr->info) {
        if ((framePtr->info == REPLAY_INFO_STATECHANGE || framePtr->info == REPLAY_INFO_PASSEDGATE)
            || (framePtr->info == REPLAY_INFO_USEFLAGS && (framePtr->changedValues & REPLAY_CHANGED_POS))) {
            if (MathHelpers_Distance(framePtr->position.x, framePtr->position.y, player->position.x, player->position.y) >= 0x20000) {
                player->position.x += (framePtr->position.x - player->position.x) >> 1;
                player->position.y += (framePtr->position.y - player->position.y) >> 1;
            }
        }
    }

    ++self->replayFrame;
}

void ReplayRecorder_Late_RecordFrames(void)
{
    ReplayFrame frame;

    RSDK_THIS(ReplayRecorder);
    if (self->replayFrame < self->maxFrameCount - 1) {
        EntityPlayer *player = self->player;
        memset(&frame, 0, sizeof(frame));

        RSDKControllerState *controller = &ControllerInfo[player->controllerID];
        int32 inputState                = 0;
        if (controller->keyUp.down || AnalogStickInfoL[player->controllerID].keyUp.down)
            inputState |= 0x01;
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
        self->curPlayerState  = player->state;

        EntityIce *ice = player->abilityPtrs[1];

        Animator *animator = &player->animator;
        if (isGimmickState && RSDK.CheckSceneFolder("PSZ2") && player->state == Ice_PlayerState_Frozen && ice->classID == Ice->classID)
            animator = &ice->contentsAnimator;

        self->animID  = animator->animationID;
        self->frameID = animator->frameID;

        // Not sure what this is for, since it doesn't do anything
        for (int32 i = 0; i < 64; ++i) {
            if (self->prevPlayerState == ReplayRecorder->actions[i])
                break;
        }

        for (int32 i = 0; i < 64; ++i) {
            if (self->curPlayerState == ReplayRecorder->actions[i])
                break;
        }

        // byte 0 info:
        // 0 = none
        // 1 = state change (first frame is always a state change)
        // 2 = some values changed (see byte 1 for info)
        // 3 = passed TA Gate

        // byte 1 info:
        // its flags for what values changed

        if (self->replayFrame == 0 || self->changeFlags > 0 || self->prevPlayerState != self->curPlayerState) {
            frame.info          = self->changeFlags == 2 ? REPLAY_INFO_PASSEDGATE : REPLAY_INFO_STATECHANGE;
            frame.changedValues = REPLAY_CHANGED_NONE;
            frame.inputs        = inputState;
            frame.direction     = player->direction;
            frame.rotation      = player->rotation;
            frame.position.x    = player->position.x;
            frame.position.y    = player->position.y;
            frame.velocity.x    = player->velocity.x;
            frame.velocity.y    = player->velocity.y;
            frame.anim          = self->animID;
            frame.frame         = self->frameID;
            if (isGimmickState)
                frame.changedValues = REPLAY_CHANGED_GIMMICK;

            self->changeFlags = 0;
        }
        else {
            int32 changedVals = 0;
            if (inputState != self->storedInputs) {
                frame.inputs = inputState;
                changedVals  = REPLAY_CHANGED_INPUT;
            }

            if (player->direction != self->storedDirection) {
                frame.direction = player->direction;
                changedVals |= REPLAY_CHANGED_DIR;
            }

            if (player->position.x != self->storedPos.x || player->position.y != self->storedPos.y) {
                changedVals |= REPLAY_CHANGED_POS;
                frame.position.x = player->position.x;
                frame.position.y = player->position.y;
            }

            if (player->velocity.x != self->storedVel.x || player->velocity.y != self->storedVel.y) {
                changedVals |= REPLAY_CHANGED_VEL;
                frame.velocity.x = player->velocity.x;
                frame.velocity.y = player->velocity.y;
            }

            // what ??? ??? ???
            // this is code from mania
            if (player->groundVel != self->storedRotation) {
                changedVals |= REPLAY_CHANGED_ROT;
                frame.rotation = player->rotation;
            }

            if (animator->animationID != self->storedAnim) {
                changedVals |= (REPLAY_CHANGED_ANIM | REPLAY_CHANGED_FRAME);
                frame.anim  = self->animID;
                frame.frame = self->frameID;
            }

            if (animator->frameID != self->storedFrame) {
                changedVals |= REPLAY_CHANGED_FRAME;
                frame.frame = self->frameID;
            }

            if (isGimmickState)
                changedVals |= REPLAY_CHANGED_GIMMICK;

            if (changedVals) {
                frame.info          = REPLAY_INFO_USEFLAGS;
                frame.changedValues = changedVals;
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

#if RETRO_INCLUDE_EDITOR
void ReplayRecorder_EditorDraw(void) {}

void ReplayRecorder_EditorLoad(void) {}
#endif

void ReplayRecorder_Serialize(void) {}
#endif
