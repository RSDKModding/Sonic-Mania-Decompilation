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
void UIChoice_Update(void);
void UIChoice_LateUpdate(void);
void UIChoice_StaticUpdate(void);
void UIChoice_Draw(void);
void UIChoice_Create(void* data);
void UIChoice_StageLoad(void);
void UIChoice_EditorDraw(void);
void UIChoice_EditorLoad(void);
void UIChoice_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UICHOICE_H
