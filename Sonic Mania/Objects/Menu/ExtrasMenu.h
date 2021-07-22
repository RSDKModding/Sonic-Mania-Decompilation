#ifndef OBJ_EXTRASMENU_H
#define OBJ_EXTRASMENU_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectExtrasMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityExtrasMenu;

// Object Struct
extern ObjectExtrasMenu *ExtrasMenu;

// Standard Entity Events
void ExtrasMenu_Update(void);
void ExtrasMenu_LateUpdate(void);
void ExtrasMenu_StaticUpdate(void);
void ExtrasMenu_Draw(void);
void ExtrasMenu_Create(void* data);
void ExtrasMenu_StageLoad(void);
void ExtrasMenu_EditorDraw(void);
void ExtrasMenu_EditorLoad(void);
void ExtrasMenu_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_EXTRASMENU_H
