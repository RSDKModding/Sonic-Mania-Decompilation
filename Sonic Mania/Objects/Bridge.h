#ifndef OBJ_BRIDGE_H
#define OBJ_BRIDGE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBridge;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBridge;

// Object Struct
extern ObjectBridge *Bridge;

// Standard Entity Events
void Bridge_Update(void);
void Bridge_LateUpdate(void);
void Bridge_StaticUpdate(void);
void Bridge_Draw(void);
void Bridge_Create(void* data);
void Bridge_StageLoad(void);
void Bridge_EditorDraw(void);
void Bridge_EditorLoad(void);
void Bridge_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BRIDGE_H
