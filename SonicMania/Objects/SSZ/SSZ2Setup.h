#ifndef OBJ_SSZ2SETUP_H
#define OBJ_SSZ2SETUP_H

#include "Game.h"

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
#if GAME_INCLUDE_EDITOR
void SSZ2Setup_EditorDraw(void);
void SSZ2Setup_EditorLoad(void);
#endif
void SSZ2Setup_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
void SSZ2Setup_StageFinish_EndAct2(void);
#endif

void SSZ2Setup_DrawHook_PrepareDrawingFX(void);
void SSZ2Setup_Scanline_BGTower(ScanlineInfo *scanlines);

void SSZ2Setup_Trigger_DestroyHotaruMKII(void);
void SSZ2Setup_Trigger_AwardAchievement(void);
void SSZ2Setup_Trigger_SSZ2BTransition(void);

#endif //! OBJ_SSZ2SETUP_H
