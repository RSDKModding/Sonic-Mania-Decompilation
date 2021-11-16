#ifndef OBJ_AIZTORNADOPATH_H
#define OBJ_AIZTORNADOPATH_H

#include "SonicMania.h"

typedef enum {
    AIZTORNADOPATH_0,
    AIZTORNADOPATH_1,
    AIZTORNADOPATH_2,
    AIZTORNADOPATH_3,
    AIZTORNADOPATH_4,
    AIZTORNADOPATH_5,
    AIZTORNADOPATH_6,
    AIZTORNADOPATH_7,
} AIZTornadoPathTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    EntityAIZTornado *tornado;
    Entity *camera;
    Vector2 field_C;
    uint16 aniFrames;
    Animator animator;
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
    Animator animator;
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
#if RETRO_INCLUDE_EDITOR
void AIZTornadoPath_EditorDraw(void);
void AIZTornadoPath_EditorLoad(void);
#endif
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
