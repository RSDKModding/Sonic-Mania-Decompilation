#ifndef OBJ_MMZSETUP_H
#define OBJ_MMZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMMZSetup : Object {

};

// Entity Class
struct EntityMMZSetup : Entity {

};

// Object Entity
extern ObjectMMZSetup MMZSetup;

// Standard Entity Events
void MMZSetup_Update();
void MMZSetup_LateUpdate();
void MMZSetup_StaticUpdate();
void MMZSetup_Draw();
void MMZSetup_Create(void* data);
void MMZSetup_StageLoad();
void MMZSetup_EditorDraw();
void MMZSetup_EditorLoad();
void MMZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_MMZSETUP_H
