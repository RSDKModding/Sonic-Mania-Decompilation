#ifndef OBJ_TORNADOPATH_H
#define OBJ_TORNADOPATH_H

#include "SonicMania.h"

typedef enum {
    TORNADOPATH_START,
    TORNADOPATH_DUD,
    TORNADOPATH_SETSPEED,
    TORNADOPATH_SETCAMERA,
    TORNADOPATH_4,
    TORNADOPATH_5,
    TORNADOPATH_6,
    TORNADOPATH_7,
    TORNADOPATH_8,
    TORNADOPATH_9,
    TORNADOPATH_10,
    TORNADOPATH_11,
    TORNADOPATH_12,
    TORNADOPATH_13,
}TornadoPathTypes;

// Object Class
struct ObjectTornadoPath {
    RSDK_OBJECT
    EntityCamera *cameraPtr;
    Vector2 moveVel;
    int32 hitboxID;
    uint16 aniFrames;
    Animator animator;
    bool32 flag;
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
    int32 targetSpeedStore;
    Animator animator;
};

// Object Struct
extern ObjectTornadoPath *TornadoPath;

// Standard Entity Events
void TornadoPath_Update(void);
void TornadoPath_LateUpdate(void);
void TornadoPath_StaticUpdate(void);
void TornadoPath_Draw(void);
void TornadoPath_Create(void* data);
void TornadoPath_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
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
void TornadoPath_Unknown6(void);
void TornadoPath_State_MoveRightJump(void);
void TornadoPath_State_FlyOff(void);
void TornadoPath_Unknown9(void);
void TornadoPath_Unknown10(void);
void TornadoPath_Unknown11(void);
void TornadoPath_Unknown12(void);
void TornadoPath_Unknown13(void);

#endif //!OBJ_TORNADOPATH_H
