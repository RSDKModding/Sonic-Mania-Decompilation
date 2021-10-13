#ifndef OBJ_AIZTORNADOPATH_H
#define OBJ_AIZTORNADOPATH_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    EntityAIZTornado *tornado;
    Entity *camera;
    Vector2 field_C;
    int32 field_14;
    int32 field_18;
    int32 field_1C;
    int32 field_20;
    int32 field_24;
    int32 field_28;
    int32 field_2C;
} ObjectAIZTornadoPath;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 targetSpeed;
    int32 timer;
    bool32 easeToSpeed;
    int32 speed;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
} EntityAIZTornadoPath;

// Object Struct
extern ObjectAIZTornadoPath *AIZTornadoPath;

// Standard Entity Events
void AIZTornadoPath_Update(void);
void AIZTornadoPath_LateUpdate(void);
void AIZTornadoPath_StaticUpdate(void);
void AIZTornadoPath_Draw(void);
void AIZTornadoPath_Create(void* data);
void AIZTornadoPath_StageLoad(void);
void AIZTornadoPath_EditorDraw(void);
void AIZTornadoPath_EditorLoad(void);
void AIZTornadoPath_Serialize(void);

// Extra Entity Functions
void AIZTornadoPath_Unknown1(void);
void AIZTornadoPath_Unknown2(void);
void AIZTornadoPath_Unknown3(void);
void AIZTornadoPath_Unknown4(void);
void AIZTornadoPath_Unknown5(void);
void AIZTornadoPath_Unknown6(void);
void AIZTornadoPath_Unknown7(void);

#endif //!OBJ_AIZTORNADOPATH_H
