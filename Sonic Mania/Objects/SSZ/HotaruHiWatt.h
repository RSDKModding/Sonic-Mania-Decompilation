#ifndef OBJ_HOTARUHIWATT_H
#define OBJ_HOTARUHIWATT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int value1[33],
          { 8, 0, 0,        -0x20000, -0x28000, 1, 0,       0x20000,  -0x28000, 2, 0,        -0x28000, -0x20000, 3, 0,       0x28000, -0x20000,
            4, 0, -0x28000, -0x18000, 5,        0, 0x28000, -0x18000, 6,        0, -0x20000, -0x10000, 7,        0, 0x20000, -0x10000 });
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
void HotaruHiWatt_Update(void);
void HotaruHiWatt_LateUpdate(void);
void HotaruHiWatt_StaticUpdate(void);
void HotaruHiWatt_Draw(void);
void HotaruHiWatt_Create(void* data);
void HotaruHiWatt_StageLoad(void);
void HotaruHiWatt_EditorDraw(void);
void HotaruHiWatt_EditorLoad(void);
void HotaruHiWatt_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HOTARUHIWATT_H
