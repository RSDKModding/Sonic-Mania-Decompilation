#ifndef OBJ_FBZSINKTRASH_H
#define OBJ_FBZSINKTRASH_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFBZSinkTrash;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFBZSinkTrash;

// Object Struct
extern ObjectFBZSinkTrash *FBZSinkTrash;

// Standard Entity Events
void FBZSinkTrash_Update();
void FBZSinkTrash_LateUpdate();
void FBZSinkTrash_StaticUpdate();
void FBZSinkTrash_Draw();
void FBZSinkTrash_Create(void* data);
void FBZSinkTrash_StageLoad();
void FBZSinkTrash_EditorDraw();
void FBZSinkTrash_EditorLoad();
void FBZSinkTrash_Serialize();

// Extra Entity Functions


#endif //!OBJ_FBZSINKTRASH_H
