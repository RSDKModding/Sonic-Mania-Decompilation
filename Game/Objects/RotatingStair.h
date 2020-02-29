#ifndef OBJ_ROTATINGSTAIR_H
#define OBJ_ROTATINGSTAIR_H

//Object Class
class RotatingStair : Object {

};

//Entity Class
class EntityRotatingStair : Entity {

};

//Entity Functions
void RotatingStair_Update();
void RotatingStair_EarlyUpdate();
void RotatingStair_LateUpdate();
void RotatingStair_Draw();
void RotatingStair_Setup(void* subtype);
void RotatingStair_StageLoad();
void RotatingStair_GetAttributes();

#endif //!OBJ_ROTATINGSTAIR_H
