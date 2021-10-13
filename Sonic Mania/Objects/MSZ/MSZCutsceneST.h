#ifndef OBJ_MSZCUTSCENEST_H
#define OBJ_MSZCUTSCENEST_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int value1[6], { -0x10000, -0x10000, 0, 0, 0, -0x10000 });
    ushort value2;
    ushort value3;
    Vector2 value4;
    byte value5;
    void *value6;
    void *value7;
    void *value8;
    void *value9;
    void *value10;
    void *value11[3];
    void *value12[3];
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
