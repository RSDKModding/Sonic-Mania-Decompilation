#ifndef OBJ_COLLAPSINGPLATFORM_H
#define OBJ_COLLAPSINGPLATFORM_H

//Object Class
class CollapsingPlatform : Object {

};

//Entity Class
class EntityCollapsingPlatform : Entity {

};

//Entity Functions
void CollapsingPlatform_Update();
void CollapsingPlatform_EarlyUpdate();
void CollapsingPlatform_LateUpdate();
void CollapsingPlatform_Draw();
void CollapsingPlatform_Setup(void* subtype);
void CollapsingPlatform_StageLoad();
void CollapsingPlatform_GetAttributes();

#endif //!OBJ_COLLAPSINGPLATFORM_H
