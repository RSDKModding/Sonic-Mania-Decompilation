#ifndef OBJ_UIREPLAYCAROUSEL_H
#define OBJ_UIREPLAYCAROUSEL_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIReplayCarousel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIReplayCarousel;

// Object Struct
extern ObjectUIReplayCarousel *UIReplayCarousel;

// Standard Entity Events
void UIReplayCarousel_Update(void);
void UIReplayCarousel_LateUpdate(void);
void UIReplayCarousel_StaticUpdate(void);
void UIReplayCarousel_Draw(void);
void UIReplayCarousel_Create(void* data);
void UIReplayCarousel_StageLoad(void);
void UIReplayCarousel_EditorDraw(void);
void UIReplayCarousel_EditorLoad(void);
void UIReplayCarousel_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_UIREPLAYCAROUSEL_H
