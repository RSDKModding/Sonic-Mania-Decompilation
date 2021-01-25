#ifndef OBJ_ERZRIDER_H
#define OBJ_ERZRIDER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectERZRider : Object {

};

// Entity Class
struct EntityERZRider : Entity {

};

// Object Entity
extern ObjectERZRider ERZRider;

// Standard Entity Events
void ERZRider_Update();
void ERZRider_LateUpdate();
void ERZRider_StaticUpdate();
void ERZRider_Draw();
void ERZRider_Create(void* data);
void ERZRider_StageLoad();
void ERZRider_EditorDraw();
void ERZRider_EditorLoad();
void ERZRider_Serialize();

// Extra Entity Functions


#endif //!OBJ_ERZRIDER_H
