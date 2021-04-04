#ifndef OBJ_FBZSTORM_H
#define OBJ_FBZSTORM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[20]; //= { 64, 128, 192, 256, 256, 128, 0, 32, 64, 128, 192, 256, 256, 192, 128, 96, 64, 32, 16, 0 };
    TextInfo value2;
    ushort value3;
    ushort value4;
    byte value5;
    ushort value6;
    byte value7; //= 1;
} ObjectFBZStorm;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFBZStorm;

// Object Struct
extern ObjectFBZStorm *FBZStorm;

// Standard Entity Events
void FBZStorm_Update(void);
void FBZStorm_LateUpdate(void);
void FBZStorm_StaticUpdate(void);
void FBZStorm_Draw(void);
void FBZStorm_Create(void* data);
void FBZStorm_StageLoad(void);
void FBZStorm_EditorDraw(void);
void FBZStorm_EditorLoad(void);
void FBZStorm_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FBZSTORM_H
