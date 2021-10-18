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
    void (*stageFinishCallback)(void);
    bool32 forcePlayerOnScreenFlag;
    void (*callbacks[0x10])(void);
#if RETRO_USE_PLUS
    int32 callbackCount;
    int32 playerFlags[4];
    uint8 playerID;
    uint8 playerCount;
    uint8 playerIDs[4];
    uint8 playerIDs2[4];
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
    int32 screenBoundsL1[PLAYER_MAX];
    int32 screenBoundsR1[PLAYER_MAX];
    int32 screenBoundsT1[PLAYER_MAX];
    int32 screenBoundsB1[PLAYER_MAX];
    int32 screenBoundsL2[PLAYER_MAX];
    int32 screenBoundsR2[PLAYER_MAX];
    int32 screenBoundsT2[PLAYER_MAX];
    int32 screenBoundsB2[PLAYER_MAX];
    int32 deathBoundary[PLAYER_MAX];
    int32 playerBoundActiveL[PLAYER_MAX];
    int32 playerBoundActiveR[PLAYER_MAX];
    int32 playerBoundActiveT[PLAYER_MAX];
    int32 playerBoundActiveB[PLAYER_MAX];
    int32 field_154;
    bool32 atlReloadFlag;
    bool32 field_15C;
    void (*timeOverState)(void);
    uint16 fgLayers;
    uint16 fgLow;
    uint16 fgHigh;
    uint16 moveLayer;
    uint16 scratchLayer;
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
    uint16 sfx_fail;
#if RETRO_USE_PLUS
    uint8 entityData[16][ENTITY_SIZE];
    int32 screenPosX[PLAYER_MAX];
    int32 screenPosY[PLAYER_MAX];
    bool32 swapGameMode;
    int32 flag;
    int32 randKey;
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
void Zone_EditorDraw(void);
void Zone_EditorLoad(void);
void Zone_Serialize(void);

// Extra Entity Functions
int32 Zone_GetZoneID(void);
void Zone_StoreEntities(int32 xOffset, int32 yOffset);
void Zone_ReloadStoredEntities(int32 xOffset, int32 yOffset, bool32 flag);
void Zone_StartFadeOut(int32 fadeSpeed, int32 fadeColour);
void Zone_StartFadeIn(int32 fadeSpeed, int32 fadeColour);
void Zone_Unknown2(void);
void Zone_Unknown3(Vector2 *posPtr, Vector2 *pos, int32 angle);
void Zone_Unknown4(int32 screen);
void Zone_StartTeleportAction(void);
void Zone_ApplyWorldBounds(void);
bool32 Zone_IsAct2(void);
int32 Zone_GetEncoreStageID(void);
int32 Zone_GetManiaStageID(void);
void Zone_StateDraw_Fadeout(void);
void Zone_State_Fadeout(void);
void Zone_State_FadeIn(void);
void Zone_State_Fadeout_Unknown(void);
void Zone_Unknown16(void);
void Zone_Unknown17(void);
void Zone_State_Fadeout_Destroy(void);
void Zone_Unknown19(void);
void Zone_Unknown20(void);
void Zone_Unknown21(void);

#endif //! OBJ_ZONE_H
