#ifndef OBJ_MSZCUTSCENEST_H
#define OBJ_MSZCUTSCENEST_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[6]; //= { -65536, -65536, 0, 0, 0, -65536 };
    ushort value2;
    ushort value3;
    TextInfo value4;
    byte value5;
    bool32 value6;
    bool32 value7;
    bool32 value8;
    bool32 value9;
    bool32 value10;
    bool32 value11;
    bool32 value12;
    byte value13;
    byte value14;
    ushort value15;
    ushort value16;
    ushort value17;
    byte value18;
} ObjectMSZCutsceneST;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMSZCutsceneST;

// Object Struct
extern ObjectMSZCutsceneST *MSZCutsceneST;

// Standard Entity Events
void MSZCutsceneST_Update(void);
void MSZCutsceneST_LateUpdate(void);
void MSZCutsceneST_StaticUpdate(void);
void MSZCutsceneST_Draw(void);
void MSZCutsceneST_Create(void* data);
void MSZCutsceneST_StageLoad(void);
void MSZCutsceneST_EditorDraw(void);
void MSZCutsceneST_EditorLoad(void);
void MSZCutsceneST_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MSZCUTSCENEST_H
