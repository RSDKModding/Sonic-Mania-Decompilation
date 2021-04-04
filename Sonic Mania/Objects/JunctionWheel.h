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
void JunctionWheel_Update(void);
void JunctionWheel_LateUpdate(void);
void JunctionWheel_StaticUpdate(void);
void JunctionWheel_Draw(void);
void JunctionWheel_Create(void* data);
void JunctionWheel_StageLoad(void);
void JunctionWheel_EditorDraw(void);
void JunctionWheel_EditorLoad(void);
void JunctionWheel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_JUNCTIONWHEEL_H
