#ifndef OBJ_LAVAFALL_H
#define OBJ_LAVAFALL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLavaFall : Object{

};

// Entity Class
struct EntityLavaFall : Entity {

};

// Object Struct
extern ObjectLavaFall *LavaFall;

// Standard Entity Events
void LavaFall_Update();
void LavaFall_LateUpdate();
void LavaFall_StaticUpdate();
void LavaFall_Draw();
void LavaFall_Create(void* data);
void LavaFall_StageLoad();
void LavaFall_EditorDraw();
void LavaFall_EditorLoad();
void LavaFall_Serialize();

// Extra Entity Functions


#endif //!OBJ_LAVAFALL_H
