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
void GHZCutsceneK_Update(void);
void GHZCutsceneK_LateUpdate(void);
void GHZCutsceneK_StaticUpdate(void);
void GHZCutsceneK_Draw(void);
void GHZCutsceneK_Create(void* data);
void GHZCutsceneK_StageLoad(void);
void GHZCutsceneK_EditorDraw(void);
void GHZCutsceneK_EditorLoad(void);
void GHZCutsceneK_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_GHZCUTSCENEK_H
