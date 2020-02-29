#ifndef OBJ_UIRANKBUTTON_H
#define OBJ_UIRANKBUTTON_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIRankButton : public Object {
public:

};

//Entity Class
class EntityUIRankButton : public Entity {
public:

};

//Object Entity
ObjectUIRankButton UIRankButton;

//Entity Functions
void UIRankButton_Update();
void UIRankButton_EarlyUpdate();
void UIRankButton_LateUpdate();
void UIRankButton_Draw();
void UIRankButton_Setup(void* subtype);
void UIRankButton_StageLoad();
void UIRankButton_GetAttributes();

#endif //!OBJ_UIRANKBUTTON_H
