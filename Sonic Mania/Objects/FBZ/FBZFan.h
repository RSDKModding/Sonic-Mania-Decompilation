#ifndef OBJ_FBZFAN_H
#define OBJ_FBZFAN_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFBZFan;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFBZFan;

// Object Struct
extern ObjectFBZFan *FBZFan;

// Standard Entity Events
void FBZFan_Update(void);
void FBZFan_LateUpdate(void);
void FBZFan_StaticUpdate(void);
void FBZFan_Draw(void);
void FBZFan_Create(void* data);
void FBZFan_StageLoad(void);
void FBZFan_EditorDraw(void);
void FBZFan_EditorLoad(void);
void FBZFan_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FBZFAN_H
