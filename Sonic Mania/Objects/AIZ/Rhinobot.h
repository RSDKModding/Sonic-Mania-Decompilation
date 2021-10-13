#ifndef OBJ_RHINOBOT_H
#define OBJ_RHINOBOT_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxHuff;
} ObjectRhinobot;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(timedState);
    int32 field_60;
    int32 timer;
    int32 field_68;
    int32 field_6C;
    bool32 flag;
    int32 field_74;
    int32 field_78;
    Vector2 startPos;
    char startDir;
    Animator animatorBot;
    Animator animatorDust;
} EntityRhinobot;

// Object Struct
extern ObjectRhinobot *Rhinobot;

// Standard Entity Events
void Rhinobot_Update(void);
void Rhinobot_LateUpdate(void);
void Rhinobot_StaticUpdate(void);
void Rhinobot_Draw(void);
void Rhinobot_Create(void* data);
void Rhinobot_StageLoad(void);
void Rhinobot_EditorDraw(void);
void Rhinobot_EditorLoad(void);
void Rhinobot_Serialize(void);

// Extra Entity Functions
bool32 Rhinobot_CheckTileCollisions(void);
void Rhinobot_CheckHit(void);
void Rhinobot_CheckOnScreen(void);

void Rhinobot_DebugDraw(void);
void Rhinobot_DebugSpawn(void);

void Rhinobot_State4(void);
void Rhinobot_StateSkidding(void);
void Rhinobot_StateFinishSkid(void);
void Rhinobot_State1(void);

void Rhinobot_State2(void);
void Rhinobot_HandleUpdate2(void);
void Rhinobot_HandleUpdate3(void);
void Rhinobot_HandleUpdate4(void);

#endif

#endif //!OBJ_RHINOBOT_H
