#ifndef OBJ_PHANTOMHAND_H
#define OBJ_PHANTOMHAND_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[41]; //= { 10, 0, 0, -131072, -65536, 1, 0, -65536, -65536, 2, 0, 65536, -65536, 3, 0, 131072, -65536, 4, 0, -131072, -32768, 5, 0, -65536, -32768, 6, 0, 65536, -32768, 7, 0, 131072, 32768, 8, 0, 131072, -32768, 9, 0, 131072, -16384 };
    byte value2;
    colour value3;
    ushort value4;
} ObjectPhantomHand;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomHand;

// Object Struct
extern ObjectPhantomHand *PhantomHand;

// Standard Entity Events
void PhantomHand_Update(void);
void PhantomHand_LateUpdate(void);
void PhantomHand_StaticUpdate(void);
void PhantomHand_Draw(void);
void PhantomHand_Create(void* data);
void PhantomHand_StageLoad(void);
void PhantomHand_EditorDraw(void);
void PhantomHand_EditorLoad(void);
void PhantomHand_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PHANTOMHAND_H
