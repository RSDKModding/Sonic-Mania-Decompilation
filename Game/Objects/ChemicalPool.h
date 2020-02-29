#ifndef OBJ_CHEMICALPOOL_H
#define OBJ_CHEMICALPOOL_H

//Object Class
class ChemicalPool : Object {

};

//Entity Class
class EntityChemicalPool : Entity {

};

//Entity Functions
void ChemicalPool_Update();
void ChemicalPool_EarlyUpdate();
void ChemicalPool_LateUpdate();
void ChemicalPool_Draw();
void ChemicalPool_Setup(void* subtype);
void ChemicalPool_StageLoad();
void ChemicalPool_GetAttributes();

#endif //!OBJ_CHEMICALPOOL_H
