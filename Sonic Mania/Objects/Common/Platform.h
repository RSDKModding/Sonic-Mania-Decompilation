#ifndef OBJ_PLATFORM_H
#define OBJ_PLATFORM_H

#include "SonicMania.h"

typedef enum {
    PLATFORM_0,
    PLATFORM_1,
    PLATFORM_2,
    PLATFORM_3,
    PLATFORM_4,
    PLATFORM_5,
    PLATFORM_6,
    PLATFORM_7,
    PLATFORM_8,
    PLATFORM_9,
    PLATFORM_10,
    PLATFORM_11,
    PLATFORM_12,
    PLATFORM_13,
    PLATFORM_14,
    PLATFORM_15,
    PLATFORM_16,
} PlatformTypes;

typedef enum {
    PLATFORM_C_0,
    PLATFORM_C_1,
    PLATFORM_C_2,
    PLATFORM_C_3,
    PLATFORM_C_4,
    PLATFORM_C_5,
    PLATFORM_C_6,
    PLATFORM_C_7,
    PLATFORM_C_8,
    PLATFORM_C_9,
    PLATFORM_C_10,
    PLATFORM_C_11,
    PLATFORM_C_12,
    PLATFORM_C_13,
    PLATFORM_C_14,
    PLATFORM_C_15,
} PlatformCollisionTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Vector2 stoodPos[4];
    uint16 sfxClacker;
    uint16 sfxClang;
    uint16 sfxPush;
    uint16 sfxClack;
    bool32 playingPushSFX;
    bool32 useClack;
} ObjectPlatform;

// Entity Class
typedef struct {
    MANIA_PLATFORM_BASE
} EntityPlatform;

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
//States
void Platform_State_Nothing(void);
void Platform_State_Normal(void);
void Platform_State_Moving(void);
void Platform_State_Swing(void);
void Platform_State_Wait(void);
void Platform_State_WaitBobbing(void);
void Platform_State_StartPush(void);
void Platform_State_StartFalling(void);
void Platform_State_PlayerActivated(void);
void Platform_State_PlayerMove(void);
void Platform_State_Pushable(void);
void Platform_State_OffScreenReset(void);
void Platform_State_MovingSpike(void);
void Platform_State_Falling(void);
void Platform_State_Collapsing(void);
void Platform_State_Circular(void);
void Platform_State_12(void);
void Platform_State_13(void);
void Platform_State_14(void);
void Platform_State_15(void);
void Platform_State_16(void);

//Collision States
void Platform_CollisionState_AllSolid(void);
void Platform_CollisionState_AllHazard(void);
void Platform_CollisionState_BottomHazard(void);
void Platform_CollisionState_LRHazard(void);
void Platform_CollisionState_Tiles(void);
void Platform_CollisionState_Sticky(void);
void Platform_CollisionState_TopHazard(void);
void Platform_CollisionState_TopSolid(void);
void Platform_CollisionState_TurnTable(void);
void Platform_CollisionState_Twister(void);
void Platform_CollisionState_None(void);
void Platform_CollisionState_AllSolid_NoCrush(void);

//Unknown
void Platform_Unknown1(void);
void Platform_Unknown2(void);
void Platform_Unknown3(void);
void Platform_Unknown4(void);
void Platform_Unknown5(void);
void Platform_Unknown6(void);
void Platform_Unknown7(void);
void Platform_Unknown8(void);
void Platform_Unknown9(void);
void Platform_Unknown10(void);
void Platform_Unknown11(void);
void Platform_Unknown12(void);
void Platform_Unknown13(void);
void Platform_Unknown14(void);

#endif //!OBJ_PLATFORM_H
