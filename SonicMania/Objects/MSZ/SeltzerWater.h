#ifndef OBJ_SELTZERWATER_H
#define OBJ_SELTZERWATER_H

#include "Game.h"

// Object Class
struct ObjectSeltzerWater {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntitySeltzerWater {
    RSDK_ENTITY
    StateMachine(state);
    int32 offsetAngle;
    int32 gravityStrength;
    int32 oscillateRadius;
    uint16 nodeSlot;
    Animator animator;
};

// Object Struct
extern ObjectSeltzerWater *SeltzerWater;

// Standard Entity Events
void SeltzerWater_Update(void);
void SeltzerWater_LateUpdate(void);
void SeltzerWater_StaticUpdate(void);
void SeltzerWater_Draw(void);
void SeltzerWater_Create(void *data);
void SeltzerWater_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SeltzerWater_EditorDraw(void);
void SeltzerWater_EditorLoad(void);
#endif
void SeltzerWater_Serialize(void);

// Extra Entity Functions
void SeltzerWater_State_Sprayed(void);
void SeltzerWater_State_Falling(void);
void SeltzerWater_State_Splash(void);

#endif //! OBJ_SELTZERWATER_H
