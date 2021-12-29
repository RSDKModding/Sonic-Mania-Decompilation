#ifndef OBJ_DRILLERDROID_H
#define OBJ_DRILLERDROID_H

#include "SonicMania.h"

typedef enum {
    DRILLERDROID_MAIN,
    DRILLERDROID_TARGET,
    DRILLERDROID_UNKNOWN,
}DrillerdroidTypes;

// Object Class
struct ObjectDrillerdroid {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    Hitbox hitbox5;
    int32 field_2C[2];
    int32 field_34[2];
    int32 field_3C[2];
    int32 field_44[2];
    int32 field_4C[2];
    int32 field_54[2];
    uint8 field_5C[5];
    uint8 field_61;
    uint8 playerTimers[4];
    uint8 shouldPlayDrillSfx;
    uint8 field_67;
    bool32 playingDrillSfx;
    uint8 armorHealth;
    uint8 field_6D;
    uint8 field_6E;
    uint8 field_6F;
    int32 field_70;
    Entity* boss;
    uint16 aniFrames;
    uint16 ticFrames;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDrill;
    uint16 sfxImpact;
    uint16 sfxJump;
    uint16 sfxTargeting;
    uint16 sfxSizzle;
    uint16 sfxDrop;
    uint16 sfxFail;
    uint16 sfxClang;
};

// Entity Class
struct EntityDrillerdroid {
    RSDK_ENTITY
    uint8 type;
    StateMachine(state);
    StateMachine(stateDraw);
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Entity* target;
    int32 field_B0;
    int32 field_B4;
    int32 startY;
    int32 timer;
    uint8 invincibilityTimer;
    int8 health;
};

// Object Struct
extern ObjectDrillerdroid *Drillerdroid;

// Standard Entity Events
void Drillerdroid_Update(void);
void Drillerdroid_LateUpdate(void);
void Drillerdroid_StaticUpdate(void);
void Drillerdroid_Draw(void);
void Drillerdroid_Create(void* data);
void Drillerdroid_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Drillerdroid_EditorDraw(void);
void Drillerdroid_EditorLoad(void);
#endif
void Drillerdroid_Serialize(void);

// Extra Entity Functions
void Drillerdroid_Hit(void);
void Drillerdroid_CheckPlayerCollisions(void);
void Drillerdroid_Explode(void);
void Drillerdroid_SpawnDebris(int offset);

void Drillerdroid_State_SetupArena(void);
void Drillerdroid_State_StartBoss(void);
void Drillerdroid_State_Unknown2(void);
void Drillerdroid_State_Unknown3(void);
void Drillerdroid_State_Unknown4(void);
void Drillerdroid_State_Unknown5(void);
void Drillerdroid_State_Unknown6(void);
void Drillerdroid_State_Unknown7(void);
void Drillerdroid_State_Unknown8(void);
void Drillerdroid_State_Unknown9(void);
void Drillerdroid_State_Unknown10(void);
void Drillerdroid_State_Unknown19(void);
void Drillerdroid_State_Unknown14(void);
void Drillerdroid_State_Unknown15(void);
void Drillerdroid_State_Unknown16(void);
void Drillerdroid_State_Unknown17(void);
void Drillerdroid_State_Unknown18(void);
void Drillerdroid_State_Unknown11(void);
void Drillerdroid_State_Unknown12(void);
void Drillerdroid_State_Unknown13(void);
void Drillerdroid_State_Destroyed(void);
void Drillerdroid_State_Finish(void);
void Drillerdroid_State_DropSignPost(void);

void Drillerdroid_StateDraw_Unknown1(void);

void Drillerdroid_StateDraw_Unknown2(void);

void Drillerdroid_State1_Unknown(void);
void Drillerdroid_StateDraw1_Unknown(void);

#endif //!OBJ_DRILLERDROID_H
