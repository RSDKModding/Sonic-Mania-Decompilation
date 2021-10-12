#ifndef OBJ_LAUNCHSPRING_H
#define OBJ_LAUNCHSPRING_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    bool32 flag;
    ushort aniFrames;
    ushort sfxGrab;
    ushort sfxClack;
    ushort sfxSpeedBooster;
} ObjectLaunchSpring;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator1;
    Animator animator2;
    Animator animator3;
    byte type;
    byte rotDir;
    byte rotSpeed;
    bool32 manual;
    bool32 strict;
    byte timer;
    byte field_B1;
    byte timer2;
    short field_B4;
    short field_B6;
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
void LaunchSpring_EditorDraw(void);
void LaunchSpring_EditorLoad(void);
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
