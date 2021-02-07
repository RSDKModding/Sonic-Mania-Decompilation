#ifndef OBJ_ANIMALS_H
#define OBJ_ANIMALS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectAnimals : Object{

};

// Entity Class
struct EntityAnimals : Entity {

};

// Object Struct
extern ObjectAnimals *Animals;

// Standard Entity Events
void Animals_Update();
void Animals_LateUpdate();
void Animals_StaticUpdate();
void Animals_Draw();
void Animals_Create(void* data);
void Animals_StageLoad();
void Animals_EditorDraw();
void Animals_EditorLoad();
void Animals_Serialize();

// Extra Entity Functions


#endif //!OBJ_ANIMALS_H
