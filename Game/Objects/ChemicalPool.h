#ifndef OBJ_CHEMICALPOOL_H
#define OBJ_CHEMICALPOOL_H

#include "../../SonicMania.h"

//Object Class
class ObjectChemicalPool : public Object {
public:

};

//Entity Class
class EntityChemicalPool : public Entity {
public:

};

//Object Entity
ObjectChemicalPool ChemicalPool;

//Entity Functions
void ChemicalPool_Update();
void ChemicalPool_EarlyUpdate();
void ChemicalPool_LateUpdate();
void ChemicalPool_Draw();
void ChemicalPool_Setup(void* subtype);
void ChemicalPool_StageLoad();
void ChemicalPool_GetAttributes();

#endif //!OBJ_CHEMICALPOOL_H
