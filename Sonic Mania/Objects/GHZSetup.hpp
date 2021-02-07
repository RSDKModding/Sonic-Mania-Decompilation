#ifndef OBJ_GHZSETUP_H
#define OBJ_GHZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGHZSetup : Object{

};

// Entity Class
struct EntityGHZSetup : Entity {

};

// Object Struct
extern ObjectGHZSetup *GHZSetup;

// Standard Entity Events
void GHZSetup_Update();
void GHZSetup_LateUpdate();
void GHZSetup_StaticUpdate();
void GHZSetup_Draw();
void GHZSetup_Create(void* data);
void GHZSetup_StageLoad();
void GHZSetup_EditorDraw();
void GHZSetup_EditorLoad();
void GHZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_GHZSETUP_H
