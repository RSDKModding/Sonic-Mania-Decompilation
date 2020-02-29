#ifndef OBJ_SWEEP_H
#define OBJ_SWEEP_H

//Object Class
class Sweep : Object {

};

//Entity Class
class EntitySweep : Entity {

};

//Entity Functions
void Sweep_Update();
void Sweep_EarlyUpdate();
void Sweep_LateUpdate();
void Sweep_Draw();
void Sweep_Setup(void* subtype);
void Sweep_StageLoad();
void Sweep_GetAttributes();

#endif //!OBJ_SWEEP_H
