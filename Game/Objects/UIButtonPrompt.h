#ifndef OBJ_UIBUTTONPROMPT_H
#define OBJ_UIBUTTONPROMPT_H

//Object Class
class UIButtonPrompt : Object {

};

//Entity Class
class EntityUIButtonPrompt : Entity {

};

//Entity Functions
void UIButtonPrompt_Update();
void UIButtonPrompt_EarlyUpdate();
void UIButtonPrompt_LateUpdate();
void UIButtonPrompt_Draw();
void UIButtonPrompt_Setup(void* subtype);
void UIButtonPrompt_StageLoad();
void UIButtonPrompt_GetAttributes();

#endif //!OBJ_UIBUTTONPROMPT_H
