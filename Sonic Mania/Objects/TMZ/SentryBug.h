#ifndef OBJ_SENTRYBUG_H
#define OBJ_SENTRYBUG_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[84], { -0x50000, 0x30000, -0x90000, 0xA0000, -0x70000, 0x100000, 0x70000,  0x30000, 0x30000,  0xA0000, -0x30000, 0x100000,
                            -0x70000, 0x30000, -0xB0000, 0xA0000, -0x80000, 0x100000, 0x90000,  0x30000, 0x60000,  0xA0000, 0,        0x100000,
                            -0x90000, 0x30000, -0xB0000, 0xA0000, -0x80000, 0x100000, 0x90000,  0x30000, 0x80000,  0xA0000, 0x30000,  0x100000,
                            -0x90000, 0x30000, -0xB0000, 0xA0000, -0x80000, 0x100000, 0x90000,  0x30000, 0xB0000,  0xA0000, 0x80000,  0x100000,
                            0x90000,  0x30000, 0xB0000,  0xA0000, 0x80000,  0x100000, -0x90000, 0x30000, -0x80000, 0xA0000, -0x30000, 0x100000,
                            0x70000,  0x30000, 0xB0000,  0xA0000, 0x80000,  0x100000, -0x90000, 0x30000, -0x60000, 0xA0000, 0,        0x100000,
                            0x50000,  0x30000, 0x90000,  0xA0000, 0x70000,  0x100000, -0x70000, 0x30000, -0x30000, 0xA0000, 0x30000,  0x100000 });
    TABLE(int32 value2[12], { -0x10000, -0x10000, -0x20000, -0x8000, -0x10000, 0x8000, 0x10000, -0x10000, 0x20000, -0x8000, 0x10000, 0x8000 });
    TABLE(int32 value3[12], { 0x10000, -0x10000, 0x20000, -0x8000, 0x10000, 0x8000, -0x10000, -0x10000, -0x20000, -0x8000, -0x10000, 0x8000 });
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
    int32 timer;
    Vector2 field_78;
    Vector2 field_80;
    EntityPlayer *playerPtr;
    int32 netX;
    int32 netY;
    int32 netRotation;
    int32 netScale;
    int32 netColour;
    Vector2 positions[6];
    Vector2 positions2[6];
    int32 field_100;
    int32 field_104;
    int32 field_108;
    int32 field_10C;
    int32 field_110;
    int32 field_114;
    int32 field_118;
    int32 field_11C;
    int32 field_120;
    int32 field_124;
    int32 field_128;
    int32 field_12C;
    int32 field_130;
    int32 field_134;
    int32 field_138;
    int32 field_13C;
    int32 field_140;
    int32 field_144;
    int32 field_148;
    int32 field_14C;
    int32 field_150;
    int32 field_154;
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
#if RETRO_INCLUDE_EDITOR
void SentryBug_EditorDraw(void);
void SentryBug_EditorLoad(void);
#endif
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
