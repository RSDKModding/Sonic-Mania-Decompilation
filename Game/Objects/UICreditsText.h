#ifndef OBJ_UICREDITSTEXT_H
#define OBJ_UICREDITSTEXT_H

#include "../../SonicMania.h"

//Object Class
class ObjectUICreditsText : public Object {
public:

};

//Entity Class
class EntityUICreditsText : public Entity {
public:

};

//Object Entity
ObjectUICreditsText UICreditsText;

//Entity Functions
void UICreditsText_Update();
void UICreditsText_EarlyUpdate();
void UICreditsText_LateUpdate();
void UICreditsText_Draw();
void UICreditsText_Setup(void* subtype);
void UICreditsText_StageLoad();
void UICreditsText_GetAttributes();

#endif //!OBJ_UICREDITSTEXT_H
