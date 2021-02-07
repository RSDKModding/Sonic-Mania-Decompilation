#ifndef OBJ_HPZEMERALD_H
#define OBJ_HPZEMERALD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHPZEmerald : Object{

};

// Entity Class
struct EntityHPZEmerald : Entity {

};

// Object Struct
extern ObjectHPZEmerald *HPZEmerald;

// Standard Entity Events
void HPZEmerald_Update();
void HPZEmerald_LateUpdate();
void HPZEmerald_StaticUpdate();
void HPZEmerald_Draw();
void HPZEmerald_Create(void* data);
void HPZEmerald_StageLoad();
void HPZEmerald_EditorDraw();
void HPZEmerald_EditorLoad();
void HPZEmerald_Serialize();

// Extra Entity Functions


#endif //!OBJ_HPZEMERALD_H
