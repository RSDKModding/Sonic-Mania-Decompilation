#ifndef OBJ_UIBUTTONLABEL_H
#define OBJ_UIBUTTONLABEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIButtonLabel : public Object {
public:

};

//Entity Class
class EntityUIButtonLabel : public Entity {
public:

};

//Object Entity
ObjectUIButtonLabel UIButtonLabel;

//Entity Functions
void UIButtonLabel_Update();
void UIButtonLabel_EarlyUpdate();
void UIButtonLabel_LateUpdate();
void UIButtonLabel_Draw();
void UIButtonLabel_Setup(void* subtype);
void UIButtonLabel_StageLoad();
void UIButtonLabel_GetAttributes();

#endif //!OBJ_UIBUTTONLABEL_H
