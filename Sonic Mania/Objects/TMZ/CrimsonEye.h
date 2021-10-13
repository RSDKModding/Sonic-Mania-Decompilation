#ifndef OBJ_CRIMSONEYE_H
#define OBJ_CRIMSONEYE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int value1[97],
          { 24,       0,  0, -0x28000, -0x28000, 1,  0, -0x14000, -0x28000, 2,  0, 0x14000,  -0x26000, 3,  0, 0x28000,  -0x26000, 4,  0, -0x26000,
            -0x24000, 5,  0, 0x26000,  -0x24000, 6,  0, -0x26000, -0x22000, 7,  0, 0x26000,  -0x22000, 8,  0, -0x24000, -0x20000, 9,  0, -0x12000,
            -0x20000, 10, 0, 0x12000,  -0x20000, 11, 0, 0x24000,  -0x20000, 12, 0, -0x22000, -0x1E000, 13, 0, -0x11000, -0x1E000, 14, 0, 0x11000,
            -0x1E000, 15, 0, 0x22000,  -0x1E000, 16, 0, -0x20000, -0x1C000, 17, 0, -0x10000, -0x1C000, 18, 0, 0x10000,  -0x1C000, 19, 0, 0x20000,
            -0x1C000, 20, 0, -0x1A000, -0x1A000, 21, 0, -0xD000,  -0x1A000, 22, 0, 0xD000,   -0x1A000, 23, 0, 0x1A000,  -0x1A000 });
    uint8 value2;
    uint8 value3;
    uint8 value4;
    uint8 value5;
    int value6;
    int value7;
    TextInfo value8;
    TextInfo value9;
    int value10[8]; //= { 1, 5, 2, 6, 3, 7, 4, 8 };
    int value11;
    int value12;
    int value13;
    int value14;
    int value15;
    int value16;
    uint8 value17;
    int value18;
    uint16 aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    Hitbox hitbox5;
    Hitbox hitbox6;
    Hitbox hitbox7;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxHover;
    uint16 sfxHover2;
    uint16 sfxElevator;
    uint16 sfxShot;
    uint16 sfxBeep;
    uint16 sfxHullClose;
    uint16 sfxButton;
    uint16 sfxImpact;
    TileLayer *liftBG;
    void *value38;
} ObjectCrimsonEye;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCrimsonEye;

// Object Struct
extern ObjectCrimsonEye *CrimsonEye;

// Standard Entity Events
void CrimsonEye_Update(void);
void CrimsonEye_LateUpdate(void);
void CrimsonEye_StaticUpdate(void);
void CrimsonEye_Draw(void);
void CrimsonEye_Create(void* data);
void CrimsonEye_StageLoad(void);
void CrimsonEye_EditorDraw(void);
void CrimsonEye_EditorLoad(void);
void CrimsonEye_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CRIMSONEYE_H
