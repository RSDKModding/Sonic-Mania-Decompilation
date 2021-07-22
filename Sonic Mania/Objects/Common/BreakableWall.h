#ifndef OBJ_BREAKABLEWALL_H
#define OBJ_BREAKABLEWALL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Animator animator;
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
void BreakableWall_State_HWall(void);
void BreakableWall_State_HandleHWall(void);
void BreakableWall_State_Tile(void);
void BreakableWall_State_Top(void);
void BreakableWall_State_VerticalChunks(void);
//Draw States
void BreakableWall_StateDraw_BWall(void);
void BreakableWall_StateDraw_BWall2(void);
void BreakableWall_StateDraw_Tile(void);
//Break
void BreakableWall_Break1(void);
void BreakableWall_Break2AND3(void);
void BreakableWall_Break4(void);
void BreakableWall_Break5(void);
void BreakableWall_BreakV(void);
void BreakableWall_BreakUnknown(EntityBreakableWall *entity, byte flip);
// Misc
void BreakableWall_GiveScoreBonus(void *plr);

#endif //!OBJ_BREAKABLEWALL_H
