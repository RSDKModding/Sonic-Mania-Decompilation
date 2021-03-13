#ifndef OBJ_CUTSCENESEQ_H
#define OBJ_CUTSCENESEQ_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCutsceneSeq;

// Entity Class
typedef struct {
    RSDK_ENTITY
    bool32 (*currentState)(Entity *);
    byte stateID;
    int timer;
    int field_64;
    int field_68;
    int field_6C[8];
    Entity *cutsceneCurEntity;
    Entity *hostEntity;
    void *cutsceneStates[0x40];
    Vector2 points[8];
    int fillTimerA;
    int fillTimerB;
    int skipState;
    void (*skipCallback)();
} EntityCutsceneSeq;

// Object Struct
extern ObjectCutsceneSeq *CutsceneSeq;

// Standard Entity Events
void CutsceneSeq_Update();
void CutsceneSeq_LateUpdate();
void CutsceneSeq_StaticUpdate();
void CutsceneSeq_Draw();
void CutsceneSeq_Create(void* data);
void CutsceneSeq_StageLoad();
void CutsceneSeq_EditorDraw();
void CutsceneSeq_EditorLoad();
void CutsceneSeq_Serialize();

// Extra Entity Functions
Entity *CutsceneSeq_GetEntity(int type);
void CutsceneSeq_LockAllPlayerControl();
void CutsceneSeq_LockPlayerControl(void *plr);
void CutsceneSeq_CheckSkip(byte skipState, EntityCutsceneSeq *entity, void (*skipCallback)());
void CutsceneSeq_NewState(int nextState, EntityCutsceneSeq *CutsceneSeq);
void CutsceneSeq_StartSequence(Entity *host, void *states);

#endif //!OBJ_CUTSCENESEQ_H
