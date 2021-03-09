#ifndef OBJ_PBL_SETUP_H
#define OBJ_PBL_SETUP_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectPBL_Setup : Object {

};

// Entity Class
struct EntityPBL_Setup : Entity {

};

// Object Struct
extern ObjectPBL_Setup *PBL_Setup;

// Standard Entity Events
void PBL_Setup_Update();
void PBL_Setup_LateUpdate();
void PBL_Setup_StaticUpdate();
void PBL_Setup_Draw();
void PBL_Setup_Create(void* data);
void PBL_Setup_StageLoad();
void PBL_Setup_EditorDraw();
void PBL_Setup_EditorLoad();
void PBL_Setup_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_SETUP_H
