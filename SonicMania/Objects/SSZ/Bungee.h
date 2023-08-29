#ifndef OBJ_BUNGEE_H
#define OBJ_BUNGEE_H

#include "Game.h"

// Object Class
struct ObjectBungee {
    RSDK_OBJECT
    uint16 aniFrames;
    Animator animator;
};

// Entity Class
struct EntityBungee {
    RSDK_ENTITY
    Vector2 startPos;
    Vector2 bungeePos;
    int32 playerY[PLAYER_COUNT];
    bool32 hasAttachedPlayer;
    EntityPlayer *attachedPlayer;
    int32 stretchForce;
    int32 deathBoundary[PLAYER_COUNT];
    uint8 timer;
};

// Object Struct
extern ObjectBungee *Bungee;

// Standard Entity Events
void Bungee_Update(void);
void Bungee_LateUpdate(void);
void Bungee_StaticUpdate(void);
void Bungee_Draw(void);
void Bungee_Create(void *data);
void Bungee_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Bungee_EditorDraw(void);
void Bungee_EditorLoad(void);
#endif
void Bungee_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_BUNGEE_H
