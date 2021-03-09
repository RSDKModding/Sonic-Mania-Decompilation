#ifndef OBJ_UIREPLAYCAROUSEL_H
#define OBJ_UIREPLAYCAROUSEL_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectUIReplayCarousel : Object {

};

// Entity Class
struct EntityUIReplayCarousel : Entity {

};

// Object Struct
extern ObjectUIReplayCarousel *UIReplayCarousel;

// Standard Entity Events
void UIReplayCarousel_Update();
void UIReplayCarousel_LateUpdate();
void UIReplayCarousel_StaticUpdate();
void UIReplayCarousel_Draw();
void UIReplayCarousel_Create(void* data);
void UIReplayCarousel_StageLoad();
void UIReplayCarousel_EditorDraw();
void UIReplayCarousel_EditorLoad();
void UIReplayCarousel_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_UIREPLAYCAROUSEL_H
