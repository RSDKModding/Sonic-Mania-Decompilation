#ifndef OBJ_MECHABU_H
#define OBJ_MECHABU_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMechaBu : Object{

};

// Entity Class
struct EntityMechaBu : Entity {

};

// Object Struct
extern ObjectMechaBu *MechaBu;

// Standard Entity Events
void MechaBu_Update();
void MechaBu_LateUpdate();
void MechaBu_StaticUpdate();
void MechaBu_Draw();
void MechaBu_Create(void* data);
void MechaBu_StageLoad();
void MechaBu_EditorDraw();
void MechaBu_EditorLoad();
void MechaBu_Serialize();

// Extra Entity Functions


#endif //!OBJ_MECHABU_H
