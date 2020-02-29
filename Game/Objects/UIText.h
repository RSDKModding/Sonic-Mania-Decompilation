#ifndef OBJ_UITEXT_H
#define OBJ_UITEXT_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIText : public Object {
public:

};

//Entity Class
class EntityUIText : public Entity {
public:

};

//Object Entity
ObjectUIText UIText;

//Entity Functions
void UIText_Update();
void UIText_EarlyUpdate();
void UIText_LateUpdate();
void UIText_Draw();
void UIText_Setup(void* subtype);
void UIText_StageLoad();
void UIText_GetAttributes();

#endif //!OBJ_UITEXT_H
