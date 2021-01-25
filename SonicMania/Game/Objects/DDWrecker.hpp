#ifndef OBJ_DDWRECKER_H
#define OBJ_DDWRECKER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDDWrecker : Object {

};

// Entity Class
struct EntityDDWrecker : Entity {

};

// Object Entity
extern ObjectDDWrecker DDWrecker;

// Standard Entity Events
void DDWrecker_Update();
void DDWrecker_LateUpdate();
void DDWrecker_StaticUpdate();
void DDWrecker_Draw();
void DDWrecker_Create(void* data);
void DDWrecker_StageLoad();
void DDWrecker_EditorDraw();
void DDWrecker_EditorLoad();
void DDWrecker_Serialize();

// Extra Entity Functions


#endif //!OBJ_DDWRECKER_H
