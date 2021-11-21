#ifndef OBJ_ZONE_H
#define OBJ_ZONE_H

#include "SonicMania.h"

#define Zone_AddCallback(callback)                                                                                                                   \
    if (Zone->callbackCount < 0x10) {                                                                                                                \
        Zone->callbacks[Zone->callbackCount] = callback;                                                                                             \
        ++Zone->callbackCount;                                                                                                                       \
    }

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 actID;
    StateMachine(stageFinishCallback);
    bool32 forcePlayerOnScreenFlag;
    StateMachine(callbacks[0x10]);
#if RETRO_USE_PLUS
    int32 callbackCount;
    int32 playerFlags[PLAYER_MAX];
    uint8 playerID;
    uint8 playerCount;
    uint8 playerIDs[PLAYER_MAX];
    uint8 playerIDs2[PLAYER_MAX];
#else
    int32 field_94;
    int32 callbackCount;
    int32 playerFlags;
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
    bool32 atlReloadFlag;
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
    uint8 drawOrderLow;
    uint8 drawOrderHigh;
    uint8 playerDrawLow;
    uint8 playerDrawHigh;
    uint8 hudDrawOrder;
    uint16 sfxfail;
#if RETRO_USE_PLUS
    uint8 entityData[16][ENTITY_SIZE];
    int32 screenPosX[PLAYER_MAX];
    int32 screenPosY[PLAYER_MAX];
    bool32 swapGameMode;
    bool32 teleportActionActive;
    int32 randSeed;
#endif
} ObjectZone;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 screenID;
    int32 timer;
    int32 fadeSpeed;
    int32 fadeColour;
} EntityZone;

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
void Zone_ReloadStoredEntities(int32 xOffset, int32 yOffset, bool32 setCamera);
void Zone_StartFadeOut(int32 fadeSpeed, int32 fadeColour);
void Zone_StartFadeIn(int32 fadeSpeed, int32 fadeColour);
void Zone_StartFadeOut_MusicFade(void);
void Zone_RotateOnPivot(Vector2 *position, Vector2 *pivotPos, int32 angle);
void Zone_ReloadScene(int32 screen);
void Zone_StartTeleportAction(void);
void Zone_ApplyWorldBounds(void);
bool32 Zone_IsAct2(void);
int32 Zone_GetEncoreStageID(void);
int32 Zone_GetManiaStageID(void);
void Zone_Draw_Fade(void);
void Zone_State_Fadeout(void);
void Zone_State_FadeIn(void);
void Zone_State_Fadeout_Unknown(void);
void Zone_TitleCard_SupressCB(void);
void Zone_State_ReloadScene(void);
void Zone_State_Fadeout_Destroy(void);
void Zone_HandlePlayerSwap(void);
void Zone_State_SwapPlayers(void);
void Zone_State_HandleSwapFadeIn(void);

#endif //! OBJ_ZONE_H
