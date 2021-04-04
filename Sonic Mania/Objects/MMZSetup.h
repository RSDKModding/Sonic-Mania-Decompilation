#ifndef OBJ_MMZSETUP_H
#define OBJ_MMZSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1;
    int value2; //= 60;
    int value3[12]; //= { 9, 4, 4, 2, 2, 2, 9, 4, 4, 2, 2, 2 };
    byte value4;
    byte value5;
    byte value6;
    byte value7;
    int value8;
    int value9;
    int value10;
    ushort value11;
} ObjectMMZSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMMZSetup;

// Object Struct
extern ObjectMMZSetup *MMZSetup;

// Standard Entity Events
void MMZSetup_Update(void);
void MMZSetup_LateUpdate(void);
void MMZSetup_StaticUpdate(void);
void MMZSetup_Draw(void);
void MMZSetup_Create(void* data);
void MMZSetup_StageLoad(void);
void MMZSetup_EditorDraw(void);
void MMZSetup_EditorLoad(void);
void MMZSetup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MMZSETUP_H
