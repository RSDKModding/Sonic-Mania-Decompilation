#ifndef OBJ_UISUBHEADING_H
#define OBJ_UISUBHEADING_H

#include "../../SonicMania.h"

//Object Class
class ObjectUISubHeading : public Object {
public:

};

//Entity Class
class EntityUISubHeading : public Entity {
public:

};

//Object Entity
ObjectUISubHeading UISubHeading;

//Entity Functions
void UISubHeading_Update();
void UISubHeading_EarlyUpdate();
void UISubHeading_LateUpdate();
void UISubHeading_Draw();
void UISubHeading_Setup(void* subtype);
void UISubHeading_StageLoad();
void UISubHeading_GetAttributes();

#endif //!OBJ_UISUBHEADING_H
