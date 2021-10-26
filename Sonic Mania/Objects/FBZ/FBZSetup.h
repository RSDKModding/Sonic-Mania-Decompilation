#ifndef OBJ_FBZSETUP_H
#define OBJ_FBZSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    int32 frameA;
    int32 frameC;
    int32 frameD;
    int32 frameB;
    int32 array1[0x400];
    int32 array2[0x400];
    int32 array3[0x400];
    uint16 aniTiles;
    uint16 bgOutsideLayer;
    EntityFBZ1Outro *outroPtr;
} ObjectFBZSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFBZSetup;

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

void FBZSetup_Unknown5(ScanlineInfo *scanlines, int32 a1, int32 a3, int32 a4, int32 a5, int32 a6);
void FBZSetup_ScanlineCallback(ScanlineInfo *scanlines);

void FBZSetup_BGSwitchCB_A(void);
void FBZSetup_BGSwitchCB_B(void);
void FBZSetup_BGSwitchCB_C(void);

void FBZSetup_GenericTriggerCB_A(void);
void FBZSetup_GenericTriggerCB_B(void);

void FBZSetup_StageFinishCB_Act1(void);
#if RETRO_USE_PLUS
void FBZSetup_StageFinishCB_Act2(void);
#endif

#endif //!OBJ_FBZSETUP_H
