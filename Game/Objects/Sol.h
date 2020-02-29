#ifndef OBJ_SOL_H
#define OBJ_SOL_H

//Object Class
class Sol : Object {

};

//Entity Class
class EntitySol : Entity {

};

//Entity Functions
void Sol_Update();
void Sol_EarlyUpdate();
void Sol_LateUpdate();
void Sol_Draw();
void Sol_Setup(void* subtype);
void Sol_StageLoad();
void Sol_GetAttributes();

#endif //!OBJ_SOL_H
