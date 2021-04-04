#ifndef OBJ_MENUSETUP_H
#define OBJ_MENUSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMenuSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMenuSetup;

// Object Struct
extern ObjectMenuSetup *MenuSetup;

// Standard Entity Events
void MenuSetup_Update(void);
void MenuSetup_LateUpdate(void);
void MenuSetup_StaticUpdate(void);
void MenuSetup_Draw(void);
void MenuSetup_Create(void* data);
void MenuSetup_StageLoad(void);
void MenuSetup_EditorDraw(void);
void MenuSetup_EditorLoad(void);
void MenuSetup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MENUSETUP_H
