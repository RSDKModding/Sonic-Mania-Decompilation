#ifndef OBJ_GHZCUTSCENEK_H
#define OBJ_GHZCUTSCENEK_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectGHZCutsceneK;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGHZCutsceneK;

// Object Struct
extern ObjectGHZCutsceneK *GHZCutsceneK;

// Standard Entity Events
void GHZCutsceneK_Update();
void GHZCutsceneK_LateUpdate();
void GHZCutsceneK_StaticUpdate();
void GHZCutsceneK_Draw();
void GHZCutsceneK_Create(void* data);
void GHZCutsceneK_StageLoad();
void GHZCutsceneK_EditorDraw();
void GHZCutsceneK_EditorLoad();
void GHZCutsceneK_Serialize();

// Extra Entity Functions


#endif //!OBJ_GHZCUTSCENEK_H
