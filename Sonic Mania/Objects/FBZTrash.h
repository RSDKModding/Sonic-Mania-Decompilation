#ifndef OBJ_FBZTRASH_H
#define OBJ_FBZTRASH_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFBZTrash;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFBZTrash;

// Object Struct
extern ObjectFBZTrash *FBZTrash;

// Standard Entity Events
void FBZTrash_Update();
void FBZTrash_LateUpdate();
void FBZTrash_StaticUpdate();
void FBZTrash_Draw();
void FBZTrash_Create(void* data);
void FBZTrash_StageLoad();
void FBZTrash_EditorDraw();
void FBZTrash_EditorLoad();
void FBZTrash_Serialize();

// Extra Entity Functions


#endif //!OBJ_FBZTRASH_H
