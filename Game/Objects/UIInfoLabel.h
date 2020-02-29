#ifndef OBJ_UIINFOLABEL_H
#define OBJ_UIINFOLABEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIInfoLabel : public Object {
public:

};

//Entity Class
class EntityUIInfoLabel : public Entity {
public:

};

//Object Entity
ObjectUIInfoLabel UIInfoLabel;

//Entity Functions
void UIInfoLabel_Update();
void UIInfoLabel_EarlyUpdate();
void UIInfoLabel_LateUpdate();
void UIInfoLabel_Draw();
void UIInfoLabel_Setup(void* subtype);
void UIInfoLabel_StageLoad();
void UIInfoLabel_GetAttributes();

#endif //!OBJ_UIINFOLABEL_H
