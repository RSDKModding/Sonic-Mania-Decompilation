#ifndef OBJ_REPLAYRECORDER_H
#define OBJ_REPLAYRECORDER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    void *actions[64];
    int32 frameCounter;
    int32 *writeBuffer;
    int32 *readBuffer;
    int32 *frameBuffer_w;
    int32 *frameBuffer_r;
    Entity *recorder_r;
    Entity *recorder_w;
    bool32 initialized;
    int32 dword124;
    int32 dword128;
    int32 dword12C;
    int32 dword130;
    int32 dword134;
    int32 dword138;
    int32 dword13C;
    int32 dword140;
    int32 dword144;
    int32 dword148;
    void *buffer;
    void (*loadCallback)(bool32);
    char filename[0x100];
    void (*saveFinishPtr)(bool32);
    int32 replayID;
    int32 replayRowID;
} ObjectReplayRecorder;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateLate);
    StateMachine(field_60);
    StateMachine(storedState);
    StateMachine(stateStore);
    uint16 animID;
    uint16 frameID;
    EntityPlayer *player;
    int32 paused;
    int32 field_78;
    int32 playing;
    int32 replayFrame;
    int32 field_84;
    int32 field_88;
    uint8 storedInputs;
    uint8 storedDirection;
    uint8 field_8E;
    uint8 field_8F;
    Vector2 storedPos;
    Vector2 storedVel;
    int32 storedRotation;
    int32 storedSpeed;
    uint8 storedAnim;
    uint8 field_A9;
    uint16 storedFrame;
    int32 alphaStore;
} EntityReplayRecorder;

// Object Struct
extern ObjectReplayRecorder *ReplayRecorder;

// Standard Entity Events
void ReplayRecorder_Update(void);
void ReplayRecorder_LateUpdate(void);
void ReplayRecorder_StaticUpdate(void);
void ReplayRecorder_Draw(void);
void ReplayRecorder_Create(void* data);
void ReplayRecorder_StageLoad(void);
void ReplayRecorder_EditorDraw(void);
void ReplayRecorder_EditorLoad(void);
void ReplayRecorder_Serialize(void);

// Extra Entity Functions
void ReplayRecorder_TitleCardCB(void);
void ReplayRecorder_Resume(EntityReplayRecorder *recorder);
void ReplayRecorder_ResumeStartCB(void);
void ReplayRecorder_FinishCB(void);
void ReplayRecorder_Buffer_Move(void);
void ReplayRecorder_SaveReplayDLG_NoCB(void);
void ReplayRecorder_SaveReplayDLG_YesCB(void);
void ReplayRecorder_Unknown6(void);
void ReplayRecorder_SaveReplay(void);
void ReplayRecorder_SavedReplay(bool32 status);
void ReplayRecorder_WaitWhileReplaySaves(bool32 a1);
void ReplayRecorder_Unknown10(int32 status);
void ReplayRecorder_Buffer_PackInPlace(int32 *tempWriteBuffer);
void ReplayRecorder_Buffer_Unpack(int32 *readBuffer, int32 *tempReadBuffer);
void ReplayRecorder_Buffer_SaveFile(const char *fileName, int32 *buffer);
void ReplayRecorder_SetReplayStatus(int32 status);
void ReplayRecorder_Buffer_LoadFile(const char *fileName, void *buffer, void (*callback)(bool32));
void ReplayRecorder_Load_CB(int32 status);
void ReplayRecorder_ConfigureGhost_CB(void);
void ReplayRecorder_SetupActions(void);
void ReplayRecorder_SetupWriteBuffer(void);
void ReplayRecorder_DrawGhostDisplay(void);
void ReplayRecorder_Record(EntityReplayRecorder *recorder, EntityPlayer *player);
void ReplayRecorder_StartRecording(EntityPlayer *player);
void ReplayRecorder_Play(EntityPlayer *player);
void ReplayRecorder_Rewind(EntityReplayRecorder *recorder);
void ReplayRecorder_Seek(EntityReplayRecorder *recorder, uint32 frame);
void ReplayRecorder_SeekFunc(EntityReplayRecorder *recorder);
void ReplayRecorder_Stop(EntityReplayRecorder *recorder);
void ReplayRecorder_SetGimmickState(EntityReplayRecorder *recorder, bool32 flag);
void ReplayRecorder_ApplyFrameData(EntityReplayRecorder *recorder, uint8 *buffer);
void ReplayRecorder_Unknown19(EntityReplayRecorder *recorder, uint8 *buffer);
bool32 ReplayRecorder_CheckPlayerGimmickState(EntityReplayRecorder *recorder);
void ReplayRecorder_PackFrame(uint8 *recording);
void ReplayRecorder_PlayBackInput(void);
void ReplayRecorder_Pause(EntityReplayRecorder *recorder);
void ReplayRecorder_PlayerState(void);
void ReplayRecorder_StatePlay(void);
void ReplayRecorder_None_Replay(void);
void ReplayRecorder_StateLate_Replay(void);
void ReplayRecorder_None_Record(void);
void ReplayRecorder_RecordFrameData(void);
void ReplayRecorder_LoadReplayDB(void (*callback)(bool32));
void ReplayRecorder_SaveReplayDB(void (*callback)(bool32));
void ReplayRecorder_CreateReplayDB(void);
uint32 ReplayRecorder_AddReplayID(uint8 actID, char zone, int32 charID, int32 score, char mode);
void ReplayRecorder_DeleteTimeAttackRow(int32 a1, void (*callback)(bool32), int32 a3);
void ReplayRecorder_DeleteReplayCB(int32 status);
void ReplayRecorder_DeleteReplaySaveCB(int32 status);
void ReplayRecorder_DeleteReplaySave2CB(int32 status);
int32 ReplayRecorder_SetStatus(int32 status);
int32 ReplayRecorder_ReplaySaveFinish(int32 status);
#endif

#endif //!OBJ_REPLAYRECORDER_H
