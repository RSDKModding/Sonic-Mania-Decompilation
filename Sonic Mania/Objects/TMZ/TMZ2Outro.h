#ifndef OBJ_TMZ2OUTRO_H
#define OBJ_TMZ2OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(colour value1[7], { 0x400168, 0x700178, 0x900188, 0xB80188, 0xF00198, 0xF060B0, 0xF0A0C8 });
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
void TMZ2Outro_Update(void);
void TMZ2Outro_LateUpdate(void);
void TMZ2Outro_StaticUpdate(void);
void TMZ2Outro_Draw(void);
void TMZ2Outro_Create(void* data);
void TMZ2Outro_StageLoad(void);
void TMZ2Outro_EditorDraw(void);
void TMZ2Outro_EditorLoad(void);
void TMZ2Outro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TMZ2OUTRO_H
