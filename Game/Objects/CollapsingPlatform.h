#ifndef OBJ_COLLAPSINGPLATFORM_H
#define OBJ_COLLAPSINGPLATFORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectCollapsingPlatform : public Object {
public:

};

//Entity Class
class EntityCollapsingPlatform : public Entity {
public:

};

//Object Entity
ObjectCollapsingPlatform CollapsingPlatform;

//Entity Functions
void CollapsingPlatform_Update();
void CollapsingPlatform_EarlyUpdate();
void CollapsingPlatform_LateUpdate();
void CollapsingPlatform_Draw();
void CollapsingPlatform_Setup(void* subtype);
void CollapsingPlatform_StageLoad();
void CollapsingPlatform_GetAttributes();

#endif //!OBJ_COLLAPSINGPLATFORM_H
