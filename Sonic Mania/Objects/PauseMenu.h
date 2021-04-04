#ifndef OBJ_PAUSEMENU_H
#define OBJ_PAUSEMENU_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPauseMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPauseMenu;

// Object Struct
extern ObjectPauseMenu *PauseMenu;

// Standard Entity Events
void PauseMenu_Update(void);
void PauseMenu_LateUpdate(void);
void PauseMenu_StaticUpdate(void);
void PauseMenu_Draw(void);
void PauseMenu_Create(void* data);
void PauseMenu_StageLoad(void);
void PauseMenu_EditorDraw(void);
void PauseMenu_EditorLoad(void);
void PauseMenu_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PAUSEMENU_H
