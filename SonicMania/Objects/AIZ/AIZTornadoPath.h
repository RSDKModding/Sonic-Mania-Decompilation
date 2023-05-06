#ifndef OBJ_AIZTORNADOPATH_H
#define OBJ_AIZTORNADOPATH_H

#include "Game.h"

typedef enum {
    AIZTORNADOPATH_START,
    AIZTORNADOPATH_DUD,
    AIZTORNADOPATH_SETSPEED,
    AIZTORNADOPATH_SETCAMERA,
    AIZTORNADOPATH_DISABLEINTERACTIONS,
    AIZTORNADOPATH_EXITTORNADO,
    AIZTORNADOPATH_ENTERTORNADO,
    AIZTORNADOPATH_TARGETNODE,
} AIZTornadoPathTypes;

// Object Class
struct ObjectAIZTornadoPath {
    RSDK_OBJECT
    EntityAIZTornado *tornado;
    EntityCamera *camera;
    Vector2 moveVel;
    uint16 aniFrames; // prolly editor-only
    Animator unusedAnimator;
};

// Entity Class
struct EntityAIZTornadoPath {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 targetSpeed;
    int32 timer;
    bool32 easeToSpeed;
    int32 speed;
    Animator unusedAnimator;
};

// Object Struct
extern ObjectAIZTornadoPath *AIZTornadoPath;

// Standard Entity Events
void AIZTornadoPath_Update(void);
void AIZTornadoPath_LateUpdate(void);
void AIZTornadoPath_StaticUpdate(void);
void AIZTornadoPath_Draw(void);
void AIZTornadoPath_Create(void *data);
void AIZTornadoPath_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void AIZTornadoPath_EditorDraw(void);
void AIZTornadoPath_EditorLoad(void);
#endif
void AIZTornadoPath_Serialize(void);

// Extra Entity Functions
void AIZTornadoPath_HandleMoveSpeed(void);
void AIZTornadoPath_State_SetTornadoSpeed(void);
void AIZTornadoPath_State_SetPlayerCamera(void);
void AIZTornadoPath_State_DisablePlayerInteractions(void);
void AIZTornadoPath_State_ExitTornadoSequence(void);
void AIZTornadoPath_State_PrepareCatchPlayer(void);
void AIZTornadoPath_State_CatchPlayer(void);

#endif //! OBJ_AIZTORNADOPATH_H
