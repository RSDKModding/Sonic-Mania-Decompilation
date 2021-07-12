#ifndef OBJ_ZONE_H
#define OBJ_ZONE_H

#include "../SonicMania.h"

#define Zone_AddCallback(callback)                                                                                                                   \
    if (Zone->callbackCount < 0x10) {                                                                                                                  \
        Zone->callbacks[Zone->callbackCount] = ZipLine_Unknown1;                                                                                     \
        ++Zone->callbackCount;                                                                                                                       \
    }

// Object Class
typedef struct {
    RSDK_OBJECT
    int actID;
    void (*stageFinishCallback)(void);
    bool32 forcePlayerOnScreenFlag;
    void (*callbacks[0x10])(void);
    int callbackCount;
    int playerFlags[4];
    byte playerID;
    byte playerCount;
    byte playerIDs[4];
    byte playerIDs2[4];
    ushort field_6E;
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
    bool32 field_158;
    int field_15C;
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
#endif
    int flag;
    int randKey;
} ObjectZone;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int screenID;
    int timer;
    int fadeSpeed;
    int fadeColour;
} EntityZone;

// Object Struct
extern ObjectZone *Zone;

// Standard Entity Events
void Zone_Update(void);
void Zone_LateUpdate(void);
void Zone_StaticUpdate(void);
void Zone_Draw(void);
void Zone_Create(void* data);
void Zone_StageLoad(void);
void Zone_EditorDraw(void);
void Zone_EditorLoad(void);
void Zone_Serialize(void);

// Extra Entity Functions
int Zone_GetZoneID(void);
void Zone_StoreEntities(int xOffset, int yOffset);
void Zone_ReloadStoredEntities(int xOffset, int yOffset, bool32 flag);
void Zone_StartFadeOut(int fadeSpeed, int fadeColour);
void Zone_Unknown2(void);
void Zone_Unknown3(Entity *entity, Vector2 *pos, int angle);
void Zone_Unknown4(int screen);
void Zone_StartTeleportAction(void);
void Zone_ApplyWorldBounds(void);
bool32 Zone_IsAct2(void);
int Zone_GetEncoreStageID(void);
int Zone_GetManiaStageID(void);
void Zone_StateDraw_Fadeout(void);
void Zone_State_Fadeout(void);
void Zone_State_Fadeout_Restart(void);
void Zone_State_Fadeout_Unknown(void);
void Zone_Unknown16(void);
void Zone_Unknown17(void);
void Zone_State_Fadeout_Destroy(void);
void Zone_Unknown19(void);
void Zone_Unknown20(void);
void Zone_Unknown21(void);

#endif //!OBJ_ZONE_H
