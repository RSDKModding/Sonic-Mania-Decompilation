#ifndef OBJ_REAGENT_H
#define OBJ_REAGENT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectReagent : Object{

};

// Entity Class
struct EntityReagent : Entity {

};

// Object Struct
extern ObjectReagent *Reagent;

// Standard Entity Events
void Reagent_Update();
void Reagent_LateUpdate();
void Reagent_StaticUpdate();
void Reagent_Draw();
void Reagent_Create(void* data);
void Reagent_StageLoad();
void Reagent_EditorDraw();
void Reagent_EditorLoad();
void Reagent_Serialize();

// Extra Entity Functions


#endif //!OBJ_REAGENT_H
