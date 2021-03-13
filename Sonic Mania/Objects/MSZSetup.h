#ifndef OBJ_MSZSETUP_H
#define OBJ_MSZSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1;
    ushort value2;
    bool32 value3;
    int value4[32]; //= { -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int value5; //= 16;
    int value6; //= 30;
    int value7;
    int value8;
    int value9;
    byte value10;
    int value11[14]; //= { 0, 1, 2, 3, 4, 5, 2, 3, 4, 5, 2, 1, 6, 7 };
    int value12[14]; //= { 16, 2, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 2, 3 };
    int value13[8]; //= { 16, 2, 4, 5, 16, 2, 4, 5 };
    int value14;
    int value15;
    int value16;
    byte value17;
    bool32 value18;
    int value19;
    int value20;
    int value21;
    ushort value22;
} ObjectMSZSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMSZSetup;

// Object Struct
extern ObjectMSZSetup *MSZSetup;

// Standard Entity Events
void MSZSetup_Update();
void MSZSetup_LateUpdate();
void MSZSetup_StaticUpdate();
void MSZSetup_Draw();
void MSZSetup_Create(void* data);
void MSZSetup_StageLoad();
void MSZSetup_EditorDraw();
void MSZSetup_EditorLoad();
void MSZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSZSETUP_H
