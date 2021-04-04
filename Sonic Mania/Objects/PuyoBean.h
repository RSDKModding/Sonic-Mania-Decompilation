#ifndef OBJ_PUYOBEAN_H
#define OBJ_PUYOBEAN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[5]; //= { 16, 12, 8, 4, 2 };
    colour value2;
    int value3;
    byte value4;
    bool32 value5;
    int value6;
    TextInfo value7;
    byte value8;
    int value9;
    AnimationData value10;
    ushort value11;
    ushort value12;
    ushort value13;
    ushort value14;
    ushort value15;
    ushort value16;
} ObjectPuyoBean;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPuyoBean;

// Object Struct
extern ObjectPuyoBean *PuyoBean;

// Standard Entity Events
void PuyoBean_Update(void);
void PuyoBean_LateUpdate(void);
void PuyoBean_StaticUpdate(void);
void PuyoBean_Draw(void);
void PuyoBean_Create(void* data);
void PuyoBean_StageLoad(void);
void PuyoBean_EditorDraw(void);
void PuyoBean_EditorLoad(void);
void PuyoBean_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PUYOBEAN_H
