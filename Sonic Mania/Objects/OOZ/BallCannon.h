#ifndef OBJ_BALLCANNON_H
#define OBJ_BALLCANNON_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    ushort aniFrames;
    TABLE(int array1[32], { -0xC0000, -0xC0000, -0x40000, -0xC0000, 0x40000,  -0xC0000, 0xC0000, -0xC0000, -0xC0000, -0x40000, -0x40000,
                            -0x40000, 0x40000,  -0x40000, 0xC0000,  -0x40000, -0xC0000, 0x40000, -0x40000, 0x40000,  0x40000,  0x40000,
                            0xC0000,  0x40000,  -0xC0000, 0xC0000,  -0x40000, 0xC0000,  0x40000, 0xC0000,  0xC0000,  0xC0000 });
    TABLE(int array2[32], { -0x40000, -0x40000, -0x20000, -0x40000, 0x20000,  -0x40000, 0x40000, -0x40000, -0x3C000, -0x20000, -0x1C000,
                            -0x20000, 0x1C000,  -0x20000, 0x3C000,  -0x20000, -0x38000, 0x20000, -0x18000, 0x20000,  0x18000,  0x20000,
                            0x38000,  0x20000,  -0x34000, 0x40000,  -0x14000, 0x40000,  0x14000, 0x40000,  0x34000,  0x40000 });
    ushort sfxLedgeBreak;
    ushort sfxFire;
} ObjectBallCannon;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte type;
    bool32 exit;
    byte field_64;
    byte activePlayers;
    byte playerTimers[4];
    int field_6C;
    int rotationSpeed;
    Animator animator;
} EntityBallCannon;

// Object Struct
extern ObjectBallCannon *BallCannon;

// Standard Entity Events
void BallCannon_Update(void);
void BallCannon_LateUpdate(void);
void BallCannon_StaticUpdate(void);
void BallCannon_Draw(void);
void BallCannon_Create(void* data);
void BallCannon_StageLoad(void);
void BallCannon_EditorDraw(void);
void BallCannon_EditorLoad(void);
void BallCannon_Serialize(void);

// Extra Entity Functions
void BallCannon_Unknown1(void);
void BallCannon_Unknown2(void);
void BallCannon_Unknown3(void);
void BallCannon_Unknown4(void);
void BallCannon_Unknown5(void);
void BallCannon_StateCheckPlayerCollisions(void);
void BallCannon_Unknown7(void);
void BallCannon_Unknown8(void);

#endif //!OBJ_BALLCANNON_H
