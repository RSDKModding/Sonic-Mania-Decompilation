#ifndef OBJ_CUTSCENESEQ_H
#define OBJ_CUTSCENESEQ_H

#include "SonicMania.h"

typedef enum {
    SKIPTYPE_DISABLED,
    SKIPTYPE_RELOADSCN,
    SKIPTYPE_NEXTSCENE,
    SKIPTYPE_CALLBACK,
} SkipTypes;

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
    int32 field_64;
    int32 field_68;
    int32 field_6C[8];
    Entity *cutsceneCurEntity;
    Entity *hostEntity;
    void *cutsceneStates[0x40];
    Vector2 points[8];
    int32 fillTimerA;
    int32 fillTimerB;
    int32 skipType;
    void (*skipCallback)(void);
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
void CutsceneSeq_EditorDraw(void);
void CutsceneSeq_EditorLoad(void);
void CutsceneSeq_Serialize(void);

// Extra Entity Functions
Entity *CutsceneSeq_GetEntity(int32 type);
void CutsceneSeq_LockAllPlayerControl(void);
void CutsceneSeq_LockPlayerControl(void *plr);
void CutsceneSeq_CheckSkip(uint8 skipType, EntityCutsceneSeq *entity, void (*skipCallback)(void));
void CutsceneSeq_NewState(int32 nextState, EntityCutsceneSeq *CutsceneSeq);
void CutsceneSeq_StartSequence(Entity *host, void **states);

#endif //!OBJ_CUTSCENESEQ_H
