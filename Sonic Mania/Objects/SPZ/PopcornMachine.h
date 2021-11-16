#ifndef OBJ_POPCORNMACHINE_H
#define OBJ_POPCORNMACHINE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    int32 field_16;
    int32 field_1A;
    uint16 sfxFanStart;
    uint16 sfxShoot;
    uint16 sfxPopcornLaunch;
} ObjectPopcornMachine;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 height;
    int32 field_60;
    Entity *parent;
    EntityPlayer *player;
    int32 bounds[PLAYER_MAX];
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    int32 field_88;
    Animator animator;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
} EntityPopcornMachine;

// Object Struct
extern ObjectPopcornMachine *PopcornMachine;

// Standard Entity Events
void PopcornMachine_Update(void);
void PopcornMachine_LateUpdate(void);
void PopcornMachine_StaticUpdate(void);
void PopcornMachine_Draw(void);
void PopcornMachine_Create(void* data);
void PopcornMachine_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PopcornMachine_EditorDraw(void);
void PopcornMachine_EditorLoad(void);
#endif
void PopcornMachine_Serialize(void);

// Extra Entity Functions
void PopcornMachine_LinkPlayer(EntityPlayer *player);
void PopcornMachine_CheckPlayerCollisions(void);

void PopcornMachine_State_Unknown1(void);
void PopcornMachine_State_Unknown2(void);
void PopcornMachine_State_Unknown4(void);
void PopcornMachine_State_Unknown3(void);

void PopcornMachine_State2_Unknown1(void);
void PopcornMachine_State2_Unknown2(void);
void PopcornMachine_State2_Unknown3(void);
void PopcornMachine_State2_Unknown4(void);

#endif //!OBJ_POPCORNMACHINE_H
