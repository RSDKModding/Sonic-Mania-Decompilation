#ifndef OBJ_TORNADOPATH_H
#define OBJ_TORNADOPATH_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    EntityCamera *cameraPtr;
    Vector2 field_8;
    int hitboxID;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
} ObjectTornadoPath;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int type;
    int targetSpeed;
    int timer;
    bool32 easeToSpeed;
    bool32 fastMode;
    Vector2 size;
    int forceSpeed;
    Hitbox hitbox;
    int targetSpeedStore;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
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
