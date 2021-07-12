#include "../SonicMania.h"

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
        StateMachine_Run(entity->stateDraw);

        int *buffer = NULL;
        if (RSDK.GetEntityID(entity) == SLOT_REPLAYRECORDER_W)
            buffer = ReplayRecorder->writeBuffer;
        else
            buffer = ReplayRecorder->readBuffer;
        if (entity->replayFrame > buffer[4] && entity->state) {
            //ReplayRecorder_Stop(entity);
        }

        if (ReplayRecorder->dword144 && !entity->player->playerID && entity->state == ReplayRecorder_Unknown26) {
            if (entity->field_88 <= 0) {
                if (!entity->field_88) {
                    //ReplayRecorder_Stop(entity);
                    //ReplayRecorder_Rewind(entity);
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

}

void ReplayRecorder_Draw(void)
{
    RSDK_THIS(ReplayRecorder);
    //if (entity->playing)
    //    ReplayRecorder_DrawSprites();
}

void ReplayRecorder_Create(void* data)
{
    RSDK_THIS(ReplayRecorder);
    entity->active    = ACTIVE_NEVER;
    entity->drawOrder = 13;
    entity->inkEffect = INK_NONE;
    entity->field_88  = -1;
    entity->drawFX    = FX_FLIP;
    entity->field_AC  = 256;
    entity->visible   = globals->gameMode == MODE_TIMEATTACK;
}

void ReplayRecorder_StageLoad(void)
{
    ReplayRecorder->lastUUID  = 0;
    ReplayRecorder->lastRowID = -1;
    if (globals->gameMode == MODE_TIMEATTACK) {
        ReplayRecorder->active       = ACTIVE_ALWAYS;
        ReplayRecorder->frameCounter = -1;
        ReplayRecorder->dword12C     = 0;
        ReplayRecorder->dword120     = 0;
        ReplayRecorder->dword130     = 0;
        ReplayRecorder->dword134     = 0;
        ReplayRecorder->dword138     = 0;
        ReplayRecorder->dword13C     = 0;
        ReplayRecorder->dword140     = 0;
        ReplayRecorder->dword144     = 0;
        ReplayRecorder->dword148     = 0;
        ReplayRecorder->loadCallback = 0;
        ReplayRecorder->buffer       = 0;
        memset(ReplayRecorder->filename, 0, 0x100);
        ReplayRecorder->writeBuffer = globals->replayWriteBuffer;
        ReplayRecorder->writeSize   = globals->replayWriteBuffer + 14;
        ReplayRecorder->readBuffer  = globals->replayReadBuffer;
        ReplayRecorder->readSize    = globals->replayReadBuffer + 14;
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

            ReplayRecorder->dword124 = 0;
            ReplayRecorder->dword128 = 0;
            int *buffer              = NULL;
            if (RSDK.GetEntityID(ReplayRecorder->recorder_w) == SLOT_REPLAYRECORDER_R)
                buffer = ReplayRecorder->writeBuffer;
            else
                buffer = ReplayRecorder->readBuffer;

            if (globals->menuParam[97] && buffer[3]) {
                if (globals->menuParam[98]) {
                    globals->playerID         = globals->playerID | (globals->playerID << 8);
                    Player->configureGhost_CB = ReplayRecorder_ConfigureGhost_CB;
                }
                else {
                    globals->playerID = globals->playerID;
                }
            }
            LogHelpers_Print("CharID: %08x", globals->playerID);
        }
    }
}

void ReplayRecorder_Unknown1(void)
{
    int *buffer = NULL;
    if (RSDK.GetEntityID(ReplayRecorder->recorder_w) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;

    if (Player->playerCount != 1 || !buffer[3]) {
        ReplayRecorder->dword124 = 1;
    }
    if (buffer[3] == 1)
        ReplayRecorder->dword128 = 1;
}

void ReplayRecorder_Resume(void)
{
    if (ReplayRecorder->dword130) {
        ((EntityReplayRecorder *)ReplayRecorder->recorder_r)->field_78 = 2;
    }
    EntityReplayRecorder *recorder = (EntityReplayRecorder *)ReplayRecorder->recorder_w;
    if (ReplayRecorder->dword134 && recorder->playing) {
        if (ReplayRecorder->dword148) {
            LogHelpers_Print("ReplayRecorder_Resume()");
            recorder->paused             = false;
            recorder->player->visible = true;
            ReplayRecorder->dword140 = 1;
            return;
        }
        //ReplayRecorder_Seek(recorder);
    }
    ReplayRecorder->dword140 = 1;
}

void ReplayRecorder_Unknown2(void)
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

int ReplayRecorder_Unknown4(void)
{
    HUD->replaySaveEnabled = 1;
    ActClear->field_2C     = 0;
    ActClear->field_14 = 0;
    return 1;
}

int ReplayRecorder_CreateReplayDBEntry(void)
{
    ReplayRecorder->lastUUID  = 0;
    ReplayRecorder->lastRowID = -1;
    int mins                          = RSDK_sceneInfo->minutes;
    int secs                          = RSDK_sceneInfo->seconds;
    int millisecds                    = RSDK_sceneInfo->milliseconds;
    LogHelpers_Print("Bout to create ReplayDB entry...");
    int entry = ReplayRecorder_AddReplayID(globals->menuParam[93], globals->menuParam[92], (globals->menuParam[91] & 0xFF),
                                           (millisecds + 100 * (secs + 60 * mins)), RSDK_sceneInfo->filter == SCN_FILTER_ENCORE);
    if (entry == -1) {
        LogHelpers_Print("Table row ID invalid! %d", -1);
        ReplayRecorder_SavedReplay(false);
    }
    else {
        uint uuid                          = User.GetUserDBEntryUUID(globals->replayTableID, entry);
        ReplayRecorder->lastRowID = entry;
        ReplayRecorder->lastUUID  = uuid;
        char fileName[0x20];
        sprintf(fileName, "Replay_%08X.bin", uuid);
        LogHelpers_Print("Replay Filename: %s", fileName);
        UIWaitSpinner_Wait();
        ReplayRecorder->dword12C = 1;
        ReplayRecorder_Buffer_SaveFile(fileName, globals->replayTempWBuffer);
        HUD->replaySaveEnabled = 0;
    }
    return 1;
}

void ReplayRecorder_Unknown6(void)
{
    TextInfo buffer;
    INIT_TEXTINFO(buffer);
    Localization_GetString(&buffer, STR_SAVEREPLAY);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&buffer);
    if (dialog) {
        UIDialog_Unknown2(1, dialog, ReplayRecorder_Unknown4, 1);
        UIDialog_Unknown2(0, dialog, ReplayRecorder_CreateReplayDBEntry, 1);
        UIDialog_Unknown3(dialog);
    }
}

void ReplayRecorder_SaveReplay(void)
{
    if (globals->replayTempWBuffer[3]) {
        LogHelpers_Print("Saving replay...");
        foreach_all(HUD, hud)
        {
            foreach_break;
        }
        RSDK.SetSpriteAnimation(HUD->hudMappings, 11, &hud->taData2, true, 0);
        RSDK.PlaySFX(HUD->sfx_Click, 0, 255);
        EntityDialogRunner *runner                   = (EntityDialogRunner*)RSDK.CreateEntity(DialogRunner->objectID, DialogRunner_HandleCallback, 0, 0);
        runner->callback         = ReplayRecorder_Unknown6;
        runner->timer            = 45;
        runner->isPermanent = true;
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
        if (ReplayRecorder->lastRowID != -1)
            User.RemoveDBEntry(globals->replayTableID, ReplayRecorder->lastRowID);
        TextInfo buffer;
        INIT_TEXTINFO(buffer);
        Localization_GetString(&buffer, STR_NOREPLAYSPACE);
        EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&buffer);
        if (dialog) {
            UIDialog_Unknown2(2, dialog, 0, 1);
            UIDialog_Unknown3(dialog);
        }
        UIWaitSpinner_Wait2();
        ActClear->field_14 = 0;
        ActClear->field_2C     = 0;
        HUD->replaySaveEnabled = 1;
    }
}

void ReplayRecorder_WaitWhileReplaySaves(bool32 a1)
{
    if (a1) {
        if (TimeAttackData->unknown == -1) {
            UIWaitSpinner_Wait2();
            ActClear->field_14 = 0;
            foreach_all(HUD, hud) { foreach_break; }
            RSDK.SetSpriteAnimation(HUD->hudMappings, 10, &hud->taData2, true, 0);
            RSDK.PlaySFX(HUD->sfx_Starpost, 0, 255);
            HUD->replaySaveEnabled = 0;
        }
        else {
            User.SetUserDBValue(globals->taTableID, TimeAttackData->unknown, 4, "replayID", &ReplayRecorder->lastUUID);
            TimeAttackData_SaveTimeAttackDB(ReplayRecorder_Unknown10);
        }
    }
    else {
        TextInfo buffer;
        INIT_TEXTINFO(buffer);
        char fileName[0x20];
        sprintf(fileName, "Replay_%08X.bin", ReplayRecorder->lastUUID);
        if (ReplayRecorder->lastRowID != -1)
            User.RemoveDBEntry(globals->replayTableID, ReplayRecorder->lastRowID);
        User.DeleteUserFile(fileName, 0);
        Localization_GetString(&buffer, STR_NOREPLAYSPACE);
        EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&buffer);
        if (dialog) {
            UIDialog_Unknown2(2, dialog, 0, 1);
            UIDialog_Unknown3(dialog);
        }
        UIWaitSpinner_Wait2();
        ActClear->field_14 = 0;
        ActClear->field_2C = 0;
        HUD->replaySaveEnabled         = 1;
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
        RSDK.PlaySFX(HUD->sfx_Starpost, 0, 255);
        HUD->replaySaveEnabled = 0;
    }
}

void ReplayRecorder_Buffer_PackInPlace(int *tempWriteBuffer)
{
    LogHelpers_Print("Buffer_PackInPlace(%08x)", tempWriteBuffer);
    int frameOff = (int)(tempWriteBuffer + 14);
    if (*tempWriteBuffer == 0xF6057BED) {
        if (tempWriteBuffer[2] == 1) {
            LogHelpers_Print("Buffer_Ppack ERROR: Buffer is already packed");
        }
        else {
            int ptr         = 56;
            int *firstFrame       = (tempWriteBuffer + 14) + (7 * (tempWriteBuffer[4] + 2));
            int *framePtr   = (tempWriteBuffer + 14);
            byte *v6              = (byte*)(tempWriteBuffer + 14);
            for (int f = 0; f < tempWriteBuffer[4]; ++f) {
                byte frame[28];
                *((int *)&frame[0])  = framePtr[0];
                *((int *)&frame[4])  = framePtr[1];
                *((int *)&frame[8])  = 0;
                *((int *)&frame[12]) = 0;
                *((int *)&frame[16]) = framePtr[4];
                *((int *)&frame[20]) = framePtr[5];
                *((int *)&frame[24]) = framePtr[6];

                //int size    = ReplayRecorder_Buffer_PackEntry(v6, frame);
                //framePtr += 7;
                //v6 = (size + frameOff);
                //ptr += size;
                //frameOff += size;
            }
            LogHelpers_Print("Packed %d frames: %luB -> %luB", tempWriteBuffer[4], firstFrame, framePtr);
            tempWriteBuffer[11] = ptr;
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
    int frameOff = (int)(tempReadBuffer + 14);
    if (*tempReadBuffer == 0xF6057BED) {
        if (tempReadBuffer[2] == 1) {
            int compSize  = tempReadBuffer[11];
            readBuffer[0] = tempReadBuffer[0];
            readBuffer[1] = tempReadBuffer[1];
            readBuffer[2] = tempReadBuffer[2];
            readBuffer[3] = tempReadBuffer[3];
            readBuffer[4] = tempReadBuffer[4];
            readBuffer[5] = tempReadBuffer[5];
            int size      = (int)(7 * (int)(tempReadBuffer[4] + 2));
            if (tempReadBuffer[4] > 0) {
                int off = (int)(readBuffer + 14);
                for (int i = 0; i < tempReadBuffer[4]; ++i) {
                    //frameOff += ReplayRecorder_Buffer_UnpackEntry(off, frameOff);
                    //off += 28;
                }
            }
            LogHelpers_Print("Unpacked %d frames: %luB -> %luB", tempReadBuffer[4], compSize, size);
            readBuffer[2]  = 0;
            readBuffer[11] = size;
            memset(tempReadBuffer, 0, 0x100000);
            //result = 1;
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
        User.SaveUserFile(fileName, buffer, buffer[11], ReplayRecorder_SetReplayStatus, true);
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
    User.LoadUserFile(fileName, buffer, 0x100000, ReplayRecorder_Load_CB);
}

void ReplayRecorder_Load_CB(int status)
{
    if (ReplayRecorder->loadCallback)
        ReplayRecorder->loadCallback((status == STATUS_OK));
    ReplayRecorder->loadCallback = NULL;
    ReplayRecorder->buffer = NULL;
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
    entity->state          = StateMachine_None; //ReplayRecorder_Pause;
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
    ReplayRecorder->actions[7]  = StateMachine_None; // Cylinder_Unknown7;
    ReplayRecorder->actions[8]  = StateMachine_None; // Cylinder_Unknown6;
    ReplayRecorder->actions[9]  = StateMachine_None; // Cylinder_Unknown8;
    ReplayRecorder->actions[10] = StateMachine_None; // Cylinder_Unknown9;
    ReplayRecorder->actions[13] = StateMachine_None; // GymBar_Unknown6;
    ReplayRecorder->actions[14] = StateMachine_None; // GymBar_Unknown5;
    ReplayRecorder->actions[15] = StateMachine_None; // GymBar_Unknown7;
    ReplayRecorder->actions[16] = StateMachine_None; // Ice_StateFrozenPlayer;
    ReplayRecorder->actions[17] = StateMachine_None; // OOZSetup_Unknown11;
    ReplayRecorder->actions[18] = StateMachine_None; // OOZSetup_Unknown8;
    ReplayRecorder->actions[19] = StateMachine_None; // OOZSetup_Unknown10;
    ReplayRecorder->actions[20] = StateMachine_None; // OOZSetup_Player_State_Slide;
    ReplayRecorder->actions[21] = StateMachine_None; // PlaneSeeSaw_Unknown4;
    ReplayRecorder->actions[22] = StateMachine_None; // PlaneSeeSaw_Unknown5;
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
    ReplayRecorder->actions[51] = Player_State_RollLock;
    ReplayRecorder->actions[52] = Player_State_ForceRoll;
    ReplayRecorder->actions[53] = Player_State_Victory;
    ReplayRecorder->actions[54] = StateMachine_None; // SizeLaser_Unknown5;
    ReplayRecorder->actions[55] = StateMachine_None; // SizeLaser_Unknown7;
    ReplayRecorder->actions[56] = StateMachine_None; // SizeLaser_Unknown6;
}

void ReplayRecorder_SetupWriteBuffer(void)
{
    int *buffer     = ReplayRecorder->writeBuffer;
    *buffer    = 0xF6057BED;
    buffer[1]  = 6;
    buffer[2]  = 0;
    buffer[8]  = globals->menuParam[91];
    buffer[6]  = globals->menuParam[92];
    buffer[7]  = globals->menuParam[93];
    buffer[9]  = RSDK_sceneInfo->filter == 5;
    buffer[10] = Zone->timer;
    buffer[11] = 56;
    buffer[5]  = ReplayRecorder->frameCounter;
    LogHelpers_Print("characterID = %d", buffer[8]);
    LogHelpers_Print("zoneID = %d", buffer[6]);
    LogHelpers_Print("act = %d", buffer[7]);
    LogHelpers_Print("isPlusLayout = %d", buffer[9]);
    LogHelpers_Print("oscillation = %d", buffer[10]);
    buffer[3] = true;
}

/*void ReplayRecorder_DrawSprites(void)
{
    EntityPlayer *v1;             // ebx
    bool v2;                      // zf
    EntityInfo *result;           // eax
    ScreenData *v4;               // edx
    int v5;                       // ST30_4
    int v6;                       // ST2C_4
    int v7;                       // esi
    int v8;                       // edi
    __int16 v9;                   // ax
    int v10;                      // ecx
    int v11;                      // edi
    int v12;                      // esi
    int v13;                      // eax
    signed int v14;               // eax
    signed int v15;               // ecx
    unsigned int v16;             // edx
    int v17;                      // esi
    int v18;                      // ebx
    int v19;                      // esi
    int v20;                      // esi
    int v21;                      // eax
    int v23;                      // ecx
    Vector2 drawPos;              // [esp+Ch] [ebp-2Ch]
    EntityPlayer *player;         // [esp+14h] [ebp-24h] MAPDST
    unsigned int v27;             // [esp+18h] [ebp-20h]
    EntityReplayRecorder *entity; // [esp+1Ch] [ebp-1Ch] MAPDST
    int v29;                      // [esp+20h] [ebp-18h]
    int y1;                       // [esp+24h] [ebp-14h]
    unsigned int Colour;          // [esp+2Ch] [ebp-Ch]
    Hitbox a3;                    // [esp+30h] [ebp-8h]

    entity = RSDK_sceneInfo->entity;
    RSDK.GetEntityByID(SLOT_PLAYER1);
    v1     = entity->player;
    v2     = entity->state == 0;
    player = entity->player;
    if (!v2)
        goto LABEL_17;
    result = entity->field_AC;
    if (result > 0) {
        result           = (result - 4);
        entity->field_AC = result;
    }
    if (!entity->field_AC)
        return result;
LABEL_17:
    result = RSDK_sceneInfo;
    if (RSDK_sceneInfo->currentScreenID)
        return result;
    v4                     = RSDK_screens;
    entity->inkEffect = 0;
    v5                     = v1->position.y;
    v6                     = v1->position.x;
    v7                     = v4->position.y + v4->centerY;
    v8                     = v4->position.x + v4->centerX;
    v9                     = v4->width >> 1;
    a3.right               = v9;
    v10                    = v4->height >> 1;
    a3.left                = -v9;
    a3.bottom              = v10;
    a3.top                 = -v10;
    v11                    = v8 << 16;
    Colour                 = v7 << 16;
    result                 = MathHelpers_Unknown8(v11, v7 << 16, a3, v6, v5);
    if (result)
        return result;
    v12       = abs(v1->position.x - v11);
    v13       = abs(v1->position.y - Colour);
    v14       = MathHelpers_Unknown6((v12 >> 16) * (v12 >> 16) + (v13 >> 16) * (v13 >> 16));
    v15       = 2000;
    drawPos.x = 0;
    drawPos.y = 0;
    if (v14 < 2000)
        v15 = v14;
    if (v15 < 100)
        v15 = 100;
    v16         = (1099511628i64 * (3 * v15 - 300)) >> 32;
    LOWORD(y1)  = a3.right - 8;
    HIWORD(y1)  = a3.bottom - 8;
    v17         = 12 - ((v16 >> 7) + (v16 >> 31));
    HIWORD(v29) = a3.top + 8;
    LOWORD(v29) = a3.left + 8;
    a3.right    = a3.right - 8 - (v17 + 24);
    a3.bottom   = a3.bottom - 8 - (v17 + 24);
    a3.top += v17 + 24 + 8;
    a3.left += v17 + 24 + 8;
    v27    = 12 - ((v16 >> 7) + (v16 >> 31));
    result = MathHelpers_Unknown14(v1->position.x, v1->position.y, v11, Colour, *&a3.left, *&a3.right);
    if (!result)
        return result;
    v18            = RSDK.ATan2(v1->position.x - drawPos.x, v1->position.y - drawPos.y);
    v19            = v17 + 18;
    *&a3.right     = (v19 * RSDK.Cos256(v18) << 8) + drawPos.x;
    y1             = (v19 * RSDK.Sin256(v18) << 8) + drawPos.y;
    v20            = *&a3.right + (RSDK.Cos256(v18) << 10);
    v21            = RSDK.Sin256(v18);
    *(&Colour + 1) = -16160;
    LOBYTE(Colour) = -32;
    PlayerProbe_DrawDebug4(Colour, v23, entity->field_AC >> 1, *&a3.right, y1, v20, (v21 << 10) + y1);
    RSDK.DrawCircle(drawPos.x, drawPos.y, v27 + 16, Colour, entity->field_AC >> 1, INK_ALPHA, 0);
    entity->alpha    = entity->field_AC;
    *&entity->drawFX = 0x207;
    entity->scale.x  = 256;
    entity->scale.y  = 256;
    if (player->tailSpriteIndex != -1) {
        entity->rotation   = player->tailRotation;
        entity->direction  = player->tailDirection;
        entity->velocity.x = player->velocity.x;
        entity->velocity.y = player->velocity.y;
        RSDK.DrawSprite(&player->tailSpriteAnimData, &drawPos, 0);
    }
    entity->rotation   = player->rotation;
    entity->direction  = player->direction;
    result                  = RSDK.DrawSprite(&player->playerAnimator, &drawPos, 0);
    entity->drawFX     = 0;
    entity->alpha      = 255;
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    return result;
}

__int16 ReplayRecorder_StartRecording(EntityPlayer *player)
{
    EntityReplayRecorder *recA;   // esi
    EntityReplayRecorder *entity; // ecx
    __int16 result;               // ax

    LogHelpers_Print("ReplayRecorder_StartRecording()");
    recA              = ReplayRecorder->recorder_r;
    recA->active = ACTIVE_NORMAL;
    memset(globals->replayTempWBuffer, 0, 0x100000u);
    memset(globals->replayWriteBuffer, 0, 0x100000u);
    LogHelpers_Print("ReplayRecorder_Rewind()");
    recA->replayFrame = 0;
    ReplayRecorder_SetupWriteBuffer();
    LogHelpers_Print("ReplayRecorder_Record()");
    entity = ReplayRecorder->recorder_r;
    if (player)
        entity->player = player;
    entity->storedState = player->state;
    entity->state       = ReplayRecorder_Unknown26;
    entity->stateDraw   = ReplayRecorder_RecordFrameData;
    entity->storedAnim  = player->playerAnimator.animationID;
    result              = player->playerAnimator.frameID;
    entity->storedFrame = result;
    return result;
}

int ReplayRecorder_Play(EntityPlayer *player)
{
    EntityReplayRecorder *v2; // esi
    int *v3;                  // ecx

    LogHelpers_Print("ReplayRecorder_Play()");
    v2 = ReplayRecorder->recorder_w;
    if (RSDK.GetEntityID(&ReplayRecorder->recorder_w->base) == 37)
        v3 = ReplayRecorder->writeBuffer;
    else
        v3 = ReplayRecorder->readBuffer;
    if (!v3[3])
        return LogHelpers_Print("No replay to play");
    v2->active = 2;
    if (player) {
        v2->player   = player;
        v2->field_60 = player->state;
    }
    v2->playing = 0;
    if (player->playerID) {
        v2->playing                  = 1;
        ReplayRecorder->dword138 = 1;
    }
    else {
        Zone->timer          = v3[10];
        player->replayInputState = ReplayRecorder_PlayBackInput;
        player->controllerID     = 2;
        RSDK.AssignControllerID(2, -2);
        ReplayRecorder->dword13C = 1;
    }
    v2->state     = ReplayRecorder_StatePlay;
    v2->stateDraw = 0;
    return (RSDK.SetSettingsValue)(SETTINGS_DIMTIMER, 54000);
}

int ReplayRecorder_Rewind(EntityReplayRecorder *RecorderPtr)
{
    EntityReplayRecorder *v1; // esi
    int result;               // eax

    v1              = RecorderPtr;
    result          = LogHelpers_Print("ReplayRecorder_Rewind()");
    v1->replayFrame = 0;
    return result;
}

_BYTE * ReplayRecorder_Seek(EntityReplayRecorder *recorder)
{
    _BYTE *result;        // eax
    unsigned int v3;      // ecx
    unsigned int v4;      // edx
    unsigned int v6;      // esi
    int v8;               // ecx
    unsigned int v9;      // edi
    _BYTE *v10;           // edx
    char v11;             // cl
    int v12;              // esi
    unsigned __int8 *v13; // ebx
    int v15;              // [esp-4h] [ebp-8h]

    if (RSDK.GetEntityID(&recorder->base) == SLOT_REPLAYRECORDER_W)
        result = ReplayRecorder->writeSize;
    else
        result = ReplayRecorder->readSize;
    v3 = recorder->field_84;
    v4 = 0;
    if (!v3)
        return result;
    while (*result != 3) {
        ++v4;
        result += 28;
        if (v4 >= v3)
            return result;
    }
    v6 = v4;
    LogHelpers_Print("ReplayRecorder_Seek(%u)", v4);
    recorder->replayFrame = v6;
    if (RSDK.GetEntityID(&recorder->base) == SLOT_REPLAYRECORDER_W)
        v8 = ReplayRecorder->writeSize;
    else
        v8 = ReplayRecorder->readSize;
    v15 = v8;
    v9  = v6;
    v10 = (v8 + 28 * v6);
    v11 = *v10;
    if (*v10 == 1)
        goto LABEL_22;
    result = v10;
    do {
        if (v11 == 3)
            break;
        if (v9 > v6)
            return result;
        v11 = *(result - 28);
        result -= 28;
        --v9;
        v10 = result;
    } while (v11 != 1);
    if (v9 > v6)
        return result;
LABEL_22:
    result = ReplayRecorder_ApplyFrameData(recorder, v10);
    if (v9 >= v6)
        return result;
    v12 = v6 - v9;
    v13 = (v15 + 28 * v9);
    do {
        v13 += 28;
        ReplayRecorder_Unknown19(recorder, v13);
        --v12;
    } while (v12);
    return result;
}

int ReplayRecorder_Stop(EntityReplayRecorder *this)
{
    EntityReplayRecorder *v1; // esi
    EntityPlayer *result;     // eax

    v1 = this;
    LogHelpers_Print("ReplayRecorder_Stop()");
    result        = v1->player;
    v1->state     = 0;
    v1->stateDraw = 0;
    if (!result)
        return result;
    if (result->replayInputState == ReplayRecorder_PlayBackInput)
        result->replayInputState = 0;
    return result;
}

void ReplayRecorder_SetChibiForms(EntityReplayRecorder *ReplayRecorder, bool32 flags) 
{ 
 //TODO
}

int ReplayRecorder_ApplyFrameData(EntityReplayRecorder *recorder, unsigned __int8 *frameData)
{
    EntityPlayer *player; // esi
    int result;           // eax

    player = recorder->player;
    if (!player)
        return result;
    player->position.x = *(frameData + 1) & 0xFFFF0000;
    player->position.y = *(frameData + 2) & 0xFFFF0000;
    player->velocity.x = *(frameData + 3) & 0xFFFF0000;
    player->velocity.y = *(frameData + 4) & 0xFFFF0000;
    player->direction  = frameData[3];
    player->rotation   = *(frameData + 5);
    ReplayRecorder_SetChibiForms(recorder, ((frameData[1] & 8u) > 0));
    RSDK.SetSpriteAnimation(player->spriteIndex, frameData[24], &player->playerAnimator, true, frameData[25]);
    result                                = 0;
    player->playerAnimator.animationSpeed = 0;
    return result;
}

void ReplayRecorder_Unknown19(EntityReplayRecorder *ReplayRecorder, unsigned __int8 *buffer)
{
    EntityPlayer *v2;    // edi
    unsigned __int8 *v3; // esi
    unsigned __int8 v4;  // al

    v2 = ReplayRecorder->player;
    v3 = buffer;
    if (v2) {
        if (buffer[1] & 2) {
            v2->position.x = *(buffer + 1) & 0xFFFF0000;
            v2->position.y = *(buffer + 2) & 0xFFFF0000;
        }
        if (buffer[1] & 4) {
            v2->velocity.x = *(buffer + 3) & 0xFFFF0000;
            v2->velocity.y = *(buffer + 4) & 0xFFFF0000;
        }
        if (buffer[1] & 0x20)
            v2->rotation = *(buffer + 5);
        if (buffer[1] & 0x10)
            v2->direction = buffer[3];
        ReplayRecorder_SetChibiForms(ReplayRecorder, ((buffer[1] & 8u) > 0));
        v4 = v3[1];
        if (v4 & 0x40) {
            RSDK.SetSpriteAnimation(v2->spriteIndex, v3[24], &v2->playerAnimator, true, v3[25]);
        }
        else {
            if ((v4 & 0x80u) == 0)
                return;
            RSDK.SetSpriteAnimation(v2->spriteIndex, v2->playerAnimator.animationID, &v2->playerAnimator, true, v3[25]);
        }
        v2->playerAnimator.animationSpeed = 0;
    }
}

int ReplayRecorder_CheckPlayerGimmickState(EntityReplayRecorder *this)
{
    EntityPlayer *player; // esi
    int result;           // eax

    player = this->player;
    if (!player || !RSDK.CheckStageFolder("MMZ") && !RSDK.CheckStageFolder("PSZ2"))
        goto LABEL_11;
    if (RSDK.CheckStageFolder("MMZ"))
        return player->isChibi;
    if (RSDK.CheckStageFolder("PSZ2"))
        result = player->state == Ice_StateFrozenPlayer;
    else
    LABEL_11:
        result = 0;
    return result;
}

EntityReplayRecorder * ReplayRecorder_PackFrame(unsigned __int8 *recording)
{
    int v2;                         // eax
    int v3;                         // esi
    int v4;                         // edi
    int v5;                         // eax MAPDST
    unsigned int v7;                // esi
    float v8;                       // xmm1_4
    float v9;                       // xmm0_4
    EntityReplayRecorder *recorder; // eax MAPDST
    int *writeBuffer;               // [esp+4h] [ebp-24h]
    unsigned __int8 frameBuf[28];   // [esp+8h] [ebp-20h]

    frameBuf[0]    = 0;
    writeBuffer    = ReplayRecorder->writeBuffer;
    v2             = RSDK_sceneInfo->entity->replayFrame;
    v3             = ReplayRecorder->writeSize;
    recorder       = RSDK_sceneInfo->entity;
    *&frameBuf[17] = 0i64;
    v4             = 7 * v2;
    *&frameBuf[25] = 0;
    *&frameBuf[1]  = 0i64;
    frameBuf[27]   = 0;
    v5             = ReplayRecorder_Buffer_PackEntry(frameBuf, recording);
    *(v3 + 4 * v4) = *recording;
    _mm_storel_epi64((v3 + 4 * v4 + 16), _mm_loadl_epi64(recording + 1));
    *(v3 + 4 * v4 + 24) = *(recording + 6);
    v7                  = writeBuffer[4];
    if (v7) {
        v8                  = v7;
        v9                  = v5;
        *(writeBuffer + 12) = ((v8 * *(writeBuffer + 12)) + v9) / (v7 + 1);
    }
    else {
        *(writeBuffer + 12) = v5;
    }
    writeBuffer[11] += v5;
    ++recorder->replayFrame;
    ++writeBuffer[4];
    return recorder;
}

int ReplayRecorder_PlayBackInput()
{
    EntityPlayer *entity;             // edi
    EntityReplayRecorder *recorder_w; // esi MAPDST
    int *buffer;                      // ecx
    int result;                       // eax
    int size;                         // edx
    unsigned __int8 *playbackBuf;     // ebx
    unsigned __int8 state;            // dl
    BOOL v7;                          // ecx
    int v8;                           // eax
    BOOL v9;                          // edx
    unsigned __int8 inputs;           // dl
    ControllerInputData *v12;         // esi
    bool v13;                         // cf
    ControllerInputData *v14;         // edx
    ControllerInputData *controllers; // ecx
    ControllerInputData *controller;  // eax MAPDST
    BOOL v20;                         // [esp+8h] [ebp-8h]
    int v21;                          // [esp+8h] [ebp-8h] MAPDST

    entity     = RSDK_sceneInfo->entity;
    recorder_w = ReplayRecorder->recorder_w;
    recorder_w = ReplayRecorder->recorder_w;
    if (RSDK.GetEntityID(&recorder_w->base) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;
    result = ReplayRecorder->frameCounter;
    if (result < buffer[5] || entity != recorder_w->player)
        return result;
    if (RSDK.GetEntityID(&recorder_w->base) == SLOT_REPLAYRECORDER_W)
        size = ReplayRecorder->writeSize;
    else
        size = ReplayRecorder->readSize;
    playbackBuf = (size + 28 * recorder_w->replayFrame);
    state       = *playbackBuf;
    if (*playbackBuf) {
        v20 = state == 2 && playbackBuf[1] & 1;
        v7  = state == 1 || state == 3;
        v8  = v7 || v20;
        v9  = state == 2 && playbackBuf[1] & 2;
        v21 = v7 || v9;
        if (v8 == 1) {
            inputs                                           = playbackBuf[2];
            v12                                              = RSDK_controller;
            RSDK_controller[entity->controllerID].keyUp.down = (playbackBuf[2] & 1u) > 0;
            v12[entity->controllerID].keyDown.down           = (inputs & 2u) > 0;
            v12[entity->controllerID].keyLeft.down           = (inputs & 4u) > 0;
            v12[entity->controllerID].keyRight.down          = (inputs & 8u) > 0;
            v12[entity->controllerID].keyA.press             = (inputs & 0x10u) > 0;
            v13                                              = (inputs & 0x20u) > 0;
            v14                                              = v12;
            v14[entity->controllerID].keyA.down              = v13;
        }
    }
    else {
        v21 = 0;
    }
    controllers = RSDK_controller;
    controller  = &RSDK_controller[entity->controllerID];
    if (controller->keyUp.down)
        controller->keyUp.press = controller->keyUp.press == 0;
    else
        controller->keyUp.press = 0;
    controller = &controllers[entity->controllerID];
    if (controller->keyDown.down)
        controller->keyDown.press = controller->keyDown.press == 0;
    else
        controller->keyDown.press = 0;
    controller = &controllers[entity->controllerID];
    if (controller->keyLeft.down)
        controller->keyLeft.press = controller->keyLeft.press == 0;
    else
        controller->keyLeft.press = 0;
    result     = 144 * entity->controllerID;
    controller = (controllers + result);
    if (controller->keyRight.down) {
        result                     = controller->keyRight.press == 0;
        controller->keyRight.press = result;
    }
    else {
        controller->keyRight.press = 0;
    }
    if (!v21)
        return result;
    recorder_w->storedPos.x = *(playbackBuf + 1);
    result                  = *(playbackBuf + 2);
    recorder_w->storedPos.y = result;
    return result;
}

ReplayRecorder *ReplayRecorder_Pause()
{
    ReplayRecorder *result;           // eax
    EntityReplayRecorder *recorder_w; // edi
    EntityPlayer *player;             // esi
    EntityReplayRecorder *v3;         // ST14_4
    int v4;                           // edx
    unsigned __int8 *frameData;       // ebx
    unsigned __int8 v6;               // al
    int v7;                           // edi
    int v8;                           // ebx
    int v9;                           // ST14_4
    int v10;                          // ST10_4
    int v11;                          // ecx
    unsigned __int8 v12;              // al
    int v13;                          // ecx
    int v14;                          // ecx
    Entity *entity;                   // [esp+Ch] [ebp-1Ch]
    int v16;                          // [esp+10h] [ebp-18h]
    int v17;                          // [esp+14h] [ebp-14h]
    int v18;                          // [esp+18h] [ebp-10h]
    int v19;                          // [esp+1Ch] [ebp-Ch]
    Hitbox a3;                        // [esp+20h] [ebp-8h]

    result     = ReplayRecorder;
    recorder_w = ReplayRecorder->recorder_w;
    player     = RSDK_sceneInfo->entity;
    if (!recorder_w->playing)
        return result;
    v3                                    = ReplayRecorder->recorder_w;
    player->playerAnimator.animationSpeed = 0;
    if (RSDK.GetEntityID(&v3->base) == SLOT_REPLAYRECORDER_W)
        v4 = ReplayRecorder->writeSize;
    else
        v4 = ReplayRecorder->readSize;
    frameData = (v4 + 28 * recorder_w->replayFrame);
    if (recorder_w->state) {
        v6 = *frameData;
        if (*frameData == 2) {
            ReplayRecorder_Unknown19(recorder_w, frameData);
        }
        else if (v6 == 1 || v6 == 3) {
            ReplayRecorder_ApplyFrameData(recorder_w, frameData);
            if (*frameData == 3 && !ReplayRecorder->dword148) {
                ReplayRecorder->dword148 = 1;
                if (!ReplayRecorder->dword140) {
                    LogHelpers_Print("ReplayRecorder_Pause()");
                    recorder_w->paused = 1;
                }
            }
        }
    }
    else {
        player->velocity.x = 0;
        player->velocity.y = 0;
    }
    if (FarPlane) {
        entity = 0;
        v7     = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        v8     = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
        if (RSDK.GetEntities(FarPlane->objectID, &entity) == 1) {
            while (1) {
                v9          = player->position.y;
                v10         = player->position.x;
                v11         = entity->position.x;
                v16         = entity[1].scale.x;
                v17         = entity[1].scale.y;
                v18         = v11 - ((v11 - v7) >> 1) + 0x8000;
                v19         = entity->position.y - ((entity->position.y - v8) >> 1) + 0x8000;
                a3.right    = HIWORD(entity[1].position.x);
                LOWORD(v11) = HIWORD(entity[1].position.y);
                a3.left     = -a3.right;
                a3.bottom   = v11;
                a3.top      = -v11;
                if (MathHelpers_Unknown8(entity[1].scale.x, entity[1].scale.y, a3, v10, v9))
                    break;
                if (RSDK.GetEntities(FarPlane->objectID, &entity) != 1)
                    goto LABEL_19;
            }
            v14                     = player->position.y - v17;
            player->position.x = v18 + ((player->position.x - v16) >> 1);
            player->position.y = (v14 >> 1) + v19;
            RSDK.BreakForeachLoop();
            player->drawFX |= 4u;
            player->scale.x = 256;
            player->scale.y = 256;
            v12                  = Zone->drawOrderLow;
        }
        else {
        LABEL_19:
            player->scale.x = 512;
            player->scale.y = 512;
            v12                  = Zone->playerDrawLow;
        }
        player->drawOrder = v12;
    }
    result = ReplayRecorder;
    if (!ReplayRecorder->dword148)
        return result;
    if (ReplayRecorder->dword140)
        return result;
    v13                  = player->timer;
    player->visible = ~(player->timer >> 2) & 1;
    result               = (v13 + 1);
    player->timer        = v13 + 1;
    return result;
}

int ReplayRecorder_StatePlay()
{
    EntityReplayRecorder *entity; // esi
    int *buffer;                  // ebx
    unsigned __int8 *v2;          // edx
    int v3;                       // edi
    int result;                   // eax
    int v5;                       // edi

    RSDK_THIS(ReplayRecorder);
    if (RSDK.GetEntityID(entity) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeBuffer;
    else
        buffer = ReplayRecorder->readBuffer;
    if (RSDK.GetEntityID(&entity->base) == SLOT_REPLAYRECORDER_W)
        v2 = ReplayRecorder->writeSize;
    else
        v2 = ReplayRecorder->readSize;
    v3     = ReplayRecorder->frameCounter;
    result = buffer[5];
    if (v3 >= result) {
        if (v3 != result) {
            if (v3 <= result)
                return result;
            v5 = v3 - result;
            if (!entity->playing)
                result = (loc_434CE0)(entity, v5);
            entity->replayFrame = v5;
        }
        entity->stateDraw = ReplayRecorder_Unknown25;
        entity->state     = ReplayRecorder_Unknown24;
    }
    else if (entity->playing) {
        result = ReplayRecorder_ApplyFrameData(entity, v2);
    }
    return result;
}*/

void ReplayRecorder_Unknown24(void) {}

void ReplayRecorder_Unknown25(void)
{
    RSDK_THIS(ReplayRecorder);
    EntityPlayer *player = entity->player;
    if (!entity->replayFrame) {
        EntityCamera* camera = player->camera;
        if (camera) {
            camera->position.x          = player->position.x;
            camera->position.y          = player->position.y;
            RSDK_screens[camera->screenID].position.x = (camera->position.x >> 16);
            RSDK_screens[camera->screenID].position.y = (camera->position.y >> 16);
            //if (WarpDoor)
            //    WarpDoor_Unknown5(-1, 0);
            //else
            //    WarpDoor_Unknown4();
            camera->state       = Camera_State_Follow;
            player->scrollDelay = 0;
        }
    }
    if (entity->playing) {
        if (player->state != StateMachine_None /*ReplayRecorder_Pause*/)
            entity->field_60 = player->state;
    }

    int *buffer = NULL;
    if (RSDK.GetEntityID(entity) == SLOT_REPLAYRECORDER_W)
        buffer = ReplayRecorder->writeSize;
    else
        buffer = ReplayRecorder->readSize;
    int *framePtr     = &buffer[7 * entity->replayFrame];
    if (!entity->playing) {
        if (*framePtr) {
            if (*framePtr == 1 || (*framePtr == 3) || (*framePtr == 2 && *(framePtr + 1) & 2)) {
                int v10 = abs(player->position.x - *(framePtr + 4));
                int v11 = abs(player->position.y - *(framePtr + 8));
                if (MathHelpers_Unknown6((v10 >> 16) * (v10 >> 16) + (v11 >> 16) * (v11 >> 16)) << 16 >= 0x20000) {
                    player->position.x += (*(framePtr + 4) - player->position.x) >> 1;
                    player->position.y += (*(framePtr + 8) - player->position.y) >> 1;
                }
            }
        }
    }
    ++entity->replayFrame;
}

void ReplayRecorder_Unknown26(void) {}

void ReplayRecorder_RecordFrameData(void)
{
    byte recording[28]; 

    RSDK_THIS(ReplayRecorder);
    if (entity->replayFrame < entity->field_84 - 1) {
        EntityPlayer *player          = entity->player;
        int inputState      = 0;
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

        bool32 isGimmickState = false; // ReplayRecorder_CheckPlayerGimmickState(entity);
        Animator *animator    = &player->playerAnimator;
        entity->stateStore    = player->state;
        /*if (isGimmickState && RSDK.CheckStageFolder("PSZ2") && player->state == Ice_StateFrozenPlayer
            && (v9 = player->field_218, *(v9 + 54) == Ice->objectID)) {
            data = (v9 + 136);
        }*/
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
            recording[3]             = player->direction;
            *((int *)&recording[20]) = player->rotation;
            *((int *)&recording[4])  = player->position.x;
            *((int *)&recording[8])  = player->position.y;
            *((int *)&recording[12]) = player->velocity.x;
            *((int *)&recording[16]) = player->velocity.y;
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
                *((int*)&recording[4]) = player->position.x;
                *((int*)&recording[8]) = player->position.y;
            }
            if (player->velocity.x != entity->storedVel.x || player->velocity.y != entity->storedVel.y) {
                changedVals |= 4;
                *((int*)&recording[12]) = player->velocity.x;
                *((int*)&recording[16]) = player->velocity.y;
            }
            if (player->groundVel != entity->storedRotation) {
                changedVals |= 0x20;
                *((int*)&recording[20]) = player->rotation;
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
                    //ReplayRecorder_PackFrame(recording);
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
        //ReplayRecorder_PackFrame(recording);
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
        entity->stateDraw = StateMachine_None;
    }
}

void ReplayRecorder_LoadReplayDB(void (*callback)(bool32))
{
    if (globals->replayTableID != 0xFFFF && globals->replayTableLoaded == STATUS_OK || globals->replayTableLoaded == STATUS_CONTINUE)
        return;
    LogHelpers_Print("Loading Replay DB");
    globals->replayTableLoaded   = STATUS_CONTINUE;
    ReplayDB->loadEntity   = RSDK_sceneInfo->entity;
    ReplayDB->loadCallback = NULL;
    globals->replayTableID       = User.LoadUserDB("ReplayDB.bin", ReplayRecorder_SetStatus); //, callback
    if (globals->replayTableID == -1) {
        LogHelpers_Print("Couldn't claim a slot for loading %s", "ReplayDB.bin");
        globals->replayTableLoaded = STATUS_ERROR;
    }
}

void ReplayRecorder_SaveReplayDB(void(*callback)(bool32))
{
    if (User.GetUserStorageNoSave() || globals->replayTableID == 0xFFFF || globals->replayTableLoaded != STATUS_OK) {
        if (callback)
            callback(false);
    }
    else {
        LogHelpers_Print("Saving Replay DB");
        ReplayDB->saveEntity   = RSDK_sceneInfo->entity;
        ReplayDB->saveCallback = callback;
        User.SaveUserDB(globals->replayTableID, ReplayRecorder_ReplaySaveFinish);
    }
}

void ReplayRecorder_CreateReplayDB(void)
{
    globals->replayTableID =
        User.InitUserDB("ReplayDB.bin", 4, "score", 2, "zoneID", 2, "act", 2, "characterID", 2, "encore", 4, "zoneSortVal", 0);

    if (globals->replayTableID == -1)
        globals->replayTableLoaded = STATUS_ERROR;
    else
        globals->replayTableLoaded = STATUS_OK;
}

uint ReplayRecorder_AddReplayID(byte actID, char zone, int charID, int score, char mode)
{
    if (globals->replayTableLoaded == STATUS_OK) {
        uint entry        = User.AddUserDBEntry(globals->replayTableID);
        int zoneStortVal = score & 0x3FFFFFF | ((actID & 1 | 2 * (mode & 1 | 2 * zone)) << 26);
        User.SetUserDBValue(globals->replayTableID, entry, 4, "score", &score);
        User.SetUserDBValue(globals->replayTableID, entry, 2, "zoneID", &zone);
        User.SetUserDBValue(globals->replayTableID, entry, 2, "act", &actID);
        User.SetUserDBValue(globals->replayTableID, entry, 2, "characterID", &charID);
        User.SetUserDBValue(globals->replayTableID, entry, 2, "encore", &actID);
        User.SetUserDBValue(globals->replayTableID, entry, 4, "zoneSortVal", &zoneStortVal);
        uint UUID = User.GetUserDBEntryUUID(globals->replayTableID, entry);
        char createTime[24];
        sprintf(createTime, "");
        User.GetUserDBCreationTime(globals->replayTableID, entry, createTime, 23, "%Y/%m/%d %H:%M:%S");
        LogHelpers_Print("Replay DB Added Entry");
        LogHelpers_Print("Created at %s", createTime);
        LogHelpers_Print("Row ID: %d", entry);
        LogHelpers_Print("UUID: %08X", UUID);
        return entry;
    }
    return -1;
}

void ReplayRecorder_DeleteTimeAttackRow(int a1, void(*callback)(bool32), int a3)
{
    int id                   = User.GetUserDBEntryUUID(globals->replayTableID, a1);
    int value                = 0;
    ReplayDB->deleteEntity   = RSDK_sceneInfo->entity;
    ReplayDB->deleteCallback = callback;
    User.RemoveDBEntry(globals->replayTableID, a1);
    TimeAttackData->status = 0;
    User.Unknown31(globals->taTableID);
    User.Unknown33(globals->taTableID, 4, "replayID", &id);
    int count = User.GetUserDBUnknownCount(globals->taTableID);
    for (int i = 0; i < count; ++i) {
        uint uuid = User.GetUserDBUnknown(globals->taTableID, i);
        LogHelpers_Print("Deleting Time Attack replay from row #%d", uuid);
        User.SetUserDBValue(globals->taTableID, uuid, 4, "replayID", &value);
    }

    char filename[0x20];
    sprintf(filename, "Replay_%08X.bin", id);
    if (!a3)
        User.DeleteUserFile(filename, ReplayRecorder_DeleteReplayCB);
    else
        User.DeleteUserFile(filename, ReplayRecorder_DeleteReplaySave2CB);
}

void ReplayRecorder_DeleteReplayCB(int status)
{
    LogHelpers_Print("DeleteReplay_CB(%d)", status);
    User.SaveUserDB(globals->replayTableID, ReplayRecorder_DeleteReplaySaveCB);
}

int ReplayRecorder_DeleteReplaySaveCB(int status)
{
    LogHelpers_Print("DeleteReplaySave_CB(%d)", status);
    User.SaveUserDB(globals->taTableID, ReplayRecorder_DeleteReplaySave2CB);
    return 1;
}

int ReplayRecorder_DeleteReplaySave2CB(int status)
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
    return 1;
}

int ReplayRecorder_SetStatus(int status)
{
    if (status == STATUS_OK) {
        globals->replayTableLoaded = STATUS_OK;
        User.Unknown31(globals->replayTableID);
        LogHelpers_Print("Load Succeeded! Replay count: %d", User.GetUserDBUnknownCount(globals->replayTableID));
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
        RSDK_sceneInfo->entity     = store;
        ReplayDB->loadCallback = NULL;
        ReplayDB->loadEntity   = NULL;
    }
    return 1;
}

int ReplayRecorder_ReplaySaveFinish(int status)
{
    if (ReplayDB->saveCallback) {
        Entity* store = RSDK_sceneInfo->entity;
        if (ReplayDB->saveEntity)
            RSDK_sceneInfo->entity = ReplayDB->saveEntity;
        ReplayDB->saveCallback((status == STATUS_OK));
        RSDK_sceneInfo->entity = store;
        ReplayDB->saveCallback = NULL;
        ReplayDB->saveEntity   = NULL;
    }
    return 1;
}

void ReplayRecorder_EditorDraw(void)
{

}

void ReplayRecorder_EditorLoad(void)
{

}

void ReplayRecorder_Serialize(void)
{

}
#endif
