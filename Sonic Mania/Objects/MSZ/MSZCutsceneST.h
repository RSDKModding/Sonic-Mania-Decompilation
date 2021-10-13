#ifndef OBJ_MSZCUTSCENEST_H
#define OBJ_MSZCUTSCENEST_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int value1[6], { -0x10000, -0x10000, 0, 0, 0, -0x10000 });
    uint16 value2;
    uint16 value3;
    Vector2 value4;
    uint8 value5;
    void *value6;
    void *value7;
    void *value8;
    void *value9;
    void *value10;
    void *value11[3];
    void *value12[3];
    uint8 value13;
    uint8 value14;
    uint16 value15;
    uint16 value16;
    uint16 value17;
    uint8 value18;
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
