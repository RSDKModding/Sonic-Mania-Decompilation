#ifndef OBJ_UFO_SETUP_H
#define OBJ_UFO_SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    void(*state)(void);
    int timedOut;
    bool32 encoreStage;
    int machLevel;
    int scoreBonus;
    int machPoints;
    int rings;
    int timer;
    int ringFrame;
    int ringPan;
    int spherePan;
    int specialStageID;
    int machQuotas[3];
    byte playFieldLayer;
    ushort sfx_BlueSphere;
    ushort sfx_SSExit;
    ushort sfx_Emerald;
    ushort sfx_Event;
    bool32 resetToTitle;
} ObjectUFO_Setup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)(void);
    int timer;
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
void UFO_Setup_EditorDraw(void);
void UFO_Setup_EditorLoad(void);
void UFO_Setup_Serialize(void);

// Extra Entity Functions
void UFO_Setup_DrawLayerCallback(void);
void UFO_Setup_ScanlineCallback_Playfield(ScanlineInfo *scanlines);
void UFO_Setup_ScanlineCallback_3DFloor(ScanlineInfo *scanlines);
void UFO_Setup_ScanlineCallback_3DRoof(ScanlineInfo *scanlines);

void UFO_Setup_PlaySphereSFX(void);
void UFO_Setup_Unknown6(void);
void UFO_Setup_Unknown7(void);
void UFO_Setup_Unknown8(void);
void UFO_Setup_Unknown9(void);
void UFO_Setup_Unknown10(void);
void UFO_Setup_Unknown11(void);
void UFO_Setup_Unknown12(void);
void UFO_Setup_Unknown13(void);
void UFO_Setup_Unknown14(void);

#endif //!OBJ_UFO_SETUP_H
