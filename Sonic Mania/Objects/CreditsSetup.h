#ifndef OBJ_CREDITSSETUP_H
#define OBJ_CREDITSSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCreditsSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCreditsSetup;

// Object Struct
extern ObjectCreditsSetup *CreditsSetup;

// Standard Entity Events
void CreditsSetup_Update();
void CreditsSetup_LateUpdate();
void CreditsSetup_StaticUpdate();
void CreditsSetup_Draw();
void CreditsSetup_Create(void* data);
void CreditsSetup_StageLoad();
void CreditsSetup_EditorDraw();
void CreditsSetup_EditorLoad();
void CreditsSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_CREDITSSETUP_H
