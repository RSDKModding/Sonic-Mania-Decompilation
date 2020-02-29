#ifndef OBJ_PLATFORM_H
#define OBJ_PLATFORM_H

//Object Class
class Platform : Object {

};

//Entity Class
class EntityPlatform : Entity {

};

//Entity Functions
void Platform_Update();
void Platform_EarlyUpdate();
void Platform_LateUpdate();
void Platform_Draw();
void Platform_Setup(void* subtype);
void Platform_StageLoad();
void Platform_GetAttributes();

#endif //!OBJ_PLATFORM_H
