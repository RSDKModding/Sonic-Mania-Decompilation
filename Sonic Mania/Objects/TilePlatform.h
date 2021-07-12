#ifndef OBJ_TILEPLATFORM_H
#define OBJ_TILEPLATFORM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
} ObjectTilePlatform;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int type;
    Vector2 amplitude;
    int speed;
    int hasTension;
    sbyte frameID;
    byte collision;
    Vector2 targetPos;
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
    Animator animator;
    int childCount;
    Vector2 size;
    ushort tiles[0x100];
} EntityTilePlatform;

// Object Struct
extern ObjectTilePlatform *TilePlatform;

// Standard Entity Events
void TilePlatform_Update(void);
void TilePlatform_LateUpdate(void);
void TilePlatform_StaticUpdate(void);
void TilePlatform_Draw(void);
void TilePlatform_Create(void* data);
void TilePlatform_StageLoad(void);
void TilePlatform_EditorDraw(void);
void TilePlatform_EditorLoad(void);
void TilePlatform_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TILEPLATFORM_H
