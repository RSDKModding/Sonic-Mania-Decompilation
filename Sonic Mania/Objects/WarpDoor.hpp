#ifndef OBJ_WARPDOOR_H
#define OBJ_WARPDOOR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectWarpDoor : Object {

};

// Entity Class
struct EntityWarpDoor : Entity {

};

// Object Entity
extern ObjectWarpDoor WarpDoor;

// Standard Entity Events
void WarpDoor_Update();
void WarpDoor_LateUpdate();
void WarpDoor_StaticUpdate();
void WarpDoor_Draw();
void WarpDoor_Create(void* data);
void WarpDoor_StageLoad();
void WarpDoor_EditorDraw();
void WarpDoor_EditorLoad();
void WarpDoor_Serialize();

// Extra Entity Functions


#endif //!OBJ_WARPDOOR_H
