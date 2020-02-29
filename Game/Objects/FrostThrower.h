#ifndef OBJ_FROSTTHROWER_H
#define OBJ_FROSTTHROWER_H

//Object Class
class FrostThrower : Object {

};

//Entity Class
class EntityFrostThrower : Entity {

};

//Entity Functions
void FrostThrower_Update();
void FrostThrower_EarlyUpdate();
void FrostThrower_LateUpdate();
void FrostThrower_Draw();
void FrostThrower_Setup(void* subtype);
void FrostThrower_StageLoad();
void FrostThrower_GetAttributes();

#endif //!OBJ_FROSTTHROWER_H
