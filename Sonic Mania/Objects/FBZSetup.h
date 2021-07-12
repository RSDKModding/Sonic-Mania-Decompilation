#ifndef OBJ_FBZSETUP_H
#define OBJ_FBZSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int frameA;
    int frameC;
    int frameD;
    int frameB;
    int array1[0x400];
    int array2[0x400];
    int array3[0x400];
    ushort aniTiles;
    ushort bgOutsideLayer;
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
void FBZSetup_EditorDraw(void);
void FBZSetup_EditorLoad(void);
void FBZSetup_Serialize(void);

// Extra Entity Functions
void FBZSetup_ActTransitionLoad(void);

void FBZSetup_Unknown5(ScanlineInfo *scanlines, int a1, int a3, int a4, int a5, int a6);
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
