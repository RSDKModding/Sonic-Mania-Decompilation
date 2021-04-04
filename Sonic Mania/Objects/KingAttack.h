#ifndef OBJ_KINGATTACK_H
#define OBJ_KINGATTACK_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[8]; //= { 32, 32, 15225048, 15225048, 15225048, 15225048, 32, 32 };
    ushort value2;
    ushort value3;
    ushort value4;
    ushort value5;
    ushort value6;
    byte value7;
} ObjectKingAttack;

// Entity Class
typedef struct {
	RSDK_ENTITY
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
void KingAttack_EditorDraw(void);
void KingAttack_EditorLoad(void);
void KingAttack_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_KINGATTACK_H
