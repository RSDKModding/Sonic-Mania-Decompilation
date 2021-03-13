#ifndef OBJ_PSZ1SETUP_H
#define OBJ_PSZ1SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[7]; //= { 64, 2, 1, 2, 2, 6, 2 };
    int value2[14]; //= { 37, 2, 1, 2, 2, 6, 2, 12, 2, 1, 2, 2, 6, 2 };
    byte value3;
    int value4;
    int value5;
    int value6;
    int value7;
    int value8;
    int value9;
    int value10;
    int value11;
    int value12;
    int value13;
    int value14;
    int value15;
    int value16;
    int value17;
    int value18;
    int value19;
    byte value20;
    byte value21;
    ushort value22;
    ushort value23;
    ushort value24;
} ObjectPSZ1Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPSZ1Setup;

// Object Struct
extern ObjectPSZ1Setup *PSZ1Setup;

// Standard Entity Events
void PSZ1Setup_Update();
void PSZ1Setup_LateUpdate();
void PSZ1Setup_StaticUpdate();
void PSZ1Setup_Draw();
void PSZ1Setup_Create(void* data);
void PSZ1Setup_StageLoad();
void PSZ1Setup_EditorDraw();
void PSZ1Setup_EditorLoad();
void PSZ1Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_PSZ1SETUP_H
