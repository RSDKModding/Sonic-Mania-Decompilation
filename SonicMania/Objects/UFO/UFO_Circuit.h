#ifndef OBJ_UFO_CIRCUIT_H
#define OBJ_UFO_CIRCUIT_H

#include "Game.h"

typedef enum { UFO_CIRCUIT_MODE_UNUSED } UFO_CircuitModes;

typedef enum {
    UFO_CIRCUIT_THRTLE_INVALID,
    UFO_CIRCUIT_THRTLE_NONE,
    UFO_CIRCUIT_THRTLE_SLOW,
    UFO_CIRCUIT_THRTLE_MED,
    UFO_CIRCUIT_THRTLE_FAST,
} UFO_CircuitThrottles;

// Object Class
struct ObjectUFO_Circuit {
    RSDK_OBJECT
    int32 nodeCount;
    int32 decelerationNoMach;
    int32 decelerationMach;
    Animator unusedAnimator1;
    uint16 aniFrames; // unused
    uint16 ufoModel;
    int32 unused1;
    uint16 emeraldModel;
    uint16 sceneIndex;
};

// Entity Class
struct EntityUFO_Circuit {
    RSDK_ENTITY
    StateMachine(state);
    uint8 mode;
    uint8 throttle;
    bool32 startNode;
    bool32 reverse;
    EntityUFO_Circuit *curNode;
    EntityUFO_Circuit *nextNode;
    EntityUFO_Circuit *prevNode;
    int32 angleY;
    int32 height;
    int32 topSpeed;
    Vector2 startPos;
    Matrix unusedMatrix1;
    Matrix matTransform;
    Matrix matWorld;
    Matrix matNormal;
    Animator ufoAnimator;
};

// Object Struct
extern ObjectUFO_Circuit *UFO_Circuit;

// Standard Entity Events
void UFO_Circuit_Update(void);
void UFO_Circuit_LateUpdate(void);
void UFO_Circuit_StaticUpdate(void);
void UFO_Circuit_Draw(void);
void UFO_Circuit_Create(void *data);
void UFO_Circuit_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Circuit_EditorDraw(void);
void UFO_Circuit_EditorLoad(void);
#endif
void UFO_Circuit_Serialize(void);

// Extra Entity Functions
void UFO_Circuit_HandleSpeedSetup(void);
void UFO_Circuit_HandleNodeSpeeds(void);
bool32 UFO_Circuit_CheckNodeChange(void);
void UFO_Circuit_State_UFO(void);
void UFO_Circuit_State_Caught(void);

#endif //! OBJ_UFO_CIRCUIT_H
