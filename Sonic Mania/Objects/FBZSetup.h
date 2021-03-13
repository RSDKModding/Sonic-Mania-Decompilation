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
void FBZSetup_Update();
void FBZSetup_LateUpdate();
void FBZSetup_StaticUpdate();
void FBZSetup_Draw();
void FBZSetup_Create(void* data);
void FBZSetup_StageLoad();
void FBZSetup_EditorDraw();
void FBZSetup_EditorLoad();
void FBZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_FBZSETUP_H
