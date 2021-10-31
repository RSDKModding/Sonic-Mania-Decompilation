#ifndef OBJ_JELLYGNITE_H
#define OBJ_JELLYGNITE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxGrab;
    uint16 sfxElectrify;
} ObjectJellygnite;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    uint8 timer;
    uint8 field_66;
    int32 field_68;
    int32 field_6C;
    Entity *grabbedPlayer;
    uint8 shakeTimer;
    uint8 shakeCount;
    uint8 lastShakeFlags;
    uint8 id;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityJellygnite;

// Object Struct
extern ObjectJellygnite *Jellygnite;

// Standard Entity Events
void Jellygnite_Update(void);
void Jellygnite_LateUpdate(void);
void Jellygnite_StaticUpdate(void);
void Jellygnite_Draw(void);
void Jellygnite_Create(void* data);
void Jellygnite_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Jellygnite_EditorDraw(void);
void Jellygnite_EditorLoad(void);
#endif
void Jellygnite_Serialize(void);

// Extra Entity Functions
void Jellygnite_DebugSpawn(void);
void Jellygnite_DebugDraw(void);

void Jellygnite_SetupAnimations(uint8 id);

void Jellygnite_CheckPlayerCollisions(void);
void Jellygnite_HandlePlayerStruggle(void);
bool32 Jellygnite_CheckInWater(void *p);

void Jellygnite_DrawPart1(void);
void Jellygnite_DrawPart2(void);

void Jellygnite_State_Setup(void);
void Jellygnite_State_Unknown1(void);
void Jellygnite_State_Unknown2(void);
void Jellygnite_State_Unknown3(void);

#endif //!OBJ_JELLYGNITE_H
