#ifndef OBJ_UICHOICE_H
#define OBJ_UICHOICE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIChoice;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIChoice;

// Object Struct
extern ObjectUIChoice *UIChoice;

// Standard Entity Events
void UIChoice_Update();
void UIChoice_LateUpdate();
void UIChoice_StaticUpdate();
void UIChoice_Draw();
void UIChoice_Create(void* data);
void UIChoice_StageLoad();
void UIChoice_EditorDraw();
void UIChoice_EditorLoad();
void UIChoice_Serialize();

// Extra Entity Functions


#endif //!OBJ_UICHOICE_H
