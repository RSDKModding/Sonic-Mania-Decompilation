#ifndef OBJ_PLATFORM_H
#define OBJ_PLATFORM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    Vector2 stoodPos[4];
    ushort sfx_Clacker;
    ushort sfx_Clang;
    ushort sfx_Push;
    ushort sfx_Clack;
    bool32 playingPushSFX;
    bool32 useClack;
} ObjectPlatform;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    void (*stateCollide)(void);
    int type;
    Vector2 amplitude;
    int speed;
    int hasTension;
    sbyte frameID;
    byte collision;
    Vector2 tileOrigin;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int stood;
    int collapseDelay;
    int stoodAngle;
    byte stoodPlayers;
    byte pushPlayersL;
    byte pushPlayersR;
    Hitbox hitbox;
    Animator animData;
    int childCount;
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
void Platform_EditorDraw(void);
void Platform_EditorLoad(void);
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
void Platform_CollisionState_None(void);
void Platform_CollisionState_Sticky(void);
void Platform_CollisionState_TopHazard(void);
void Platform_CollisionState_TopSolid(void);
void Platform_CollisionState_TurnTable(void);
void Platform_CollisionState_Twister(void);
void Platform_CollisionState_Null(void);
void Platform_CollisionState_15(void);

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
