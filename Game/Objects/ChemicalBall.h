#ifndef OBJ_CHEMICALBALL_H
#define OBJ_CHEMICALBALL_H

#include "../../SonicMania.h"

//Object Class
class ObjectChemicalBall : public Object {
public:

};

//Entity Class
class EntityChemicalBall : public Entity {
public:

};

//Object Entity
ObjectChemicalBall ChemicalBall;

//Entity Functions
void ChemicalBall_Update();
void ChemicalBall_EarlyUpdate();
void ChemicalBall_LateUpdate();
void ChemicalBall_Draw();
void ChemicalBall_Setup(void* subtype);
void ChemicalBall_StageLoad();
void ChemicalBall_GetAttributes();

#endif //!OBJ_CHEMICALBALL_H
