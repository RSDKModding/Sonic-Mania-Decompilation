#ifndef OBJ_BSS_COLLECTABLE_H
#define OBJ_BSS_COLLECTABLE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBSS_Collectable : Object {

};

// Entity Class
struct EntityBSS_Collectable : Entity {

};

// Object Struct
extern ObjectBSS_Collectable BSS_Collectable;

// Standard Entity Events
void BSS_Collectable_Update();
void BSS_Collectable_LateUpdate();
void BSS_Collectable_StaticUpdate();
void BSS_Collectable_Draw();
void BSS_Collectable_Create(void* data);
void BSS_Collectable_StageLoad();
void BSS_Collectable_EditorDraw();
void BSS_Collectable_EditorLoad();
void BSS_Collectable_Serialize();

// Extra Entity Functions


#endif //!OBJ_BSS_COLLECTABLE_H
