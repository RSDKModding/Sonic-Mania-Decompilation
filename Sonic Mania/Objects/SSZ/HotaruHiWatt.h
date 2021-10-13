#ifndef OBJ_HOTARUHIWATT_H
#define OBJ_HOTARUHIWATT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[33],
          { 8, 0, 0,        -0x20000, -0x28000, 1, 0,       0x20000,  -0x28000, 2, 0,        -0x28000, -0x20000, 3, 0,       0x28000, -0x20000,
            4, 0, -0x28000, -0x18000, 5,        0, 0x28000, -0x18000, 6,        0, -0x20000, -0x10000, 7,        0, 0x20000, -0x10000 });
    int32 value2;
    int32 value3;
    int32 value4;
    int32 value5;
    int32 value6;
    int32 value7;
    uint8 value8;
    uint16 value9;
    uint16 value10;
    uint16 value11;
    uint16 value12;
    uint16 value13;
    uint16 value14;
    uint16 value15;
    uint16 value16;
    uint16 value17;
    uint16 value18;
    uint16 value19;
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
