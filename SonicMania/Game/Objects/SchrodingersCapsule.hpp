#ifndef OBJ_SCHRODINGERSCAPSULE_H
#define OBJ_SCHRODINGERSCAPSULE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSchrodingersCapsule : Object {

};

// Entity Class
struct EntitySchrodingersCapsule : Entity {

};

// Object Entity
extern ObjectSchrodingersCapsule SchrodingersCapsule;

// Standard Entity Events
void SchrodingersCapsule_Update();
void SchrodingersCapsule_LateUpdate();
void SchrodingersCapsule_StaticUpdate();
void SchrodingersCapsule_Draw();
void SchrodingersCapsule_Create(void* data);
void SchrodingersCapsule_StageLoad();
void SchrodingersCapsule_EditorDraw();
void SchrodingersCapsule_EditorLoad();
void SchrodingersCapsule_Serialize();

// Extra Entity Functions


#endif //!OBJ_SCHRODINGERSCAPSULE_H
