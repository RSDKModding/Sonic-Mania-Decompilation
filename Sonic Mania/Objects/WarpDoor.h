#ifndef OBJ_WARPDOOR_H
#define OBJ_WARPDOOR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWarpDoor;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWarpDoor;

// Object Struct
extern ObjectWarpDoor *WarpDoor;

// Standard Entity Events
void WarpDoor_Update(void);
void WarpDoor_LateUpdate(void);
void WarpDoor_StaticUpdate(void);
void WarpDoor_Draw(void);
void WarpDoor_Create(void* data);
void WarpDoor_StageLoad(void);
void WarpDoor_EditorDraw(void);
void WarpDoor_EditorLoad(void);
void WarpDoor_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_WARPDOOR_H
