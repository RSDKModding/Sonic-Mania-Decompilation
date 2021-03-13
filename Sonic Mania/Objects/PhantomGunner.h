#ifndef OBJ_PHANTOMGUNNER_H
#define OBJ_PHANTOMGUNNER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[32]; //= { -22, -24, -10, -24, -22, -31, -10, -31, -22, -38, -10, -38, -22, -45, -10, -45, 10, -24, 22, -24, 10, -31, 22, -31, 10, -38, 22, -38, 10, -45, 22, -45 };
    int value2;
    colour value3;
    colour value4;
    colour value5;
    ushort value6;
    ushort value7;
} ObjectPhantomGunner;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomGunner;

// Object Struct
extern ObjectPhantomGunner *PhantomGunner;

// Standard Entity Events
void PhantomGunner_Update();
void PhantomGunner_LateUpdate();
void PhantomGunner_StaticUpdate();
void PhantomGunner_Draw();
void PhantomGunner_Create(void* data);
void PhantomGunner_StageLoad();
void PhantomGunner_EditorDraw();
void PhantomGunner_EditorLoad();
void PhantomGunner_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMGUNNER_H
