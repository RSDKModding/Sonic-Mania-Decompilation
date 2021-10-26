#ifndef OBJ_KLEPTOMOBILE_H
#define OBJ_KLEPTOMOBILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[29], { 4,       0,        0, -0x20000, -0x28000, 1,        0, -0x20000, -0x28000, 2,        0, 0x20000, -0x20000, 3,       0,
                            0x28000, -0x20000, 4, 0,        -0x10000, -0x10000, 5, 0,        0x10000,  -0x10000, 6, 0,       -0x8000,  -0x10000 });
    int32 value2;
    int32 value3;
    int32 value4;
    int32 value5;
    int32 value6;
    uint16 value7;
    uint16 value8;
    uint16 value9;
    uint16 value10;
    uint16 value11;
    uint16 value12;
    uint16 value13;
    uint8 value14;
} ObjectKleptoMobile;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityKleptoMobile;

// Object Struct
extern ObjectKleptoMobile *KleptoMobile;

// Standard Entity Events
void KleptoMobile_Update(void);
void KleptoMobile_LateUpdate(void);
void KleptoMobile_StaticUpdate(void);
void KleptoMobile_Draw(void);
void KleptoMobile_Create(void* data);
void KleptoMobile_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void KleptoMobile_EditorDraw(void);
void KleptoMobile_EditorLoad(void);
#endif
void KleptoMobile_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_KLEPTOMOBILE_H
