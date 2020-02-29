#ifndef OBJ_PINATA_H
#define OBJ_PINATA_H

#include "../../SonicMania.h"

//Object Class
class ObjectPinata : public Object {
public:

};

//Entity Class
class EntityPinata : public Entity {
public:

};

//Object Entity
ObjectPinata Pinata;

//Entity Functions
void Pinata_Update();
void Pinata_EarlyUpdate();
void Pinata_LateUpdate();
void Pinata_Draw();
void Pinata_Setup(void* subtype);
void Pinata_StageLoad();
void Pinata_GetAttributes();

#endif //!OBJ_PINATA_H
