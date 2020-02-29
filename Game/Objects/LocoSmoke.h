#ifndef OBJ_LOCOSMOKE_H
#define OBJ_LOCOSMOKE_H

#include "../../SonicMania.h"

//Object Class
class ObjectLocoSmoke : public Object {
public:

};

//Entity Class
class EntityLocoSmoke : public Entity {
public:

};

//Object Entity
ObjectLocoSmoke LocoSmoke;

//Entity Functions
void LocoSmoke_Update();
void LocoSmoke_EarlyUpdate();
void LocoSmoke_LateUpdate();
void LocoSmoke_Draw();
void LocoSmoke_Setup(void* subtype);
void LocoSmoke_StageLoad();
void LocoSmoke_GetAttributes();

#endif //!OBJ_LOCOSMOKE_H
