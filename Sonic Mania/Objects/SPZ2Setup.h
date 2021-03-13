#ifndef OBJ_SPZ2SETUP_H
#define OBJ_SPZ2SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1;
    int value2;
    int value3; //= 12;
    int value4;
    int value5[8]; //= { 11, 2, 2, 2, 2, 2, 2, 2 };
    int value6; //= 30;
    int value7;
    int value8[12]; //= { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1 };
    int value9[12]; //= { 30, 9, 6, 6, 6, 9, 30, 9, 6, 6, 6, 9 };
    int value10[32]; //= { -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int value11[16]; //= { 96, 96, 112, 96, 112, 112, 112, 112, 112, 112, 112, 112, 112, 96, 112, 96 };
    int value12[16]; //= { 0, 0, 0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 0, 0 };
    int value13[16]; //= { 0, 0, 0, 16, 32, 48, 32, 32, 48, 32, 32, 16, 0, 0, 0, 0 };
    int value14;
    int value15;
    bool32 value16;
    bool32 value17;
    ushort value18;
    ushort value19;
    bool32 value20;
} ObjectSPZ2Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySPZ2Setup;

// Object Struct
extern ObjectSPZ2Setup *SPZ2Setup;

// Standard Entity Events
void SPZ2Setup_Update();
void SPZ2Setup_LateUpdate();
void SPZ2Setup_StaticUpdate();
void SPZ2Setup_Draw();
void SPZ2Setup_Create(void* data);
void SPZ2Setup_StageLoad();
void SPZ2Setup_EditorDraw();
void SPZ2Setup_EditorLoad();
void SPZ2Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPZ2SETUP_H
