#ifndef OBJ_CHEMICALBALL_H
#define OBJ_CHEMICALBALL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectChemicalBall : Object {

};

// Entity Class
struct EntityChemicalBall : Entity {

};

// Object Entity
extern ObjectChemicalBall ChemicalBall;

// Standard Entity Events
void ChemicalBall_Update();
void ChemicalBall_LateUpdate();
void ChemicalBall_StaticUpdate();
void ChemicalBall_Draw();
void ChemicalBall_Create(void* data);
void ChemicalBall_StageLoad();
void ChemicalBall_EditorDraw();
void ChemicalBall_EditorLoad();
void ChemicalBall_Serialize();

// Extra Entity Functions


#endif //!OBJ_CHEMICALBALL_H
