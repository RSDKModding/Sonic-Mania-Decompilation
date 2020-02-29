#ifndef OBJ_FIREFLIES_H
#define OBJ_FIREFLIES_H

#include "../../SonicMania.h"

//Object Class
class ObjectFireflies : public Object {
public:

};

//Entity Class
class EntityFireflies : public Entity {
public:

};

//Object Entity
ObjectFireflies Fireflies;

//Entity Functions
void Fireflies_Update();
void Fireflies_EarlyUpdate();
void Fireflies_LateUpdate();
void Fireflies_Draw();
void Fireflies_Setup(void* subtype);
void Fireflies_StageLoad();
void Fireflies_GetAttributes();

#endif //!OBJ_FIREFLIES_H
