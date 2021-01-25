#ifndef OBJ_FBZTRASH_H
#define OBJ_FBZTRASH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFBZTrash : Object {

};

// Entity Class
struct EntityFBZTrash : Entity {

};

// Object Entity
extern ObjectFBZTrash FBZTrash;

// Standard Entity Events
void FBZTrash_Update();
void FBZTrash_LateUpdate();
void FBZTrash_StaticUpdate();
void FBZTrash_Draw();
void FBZTrash_Create(void* data);
void FBZTrash_StageLoad();
void FBZTrash_EditorDraw();
void FBZTrash_EditorLoad();
void FBZTrash_Serialize();

// Extra Entity Functions


#endif //!OBJ_FBZTRASH_H
