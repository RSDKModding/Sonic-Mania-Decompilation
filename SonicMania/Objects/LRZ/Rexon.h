#ifndef OBJ_REXON_H
#define OBJ_REXON_H

#include "Game.h"

#define REXON_SEGMENT_COUNT (5)

typedef enum {
    REXON_MAIN,
    REXON_DEBRIS,
    REXON_SHOT,
} RexonTypes;

// Object Class
struct ObjectRexon {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxShell;
    Hitbox hitboxRange;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
    uint16 sfxShot;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityRexon {
    RSDK_ENTITY
    StateMachine(state);
    bool32 noMove;
    int32 type;
    Vector2 startPos;
    uint8 startDir;
    Vector2 positions[REXON_SEGMENT_COUNT + 1]; // neck + head
    int32 segmentMagnitude[REXON_SEGMENT_COUNT];
    int32 segmentAmplitude[REXON_SEGMENT_COUNT];
    uint8 segmentDirections[REXON_SEGMENT_COUNT];
    uint8 segmentID;
    int32 timer;
    int32 destroyDelay;
    Animator bodyAnimator;
    Animator neckAnimator;
    Animator headAnimator;
};

// Object Struct
extern ObjectRexon *Rexon;

// Standard Entity Events
void Rexon_Update(void);
void Rexon_LateUpdate(void);
void Rexon_StaticUpdate(void);
void Rexon_Draw(void);
void Rexon_Create(void *data);
void Rexon_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Rexon_EditorDraw(void);
void Rexon_EditorLoad(void);
#endif
void Rexon_Serialize(void);

// Extra Entity Functions
void Rexon_DebugSpawn(void);
void Rexon_DebugDraw(void);

void Rexon_CheckOffScreen(void);
void Rexon_CheckPlayerCollisions(void);
void Rexon_Destroy(EntityRexon *rexon, bool32 crushed);

void Rexon_State_Init(void);
void Rexon_State_Hidden(void);
void Rexon_State_Rising(void);
void Rexon_State_Shooting(void);
void Rexon_State_Destroyed(void);
void Rexon_State_Explode(void);

void Rexon_State_Debris(void);
void Rexon_State_Projectile(void);

#endif //! OBJ_REXON_H
