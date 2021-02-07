#ifndef OBJ_PETALPILE_H
#define OBJ_PETALPILE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPetalPile : Object{

};

// Entity Class
struct EntityPetalPile : Entity {

};

// Object Struct
extern ObjectPetalPile *PetalPile;

// Standard Entity Events
void PetalPile_Update();
void PetalPile_LateUpdate();
void PetalPile_StaticUpdate();
void PetalPile_Draw();
void PetalPile_Create(void* data);
void PetalPile_StageLoad();
void PetalPile_EditorDraw();
void PetalPile_EditorLoad();
void PetalPile_Serialize();

// Extra Entity Functions


#endif //!OBJ_PETALPILE_H
