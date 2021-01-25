#ifndef OBJ_PBL_RING_H
#define OBJ_PBL_RING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPBL_Ring : Object {

};

// Entity Class
struct EntityPBL_Ring : Entity {

};

// Object Entity
extern ObjectPBL_Ring PBL_Ring;

// Standard Entity Events
void PBL_Ring_Update();
void PBL_Ring_LateUpdate();
void PBL_Ring_StaticUpdate();
void PBL_Ring_Draw();
void PBL_Ring_Create(void* data);
void PBL_Ring_StageLoad();
void PBL_Ring_EditorDraw();
void PBL_Ring_EditorLoad();
void PBL_Ring_Serialize();

// Extra Entity Functions


#endif //!OBJ_PBL_RING_H
