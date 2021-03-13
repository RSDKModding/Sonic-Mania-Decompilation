#ifndef OBJ_TILEPLATFORM_H
#define OBJ_TILEPLATFORM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTilePlatform;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTilePlatform;

// Object Struct
extern ObjectTilePlatform *TilePlatform;

// Standard Entity Events
void TilePlatform_Update();
void TilePlatform_LateUpdate();
void TilePlatform_StaticUpdate();
void TilePlatform_Draw();
void TilePlatform_Create(void* data);
void TilePlatform_StageLoad();
void TilePlatform_EditorDraw();
void TilePlatform_EditorLoad();
void TilePlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_TILEPLATFORM_H
