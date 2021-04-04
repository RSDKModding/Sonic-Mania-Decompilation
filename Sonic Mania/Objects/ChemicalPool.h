#ifndef OBJ_CHEMICALPOOL_H
#define OBJ_CHEMICALPOOL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[4]; //= { 15790320, 15790320, 14737632, 14737632 };
    ushort value2;
    int value3;
    int value4;
    int value5;
    ushort value6;
    ushort value7;
    ushort value8;
    ushort value9;
} ObjectChemicalPool;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityChemicalPool;

// Object Struct
extern ObjectChemicalPool *ChemicalPool;

// Standard Entity Events
void ChemicalPool_Update(void);
void ChemicalPool_LateUpdate(void);
void ChemicalPool_StaticUpdate(void);
void ChemicalPool_Draw(void);
void ChemicalPool_Create(void* data);
void ChemicalPool_StageLoad(void);
void ChemicalPool_EditorDraw(void);
void ChemicalPool_EditorLoad(void);
void ChemicalPool_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CHEMICALPOOL_H
