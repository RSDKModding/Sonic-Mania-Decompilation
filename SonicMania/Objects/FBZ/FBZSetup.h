#ifndef OBJ_FBZSETUP_H
#define OBJ_FBZSETUP_H

#include "Game.h"

typedef enum { FBZ_GENERICTRIGGER_EXTERIOR, FBZ_GENERICTRIGGER_INTERIOR } GenericTriggerTypesFBZ;

typedef enum {
    FBZ_BG_INSIDE1,
    FBZ_BG_INSIDE2,
    FBZ_BG_INSIDE1_NOSTORM,
} BGSwitchIDsFBZ;

typedef enum {
    FBZ_DECORATION_BLUEPRINT1,
    FBZ_DECORATION_BLUEPRINT2,
} DecorationTypesFBZ;

typedef enum {
    FBZ_PARALLAXSPRITE_CLOUDS,
} ParallaxSpriteAniIDsFBZ;

// Object Class
struct ObjectFBZSetup {
    RSDK_OBJECT
    int32 unused;
    int32 cylinderAniFrame;
    int32 propellerShaftAniFrame;
    int32 spiralPlatformAniFrame;
    int32 propellerShaftAniFrameUnused;
    int32 scanlineDeformX[0x400];
    int32 positionYMove[0x400];
    int32 scanlinePosY[0x400];
    uint16 aniTiles;
    uint16 backgroundOutside;
    EntityFBZ1Outro *outro;
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
void FBZSetup_Create(void *data);
void FBZSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FBZSetup_EditorDraw(void);
void FBZSetup_EditorLoad(void);
#endif
void FBZSetup_Serialize(void);

// Extra Entity Functions
void FBZSetup_ActTransitionLoad(void);

void FBZSetup_AddDynamicBG(ScanlineInfo *scanlines, int32 parallaxFactorX, int32 parallaxFactorY, int32 startLine, int32 lineCount, int32 sourcePosY);
void FBZSetup_Scanline_BGInside(ScanlineInfo *scanlines);

void FBZSetup_BGSwitch_ShowInside1(void);
void FBZSetup_BGSwitch_ShowInside2(void);
void FBZSetup_BGSwitch_ShowInside1_NoStorm(void);

void FBZSetup_Trigger_ShowExterior(void);
void FBZSetup_Trigger_ShowInterior(void);

void FBZSetup_StageFinish_EndAct1(void);
#if MANIA_USE_PLUS
void FBZSetup_StageFinish_EndAct2(void);
#endif

#endif //! OBJ_FBZSETUP_H
