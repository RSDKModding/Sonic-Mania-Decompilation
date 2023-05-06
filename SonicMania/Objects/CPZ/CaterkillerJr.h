#ifndef OBJ_CATERKILLERJR_H
#define OBJ_CATERKILLERJR_H

#include "Game.h"

#define CATERKILLERJR_SEGMENT_COUNT (7)

// Object Class
struct ObjectCaterkillerJr {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityCaterkillerJr {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Vector2 startPos;
    Vector2 bodyPosition[8];
    Vector2 bodyVelocity[8];
    int32 bodyDirection[8];
    int32 bodyTimer[8];
    int32 boundsL;
    int32 boundsR;
    Animator bodyAnimators[CATERKILLERJR_SEGMENT_COUNT];
    Animator smokePuffAnimators[3];
};

// Object Struct
extern ObjectCaterkillerJr *CaterkillerJr;

// Standard Entity Events
void CaterkillerJr_Update(void);
void CaterkillerJr_LateUpdate(void);
void CaterkillerJr_StaticUpdate(void);
void CaterkillerJr_Draw(void);
void CaterkillerJr_Create(void *data);
void CaterkillerJr_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CaterkillerJr_EditorDraw(void);
void CaterkillerJr_EditorLoad(void);
#endif
void CaterkillerJr_Serialize(void);

// Extra Entity Functions
void CaterkillerJr_DebugDraw(void);
void CaterkillerJr_DebugSpawn(void);

void CaterkillerJr_SetupPositions(void);
void CaterkillerJr_State_SetupVelocities(void);
void CaterkillerJr_State_Move(void);

#endif //! OBJ_CATERKILLERJR_H
