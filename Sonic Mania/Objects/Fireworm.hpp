#ifndef OBJ_FIREWORM_H
#define OBJ_FIREWORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFireworm : Object {

};

// Entity Class
struct EntityFireworm : Entity {

};

// Object Entity
extern ObjectFireworm Fireworm;

// Standard Entity Events
void Fireworm_Update();
void Fireworm_LateUpdate();
void Fireworm_StaticUpdate();
void Fireworm_Draw();
void Fireworm_Create(void* data);
void Fireworm_StageLoad();
void Fireworm_EditorDraw();
void Fireworm_EditorLoad();
void Fireworm_Serialize();

// Extra Entity Functions


#endif //!OBJ_FIREWORM_H
