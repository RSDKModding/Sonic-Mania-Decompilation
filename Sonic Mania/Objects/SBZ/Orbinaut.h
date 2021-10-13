#ifndef OBJ_ORBINAUT_H
#define OBJ_ORBINAUT_H

#include "SonicMania.h"

#define Orbinaut_MaxOrbs (4)

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
} ObjectOrbinaut;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int planeFilter;
    Vector2 startPos;
    uint8 startDir;
    uint8 activeOrbs;
    Vector2 orbPositions[Orbinaut_MaxOrbs];
    bool32 fireOrbs;
    Animator animatorFace;
    Animator animatorOrb;
} EntityOrbinaut;

// Object Entity
extern ObjectOrbinaut *Orbinaut;

// Standard Entity Events
void Orbinaut_Update(void);
void Orbinaut_LateUpdate(void);
void Orbinaut_StaticUpdate(void);
void Orbinaut_Draw(void);
void Orbinaut_Create(void* data);
void Orbinaut_StageLoad(void);
void Orbinaut_EditorDraw(void);
void Orbinaut_EditorLoad(void);
void Orbinaut_Serialize(void);

// Extra Entity Functions
void Orbinaut_DebugSpawn(void);
void Orbinaut_DebugDraw(void);

void Orbinaut_HandlePlayerInteractions(void);
void Orbinaut_HandleRotation(void);
void Orbinaut_CheckOnScreen(void);

void Orbinaut_State_Setup(void);
void Orbinaut_Unknown6(void);
void Orbinaut_Unknown7(void);
void Orbinaut_Unknown8(void);
void Orbinaut_State_Orb(void);
void Orbinaut_Unknown10(void);

#endif //!OBJ_ORBINAUT_H
