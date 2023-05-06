#ifndef OBJ_UFO_SETUP_H
#define OBJ_UFO_SETUP_H

#include "Game.h"

typedef enum {
    UFO_TFLAGS_NORMAL,
    UFO_TFLAGS_BUMPER,
    UFO_TFLAGS_GRITTYGROUND,
} TileFlagsUFO;

// Object Class
struct ObjectUFO_Setup {
    RSDK_OBJECT
    StateMachine(deformCB);
    int32 timedOut;
#if MANIA_USE_PLUS
    bool32 encoreStage;
#endif
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
};

// Entity Class
struct EntityUFO_Setup {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    color fadeColor;
};

// Object Struct
extern ObjectUFO_Setup *UFO_Setup;

// Standard Entity Events
void UFO_Setup_Update(void);
void UFO_Setup_LateUpdate(void);
void UFO_Setup_StaticUpdate(void);
void UFO_Setup_Draw(void);
void UFO_Setup_Create(void *data);
void UFO_Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Setup_EditorDraw(void);
void UFO_Setup_EditorLoad(void);
#endif
void UFO_Setup_Serialize(void);

// Extra Entity Functions
void UFO_Setup_DrawHook_PrepareDrawingFX(void);
void UFO_Setup_Scanline_Playfield(ScanlineInfo *scanlines);
void UFO_Setup_Scanline_3DFloor(ScanlineInfo *scanlines);
void UFO_Setup_Scanline_3DRoof(ScanlineInfo *scanlines);

void UFO_Setup_PlaySphereSfx(void);
void UFO_Setup_Finish_Win(void);
void UFO_Setup_Finish_Fail(void);
void UFO_Setup_Deform_UFO3(void);
void UFO_Setup_Deform_UFO4(void);
void UFO_Setup_Deform_UFO5(void);
void UFO_Setup_State_ShowStartMessage(void);
void UFO_Setup_State_FinishFadeout(void);
void UFO_Setup_State_HandleRingDrain(void);
void UFO_Setup_State_TimedOver(void);

#endif //! OBJ_UFO_SETUP_H
