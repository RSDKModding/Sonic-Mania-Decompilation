#ifndef OBJ_METERDROID_H
#define OBJ_METERDROID_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[21]; //= { 5, 0, 0, -131072, -163840, 1, 0, 131072, -131072, 2, 0, -163840, -65536, 3, 0, 163840, -65536, 4, 0, 65536, 65536 };
    colour value2;
    colour value3;
    colour value4;
    int value5;
    int value6;
    int value7;
    int value8;
    int value9;
    ushort value10;
    ushort value11;
    ushort value12;
    ushort value13;
    ushort value14;
    ushort value15;
    ushort value16;
    ushort value17;
} ObjectMeterDroid;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMeterDroid;

// Object Struct
extern ObjectMeterDroid *MeterDroid;

// Standard Entity Events
void MeterDroid_Update();
void MeterDroid_LateUpdate();
void MeterDroid_StaticUpdate();
void MeterDroid_Draw();
void MeterDroid_Create(void* data);
void MeterDroid_StageLoad();
void MeterDroid_EditorDraw();
void MeterDroid_EditorLoad();
void MeterDroid_Serialize();

// Extra Entity Functions


#endif //!OBJ_METERDROID_H
