#ifndef OBJ_HEAVYKING_H
#define OBJ_HEAVYKING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[32]; //= { 0, 0, 1, 0, 2, 2, 1, 0, 0, 1, 1, 2, 0, 2, 0, 0, 2, 1, 1, 2, 1, 0, 1, 0, 2, 2, 0, 1, 0, 0, 2, 1 };
    int value2;
    colour value3;
    colour value4;
    colour value5;
    int value6;
    int value7;
    int value8;
    int value9;
    int value10;
    ushort value11;
    ushort value12;
    ushort value13;
    ushort value14;
    ushort value15;
    ushort value16;
    ushort value17;
    ushort value18;
    ushort value19;
    ushort value20;
    ushort value21;
    ushort value22;
    ushort value23;
} ObjectHeavyKing;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHeavyKing;

// Object Struct
extern ObjectHeavyKing *HeavyKing;

// Standard Entity Events
void HeavyKing_Update();
void HeavyKing_LateUpdate();
void HeavyKing_StaticUpdate();
void HeavyKing_Draw();
void HeavyKing_Create(void* data);
void HeavyKing_StageLoad();
void HeavyKing_EditorDraw();
void HeavyKing_EditorLoad();
void HeavyKing_Serialize();

// Extra Entity Functions


#endif //!OBJ_HEAVYKING_H
