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

        if (ReplayRecorder->dword144 && !self->player->playerID && self->state == ReplayRecorder_None_Record) {
            if (self->field_88 <= 0) {
                if (!self->field_88) {
                    ReplayRecorder_Stop(self);
                    ReplayRecorder_Rewind(self);
                    self->field_88 = -1;
                }
            }
            else {
                self->field_88--;
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
                recorder1->storedAnim  = playerR1->animator.animationID;
                recorder1->storedFrame = playerR1->animator.frameID;
            }

            EntityReplayRecorder *recorder2 = (EntityReplayRecorder *)ReplayRecorder->recorder_w;
            EntityPlayer *playerR2          = recorder2->player;
            if (recorder2->playing && playerR2)
                playerR2->state = ReplayRecorder_PlayerState;

            if ((ControllerInfo->keyStart.press || UnknownInfo->field_10) && SceneInfo->state == ENGINESTATE_REGULAR) {
                EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                bool32 flag                = true;
                if (ActClear && ActClear->actClearActive)
                    flag = false;
                if (!RSDK.GetEntityCount(TitleCard->objectID, 0) && !pauseMenu->objectID && flag) {
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
    self->active     = ACTIVE_NEVER;
    self->drawOrder  = 13;
    self->inkEffect  = INK_NONE;
    self->field_88   = -1;
    self->drawFX     = FX_FLIP;
    self->alphaStore = 256;
    self->visible    = globals->gameMode == MODE_TIMEATTACK;
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
        memset(ReplayRecorder->filename, 0, sizeof(ReplayRecorder->filename));
        ReplayRecorder->writeBuffer   = globals->replayWriteBuffer;
        ReplayRecorder->frameBuffer_w = &globals->replayWriteBuffer[REPLAY_HDR_SIZE];
        ReplayRecorder->readBuffer    = globals->replayReadBuffer;
        ReplayRecorder->frameBuffer_r = &globals->replayReadBuffer[REPLAY_HDR_SIZE];
        if (Zone) {
            if (!SceneInfo->inEditor)
                ReplayRecorder_SetupActions();
            RSDK.SetRandSeed(1624633040);

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
            int32 *buffer            = NULL;
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
        ReplayRecorder->dword124 = true;
    if (buffer[REPLAY_HDR_PACKED])
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
    int32 *buffer = NULL;
    if (RSDK.GetEntityID(ReplayRecorder->recorder_w) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;
    if (buffer[REPLAY_HDR_PACKED]) {
        if (buffer[REPLAY_HDR_FRAMECOUNT2] < (((EntityReplayRecorder *)ReplayRecorder->recorder_r)->field_84 - 1)) {
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
                                             SceneInfo->filter == SCN_FILTER_ENCORE);
    if (rowID == -1) {
        LogHelpers_Print("Table row ID invalid! %d", -1);
        ReplayRecorder_SavedReplay(false);
    }
    else {
        uint32 uuid                 = API.GetUserDBRowUUID(globals->replayTableID, rowID);
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
        ReplayRecorder_SaveReplayDB(ReplayRecorder_WaitWhileReplaySaves);
    }
    else {
        if (ReplayRecorder->replayRowID != -1)
            API.RemoveDBRow(globals->replayTableID, ReplayRecorder->replayRowID);
        TextInfo message;
        INIT_TEXTINFO(message);
        Localization_GetString(&message, STR_NOREPLAYSPACE);
        UIDialog_CreateDialogOk(&message, NULL, true);
        UIWaitSpinner_Wait2();
        ActClear->disableResultsInput = false;
        ActClear->hasSavedReplay      = false;
        HUD->replaySaveEnabled        = true;
    }
}

void ReplayRecorder_WaitWhileReplaySaves(bool32 flag)
{
    if (flag) {
        if (TimeAttackData->rowID == -1) {
            UIWaitSpinner_Wait2();
            ActClear->disableResultsInput = false;
            foreach_all(HUD, hud)
            {
                RSDK.SetSpriteAnimation(HUD->aniFrames, 10, &hud->taAnimator2, true, 0);
                foreach_break;
            }
            RSDK.PlaySfx(HUD->sfxStarpost, 0, 255);
            HUD->replaySaveEnabled = 0;
        }
        else {
            API.SetUserDBValue(globals->taTableID, TimeAttackData->rowID, 4, "replayID", &ReplayRecorder->replayID);
            TimeAttackData_SaveTimeAttackDB(ReplayRecorder_Unknown10);
        }
    }
    else {
        TextInfo message;
        INIT_TEXTINFO(message);
        char fileName[0x20];
        sprintf(fileName, "Replay_%08X.bin", ReplayRecorder->replayID);
        if (ReplayRecorder->replayRowID != -1)
            API.RemoveDBRow(globals->replayTableID, ReplayRecorder->replayRowID);
        API.DeleteUserFile(fileName, 0);
        Localization_GetString(&message, STR_NOREPLAYSPACE);
        UIDialog_CreateDialogOk(&message, NULL, true);
        UIWaitSpinner_Wait2();
        ActClear->disableResultsInput = false;
        ActClear->hasSavedReplay      = false;
        HUD->replaySaveEnabled        = true;
    }
}

void ReplayRecorder_Unknown10(int32 status)
{
    UIWaitSpinner_Wait2();
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
        RSDK.PlaySfx(HUD->sfxStarpost, 0, 255);
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
            int32 uncompressedSize = 28 * (tempWriteBuffer[REPLAY_HDR_FRAMECOUNT2] + 2);

            uint8 *framePtr       = (uint8 *)&tempWriteBuffer[REPLAY_HDR_SIZE];
            uint8 *compressedData = (uint8 *)&tempWriteBuffer[REPLAY_HDR_SIZE];
            for (int32 f = 0; f < tempWriteBuffer[REPLAY_HDR_FRAMECOUNT2]; ++f) {
                uint8 uncompressedData[28];
                memcpy(uncompressedData, framePtr, 28 * sizeof(uint8));
                int32 *dataPtr = (int32 *)uncompressedData;

                // set velocity to zero
                dataPtr[2] = 0;
                dataPtr[3] = 0;

                int32 size = ReplayDB_Buffer_PackEntry(compressedData, uncompressedData);
                framePtr += 28;
                compressedData += size;
                compSize += size;
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

    uint8 *compressedData = (uint8 *)&tempReadBuffer[14];
    if (*tempReadBuffer == Replay_Signature) {
        if (tempReadBuffer[REPLAY_HDR_PACKED2]) {
            int32 compSize                     = tempReadBuffer[REPLAY_HDR_COMPSIZE];
            readBuffer[REPLAY_HDR_SIG]         = tempReadBuffer[REPLAY_HDR_SIG];
            readBuffer[REPLAY_HDR_VER]         = tempReadBuffer[REPLAY_HDR_VER];
            readBuffer[REPLAY_HDR_PACKED2]     = tempReadBuffer[REPLAY_HDR_PACKED2];
            readBuffer[REPLAY_HDR_PACKED]      = tempReadBuffer[REPLAY_HDR_PACKED];
            readBuffer[REPLAY_HDR_FRAMECOUNT2] = tempReadBuffer[REPLAY_HDR_FRAMECOUNT2];
            readBuffer[REPLAY_HDR_FRAMECOUNT]  = tempReadBuffer[REPLAY_HDR_FRAMECOUNT];
            int32 frameSize                    = 28 * (tempReadBuffer[REPLAY_HDR_FRAMECOUNT2] + 2);
            uint8 *uncompressedBuffer          = (uint8 *)&readBuffer[REPLAY_HDR_SIZE];
            for (int32 i = 0; i < tempReadBuffer[REPLAY_HDR_FRAMECOUNT2]; ++i) {
                int32 size = ReplayDB_Buffer_UnpackEntry(uncompressedBuffer, compressedData);
                compressedData += size;
                uncompressedBuffer += 28;
            }
            LogHelpers_Print("Unpacked %d frames: %luB -> %luB", tempReadBuffer[REPLAY_HDR_FRAMECOUNT2], compSize, frameSize);
            readBuffer[REPLAY_HDR_PACKED2]  = false;
            readBuffer[REPLAY_HDR_COMPSIZE] = frameSize;
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
    ReplayRecorder->actions[3]  = Current_Player_State_Type3;
    ReplayRecorder->actions[4]  = Current_Player_State_Type0;
    ReplayRecorder->actions[5]  = Current_Player_State_Type1;
    ReplayRecorder->actions[6]  = Current_Player_State_Type2;
    ReplayRecorder->actions[7]  = Cylinder_Player_State_Unknown2;
    ReplayRecorder->actions[8]  = Cylinder_Player_State_Unknown3;
    ReplayRecorder->actions[9]  = Cylinder_Player_State_Unknown4;
    ReplayRecorder->actions[10] = Cylinder_Player_State_Unknown1;
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
    buffer[REPLAY_HDR_FRAMECOUNT]   = ReplayRecorder->frameCounter;                // frameCount
    buffer[REPLAY_HDR_ZONEID]       = param->zoneID;                               // zoneID
    buffer[REPLAY_HDR_ACTID]        = param->actID;                                // actID
    buffer[REPLAY_HDR_CHARID]       = param->characterID;                          // characterID
    buffer[REPLAY_HDR_ISPLUSLAYOUT] = SceneInfo->filter == SCN_FILTER_ENCORE; // isEncore
    buffer[REPLAY_HDR_OSC]          = Zone->timer;                                 // oscillation
    buffer[REPLAY_HDR_COMPSIZE]     = REPLAY_HDR_SIZE * sizeof(int);               // header size
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
        int32 screenX     = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
        int32 screenY     = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;

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
            if (MathHelpers_Unknown14(&drawPos, player->position.x, player->position.y, screenPos, hitbox)) {
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
            recorder->player   = player;
            recorder->playerState = player->state;
        }
        recorder->playing = false;
        if (player->playerID) {
            recorder->playing        = true;
            ReplayRecorder->dword138 = true;
        }
        else {
            Zone->timer              = buffer[REPLAY_HDR_OSC];
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

void ReplayRecorder_Seek(EntityReplayRecorder *recorder, uint32 frame)
{
    LogHelpers_Print("ReplayRecorder_Seek(%u)", frame);
    recorder->replayFrame = frame;

    int32 *frameBuffer = NULL;
    if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
        frameBuffer = ReplayRecorder->frameBuffer_w;
    else
        frameBuffer = ReplayRecorder->frameBuffer_r;
    uint8 *byteBuf = (uint8 *)frameBuffer;
    int32 newFrame = frame;

    uint8 *framePtr = &byteBuf[28 * frame];
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
            int32 count = frame - newFrame;
            uint8 *ptr  = &byteBuf[28 * newFrame];
            for (int32 i = 0; i < count; ++i) {
                ptr += 28;
                ReplayRecorder_Unknown19(recorder, ptr);
            }
        }
    }
}

void ReplayRecorder_SeekFunc(EntityReplayRecorder *recorder)
{
    int32 *buffer2 = NULL;
    if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
        buffer2 = ReplayRecorder->frameBuffer_w;
    else
        buffer2 = ReplayRecorder->frameBuffer_r;
    uint8 *byteBuf = (uint8 *)buffer2;

    for (int32 f = 0; f < recorder->field_84; ++f) {
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
                            player->aniFrames     = SizeLaser->tailsIndex;
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
                        player->aniFrames     = Player->tailsSpriteIndex;
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

void ReplayRecorder_ApplyFrameData(EntityReplayRecorder *recorder, uint8 *buffer)
{
    EntityPlayer *player = recorder->player;
    int32 *bufferPtr     = (int32 *)buffer;
    if (player) {
        player->position.x = bufferPtr[1] & 0xFFFF0000;
        player->position.y = bufferPtr[2] & 0xFFFF0000;
        player->velocity.x = bufferPtr[3] & 0xFFFF0000;
        player->velocity.y = bufferPtr[4] & 0xFFFF0000;
        player->direction  = buffer[3];
        player->rotation   = bufferPtr[5];
        ReplayRecorder_SetGimmickState(recorder, ((buffer[1] & 8) > 0));
        RSDK.SetSpriteAnimation(player->aniFrames, buffer[24], &player->animator, true, buffer[25]);
        player->animator.animationSpeed = 0;
    }
}

void ReplayRecorder_Unknown19(EntityReplayRecorder *recorder, uint8 *buffer)
{
    EntityPlayer *player = recorder->player;
    int32 *bufferPtr     = (int32 *)buffer;
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
            RSDK.SetSpriteAnimation(player->aniFrames, buffer[24], &player->animator, true, buffer[25]);
        }
        else if ((buffer[1] & 0x80)) {
            RSDK.SetSpriteAnimation(player->aniFrames, player->animator.animationID, &player->animator, true, buffer[25]);
        }
        player->animator.animationSpeed = 0;
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

void ReplayRecorder_PackFrame(uint8 *recording)
{
    RSDK_THIS(ReplayRecorder);

    uint8 frameBuf[28];
    memset(frameBuf, 0, sizeof(frameBuf));
    frameBuf[0]         = 0;
    int32 *writeBuffer  = ReplayRecorder->writeBuffer;
    int32 *writeBuffer2 = ReplayRecorder->frameBuffer_w;
    int32 size          = ReplayDB_Buffer_PackEntry(frameBuf, recording);
    memcpy(&writeBuffer2[7 * self->replayFrame], recording, 0x28 * sizeof(uint8));

    if (writeBuffer[REPLAY_HDR_FRAMECOUNT2]) {
        writeBuffer[REPLAY_HDR_FRAMECOUNT2] =
            ((float)(writeBuffer[REPLAY_HDR_FRAMECOUNT2] * writeBuffer[REPLAY_HDR_FRAMECOUNT2]) + size) / (writeBuffer[REPLAY_HDR_FRAMECOUNT2] + 1);
    }
    else {
        writeBuffer[REPLAY_HDR_FRAMECOUNT2] = size;
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
        int32 *frameBuffer = NULL;
        if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
            frameBuffer = ReplayRecorder->frameBuffer_w;
        else
            frameBuffer = ReplayRecorder->frameBuffer_r;
        int32 *playbackBuf = &frameBuffer[7 * recorder->replayFrame];
        uint8 *bufBytes    = (uint8 *)playbackBuf;

        bool32 setPos = false;
        if (bufBytes[0]) {
            setPos = (bufBytes[0] == 1 || bufBytes[0] == 3) || (bufBytes[0] == 2 && (bufBytes[1] & 2));
            if ((bufBytes[0] == 1 || bufBytes[0] == 3) || (bufBytes[0] == 2 && (bufBytes[1] & 1))) {
                int32 inputs                                        = bufBytes[2];
                ControllerInfo[self->controllerID].keyUp.down    = (inputs & 0x01) > 0;
                ControllerInfo[self->controllerID].keyDown.down  = (inputs & 0x02) > 0;
                ControllerInfo[self->controllerID].keyLeft.down  = (inputs & 0x04) > 0;
                ControllerInfo[self->controllerID].keyRight.down = (inputs & 0x08) > 0;
                ControllerInfo[self->controllerID].keyA.press    = (inputs & 0x10) > 0;
                ControllerInfo[self->controllerID].keyA.down     = (inputs & 0x20) > 0;
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
        self->animator.animationSpeed = 0;

        int32 *buffer = NULL;
        if (RSDK.GetEntityID(recorder) == SLOT_REPLAYRECORDER_W)
            buffer = ReplayRecorder->frameBuffer_w;
        else
            buffer = ReplayRecorder->frameBuffer_r;

        uint8 *frameData = (uint8 *)&buffer[7 * recorder->replayFrame];
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

        if (ReplayRecorder->dword148 && !ReplayRecorder->dword140) {
            self->visible = ~(self->timer >> 2) & 1;
            self->timer++;
        }
    }
}

void ReplayRecorder_StatePlay(void)
{
    RSDK_THIS(ReplayRecorder);
    int32 *buffer      = NULL;
    int32 *frameBuffer = NULL;
    if (RSDK.GetEntityID(self) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;

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
            ReplayRecorder_ApplyFrameData(self, (uint8 *)frameBuffer);
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
            camera->position.x                        = player->position.x;
            camera->position.y                        = player->position.y;
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

    int32 *frameBuffer = NULL;
    if (RSDK.GetEntityID(self) == SLOT_REPLAYRECORDER_W)
        frameBuffer = ReplayRecorder->frameBuffer_w;
    else
        frameBuffer = ReplayRecorder->frameBuffer_r;
    int32 *framePtr = &frameBuffer[7 * self->replayFrame];
    uint8 *bufBytes = (uint8 *)framePtr;
    if (!self->playing) {
        if (bufBytes[0]) {
            if ((bufBytes[0] == 1 || bufBytes[0] == 3) || (bufBytes[0] == 2 && (bufBytes[1] & 2))) {
                int32 distX = abs(player->position.x - framePtr[1]);
                int32 distY = abs(player->position.y - framePtr[2]);
                if (MathHelpers_SquareRoot((distX >> 16) * (distX >> 16) + (distY >> 16) * (distY >> 16)) << 16 >= 0x20000) {
                    player->position.x += (framePtr[1] - player->position.x) >> 1;
                    player->position.y += (framePtr[2] - player->position.y) >> 1;
                }
            }
        }
    }
    ++self->replayFrame;
}

void ReplayRecorder_None_Record(void) {}

void ReplayRecorder_RecordFrameData(void)
{
    uint8 recording[28];
    int32 *recordingPtr = (int32 *)recording;

    RSDK_THIS(ReplayRecorder);
    if (self->replayFrame < self->field_84 - 1) {
        EntityPlayer *player = self->player;
        int32 inputState     = 0;
        memset(recording, 0, sizeof(recording));

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
        self->stateStore    = player->state;

        EntityIce *ice  = player->abilityPtrs[1];
        Animator *animator = &player->animator;
        if (isGimmickState && RSDK.CheckStageFolder("PSZ2") && player->state == Ice_State_FrozenPlayer && ice->objectID == Ice->objectID) {
            animator = &ice->animator2;
        }
        void *storedState = self->storedState;
        self->animID    = animator->animationID;
        self->frameID   = animator->frameID;

        for (int32 i = 0; i < 64; ++i) {
            if (storedState == ReplayRecorder->actions[i])
                break;
        }

        for (int32 i = 0; i < 64; ++i) {
            if (self->stateStore == ReplayRecorder->actions[i])
                break;
        }

        if (self->replayFrame == 0 || self->field_78 > 0 || self->storedState != self->stateStore) {
            self->field_78 = 0;
            recording[2]     = inputState;
            recording[1]     = 0;
            recording[0]     = 2 * (self->field_78 == 2) + 1;
            recording[3]     = player->direction;
            recordingPtr[5]  = player->rotation;
            recordingPtr[1]  = player->position.x;
            recordingPtr[2]  = player->position.y;
            recordingPtr[3]  = player->velocity.x;
            recordingPtr[4]  = player->velocity.y;
            recording[24]    = self->animID;
            recording[25]    = self->frameID;
            if (isGimmickState)
                recording[1] = 8;
        }
        else {
            int32 changedVals = 0;
            if (inputState != self->storedInputs) {
                recording[2] = inputState;
                changedVals  = 1;
            }
            if (player->direction != self->storedDirection) {
                recording[3] = player->direction;
                changedVals |= 0x10;
            }
            if (player->position.x != self->storedPos.x || player->position.y != self->storedPos.y) {
                changedVals |= 2;
                recordingPtr[1] = player->position.x;
                recordingPtr[2] = player->position.y;
            }
            if (player->velocity.x != self->storedVel.x || player->velocity.y != self->storedVel.y) {
                changedVals |= 4;
                recordingPtr[2] = player->velocity.x;
                recordingPtr[3] = player->velocity.y;
            }
            if (player->groundVel != self->storedRotation) {
                changedVals |= 0x20;
                recordingPtr[5] = player->rotation;
            }
            if (animator->animationID == self->storedAnim) {
                if (animator->frameID == self->storedFrame) {
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
                    return;
                }
                changedVals |= 0x80;
            }
            else {
                changedVals |= 0xC0;
                recording[24] = self->animID;
            }
            recording[25] = self->frameID;

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
    globals->replayTableID = API.InitUserDB("ReplayDB.bin", 4, "score", 2, "zoneID", 2, "act", 2, "characterID", 2, "encore", 4, "zoneSortVal", 0);

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
        API.SetUserDBValue(globals->replayTableID, rowID, 4, "score", &score);
        API.SetUserDBValue(globals->replayTableID, rowID, 2, "zoneID", &zone);
        API.SetUserDBValue(globals->replayTableID, rowID, 2, "act", &actID);
        API.SetUserDBValue(globals->replayTableID, rowID, 2, "characterID", &charID);
        API.SetUserDBValue(globals->replayTableID, rowID, 2, "encore", &actID);
        API.SetUserDBValue(globals->replayTableID, rowID, 4, "zoneSortVal", &zoneStortVal);
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

void ReplayRecorder_DeleteTimeAttackRow(int32 a1, void (*callback)(bool32), int32 a3)
{
    int32 id                 = API.GetUserDBRowUUID(globals->replayTableID, a1);
    int32 value              = 0;
    ReplayDB->deleteEntity   = SceneInfo->entity;
    ReplayDB->deleteCallback = callback;
    API.RemoveDBRow(globals->replayTableID, a1);
    TimeAttackData->status = 0;
    API.SetupSortedUserDBRowIDs(globals->taTableID);
    API.Unknown33(globals->taTableID, 4, "replayID", &id);
    int32 count = API.GetSortedUserDBRowCount(globals->taTableID);
    for (int32 i = 0; i < count; ++i) {
        uint32 uuid = API.GetSortedUserDBRowID(globals->taTableID, i);
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
        SceneInfo->entity   = store;
        ReplayDB->deleteCallback = NULL;
        ReplayDB->deleteEntity   = NULL;
    }
}

int32 ReplayRecorder_SetStatus(int32 status)
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
        Entity *store = SceneInfo->entity;
        if (ReplayDB->loadEntity)
            SceneInfo->entity = ReplayDB->loadEntity;
        ReplayDB->loadCallback(status == STATUS_OK);
        SceneInfo->entity = store;
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
        SceneInfo->entity = store;
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
