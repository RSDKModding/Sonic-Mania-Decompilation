#ifndef OBJ_UICAROUSEL_H
#define OBJ_UICAROUSEL_H

//Object Class
class UICarousel : Object {

};

//Entity Class
class EntityUICarousel : Entity {

};

//Entity Functions
void UICarousel_Update();
void UICarousel_EarlyUpdate();
void UICarousel_LateUpdate();
void UICarousel_Draw();
void UICarousel_Setup(void* subtype);
void UICarousel_StageLoad();
void UICarousel_GetAttributes();

#endif //!OBJ_UICAROUSEL_H
