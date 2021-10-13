#ifndef OBJ_PHANTOMHAND_H
#define OBJ_PHANTOMHAND_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int value1[41], { 10,       0, 0, -0x20000, -0x10000, 1, 0, -0x10000, -0x10000, 2, 0, 0x10000, -0x10000, 3, 0, 0x20000,
                            -0x10000, 4, 0, -0x20000, -0x8000,  5, 0, -0x10000, -0x8000,  6, 0, 0x10000, -0x8000,  7, 0, 0x20000,
                            0x8000,   8, 0, 0x20000,  -0x8000,  9, 0, 0x20000,  -0x4000 });
    byte value2;
    Hitbox value3;
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
