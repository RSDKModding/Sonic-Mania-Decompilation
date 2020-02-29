#ifndef OBJ_UIVIDEO_H
#define OBJ_UIVIDEO_H

//Object Class
class UIVideo : Object {

};

//Entity Class
class EntityUIVideo : Entity {

};

//Entity Functions
void UIVideo_Update();
void UIVideo_EarlyUpdate();
void UIVideo_LateUpdate();
void UIVideo_Draw();
void UIVideo_Setup(void* subtype);
void UIVideo_StageLoad();
void UIVideo_GetAttributes();

#endif //!OBJ_UIVIDEO_H
