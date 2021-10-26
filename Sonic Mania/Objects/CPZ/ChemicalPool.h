#ifndef OBJ_CHEMICALPOOL_H
#define OBJ_CHEMICALPOOL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(colour colours[4], { 0xF0F0F0, 0xF0F0F0, 0xE0E0E0, 0xE0E0E0 });
    uint16 aniFrames;
    int32 table1[0x1000];
    int32 table2[0x1000];
    int32 table3[0x1000];
    uint16 field_C016;
    uint16 sfxChemYellow;
    uint16 sfxChemRed;
    uint16 sfxChemChange;
} ObjectChemicalPool;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    Vector2 size;
    int32 timer2;
    int32 timer;
    int32 tileSizeX;
    int32 offsetY;
    int32 field_78;
    int32 r;
    int32 g;
    int32 b;
    int32 dword88;
    int32 dword8C;
    int32 dword90;
    int32 dword94;
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
#if RETRO_INCLUDE_EDITOR
void ChemicalPool_EditorDraw(void);
void ChemicalPool_EditorLoad(void);
#endif
void ChemicalPool_Serialize(void);

// Extra Entity Functions
void ChemicalPool_ChangeState(EntityChemicalPool *chemPool, int32 a1, int32 a3, int32 a4, int32 a5);
void ChemicalPool_ProcessDeformations(void);
void ChemicalPool_SetDeform(int32 x, int32 y);
void ChemicalPool_SpawnDebris(int32 x, int32 y);
void ChemicalPool_SetupColours(void);
//States
void ChemicalPool_State_HarmfulBlue(void);
void ChemicalPool_State_Green(void);
void ChemicalPool_State_Blue(void);
void ChemicalPool_State_Change(void);

#endif //!OBJ_CHEMICALPOOL_H
