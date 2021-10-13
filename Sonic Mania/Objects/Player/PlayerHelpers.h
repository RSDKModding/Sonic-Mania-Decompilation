#ifndef OBJ_PLAYERHELPERS_H
#define OBJ_PLAYERHELPERS_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPlayerHelpers;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPlayerHelpers;

// Object Entity
extern ObjectPlayerHelpers *PlayerHelpers;

// Standard Entity Events
void PlayerHelpers_Update(void);
void PlayerHelpers_LateUpdate(void);
void PlayerHelpers_StaticUpdate(void);
void PlayerHelpers_Draw(void);
void PlayerHelpers_Create(void* data);
void PlayerHelpers_StageLoad(void);
void PlayerHelpers_EditorDraw(void);
void PlayerHelpers_EditorLoad(void);
void PlayerHelpers_Serialize(void);

// Extra Entity Functions
bool32 PlayerHelpers_CheckAct1(void);
bool32 PlayerHelpers_CheckAct2(void);
bool32 PlayerHelpers_CheckIntro(void);
bool32 PlayerHelpers_CheckAct1Regular(void);
bool32 PlayerHelpers_CheckStageReload(void);
bool32 PlayerHelpers_CheckPlayerPos(int32 x1, int32 y1, int32 x2, int32 y2);

#endif //!OBJ_PLAYERHELPERS_H
