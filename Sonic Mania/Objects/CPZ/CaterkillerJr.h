#ifndef OBJ_CATERKILLERJR_H
#define OBJ_CATERKILLERJR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
} ObjectCaterkillerJr;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Vector2 headStartPos;
    Vector2 bodyPosition[8];
    Vector2 bodyVelocity[8];
    int32 bodyDirection[8];
    int32 bodyTimer[8];
    int32 boundL;
    int32 boundR;
    Animator bodyAnimators[8];
    Animator animator1;
    Animator animator2;
} EntityCaterkillerJr;

// Object Struct
extern ObjectCaterkillerJr *CaterkillerJr;

// Standard Entity Events
void CaterkillerJr_Update(void);
void CaterkillerJr_LateUpdate(void);
void CaterkillerJr_StaticUpdate(void);
void CaterkillerJr_Draw(void);
void CaterkillerJr_Create(void* data);
void CaterkillerJr_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void CaterkillerJr_EditorDraw(void);
void CaterkillerJr_EditorLoad(void);
#endif
void CaterkillerJr_Serialize(void);

// Extra Entity Functions
void CaterkillerJr_DebugDraw(void);
void CaterkillerJr_DebugSpawn(void);

void CaterkillerJr_SetupPositions(void);
void CaterkillerJr_State1(void);
void CaterkillerJr_State2(void);

#endif //!OBJ_CATERKILLERJR_H
