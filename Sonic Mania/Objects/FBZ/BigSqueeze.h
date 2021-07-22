#ifndef OBJ_BIGSQUEEZE_H
#define OBJ_BIGSQUEEZE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[10]; //= { 3, 12, -131072, -65536, 13, 0, -65536, 14, 131072, -65536 };
    int value2[13]; //= { 4, 8, -131072, -65536, 9, -65536, -65536, 10, 65536, -65536, 11, 131072, -65536 };
    int value3[25]; //= { 8, 0, -131072, -65536, 1, -65536, -65536, 2, 65536, -65536, 3, 131072, -65536, 4, -131072, -32768, 5, -65536, -32768, 6, 65536, -32768, 7, 131072, -32768 };
    int value4;
    byte value5;
    int value6;
    int value7;
    ushort value8;
    ushort value9;
    ushort value10;
    ushort value11;
    ushort value12;
} ObjectBigSqueeze;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBigSqueeze;

// Object Struct
extern ObjectBigSqueeze *BigSqueeze;

// Standard Entity Events
void BigSqueeze_Update(void);
void BigSqueeze_LateUpdate(void);
void BigSqueeze_StaticUpdate(void);
void BigSqueeze_Draw(void);
void BigSqueeze_Create(void* data);
void BigSqueeze_StageLoad(void);
void BigSqueeze_EditorDraw(void);
void BigSqueeze_EditorLoad(void);
void BigSqueeze_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BIGSQUEEZE_H
