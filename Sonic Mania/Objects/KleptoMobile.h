#ifndef OBJ_KLEPTOMOBILE_H
#define OBJ_KLEPTOMOBILE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[29]; //= { 4, 0, 0, -131072, -163840, 1, 0, -131072, -163840, 2, 0, 131072, -131072, 3, 0, 163840, -131072, 4, 0, -65536, -65536, 5, 0, 65536, -65536, 6, 0, -32768, -65536 };
    int value2;
    int value3;
    int value4;
    int value5;
    int value6;
    ushort value7;
    ushort value8;
    ushort value9;
    ushort value10;
    ushort value11;
    ushort value12;
    ushort value13;
    byte value14;
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
