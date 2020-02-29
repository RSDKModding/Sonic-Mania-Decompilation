#ifndef OBJ_UITEXT_H
#define OBJ_UITEXT_H

//Object Class
class UIText : Object {

};

//Entity Class
class EntityUIText : Entity {

};

//Entity Functions
void UIText_Update();
void UIText_EarlyUpdate();
void UIText_LateUpdate();
void UIText_Draw();
void UIText_Setup(void* subtype);
void UIText_StageLoad();
void UIText_GetAttributes();

#endif //!OBJ_UITEXT_H
