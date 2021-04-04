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
void FBZSinkTrash_Update(void);
void FBZSinkTrash_LateUpdate(void);
void FBZSinkTrash_StaticUpdate(void);
void FBZSinkTrash_Draw(void);
void FBZSinkTrash_Create(void* data);
void FBZSinkTrash_StageLoad(void);
void FBZSinkTrash_EditorDraw(void);
void FBZSinkTrash_EditorLoad(void);
void FBZSinkTrash_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FBZSINKTRASH_H
