#ifndef OBJ_E3MENUSETUP_H
#define OBJ_E3MENUSETUP_H

#include "../SonicMania.h"

#if !RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectE3MenuSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityE3MenuSetup;

// Object Entity
extern ObjectE3MenuSetup *E3MenuSetup;

// Standard Entity Events
void E3MenuSetup_Update(void);
void E3MenuSetup_LateUpdate(void);
void E3MenuSetup_StaticUpdate(void);
void E3MenuSetup_Draw(void);
void E3MenuSetup_Create(void* data);
void E3MenuSetup_StageLoad(void);
void E3MenuSetup_EditorDraw(void);
void E3MenuSetup_EditorLoad(void);
void E3MenuSetup_Serialize(void);

// Extra Entity Functions

#endif


#endif //!OBJ_E3MENUSETUP_H
