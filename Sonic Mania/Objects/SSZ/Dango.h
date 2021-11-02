#ifndef OBJ_DANGO_H
#define OBJ_DANGO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxBumper;
} ObjectDango;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 timer2;
    bool32 flag;
    Vector2 startPos;
    uint8 startDir;
    Animator animator;
} EntityDango;

// Object Struct
extern ObjectDango *Dango;

// Standard Entity Events
void Dango_Update(void);
void Dango_LateUpdate(void);
void Dango_StaticUpdate(void);
void Dango_Draw(void);
void Dango_Create(void* data);
void Dango_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Dango_EditorDraw(void);
void Dango_EditorLoad(void);
#endif
void Dango_Serialize(void);

// Extra Entity Functions
void Dango_DebugSpawn(void);
void Dango_DebugDraw(void);

void Dango_CheckOnScreen(void);
void Dango_CheckPlayerCollisions(void);
bool32 Dango_HandleTileCollisions(StateMachine(nextState), uint8 anim);

void Dango_State_Setup(void);
void Dango_State_Unknown1(void);
void Dango_State_Unknown6(void);
void Dango_State_Unknown2(void);
void Dango_State_Unknown3(void);
void Dango_State_Unknown4(void);
void Dango_State_Unknown5(void);
void Dango_State_Unknown7(void);

#if RETRO_USE_PLUS
void Dango_StateTaunt_Unknown1(void);
void Dango_StateTaunt_Unknown2(void);
void Dango_StateTaunt_Unknown3(void);
void Dango_StateTaunt_Unknown4(void);
void Dango_StateTaunt_Unknown5(void);
void Dango_StateTaunt_Unknown6(void);
#endif

#endif //!OBJ_DANGO_H
