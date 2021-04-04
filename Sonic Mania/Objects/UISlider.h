#ifndef OBJ_UISLIDER_H
#define OBJ_UISLIDER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUISlider;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUISlider;

// Object Struct
extern ObjectUISlider *UISlider;

// Standard Entity Events
void UISlider_Update(void);
void UISlider_LateUpdate(void);
void UISlider_StaticUpdate(void);
void UISlider_Draw(void);
void UISlider_Create(void* data);
void UISlider_StageLoad(void);
void UISlider_EditorDraw(void);
void UISlider_EditorLoad(void);
void UISlider_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UISLIDER_H
