#ifndef OBJ_CHEMICALPOOL_H
#define OBJ_CHEMICALPOOL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectChemicalPool : Object {

};

// Entity Class
struct EntityChemicalPool : Entity {

};

// Object Entity
extern ObjectChemicalPool ChemicalPool;

// Standard Entity Events
void ChemicalPool_Update();
void ChemicalPool_LateUpdate();
void ChemicalPool_StaticUpdate();
void ChemicalPool_Draw();
void ChemicalPool_Create(void* data);
void ChemicalPool_StageLoad();
void ChemicalPool_EditorDraw();
void ChemicalPool_EditorLoad();
void ChemicalPool_Serialize();

// Extra Entity Functions


#endif //!OBJ_CHEMICALPOOL_H
