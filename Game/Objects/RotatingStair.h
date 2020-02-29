#ifndef OBJ_ROTATINGSTAIR_H
#define OBJ_ROTATINGSTAIR_H

#include "../../SonicMania.h"

//Object Class
class ObjectRotatingStair : public Object {
public:

};

//Entity Class
class EntityRotatingStair : public Entity {
public:

};

//Object Entity
ObjectRotatingStair RotatingStair;

//Entity Functions
void RotatingStair_Update();
void RotatingStair_EarlyUpdate();
void RotatingStair_LateUpdate();
void RotatingStair_Draw();
void RotatingStair_Setup(void* subtype);
void RotatingStair_StageLoad();
void RotatingStair_GetAttributes();

#endif //!OBJ_ROTATINGSTAIR_H
