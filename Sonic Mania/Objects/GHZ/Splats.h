#ifndef OBJ_SPLATS_H
#define OBJ_SPLATS_H

#include "SonicMania.h"

typedef enum {
    //GHZ
    SPLATS_BOUNCEAROUND = 0,
    //PGZ
    SPLATS_SPAWNER = 0,
    SPLATS_INKSPLATS,
    SPLATS_SPLAT,
}SplatsTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    StateMachine(state);
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 aniFrames;
    uint16 sfxSplatsSpawn;
    uint16 sfxSplatsLand;
} ObjectSplats;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 bounceCount;
    uint8 activeCount;
    uint8 numActive;
    uint8 field_5F;
    uint16 minDelay;
    uint16 field_62;
    int32 delay;
    int32 field_68;
    Entity *parent;
    Vector2 startPos;
    int32 startDir;
    Animator animator1;
    Animator animator2;
} EntitySplats;

// Object Struct
extern ObjectSplats *Splats;

// Standard Entity Events
void Splats_Update(void);
void Splats_LateUpdate(void);
void Splats_StaticUpdate(void);
void Splats_Draw(void);
void Splats_Create(void* data);
void Splats_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Splats_EditorDraw(void);
void Splats_EditorLoad(void);
#endif
void Splats_Serialize(void);

// Extra Entity Functions
void Splats_DebugDraw(void);
void Splats_DebugSpawn(void);

void Splats_CheckPlayerCollisions(void);
void Splats_CheckOnScreen(void);

void Splats_State_Setup(void);
void Splats_State_BounceAround(void);
void Splats_State_SetupInkJar(void);
void Splats_State_InkJarSpawner(void);
void Splats_State_JumpOutOfJar(void);
void Splats_State_HandleBouncing(void);
void Splats_State_HandleLanding(void);
void Splats_State_NoMoreJumps(void);
void Splats_State_InkSplat(void);

#endif //!OBJ_SPLATS_H
