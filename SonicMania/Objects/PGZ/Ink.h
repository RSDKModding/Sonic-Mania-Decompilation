#ifndef OBJ_INK_H
#define OBJ_INK_H

#include "Game.h"

typedef enum {
    INK_C,
    INK_M,
    INK_Y,
} InkTypes;

// Object Class
struct ObjectInk {
    RSDK_OBJECT
    uint8 playerColors[PLAYER_COUNT];
    uint16 aniFrames;
    Hitbox hitboxBottleL;
    Hitbox hitboxBottleR;
    Hitbox hitboxBottleBottom;
};

// Entity Class
struct EntityInk {
    RSDK_ENTITY
    uint8 type;
    uint8 inkedPlayers;
    Animator bottleAnimator;
    Animator contentAnimator;
    Animator splashAnimator;
};

// Object Struct
extern ObjectInk *Ink;

// Standard Entity Events
void Ink_Update(void);
void Ink_LateUpdate(void);
void Ink_StaticUpdate(void);
void Ink_Draw(void);
void Ink_Create(void *data);
void Ink_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Ink_EditorDraw(void);
void Ink_EditorLoad(void);
#endif
void Ink_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_INK_H
