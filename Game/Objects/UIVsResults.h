#ifndef OBJ_UIVSRESULTS_H
#define OBJ_UIVSRESULTS_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIVsResults : public Object {
public:

};

//Entity Class
class EntityUIVsResults : public Entity {
public:

};

//Object Entity
ObjectUIVsResults UIVsResults;

//Entity Functions
void UIVsResults_Update();
void UIVsResults_EarlyUpdate();
void UIVsResults_LateUpdate();
void UIVsResults_Draw();
void UIVsResults_Setup(void* subtype);
void UIVsResults_StageLoad();
void UIVsResults_GetAttributes();

#endif //!OBJ_UIVSRESULTS_H
