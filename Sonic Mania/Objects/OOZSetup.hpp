#ifndef OBJ_OOZSETUP_H
#define OBJ_OOZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectOOZSetup : Object{

};

// Entity Class
struct EntityOOZSetup : Entity {

};

// Object Struct
extern ObjectOOZSetup *OOZSetup;

// Standard Entity Events
void OOZSetup_Update();
void OOZSetup_LateUpdate();
void OOZSetup_StaticUpdate();
void OOZSetup_Draw();
void OOZSetup_Create(void* data);
void OOZSetup_StageLoad();
void OOZSetup_EditorDraw();
void OOZSetup_EditorLoad();
void OOZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_OOZSETUP_H
