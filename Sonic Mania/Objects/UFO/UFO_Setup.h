#ifndef OBJ_UFO_SETUP_H
#define OBJ_UFO_SETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    StateMachine(state);
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
void UFO_Setup_Unknown8(void);
void UFO_Setup_Unknown9(void);
void UFO_Setup_Unknown10(void);
void UFO_Setup_Unknown11(void);
void UFO_Setup_Unknown12(void);
void UFO_Setup_Unknown13(void);
void UFO_Setup_Unknown14(void);

#endif //!OBJ_UFO_SETUP_H
