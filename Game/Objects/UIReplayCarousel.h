#ifndef OBJ_UIREPLAYCAROUSEL_H
#define OBJ_UIREPLAYCAROUSEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIReplayCarousel : public Object {
public:

};

//Entity Class
class EntityUIReplayCarousel : public Entity {
public:

};

//Object Entity
ObjectUIReplayCarousel UIReplayCarousel;

//Entity Functions
void UIReplayCarousel_Update();
void UIReplayCarousel_EarlyUpdate();
void UIReplayCarousel_LateUpdate();
void UIReplayCarousel_Draw();
void UIReplayCarousel_Setup(void* subtype);
void UIReplayCarousel_StageLoad();
void UIReplayCarousel_GetAttributes();

#endif //!OBJ_UIREPLAYCAROUSEL_H
