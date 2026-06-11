#ifndef OBJ_PLAYERHELPERS_H
#define OBJ_PLAYERHELPERS_H

#include "Game.h"

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
void PlayerHelpers_Create(void *data);
void PlayerHelpers_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PlayerHelpers_EditorDraw(void);
void PlayerHelpers_EditorLoad(void);
#endif
void PlayerHelpers_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_PLAYERHELPERS_H
