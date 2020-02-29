#ifndef OBJ_PETALPILE_H
#define OBJ_PETALPILE_H

#include "../../SonicMania.h"

//Object Class
class ObjectPetalPile : public Object {
public:

};

//Entity Class
class EntityPetalPile : public Entity {
public:

};

//Object Entity
ObjectPetalPile PetalPile;

//Entity Functions
void PetalPile_Update();
void PetalPile_EarlyUpdate();
void PetalPile_LateUpdate();
void PetalPile_Draw();
void PetalPile_Setup(void* subtype);
void PetalPile_StageLoad();
void PetalPile_GetAttributes();

#endif //!OBJ_PETALPILE_H
