#ifndef OBJ_UIVSCHARSELECTOR_H
#define OBJ_UIVSCHARSELECTOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIVsCharSelector : public Object {
public:

};

//Entity Class
class EntityUIVsCharSelector : public Entity {
public:

};

//Object Entity
ObjectUIVsCharSelector UIVsCharSelector;

//Entity Functions
void UIVsCharSelector_Update();
void UIVsCharSelector_EarlyUpdate();
void UIVsCharSelector_LateUpdate();
void UIVsCharSelector_Draw();
void UIVsCharSelector_Setup(void* subtype);
void UIVsCharSelector_StageLoad();
void UIVsCharSelector_GetAttributes();

#endif //!OBJ_UIVSCHARSELECTOR_H
