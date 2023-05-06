#ifndef OBJ_KABASIRA_H
#define OBJ_KABASIRA_H

#include "Game.h"

#define KABASIRA_BODY_COUNT (4)

// Object Class
struct ObjectKabasira {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Vector2 onScreenRange;
    uint16 aniFrames;
    uint16 sfxPon;
    uint16 sfxExplosion2;
};

// Entity Class
struct EntityKabasira {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 unused2;
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    Animator bodyAnimator;
    Animator wingsAnimator;
    int32 unused3;
};

// Object Struct
extern ObjectKabasira *Kabasira;

// Standard Entity Events
void Kabasira_Update(void);
void Kabasira_LateUpdate(void);
void Kabasira_StaticUpdate(void);
void Kabasira_Draw(void);
void Kabasira_Create(void *data);
void Kabasira_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Kabasira_EditorDraw(void);
void Kabasira_EditorLoad(void);
#endif
void Kabasira_Serialize(void);

// Extra Entity Functions
void Kabasira_DebugSpawn(void);
void Kabasira_DebugDraw(void);

bool32 Kabasira_HandleAnimations(uint8 angle);
void Kabasira_DrawSegment(int32 angle, int32 alpha);
void Kabasira_CheckPlayerCollisions(void);
void Kabasira_CheckOffScreen(void);

void Kabasira_State_Init(void);
void Kabasira_State_Moving(void);
void Kabasira_State_LaunchedAttack(void);

#endif //! OBJ_KABASIRA_H
