#ifndef OBJ_UIVSRESULTS_H
#define OBJ_UIVSRESULTS_H

//Object Class
class UIVsResults : Object {

};

//Entity Class
class EntityUIVsResults : Entity {

};

//Entity Functions
void UIVsResults_Update();
void UIVsResults_EarlyUpdate();
void UIVsResults_LateUpdate();
void UIVsResults_Draw();
void UIVsResults_Setup(void* subtype);
void UIVsResults_StageLoad();
void UIVsResults_GetAttributes();

#endif //!OBJ_UIVSRESULTS_H
