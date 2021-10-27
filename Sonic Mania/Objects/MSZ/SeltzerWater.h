#ifndef OBJ_SELTZERWATER_H
#define OBJ_SELTZERWATER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectSeltzerWater;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 field_5C;
    int32 gravityStrength;
    int32 field_64;
    uint16 nodeSlot;
    Animator animator;
} EntitySeltzerWater;

// Object Struct
extern ObjectSeltzerWater *SeltzerWater;

// Standard Entity Events
void SeltzerWater_Update(void);
void SeltzerWater_LateUpdate(void);
void SeltzerWater_StaticUpdate(void);
void SeltzerWater_Draw(void);
void SeltzerWater_Create(void* data);
void SeltzerWater_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SeltzerWater_EditorDraw(void);
void SeltzerWater_EditorLoad(void);
#endif
void SeltzerWater_Serialize(void);

// Extra Entity Functions
void SeltzerWater_Unknown1(void);
void SeltzerWater_Unknown2(void);
void SeltzerWater_Unknown3(void);

#endif //!OBJ_SELTZERWATER_H
