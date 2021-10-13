#ifndef OBJ_KLEPTOMOBILE_H
#define OBJ_KLEPTOMOBILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int value1[29], { 4,       0,        0, -0x20000, -0x28000, 1,        0, -0x20000, -0x28000, 2,        0, 0x20000, -0x20000, 3,       0,
                            0x28000, -0x20000, 4, 0,        -0x10000, -0x10000, 5, 0,        0x10000,  -0x10000, 6, 0,       -0x8000,  -0x10000 });
    int value2;
    int value3;
    int value4;
    int value5;
    int value6;
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
void KleptoMobile_EditorDraw(void);
void KleptoMobile_EditorLoad(void);
void KleptoMobile_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_KLEPTOMOBILE_H
