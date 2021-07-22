#ifndef OBJ_UIVSRESULTS_H
#define OBJ_UIVSRESULTS_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIVsResults;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIVsResults;

// Object Struct
extern ObjectUIVsResults *UIVsResults;

// Standard Entity Events
void UIVsResults_Update(void);
void UIVsResults_LateUpdate(void);
void UIVsResults_StaticUpdate(void);
void UIVsResults_Draw(void);
void UIVsResults_Create(void* data);
void UIVsResults_StageLoad(void);
void UIVsResults_EditorDraw(void);
void UIVsResults_EditorLoad(void);
void UIVsResults_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIVSRESULTS_H
