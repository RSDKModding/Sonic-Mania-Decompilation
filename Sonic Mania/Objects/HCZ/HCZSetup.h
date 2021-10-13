#ifndef OBJ_HCZSETUP_H
#define OBJ_HCZSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int aniTilesDelay[14], { 4, 3, 2, 2, 2, 3, 3, 4, 3, 2, 2, 2, 3, 3 });
    int field_3C;
    int timerB;
    int timerA;
    int field_48;
    ushort aniTiles1;
    ushort aniTiles2;
    ushort aniTiles3;
    TileLayer *bg;
#if RETRO_USE_PLUS
    int activePlayerCount;
    bool32 playingLoopSFX;
    int waterfallSFXTimer;
    ushort sfxWaterfall;
    ushort sfxWaterfallLoop;
#endif
} ObjectHCZSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHCZSetup;

// Object Struct
extern ObjectHCZSetup *HCZSetup;

// Standard Entity Events
void HCZSetup_Update(void);
void HCZSetup_LateUpdate(void);
void HCZSetup_StaticUpdate(void);
void HCZSetup_Draw(void);
void HCZSetup_Create(void* data);
void HCZSetup_StageLoad(void);
void HCZSetup_EditorDraw(void);
void HCZSetup_EditorLoad(void);
void HCZSetup_Serialize(void);

// Extra Entity Functions
void HCZSetup_ScanlineCallback(ScanlineInfo *scanlines);
void HCZSetup_HandleActTransition(void);
void HCZSetup_StageFinishCB_Act2(void);

#endif //!OBJ_HCZSETUP_H
