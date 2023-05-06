#ifndef OBJ_ORBINAUT_H
#define OBJ_ORBINAUT_H

#include "Game.h"

#define ORBINAUT_ORB_COUNT (4)

// Object Class
struct ObjectOrbinaut {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxOrb;
    uint16 aniFrames;
};

// Entity Class
struct EntityOrbinaut {
    RSDK_ENTITY
    StateMachine(state);
    int32 planeFilter;
    Vector2 startPos;
    uint8 startDir;
    uint8 activeOrbs;
    Vector2 orbPositions[ORBINAUT_ORB_COUNT];
    bool32 fireOrbs;
    Animator animatorFace;
    Animator animatorOrb;
};

// Object Entity
extern ObjectOrbinaut *Orbinaut;

// Standard Entity Events
void Orbinaut_Update(void);
void Orbinaut_LateUpdate(void);
void Orbinaut_StaticUpdate(void);
void Orbinaut_Draw(void);
void Orbinaut_Create(void *data);
void Orbinaut_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Orbinaut_EditorDraw(void);
void Orbinaut_EditorLoad(void);
#endif
void Orbinaut_Serialize(void);

// Extra Entity Functions
void Orbinaut_DebugSpawn(void);
void Orbinaut_DebugDraw(void);

void Orbinaut_HandlePlayerInteractions(void);
void Orbinaut_HandleRotation(void);
void Orbinaut_CheckOffScreen(void);

void Orbinaut_State_Init(void);
void Orbinaut_State_Moving(void);
void Orbinaut_State_ReleasingOrbs(void);
void Orbinaut_State_Orbless(void);

void Orbinaut_State_Orb(void);
void Orbinaut_State_OrbDebris(void);

#endif //! OBJ_ORBINAUT_H
