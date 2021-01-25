#ifndef OBJ_LOCOSMOKE_H
#define OBJ_LOCOSMOKE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLocoSmoke : Object {

};

// Entity Class
struct EntityLocoSmoke : Entity {

};

// Object Entity
extern ObjectLocoSmoke LocoSmoke;

// Standard Entity Events
void LocoSmoke_Update();
void LocoSmoke_LateUpdate();
void LocoSmoke_StaticUpdate();
void LocoSmoke_Draw();
void LocoSmoke_Create(void* data);
void LocoSmoke_StageLoad();
void LocoSmoke_EditorDraw();
void LocoSmoke_EditorLoad();
void LocoSmoke_Serialize();

// Extra Entity Functions


#endif //!OBJ_LOCOSMOKE_H
