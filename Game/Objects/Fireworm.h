#ifndef OBJ_FIREWORM_H
#define OBJ_FIREWORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectFireworm : public Object {
public:

};

//Entity Class
class EntityFireworm : public Entity {
public:

};

//Object Entity
ObjectFireworm Fireworm;

//Entity Functions
void Fireworm_Update();
void Fireworm_EarlyUpdate();
void Fireworm_LateUpdate();
void Fireworm_Draw();
void Fireworm_Setup(void* subtype);
void Fireworm_StageLoad();
void Fireworm_GetAttributes();

#endif //!OBJ_FIREWORM_H
