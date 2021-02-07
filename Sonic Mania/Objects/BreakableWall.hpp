#ifndef OBJ_BREAKABLEWALL_H
#define OBJ_BREAKABLEWALL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBreakableWall : Object{

};

// Entity Class
struct EntityBreakableWall : Entity {

};

// Object Struct
extern ObjectBreakableWall *BreakableWall;

// Standard Entity Events
void BreakableWall_Update();
void BreakableWall_LateUpdate();
void BreakableWall_StaticUpdate();
void BreakableWall_Draw();
void BreakableWall_Create(void* data);
void BreakableWall_StageLoad();
void BreakableWall_EditorDraw();
void BreakableWall_EditorLoad();
void BreakableWall_Serialize();

// Extra Entity Functions


#endif //!OBJ_BREAKABLEWALL_H
