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
    int *writeSize;
    int *readSize;
    Entity *recorder_r;
    Entity *recorder_w;
    int dword120;
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
    int lastUUID;
    int lastRowID;
} ObjectReplayRecorder;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    StateMachine(field_60);
    StateMachine(storedState);
    StateMachine(stateStore);
    ushort animID;
    ushort frameID;
    EntityPlayer *player;
    int paused;
    int field_78;
    int playing;
    int replayFrame;
    int field_84;
    int field_88;
    byte storedInputs;
    byte storedDirection;
    byte field_8E;
    byte field_8F;
    Vector2 storedPos;
    Vector2 storedVel;
    int storedRotation;
    int storedSpeed;
    byte storedAnim;
    byte field_A9;
    ushort storedFrame;
    int field_AC;
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
void ReplayRecorder_Unknown1(void);
void ReplayRecorder_Resume(void);
void ReplayRecorder_Unknown2(void);
void ReplayRecorder_Buffer_Move(void);
void ReplayRecorder_Unknown4(void);
void ReplayRecorder_CreateReplayDBEntry(void);
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
void ReplayRecorder_DrawSprites(void);
void ReplayRecorder_StartRecording(EntityPlayer *player);
void ReplayRecorder_Play(EntityPlayer *player);
void ReplayRecorder_Rewind(EntityReplayRecorder *RecorderPtr);
void ReplayRecorder_Seek(EntityReplayRecorder *recorder);
void ReplayRecorder_Stop(EntityReplayRecorder *this);
void ReplayRecorder_SetChibiForms(EntityReplayRecorder *ReplayRecorder, bool32 flags);
void ReplayRecorder_Unknown19(EntityReplayRecorder *ReplayRecorder, byte *buffer);
bool32 ReplayRecorder_CheckPlayerGimmickState(EntityReplayRecorder *this);
void ReplayRecorder_PackFrame(byte *recording);
void ReplayRecorder_PlayBackInput(void);
void ReplayRecorder_Pause(void);
void ReplayRecorder_StatePlay(void);
void ReplayRecorder_Unknown24(void);
void ReplayRecorder_Unknown25(void);
void ReplayRecorder_Unknown26(void);
void ReplayRecorder_RecordFrameData(void);
void ReplayRecorder_LoadReplayDB(void (*callback)(bool32));
void ReplayRecorder_SaveReplayDB(void (*callback)(bool32));
void ReplayRecorder_CreateReplayDB(void);
uint ReplayRecorder_AddReplayID(byte actID, char zone, int charID, int score, char mode);
void ReplayRecorder_DeleteTimeAttackRow(int a1, void (*callback)(bool32), int a3);
void ReplayRecorder_DeleteReplayCB(int status);
void ReplayRecorder_DeleteReplaySaveCB(int status);
void ReplayRecorder_DeleteReplaySave2CB(int status);
int ReplayRecorder_SetStatus(int status);
int ReplayRecorder_ReplaySaveFinish(int status);
#endif

#endif //!OBJ_REPLAYRECORDER_H
