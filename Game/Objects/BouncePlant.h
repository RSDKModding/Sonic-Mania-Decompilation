#ifndef OBJ_BOUNCEPLANT_H
#define OBJ_BOUNCEPLANT_H

#include "../../SonicMania.h"

//Object Class
class ObjectBouncePlant : public Object {
public:

};

//Entity Class
class EntityBouncePlant : public Entity {
public:

};

//Object Entity
ObjectBouncePlant BouncePlant;

//Entity Functions
void BouncePlant_Update();
void BouncePlant_EarlyUpdate();
void BouncePlant_LateUpdate();
void BouncePlant_Draw();
void BouncePlant_Setup(void* subtype);
void BouncePlant_StageLoad();
void BouncePlant_GetAttributes();

#endif //!OBJ_BOUNCEPLANT_H
