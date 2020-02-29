#ifndef OBJ_UIMODEBUTTON_H
#define OBJ_UIMODEBUTTON_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIModeButton : public Object {
public:

};

//Entity Class
class EntityUIModeButton : public Entity {
public:

};

//Object Entity
ObjectUIModeButton UIModeButton;

//Entity Functions
void UIModeButton_Update();
void UIModeButton_EarlyUpdate();
void UIModeButton_LateUpdate();
void UIModeButton_Draw();
void UIModeButton_Setup(void* subtype);
void UIModeButton_StageLoad();
void UIModeButton_GetAttributes();

#endif //!OBJ_UIMODEBUTTON_H
