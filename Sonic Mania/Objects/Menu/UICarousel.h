#ifndef OBJ_UICAROUSEL_H
#define OBJ_UICAROUSEL_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUICarousel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUICarousel;

// Object Struct
extern ObjectUICarousel *UICarousel;

// Standard Entity Events
void UICarousel_Update(void);
void UICarousel_LateUpdate(void);
void UICarousel_StaticUpdate(void);
void UICarousel_Draw(void);
void UICarousel_Create(void* data);
void UICarousel_StageLoad(void);
void UICarousel_EditorDraw(void);
void UICarousel_EditorLoad(void);
void UICarousel_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_UICAROUSEL_H
