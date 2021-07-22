#ifndef OBJ_SELTZERWATER_H
#define OBJ_SELTZERWATER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectSeltzerWater;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int field_5C;
    int gravityStrength;
    int field_64;
    ushort nodeSlot;
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
void SeltzerWater_EditorDraw(void);
void SeltzerWater_EditorLoad(void);
void SeltzerWater_Serialize(void);

// Extra Entity Functions
void SeltzerWater_Unknown1(void);
void SeltzerWater_Unknown2(void);
void SeltzerWater_Unknown3(void);

#endif //!OBJ_SELTZERWATER_H
