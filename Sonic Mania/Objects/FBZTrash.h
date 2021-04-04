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
void FBZTrash_Update(void);
void FBZTrash_LateUpdate(void);
void FBZTrash_StaticUpdate(void);
void FBZTrash_Draw(void);
void FBZTrash_Create(void* data);
void FBZTrash_StageLoad(void);
void FBZTrash_EditorDraw(void);
void FBZTrash_EditorLoad(void);
void FBZTrash_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FBZTRASH_H
