#ifndef OBJ_KINGATTACK_H
#define OBJ_KINGATTACK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 laserColours[8], { 0x000020, 0x000020, 0xE850D8, 0xE850D8, 0xE850D8, 0xE850D8, 0x000020, 0x000020 });
    uint16 aniFrames;
    uint16 sfxLaserSweep;
    uint16 sfxLaserErupt;
    uint16 sfxElecPulse;
    uint16 sfxTwinShot;
    bool32 flag;
} ObjectKingAttack;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 timer;
    Entity *parent;
    Vector2 lineEndPos;
    Vector2 field_70;
    Vector2 laserVertPostions[8];
    int32 *laserColours;
    Hitbox hitbox;
    Animator animator;
} EntityKingAttack;

// Object Struct
extern ObjectKingAttack *KingAttack;

// Standard Entity Events
void KingAttack_Update(void);
void KingAttack_LateUpdate(void);
void KingAttack_StaticUpdate(void);
void KingAttack_Draw(void);
void KingAttack_Create(void* data);
void KingAttack_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void KingAttack_EditorDraw(void);
void KingAttack_EditorLoad(void);
#endif
void KingAttack_Serialize(void);

// Extra Entity Functions
bool32 KingAttack_ElecIdleCheckCB(void);
void KingAttack_CheckPlayerCollisions(void);
void KingAttack_HandleLaserPositions(void);

void KingAttack_State0_Unknown(void);
void KingAttack_State1_Unknown1(void);
void KingAttack_State1_Unknown2(void);
void KingAttack_State2_Unknown1(void);
void KingAttack_State2_Unknown2(void);
void KingAttack_State_Unknown(void);
void KingAttack_State3_Unknown1(void);
void KingAttack_State4_Unknown1(void);
void KingAttack_State5_Unknown(void);
void KingAttack_State4_Unknown2(void);
void KingAttack_State6_Unknown1(void);
void KingAttack_State6_Unknown2(void);

#endif //!OBJ_KINGATTACK_H
