#ifndef OBJ_UFO_SETUP_H
#define OBJ_UFO_SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    void(*state)();
    int timedOut;
    bool32 encoreStage;
    int machLevel;
    int field_14;
    int dword18;
    int rings;
    int timer;
    int ringFrame;
    int ringPan;
    int spherePan;
    int specialStageID;
    int machQuota1;
    int machQuota2;
    int machQuota3;
    byte playFieldLayer;
    char field_41;
    ushort sfx_BlueSphere;
    ushort sfx_SSExit;
    ushort sfx_Emerald;
    ushort sfx_Event;
    int dword4C;
} ObjectUFO_Setup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    int timer;
    colour fadeColour;
} EntityUFO_Setup;

// Object Struct
extern ObjectUFO_Setup *UFO_Setup;

// Standard Entity Events
void UFO_Setup_Update();
void UFO_Setup_LateUpdate();
void UFO_Setup_StaticUpdate();
void UFO_Setup_Draw();
void UFO_Setup_Create(void* data);
void UFO_Setup_StageLoad();
void UFO_Setup_EditorDraw();
void UFO_Setup_EditorLoad();
void UFO_Setup_Serialize();

// Extra Entity Functions
void UFO_Setup_DrawLayerCallback(void);
void UFO_Setup_ScanlineCallback_Playfield(ScanlineInfo *scanlines);
void UFO_Setup_ScanlineCallback_3DFloor(ScanlineInfo *scanlines);
void UFO_Setup_ScanlineCallback_3DRoof(ScanlineInfo *scanlines);

void UFO_Setup_PlaySphereSFX();
void UFO_Setup_Unknown6();
void UFO_Setup_Unknown7();
void UFO_Setup_Unknown8();
void UFO_Setup_Unknown9();
void UFO_Setup_Unknown10();
void UFO_Setup_Unknown11();
void UFO_Setup_Unknown12();
void UFO_Setup_Unknown13();
void UFO_Setup_Unknown14();

#endif //!OBJ_UFO_SETUP_H
