#ifndef OBJ_UICAROUSEL_H
#define OBJ_UICAROUSEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectUICarousel : public Object {
public:

};

//Entity Class
class EntityUICarousel : public Entity {
public:

};

//Object Entity
ObjectUICarousel UICarousel;

//Entity Functions
void UICarousel_Update();
void UICarousel_EarlyUpdate();
void UICarousel_LateUpdate();
void UICarousel_Draw();
void UICarousel_Setup(void* subtype);
void UICarousel_StageLoad();
void UICarousel_GetAttributes();

#endif //!OBJ_UICAROUSEL_H
