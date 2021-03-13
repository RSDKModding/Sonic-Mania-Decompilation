#ifndef OBJ_PLAYERHELPERS_H
#define OBJ_PLAYERHELPERS_H

#include "../SonicMania.h"

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
void PlayerHelpers_Update();
void PlayerHelpers_LateUpdate();
void PlayerHelpers_StaticUpdate();
void PlayerHelpers_Draw();
void PlayerHelpers_Create(void* data);
void PlayerHelpers_StageLoad();
void PlayerHelpers_EditorDraw();
void PlayerHelpers_EditorLoad();
void PlayerHelpers_Serialize();

// Extra Entity Functions


#endif //!OBJ_PLAYERHELPERS_H
