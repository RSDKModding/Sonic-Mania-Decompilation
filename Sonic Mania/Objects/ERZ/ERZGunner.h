#ifndef OBJ_ERZGUNNER_H
#define OBJ_ERZGUNNER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int value1[32], { -22, -24, -10, -24, -22, -31, -10, -31, -22, -38, -10, -38, -22, -45, -10, -45, 10, -24, 22, -24, 10, -31, 22, -31, 10, -38, 22, -38, 10, -45, 22, -45 });
    int value2;
    colour value3;
    colour value4;
    colour value5;
    uint16 value6;
} ObjectERZGunner;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityERZGunner;

// Object Struct
extern ObjectERZGunner *ERZGunner;

// Standard Entity Events
void ERZGunner_Update(void);
void ERZGunner_LateUpdate(void);
void ERZGunner_StaticUpdate(void);
void ERZGunner_Draw(void);
void ERZGunner_Create(void* data);
void ERZGunner_StageLoad(void);
void ERZGunner_EditorDraw(void);
void ERZGunner_EditorLoad(void);
void ERZGunner_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ERZGUNNER_H
