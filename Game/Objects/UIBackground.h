#ifndef OBJ_UIBACKGROUND_H
#define OBJ_UIBACKGROUND_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIBackground : public Object {
public:

};

//Entity Class
class EntityUIBackground : public Entity {
public:

};

//Object Entity
ObjectUIBackground UIBackground;

//Entity Functions
void UIBackground_Update();
void UIBackground_EarlyUpdate();
void UIBackground_LateUpdate();
void UIBackground_Draw();
void UIBackground_Setup(void* subtype);
void UIBackground_StageLoad();
void UIBackground_GetAttributes();

#endif //!OBJ_UIBACKGROUND_H
