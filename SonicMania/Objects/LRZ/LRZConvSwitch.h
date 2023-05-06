#ifndef OBJ_LRZCONVSWITCH_H
#define OBJ_LRZCONVSWITCH_H

#include "Game.h"

typedef enum { LRZCONVSWITCH_RIGHT, LRZCONVSWITCH_LEFT } LRZConvSwitchCalibrateModes;

// Object Class
struct ObjectLRZConvSwitch {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxClack;
};

// Entity Class
struct EntityLRZConvSwitch {
    RSDK_ENTITY
    StateMachine(state);
    uint8 calibration;
    uint8 conveyorDir;
    int32 dir;
    Vector2 playerPositions[PLAYER_COUNT];
    int32 unused1;
    Vector2 startPos;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    Animator animator;
};

// Object Struct
extern ObjectLRZConvSwitch *LRZConvSwitch;

// Standard Entity Events
void LRZConvSwitch_Update(void);
void LRZConvSwitch_LateUpdate(void);
void LRZConvSwitch_StaticUpdate(void);
void LRZConvSwitch_Draw(void);
void LRZConvSwitch_Create(void *data);
void LRZConvSwitch_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LRZConvSwitch_EditorDraw(void);
void LRZConvSwitch_EditorLoad(void);
#endif
void LRZConvSwitch_Serialize(void);

// Extra Entity Functions
void LRZConvSwitch_Calibrate(void);

#endif //! OBJ_LRZCONVSWITCH_H
