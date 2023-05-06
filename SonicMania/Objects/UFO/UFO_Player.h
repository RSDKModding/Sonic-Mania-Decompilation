#ifndef OBJ_UFO_PLAYER_H
#define OBJ_UFO_PLAYER_H

#include "Game.h"

// Object Class
struct ObjectUFO_Player {
    RSDK_OBJECT
    int32 maxSpeed;
    uint16 aniFrames; // unused
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
};

// Entity Class
struct EntityUFO_Player {
    RSDK_ENTITY
    StateMachine(state);
    int32 machQuota1;
    int32 machQuota2;
    int32 machQuota3;
    int32 startingRings;
    int32 timer;
    int32 courseOutTimer;
    int32 angleX;
    int32 height;
    int32 gravityStrength;
    int32 angleZ;
    int32 velDivisor;
    int32 bumperTimer;
    int32 angleVel;
    int32 skidTimer;
    EntityUFO_Camera *camera;
    EntityUFO_Circuit *circuitPtr;
    Matrix matRotate;
    Matrix matTransform;
    Matrix matWorld;
    Matrix matNormal;
    StateMachine(stateInput);
    int32 controllerID;
    bool32 up;
    bool32 down;
    bool32 left;
    bool32 right;
    bool32 jumpPress;
    bool32 jumpHold;
    Animator animator;
};

// Object Struct
extern ObjectUFO_Player *UFO_Player;

// Standard Entity Events
void UFO_Player_Update(void);
void UFO_Player_LateUpdate(void);
void UFO_Player_StaticUpdate(void);
void UFO_Player_Draw(void);
void UFO_Player_Create(void *data);
void UFO_Player_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Player_EditorDraw(void);
void UFO_Player_EditorLoad(void);
#endif
void UFO_Player_Serialize(void);

// Extra Entity Functions
void UFO_Player_Input_P1(void);
void UFO_Player_ChangeMachState(void);
void UFO_Player_HandleBumperTiles(void);
void UFO_Player_HandleSpeedUp(void);
void UFO_Player_State_Run(void);
void UFO_Player_State_Jump(void);
void UFO_Player_State_Springboard(void);
void UFO_Player_State_Trip(void);
void UFO_Player_State_CourseOut(void);
void UFO_Player_State_UFOCaught_Charge(void);
void UFO_Player_State_UFOCaught_Released(void);

#endif //! OBJ_UFO_PLAYER_H
