#ifndef OBJ_KANABUN_H
#define OBJ_KANABUN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectKanabun : Object {

};

// Entity Class
struct EntityKanabun : Entity {

};

// Object Entity
extern ObjectKanabun Kanabun;

// Standard Entity Events
void Kanabun_Update();
void Kanabun_LateUpdate();
void Kanabun_StaticUpdate();
void Kanabun_Draw();
void Kanabun_Create(void* data);
void Kanabun_StageLoad();
void Kanabun_EditorDraw();
void Kanabun_EditorLoad();
void Kanabun_Serialize();

// Extra Entity Functions


#endif //!OBJ_KANABUN_H
