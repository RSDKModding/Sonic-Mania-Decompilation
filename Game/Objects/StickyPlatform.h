#ifndef OBJ_STICKYPLATFORM_H
#define OBJ_STICKYPLATFORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectStickyPlatform : public Object {
public:

};

//Entity Class
class EntityStickyPlatform : public Entity {
public:

};

//Object Entity
ObjectStickyPlatform StickyPlatform;

//Entity Functions
void StickyPlatform_Update();
void StickyPlatform_EarlyUpdate();
void StickyPlatform_LateUpdate();
void StickyPlatform_Draw();
void StickyPlatform_Setup(void* subtype);
void StickyPlatform_StageLoad();
void StickyPlatform_GetAttributes();

#endif //!OBJ_STICKYPLATFORM_H
