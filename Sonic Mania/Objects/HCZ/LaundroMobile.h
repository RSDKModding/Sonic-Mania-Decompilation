#ifndef OBJ_LAUNDROMOBILE_H
#define OBJ_LAUNDROMOBILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint8 value1;
    uint8 value2;
    int32 value3;
    void *value4[6];
    uint8 value5;
    uint8 value6;
    uint8 value7;
    uint8 value8;
    int8 value9;
    TABLE(int32 value10[8], { 1, 2, 2, 2, 1, 2, 2, 3 });
    uint8 value11;
    int32 value12;
    uint8 value13;
    int32 value14[4];
    Vector2 value15[4];
    uint8 value16[4];
    int32 value17[2];
    int32 value18[4];
    int32 value19[4];
    uint8 value20;
    uint8 value21;
    void *value22;
    TABLE(int32 value23[73], { 12,       6,        0, -0x50000, -0x40000, 0, 0,        7,        0, 0x50000,  -0x40000, 0, 0,        10,       0,
                             -0x50000, -0x40000, 0, 0,        11,       0, 0x50000,  -0x40000, 0, 0,        10,       0, -0x50000, -0x40000, 0,
                             0x200000, 11,       0, 0x50000,  -0x40000, 0, 0x200000, 10,       0, -0x50000, -0x40000, 0, 0x400000, 11,       0,
                             0x50000,  -0x40000, 0, 0x400000, 8,        0, -0x50000, -0x40000, 0, 0x400000, 9,        0, 0x50000,  -0x40000, 0,
                             0x400000, 12,       0, -0x50000, -0x40000, 0, 0x400000, 13,       0, 0x50000,  -0x40000, 0, 0x400000 });
    uint16 value24;
    uint16 value25;
    Hitbox value26;
    Hitbox value27;
    Hitbox value28;
    Hitbox value29;
    Hitbox value30;
    Hitbox value31;
    uint16 value32;
    uint16 value33;
    uint16 value34;
    uint16 value35;
    uint16 value36;
    uint16 value37;
    uint16 value38;
    uint16 value39;
    uint16 value40;
    uint16 value41;
    uint16 value42;
    uint16 value43;
    uint16 value44;
    uint16 value45;
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
#if RETRO_INCLUDE_EDITOR
void LaundroMobile_EditorDraw(void);
void LaundroMobile_EditorLoad(void);
#endif
void LaundroMobile_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_LAUNDROMOBILE_H
