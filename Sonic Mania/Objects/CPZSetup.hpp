#ifndef OBJ_CPZSETUP_H
#define OBJ_CPZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCPZSetup : Object {

};

// Entity Class
struct EntityCPZSetup : Entity {

};

// Object Entity
extern ObjectCPZSetup CPZSetup;

// Standard Entity Events
void CPZSetup_Update();
void CPZSetup_LateUpdate();
void CPZSetup_StaticUpdate();
void CPZSetup_Draw();
void CPZSetup_Create(void* data);
void CPZSetup_StageLoad();
void CPZSetup_EditorDraw();
void CPZSetup_EditorLoad();
void CPZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_CPZSETUP_H
