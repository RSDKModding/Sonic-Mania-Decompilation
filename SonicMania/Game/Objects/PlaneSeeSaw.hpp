#ifndef OBJ_PLANESEESAW_H
#define OBJ_PLANESEESAW_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPlaneSeeSaw : Object {

};

// Entity Class
struct EntityPlaneSeeSaw : Entity {

};

// Object Entity
extern ObjectPlaneSeeSaw PlaneSeeSaw;

// Standard Entity Events
void PlaneSeeSaw_Update();
void PlaneSeeSaw_LateUpdate();
void PlaneSeeSaw_StaticUpdate();
void PlaneSeeSaw_Draw();
void PlaneSeeSaw_Create(void* data);
void PlaneSeeSaw_StageLoad();
void PlaneSeeSaw_EditorDraw();
void PlaneSeeSaw_EditorLoad();
void PlaneSeeSaw_Serialize();

// Extra Entity Functions


#endif //!OBJ_PLANESEESAW_H
