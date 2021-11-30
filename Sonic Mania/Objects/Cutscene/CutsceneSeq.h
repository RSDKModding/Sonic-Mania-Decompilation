#ifndef OBJ_CUTSCENESEQ_H
#define OBJ_CUTSCENESEQ_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
typedef enum {
    SKIPTYPE_DISABLED,
    SKIPTYPE_RELOADSCN,
    SKIPTYPE_NEXTSCENE,
    SKIPTYPE_CALLBACK,
} SkipTypes;
#endif

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCutsceneSeq;

// Entity Class
typedef struct {
    RSDK_ENTITY
    bool32 (*currentState)(Entity *host);
    uint8 stateID;
    int32 timer;
    int32 storedValue1;
    int32 storedValue2;
    int32 values[8];
    Entity *cutsceneCurEntity;
    Entity *hostEntity;
    void *cutsceneStates[0x40];
    Vector2 points[8];
    int32 fillTimerA;
    int32 fillTimerB;
#if RETRO_USE_PLUS
    int32 skipType;
    void (*skipCallback)(void);
#endif
} EntityCutsceneSeq;

// Object Struct
extern ObjectCutsceneSeq *CutsceneSeq;

// Standard Entity Events
void CutsceneSeq_Update(void);
void CutsceneSeq_LateUpdate(void);
void CutsceneSeq_StaticUpdate(void);
void CutsceneSeq_Draw(void);
void CutsceneSeq_Create(void* data);
void CutsceneSeq_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void CutsceneSeq_EditorDraw(void);
void CutsceneSeq_EditorLoad(void);
#endif
void CutsceneSeq_Serialize(void);

// Extra Entity Functions
void CutsceneSeq_NewState(int32 nextState, EntityCutsceneSeq *seq);
#if RETRO_USE_PLUS
void CutsceneSeq_CheckSkip(uint8 skipType, EntityCutsceneSeq *seq, void (*skipCallback)(void));
#endif
Entity *CutsceneSeq_GetEntity(int32 type);
void CutsceneSeq_LockPlayerControl(void *plr);
void CutsceneSeq_LockAllPlayerControl(void);
void CutsceneSeq_StartSequence(Entity *host, void **states);

#endif //!OBJ_CUTSCENESEQ_H
