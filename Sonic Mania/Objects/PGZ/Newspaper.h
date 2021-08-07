#ifndef OBJ_NEWSPAPER_H
#define OBJ_NEWSPAPER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
#if RETRO_USE_PLUS
    ushort sfxPaperStack;
#endif
} ObjectNewspaper;

// Entity Class
typedef struct {
    RSDK_ENTITY
#if RETRO_USE_PLUS
    StateMachine(state);
    Hitbox hitbox;
    byte type;
    int timer;
    Animator animator;
#else
    StateMachine(state);
    StateMachine(stateCollide);
    int type;
    Vector2 amplitude;
    int node;
    bool32 hasTension;
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
    Animator animator;
    int childCount;
#endif
} EntityNewspaper;

// Object Struct
extern ObjectNewspaper *Newspaper;

// Standard Entity Events
void Newspaper_Update(void);
void Newspaper_LateUpdate(void);
void Newspaper_StaticUpdate(void);
void Newspaper_Draw(void);
void Newspaper_Create(void* data);
void Newspaper_StageLoad(void);
void Newspaper_EditorDraw(void);
void Newspaper_EditorLoad(void);
void Newspaper_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
void Newspaper_HandleInteractions(void);
#endif

#endif //!OBJ_NEWSPAPER_H
