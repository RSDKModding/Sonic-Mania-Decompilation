#ifndef OBJ_PLATFORM_H
#define OBJ_PLATFORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectPlatform : public Object {
public:

};

//Entity Class
class EntityPlatform : public Entity {
public:

};

//Object Entity
ObjectPlatform Platform;

//Entity Functions
void Platform_Update();
void Platform_EarlyUpdate();
void Platform_LateUpdate();
void Platform_Draw();
void Platform_Setup(void* subtype);
void Platform_StageLoad();
void Platform_GetAttributes();

#endif //!OBJ_PLATFORM_H
