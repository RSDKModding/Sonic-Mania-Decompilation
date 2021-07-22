#ifndef OBJ_UIMODEBUTTON_H
#define OBJ_UIMODEBUTTON_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIModeButton;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIModeButton;

// Object Struct
extern ObjectUIModeButton *UIModeButton;

// Standard Entity Events
void UIModeButton_Update(void);
void UIModeButton_LateUpdate(void);
void UIModeButton_StaticUpdate(void);
void UIModeButton_Draw(void);
void UIModeButton_Create(void* data);
void UIModeButton_StageLoad(void);
void UIModeButton_EditorDraw(void);
void UIModeButton_EditorLoad(void);
void UIModeButton_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIMODEBUTTON_H
