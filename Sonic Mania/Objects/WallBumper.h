#ifndef OBJ_WALLBUMPER_H
#define OBJ_WALLBUMPER_H

#include "../SonicMania.h"

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
void WallBumper_Update();
void WallBumper_LateUpdate();
void WallBumper_StaticUpdate();
void WallBumper_Draw();
void WallBumper_Create(void* data);
void WallBumper_StageLoad();
void WallBumper_EditorDraw();
void WallBumper_EditorLoad();
void WallBumper_Serialize();

// Extra Entity Functions


#endif //!OBJ_WALLBUMPER_H
