#ifndef OBJ_UISUBHEADING_H
#define OBJ_UISUBHEADING_H

//Object Class
class UISubHeading : Object {

};

//Entity Class
class EntityUISubHeading : Entity {

};

//Entity Functions
void UISubHeading_Update();
void UISubHeading_EarlyUpdate();
void UISubHeading_LateUpdate();
void UISubHeading_Draw();
void UISubHeading_Setup(void* subtype);
void UISubHeading_StageLoad();
void UISubHeading_GetAttributes();

#endif //!OBJ_UISUBHEADING_H
