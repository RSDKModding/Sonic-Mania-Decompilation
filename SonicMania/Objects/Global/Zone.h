#ifndef OBJ_ZONE_H
#define OBJ_ZONE_H

#include "Game.h"

// Macro to reduce ifdefs needed when calling RandSeeded with Zone->randSeed, ensures easy pre-plus compatibility
#if MANIA_USE_PLUS
#define ZONE_RAND(min, max) RSDK.RandSeeded(min, max, &Zone->randSeed)
#else
#define ZONE_RAND(min, max) RSDK.Rand(min, max)
#endif

#define Zone_AddVSSwapCallback(callback)                                                                                                             \
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
#if MANIA_USE_PLUS
    ZONE_AIZ,
#endif
    // total zone count
    ZONE_COUNT,
    // zone count for save files
    ZONE_COUNT_SAVEFILE = ZONE_ERZ + 1,
} ZoneIDs;

// Object Class
struct ObjectZone {
    RSDK_OBJECT
    int32 actID;
    StateMachine(stageFinishCallback);
    bool32 shouldRecoverPlayers; // a little misleading, forces the player on-screen before an act transition if enabled
    StateMachine(vsSwapCB[0x10]);
    int32 vsSwapCBCount;
#if MANIA_USE_PLUS
    int32 playerSwapEnabled[PLAYER_COUNT];
    uint8 swapPlayerID;
    uint8 swapPlayerCount;
    uint8 preSwapPlayerIDs[PLAYER_COUNT];
    uint8 swappedPlayerIDs[PLAYER_COUNT];
#else
    bool32 playerSwapEnabled;
#endif
    int32 listPos;
    int32 prevListPos;
    int32 ringFrame;
    int32 timer;
    int32 persistentTimer;
    int32 cameraBoundsL[PLAYER_COUNT];
    int32 cameraBoundsR[PLAYER_COUNT];
    int32 cameraBoundsT[PLAYER_COUNT];
    int32 cameraBoundsB[PLAYER_COUNT];
    int32 playerBoundsL[PLAYER_COUNT];
    int32 playerBoundsR[PLAYER_COUNT];
    int32 playerBoundsT[PLAYER_COUNT];
    int32 playerBoundsB[PLAYER_COUNT];
    int32 deathBoundary[PLAYER_COUNT];
    int32 playerBoundActiveL[PLAYER_COUNT];
    int32 playerBoundActiveR[PLAYER_COUNT];
    int32 playerBoundActiveT[PLAYER_COUNT];
    int32 playerBoundActiveB[PLAYER_COUNT];
    int32 autoScrollSpeed;
    bool32 setATLBounds;
    bool32 gotTimeOver;
    StateMachine(timeOverCallback);
    uint16 collisionLayers;
    uint16 fgLayer[2]; // { lowPriority, highPriority }
    uint16 moveLayer;
#if MANIA_USE_PLUS
    uint16 scratchLayer;
#endif
    uint16 fgLayerMask[2]; // { lowPriority, highPriority }
    uint16 moveLayerMask;
    uint8 fgDrawGroup[2];     // { lowPriority, highPriority }
    uint8 objectDrawGroup[2]; // { lowPriority, highPriority }
    uint8 playerDrawGroup[2]; // { lowPriority, highPriority }
    uint8 hudDrawGroup;
    uint16 sfxFail;
#if MANIA_USE_PLUS
    uint8 entityStorage[16][ENTITY_SIZE];
    int32 screenPosX[PLAYER_COUNT];
    int32 screenPosY[PLAYER_COUNT];
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
#if GAME_INCLUDE_EDITOR
void Zone_EditorDraw(void);
void Zone_EditorLoad(void);
#endif
void Zone_Serialize(void);

// Extra Entity Functions
int32 Zone_GetZoneID(void);
// Stores entities (in preparation for an act transition)
void Zone_StoreEntities(int32 xOffset, int32 yOffset);
// Reloads stored entities (used after an act transition)
void Zone_ReloadStoredEntities(int32 xOffset, int32 yOffset, bool32 setATLBounds);
// Fades out & loads the active stage
void Zone_StartFadeOut(int32 fadeSpeed, int32 fadeColor);
// Fades in
void Zone_StartFadeIn(int32 fadeSpeed, int32 fadeColor);
// Does the same as Zone_StartFadeOut, but also fades out the music
void Zone_StartFadeOut_MusicFade(int32 fadeSpeed, int32 fadeColor);
// Basically the same as Zone_StartFadeOut, but used for competition stages
void Zone_StartFadeOut_Competition(int32 fadeSpeed, int32 fadeColor);
// rotates 'pivotPos' around 'originPos'
void Zone_RotateOnPivot(Vector2 *pivotPos, Vector2 *originPos, int32 angle);
void Zone_ReloadScene(int32 screen);
void Zone_StartTeleportAction(void);
void Zone_HandlePlayerBounds(void);
void Zone_ApplyWorldBounds(void);

bool32 Zone_IsZoneLastAct(void);
#if MANIA_USE_PLUS
int32 Zone_GetListPos_EncoreMode(void);
int32 Zone_GetListPos_ManiaMode(void);
#endif

// Draw States
void Zone_Draw_Fade(void);

// States & Stuff
void Zone_State_FadeOut(void);
void Zone_State_FadeIn(void);
void Zone_State_FadeOut_Competition(void);
#if MANIA_USE_PLUS
void Zone_TitleCard_SupressCB(void);
void Zone_State_ReloadScene(void);
#endif
void Zone_State_FadeOut_Destroy(void);
void Zone_HandlePlayerSwap(void);
void Zone_State_SwapPlayers(void);
void Zone_State_HandleSwapFadeIn(void);

#endif //! OBJ_ZONE_H
