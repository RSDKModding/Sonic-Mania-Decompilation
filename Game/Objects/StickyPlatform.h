#ifndef OBJ_STICKYPLATFORM_H
#define OBJ_STICKYPLATFORM_H

//Object Class
class StickyPlatform : Object {

};

//Entity Class
class EntityStickyPlatform : Entity {

};

//Entity Functions
void StickyPlatform_Update();
void StickyPlatform_EarlyUpdate();
void StickyPlatform_LateUpdate();
void StickyPlatform_Draw();
void StickyPlatform_Setup(void* subtype);
void StickyPlatform_StageLoad();
void StickyPlatform_GetAttributes();

#endif //!OBJ_STICKYPLATFORM_H
