#ifndef OBJ_PUYOBEAN_H
#define OBJ_PUYOBEAN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[5], { 16, 12, 8, 4, 2 });
    colour value2;
    int32 value3;
    uint8 value4;
    bool32 value5;
    int32 value6;
    TextInfo value7;
    uint8 value8;
    int32 value9;
    Animator value10;
    uint16 value11;
    uint16 value12;
    uint16 value13;
    uint16 value14;
    uint16 value15;
    uint16 value16;
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
