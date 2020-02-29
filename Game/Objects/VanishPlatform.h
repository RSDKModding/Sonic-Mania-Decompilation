#ifndef OBJ_VANISHPLATFORM_H
#define OBJ_VANISHPLATFORM_H

//Object Class
class VanishPlatform : Object {

};

//Entity Class
class EntityVanishPlatform : Entity {

};

//Entity Functions
void VanishPlatform_Update();
void VanishPlatform_EarlyUpdate();
void VanishPlatform_LateUpdate();
void VanishPlatform_Draw();
void VanishPlatform_Setup(void* subtype);
void VanishPlatform_StageLoad();
void VanishPlatform_GetAttributes();

#endif //!OBJ_VANISHPLATFORM_H
