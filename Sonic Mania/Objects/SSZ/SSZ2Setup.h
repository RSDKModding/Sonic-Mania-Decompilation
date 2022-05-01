#ifndef OBJ_SSZ2SETUP_H
#define OBJ_SSZ2SETUP_H

#include "SonicMania.h"

typedef enum { GENERICTRIGGER_SSZ2_DESTROYHOTARUMKII, GENERICTRIGGER_SSZ2_ACHIEVEMENT, GENERICTRIGGER_SSZ2_ACTTRANSITION } GenericTriggerTypesSSZ2;

typedef enum { SSZ2_TFLAGS_NORMAL, SSZ2_TFLAGS_SPARKS } TileFlagsSSZ2;

// Object Class
struct ObjectSSZ2Setup {
    RSDK_OBJECT
    uint8 towerID;
    uint16 sparkTimer;
    bool32 hasAchievement;
    uint16 sfxSpark;
    TileLayer *towerLayer;
};

// Entity Class
struct EntitySSZ2Setup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectSSZ2Setup *SSZ2Setup;

// Standard Entity Events
void SSZ2Setup_Update(void);
void SSZ2Setup_LateUpdate(void);
void SSZ2Setup_StaticUpdate(void);
void SSZ2Setup_Draw(void);
void SSZ2Setup_Create(void *data);
void SSZ2Setup_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SSZ2Setup_EditorDraw(void);
void SSZ2Setup_EditorLoad(void);
#endif
void SSZ2Setup_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
void SSZ2Setup_StageFinishCB(void);
#endif

void SSZ2Setup_TowerDrawLayerCB(void);
void SSZ2Setup_TowerScanlineCB(ScanlineInfo *scanlines);

void SSZ2Setup_GenericTriggerCB_DestroyHotaruMKII(void);
void SSZ2Setup_GenericTriggerCB_CheckSSZAchievement(void);
void SSZ2Setup_GenericTriggerCB_SSZ2BTransition(void);

#endif //! OBJ_SSZ2SETUP_H
