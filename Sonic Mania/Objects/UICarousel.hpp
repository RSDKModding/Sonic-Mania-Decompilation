#ifndef OBJ_UICAROUSEL_H
#define OBJ_UICAROUSEL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUICarousel : Object{

};

// Entity Class
struct EntityUICarousel : Entity {

};

// Object Struct
extern ObjectUICarousel *UICarousel;

// Standard Entity Events
void UICarousel_Update();
void UICarousel_LateUpdate();
void UICarousel_StaticUpdate();
void UICarousel_Draw();
void UICarousel_Create(void* data);
void UICarousel_StageLoad();
void UICarousel_EditorDraw();
void UICarousel_EditorLoad();
void UICarousel_Serialize();

// Extra Entity Functions


#endif //!OBJ_UICAROUSEL_H
