#ifndef OBJ_AIZROCKPILE_H
#define OBJ_AIZROCKPILE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectAIZRockPile : Object {

};

// Entity Class
struct EntityAIZRockPile : Entity {

};

// Object Entity
extern ObjectAIZRockPile AIZRockPile;

// Standard Entity Events
void AIZRockPile_Update();
void AIZRockPile_LateUpdate();
void AIZRockPile_StaticUpdate();
void AIZRockPile_Draw();
void AIZRockPile_Create(void* data);
void AIZRockPile_StageLoad();
void AIZRockPile_EditorDraw();
void AIZRockPile_EditorLoad();
void AIZRockPile_Serialize();

// Extra Entity Functions


#endif //!OBJ_AIZROCKPILE_H
