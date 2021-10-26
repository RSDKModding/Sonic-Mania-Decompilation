#ifndef OBJ_PHANTOMGUNNER_H
#define OBJ_PHANTOMGUNNER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[32], { -22, -24, -10, -24, -22, -31, -10, -31, -22, -38, -10, -38, -22, -45, -10, -45,
                            10,  -24, 22,  -24, 10,  -31, 22,  -31, 10,  -38, 22,  -38, 10,  -45, 22,  -45 });
    int32 value2;
    Hitbox value3;
    Hitbox value4;
    Hitbox value5;
    uint16 value6;
    uint16 value7;
} ObjectPhantomGunner;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomGunner;

// Object Struct
extern ObjectPhantomGunner *PhantomGunner;

// Standard Entity Events
void PhantomGunner_Update(void);
void PhantomGunner_LateUpdate(void);
void PhantomGunner_StaticUpdate(void);
void PhantomGunner_Draw(void);
void PhantomGunner_Create(void* data);
void PhantomGunner_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PhantomGunner_EditorDraw(void);
void PhantomGunner_EditorLoad(void);
#endif
void PhantomGunner_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PHANTOMGUNNER_H
