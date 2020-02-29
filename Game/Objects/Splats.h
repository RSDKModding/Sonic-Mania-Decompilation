#ifndef OBJ_SPLATS_H
#define OBJ_SPLATS_H

#include "../../SonicMania.h"

//Object Class
class ObjectSplats : public Object {
public:

};

//Entity Class
class EntitySplats : public Entity {
public:

};

//Object Entity
ObjectSplats Splats;

//Entity Functions
void Splats_Update();
void Splats_EarlyUpdate();
void Splats_LateUpdate();
void Splats_Draw();
void Splats_Setup(void* subtype);
void Splats_StageLoad();
void Splats_GetAttributes();

#endif //!OBJ_SPLATS_H
