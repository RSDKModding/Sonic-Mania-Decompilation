#ifndef OBJ_UITAZONEMODULE_H
#define OBJ_UITAZONEMODULE_H

#include "../../SonicMania.h"

//Object Class
class ObjectUITAZoneModule : public Object {
public:

};

//Entity Class
class EntityUITAZoneModule : public Entity {
public:

};

//Object Entity
ObjectUITAZoneModule UITAZoneModule;

//Entity Functions
void UITAZoneModule_Update();
void UITAZoneModule_EarlyUpdate();
void UITAZoneModule_LateUpdate();
void UITAZoneModule_Draw();
void UITAZoneModule_Setup(void* subtype);
void UITAZoneModule_StageLoad();
void UITAZoneModule_GetAttributes();

#endif //!OBJ_UITAZONEMODULE_H
