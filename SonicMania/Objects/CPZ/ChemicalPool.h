#ifndef OBJ_CHEMICALPOOL_H
#define OBJ_CHEMICALPOOL_H

#include "Game.h"

typedef enum { CHEMICALPOOL_BLUE, CHEMICALPOOL_GREEN, CHEMICALPOOL_CYAN } ChemicalPoolTypes;

// Object Class
struct ObjectChemicalPool {
    RSDK_OBJECT
    TABLE(color surfaceColorsFlash[4], { 0xF0F0F0, 0xF0F0F0, 0xE0E0E0, 0xE0E0E0 });
    uint16 aniFrames;
    int32 surfaceDeformation[0x1000];
    int32 deformTable[0x1000];
    int32 impactTable[0x1000];
    uint16 unused; // exists, it's in the static object, but never used
    uint16 sfxChemYellow;
    uint16 sfxChemRed;
    uint16 sfxChemChange;
};

// Entity Class
struct EntityChemicalPool {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    Vector2 size;
    int32 changeTimer;
    int32 timer;
    int32 tileSizeX;
    int32 offsetY;
    int32 maxDeform;
    int32 r;
    int32 g;
    int32 b;
    int32 leftEdge;
    int32 rightEdge;
    int32 impactPower;
    int32 impactPowerSides;
    Hitbox hitbox;
    Vector2 vertices[66];
    color surfaceColors[4];
    Animator animator;
};

// Object Struct
extern ObjectChemicalPool *ChemicalPool;

// Standard Entity Events
void ChemicalPool_Update(void);
void ChemicalPool_LateUpdate(void);
void ChemicalPool_StaticUpdate(void);
void ChemicalPool_Draw(void);
void ChemicalPool_Create(void *data);
void ChemicalPool_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ChemicalPool_EditorDraw(void);
void ChemicalPool_EditorLoad(void);
#endif
void ChemicalPool_Serialize(void);

// Extra Entity Functions
void ChemicalPool_ChangeState(EntityChemicalPool *chemPool, int32 newType, int32 newR, int32 newG, int32 newB);
void ChemicalPool_ProcessDeformations(void);
void ChemicalPool_SetDeform(int32 impactX, int32 impactVelocity);
void ChemicalPool_SpawnDebris(int32 x, int32 y);
void ChemicalPool_SetupColors(void);
// States
void ChemicalPool_State_HarmfulBlue(void);
void ChemicalPool_State_Green(void);
void ChemicalPool_State_Cyan(void);
void ChemicalPool_State_Changing(void);

#endif //! OBJ_CHEMICALPOOL_H
