#ifndef OBJ_PBL_CRANE_H
#define OBJ_PBL_CRANE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPBL_Crane : Object {

};

// Entity Class
struct EntityPBL_Crane : Entity {

};

// Object Entity
extern ObjectPBL_Crane PBL_Crane;

// Standard Entity Events
void PBL_Crane_Update();
void PBL_Crane_LateUpdate();
void PBL_Crane_StaticUpdate();
void PBL_Crane_Draw();
void PBL_Crane_Create(void* data);
void PBL_Crane_StageLoad();
void PBL_Crane_EditorDraw();
void PBL_Crane_EditorLoad();
void PBL_Crane_Serialize();

// Extra Entity Functions


#endif //!OBJ_PBL_CRANE_H
