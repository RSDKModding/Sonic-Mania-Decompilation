#ifndef OBJ_UIWINSIZE_H
#define OBJ_UIWINSIZE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIWinSize;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIWinSize;

// Object Struct
extern ObjectUIWinSize *UIWinSize;

// Standard Entity Events
void UIWinSize_Update(void);
void UIWinSize_LateUpdate(void);
void UIWinSize_StaticUpdate(void);
void UIWinSize_Draw(void);
void UIWinSize_Create(void* data);
void UIWinSize_StageLoad(void);
void UIWinSize_EditorDraw(void);
void UIWinSize_EditorLoad(void);
void UIWinSize_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIWINSIZE_H
