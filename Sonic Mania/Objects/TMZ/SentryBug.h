#ifndef OBJ_SENTRYBUG_H
#define OBJ_SENTRYBUG_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int value1[84], { -0x50000, 0x30000, -0x90000, 0xA0000, -0x70000, 0x100000, 0x70000,  0x30000, 0x30000,  0xA0000, -0x30000, 0x100000,
                            -0x70000, 0x30000, -0xB0000, 0xA0000, -0x80000, 0x100000, 0x90000,  0x30000, 0x60000,  0xA0000, 0,        0x100000,
                            -0x90000, 0x30000, -0xB0000, 0xA0000, -0x80000, 0x100000, 0x90000,  0x30000, 0x80000,  0xA0000, 0x30000,  0x100000,
                            -0x90000, 0x30000, -0xB0000, 0xA0000, -0x80000, 0x100000, 0x90000,  0x30000, 0xB0000,  0xA0000, 0x80000,  0x100000,
                            0x90000,  0x30000, 0xB0000,  0xA0000, 0x80000,  0x100000, -0x90000, 0x30000, -0x80000, 0xA0000, -0x30000, 0x100000,
                            0x70000,  0x30000, 0xB0000,  0xA0000, 0x80000,  0x100000, -0x90000, 0x30000, -0x60000, 0xA0000, 0,        0x100000,
                            0x50000,  0x30000, 0x90000,  0xA0000, 0x70000,  0x100000, -0x70000, 0x30000, -0x30000, 0xA0000, 0x30000,  0x100000 });
    TABLE(int value2[12], { -0x10000, -0x10000, -0x20000, -0x8000, -0x10000, 0x8000, 0x10000, -0x10000, 0x20000, -0x8000, 0x10000, 0x8000 });
    TABLE(int value3[12], { 0x10000, -0x10000, 0x20000, -0x8000, 0x10000, 0x8000, -0x10000, -0x10000, -0x20000, -0x8000, -0x10000, 0x8000 });
    Hitbox hitbox;
    uint16 meshFrames;
    uint16 sceneIndex;
    uint16 aniFrames;
    uint16 sfxPon;
    uint16 sfxSwarm;
} ObjectSentryBug;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateLate);
    uint8 speed;
    Vector2 amplitude;
    bool32 drawFlag;
    bool32 drawNet;
    int timer;
    Vector2 field_78;
    Vector2 field_80;
    EntityPlayer *playerPtr;
    int netX;
    int netY;
    int netRotation;
    int netScale;
    int netColour;
    Vector2 positions[6];
    Vector2 positions2[6];
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    Matrix matrix1;
    Matrix matrix2;
    Animator animator2;
    Animator animator1;
} EntitySentryBug;

// Object Struct
extern ObjectSentryBug *SentryBug;

// Standard Entity Events
void SentryBug_Update(void);
void SentryBug_LateUpdate(void);
void SentryBug_StaticUpdate(void);
void SentryBug_Draw(void);
void SentryBug_Create(void* data);
void SentryBug_StageLoad(void);
void SentryBug_EditorDraw(void);
void SentryBug_EditorLoad(void);
void SentryBug_Serialize(void);

// Extra Entity Functions
void SentryBug_DebugSpawn(void);
void SentryBug_DebugDraw(void);

void SentryBug_CheckPlayerCollisions(void);

void SentryBug_StateLate_Unknown1(void);
void SentryBug_SetupPositions2(void);
void SentryBug_StateLate_Unknown2(void);
void SentryBug_StateLate_Unknown3(void);
void SentryBug_StateLate_Unknown4(void);
void SentryBug_StateLate_Unknown5(void);

void SentryBug_State_Unknown1(void);
void SentryBug_State_Unknown2(void);
void SentryBug_State_Unknown3(void);
void SentryBug_State_Unknown4(void);
void SentryBug_State_Unknown5(void);
void SentryBug_State_Unknown6(void);

#endif //!OBJ_SENTRYBUG_H
