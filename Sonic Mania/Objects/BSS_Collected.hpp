#ifndef OBJ_BSS_COLLECTED_H
#define OBJ_BSS_COLLECTED_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBSS_Collected : Object {

};

// Entity Class
struct EntityBSS_Collected : Entity {

};

// Object Struct
extern ObjectBSS_Collected BSS_Collected;

// Standard Entity Events
void BSS_Collected_Update();
void BSS_Collected_LateUpdate();
void BSS_Collected_StaticUpdate();
void BSS_Collected_Draw();
void BSS_Collected_Create(void* data);
void BSS_Collected_StageLoad();
void BSS_Collected_EditorDraw();
void BSS_Collected_EditorLoad();
void BSS_Collected_Serialize();

// Extra Entity Functions


#endif //!OBJ_BSS_COLLECTED_H
