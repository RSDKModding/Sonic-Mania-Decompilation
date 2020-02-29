#ifndef OBJ_UITABANNER_H
#define OBJ_UITABANNER_H

#include "../../SonicMania.h"

//Object Class
class ObjectUITABanner : public Object {
public:

};

//Entity Class
class EntityUITABanner : public Entity {
public:

};

//Object Entity
ObjectUITABanner UITABanner;

//Entity Functions
void UITABanner_Update();
void UITABanner_EarlyUpdate();
void UITABanner_LateUpdate();
void UITABanner_Draw();
void UITABanner_Setup(void* subtype);
void UITABanner_StageLoad();
void UITABanner_GetAttributes();

#endif //!OBJ_UITABANNER_H
