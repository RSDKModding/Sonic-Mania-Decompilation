#ifndef OBJ_BIGSQUEEZE_H
#define OBJ_BIGSQUEEZE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBigSqueeze : Object {

};

// Entity Class
struct EntityBigSqueeze : Entity {

};

// Object Entity
extern ObjectBigSqueeze BigSqueeze;

// Standard Entity Events
void BigSqueeze_Update();
void BigSqueeze_LateUpdate();
void BigSqueeze_StaticUpdate();
void BigSqueeze_Draw();
void BigSqueeze_Create(void* data);
void BigSqueeze_StageLoad();
void BigSqueeze_EditorDraw();
void BigSqueeze_EditorLoad();
void BigSqueeze_Serialize();

// Extra Entity Functions


#endif //!OBJ_BIGSQUEEZE_H
