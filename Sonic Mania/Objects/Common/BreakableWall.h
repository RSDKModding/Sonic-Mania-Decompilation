#ifndef OBJ_BREAKABLEWALL_H
#define OBJ_BREAKABLEWALL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Animator animator;
    ushort spriteIndex;
    TABLE(int value3[16], { -0x80000, -0x180000, 0x80000, -0x180000, -0x80000, -0x80000, 0x80000, -0x80000, -0x80000, 0x80000, 0x80000, 0x80000,
                            -0x80000, 0x180000, 0x80000, 0x180000 });
    TABLE(int value4[16], { -0x60000, -0x60000, -0x40000, -0x50000, -0x120000, -0x20000, -0x60000, -0x10000, -0x80000, 0x20000, -0x60000, 0x10000,
                            -0x60000, 0x60000, -0x40000, 0x50000 });
    TABLE(int value5[16], { 0x40000, -0x50000, 0x60000, -0x60000, 0x60000, -0x10000, 0x80000, -0x20000, 0x60000, 0x10000, 0x80000, 0x20000, 0x40000,
                            0x50000, 0x60000, 0x60000 });
    TABLE(int value6[8], { -0x80000, -0x80000, 0x80000, -0x80000, -0x80000, 0x80000, 0x80000, 0x80000 });
    TABLE(int value7[8], { -0x20000, -0x20000, 0x20000, -0x20000, -0x10000, -0x10000, 0x10000, -0x10000 }); 
    ushort sfx_Break;
    ushort farPlaneLayer;
} ObjectBreakableWall;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    byte type;
    bool32 onlyKnux;
    bool32 onlyMighty;
    bool32 priority;
    Vector2 size;
    ushort tileInfo;
    ushort layerID;
    int timer;
    Vector2 tilePos;
    int dword88;
    int gravityStrength;
    Hitbox hitbox;
} EntityBreakableWall;

// Object Struct
extern ObjectBreakableWall *BreakableWall;

// Standard Entity Events
void BreakableWall_Update(void);
void BreakableWall_LateUpdate(void);
void BreakableWall_StaticUpdate(void);
void BreakableWall_Draw(void);
void BreakableWall_Create(void* data);
void BreakableWall_StageLoad(void);
void BreakableWall_EditorDraw(void);
void BreakableWall_EditorLoad(void);
void BreakableWall_Serialize(void);

// Extra Entity Functions

//States
void BreakableWall_State_BottomChunks(void);
void BreakableWall_State_BottomFull(void);
void BreakableWall_State_FallingTile(void);
void BreakableWall_State_BreakableSides(void);
void BreakableWall_State_Tile(void);
void BreakableWall_State_Top(void);
void BreakableWall_State_TopChunks(void);

//Draw States
void BreakableWall_StateDraw_Outline(void);
void BreakableWall_StateDraw_Outline2(void);
void BreakableWall_StateDraw_Tile(void);

//Breaking
void BreakableWall_HandleTopBreak_All(void);
void BreakableWall_HandleTopBreak_Chunks(void);
void BreakableWall_HandleSidesBreak(void);
void BreakableWall_HandleBottomBreak_Chunks(void);
void BreakableWall_HandleBottomBreak_All(void);
void BreakableWall_HandleBlockBreak_V(void);
void BreakableWall_HandleBlockBreak_H(EntityBreakableWall *entity, byte flip);

// Misc
void BreakableWall_GiveScoreBonus(void *plr);

#endif //!OBJ_BREAKABLEWALL_H
