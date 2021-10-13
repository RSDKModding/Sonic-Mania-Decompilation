#ifndef OBJ_REPLAYRECORDER_H
#define OBJ_REPLAYRECORDER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    void *actions[64];
    int frameCounter;
    int *writeBuffer;
    int *readBuffer;
    int *frameBuffer_w;
    int *frameBuffer_r;
    Entity *recorder_r;
    Entity *recorder_w;
    bool32 initialized;
    int dword124;
    int dword128;
    int dword12C;
    int dword130;
    int dword134;
    int dword138;
    int dword13C;
    int dword140;
    int dword144;
    int dword148;
    void* buffer;
    void(*loadCallback)(bool32);
    char filename[0x100];
    void(*saveFinishPtr)(bool32);
    int replayID;
    int replayRowID;
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
    int paused;
    int field_78;
    int playing;
    int replayFrame;
    int field_84;
    int field_88;
    uint8 storedInputs;
    uint8 storedDirection;
    uint8 field_8E;
    uint8 field_8F;
    Vector2 storedPos;
    Vector2 storedVel;
    int storedRotation;
    int storedSpeed;
    uint8 storedAnim;
    uint8 field_A9;
    uint16 storedFrame;
    int alphaStore;
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
void ReplayRecorder_Unknown10(int status);
void ReplayRecorder_Buffer_PackInPlace(int *tempWriteBuffer);
void ReplayRecorder_Buffer_Unpack(int *readBuffer, int *tempReadBuffer);
void ReplayRecorder_Buffer_SaveFile(const char *fileName, int *buffer);
void ReplayRecorder_SetReplayStatus(int status);
void ReplayRecorder_Buffer_LoadFile(const char *fileName, void *buffer, void (*callback)(bool32));
void ReplayRecorder_Load_CB(int status);
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
uint32 ReplayRecorder_AddReplayID(uint8 actID, char zone, int charID, int score, char mode);
void ReplayRecorder_DeleteTimeAttackRow(int a1, void (*callback)(bool32), int a3);
void ReplayRecorder_DeleteReplayCB(int status);
void ReplayRecorder_DeleteReplaySaveCB(int status);
void ReplayRecorder_DeleteReplaySave2CB(int status);
int ReplayRecorder_SetStatus(int status);
int ReplayRecorder_ReplaySaveFinish(int status);
#endif

#endif //!OBJ_REPLAYRECORDER_H
