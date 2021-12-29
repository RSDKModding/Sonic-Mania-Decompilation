#ifndef OBJ_GACHAPANDORA_H
#define OBJ_GACHAPANDORA_H

#include "SonicMania.h"

#define Gachapandora_CapsuleCount (4)

typedef enum {
    GACHAPANDORA_MAIN,
    GACHAPANDORA_1,
    GACHAPANDORA_PRIZE,
    GACHAPANDORA_AMY,
    GACHAPANDORA_DRILLER,
    GACHAPANDORA_FIREDROPPER,
    GACHAPANDORA_FIREBALL,
    GACHAPANDORA_DEBRIS,
    GACHAPANDORA_SPARK,
}GachapandoraTypes;

// Object Class
struct ObjectGachapandora {
    RSDK_OBJECT
    TABLE(int32 capsuleOffsets[56],
          { 37, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 49, 50, 50, 50, 51, 51, 51, 51, 52, 52, 52, 52, 53, 53, 53, 53,
                            54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 53, 53, 53, 52, 52, 52, 52, 51, 51, 51, 50, 50, 50, 50, 49, 49, 48, 48 });
    uint8 activeToys;
    uint8 handleDir;
    uint8 value4;
    uint8 capsuleOffsetL[4];
    uint8 capsuleSpeedL[4];
    uint8 capsuleMaxL[4];
    uint8 capsuleTypeL[4];
    uint8 capsuleDelayL[4];
    uint8 capsuleOffsetR[4];
    uint8 capsuleSpeedR[4];
    uint8 capsuleMaxR[4];
    uint8 capsuleTypeR[4];
    uint8 capsuleDelayR[4];
    int16 value15;
    uint8 nextChildType;
    uint8 nextCapsuleL;
    uint8 nextCapsuleR;
    uint8 value19;
    uint8 value20[3];
    uint8 capsuleSide;
    uint8 value22;
    uint8 value23[2];
    int32 value24;
    int32 value25;
    uint8 value26;
    uint8 value27;
    Entity *eggman;
    uint16 aniFrames;
    uint16 eggmanFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    Hitbox hitbox5;
    Hitbox hitbox6;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxUnravel;
    uint16 sfxPon;
    uint16 sfxFireball;
    uint16 sfxGiggle;
};

// Entity Class
struct EntityGachapandora {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 field_60;
    uint8 type;
    uint16 shakeTimer;
    uint16 shakeCount;
    uint16 prevShakeFlags;
    int32 timer;
    int32 startY;
    int32 invincibilityTimer;
    uint8 health;
    Vector2 posUnknown;
    Entity *parent;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Hitbox hitbox;
};

// Object Struct
extern ObjectGachapandora *Gachapandora;

// Standard Entity Events
void Gachapandora_Update(void);
void Gachapandora_LateUpdate(void);
void Gachapandora_StaticUpdate(void);
void Gachapandora_Draw(void);
void Gachapandora_Create(void* data);
void Gachapandora_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Gachapandora_EditorDraw(void);
void Gachapandora_EditorLoad(void);
#endif
void Gachapandora_Serialize(void);

// Extra Entity Functions
void Gachapandora_CheckPlayerCollisions(void);
void Gachapandora_Explode(int xMax, int xMin, int yMin, int yMax);

void Gachapandora_Player_StateInput_P1Grabbed(void);
void Gachapandora_Player_StateInput_P2PlayerGrabbed(void);
void Gachapandora_Player_StateInput_P2AIGrabbed(void);

void Gachapandora_HandleChildren(void);
void Gachapandora_HandleAnimations(void);

void Gachapandora_State_SetupArena(void);
void Gachapandora_State_Unknown1(void);
void Gachapandora_State_Unknown2(void);
void Gachapandora_State_Unknown3(void);
void Gachapandora_State_Unknown4(void);
void Gachapandora_State_Unknown5(void);
void Gachapandora_State_Unknown6(void);
void Gachapandora_State_Unknown7(void);
void Gachapandora_State_Unknown8(void);
void Gachapandora_State_Unknown9(void);
void Gachapandora_State_Unknown10(void);
void Gachapandora_State_Finish(void);
void Gachapandora_StateDraw_Unknown1(void);
void Gachapandora_StateDraw_Unknown2(void);

void Gachapandora_State2_Unknown1(void);
void Gachapandora_State2_Unknown2(void);
void Gachapandora_StateDraw2_Unknown(void);
void Gachapandora_State_Destroyed(void);
void Gachapandora_State2_Unknown3(void);
void Gachapandora_State2_Unknown4(void);
void Gachapandora_State2_Unknown5(void);

void Gachapandora_State6_Unknown1(void);
void Gachapandora_State6_Unknown2(void);

void Gachapandora_StateDraw8_Unknown(void);

void Gachapandora_State2_Unknown7(void);
void Gachapandora_State2_Unknown6(void);
void Gachapandora_State2_Unknown8(void);
void Gachapandora_State2_Unknown9(void);
void Gachapandora_State2_Unknown10(void);

void Gachapandora_State7_Unknown1(void);
void Gachapandora_State7_Unknown2(void);
void Gachapandora_State7_Unknown3(void);
void Gachapandora_StateDraw7_Unknown1(void);

void Gachapandora_State8_Unknown1(void);
void Gachapandora_State8_Unknown2(void);

void Gachapandora_State1_Unknown1(void);
void Gachapandora_State1_Unknown2(void);
void Gachapandora_State1_Unknown3(void);

#endif //!OBJ_GACHAPANDORA_H
