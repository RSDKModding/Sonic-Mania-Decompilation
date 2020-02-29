#ifndef OBJ_LARGEGEAR_H
#define OBJ_LARGEGEAR_H

//Object Class
class LargeGear : Object {

};

//Entity Class
class EntityLargeGear : Entity {

};

//Entity Functions
void LargeGear_Update();
void LargeGear_EarlyUpdate();
void LargeGear_LateUpdate();
void LargeGear_Draw();
void LargeGear_Setup(void* subtype);
void LargeGear_StageLoad();
void LargeGear_GetAttributes();

#endif //!OBJ_LARGEGEAR_H
