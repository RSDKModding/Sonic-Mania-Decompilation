#ifndef OBJ_LRZROCKPILE_H
#define OBJ_LRZROCKPILE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZRockPile : Object {

};

// Entity Class
struct EntityLRZRockPile : Entity {

};

// Object Struct
extern ObjectLRZRockPile *LRZRockPile;

// Standard Entity Events
void LRZRockPile_Update();
void LRZRockPile_LateUpdate();
void LRZRockPile_StaticUpdate();
void LRZRockPile_Draw();
void LRZRockPile_Create(void* data);
void LRZRockPile_StageLoad();
void LRZRockPile_EditorDraw();
void LRZRockPile_EditorLoad();
void LRZRockPile_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZROCKPILE_H
