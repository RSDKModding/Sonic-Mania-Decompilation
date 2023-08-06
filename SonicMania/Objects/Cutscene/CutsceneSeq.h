#ifndef OBJ_CUTSCENESEQ_H
#define OBJ_CUTSCENESEQ_H

#include "Game.h"

#define CUTSCENESEQ_POINT_COUNT (8)

#if MANIA_USE_PLUS
typedef enum {
    SKIPTYPE_DISABLED,
    SKIPTYPE_RELOADSCN,
    SKIPTYPE_NEXTSCENE,
    SKIPTYPE_CALLBACK,
} SkipTypes;
#endif

// Object Class
struct ObjectCutsceneSeq {
    RSDK_OBJECT
};

// Entity Class
struct EntityCutsceneSeq {
    RSDK_ENTITY
    bool32 (*currentState)(EntityCutsceneSeq *host);
    uint8 stateID;
    int32 timer;
    int32 storedValue; // never reset, unlike timer & the 8 values
    int32 storedTimer;
    int32 values[8];
    Entity *activeEntity;  // the entity that called StartSequence
    Entity *managerEntity; // the host entity of the sequence
    bool32 (*cutsceneStates[0x40])(EntityCutsceneSeq *host);
    Vector2 points[CUTSCENESEQ_POINT_COUNT];
    int32 fadeWhite;
    int32 fadeBlack;
#if MANIA_USE_PLUS
    int32 skipType;
    void (*skipCallback)(void);
#endif
};

// Object Struct
extern ObjectCutsceneSeq *CutsceneSeq;

// Standard Entity Events
void CutsceneSeq_Update(void);
void CutsceneSeq_LateUpdate(void);
void CutsceneSeq_StaticUpdate(void);
void CutsceneSeq_Draw(void);
void CutsceneSeq_Create(void *data);
void CutsceneSeq_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CutsceneSeq_EditorDraw(void);
void CutsceneSeq_EditorLoad(void);
#endif
void CutsceneSeq_Serialize(void);

// Extra Entity Functions
// Initializes a new state with ID of `nextState`
void CutsceneSeq_NewState(int32 nextState, EntityCutsceneSeq *seq);
#if MANIA_USE_PLUS
// Sets the cutscene's skip type (non-callback)
void CutsceneSeq_SetSkipType(uint8 type);
// Sets the cutscene's skip type to SKIPTYPE_CALLBACK and set the callback function
void CutsceneSeq_SetSkipTypeCallback(void (*callback)(void));
// Checks if the cutscene was skipped
void CutsceneSeq_CheckSkip(uint8 skipType, EntityCutsceneSeq *seq, void (*skipCallback)(void));
#endif
// Does a foreach loop for the entity of type `type`
Entity *CutsceneSeq_GetEntity(int32 type);
// Locks control of the selected player
void CutsceneSeq_LockPlayerControl(EntityPlayer *player);
// Locks Control of all players
void CutsceneSeq_LockAllPlayerControl(void);
// Sets up a cutscene sequence, the cutscene object should be passed as 'manager', then the cutscene states should be passed in order, make sure to
// end the states with StateMachine_None to tell it when to stop reading states
void CutsceneSeq_StartSequence(void *manager, ...);

#endif //! OBJ_CUTSCENESEQ_H
