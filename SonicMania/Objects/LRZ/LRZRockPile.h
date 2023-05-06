#ifndef OBJ_LRZROCKPILE_H
#define OBJ_LRZROCKPILE_H

#include "Game.h"

typedef enum {
    LRZROCKPILE_WALL,
    LRZROCKPILE_FLOOR_TOPSOLID,
    LRZROCKPILE_FLOOR_ALLSOLID,
} LRZRockPileTypes;

// Object Class
struct ObjectLRZRockPile {
    RSDK_OBJECT
    Hitbox hitboxWall;
    Hitbox hitboxFloorTopSolid;
    Hitbox hitboxFloorAllSolid;
    uint16 sfxLedgeBreak;
    uint16 aniFrames;
    uint16 particleFrames;
};

// Entity Class
struct EntityLRZRockPile {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint16 delay;
    bool32 debugParticles;
    bool32 unbreakable;
    PlaneFilterTypes planeFilter;
    bool32 onlyKnux;
#if MANIA_USE_PLUS
    bool32 onlyMighty;
#endif
    int32 rockDebrisInfo[32 * 6];
    uint8 debrisCount;
    int32 timer;
    int32 destroyTimer;
    Vector2 startPos;
    Animator animator;
    bool32 canCollapse;
    Hitbox hitbox;
};

// Object Struct
extern ObjectLRZRockPile *LRZRockPile;

// Standard Entity Events
void LRZRockPile_Update(void);
void LRZRockPile_LateUpdate(void);
void LRZRockPile_StaticUpdate(void);
void LRZRockPile_Draw(void);
void LRZRockPile_Create(void *data);
void LRZRockPile_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LRZRockPile_EditorDraw(void);
void LRZRockPile_EditorLoad(void);
#endif
void LRZRockPile_Serialize(void);

// Extra Entity Functions
void LRZRockPile_SetupDebris(void);
void LRZRockPile_SpawnRockDebris(int32 x, int32 y, int32 velX, int32 velY);

void LRZRockPile_State_Wall(void);
void LRZRockPile_State_Floor_TopSolid(void);
void LRZRockPile_State_Floor_AllSolid(void);
void LRZRockPile_State_Broken(void);

#endif //! OBJ_LRZROCKPILE_H
