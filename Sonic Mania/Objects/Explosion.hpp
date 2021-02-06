#ifndef OBJ_EXPLOSION_H
#define OBJ_EXPLOSION_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectExplosion : Object {

};

// Entity Class
struct EntityExplosion : Entity {

};

// Object Entity
extern ObjectExplosion Explosion;

// Standard Entity Events
void Explosion_Update();
void Explosion_LateUpdate();
void Explosion_StaticUpdate();
void Explosion_Draw();
void Explosion_Create(void* data);
void Explosion_StageLoad();
void Explosion_EditorDraw();
void Explosion_EditorLoad();
void Explosion_Serialize();

// Extra Entity Functions


#endif //!OBJ_EXPLOSION_H
