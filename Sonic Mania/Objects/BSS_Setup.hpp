#ifndef OBJ_BSS_SETUP_H
#define OBJ_BSS_SETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBSS_Setup : Object {

};

// Entity Class
struct EntityBSS_Setup : Entity {

};

// Object Struct
extern ObjectBSS_Setup BSS_Setup;

// Standard Entity Events
void BSS_Setup_Update();
void BSS_Setup_LateUpdate();
void BSS_Setup_StaticUpdate();
void BSS_Setup_Draw();
void BSS_Setup_Create(void* data);
void BSS_Setup_StageLoad();
void BSS_Setup_EditorDraw();
void BSS_Setup_EditorLoad();
void BSS_Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_BSS_SETUP_H
