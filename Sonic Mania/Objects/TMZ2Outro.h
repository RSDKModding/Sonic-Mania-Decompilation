#ifndef OBJ_TMZ2OUTRO_H
#define OBJ_TMZ2OUTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint value1[7]; //= { 4194664, 7340408, 9437576, 12059016, 15729048, 15753392, 15769800 };
    ushort value2;
    ushort value3;
    ushort value4;
    ushort value5;
    ushort value6;
    ushort value7;
    ushort value8;
    ushort value9;
    byte value10;
    int value11;
} ObjectTMZ2Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTMZ2Outro;

// Object Struct
extern ObjectTMZ2Outro *TMZ2Outro;

// Standard Entity Events
void TMZ2Outro_Update();
void TMZ2Outro_LateUpdate();
void TMZ2Outro_StaticUpdate();
void TMZ2Outro_Draw();
void TMZ2Outro_Create(void* data);
void TMZ2Outro_StageLoad();
void TMZ2Outro_EditorDraw();
void TMZ2Outro_EditorLoad();
void TMZ2Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_TMZ2OUTRO_H
