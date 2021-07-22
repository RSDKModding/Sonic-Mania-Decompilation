#ifndef OBJ_UIVSCHARSELECTOR_H
#define OBJ_UIVSCHARSELECTOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIVsCharSelector;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIVsCharSelector;

// Object Struct
extern ObjectUIVsCharSelector *UIVsCharSelector;

// Standard Entity Events
void UIVsCharSelector_Update(void);
void UIVsCharSelector_LateUpdate(void);
void UIVsCharSelector_StaticUpdate(void);
void UIVsCharSelector_Draw(void);
void UIVsCharSelector_Create(void* data);
void UIVsCharSelector_StageLoad(void);
void UIVsCharSelector_EditorDraw(void);
void UIVsCharSelector_EditorLoad(void);
void UIVsCharSelector_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIVSCHARSELECTOR_H
