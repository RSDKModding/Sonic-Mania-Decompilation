#ifndef OBJ_MSBOMB_H
#define OBJ_MSBOMB_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectMSBomb : Object {

};

// Entity Class
struct EntityMSBomb : Entity {

};

// Object Struct
extern ObjectMSBomb *MSBomb;

// Standard Entity Events
void MSBomb_Update();
void MSBomb_LateUpdate();
void MSBomb_StaticUpdate();
void MSBomb_Draw();
void MSBomb_Create(void* data);
void MSBomb_StageLoad();
void MSBomb_EditorDraw();
void MSBomb_EditorLoad();
void MSBomb_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_MSBOMB_H
