#ifndef OBJ_UITABANNER_H
#define OBJ_UITABANNER_H

//Object Class
class UITABanner : Object {

};

//Entity Class
class EntityUITABanner : Entity {

};

//Entity Functions
void UITABanner_Update();
void UITABanner_EarlyUpdate();
void UITABanner_LateUpdate();
void UITABanner_Draw();
void UITABanner_Setup(void* subtype);
void UITABanner_StageLoad();
void UITABanner_GetAttributes();

#endif //!OBJ_UITABANNER_H
