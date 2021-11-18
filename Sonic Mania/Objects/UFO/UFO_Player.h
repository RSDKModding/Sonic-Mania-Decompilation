#ifndef OBJ_UFO_PLAYER_H
#define OBJ_UFO_PLAYER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 maxSpeed;
    int32 field_8;
    uint16 sfxJump;
    uint16 sfxCharge;
    uint16 sfxRelease;
    uint16 sfxLoseRings;
    uint16 sfxDrop;
    uint16 sfxBumper;
    uint16 sfxSpring;
    uint16 sfxSkid;
    uint16 sfxGrittyGround;
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
    Animator animator;
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
#if RETRO_INCLUDE_EDITOR
void UFO_Player_EditorDraw(void);
void UFO_Player_EditorLoad(void);
#endif
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
