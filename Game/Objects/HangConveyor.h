#ifndef OBJ_HANGCONVEYOR_H
#define OBJ_HANGCONVEYOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectHangConveyor : public Object {
public:

};

//Entity Class
class EntityHangConveyor : public Entity {
public:

};

//Object Entity
ObjectHangConveyor HangConveyor;

//Entity Functions
void HangConveyor_Update();
void HangConveyor_EarlyUpdate();
void HangConveyor_LateUpdate();
void HangConveyor_Draw();
void HangConveyor_Setup(void* subtype);
void HangConveyor_StageLoad();
void HangConveyor_GetAttributes();

#endif //!OBJ_HANGCONVEYOR_H
