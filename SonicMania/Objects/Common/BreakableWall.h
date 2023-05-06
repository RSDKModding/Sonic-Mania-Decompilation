#ifndef OBJ_BREAKABLEWALL_H
#define OBJ_BREAKABLEWALL_H

#include "Game.h"

typedef enum {
    BREAKWALL_TYPE_WALL,
    BREAKWALL_TYPE_FLOOR,
    BREAKWALL_TYPE_BURROWFLOOR,
    BREAKWALL_TYPE_BURROWFLOOR_B,
    BREAKWALL_TYPE_BURROWFLOORUP,
    BREAKWALL_TYPE_CEILING
} BreakableWallTypes;

typedef enum {
    BREAKWALL_PRIO_HIGH,
    BREAKWALL_PRIO_LOW,
} BreakableWallPriorities;

typedef enum {
    BREAKWALL_TILE_FIXED = 1,
    BREAKWALL_TILE_DYNAMIC,
} BreakableWallTileTypes;

// Object Class
struct ObjectBreakableWall {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
    // possibly used before math (and in v4); these are reportedly velocity tables
    TABLE(int32 unused1[16], { -0x80000, -0x180000, 0x80000, -0x180000, -0x80000, -0x80000, 0x80000, -0x80000, -0x80000, 0x80000, 0x80000, 0x80000,
                               -0x80000, 0x180000, 0x80000, 0x180000 });
    TABLE(int32 unused2[16], { -0x60000, -0x60000, -0x40000, -0x50000, -0x120000, -0x20000, -0x60000, -0x10000, -0x80000, 0x20000, -0x60000, 0x10000,
                               -0x60000, 0x60000, -0x40000, 0x50000 });
    TABLE(int32 breakOffsets[16], { 0x40000, -0x50000, 0x60000, -0x60000, 0x60000, -0x10000, 0x80000, -0x20000, 0x60000, 0x10000, 0x80000, 0x20000,
                                    0x40000, 0x50000, 0x60000, 0x60000 });
    TABLE(int32 breakVelocitiesL[8], { -0x80000, -0x80000, 0x80000, -0x80000, -0x80000, 0x80000, 0x80000, 0x80000 });
    TABLE(int32 breakVelocitiesR[8], { -0x20000, -0x20000, 0x20000, -0x20000, -0x10000, -0x10000, 0x10000, -0x10000 });
    uint16 sfxBreak;
    uint16 farPlaneLayer;
};

// Entity Class
struct EntityBreakableWall {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    uint8 type;
    bool32 onlyKnux;
    bool32 onlyMighty;
    BreakableWallPriorities priority;
    Vector2 size;
    uint16 tileInfo;
    uint16 targetLayer;
    int32 timer;
    Vector2 tilePos;
    int32 tileRotation;
    int32 gravityStrength;
    Hitbox hitbox;
};

// Object Struct
extern ObjectBreakableWall *BreakableWall;

// Standard Entity Events
void BreakableWall_Update(void);
void BreakableWall_LateUpdate(void);
void BreakableWall_StaticUpdate(void);
void BreakableWall_Draw(void);
void BreakableWall_Create(void *data);
void BreakableWall_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BreakableWall_EditorDraw(void);
void BreakableWall_EditorLoad(void);
#endif
void BreakableWall_Serialize(void);

// Extra Entity Functions

// States
void BreakableWall_State_FallingTile(void);
void BreakableWall_State_Tile(void);
void BreakableWall_State_Wall(void);
void BreakableWall_State_Floor(void);
void BreakableWall_State_BurrowFloor(void);
void BreakableWall_State_BurrowFloorUp(void);
void BreakableWall_State_Ceiling(void);

// Draw States
void BreakableWall_Draw_Wall(void);
void BreakableWall_Draw_Floor(void);
void BreakableWall_Draw_Tile(void);

// Breaking
void BreakableWall_CheckBreak_Wall(void);
void BreakableWall_CheckBreak_Floor(void);
void BreakableWall_CheckBreak_BurrowFloor(void);
void BreakableWall_CheckBreak_BurrowFloorUp(void);
void BreakableWall_CheckBreak_Ceiling(void);

void BreakableWall_Break(EntityBreakableWall *self, uint8 direction);

// Misc
void BreakableWall_GiveScoreBonus(EntityPlayer *player);

#endif //! OBJ_BREAKABLEWALL_H
