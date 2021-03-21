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
    void (*state)();
    void (*stateCollide)();
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
    AnimationData animData;
    int childCount;
} EntityPlatform;

// Object Struct
extern ObjectPlatform *Platform;

// Standard Entity Events
void Platform_Update();
void Platform_LateUpdate();
void Platform_StaticUpdate();
void Platform_Draw();
void Platform_Create(void* data);
void Platform_StageLoad();
void Platform_EditorDraw();
void Platform_EditorLoad();
void Platform_Serialize();

// Extra Entity Functions
//States
void Platform_State_Nothing();
void Platform_State_Normal();
void Platform_State_Moving();
void Platform_State_Swing();
void Platform_State_Wait();
void Platform_State_WaitBobbing();
void Platform_State_StartPush();
void Platform_State_StartFalling();
void Platform_State_PlayerActivated();
void Platform_State_PlayerMove();
void Platform_State_Pushable();
void Platform_State_OffScreenReset();
void Platform_State_MovingSpike();
void Platform_State_Falling();
void Platform_State_Collapsing();
void Platform_State_Circular();
void Platform_State_12();
void Platform_State_13();
void Platform_State_14();
void Platform_State_15();
void Platform_State_16();

//Collision States

//Unknown
void Platform_Unknown1();
void Platform_Unknown2();
void Platform_Unknown3();
void Platform_Unknown4();
void Platform_Unknown5();
void Platform_Unknown6();
void Platform_Unknown7();
void Platform_Unknown8();
void Platform_Unknown9();
void Platform_Unknown10();
void Platform_Unknown11();
void Platform_Unknown12();
void Platform_Unknown13();
void Platform_Unknown14();

#endif //!OBJ_PLATFORM_H
