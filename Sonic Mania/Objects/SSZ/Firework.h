#ifndef OBJ_FIREWORK_H
#define OBJ_FIREWORK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxRocketJet;
    uint16 sfxFlame;
    uint16 sfxExplosion2;
    uint16 sfxExplosion;
    bool32 playingFlameSfx;
    Hitbox hitbox1;
    Hitbox hitbox2;
} ObjectFirework;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 distance;
    int32 innerRadius;
    int32 outerRadius;
    StateMachine(state);
    uint8 activePlayers;
    int32 playerTimers[PLAYER_MAX];
    int32 field_7C;
    bool32 activated;
    int32 timer;
    int32 field_88;
    int32 field_8C;
    Vector2 startPos;
    Vector2 field_98;
    bool32 field_A0;
    Animator animator1;
    Animator animator2;
} EntityFirework;

// Object Struct
extern ObjectFirework *Firework;

// Standard Entity Events
void Firework_Update(void);
void Firework_LateUpdate(void);
void Firework_StaticUpdate(void);
void Firework_Draw(void);
void Firework_Create(void* data);
void Firework_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Firework_EditorDraw(void);
void Firework_EditorLoad(void);
#endif
void Firework_Serialize(void);

// Extra Entity Functions
void Firework_CheckPlayerCollisions(void);
void Firework_HandlePlayerTimers(void);
void Firework_RemovePlayers(bool32 hurt);
void Firework_HandlePlayerRemoval(void *p, bool32 hurt);
void Firework_HandlePlayerJump(void);
void Firework_HandlePlayerControl(void);
void Firework_HandleMoveDir(void);
void Firework_HandleDebrisSpawn(void);
void Firework_HandleTileCollisions(void);
void Firework_HandleRideEnd(bool32 hurt);
void Firework_CheckOnScreen(void);
void Firework_HandleAnimations(int a1, int16 angle, Animator *animator1, Animator *animator2, uint8 *direction);

void Firework_State_Setup(void);
void Firework_State_Unknown1(void);
void Firework_State_Unknown2(void);
void Firework_State_Unknown3(void);
void Firework_State_Unknown4(void);
void Firework_State_ResetOnScreen(void);
void Firework_State_Unknown5(void);
void Firework_State_Unknown6(void);

#endif //!OBJ_FIREWORK_H
