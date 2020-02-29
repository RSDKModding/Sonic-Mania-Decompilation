#ifndef OBJ_FIREWORM_H
#define OBJ_FIREWORM_H

//Object Class
class Fireworm : Object {

};

//Entity Class
class EntityFireworm : Entity {

};

//Entity Functions
void Fireworm_Update();
void Fireworm_EarlyUpdate();
void Fireworm_LateUpdate();
void Fireworm_Draw();
void Fireworm_Setup(void* subtype);
void Fireworm_StageLoad();
void Fireworm_GetAttributes();

#endif //!OBJ_FIREWORM_H
