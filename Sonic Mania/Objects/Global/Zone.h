#ifndef OBJ_ZONE_H
#define OBJ_ZONE_H

#include "SonicMania.h"

#define Zone_AddVSSwapCallback(callback)                                                                                                                   \
    if (Zone->vsSwapCBCount < 0x10) {                                                                                                                \
        Zone->vsSwapCB[Zone->vsSwapCBCount] = callback;                                                                                              \
        ++Zone->vsSwapCBCount;                                                                                                                       \
    }

typedef enum {
    ZONE_INVALID = -1,
    ZONE_GHZ,
    ZONE_CPZ,
    ZONE_SPZ,
    ZONE_FBZ,
    ZONE_PGZ,
    ZONE_SSZ,
    ZONE_HCZ,
    ZONE_MSZ,
    ZONE_OOZ,
    ZONE_LRZ,
    ZONE_MMZ,
    ZONE_TMZ,
    ZONE_ERZ,
#if RETRO_USE_PLUS
    ZONE_AIZ,
#endif
    ZONE_COUNT,
}ZoneIDs;

// Object Class
struct ObjectZone {
    RSDK_OBJECT
    int32 actID;
    StateMachine(stageFinishCallback);
    bool32 forcePlayerOnScreen; // a little misleading, forces the player on-screen before an act transition if enabled
    StateMachine(vsSwapCB[0x10]);
    int32 vsSwapCBCount;
#if RETRO_USE_PLUS
    int32 playerSwapEnabled[PLAYER_MAX];
    uint8 swapPlayerID;
    uint8 swapPlayerCount;
    uint8 preSwapPlayerIDs[PLAYER_MAX];
    uint8 swappedPlayerIDs[PLAYER_MAX];
#else
    bool32 playerSwapEnabled;
#endif
    int32 listPos;
    int32 prevListPos;
    int32 ringFrame;
    int32 timer;
    int32 timer2;
    int32 cameraBoundsL[PLAYER_MAX];
    int32 cameraBoundsR[PLAYER_MAX];
    int32 cameraBoundsT[PLAYER_MAX];
    int32 cameraBoundsB[PLAYER_MAX];
    int32 playerBoundsL[PLAYER_MAX];
    int32 playerBoundsR[PLAYER_MAX];
    int32 playerBoundsT[PLAYER_MAX];
    int32 playerBoundsB[PLAYER_MAX];
    int32 deathBoundary[PLAYER_MAX];
    int32 playerBoundActiveL[PLAYER_MAX];
    int32 playerBoundActiveR[PLAYER_MAX];
    int32 playerBoundActiveT[PLAYER_MAX];
    int32 playerBoundActiveB[PLAYER_MAX];
    int32 autoScrollSpeed;
    bool32 setATLBounds;
    bool32 gotTimeOver;
    StateMachine(timeOverCallback);
    uint16 fgLayers;
    uint16 fgLow;
    uint16 fgHigh;
    uint16 moveLayer;
#if RETRO_USE_PLUS
    uint16 scratchLayer;
#endif
    uint16 fgLowID;
    uint16 fgHighID;
    uint16 moveID;
    uint8 fgLayerLow;
    uint8 fgLayerHigh;
    uint8 objectDrawLow;
    uint8 objectDrawHigh;
    uint8 playerDrawLow;
    uint8 playerDrawHigh;
    uint8 hudDrawOrder;
    uint16 sfxFail;
#if RETRO_USE_PLUS
    uint8 entityData[16][ENTITY_SIZE];
    int32 screenPosX[PLAYER_MAX];
    int32 screenPosY[PLAYER_MAX];
    bool32 swapGameMode;
    bool32 teleportActionActive;
    int32 randSeed;
#endif
};

// Entity Class
struct EntityZone {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 screenID;
    int32 timer;
    int32 fadeSpeed;
    int32 fadeColor;
};

// Object Struct
extern ObjectZone *Zone;

// Standard Entity Events
void Zone_Update(void);
void Zone_LateUpdate(void);
void Zone_StaticUpdate(void);
void Zone_Draw(void);
void Zone_Create(void *data);
void Zone_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Zone_EditorDraw(void);
void Zone_EditorLoad(void);
#endif
void Zone_Serialize(void);

// Extra Entity Functions
int32 Zone_GetZoneID(void);
void Zone_StoreEntities(int32 xOffset, int32 yOffset);
void Zone_ReloadStoredEntities(int32 xOffset, int32 yOffset, bool32 setATLBounds);
void Zone_StartFadeOut(int32 fadeSpeed, int32 fadeColor);
void Zone_StartFadeIn(int32 fadeSpeed, int32 fadeColor);
void Zone_StartFadeOut_MusicFade(int32 fadeSpeed, int32 fadeColor);
void Zone_StartFadeOut_Competition(int32 fadeSpeed, int32 fadeColor);
void Zone_RotateOnPivot(Vector2 *position, Vector2 *pivot, int32 angle);
void Zone_ReloadScene(int32 screen);
void Zone_StartTeleportAction(void);
void Zone_ApplyWorldBounds(void);

bool32 Zone_IsZoneLastAct(void);
int32 Zone_GetEncoreStageID(void);
int32 Zone_GetManiaStageID(void);

// Draw States
void Zone_Draw_Fade(void);

// States & Stuff
void Zone_State_Fadeout(void);
void Zone_State_FadeIn(void);
void Zone_State_Fadeout_Competition(void);
void Zone_TitleCard_SupressCB(void);
void Zone_State_ReloadScene(void);
void Zone_State_Fadeout_Destroy(void);
void Zone_HandlePlayerSwap(void);
void Zone_State_SwapPlayers(void);
void Zone_State_HandleSwapFadeIn(void);

#endif //! OBJ_ZONE_H
