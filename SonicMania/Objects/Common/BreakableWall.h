#ifndef OBJ_BREAKABLEWALL_H
#define OBJ_BREAKABLEWALL_H

#include "SonicMania.h"

typedef enum {
    BREAKWALL_TYPE_SIDES,
    BREAKWALL_TYPE_TOP,
    BREAKWALL_TYPE_TOPCHUNK,
    BREAKWALL_TYPE_TOPCHUNK_B,
    BREAKWALL_TYPE_BOTTOMCHUNK,
    BREAKWALL_TYPE_BOTTOMFULL
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
    uint16 layerID;
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
#if RETRO_INCLUDE_EDITOR
void BreakableWall_EditorDraw(void);
void BreakableWall_EditorLoad(void);
#endif
void BreakableWall_Serialize(void);

// Extra Entity Functions

// States
void BreakableWall_State_BottomChunks(void);
void BreakableWall_State_BottomFull(void);
void BreakableWall_State_FallingTile(void);
void BreakableWall_State_BreakableSides(void);
void BreakableWall_State_Tile(void);
void BreakableWall_State_Top(void);
void BreakableWall_State_TopChunks(void);

// Draw States
void BreakableWall_Draw_Outline_Sides(void);
void BreakableWall_Draw_Outline(void);
void BreakableWall_Draw_Tile(void);

// Breaking
void BreakableWall_HandleTopBreak_All(void);
void BreakableWall_HandleTopBreak_Chunks(void);
void BreakableWall_HandleSidesBreak(void);
void BreakableWall_HandleBottomBreak_Chunks(void);
void BreakableWall_HandleBottomBreak_All(void);
void BreakableWall_HandleBlockBreak_V(void);
void BreakableWall_HandleBlockBreak_H(EntityBreakableWall *wall, uint8 direction);

// Misc
void BreakableWall_GiveScoreBonus(EntityPlayer *player);

#endif //! OBJ_BREAKABLEWALL_H
