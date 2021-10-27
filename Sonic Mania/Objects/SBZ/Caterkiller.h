#ifndef OBJ_CATERKILLER_H
#define OBJ_CATERKILLER_H

#include "SonicMania.h"

#define Caterkiller_BodyCount (3)

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
} ObjectCaterkiller;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 planeFilter;
    Vector2 bodyPosition[Caterkiller_BodyCount];
    uint8 bodyDirection[Caterkiller_BodyCount];
    uint8 bodyOffset[Caterkiller_BodyCount];
    uint8 headOffset;
    uint8 timer;
    Vector2 startPos;
    uint8 startDir;
    Animator animator;
    Animator animator2;
} EntityCaterkiller;

// Object Entity
extern ObjectCaterkiller *Caterkiller;

// Standard Entity Events
void Caterkiller_Update(void);
void Caterkiller_LateUpdate(void);
void Caterkiller_StaticUpdate(void);
void Caterkiller_Draw(void);
void Caterkiller_Create(void* data);
void Caterkiller_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Caterkiller_EditorDraw(void);
void Caterkiller_EditorLoad(void);
#endif
void Caterkiller_Serialize(void);

// Extra Entity Functions
void Caterkiller_DebugDraw(void);

void Caterkiller_DebugSpawn(void);

void Caterkiller_CheckOnScreen(void);
void Caterkiller_CheckTileCollisions(void);

void Caterkiller_StateDraw_Body(void);
void Caterkiller_StateDraw_Split(void);

void Caterkiller_HandlePlayerInteractions(void);
bool32 Caterkiller_CheckTileAngle(int32 x, int32 y, int32 dir);

//Body States
void Caterkiller_Unknown5(void);
void Caterkiller_Unknown6(void);
void Caterkiller_Unknown7(void);
void Caterkiller_Unknown8(void);
void Caterkiller_Unknown9(void);

//Split States
void Caterkiller_State_Split_Head(void);
void Caterkiller_State_Split_Body(void);

#endif //!OBJ_CATERKILLER_H
