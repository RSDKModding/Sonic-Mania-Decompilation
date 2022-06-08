#ifndef OBJ_PLATFORM_H
#define OBJ_PLATFORM_H

#include "Game.h"

typedef enum {
    PLATFORM_FIXED,
    PLATFORM_COLLAPSING,
    PLATFORM_MOVING,
    PLATFORM_CIRCULAR,
    PLATFORM_SWINGING,
    PLATFORM_CONTROLLED,
    PLATFORM_PUSHABLE,
    PLATFORM_TRACK,
    PLATFORM_WAIT,
    PLATFORM_WAIT_OSC,
    PLATFORM_ACTIVEABOVE,
    PLATFORM_CONT_ACTIVATER,
    PLATFORM_WAIT_ARC,
    PLATFORM_STICKY,
    PLATFORM_SWING_CLACK,
    PLATFORM_STATIC,
    PLATFORM_SINKER,
} PlatformTypes;

typedef enum {
    PLATFORM_C_SOLID_TOP,
    PLATFORM_C_SOLID_ALL,
    PLATFORM_C_USE_TILES,
    PLATFORM_C_HAZARD_ALL,
    PLATFORM_C_SOLID_NONE,
    PLATFORM_C_HAZARD_SIDES,
    PLATFORM_C_HAZARD_BOTTOM,
    PLATFORM_C_HAZARD_TOP,
    PLATFORM_C_TWISTER,
    PLATFORM_C_STICKY_ALL,
    PLATFORM_C_STICKY_TOP,
    PLATFORM_C_STICKY_LEFT,
    PLATFORM_C_STICKY_RIGHT,
    PLATFORM_C_STICKY_BOTTOM,
    PLATFORM_C_TURNTABLE,
    PLATFORM_C_SOLID_ALL_NOCRUSH,
} PlatformCollisionTypes;

// Object Class
struct ObjectPlatform {
    RSDK_OBJECT
    uint16 aniFrames;
    Vector2 stoodPos[PLAYER_COUNT];
    uint16 sfxClacker;
    uint16 sfxClang;
    uint16 sfxPush;
    uint16 sfxClack;
    bool32 playingPushSFX;
    bool32 useClack;
};

// Entity Class
struct EntityPlatform {
    MANIA_PLATFORM_BASE
};

// Object Struct
extern ObjectPlatform *Platform;

// Standard Entity Events
void Platform_Update(void);
void Platform_LateUpdate(void);
void Platform_StaticUpdate(void);
void Platform_Draw(void);
void Platform_Create(void* data);
void Platform_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Platform_EditorDraw(void);
void Platform_EditorLoad(void);
#endif
void Platform_Serialize(void);

// Extra Entity Functions

// Collision States
void Platform_Collision_TopSolid(void);
void Platform_Collision_AllSolid(void);
void Platform_Collision_AllSolid_NoCrush(void);
void Platform_Collision_LRHazard(void);
void Platform_Collision_BottomHazard(void);
void Platform_Collision_TopHazard(void);
void Platform_Collision_Twister(void);
void Platform_Collision_TurnTable(void);
void Platform_Collision_Sticky(void);
void Platform_Collision_Tiles(void);
void Platform_Collision_AllHazard(void);
void Platform_Collision_None(void);

//States
void Platform_State_Fixed(void);
void Platform_State_Collapse(void);
void Platform_State_Collapse_StartFall(void);
void Platform_State_Collapse_Falling(void);
void Platform_State_Collapse_CheckReset(void);
void Platform_State_Moving(void);
void Platform_State_Circular(void);
void Platform_State_Swing(void);
void Platform_State_Swing_Clack(void);
void Platform_State_StartPush(void);
void Platform_State_Pushable(void);
void Platform_State_Pushable_FallingL(void);
void Platform_State_Pushable_FallingR(void);
void Platform_State_Pushable_Falling(void);
void Platform_State_Controlled(void);
void Platform_State_AwaitControlCommand(void);
void Platform_State_Track(void);
void Platform_State_Wait(void);
void Platform_State_WaitOscillating(void);
void Platform_State_PlayerMove_Starting(void);
void Platform_State_PlayerMove_Moving(void);
void Platform_State_PlayerMove_Rotate(void);
void Platform_State_PlayerMove_HandleRestart(void);
void Platform_State_ActiveFromAbove(void);
void Platform_State_ActivateControlOnStood(void);
void Platform_State_Static(void);
void Platform_State_SwingMove_CheckStood(void);
void Platform_State_SwingMove_MoveToDest(void);
void Platform_State_SwingMove_WaitNotStood(void);
void Platform_State_SwingMove_MoveToStart(void);
void Platform_State_Sticky_CheckRide(void);
void Platform_State_Sticky_MoveToDest(void);
void Platform_State_Sticky_WaitNotStood(void);
void Platform_State_Sticky_MoveToStart(void);
void Platform_State_Sinker(void);

#endif //!OBJ_PLATFORM_H
