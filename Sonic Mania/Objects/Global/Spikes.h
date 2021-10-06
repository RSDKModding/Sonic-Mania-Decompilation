#ifndef OBJ_SPIKES_H
#define OBJ_SPIKES_H

#include "SonicMania.h"

// Object Class
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
    int stateMove;
    int moveOffset;
    Vector2 offset;
    short timer2;
    byte shatterTimer;
    Hitbox hitbox;
    Animator animator;
} EntitySpikes;

// Object Struct
extern ObjectSpikes *Spikes;

// Standard Entity Events
void Spikes_Update(void);
void Spikes_LateUpdate(void);
void Spikes_StaticUpdate(void);
void Spikes_Draw(void);
void Spikes_Create(void* data);
void Spikes_StageLoad(void);
void Spikes_EditorDraw(void);
void Spikes_EditorLoad(void);
void Spikes_Serialize(void);

// Extra Entity Functions
void Spikes_StateDraw_Global(void);
void Spikes_StateDraw_Stage(void);

void Spikes_Unknown1(int a1, int a2);
void Spikes_CheckHit(EntityPlayer *player, int playerVelX, int playerVelY);

#endif //!OBJ_SPIKES_H
