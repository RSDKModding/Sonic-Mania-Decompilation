#ifndef OBJ_UFO_SETUP_H
#define OBJ_UFO_SETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    StateMachine(deformCB);
    int32 timedOut;
    bool32 encoreStage;
    int32 machLevel;
    int32 scoreBonus;
    int32 machPoints;
    int32 rings;
    int32 timer;
    int32 ringFrame;
    int32 ringPan;
    int32 spherePan;
    int32 specialStageID;
    int32 machQuotas[3];
    uint8 playFieldLayer;
    uint16 sfxBlueSphere;
    uint16 sfxSSExit;
    uint16 sfxEmerald;
    uint16 sfxEvent;
    bool32 resetToTitle;
} ObjectUFO_Setup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    colour fadeColour;
} EntityUFO_Setup;

// Object Struct
extern ObjectUFO_Setup *UFO_Setup;

// Standard Entity Events
void UFO_Setup_Update(void);
void UFO_Setup_LateUpdate(void);
void UFO_Setup_StaticUpdate(void);
void UFO_Setup_Draw(void);
void UFO_Setup_Create(void* data);
void UFO_Setup_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UFO_Setup_EditorDraw(void);
void UFO_Setup_EditorLoad(void);
#endif
void UFO_Setup_Serialize(void);

// Extra Entity Functions
void UFO_Setup_DrawLayerCallback(void);
void UFO_Setup_ScanlineCallback_Playfield(ScanlineInfo *scanlines);
void UFO_Setup_ScanlineCallback_3DFloor(ScanlineInfo *scanlines);
void UFO_Setup_ScanlineCallback_3DRoof(ScanlineInfo *scanlines);

void UFO_Setup_PlaySphereSFX(void);
void UFO_Setup_Finish_Win(void);
void UFO_Setup_Finish_Fail(void);
void UFO_Setup_DeformCB_UFO3(void);
void UFO_Setup_DeformCB_UFO4(void);
void UFO_Setup_DeformCB_UFO5(void);
void UFO_Setup_State_ShowStartMessage(void);
void UFO_Setup_State_FinishFadeout(void);
void UFO_Setup_State_HandleRingDrain(void);
void UFO_Setup_State_TimedOver(void);

#endif //!OBJ_UFO_SETUP_H
