#ifndef OBJ_MATRYOSHKABOM_H
#define OBJ_MATRYOSHKABOM_H

#include "Game.h"

typedef enum {
    MATRYOSHKA_SIZE_BIG,
    MATRYOSHKA_SIZE_MED,
    MATRYOSHKA_SIZE_SMALL,
    MATRYOSHKA_SIZE_SHRAPNEL,
} MatryoshkaSizes;

// Object Class
struct ObjectMatryoshkaBom {
    RSDK_OBJECT
    Hitbox hitboxHurt;
    Hitbox hitboxExplode;
    Hitbox hitboxShrapnel;
    uint16 aniFrames;
    uint16 sfxExplosion;
    uint16 sfxButton;
    uint16 sfxPon;
};

// Entity Class
struct EntityMatryoshkaBom {
    RSDK_ENTITY
    StateMachine(state);
    PlaneFilterTypes planeFilter;
    MatryoshkaSizes size;
    bool32 canExplode;
    bool32 destroyOffscreen;
    int32 offsetY;
    int32 offsetX;
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    int32 fusePos;
    Animator bodyAnimator;
    Animator fuseAnimator;
    Animator legsAnimator;
};

// Object Struct
extern ObjectMatryoshkaBom *MatryoshkaBom;

// Standard Entity Events
void MatryoshkaBom_Update(void);
void MatryoshkaBom_LateUpdate(void);
void MatryoshkaBom_StaticUpdate(void);
void MatryoshkaBom_Draw(void);
void MatryoshkaBom_Create(void *data);
void MatryoshkaBom_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MatryoshkaBom_EditorDraw(void);
void MatryoshkaBom_EditorLoad(void);
#endif
void MatryoshkaBom_Serialize(void);

// Extra Entity Functions
void MatryoshkaBom_DebugSpawn(void);
void MatryoshkaBom_DebugDraw(void);

void MatryoshkaBom_CheckPlayerCollisions(void);
void MatryoshkaBom_CheckOffScreen(void);

void MatryoshkaBom_State_Init(void);
void MatryoshkaBom_State_Walk(void);
void MatryoshkaBom_State_Stopped(void);
void MatryoshkaBom_State_Turn(void);
void MatryoshkaBom_State_FuseLit(void);
void MatryoshkaBom_State_ReleaseSmallerBuddy(void);
void MatryoshkaBom_State_Hatched(void);
void MatryoshkaBom_State_Shrapnel(void);

#endif //! OBJ_MATRYOSHKABOM_H
