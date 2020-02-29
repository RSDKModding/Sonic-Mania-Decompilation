#ifndef OBJ_KANABUN_H
#define OBJ_KANABUN_H

#include "../../SonicMania.h"

//Object Class
class ObjectKanabun : public Object {
public:

};

//Entity Class
class EntityKanabun : public Entity {
public:

};

//Object Entity
ObjectKanabun Kanabun;

//Entity Functions
void Kanabun_Update();
void Kanabun_EarlyUpdate();
void Kanabun_LateUpdate();
void Kanabun_Draw();
void Kanabun_Setup(void* subtype);
void Kanabun_StageLoad();
void Kanabun_GetAttributes();

#endif //!OBJ_KANABUN_H
