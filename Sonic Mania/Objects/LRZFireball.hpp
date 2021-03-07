#ifndef OBJ_LRZFIREBALL_H
#define OBJ_LRZFIREBALL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZFireball : Object {

};

// Entity Class
struct EntityLRZFireball : Entity {

};

// Object Struct
extern ObjectLRZFireball *LRZFireball;

// Standard Entity Events
void LRZFireball_Update();
void LRZFireball_LateUpdate();
void LRZFireball_StaticUpdate();
void LRZFireball_Draw();
void LRZFireball_Create(void* data);
void LRZFireball_StageLoad();
void LRZFireball_EditorDraw();
void LRZFireball_EditorLoad();
void LRZFireball_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZFIREBALL_H
