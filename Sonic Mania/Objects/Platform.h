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
    EntityAnimationData animData;
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
void Platform_State_Falling();
void Platform_State_OffScreenReset();

#endif //!OBJ_PLATFORM_H
