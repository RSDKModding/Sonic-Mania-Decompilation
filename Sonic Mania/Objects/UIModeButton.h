#ifndef OBJ_UIMODEBUTTON_H
#define OBJ_UIMODEBUTTON_H

#include "../SonicMania.h"

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
void UIModeButton_Update();
void UIModeButton_LateUpdate();
void UIModeButton_StaticUpdate();
void UIModeButton_Draw();
void UIModeButton_Create(void* data);
void UIModeButton_StageLoad();
void UIModeButton_EditorDraw();
void UIModeButton_EditorLoad();
void UIModeButton_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIMODEBUTTON_H
