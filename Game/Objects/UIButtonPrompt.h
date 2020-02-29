#ifndef OBJ_UIBUTTONPROMPT_H
#define OBJ_UIBUTTONPROMPT_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIButtonPrompt : public Object {
public:

};

//Entity Class
class EntityUIButtonPrompt : public Entity {
public:

};

//Object Entity
ObjectUIButtonPrompt UIButtonPrompt;

//Entity Functions
void UIButtonPrompt_Update();
void UIButtonPrompt_EarlyUpdate();
void UIButtonPrompt_LateUpdate();
void UIButtonPrompt_Draw();
void UIButtonPrompt_Setup(void* subtype);
void UIButtonPrompt_StageLoad();
void UIButtonPrompt_GetAttributes();

#endif //!OBJ_UIBUTTONPROMPT_H
