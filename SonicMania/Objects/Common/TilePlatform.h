#ifndef OBJ_TILEPLATFORM_H
#define OBJ_TILEPLATFORM_H

#include "Game.h"

// Object Class
struct ObjectTilePlatform {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
};

// Entity Class
struct EntityTilePlatform {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int32 type;
    Vector2 amplitude;
    int32 speed;
    bool32 hasTension;
    int8 frameID;
    uint8 collision;
    Vector2 targetPos;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int32 stood;
    int32 timer;
    int32 stoodAngle;
    uint8 stoodPlayers;
    uint8 pushPlayersL;
    uint8 pushPlayersR;
    Hitbox hitbox;
    Animator animator;
    int32 childCount;
    Vector2 size;
    uint16 tiles[0x100];
};

// Object Struct
extern ObjectTilePlatform *TilePlatform;

// Standard Entity Events
void TilePlatform_Update(void);
void TilePlatform_LateUpdate(void);
void TilePlatform_StaticUpdate(void);
void TilePlatform_Draw(void);
void TilePlatform_Create(void *data);
void TilePlatform_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TilePlatform_EditorDraw(void);
void TilePlatform_EditorLoad(void);
#endif
void TilePlatform_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_TILEPLATFORM_H
