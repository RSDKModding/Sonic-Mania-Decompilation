#ifndef OBJ_SHURIKEN_H
#define OBJ_SHURIKEN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    int timerGroups[255];
    ushort aniFrames;
    ushort sfxArrowLaunch;
    ushort sfxArrowHit;
} ObjectShuriken;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    ushort timer;
    ushort dropTimer;
    byte activePlayers;
    Animator animator;
    ushort delay;
    bool32 timerMode;
    ushort interval;
    ushort intervalOffset;
    byte timerGroup;
} EntityShuriken;

// Object Struct
extern ObjectShuriken *Shuriken;

// Standard Entity Events
void Shuriken_Update(void);
void Shuriken_LateUpdate(void);
void Shuriken_StaticUpdate(void);
void Shuriken_Draw(void);
void Shuriken_Create(void* data);
void Shuriken_StageLoad(void);
void Shuriken_EditorDraw(void);
void Shuriken_EditorLoad(void);
void Shuriken_Serialize(void);

// Extra Entity Functions
void Shuriken_CheckPlayerCollisions(void);
void Shuriken_HandleSolidCollisions(void);
void Shuriken_State_Setup(void);
void Shuriken_Unknown4(void);
void Shuriken_Unknown5(void);
void Shuriken_Unknown6(void);
void Shuriken_Unknown7(void);
void Shuriken_Unknown8(void);
void Shuriken_Unknown9(void);
void Shuriken_Unknown10(void);
void Shuriken_Unknown11(void);
void Shuriken_Unknown12(void);
void Shuriken_Unknown13(void);
void Shuriken_Unknown14(void);

#endif //!OBJ_SHURIKEN_H
