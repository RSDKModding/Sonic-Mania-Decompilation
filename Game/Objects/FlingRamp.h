#ifndef OBJ_FLINGRAMP_H
#define OBJ_FLINGRAMP_H

//Object Class
class FlingRamp : Object {

};

//Entity Class
class EntityFlingRamp : Entity {

};

//Entity Functions
void FlingRamp_Update();
void FlingRamp_EarlyUpdate();
void FlingRamp_LateUpdate();
void FlingRamp_Draw();
void FlingRamp_Setup(void* subtype);
void FlingRamp_StageLoad();
void FlingRamp_GetAttributes();

#endif //!OBJ_FLINGRAMP_H
