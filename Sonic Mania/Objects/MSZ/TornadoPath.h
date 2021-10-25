#ifndef OBJ_TORNADOPATH_H
#define OBJ_TORNADOPATH_H

#include "SonicMania.h"

typedef enum {
    TORNADOPATH_0,
    TORNADOPATH_1,
    TORNADOPATH_2,
    TORNADOPATH_3,
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
typedef struct {
    RSDK_OBJECT
    EntityCamera *cameraPtr;
    Vector2 field_8;
    int32 hitboxID;
    uint16 aniFrames;
    Animator animator;
    bool32 flag;
} ObjectTornadoPath;

// Entity Class
typedef struct {
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
} EntityTornadoPath;

// Object Struct
extern ObjectTornadoPath *TornadoPath;

// Standard Entity Events
void TornadoPath_Update(void);
void TornadoPath_LateUpdate(void);
void TornadoPath_StaticUpdate(void);
void TornadoPath_Draw(void);
void TornadoPath_Create(void* data);
void TornadoPath_StageLoad(void);
void TornadoPath_EditorDraw(void);
void TornadoPath_EditorLoad(void);
void TornadoPath_Serialize(void);

// Extra Entity Functions
void TornadoPath_SetupHitbox(void);
void TornadoPath_Unknown2(void);
void TornadoPath_Unknown3(void);
void TornadoPath_State_ReturnCamera(void);
void TornadoPath_Unknown5(void);
void TornadoPath_Unknown6(void);
void TornadoPath_Unknown7(void);
void TornadoPath_Unknown8(void);
void TornadoPath_Unknown9(void);
void TornadoPath_Unknown10(void);
void TornadoPath_Unknown11(void);
void TornadoPath_Unknown12(void);
void TornadoPath_Unknown13(void);

#endif //!OBJ_TORNADOPATH_H
