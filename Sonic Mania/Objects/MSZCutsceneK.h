#ifndef OBJ_MSZCUTSCENEK_H
#define OBJ_MSZCUTSCENEK_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMSZCutsceneK;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMSZCutsceneK;

// Object Struct
extern ObjectMSZCutsceneK *MSZCutsceneK;

// Standard Entity Events
void MSZCutsceneK_Update();
void MSZCutsceneK_LateUpdate();
void MSZCutsceneK_StaticUpdate();
void MSZCutsceneK_Draw();
void MSZCutsceneK_Create(void* data);
void MSZCutsceneK_StageLoad();
void MSZCutsceneK_EditorDraw();
void MSZCutsceneK_EditorLoad();
void MSZCutsceneK_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSZCUTSCENEK_H
