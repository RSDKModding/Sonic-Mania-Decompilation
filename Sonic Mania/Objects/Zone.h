#ifndef OBJ_ZONE_H
#define OBJ_ZONE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int actID;
    void (*stageFinishCallback)(void);
    bool32 forcePlayerOnScreenFlag;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54[4];
    byte field_64;
    char field_65;
    char field_66;
    char field_67;
    int field_68;
    int field_6C;
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
    int field_158;
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
    byte playerDrawHigh;
    byte drawOrderLow;
    byte drawOrderHigh;
    byte playerDrawLow;
    byte uiDrawLow;
    byte uiDrawHigh;
    byte field_17B;
    ushort sfx_fail;
    ushort field_17E;
#if RETRO_USE_PLUS
    byte entityData[16][ENTITY_SIZE];
    byte field_4700;
    byte field_4701;
    byte field_4702;
    byte field_4703;
    byte field_4704;
    byte field_4705;
    byte field_4706;
    byte field_4707;
    byte field_4708;
    byte field_4709;
    byte field_470A;
    byte field_470B;
    byte field_470C;
    byte field_470D;
    byte field_470E;
    byte field_470F;
    byte field_4710;
    byte field_4711;
    byte field_4712;
    byte field_4713;
    byte field_4714;
    byte field_4715;
    byte field_4716;
    byte field_4717;
    byte field_4718;
    byte field_4719;
    byte field_471A;
    byte field_471B;
    byte field_471C;
    byte field_471D;
    byte field_471E;
    byte field_471F;
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
void Zone_Unknown5(void);
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
