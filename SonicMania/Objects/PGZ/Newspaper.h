#ifndef OBJ_NEWSPAPER_H
#define OBJ_NEWSPAPER_H

#include "Game.h"

#if MANIA_USE_PLUS
typedef enum {
    NEWSPAPER_WHITE_FG,
    NEWSPAPER_BLUE_FG,
    NEWSPAPER_WHITE_BG,
    NEWSPAPER_BLUE_BG,
} NewspaperTypes;
#endif

// Object Class
struct ObjectNewspaper {
    RSDK_OBJECT
    uint16 aniFrames;
#if MANIA_USE_PLUS
    uint16 sfxPaperStack;
#endif
};

// Entity Class
struct EntityNewspaper {
    RSDK_ENTITY
#if MANIA_USE_PLUS
    StateMachine(state);
    Hitbox hitbox;
    uint8 type;
    int32 timer;
    Animator animator;
#else
    StateMachine(state);
    StateMachine(stateCollide);
    int32 type;
    Vector2 amplitude;
    int32 node;
    bool32 hasTension;
    int8 frameID;
    uint8 collision;
    Vector2 tileOrigin;
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
#endif
};

// Object Struct
extern ObjectNewspaper *Newspaper;

// Standard Entity Events
void Newspaper_Update(void);
void Newspaper_LateUpdate(void);
void Newspaper_StaticUpdate(void);
void Newspaper_Draw(void);
void Newspaper_Create(void *data);
void Newspaper_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Newspaper_EditorDraw(void);
void Newspaper_EditorLoad(void);
#endif
void Newspaper_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
void Newspaper_HandleInteractions(void);
#endif

#endif //! OBJ_NEWSPAPER_H
