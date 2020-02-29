#ifndef OBJ_TURBOTURTLE_H
#define OBJ_TURBOTURTLE_H

#include "../../SonicMania.h"

//Object Class
class ObjectTurboTurtle : public Object {
public:

};

//Entity Class
class EntityTurboTurtle : public Entity {
public:

};

//Object Entity
ObjectTurboTurtle TurboTurtle;

//Entity Functions
void TurboTurtle_Update();
void TurboTurtle_EarlyUpdate();
void TurboTurtle_LateUpdate();
void TurboTurtle_Draw();
void TurboTurtle_Setup(void* subtype);
void TurboTurtle_StageLoad();
void TurboTurtle_GetAttributes();

#endif //!OBJ_TURBOTURTLE_H
