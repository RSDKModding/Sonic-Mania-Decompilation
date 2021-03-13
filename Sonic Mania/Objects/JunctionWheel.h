#ifndef OBJ_JUNCTIONWHEEL_H
#define OBJ_JUNCTIONWHEEL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectJunctionWheel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityJunctionWheel;

// Object Struct
extern ObjectJunctionWheel *JunctionWheel;

// Standard Entity Events
void JunctionWheel_Update();
void JunctionWheel_LateUpdate();
void JunctionWheel_StaticUpdate();
void JunctionWheel_Draw();
void JunctionWheel_Create(void* data);
void JunctionWheel_StageLoad();
void JunctionWheel_EditorDraw();
void JunctionWheel_EditorLoad();
void JunctionWheel_Serialize();

// Extra Entity Functions


#endif //!OBJ_JUNCTIONWHEEL_H
