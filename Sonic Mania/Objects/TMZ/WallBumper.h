#ifndef OBJ_WALLBUMPER_H
#define OBJ_WALLBUMPER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWallBumper;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWallBumper;

// Object Struct
extern ObjectWallBumper *WallBumper;

// Standard Entity Events
void WallBumper_Update(void);
void WallBumper_LateUpdate(void);
void WallBumper_StaticUpdate(void);
void WallBumper_Draw(void);
void WallBumper_Create(void* data);
void WallBumper_StageLoad(void);
void WallBumper_EditorDraw(void);
void WallBumper_EditorLoad(void);
void WallBumper_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_WALLBUMPER_H
