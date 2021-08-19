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
    int array1[32]; //= { -786432, -786432, -262144, -786432, 262144, -786432, 786432, -786432, -786432, -262144, -262144, -262144, 262144, -262144, 786432, -262144, -786432, 262144, -262144, 262144, 262144, 262144, 786432, 262144, -786432, 786432, -262144, 786432, 262144, 786432, 786432, 786432 };
    int array2[32]; //= { -262144, -262144, -131072, -262144, 131072, -262144, 262144, -262144, -245760, -131072, -114688, -131072, 114688, -131072, 245760, -131072, -229376, 131072, -98304, 131072, 98304, 131072, 229376, 131072, -212992, 262144, -81920, 262144, 81920, 262144, 212992, 262144 };
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
