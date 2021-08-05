#ifndef OBJ_REDZ_H
#define OBJ_REDZ_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitbox2;
    Hitbox hitboxFlame;
    Hitbox hitboxRange;
    ushort aniFrames;
    ushort sfxFlame;
} ObjectRedz;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    byte startDir;
    ushort timer2;
    ushort timer;
    Animator animator;
} EntityRedz;

// Object Entity
extern ObjectRedz *Redz;

// Standard Entity Events
void Redz_Update(void);
void Redz_LateUpdate(void);
void Redz_StaticUpdate(void);
void Redz_Draw(void);
void Redz_Create(void* data);
void Redz_StageLoad(void);
void Redz_EditorDraw(void);
void Redz_EditorLoad(void);
void Redz_Serialize(void);

// Extra Entity Functions
void Redz_DebugSpawn(void);
void Redz_DebugDraw(void);

void Redz_HandlePlayerInteractions(void);

void Redz_State_Setup(void);
void Redz_State_Unknown(void);
void Redz_State_Unknown2(void);
void Redz_State_Unknown3(void);
void Redz_State_Unknown4(void);
void Redz_State_FlameSetup(void);
void Redz_State_Flame(void);

#endif //!OBJ_REDZ_H
