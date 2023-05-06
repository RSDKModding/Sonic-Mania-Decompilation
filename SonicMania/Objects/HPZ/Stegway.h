#ifndef OBJ_STEGWAY_H
#define OBJ_STEGWAY_H

#include "Game.h"

// Object Class
struct ObjectStegway {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    uint16 aniFrames;
    uint16 sfxRev;
    uint16 sfxRelease;
};

// Entity Class
struct EntityStegway {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    int32 unused1;
    Animator mainAnimator;
    Animator wheelAnimator;
    Animator jetAnimator;
    bool32 showJet;
    int32 unused2;
    bool32 noFloor;
};

// Object Entity
extern ObjectStegway *Stegway;

// Standard Entity Events
void Stegway_Update(void);
void Stegway_LateUpdate(void);
void Stegway_StaticUpdate(void);
void Stegway_Draw(void);
void Stegway_Create(void *data);
void Stegway_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Stegway_EditorDraw(void);
void Stegway_EditorLoad(void);
#endif
void Stegway_Serialize(void);

// Extra Entity Functions
void Stegway_DebugSpawn(void);
void Stegway_DebugDraw(void);

void Stegway_CheckOffScreen(void);
void Stegway_HandlePlayerInteractions(void);
void Stegway_SetupAnims(char type, bool32 force);

void Stegway_State_Init(void);
void Stegway_State_Moving(void);
void Stegway_State_Turn(void);
void Stegway_State_RevUp(void);
void Stegway_State_RevRelease(void);
void Stegway_State_Dash(void);

#endif //! OBJ_STEGWAY_H
