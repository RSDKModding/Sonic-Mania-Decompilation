#ifndef OBJ_UIRANKBUTTON_H
#define OBJ_UIRANKBUTTON_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIRankButton;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIRankButton;

// Object Struct
extern ObjectUIRankButton *UIRankButton;

// Standard Entity Events
void UIRankButton_Update();
void UIRankButton_LateUpdate();
void UIRankButton_StaticUpdate();
void UIRankButton_Draw();
void UIRankButton_Create(void* data);
void UIRankButton_StageLoad();
void UIRankButton_EditorDraw();
void UIRankButton_EditorLoad();
void UIRankButton_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIRANKBUTTON_H
