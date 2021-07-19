#ifndef OBJ_UIRANKBUTTON_H
#define OBJ_UIRANKBUTTON_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
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
void UIRankButton_Update(void);
void UIRankButton_LateUpdate(void);
void UIRankButton_StaticUpdate(void);
void UIRankButton_Draw(void);
void UIRankButton_Create(void* data);
void UIRankButton_StageLoad(void);
void UIRankButton_EditorDraw(void);
void UIRankButton_EditorLoad(void);
void UIRankButton_Serialize(void);

// Extra Entity Functions
#endif


#endif //!OBJ_UIRANKBUTTON_H
