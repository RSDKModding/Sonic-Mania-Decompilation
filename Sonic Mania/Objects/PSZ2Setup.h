#ifndef OBJ_PSZ2SETUP_H
#define OBJ_PSZ2SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[8]; //= { 9, 7, 6, 7, 9, 7, 6, 7 };
    int value2;
    int value3;
    byte value4;
    short value5;
    ushort value6;
    ushort value7;
} ObjectPSZ2Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPSZ2Setup;

// Object Struct
extern ObjectPSZ2Setup *PSZ2Setup;

// Standard Entity Events
void PSZ2Setup_Update(void);
void PSZ2Setup_LateUpdate(void);
void PSZ2Setup_StaticUpdate(void);
void PSZ2Setup_Draw(void);
void PSZ2Setup_Create(void* data);
void PSZ2Setup_StageLoad(void);
void PSZ2Setup_EditorDraw(void);
void PSZ2Setup_EditorLoad(void);
void PSZ2Setup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PSZ2SETUP_H
