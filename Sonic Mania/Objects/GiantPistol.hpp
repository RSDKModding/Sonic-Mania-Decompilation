#ifndef OBJ_GIANTPISTOL_H
#define OBJ_GIANTPISTOL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGiantPistol : Object{

};

// Entity Class
struct EntityGiantPistol : Entity {

};

// Object Struct
extern ObjectGiantPistol *GiantPistol;

// Standard Entity Events
void GiantPistol_Update();
void GiantPistol_LateUpdate();
void GiantPistol_StaticUpdate();
void GiantPistol_Draw();
void GiantPistol_Create(void* data);
void GiantPistol_StageLoad();
void GiantPistol_EditorDraw();
void GiantPistol_EditorLoad();
void GiantPistol_Serialize();

// Extra Entity Functions


#endif //!OBJ_GIANTPISTOL_H
