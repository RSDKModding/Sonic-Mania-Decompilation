#ifndef OBJ_UFO_CIRCUIT_H
#define OBJ_UFO_CIRCUIT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int nodeCount;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    ushort field_28;
    ushort ufoModel;
    int field_2C;
    ushort emeraldModel;
    ushort sceneIndex;
} ObjectUFO_Circuit;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte mode;
    byte throttle;
    bool32 startNode;
    bool32 reverse;
    void *curNode;
    void *prevNode;
    void *nextNode;
    int angleY;
    int height;
    int field_7C;
    Vector2 startPos;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    Animator ufoData;
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
