#ifndef OBJ_TECHNOSQUEEK_H
#define OBJ_TECHNOSQUEEK_H

#include "Game.h"

typedef enum {
    TECHNOSQUEEK_H,
    TECHNOSQUEEK_V,
} TechnosqueekTypes;

// Object Class
struct ObjectTechnosqueek {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    uint16 aniFrames;
};

// Entity Class
struct EntityTechnosqueek {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 speed;
    uint16 distance;
    uint8 distRemain;
    int32 acceleration;
    int32 targetVel;
    int32 tailOffset;
    bool32 shouldTurn;
    Vector2 startPos;
    uint8 startDir;
    Animator animator;
    Animator tailAnimator;
};

// Object Struct
extern ObjectTechnosqueek *Technosqueek;

// Standard Entity Events
void Technosqueek_Update(void);
void Technosqueek_LateUpdate(void);
void Technosqueek_StaticUpdate(void);
void Technosqueek_Draw(void);
void Technosqueek_Create(void *data);
void Technosqueek_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Technosqueek_EditorDraw(void);
void Technosqueek_EditorLoad(void);
#endif
void Technosqueek_Serialize(void);

// Extra Entity Functions
void Technosqueek_DebugSpawn(void);
void Technosqueek_DebugDraw(void);

void Technosqueek_HandlePlayerCollisions(void);
void Technosqueek_CheckOffScreen(void);

void Technosqueek_State_Init(void);
void Technosqueek_State_MoveHorizontal(void);
void Technosqueek_State_TurnHorizontal(void);
void Technosqueek_State_MoveVertical(void);
void Technosqueek_State_TurnVertical(void);
void Technosqueek_State_Fall(void);

#endif //! OBJ_TECHNOSQUEEK_H
