#ifndef OBJ_HOTARUHIWATT_H
#define OBJ_HOTARUHIWATT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[33]; //= { 8, 0, 0, -131072, -163840, 1, 0, 131072, -163840, 2, 0, -163840, -131072, 3, 0, 163840, -131072, 4, 0, -163840, -98304, 5, 0, 163840, -98304, 6, 0, -131072, -65536, 7, 0, 131072, -65536 };
    int value2;
    int value3;
    int value4;
    int value5;
    int value6;
    int value7;
    byte value8;
    ushort value9;
    ushort value10;
    ushort value11;
    ushort value12;
    ushort value13;
    ushort value14;
    ushort value15;
    ushort value16;
    ushort value17;
    ushort value18;
    ushort value19;
} ObjectHotaruHiWatt;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHotaruHiWatt;

// Object Struct
extern ObjectHotaruHiWatt *HotaruHiWatt;

// Standard Entity Events
void HotaruHiWatt_Update();
void HotaruHiWatt_LateUpdate();
void HotaruHiWatt_StaticUpdate();
void HotaruHiWatt_Draw();
void HotaruHiWatt_Create(void* data);
void HotaruHiWatt_StageLoad();
void HotaruHiWatt_EditorDraw();
void HotaruHiWatt_EditorLoad();
void HotaruHiWatt_Serialize();

// Extra Entity Functions


#endif //!OBJ_HOTARUHIWATT_H
