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
void MSZCutsceneK_Update(void);
void MSZCutsceneK_LateUpdate(void);
void MSZCutsceneK_StaticUpdate(void);
void MSZCutsceneK_Draw(void);
void MSZCutsceneK_Create(void* data);
void MSZCutsceneK_StageLoad(void);
void MSZCutsceneK_EditorDraw(void);
void MSZCutsceneK_EditorLoad(void);
void MSZCutsceneK_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MSZCUTSCENEK_H
