#ifndef OBJ_LRZROCKPILE_H
#define OBJ_LRZROCKPILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 sfxLedgeBreak;
    uint16 aniFrames;
    uint16 particleFrames;
} ObjectLRZRockPile;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint16 delay;
    bool32 debugParticles;
    bool32 unbreakable;
    PlaneFilterTypes planeFilter;
    bool32 onlyKnux;
#if RETRO_USE_PLUS
    bool32 onlyMighty;
#endif
    int32 debrisInfo[0xC0];
    uint8 debrisCount;
    int32 timer;
    int32 timer2;
    Vector2 startPos;
    Animator animator;
    bool32 flag;
    Hitbox hitbox;
} EntityLRZRockPile;

// Object Struct
extern ObjectLRZRockPile *LRZRockPile;

// Standard Entity Events
void LRZRockPile_Update(void);
void LRZRockPile_LateUpdate(void);
void LRZRockPile_StaticUpdate(void);
void LRZRockPile_Draw(void);
void LRZRockPile_Create(void* data);
void LRZRockPile_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LRZRockPile_EditorDraw(void);
void LRZRockPile_EditorLoad(void);
#endif
void LRZRockPile_Serialize(void);

// Extra Entity Functions
void LRZRockPile_SetupDebris(void);
void LRZRockPile_SpawnRockDebris(int x, int y, int velX, int velY);

void LRZRockPile_State_Type0(void);
void LRZRockPile_State_Type1(void);
void LRZRockPile_State_Type2(void);
void LRZRockPile_State_Broken(void);

#endif //!OBJ_LRZROCKPILE_H
