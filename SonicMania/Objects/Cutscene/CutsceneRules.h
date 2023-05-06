#ifndef OBJ_CUTSCENERULES_H
#define OBJ_CUTSCENERULES_H

#include "Game.h"

// Object Class
struct ObjectCutsceneRules {
    RSDK_OBJECT
};

// Entity Class
struct EntityCutsceneRules {
    MANIA_CUTSCENE_BASE
};

// Object Entity
extern ObjectCutsceneRules *CutsceneRules;

// Standard Entity Events
void CutsceneRules_Update(void);
void CutsceneRules_LateUpdate(void);
void CutsceneRules_StaticUpdate(void);
void CutsceneRules_Draw(void);
void CutsceneRules_Create(void *data);
void CutsceneRules_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CutsceneRules_EditorDraw(void);
void CutsceneRules_EditorLoad(void);
#endif
void CutsceneRules_Serialize(void);

// Extra Entity Functions
bool32 CutsceneRules_IsAct1(void);
bool32 CutsceneRules_IsAct2(void);
bool32 CutsceneRules_IsIntroEnabled(void);
bool32 CutsceneRules_IsAct1Regular(void);
bool32 CutsceneRules_CheckStageReload(void);
bool32 CutsceneRules_CheckPlayerPos(int32 x1, int32 y1, int32 x2, int32 y2);

// Extra Helpers
void CutsceneRules_SetupEntity(void *e, Vector2 *size, Hitbox *hitbox);
void CutsceneRules_DrawCutsceneBounds(void *e, Vector2 *size);

#endif //! OBJ_CUTSCENERULES_H
