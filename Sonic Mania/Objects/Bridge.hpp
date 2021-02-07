#ifndef OBJ_BRIDGE_H
#define OBJ_BRIDGE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBridge : Object{

};

// Entity Class
struct EntityBridge : Entity {

};

// Object Struct
extern ObjectBridge *Bridge;

// Standard Entity Events
void Bridge_Update();
void Bridge_LateUpdate();
void Bridge_StaticUpdate();
void Bridge_Draw();
void Bridge_Create(void* data);
void Bridge_StageLoad();
void Bridge_EditorDraw();
void Bridge_EditorLoad();
void Bridge_Serialize();

// Extra Entity Functions


#endif //!OBJ_BRIDGE_H
