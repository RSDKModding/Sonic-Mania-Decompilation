#ifndef OBJ_TURBOTURTLE_H
#define OBJ_TURBOTURTLE_H

//Object Class
class TurboTurtle : Object {

};

//Entity Class
class EntityTurboTurtle : Entity {

};

//Entity Functions
void TurboTurtle_Update();
void TurboTurtle_EarlyUpdate();
void TurboTurtle_LateUpdate();
void TurboTurtle_Draw();
void TurboTurtle_Setup(void* subtype);
void TurboTurtle_StageLoad();
void TurboTurtle_GetAttributes();

#endif //!OBJ_TURBOTURTLE_H
