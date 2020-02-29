#ifndef OBJ_UIVSSCOREBOARD_H
#define OBJ_UIVSSCOREBOARD_H

//Object Class
class UIVsScoreboard : Object {

};

//Entity Class
class EntityUIVsScoreboard : Entity {

};

//Entity Functions
void UIVsScoreboard_Update();
void UIVsScoreboard_EarlyUpdate();
void UIVsScoreboard_LateUpdate();
void UIVsScoreboard_Draw();
void UIVsScoreboard_Setup(void* subtype);
void UIVsScoreboard_StageLoad();
void UIVsScoreboard_GetAttributes();

#endif //!OBJ_UIVSSCOREBOARD_H
