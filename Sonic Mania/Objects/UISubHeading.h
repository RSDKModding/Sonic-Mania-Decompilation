#ifndef OBJ_UISUBHEADING_H
#define OBJ_UISUBHEADING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUISubHeading;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUISubHeading;

// Object Struct
extern ObjectUISubHeading *UISubHeading;

// Standard Entity Events
void UISubHeading_Update(void);
void UISubHeading_LateUpdate(void);
void UISubHeading_StaticUpdate(void);
void UISubHeading_Draw(void);
void UISubHeading_Create(void* data);
void UISubHeading_StageLoad(void);
void UISubHeading_EditorDraw(void);
void UISubHeading_EditorLoad(void);
void UISubHeading_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UISUBHEADING_H
