#ifndef OBJ_LOVETESTER_H
#define OBJ_LOVETESTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 aniFrames;
    uint16 sfxScore;
    Vector2 field_20;
    Vector2 field_28;
    Vector2 positions[10];
} ObjectLoveTester;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateLate);
    uint8 activePlayers;
    Vector2 storedPos;
    Entity *playerPtr;
    int32 field_70;
    int32 field_74;
    uint8 playerID;
    uint8 field_79;
    uint8 field_7A;
    uint8 field_7B;
    int32 field_7C;
    uint8 field_80;
    uint8 field_81;
    uint8 field_82;
    uint8 field_83;
    int32 timer;
    int32 field_88;
    Animator animator1;
    Animator animators[10];
} EntityLoveTester;

// Object Struct
extern ObjectLoveTester *LoveTester;

// Standard Entity Events
void LoveTester_Update(void);
void LoveTester_LateUpdate(void);
void LoveTester_StaticUpdate(void);
void LoveTester_Draw(void);
void LoveTester_Create(void* data);
void LoveTester_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LoveTester_EditorDraw(void);
void LoveTester_EditorLoad(void);
#endif
void LoveTester_Serialize(void);

// Extra Entity Functions
void LoveTester_SetupHitboxes(void);
void LoveTester_SetupPositions(void);
void LoveTester_DrawSprites(void);
void LoveTester_Unknown2(uint8 list, bool32 flag, uint8 frame);
void LoveTester_CheckPlayerCollisions(void);
void LoveTester_CheckPlayerCollisions2(bool32 flag);
void LoveTester_GiveScore(void *p);
void LoveTester_Unknown6(void);

void LoveTester_State_Unknown1(void);
void LoveTester_State_Unknown2(void);
void LoveTester_State_Unknown3(void);
void LoveTester_State_Unknown4(void);
void LoveTester_State_Unknown5(void);
void LoveTester_State_Unknown6(void);
void LoveTester_State_Unknown7(void);
void LoveTester_State_Unknown8(void);

void LoveTester_State2_Unknown(void);

void LoveTester_StateLate_Unknown1(void);
void LoveTester_StateLate_Unknown2(void);
void LoveTester_StateLate_Unknown3(void);

#endif //!OBJ_LOVETESTER_H
