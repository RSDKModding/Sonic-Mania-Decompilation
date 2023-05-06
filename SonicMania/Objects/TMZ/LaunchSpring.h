#ifndef OBJ_LAUNCHSPRING_H
#define OBJ_LAUNCHSPRING_H

#include "Game.h"

typedef enum {
    LAUNCHSPRING_SPINNING,
    LAUNCHSPRING_CANNON,
    LAUNCHSPRING_ROTATE_45DEG,
    LAUNCHSPRING_ROTATE_90DEG,
    LAUNCHSPRING_ROTATE_135DEG,
    LAUNCHSPRING_ROTATE_180DEG,
} LaunchSpringTypes;

// Object Class
struct ObjectLaunchSpring {
    RSDK_OBJECT
    Hitbox hitbox;
    bool32 isTMZ;
    uint16 aniFrames;
    uint16 sfxGrab;
    uint16 sfxClack;
    uint16 sfxSpeedBooster;
};

// Entity Class
struct EntityLaunchSpring {
    RSDK_ENTITY
    StateMachine(state);
    Animator mainAnimator;
    Animator jointAnimator;
    Animator springAnimator;
    uint8 type;
    uint8 rotDir;
    uint8 rotSpeed;
    bool32 manual;
    bool32 strict;
    uint8 timer;
    uint8 springPivot;
    uint8 autoFireTimer;
    int16 minAngle;
    int16 maxAngle;
    EntityPlayer *playerPtr;
};

// Object Struct
extern ObjectLaunchSpring *LaunchSpring;

// Standard Entity Events
void LaunchSpring_Update(void);
void LaunchSpring_LateUpdate(void);
void LaunchSpring_StaticUpdate(void);
void LaunchSpring_Draw(void);
void LaunchSpring_Create(void *data);
void LaunchSpring_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LaunchSpring_EditorDraw(void);
void LaunchSpring_EditorLoad(void);
#endif
void LaunchSpring_Serialize(void);

// Extra Entity Functions
bool32 LaunchSpring_CheckFireworkActive(EntityPlayer *player);
void LaunchSpring_CheckPlayerCollisions(void *nextState);

void LaunchSpring_LaunchPlayer(void);
void LaunchSpring_State_Spinning(void);
void LaunchSpring_State_Spinning_GrabbedPlayer(void);
void LaunchSpring_State_Spinning_ReadyToFire(void);
void LaunchSpring_State_Spinning_FiredPlayer(void);
void LaunchSpring_State_Spinning_ReadyUp(void);
void LaunchSpring_State_Cannon(void);
void LaunchSpring_State_Cannon_Fire_Automatic(void);
void LaunchSpring_State_Cannon_Fire_Manual(void);
void LaunchSpring_State_Cannon_FiredPlayer(void);
void LaunchSpring_State_Rotating(void);
void LaunchSpring_State_Rotate_Fire_Automatic(void);
void LaunchSpring_State_Rotate_Fire_Manual(void);
void LaunchSpring_State_Rotating_FiredPlayer(void);

#endif //! OBJ_LAUNCHSPRING_H
