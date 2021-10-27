#ifndef OBJ_LIGHTBULB_H
#define OBJ_LIGHTBULB_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxBulbPop;
} ObjectLightBulb;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    bool32 respawn;
    int32 priority;
    Animator animator;
} EntityLightBulb;

// Object Struct
extern ObjectLightBulb *LightBulb;

// Standard Entity Events
void LightBulb_Update(void);
void LightBulb_LateUpdate(void);
void LightBulb_StaticUpdate(void);
void LightBulb_Draw(void);
void LightBulb_Create(void* data);
void LightBulb_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LightBulb_EditorDraw(void);
void LightBulb_EditorLoad(void);
#endif
void LightBulb_Serialize(void);

// Extra Entity Functions
void LightBulb_DebugSpawn(void);
void LightBulb_DebugDraw(void);

void LightBulb_State_CheckPlayerCollisions(void);
void LightBulb_State_Destroyed(void);

#endif //!OBJ_LIGHTBULB_H
