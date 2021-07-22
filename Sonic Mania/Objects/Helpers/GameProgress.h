#ifndef OBJ_GAMEPROGRESS_H
#define OBJ_GAMEPROGRESS_H

#include "SonicMania.h"

// Object Class
typedef struct {
#if !RETRO_USE_PLUS
	RSDK_OBJECT
#else
    byte tmp; //TODO
#endif
} ObjectGameProgress;

#if !RETRO_USE_PLUS
// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGameProgress;
#endif

// Object Struct
extern ObjectGameProgress *GameProgress;

// Standard Entity Events
void GameProgess_Update(void);
void GameProgess_LateUpdate(void);
void GameProgess_StaticUpdate(void);
void GameProgess_Draw(void);
void GameProgess_Create(void *data);
void GameProgess_StageLoad(void);
void GameProgess_EditorDraw(void);
void GameProgess_EditorLoad(void);
void GameProgess_Serialize(void);

#endif //!OBJ_GAMEPROGRESS_H
