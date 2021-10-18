#ifndef OBJ_PKINGATTACK_H
#define OBJ_PKINGATTACK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[8], { 0x000020, 0x000020, 0xE850D8, 0xE850D8, 0xE850D8, 0xE850D8, 0x000020, 0x000020 });
    uint16 sfxPulse;
    uint16 aniFrames;
} ObjectPKingAttack;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 timer;
    Entity *target;
    Vector2 targetPos;
    Vector2 field_70;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    int32 field_98;
    int32 field_9C;
    int32 field_A0;
    int32 field_A4;
    int32 field_A8;
    int32 field_AC;
    int32 field_B0;
    int32 field_B4;
    int32 field_B8;
    Hitbox hitbox;
    Animator animator;
} EntityPKingAttack;

// Object Struct
extern ObjectPKingAttack *PKingAttack;

// Standard Entity Events
void PKingAttack_Update(void);
void PKingAttack_LateUpdate(void);
void PKingAttack_StaticUpdate(void);
void PKingAttack_Draw(void);
void PKingAttack_Create(void* data);
void PKingAttack_StageLoad(void);
void PKingAttack_EditorDraw(void);
void PKingAttack_EditorLoad(void);
void PKingAttack_Serialize(void);

// Extra Entity Functions
void PKingAttack_CheckPlayerCollisions(void);

void PKingAttack_Unknown2(void);
void PKingAttack_Unknown3(void);
void PKingAttack_Unknown4(void);
void PKingAttack_Unknown5(void);
void PKingAttack_Unknown6(void);

#endif //!OBJ_PKINGATTACK_H
