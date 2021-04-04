#ifndef OBJ_CONTINUESETUP_H
#define OBJ_CONTINUESETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectContinueSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityContinueSetup;

// Object Struct
extern ObjectContinueSetup *ContinueSetup;

// Standard Entity Events
void ContinueSetup_Update(void);
void ContinueSetup_LateUpdate(void);
void ContinueSetup_StaticUpdate(void);
void ContinueSetup_Draw(void);
void ContinueSetup_Create(void* data);
void ContinueSetup_StageLoad(void);
void ContinueSetup_EditorDraw(void);
void ContinueSetup_EditorLoad(void);
void ContinueSetup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CONTINUESETUP_H
