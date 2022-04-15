#ifndef OBJ_FBZSETUP_H
#define OBJ_FBZSETUP_H

#include "SonicMania.h"

typedef enum { FBZ_GENERICTRIGGER_EXTERIOR, FBZ_GENERICTRIGGER_INTERIOR } GenericTriggerTypesFBZ;

typedef enum {
    FBZ_BG_INSIDE1,
    FBZ_BG_INSIDE2,
    FBZ_BG_INSIDE1_DUP,
} BGSwitchIDsFBZ;

typedef enum {
    FBZ_DECORATION_BLUEPRINT1,
    FBZ_DECORATION_BLUEPRINT2,
} DecorationTypesFBZ;

// Object Class
struct ObjectFBZSetup {
    RSDK_OBJECT
    int32 unused;
    int32 frameA;
    int32 frameC;
    int32 frameD;
    int32 frameB;
    int32 deformX[0x400];
    int32 positionYMove[0x400];
    int32 positionY[0x400];
    uint16 aniTiles;
    uint16 bgOutsideLayer;
    EntityFBZ1Outro *outroPtr;
};

// Entity Class
struct EntityFBZSetup {
	RSDK_ENTITY
};

// Object Struct
extern ObjectFBZSetup *FBZSetup;

// Standard Entity Events
void FBZSetup_Update(void);
void FBZSetup_LateUpdate(void);
void FBZSetup_StaticUpdate(void);
void FBZSetup_Draw(void);
void FBZSetup_Create(void* data);
void FBZSetup_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void FBZSetup_EditorDraw(void);
void FBZSetup_EditorLoad(void);
#endif
void FBZSetup_Serialize(void);

// Extra Entity Functions
void FBZSetup_ActTransitionLoad(void);

void FBZSetup_HandleScanlines(ScanlineInfo *scanlines, int32 a1, int32 a3, int32 a4, int32 a5, int32 a6);
void FBZSetup_ScanlineCallback(ScanlineInfo *scanlines);

void FBZSetup_BGSwitchCB_ShowInside1(void);
void FBZSetup_BGSwitchCB_ShowInside2(void);
void FBZSetup_BGSwitchCB_ShowInside1_Dup(void);

void FBZSetup_GenericTriggerCB_ShowExterior(void);
void FBZSetup_GenericTriggerCB_ShowInterior(void);

void FBZSetup_StageFinishCB_Act1(void);
#if RETRO_USE_PLUS
void FBZSetup_StageFinishCB_Act2(void);
#endif

#endif //!OBJ_FBZSETUP_H
