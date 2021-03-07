#ifndef OBJ_DASETUP_H
#define OBJ_DASETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDASetup : Object {

};

// Entity Class
struct EntityDASetup : Entity {

};

// Object Struct
extern ObjectDASetup *DASetup;

// Standard Entity Events
void DASetup_Update();
void DASetup_LateUpdate();
void DASetup_StaticUpdate();
void DASetup_Draw();
void DASetup_Create(void* data);
void DASetup_StageLoad();
void DASetup_EditorDraw();
void DASetup_EditorLoad();
void DASetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_DASETUP_H
