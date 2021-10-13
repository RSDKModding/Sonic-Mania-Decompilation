#ifndef OBJ_TORNADOPATH_H
#define OBJ_TORNADOPATH_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    EntityCamera *cameraPtr;
    Vector2 field_8;
    int32 hitboxID;
    int32 field_14;
    int32 field_18;
    int32 field_1C;
    int32 field_20;
    int32 field_24;
    int32 field_28;
    int32 field_2C;
    int32 field_30;
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
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    int32 field_98;
    int32 field_9C;
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
