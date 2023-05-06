#ifndef OBJ_WATER_H
#define OBJ_WATER_H

#include "Game.h"

typedef enum {
    WATER_WATERLEVEL,
    WATER_POOL,
    WATER_BUBBLER,
    WATER_HEIGHT_TRIGGER,
    WATER_BIG_BUBBLER,
    WATER_BTN_BIG_BUBBLE,
    WATER_SPLASH,
    WATER_BUBBLE,
    WATER_COUNTDOWN,
} WaterTypes;

typedef enum {
    WATER_PRIORITY_LOWEST,
    WATER_PRIORITY_LOW,
    WATER_PRIORITY_HIGH,
    WATER_PRIORITY_HIGHEST,
} WaterPriorities;

// Object Class
struct ObjectWater {
    RSDK_OBJECT
    int32 waterLevel;
    int32 newWaterLevel;
    int32 targetWaterLevel;
    int32 waterMoveSpeed;
    int32 constBubbleTimer[PLAYER_COUNT];
    int32 randBubbleTimer[PLAYER_COUNT];
    int32 unused1[PLAYER_COUNT]; // unused but set to 0 on bubble timer sets. maybe an old bubble/drowning timer?
    TABLE(int32 bubbleSizes[18], { 2, 4, 2, 2, 2, 2, 4, 2, 4, 2, 2, 4, 2, 4, 2, 2, 4, 2 });
    uint16 aniFrames;
    uint16 bigBubbleFrames;
    uint16 wakeFrames;
    Hitbox hitboxPlayerBubble;
    Hitbox hitboxPoint;
    uint16 sfxSplash;
    uint16 sfxBreathe;
    uint16 sfxWarning;
    uint16 sfxDrownAlert;
    uint16 sfxDrown;
    uint16 sfxSkim;
    uint16 sfxDNAGrab;
    uint16 sfxDNABurst;
    uint16 sfxWaterLevelL;
    uint16 sfxWaterLevelR;
    int32 waterLevelChannelL;
    int32 waterLevelChannelR;
    int32 unused2;
    bool32 playingWaterLevelSfx;
    bool32 moveWaterLevel;
    int32 waterLevelVolume;
    int32 waterPalette;
    bool32 disableWaterSplash; // this is never set except for once and it's used in if ! statements to link the player or not
    int32 wakePosX[PLAYER_COUNT];
    uint8 wakeDir[PLAYER_COUNT];
    Animator wakeAnimator;
    int32 unused3;
    bool32 playingSkimSfx;
};

// Entity Class
struct EntityWater {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    void *childPtr;
    int32 bubbleX;
    int8 bubbleType1;
    uint8 dudsRemaining;
    uint8 bubbleFlags;
    int8 bubbleType2;
    int32 numDuds;
    int32 countdownID;
    bool32 playerInBubble;
    Vector2 size;
    Vector2 height;
    int32 speed;
    int32 buttonTag;
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 priority;
    bool32 destroyOnTrigger;
    Hitbox hitbox;
    bool32 isBigBubble;
    int32 bigBubbleTimer;
    int32 timer;
    uint8 activePlayers;
    uint8 releasedPlayers;
    Vector2 bubbleOffset;
    Vector2 bubbleVelocity;
    EntityButton *taggedButton;
    Animator animator;
};

// Object Struct
extern ObjectWater *Water;

// Standard Entity Events
void Water_Update(void);
void Water_LateUpdate(void);
void Water_StaticUpdate(void);
void Water_Draw(void);
void Water_Create(void *data);
void Water_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Water_EditorDraw(void);
void Water_EditorLoad(void);
#endif
void Water_Serialize(void);

// Extra Entity Functions

// Palette stuff
void Water_DrawHook_ApplyWaterPalette(void);
void Water_DrawHook_RemoveWaterPalette(void);

// Utils
void Water_SetupTagLink(void);
void Water_SpawnBubble(EntityPlayer *player, int32 id);
void Water_SpawnCountDownBubble(EntityPlayer *player, int32 id, uint8 bubbleID);
EntityWater *Water_GetPlayerBubble(EntityPlayer *entityPtr);
void Water_HandleBubbleMovement(void);
void Water_PopBigBubble(EntityWater *self, bool32 jumpedOut);

void Water_State_Water(void);
void Water_State_Pool(void);
void Water_State_Splash(void);
void Water_State_Bubble(void);
void Water_State_BubbleBreathed(void);
void Water_State_BigBubble(void);
void Water_State_BtnBigBubble(void);
void Water_State_Bubbler(void);
void Water_State_Countdown(void);
void Water_State_CountdownFollow(void);
void Water_State_HeightTrigger(void);

// Draw States
void Water_Draw_Water(void);
void Water_Draw_Pool(void);
void Water_Draw_Splash(void);
void Water_Draw_Countdown(void);
void Water_Draw_Bubbler(void);
void Water_Draw_Bubble(void);

#endif //! OBJ_WATER_H
