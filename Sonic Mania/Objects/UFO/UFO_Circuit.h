#ifndef OBJ_UFO_CIRCUIT_H
#define OBJ_UFO_CIRCUIT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 nodeCount;
    int32 field_8;
    int32 field_C;
    int32 field_10;
    int32 field_14;
    int32 field_18;
    int32 field_1C;
    int32 field_20;
    int32 field_24;
    uint16 field_28;
    uint16 ufoModel;
    int32 field_2C;
    uint16 emeraldModel;
    uint16 sceneIndex;
} ObjectUFO_Circuit;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 mode;
    uint8 throttle;
    bool32 startNode;
    bool32 reverse;
    void *curNode;
    void *nextNode;
    void *prevNode;
    int32 angleY;
    int32 height;
    int32 field_7C;
    Vector2 startPos;
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    int32 field_98;
    int32 field_9C;
    int32 field_A0;
    int32 field_A4;
    int32 field_A8;
    int32 field_AC;
    int32 field_B0;
    int32 field_B4;
    int32 field_B8;
    int32 field_BC;
    int32 field_C0;
    int32 field_C4;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    Animator animatorUFO;
} EntityUFO_Circuit;

// Object Struct
extern ObjectUFO_Circuit *UFO_Circuit;

// Standard Entity Events
void UFO_Circuit_Update(void);
void UFO_Circuit_LateUpdate(void);
void UFO_Circuit_StaticUpdate(void);
void UFO_Circuit_Draw(void);
void UFO_Circuit_Create(void* data);
void UFO_Circuit_StageLoad(void);
void UFO_Circuit_EditorDraw(void);
void UFO_Circuit_EditorLoad(void);
void UFO_Circuit_Serialize(void);

// Extra Entity Functions
void UFO_Circuit_Unknown1(void);
void UFO_Circuit_Unknown2(void);
bool32 UFO_Circuit_Unknown3(void);
void UFO_Circuit_Unknown4(void);
void UFO_Circuit_Unknown5(void);

#endif //!OBJ_UFO_CIRCUIT_H
