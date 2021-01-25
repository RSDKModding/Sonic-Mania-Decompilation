#ifndef OBJ_INVISIBLEBLOCK_H
#define OBJ_INVISIBLEBLOCK_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectInvisibleBlock : Object {

};

// Entity Class
struct EntityInvisibleBlock : Entity {

};

// Object Entity
extern ObjectInvisibleBlock InvisibleBlock;

// Standard Entity Events
void InvisibleBlock_Update();
void InvisibleBlock_LateUpdate();
void InvisibleBlock_StaticUpdate();
void InvisibleBlock_Draw();
void InvisibleBlock_Create(void* data);
void InvisibleBlock_StageLoad();
void InvisibleBlock_EditorDraw();
void InvisibleBlock_EditorLoad();
void InvisibleBlock_Serialize();

// Extra Entity Functions


#endif //!OBJ_INVISIBLEBLOCK_H
