#ifndef OBJ_MAGPLATFORM_H
#define OBJ_MAGPLATFORM_H

//Object Class
class MagPlatform : Object {

};

//Entity Class
class EntityMagPlatform : Entity {

};

//Entity Functions
void MagPlatform_Update();
void MagPlatform_EarlyUpdate();
void MagPlatform_LateUpdate();
void MagPlatform_Draw();
void MagPlatform_Setup(void* subtype);
void MagPlatform_StageLoad();
void MagPlatform_GetAttributes();

#endif //!OBJ_MAGPLATFORM_H
