#ifndef OBJ_PHANTOMKING_H
#define OBJ_PHANTOMKING_H

#include "SonicMania.h"

typedef enum {
    PHANTOMKING_KING,
    PHANTOMKING_ARM_L,
    PHANTOMKING_ARM_R,
} PhantomKingTypes;

// Object Class
struct ObjectPhantomKing {
    RSDK_OBJECT
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 aniFrames;
    bool32 defeated;
};

// Entity Class
struct EntityPhantomKing {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 originPos;
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 typeChangeTimer;
    int32 angle2;
    int32 frameIDs[10];
    Vector2 framePositions[10];
    int32 field_F8;
    int32 field_FC;
    Vector2 rubyPos;
    uint8 field_108;
    int32 field_10C;
    bool32 drawRuby;
    bool32 field_114;
    Entity *parent;
    int32 field_11C;
    int32 field_120;
    int32 field_124;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animatorRuby;
    Animator animator7;
    Animator animator8;
    Animator animator9;
    Animator animator10;
    Hitbox hitbox;
    uint8 storeDrawFX;
    int32 field_224;
};

// Object Struct
extern ObjectPhantomKing *PhantomKing;

// Standard Entity Events
void PhantomKing_Update(void);
void PhantomKing_LateUpdate(void);
void PhantomKing_StaticUpdate(void);
void PhantomKing_Draw(void);
void PhantomKing_Create(void* data);
void PhantomKing_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PhantomKing_EditorDraw(void);
void PhantomKing_EditorLoad(void);
#endif
void PhantomKing_Serialize(void);

// Extra Entity Functions
void PhantomKing_CheckPlayerCollisions(void);
void PhantomKing_Hit(void);
void PhantomKing_HandleFrames(void);
void PhantomKing_HandleAttacks(void);
void PhantomKing_SwitchToEggman(void);
void PhantomKing_SetupKing(EntityPhantomKing *king);

void PhantomKing_Draw_Body(void);
void PhantomKing_Draw_Arm(void);

void PhantomKing_State_Initialize(void);
void PhantomKing_State_SetupArms(void);
void PhantomKing_State_Unknown3(void);
void PhantomKing_State_Unknown4(void);
void PhantomKing_State_Unknown5(void);
void PhantomKing_State_Unknown6(void);
void PhantomKing_State_Unknown7(void);
void PhantomKing_State_Unknown8(void);
void PhantomKing_State_Switch(void);
void PhantomKing_State_HitFall(void);

void PhantomKing_StateArm1_Unknown2(void);
void PhantomKing_StateArm1_Unknown3(void);
void PhantomKing_Unknown21(void);
void PhantomKing_StateArm1_Unknown1(void);
void PhantomKing_StateArm1_Unknown4(void);

void PhantomKing_DestroyEntity(void);

void PhantomKing_State_Explode(void);
void PhantomKing_State_Destroyed(void);

#endif //!OBJ_PHANTOMKING_H
