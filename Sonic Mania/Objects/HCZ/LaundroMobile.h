#ifndef OBJ_LAUNDROMOBILE_H
#define OBJ_LAUNDROMOBILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    byte value1;
    byte value2;
    int value3;
    void *value4[6];
    byte value5;
    byte value6;
    byte value7;
    byte value8;
    sbyte value9;
    TABLE(int value10[8], { 1, 2, 2, 2, 1, 2, 2, 3 });
    byte value11;
    int value12;
    byte value13;
    int value14[4];
    Vector2 value15[4];
    byte value16[4];
    int value17[2];
    int value18[4];
    int value19[4];
    byte value20;
    byte value21;
    void *value22;
    TABLE(int value23[73], { 12,       6,        0, -0x50000, -0x40000, 0, 0,        7,        0, 0x50000,  -0x40000, 0, 0,        10,       0,
                             -0x50000, -0x40000, 0, 0,        11,       0, 0x50000,  -0x40000, 0, 0,        10,       0, -0x50000, -0x40000, 0,
                             0x200000, 11,       0, 0x50000,  -0x40000, 0, 0x200000, 10,       0, -0x50000, -0x40000, 0, 0x400000, 11,       0,
                             0x50000,  -0x40000, 0, 0x400000, 8,        0, -0x50000, -0x40000, 0, 0x400000, 9,        0, 0x50000,  -0x40000, 0,
                             0x400000, 12,       0, -0x50000, -0x40000, 0, 0x400000, 13,       0, 0x50000,  -0x40000, 0, 0x400000 });
    ushort value24;
    ushort value25;
    Hitbox value26;
    Hitbox value27;
    Hitbox value28;
    Hitbox value29;
    Hitbox value30;
    Hitbox value31;
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
void LaundroMobile_Create(void *data);
void LaundroMobile_StageLoad(void);
void LaundroMobile_EditorDraw(void);
void LaundroMobile_EditorLoad(void);
void LaundroMobile_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_LAUNDROMOBILE_H
