#ifndef OBJ_CHEMICALBALL_H
#define OBJ_CHEMICALBALL_H

//Object Class
class ChemicalBall : Object {

};

//Entity Class
class EntityChemicalBall : Entity {

};

//Entity Functions
void ChemicalBall_Update();
void ChemicalBall_EarlyUpdate();
void ChemicalBall_LateUpdate();
void ChemicalBall_Draw();
void ChemicalBall_Setup(void* subtype);
void ChemicalBall_StageLoad();
void ChemicalBall_GetAttributes();

#endif //!OBJ_CHEMICALBALL_H
