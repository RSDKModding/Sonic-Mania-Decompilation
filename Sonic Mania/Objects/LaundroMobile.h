#ifndef OBJ_LAUNDROMOBILE_H
#define OBJ_LAUNDROMOBILE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    byte value1;
    byte value2;
    int value3;
    bool32 value4;
    byte value5;
    byte value6;
    byte value7;
    byte value8;
    byte value9;
    int value10[8]; //= { 1, 2, 2, 2, 1, 2, 2, 3 };
    byte value11;
    int value12;
    byte value13;
    int value14;
    TextInfo value15;
    byte value16;
    int value17;
    int value18;
    int value19;
    byte value20;
    byte value21;
    bool32 value22;
    int value23[73]; //= { 12, 6, 0, -327680, -262144, 0, 0, 7, 0, 327680, -262144, 0, 0, 10, 0, -327680, -262144, 0, 0, 11, 0, 327680, -262144, 0, 0, 10, 0, -327680, -262144, 0, 2097152, 11, 0, 327680, -262144, 0, 2097152, 10, 0, -327680, -262144, 0, 4194304, 11, 0, 327680, -262144, 0, 4194304, 8, 0, -327680, -262144, 0, 4194304, 9, 0, 327680, -262144, 0, 4194304, 12, 0, -327680, -262144, 0, 4194304, 13, 0, 327680, -262144, 0, 4194304 };
    ushort value24;
    ushort value25;
    colour value26;
    colour value27;
    colour value28;
    colour value29;
    colour value30;
    colour value31;
    ushort value32;
    ushort value33;
    ushort value34;
    ushort value35;
    ushort value36;
    ushort value37;
    ushort value38;
    ushort value39;
    ushort value40;
    ushort value41;
    ushort value42;
    ushort value43;
    ushort value44;
    ushort value45;
} ObjectLaundroMobile;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLaundroMobile;

// Object Struct
extern ObjectLaundroMobile *LaundroMobile;

// Standard Entity Events
void LaundroMobile_Update(void);
void LaundroMobile_LateUpdate(void);
void LaundroMobile_StaticUpdate(void);
void LaundroMobile_Draw(void);
void LaundroMobile_Create(void* data);
void LaundroMobile_StageLoad(void);
void LaundroMobile_EditorDraw(void);
void LaundroMobile_EditorLoad(void);
void LaundroMobile_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LAUNDROMOBILE_H
