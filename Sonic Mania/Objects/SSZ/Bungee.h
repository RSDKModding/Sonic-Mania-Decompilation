#ifndef OBJ_BUNGEE_H
#define OBJ_BUNGEE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Animator animator;
} ObjectBungee;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 startPos;
    Vector2 bungeePos;
    int32 playerY[PLAYER_MAX];
    bool32 hasAttatchedPlayer;
    Entity *attatchedPlayer;
    int32 field_80;
    int32 playerDeathBoundary[PLAYER_MAX];
    uint8 timer;
} EntityBungee;

// Object Struct
extern ObjectBungee *Bungee;

// Standard Entity Events
void Bungee_Update(void);
void Bungee_LateUpdate(void);
void Bungee_StaticUpdate(void);
void Bungee_Draw(void);
void Bungee_Create(void* data);
void Bungee_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Bungee_EditorDraw(void);
void Bungee_EditorLoad(void);
#endif
void Bungee_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BUNGEE_H
