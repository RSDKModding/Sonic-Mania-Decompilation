#ifndef OBJ_GONDOLA_H
#define OBJ_GONDOLA_H

#include "../../SonicMania.h"

//Object Class
class ObjectGondola : public Object {
public:

};

//Entity Class
class EntityGondola : public Entity {
public:

};

//Object Entity
ObjectGondola Gondola;

//Entity Functions
void Gondola_Update();
void Gondola_EarlyUpdate();
void Gondola_LateUpdate();
void Gondola_Draw();
void Gondola_Setup(void* subtype);
void Gondola_StageLoad();
void Gondola_GetAttributes();

#endif //!OBJ_GONDOLA_H
