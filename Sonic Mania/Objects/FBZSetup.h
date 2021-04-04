#ifndef OBJ_FBZSETUP_H
#define OBJ_FBZSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFBZSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFBZSetup;

// Object Struct
extern ObjectFBZSetup *FBZSetup;

// Standard Entity Events
void FBZSetup_Update(void);
void FBZSetup_LateUpdate(void);
void FBZSetup_StaticUpdate(void);
void FBZSetup_Draw(void);
void FBZSetup_Create(void* data);
void FBZSetup_StageLoad(void);
void FBZSetup_EditorDraw(void);
void FBZSetup_EditorLoad(void);
void FBZSetup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FBZSETUP_H
