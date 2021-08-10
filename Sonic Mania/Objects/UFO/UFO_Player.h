#ifndef OBJ_UFO_PLAYER_H
#define OBJ_UFO_PLAYER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int maxSpeed;
    int field_8;
    ushort sfx_Jump;
    ushort sfx_Charge;
    ushort sfx_Release;
    ushort sfx_LoseRings;
    ushort sfx_Drop;
    ushort sfx_Bumper;
    ushort sfx_Spring;
    ushort sfx_Skid;
    ushort sfx_GrittyGround;
    ushort jogModel;
    ushort dashModel;
    ushort jumpModel;
    ushort ballModel;
    ushort tumbleModel;
    ushort sceneIndex;
} ObjectUFO_Player;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int machQuota1;
    int machQuota2;
    int machQuota3;
    int startingRings;
    int timer;
    int field_70;
    int field_74;
    int height;
    int gravityStrength;
    int angleZ;
    int field_84;
    int bumperTimer;
    int field_8C;
    int field_90;
    EntityUFO_Camera *camera;
    EntityUFO_Circuit *circuitPtr;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    Matrix matrix4;
    StateMachine(stateInput);
    int controllerID;
    bool32 up;
    bool32 down;
    bool32 left;
    bool32 right;
    bool32 jumpPress;
    bool32 jumpHold;
    Animator playerAnimator;
#if RETRO_USE_TOUCH_CONTROLS
    int touchJump;
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
