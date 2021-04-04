#ifndef OBJ_CUTSCENEHBH_H
#define OBJ_CUTSCENEHBH_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCutsceneHBH;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCutsceneHBH;

// Object Struct
extern ObjectCutsceneHBH *CutsceneHBH;

// Standard Entity Events
void CutsceneHBH_Update(void);
void CutsceneHBH_LateUpdate(void);
void CutsceneHBH_StaticUpdate(void);
void CutsceneHBH_Draw(void);
void CutsceneHBH_Create(void* data);
void CutsceneHBH_StageLoad(void);
void CutsceneHBH_EditorDraw(void);
void CutsceneHBH_EditorLoad(void);
void CutsceneHBH_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CUTSCENEHBH_H
