#ifndef OBJ_TTCUTSCENE_H
#define OBJ_TTCUTSCENE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTTCutscene;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTTCutscene;

// Object Struct
extern ObjectTTCutscene *TTCutscene;

// Standard Entity Events
void TTCutscene_Update(void);
void TTCutscene_LateUpdate(void);
void TTCutscene_StaticUpdate(void);
void TTCutscene_Draw(void);
void TTCutscene_Create(void* data);
void TTCutscene_StageLoad(void);
void TTCutscene_EditorDraw(void);
void TTCutscene_EditorLoad(void);
void TTCutscene_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TTCUTSCENE_H
