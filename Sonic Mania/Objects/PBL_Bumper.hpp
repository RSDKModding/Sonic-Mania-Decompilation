#ifndef OBJ_PBL_BUMPER_H
#define OBJ_PBL_BUMPER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPBL_Bumper : Object {

};

// Entity Class
struct EntityPBL_Bumper : Entity {

};

// Object Struct
extern ObjectPBL_Bumper PBL_Bumper;

// Standard Entity Events
void PBL_Bumper_Update();
void PBL_Bumper_LateUpdate();
void PBL_Bumper_StaticUpdate();
void PBL_Bumper_Draw();
void PBL_Bumper_Create(void* data);
void PBL_Bumper_StageLoad();
void PBL_Bumper_EditorDraw();
void PBL_Bumper_EditorLoad();
void PBL_Bumper_Serialize();

// Extra Entity Functions


#endif //!OBJ_PBL_BUMPER_H
