#ifndef OBJ_LRZ1SETUP_H
#define OBJ_LRZ1SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1;
    int value2;
    bool32 value3;
    bool32 value4;
    bool32 value5;
    bool32 value6;
    int value7[32]; //= { -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int value8[32]; //= { -2, -2, -2, 0, -2, 0, -4, -2, 0, -4, -4, -1, 0, 0, -3, -4, -4, -1, -4, -4, -3, -4, -1, 0, 0, 0, -3, -1, -1, -1, -2, 0 };
} ObjectLRZ1Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZ1Setup;

// Object Struct
extern ObjectLRZ1Setup *LRZ1Setup;

// Standard Entity Events
void LRZ1Setup_Update();
void LRZ1Setup_LateUpdate();
void LRZ1Setup_StaticUpdate();
void LRZ1Setup_Draw();
void LRZ1Setup_Create(void* data);
void LRZ1Setup_StageLoad();
void LRZ1Setup_EditorDraw();
void LRZ1Setup_EditorLoad();
void LRZ1Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZ1SETUP_H
