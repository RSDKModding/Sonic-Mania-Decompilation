#ifndef OBJ_JUGGLESAW_H
#define OBJ_JUGGLESAW_H

//Object Class
class JuggleSaw : Object {

};

//Entity Class
class EntityJuggleSaw : Entity {

};

//Entity Functions
void JuggleSaw_Update();
void JuggleSaw_EarlyUpdate();
void JuggleSaw_LateUpdate();
void JuggleSaw_Draw();
void JuggleSaw_Setup(void* subtype);
void JuggleSaw_StageLoad();
void JuggleSaw_GetAttributes();

#endif //!OBJ_JUGGLESAW_H
