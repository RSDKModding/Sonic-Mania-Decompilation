#ifndef OBJ_UICREDITSTEXT_H
#define OBJ_UICREDITSTEXT_H

//Object Class
class UICreditsText : Object {

};

//Entity Class
class EntityUICreditsText : Entity {

};

//Entity Functions
void UICreditsText_Update();
void UICreditsText_EarlyUpdate();
void UICreditsText_LateUpdate();
void UICreditsText_Draw();
void UICreditsText_Setup(void* subtype);
void UICreditsText_StageLoad();
void UICreditsText_GetAttributes();

#endif //!OBJ_UICREDITSTEXT_H
