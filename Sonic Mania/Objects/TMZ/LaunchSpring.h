#ifndef OBJ_LAUNCHSPRING_H
#define OBJ_LAUNCHSPRING_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    bool32 flag;
    uint16 aniFrames;
    uint16 sfxGrab;
    uint16 sfxClack;
    uint16 sfxSpeedBooster;
} ObjectLaunchSpring;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator1;
    Animator animator2;
    Animator animator3;
    uint8 type;
    uint8 rotDir;
    uint8 rotSpeed;
    bool32 manual;
    bool32 strict;
    uint8 timer;
    uint8 field_B1;
    uint8 timer2;
    int16 field_B4;
    int16 field_B6;
    Entity *playerPtr;
} EntityLaunchSpring;

// Object Struct
extern ObjectLaunchSpring *LaunchSpring;

// Standard Entity Events
void LaunchSpring_Update(void);
void LaunchSpring_LateUpdate(void);
void LaunchSpring_StaticUpdate(void);
void LaunchSpring_Draw(void);
void LaunchSpring_Create(void* data);
void LaunchSpring_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LaunchSpring_EditorDraw(void);
void LaunchSpring_EditorLoad(void);
#endif
void LaunchSpring_Serialize(void);

// Extra Entity Functions
bool32 LaunchSpring_Unknown1(void *p);
void LaunchSpring_CheckPlayerCollisions(void *nextState);

void LaunchSpring_Unknown3(void);
void LaunchSpring_Unknown4(void);
void LaunchSpring_Unknown5(void);
void LaunchSpring_Unknown6(void);
void LaunchSpring_Unknown7(void);
void LaunchSpring_Unknown8(void);
void LaunchSpring_Unknown9(void);
void LaunchSpring_Unknown10(void);
void LaunchSpring_Unknown11(void);
void LaunchSpring_Unknown12(void);
void LaunchSpring_Unknown13(void);
void LaunchSpring_Unknown14(void);
void LaunchSpring_Unknown15(void);
void LaunchSpring_Unknown16(void);

#endif //!OBJ_LAUNCHSPRING_H
