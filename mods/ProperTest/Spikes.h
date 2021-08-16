#pragma once
#include "ProperTest.h"

typedef struct {
    RSDK_OBJECT
    StateMachine(stateDraw);
    Animator vData;
    Animator hData;
    ushort spriteIndex;
    int dword3C;
    int dword40;
    ushort sfx_Move;
    ushort sfx_Spike;
} ObjectSpikes;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int type;
    bool32 moving;
    byte count;
    byte stagger;
    short timer;
    int planeFilter;
    int dword6C;
    int dword70;
    Vector2 offset;
    short timer2;
    byte field_7E;
    Hitbox hitbox;
    Animator animator;
} EntitySpikes;

// Object Struct
extern ObjectSpikes *Spikes;

void Spikes_Update(void);
void Spikes_StageLoad(void);
