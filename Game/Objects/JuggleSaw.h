#ifndef OBJ_JUGGLESAW_H
#define OBJ_JUGGLESAW_H

#include "../../SonicMania.h"

//Object Class
class ObjectJuggleSaw : public Object {
public:

};

//Entity Class
class EntityJuggleSaw : public Entity {
public:

};

//Object Entity
ObjectJuggleSaw JuggleSaw;

//Entity Functions
void JuggleSaw_Update();
void JuggleSaw_EarlyUpdate();
void JuggleSaw_LateUpdate();
void JuggleSaw_Draw();
void JuggleSaw_Setup(void* subtype);
void JuggleSaw_StageLoad();
void JuggleSaw_GetAttributes();

#endif //!OBJ_JUGGLESAW_H
