#ifndef OBJ_UFO_PLAYER_H
#define OBJ_UFO_PLAYER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 maxSpeed;
    int32 field_8;
    uint16 sfx_Jump;
    uint16 sfx_Charge;
    uint16 sfx_Release;
    uint16 sfx_LoseRings;
    uint16 sfx_Drop;
    uint16 sfx_Bumper;
    uint16 sfx_Spring;
    uint16 sfx_Skid;
    uint16 sfx_GrittyGround;
    uint16 jogModel;
    uint16 dashModel;
    uint16 jumpModel;
    uint16 ballModel;
    uint16 tumbleModel;
    uint16 sceneIndex;
} ObjectUFO_Player;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 machQuota1;
    int32 machQuota2;
    int32 machQuota3;
    int32 startingRings;
    int32 timer;
    int32 field_70;
    int32 field_74;
    int32 height;
    int32 gravityStrength;
    int32 angleZ;
    int32 field_84;
    int32 bumperTimer;
    int32 field_8C;
    int32 field_90;
    EntityUFO_Camera *camera;
    EntityUFO_Circuit *circuitPtr;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    Matrix matrix4;
    StateMachine(stateInput);
    int32 controllerID;
    bool32 up;
    bool32 down;
    bool32 left;
    bool32 right;
    bool32 jumpPress;
    bool32 jumpHold;
    Animator playerAnimator;
#if RETRO_USE_TOUCH_CONTROLS
    int32 touchJump;
#endif
} EntityUFO_Player;

// Object Struct
extern ObjectUFO_Player *UFO_Player;

// Standard Entity Events
void UFO_Player_Update(void);
void UFO_Player_LateUpdate(void);
void UFO_Player_StaticUpdate(void);
void UFO_Player_Draw(void);
void UFO_Player_Create(void* data);
void UFO_Player_StageLoad(void);
void UFO_Player_EditorDraw(void);
void UFO_Player_EditorLoad(void);
void UFO_Player_Serialize(void);

// Extra Entity Functions
void UFO_Player_ProcessPlayerControl(void);
void UFO_Player_ChangeMachState(void);
void UFO_Player_HandleBumperTiles(void);
void UFO_Player_HandleSpeedUp(void);
void UFO_Player_StateRun(void);
void UFO_Player_StateJump(void);
void UFO_Player_HandleTilt(void);
void UFO_Player_Unknown8(void);
void UFO_Player_Unknown9(void);
void UFO_Player_Unknown10(void);
void UFO_Player_Unknown11(void);

#endif //!OBJ_UFO_PLAYER_H
