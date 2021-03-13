#ifndef OBJ_BREAKABLEWALL_H
#define OBJ_BREAKABLEWALL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    EntityAnimationData animData;
    ushort spriteIndex;
    int value3[16]; //= { -524288, -1572864, 524288, -1572864, -524288, -524288, 524288, -524288, -524288, 524288, 524288, 524288, -524288, 1572864, 524288, 1572864 };
    int value4[16]; //= { -393216, -393216, -262144, -327680, -1179648, -131072, -393216, -65536, -524288, 131072, -393216, 65536, -393216, 393216, -262144, 327680 };
    int value5[16]; //= { 262144, -327680, 393216, -393216, 393216, -65536, 524288, -131072, 393216, 65536, 524288, 131072, 262144, 327680, 393216, 393216 };
    int value6[8]; //= { -524288, -524288, 524288, -524288, -524288, 524288, 524288, 524288 };
    int value7[8]; //= { -131072, -131072, 131072, -131072, -65536, -65536, 65536, -65536 };
    ushort sfx_Break;
    ushort farPlaneLayer;
} ObjectBreakableWall;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    void (*stateDraw)();
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
void BreakableWall_Update();
void BreakableWall_LateUpdate();
void BreakableWall_StaticUpdate();
void BreakableWall_Draw();
void BreakableWall_Create(void* data);
void BreakableWall_StageLoad();
void BreakableWall_EditorDraw();
void BreakableWall_EditorLoad();
void BreakableWall_Serialize();

// Extra Entity Functions
//States
void BreakableWall_State_BottomChunks();
void BreakableWall_State_BottomFull();
void BreakableWall_State_FallingTile();
void BreakableWall_State_HWall();
void BreakableWall_State_HandleHWall();
void BreakableWall_State_Tile();
void BreakableWall_State_Top();
void BreakableWall_State_VerticalChunks();
//Draw States
void BreakableWall_StateDraw_BWall();
void BreakableWall_StateDraw_BWall2();
void BreakableWall_StateDraw_Tile();
//Break
void BreakableWall_Break1();
void BreakableWall_Break2AND3();
void BreakableWall_Break4();
void BreakableWall_Break5();
void BreakableWall_BreakV();
void BreakableWall_BreakUnknown(EntityBreakableWall *entity, byte flip);
// Misc
void BreakableWall_GiveScoreBonus(void *plr);

#endif //!OBJ_BREAKABLEWALL_H
