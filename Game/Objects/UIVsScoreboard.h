#ifndef OBJ_UIVSSCOREBOARD_H
#define OBJ_UIVSSCOREBOARD_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIVsScoreboard : public Object {
public:

};

//Entity Class
class EntityUIVsScoreboard : public Entity {
public:

};

//Object Entity
ObjectUIVsScoreboard UIVsScoreboard;

//Entity Functions
void UIVsScoreboard_Update();
void UIVsScoreboard_EarlyUpdate();
void UIVsScoreboard_LateUpdate();
void UIVsScoreboard_Draw();
void UIVsScoreboard_Setup(void* subtype);
void UIVsScoreboard_StageLoad();
void UIVsScoreboard_GetAttributes();

#endif //!OBJ_UIVSSCOREBOARD_H
