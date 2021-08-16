#ifndef OBJ_ZONE_H
#define OBJ_ZONE_H

#include "ProperTest.h"

typedef struct {
    RSDK_OBJECT
    int actID;
    void (*stageFinishCallback)(void);
    bool32 forcePlayerOnScreenFlag;
    void (*callbacks[0x10])(void);
#if RETRO_USE_PLUS
    int callbackCount;
    int playerFlags[4];
    byte playerID;
    byte playerCount;
    byte playerIDs[4];
    byte playerIDs2[4];
#else
    int field_94;
    int callbackCount;
    int playerFlags;
#endif
    int listPos;
    int prevListPos;
    int ringFrame;
    int timer;
    int timer2;
    int screenBoundsL1[PLAYER_MAX];
    int screenBoundsR1[PLAYER_MAX];
    int screenBoundsT1[PLAYER_MAX];
    int screenBoundsB1[PLAYER_MAX];
    int screenBoundsL2[PLAYER_MAX];
    int screenBoundsR2[PLAYER_MAX];
    int screenBoundsT2[PLAYER_MAX];
    int screenBoundsB2[PLAYER_MAX];
    int deathBoundary[PLAYER_MAX];
    int playerBoundActiveL[PLAYER_MAX];
    int playerBoundActiveR[PLAYER_MAX];
    int playerBoundActiveT[PLAYER_MAX];
    int playerBoundActiveB[PLAYER_MAX];
    int field_154;
    bool32 atlReloadFlag;
    bool32 field_15C;
    void (*timeOverState)(void);
    ushort fgLayers;
    ushort fgLow;
    ushort fgHigh;
    ushort moveLayer;
    ushort scratchLayer;
    ushort fgLowID;
    ushort fgHighID;
    ushort moveID;
    byte fgLayerLow;
    byte fgLayerHigh;
    byte drawOrderLow;
    byte drawOrderHigh;
    byte playerDrawLow;
    byte playerDrawHigh;
    byte hudDrawOrder;
    ushort sfx_fail;
#if RETRO_USE_PLUS
    byte entityData[16][ENTITY_SIZE];
    int screenPosX[PLAYER_MAX];
    int screenPosY[PLAYER_MAX];
    bool32 swapGameMode;
    int flag;
    int randKey;
#endif
} ObjectZone;

#endif