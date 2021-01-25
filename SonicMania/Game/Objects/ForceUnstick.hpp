#ifndef OBJ_FORCEUNSTICK_H
#define OBJ_FORCEUNSTICK_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectForceUnstick : Object {

};

// Entity Class
struct EntityForceUnstick : Entity {

};

// Object Entity
extern ObjectForceUnstick ForceUnstick;

// Standard Entity Events
void ForceUnstick_Update();
void ForceUnstick_LateUpdate();
void ForceUnstick_StaticUpdate();
void ForceUnstick_Draw();
void ForceUnstick_Create(void* data);
void ForceUnstick_StageLoad();
void ForceUnstick_EditorDraw();
void ForceUnstick_EditorLoad();
void ForceUnstick_Serialize();

// Extra Entity Functions


#endif //!OBJ_FORCEUNSTICK_H
