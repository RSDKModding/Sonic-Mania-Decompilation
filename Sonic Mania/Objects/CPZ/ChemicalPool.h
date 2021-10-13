#ifndef OBJ_CHEMICALPOOL_H
#define OBJ_CHEMICALPOOL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(colour colours[4], { 0xF0F0F0, 0xF0F0F0, 0xE0E0E0, 0xE0E0E0 });
    uint16 aniFrames;
    int table1[0x1000];
    int table2[0x1000];
    int table3[0x1000];
    uint16 field_C016;
    uint16 sfxChemYellow;
    uint16 sfxChemRed;
    uint16 sfxChemChange;
} ObjectChemicalPool;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int type;
    Vector2 size;
    int timer2;
    int timer;
    int tileSizeX;
    int offsetY;
    int field_78;
    int r;
    int g;
    int b;
    int dword88;
    int dword8C;
    int dword90;
    int dword94;
    Hitbox hitbox;
    Vector2 field_A0[66];
    colour colours[4];
    Animator animator;
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
void ChemicalPool_ChangeState(EntityChemicalPool *chemPool, int a1, int a3, int a4, int a5);
void ChemicalPool_ProcessDeformations(void);
void ChemicalPool_SetDeform(int x, int y);
void ChemicalPool_SpawnDebris(int x, int y);
void ChemicalPool_SetupColours(void);
//States
void ChemicalPool_State_HarmfulBlue(void);
void ChemicalPool_State_Green(void);
void ChemicalPool_State_Blue(void);
void ChemicalPool_State_Change(void);

#endif //!OBJ_CHEMICALPOOL_H
