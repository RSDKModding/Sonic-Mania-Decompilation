#ifndef OBJ_FAN_H
#define OBJ_FAN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int activePlayers;
    byte field_8;
    int field_C;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox playerHitbox;
    ushort aniFrames;
    ushort sfxFan;
    bool32 playingFanSFX;
} ObjectFan;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(state3);
    StateMachine(state2);
    byte type;
    byte activation;
    byte deactivation;
    int size;
    ushort interval;
    ushort intervalOffset;
    ushort duration;
    ushort durationStore;
    int buttonTag;
    EntityButton *buttonPtr;
    Animator animator;
} EntityFan;

// Object Struct
extern ObjectFan *Fan;

// Standard Entity Events
void Fan_Update(void);
void Fan_LateUpdate(void);
void Fan_StaticUpdate(void);
void Fan_Draw(void);
void Fan_Create(void* data);
void Fan_StageLoad(void);
void Fan_EditorDraw(void);
void Fan_EditorLoad(void);
void Fan_Serialize(void);

// Extra Entity Functions
void Fan_SetupTagLinks(void);

void Fan_HandlePlayerInteractions_Top(void);
void Fan_HandlePlayerInteractions_Bottom(void);
void Fan_HandlePlayerInteractions_Left(void);
void Fan_HandlePlayerInteractions_Right(void);

void Fan_ProcessAnimationSpeed_Fast(void);
void Fan_ProcessAnimationSpeed_Slow(void);

void Fan_HandleDurationTimer(void);
void Fan_Unknown8(void);
void Fan_Unknown9(void);
void Fan_Unknown10(void);
void Fan_Unknown11(void);

#endif //!OBJ_FAN_H
