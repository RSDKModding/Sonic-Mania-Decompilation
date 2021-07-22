#ifndef OBJ_FBZMISSILE_H
#define OBJ_FBZMISSILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    colour value1;
    colour value2;
    colour value3;
    int value4[8]; //= { 65536, 57344, 73728, 81920, 65536, 57344, 49152, 57344 };
    int value5;
    ushort value6;
    ushort value7;
    ushort value8;
    ushort value9;
} ObjectFBZMissile;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFBZMissile;

// Object Struct
extern ObjectFBZMissile *FBZMissile;

// Standard Entity Events
void FBZMissile_Update(void);
void FBZMissile_LateUpdate(void);
void FBZMissile_StaticUpdate(void);
void FBZMissile_Draw(void);
void FBZMissile_Create(void* data);
void FBZMissile_StageLoad(void);
void FBZMissile_EditorDraw(void);
void FBZMissile_EditorLoad(void);
void FBZMissile_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FBZMISSILE_H
