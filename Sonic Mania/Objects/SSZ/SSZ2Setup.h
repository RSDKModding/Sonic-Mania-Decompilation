#ifndef OBJ_SSZ2SETUP_H
#define OBJ_SSZ2SETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint8 towerID;
    uint16 sparkTimer;
    bool32 hasAchievement;
    uint16 sfxSpark;
    TileLayer *towerLayer;
} ObjectSSZ2Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySSZ2Setup;

// Object Struct
extern ObjectSSZ2Setup *SSZ2Setup;

// Standard Entity Events
void SSZ2Setup_Update(void);
void SSZ2Setup_LateUpdate(void);
void SSZ2Setup_StaticUpdate(void);
void SSZ2Setup_Draw(void);
void SSZ2Setup_Create(void* data);
void SSZ2Setup_StageLoad(void);
void SSZ2Setup_EditorDraw(void);
void SSZ2Setup_EditorLoad(void);
void SSZ2Setup_Serialize(void);

// Extra Entity Functions
void SSZ2Setup_StageFinishCallback(void);

void SSZ2Setup_TowerDrawLayerCallback(void);
void SSZ2Setup_TowerScanlineCallback(ScanlineInfo *scanlines);

void SSZ2Setup_GenericTriggerCallback1(void);
void SSZ2Setup_GenericTriggerCallback2(void);
void SSZ2Setup_GenericTriggerCallback3(void);

#endif //!OBJ_SSZ2SETUP_H
