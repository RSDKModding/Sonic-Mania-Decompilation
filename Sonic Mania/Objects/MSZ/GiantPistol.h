#ifndef OBJ_GIANTPISTOL_H
#define OBJ_GIANTPISTOL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
#if RETRO_USE_PLUS
    bool32 flag;
#endif
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 sfxClack;
    uint16 sfxCannonFire;
    uint16 aniFrames;
} ObjectGiantPistol;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer2;
    int32 activePlayers;
    int32 field_64;
    int32 timer;
    Vector2 startPos;
    Vector2 field_74;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    Animator animator7;
    Animator animator8;
} EntityGiantPistol;

// Object Struct
extern ObjectGiantPistol *GiantPistol;

// Standard Entity Events
void GiantPistol_Update(void);
void GiantPistol_LateUpdate(void);
void GiantPistol_StaticUpdate(void);
void GiantPistol_Draw(void);
void GiantPistol_Create(void* data);
void GiantPistol_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void GiantPistol_EditorDraw(void);
void GiantPistol_EditorLoad(void);
#endif
void GiantPistol_Serialize(void);

// Extra Entity Functions
void GiantPistol_State_WaitForPlayer(void);
void GiantPistol_State_Unknown2(void);
void GiantPistol_State_Unknown3(void);
void GiantPistol_State_Aiming(void);
void GiantPistol_State_FiredPlayers(void);

void GiantPistol_Player_State_Unknown1(void);
void GiantPistol_Player_State_Unknown2(void);

#endif //!OBJ_GIANTPISTOL_H
