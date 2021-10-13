#ifndef OBJ_HEAVYKING_H
#define OBJ_HEAVYKING_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[32], { 0, 0, 1, 0, 2, 2, 1, 0, 0, 1, 1, 2, 0, 2, 0, 0, 2, 1, 1, 2, 1, 0, 1, 0, 2, 2, 0, 1, 0, 0, 2, 1 });
	int32 value2;
	Hitbox value3;
	Hitbox value4;
	Hitbox value5;
	int32 value6;
	int32 value7;
	int32 value8;
	int32 value9;
	int32 value10;
	uint16 value11;
	uint16 value12;
	uint16 value13;
	uint16 value14;
	uint16 value15;
	uint16 value16;
	uint16 value17;
	uint16 value18;
	uint16 value19;
	uint16 value20;
	uint16 value21;
	uint16 value22;
	uint16 value23;
} ObjectHeavyKing;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHeavyKing;

// Object Struct
extern ObjectHeavyKing *HeavyKing;

// Standard Entity Events
void HeavyKing_Update(void);
void HeavyKing_LateUpdate(void);
void HeavyKing_StaticUpdate(void);
void HeavyKing_Draw(void);
void HeavyKing_Create(void* data);
void HeavyKing_StageLoad(void);
void HeavyKing_EditorDraw(void);
void HeavyKing_EditorLoad(void);
void HeavyKing_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HEAVYKING_H
