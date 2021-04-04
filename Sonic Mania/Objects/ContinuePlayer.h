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
void ContinuePlayer_Update(void);
void ContinuePlayer_LateUpdate(void);
void ContinuePlayer_StaticUpdate(void);
void ContinuePlayer_Draw(void);
void ContinuePlayer_Create(void* data);
void ContinuePlayer_StageLoad(void);
void ContinuePlayer_EditorDraw(void);
void ContinuePlayer_EditorLoad(void);
void ContinuePlayer_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CONTINUEPLAYER_H
