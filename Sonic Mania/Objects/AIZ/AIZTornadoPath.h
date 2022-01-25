#ifndef OBJ_AIZTORNADOPATH_H
#define OBJ_AIZTORNADOPATH_H

#include "SonicMania.h"

typedef enum {
    AIZTORNADOPATH_START,
    AIZTORNADOPATH_DUD,
    AIZTORNADOPATH_SETSPEED,
    AIZTORNADOPATH_SETCAMERA,
    AIZTORNADOPATH_DISABLEINTERACTIONS,
    AIZTORNADOPATH_JUMPOFF,
    AIZTORNADOPATH_6,
    AIZTORNADOPATH_SETSPEED_ALT,
} AIZTornadoPathTypes;

// Object Class
struct ObjectAIZTornadoPath {
    RSDK_OBJECT
    EntityAIZTornado *tornado;
    Entity *camera;
    Vector2 moveVel;
    uint16 aniFrames;
    Animator animator;
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
    Animator animator;
};

// Object Struct
extern ObjectAIZTornadoPath *AIZTornadoPath;

// Standard Entity Events
void AIZTornadoPath_Update(void);
void AIZTornadoPath_LateUpdate(void);
void AIZTornadoPath_StaticUpdate(void);
void AIZTornadoPath_Draw(void);
void AIZTornadoPath_Create(void* data);
void AIZTornadoPath_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void AIZTornadoPath_EditorDraw(void);
void AIZTornadoPath_EditorLoad(void);
#endif
void AIZTornadoPath_Serialize(void);

// Extra Entity Functions
void AIZTornadoPath_HandleMoveSpeed(void);
void AIZTornadoPath_State_SetTornadoSpeed(void);
void AIZTornadoPath_State_SetPlayerCamera(void);
void AIZTornadoPath_State_DisablePlayerInteractions(void);
void AIZTornadoPath_JumpOffTornado(void);
void AIZTornadoPath_State_MoveRightJump(void);
void AIZTornadoPath_State_FlyOff(void);

#endif //!OBJ_AIZTORNADOPATH_H
