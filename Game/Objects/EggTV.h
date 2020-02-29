#ifndef OBJ_EGGTV_H
#define OBJ_EGGTV_H

//Object Class
class EggTV : Object {

};

//Entity Class
class EntityEggTV : Entity {

};

//Entity Functions
void EggTV_Update();
void EggTV_EarlyUpdate();
void EggTV_LateUpdate();
void EggTV_Draw();
void EggTV_Setup(void* subtype);
void EggTV_StageLoad();
void EggTV_GetAttributes();

#endif //!OBJ_EGGTV_H
