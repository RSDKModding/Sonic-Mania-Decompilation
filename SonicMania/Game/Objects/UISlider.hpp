#ifndef OBJ_UISLIDER_H
#define OBJ_UISLIDER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUISlider : Object {

};

// Entity Class
struct EntityUISlider : Entity {

};

// Object Entity
extern ObjectUISlider UISlider;

// Standard Entity Events
void UISlider_Update();
void UISlider_LateUpdate();
void UISlider_StaticUpdate();
void UISlider_Draw();
void UISlider_Create(void* data);
void UISlider_StageLoad();
void UISlider_EditorDraw();
void UISlider_EditorLoad();
void UISlider_Serialize();

// Extra Entity Functions


#endif //!OBJ_UISLIDER_H
