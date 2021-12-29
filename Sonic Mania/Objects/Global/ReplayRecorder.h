#ifndef OBJ_REPLAYRECORDER_H
#define OBJ_REPLAYRECORDER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS

#define Replay_Signature  (0xF6057BED)
#define Replay_BufferSize (0x100000)

#define Replay_MaxFrameCount (37447)

typedef struct {
    uint32 signature;
    int32 version;
    bool32 isPacked;
    bool32 isNotEmpty;
    int32 frameCount;
    int32 startingFrame;
    int32 zoneID;
    int32 act;
    int32 characterID;
    bool32 isPlusLayout;
    int32 oscillation;
    int32 bufferSize;
    float averageFrameSize;
    int32 unused;
} ReplayHeader;

typedef struct {
    uint8 info;
    uint8 flags;
    uint8 inputs;
    uint8 dir;
    Vector2 position;
    Vector2 velocity;
    int32 rotation;
    uint8 anim;
    uint8 frame;
} ReplayFrame;

typedef struct {
    ReplayHeader header;
    ReplayFrame frames[Replay_MaxFrameCount];
    // there's an extra 4 bytes here, but they're just padding to make the size correct
    int32 padding;
} Replay;

typedef enum {
    REPLAY_INFO_NONE,
    REPLAY_INFO_STATECHANGE,
    REPLAY_INFO_USEFLAGS,
    REPLAY_INFO_PASSEDGATE,
} ReplayFrameInfoTypes;

typedef enum {
    REPLAY_FLAG_NONE    = 0,
    REPLAY_FLAG_INPUT   = 1 << 0,
    REPLAY_FLAG_POS     = 1 << 1,
    REPLAY_FLAG_VEL     = 1 << 2,
    REPLAY_FLAG_GIMMICK = 1 << 3,
    REPLAY_FLAG_DIR     = 1 << 4,
    REPLAY_FLAG_ROT     = 1 << 5,
    REPLAY_FLAG_ANIM    = 1 << 6,
    REPLAY_FLAG_FRAME   = 1 << 7,
} ReplayFrameFlagTypes;

// Object Class
struct ObjectReplayRecorder {
    RSDK_OBJECT
    void *actions[64];
    int32 frameCounter;
    Replay *writeBuffer;
    Replay *readBuffer;
    ReplayFrame *frameBuffer_w;
    ReplayFrame *frameBuffer_r;
    Entity *recorder_r;
    Entity *recorder_w;
    bool32 initialized;
    int32 startRecording;
    int32 startPlayback;
    int32 savedReplay;
    int32 startedRecording;
    int32 isReplaying;
    int32 hasSetupGhostView;
    int32 hasSetupGhostVS;
    int32 passedStartLine;
    int32 reachedGoal;
    int32 packedStartFrame;
    void *buffer;
    void (*loadCallback)(bool32);
    char filename[0x100];
    void (*saveFinishPtr)(bool32);
    int32 replayID;
    int32 replayRowID;
};

// Entity Class
struct EntityReplayRecorder {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateLate);
    StateMachine(playerState);
    StateMachine(storedState);
    StateMachine(stateStore);
    uint16 animID;
    uint16 frameID;
    EntityPlayer *player;
    int32 paused;
    int32 changeFlags;
    int32 playing;
    int32 replayFrame;
    int32 maxFrameCount;
    int32 replayStopDelay;
    uint8 storedInputs;
    uint8 storedDirection;
    Vector2 storedPos;
    Vector2 storedVel;
    int32 storedRotation;
    int32 storedSpeed;
    uint8 storedAnim;
    uint16 storedFrame;
    int32 alphaStore;
};

// Object Struct
extern ObjectReplayRecorder *ReplayRecorder;

// Standard Entity Events
void ReplayRecorder_Update(void);
void ReplayRecorder_LateUpdate(void);
void ReplayRecorder_StaticUpdate(void);
void ReplayRecorder_Draw(void);
void ReplayRecorder_Create(void* data);
void ReplayRecorder_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ReplayRecorder_EditorDraw(void);
void ReplayRecorder_EditorLoad(void);
#endif
void ReplayRecorder_Serialize(void);

// Extra Entity Functions
void ReplayRecorder_TitleCardCB(void);
void ReplayRecorder_Resume(EntityReplayRecorder *recorder);
void ReplayRecorder_StartCB(void);
void ReplayRecorder_FinishCB(void);
void ReplayRecorder_Buffer_Move(void);
void ReplayRecorder_SaveReplayDLG_NoCB(void);
void ReplayRecorder_SaveReplayDLG_YesCB(void);
void ReplayRecorder_Unknown6(void);
void ReplayRecorder_SaveReplay(void);
void ReplayRecorder_ReplaySaveFinish_CB(bool32 success);
void ReplayRecorder_ReplaySave_CB(bool32 success);
void ReplayRecorder_SaveTimeAttackDB_CB(bool32 success);
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
void ReplayRecorder_ForceApplyFramePtr(EntityReplayRecorder *recorder, ReplayFrame* framePtr);
void ReplayRecorder_ApplyFramePtr(EntityReplayRecorder *recorder, ReplayFrame *framePtr);
bool32 ReplayRecorder_CheckPlayerGimmickState(EntityReplayRecorder *recorder);
void ReplayRecorder_PackFrame(ReplayFrame *recording);
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
void ReplayRecorder_DeleteReplay(int32 row, void (*callback)(bool32), bool32 useAltCB);
void ReplayRecorder_DeleteReplay_CB(int32 status);
void ReplayRecorder_DeleteReplaySave_CB(int32 status);
void ReplayRecorder_DeleteReplaySave2_CB(int32 status);
void ReplayRecorder_SetStatus(int32 status);
void ReplayRecorder_ReplaySaveFinish(int32 status);
#endif

#endif //!OBJ_REPLAYRECORDER_H
