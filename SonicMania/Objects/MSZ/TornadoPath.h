#ifndef OBJ_TORNADOPATH_H
#define OBJ_TORNADOPATH_H

#include "Game.h"

typedef enum {
    TORNADOPATH_START,
    TORNADOPATH_DUD,
    TORNADOPATH_SETSPEED,
    TORNADOPATH_SETCAMERA,
    TORNADOPATH_DISABLEINTERACTIONS,
    TORNADOPATH_EXITTORNADO_STOPAUTOSCROLL,
    TORNADOPATH_EXITTORNADO_TRAINSEQ,
    TORNADOPATH_ENTERTORNADO,
    TORNADOPATH_ENTERTORNADO_FLYTOBOSS,
    TORNADOPATH_HANDLEBOSS_MSZ1ST,
    TORNADOPATH_SETSPEED_HIGH,
    TORNADOPATH_GOTOSTOP,
    TORNADOPATH_SETUPMSZ1CUTSCENEST,
    TORNADOPATH_CRASHFINISH,
} TornadoPathTypes;

// Object Class
struct ObjectTornadoPath {
    RSDK_OBJECT
    EntityCamera *camera;
    Vector2 moveVel;
    int32 hitboxID;
    uint16 aniFrames;
    Animator animator;
    bool32 crashFinished;
};

// Entity Class
struct EntityTornadoPath {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 targetSpeed;
    int32 timer;
    bool32 easeToSpeed;
    bool32 fastMode;
    Vector2 size;
    int32 forceSpeed;
    Hitbox hitbox;
    int32 currentSpeed;
    Animator animator;
};

// Object Struct
extern ObjectTornadoPath *TornadoPath;

// Standard Entity Events
void TornadoPath_Update(void);
void TornadoPath_LateUpdate(void);
void TornadoPath_StaticUpdate(void);
void TornadoPath_Draw(void);
void TornadoPath_Create(void *data);
void TornadoPath_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TornadoPath_EditorDraw(void);
void TornadoPath_EditorLoad(void);
#endif
void TornadoPath_Serialize(void);

// Extra Entity Functions
void TornadoPath_SetupHitbox(void);
void TornadoPath_HandleMoveSpeed(void);
void TornadoPath_State_SetTornadoSpeed(void);
void TornadoPath_State_ReturnCamera(void);
void TornadoPath_State_DisablePlayerInteractions(void);
void TornadoPath_State_ExitTornadoSequence(void);
void TornadoPath_State_PrepareCatchPlayer(void);
void TornadoPath_State_CatchPlayer(void);
void TornadoPath_State_UberCaterkillerBossNode(void);
void TornadoPath_State_HandleUberCaterkillerBoss(void);
void TornadoPath_State_GoToStopNode(void);
void TornadoPath_State_SetupMSZ1CutsceneST(void);
void TornadoPath_State_FinishCrash(void);

#endif //! OBJ_TORNADOPATH_H
