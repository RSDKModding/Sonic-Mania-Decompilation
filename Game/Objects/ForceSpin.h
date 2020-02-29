#ifndef OBJ_FORCESPIN_H
#define OBJ_FORCESPIN_H

//Object Class
class ForceSpin : Object {

};

//Entity Class
class EntityForceSpin : Entity {

};

//Entity Functions
void ForceSpin_Update();
void ForceSpin_EarlyUpdate();
void ForceSpin_LateUpdate();
void ForceSpin_Draw();
void ForceSpin_Setup(void* subtype);
void ForceSpin_StageLoad();
void ForceSpin_GetAttributes();

#endif //!OBJ_FORCESPIN_H
