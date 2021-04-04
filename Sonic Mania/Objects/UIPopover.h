#ifndef OBJ_UIPOPOVER_H
#define OBJ_UIPOPOVER_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIPopover;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIPopover;

// Object Struct
extern ObjectUIPopover *UIPopover;

// Standard Entity Events
void UIPopover_Update(void);
void UIPopover_LateUpdate(void);
void UIPopover_StaticUpdate(void);
void UIPopover_Draw(void);
void UIPopover_Create(void* data);
void UIPopover_StageLoad(void);
void UIPopover_EditorDraw(void);
void UIPopover_EditorLoad(void);
void UIPopover_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_UIPOPOVER_H
