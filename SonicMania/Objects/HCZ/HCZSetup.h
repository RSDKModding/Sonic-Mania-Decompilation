#ifndef OBJ_HCZSETUP_H
#define OBJ_HCZSETUP_H

#include "Game.h"

// Object Class
struct ObjectHCZSetup {
    RSDK_OBJECT
    TABLE(int32 pendulumAniTileDurations[14], { 4, 3, 2, 2, 2, 3, 3, 4, 3, 2, 2, 2, 3, 3 });
    int32 unused1;
    int32 waterlineAniTileFrame;
    int32 bgCandlesAniTileFrame;
    int32 unused2;
    uint16 bgCandlesAniTiles;
    uint16 waterlineAniTiles;
    uint16 pendulumAniTiles;
    TileLayer *background2Layer;
#if MANIA_USE_PLUS
    int32 waterslidingPlayerCount;
    bool32 playingWaterfallLoop;
    int32 waterfallLoopTimer;
    uint16 sfxWaterfall;
    uint16 sfxWaterfallLoop;
#endif
};

// Entity Class
struct EntityHCZSetup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectHCZSetup *HCZSetup;

// Standard Entity Events
void HCZSetup_Update(void);
void HCZSetup_LateUpdate(void);
void HCZSetup_StaticUpdate(void);
void HCZSetup_Draw(void);
void HCZSetup_Create(void *data);
void HCZSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HCZSetup_EditorDraw(void);
void HCZSetup_EditorLoad(void);
#endif
void HCZSetup_Serialize(void);

// Extra Entity Functions
void HCZSetup_Scanline_WaterLine(ScanlineInfo *scanlines);
void HCZSetup_StageFinish_EndAct1(void);
void HCZSetup_StageFinish_EndAct2(void);

#endif //! OBJ_HCZSETUP_H
