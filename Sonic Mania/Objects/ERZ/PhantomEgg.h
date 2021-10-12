#ifndef OBJ_PHANTOMEGG_H
#define OBJ_PHANTOMEGG_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[29]; //= { 4, 0, 0, -131072, -163840, 1, 0, -131072, -163840, 2, 0, 131072, -131072, 3, 0, 163840, -131072, 4, 0, -65536, -65536, 5, 0, 65536, -65536, 6, 0, -32768, -65536 };
    int value2[32]; //= { 0, 0, 0, 0, 1, 0, 0, 2, 0, 1, 3, 0, 2, 0, 0, 4, 0, 0, 1, 0, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 };
    int value3[32]; //= { 0, 0, 2, 0, 1, 3, 0, 2, 0, 0, 3, 0, 2, 0, 0, 3, 0, 0, 0, 1, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 };
    int value4[32]; //= { 0, 0, 2, 0, 1, 3, 3, 2, 0, 3, 3, 0, 2, 0, 0, 3, 0, 0, 0, 1, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 };
    int value5;
    int value6;
    int value7;
    int value8;
    int value9;
    int value10;
    int value11;
    int value12;
    int value13;
    int value14;
    int value15;
    bool32 setupPalette;
    bool32 value17;
    ushort sfxHit;
    ushort sfxExplosion2;
    ushort sfxRocketJet;
    ushort sfxExplosion3;
    ushort sfxJump;
    ushort sfxLand;
    ushort sfxRepel;
    ushort sfxShield;
    ushort sfxShock;
    ushort sfxSummon;
    ushort sfxMissile;
    ushort aniFrames;
    bool32 field_244;
} ObjectPhantomEgg;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomEgg;

// Object Struct
extern ObjectPhantomEgg *PhantomEgg;

// Standard Entity Events
void PhantomEgg_Update(void);
void PhantomEgg_LateUpdate(void);
void PhantomEgg_StaticUpdate(void);
void PhantomEgg_Draw(void);
void PhantomEgg_Create(void* data);
void PhantomEgg_StageLoad(void);
void PhantomEgg_EditorDraw(void);
void PhantomEgg_EditorLoad(void);
void PhantomEgg_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PHANTOMEGG_H
