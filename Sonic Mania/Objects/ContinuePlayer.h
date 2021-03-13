#ifndef OBJ_CONTINUEPLAYER_H
#define OBJ_CONTINUEPLAYER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectContinuePlayer;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityContinuePlayer;

// Object Struct
extern ObjectContinuePlayer *ContinuePlayer;

// Standard Entity Events
void ContinuePlayer_Update();
void ContinuePlayer_LateUpdate();
void ContinuePlayer_StaticUpdate();
void ContinuePlayer_Draw();
void ContinuePlayer_Create(void* data);
void ContinuePlayer_StageLoad();
void ContinuePlayer_EditorDraw();
void ContinuePlayer_EditorLoad();
void ContinuePlayer_Serialize();

// Extra Entity Functions


#endif //!OBJ_CONTINUEPLAYER_H
