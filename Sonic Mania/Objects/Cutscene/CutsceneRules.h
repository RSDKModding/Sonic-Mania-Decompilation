#ifndef OBJ_CUTSCENERULES_H
#define OBJ_CUTSCENERULES_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCutsceneRules;

// Entity Class
typedef struct {
    MANIA_CUTSCENE_BASE
} EntityCutsceneRules;

// Object Entity
extern ObjectCutsceneRules *CutsceneRules;

// Standard Entity Events
void CutsceneRules_Update(void);
void CutsceneRules_LateUpdate(void);
void CutsceneRules_StaticUpdate(void);
void CutsceneRules_Draw(void);
void CutsceneRules_Create(void* data);
void CutsceneRules_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void CutsceneRules_EditorDraw(void);
void CutsceneRules_EditorLoad(void);
#endif
void CutsceneRules_Serialize(void);

// Extra Entity Functions
void CutsceneRules_SetupEntity(void *e);
void CutsceneRules_DrawCutsceneBounds(void *e);

#endif //!OBJ_CUTSCENERULES_H
