#ifndef OBJ_BUMPALO_H
#define OBJ_BUMPALO_H

#include "SonicMania.h"

// Object Class
struct ObjectBumpalo {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxBumper;
    Hitbox hitboxClack;
    Hitbox hitboxCharge;
    Hitbox hitbox5;
    uint16 aniFrames;
    uint16 sfxBumper;
    uint16 sfxHuff;
    uint16 sfxClack;
    uint16 sfxImpact;
};

// Entity Class
struct EntityBumpalo {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    bool32 ignoreCliffs;
    bool32 flag;
    Animator animator1;
    Animator animator2;
    Animator animator3;
};

// Object Struct
extern ObjectBumpalo *Bumpalo;

// Standard Entity Events
void Bumpalo_Update(void);
void Bumpalo_LateUpdate(void);
void Bumpalo_StaticUpdate(void);
void Bumpalo_Draw(void);
void Bumpalo_Create(void* data);
void Bumpalo_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Bumpalo_EditorDraw(void);
void Bumpalo_EditorLoad(void);
#endif
void Bumpalo_Serialize(void);

// Extra Entity Functions
void Bumpalo_DebugSpawn(void);
void Bumpalo_DebugDraw(void);

void Bumpalo_CheckOffScreen(void);
void Bumpalo_BumpPlayer(void *p);
void Bumpalo_CheckPlayerCollisions(void);
void Bumpalo_HandlePlatformCollisions(void *p);
void Bumpalo_HandleObjectCollisions(void);

void Bumpalo_State_Setup(void);
void Bumpalo_State_Unknown1(void);
void Bumpalo_State_Unknown2(void);
void Bumpalo_State_Unknown3(void);
void Bumpalo_State_Unknown4(void);
void Bumpalo_State_Unknown5(void);
void Bumpalo_State_Unknown6(void);

#endif //!OBJ_BUMPALO_H
