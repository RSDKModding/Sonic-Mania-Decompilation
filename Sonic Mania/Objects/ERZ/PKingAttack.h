#ifndef OBJ_PKINGATTACK_H
#define OBJ_PKINGATTACK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[8], { 0x000020, 0x000020, 0xE850D8, 0xE850D8, 0xE850D8, 0xE850D8, 0x000020, 0x000020 });
    uint16 value2;
    uint16 value3;
} ObjectPKingAttack;

// Entity Class
typedef struct {
	RSDK_ENTITY
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


#endif //!OBJ_PKINGATTACK_H
