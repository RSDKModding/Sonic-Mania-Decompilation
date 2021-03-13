#ifndef OBJ_FBZ2OUTRO_H
#define OBJ_FBZ2OUTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFBZ2Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFBZ2Outro;

// Object Struct
extern ObjectFBZ2Outro *FBZ2Outro;

// Standard Entity Events
void FBZ2Outro_Update();
void FBZ2Outro_LateUpdate();
void FBZ2Outro_StaticUpdate();
void FBZ2Outro_Draw();
void FBZ2Outro_Create(void* data);
void FBZ2Outro_StageLoad();
void FBZ2Outro_EditorDraw();
void FBZ2Outro_EditorLoad();
void FBZ2Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_FBZ2OUTRO_H
