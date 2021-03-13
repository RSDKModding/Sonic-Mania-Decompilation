#ifndef OBJ_THANKSSETUP_H
#define OBJ_THANKSSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectThanksSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityThanksSetup;

// Object Struct
extern ObjectThanksSetup *ThanksSetup;

// Standard Entity Events
void ThanksSetup_Update();
void ThanksSetup_LateUpdate();
void ThanksSetup_StaticUpdate();
void ThanksSetup_Draw();
void ThanksSetup_Create(void* data);
void ThanksSetup_StageLoad();
void ThanksSetup_EditorDraw();
void ThanksSetup_EditorLoad();
void ThanksSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_THANKSSETUP_H
