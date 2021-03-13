#ifndef OBJ_CUTSCENERULES_H
#define OBJ_CUTSCENERULES_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCutsceneRules;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCutsceneRules;

// Object Entity
extern ObjectCutsceneRules *CutsceneRules;

// Standard Entity Events
void CutsceneRules_Update();
void CutsceneRules_LateUpdate();
void CutsceneRules_StaticUpdate();
void CutsceneRules_Draw();
void CutsceneRules_Create(void* data);
void CutsceneRules_StageLoad();
void CutsceneRules_EditorDraw();
void CutsceneRules_EditorLoad();
void CutsceneRules_Serialize();

// Extra Entity Functions


#endif //!OBJ_CUTSCENERULES_H
