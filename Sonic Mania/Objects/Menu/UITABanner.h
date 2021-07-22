#ifndef OBJ_UITABANNER_H
#define OBJ_UITABANNER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUITABanner;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUITABanner;

// Object Struct
extern ObjectUITABanner *UITABanner;

// Standard Entity Events
void UITABanner_Update(void);
void UITABanner_LateUpdate(void);
void UITABanner_StaticUpdate(void);
void UITABanner_Draw(void);
void UITABanner_Create(void* data);
void UITABanner_StageLoad(void);
void UITABanner_EditorDraw(void);
void UITABanner_EditorLoad(void);
void UITABanner_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_UITABANNER_H
