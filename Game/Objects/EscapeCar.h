#ifndef OBJ_ESCAPECAR_H
#define OBJ_ESCAPECAR_H

#include "../../SonicMania.h"

//Object Class
class ObjectEscapeCar : public Object {
public:

};

//Entity Class
class EntityEscapeCar : public Entity {
public:

};

//Object Entity
ObjectEscapeCar EscapeCar;

//Entity Functions
void EscapeCar_Update();
void EscapeCar_EarlyUpdate();
void EscapeCar_LateUpdate();
void EscapeCar_Draw();
void EscapeCar_Setup(void* subtype);
void EscapeCar_StageLoad();
void EscapeCar_GetAttributes();

#endif //!OBJ_ESCAPECAR_H
