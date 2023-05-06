#ifndef OBJ_CATERKILLER_H
#define OBJ_CATERKILLER_H

#include "Game.h"

#define CATERKILLER_BODY_COUNT (3)

// Object Class
struct ObjectCaterkiller {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityCaterkiller {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 planeFilter;
    Vector2 bodyPosition[CATERKILLER_BODY_COUNT];
    uint8 bodyDirection[CATERKILLER_BODY_COUNT];
    uint8 bodyOffset[CATERKILLER_BODY_COUNT];
    uint8 headOffset;
    uint8 timer;
    Vector2 startPos;
    uint8 startDir;
    Animator headAnimator;
    Animator bodyAnimator;
};

// Object Entity
extern ObjectCaterkiller *Caterkiller;

// Standard Entity Events
void Caterkiller_Update(void);
void Caterkiller_LateUpdate(void);
void Caterkiller_StaticUpdate(void);
void Caterkiller_Draw(void);
void Caterkiller_Create(void *data);
void Caterkiller_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Caterkiller_EditorDraw(void);
void Caterkiller_EditorLoad(void);
#endif
void Caterkiller_Serialize(void);

// Extra Entity Functions
void Caterkiller_DebugSpawn(void);
void Caterkiller_DebugDraw(void);

void Caterkiller_CheckOffScreen(void);
void Caterkiller_CheckTileCollisions(void);

void Caterkiller_Draw_Body(void);
void Caterkiller_Draw_Segment(void);

void Caterkiller_HandlePlayerInteractions(void);
bool32 Caterkiller_CheckTileAngle(int32 x, int32 y, int32 dir);

// Body States
void Caterkiller_State_Init(void);
void Caterkiller_State_Contract(void);
void Caterkiller_State_LiftHead(void);
void Caterkiller_State_Uncontract(void);
void Caterkiller_State_LowerHead(void);

// Split States
void Caterkiller_StateSplit_Head(void);
void Caterkiller_StateSplit_Body(void);

#endif //! OBJ_CATERKILLER_H
