#ifndef OBJ_LOVETESTER_H
#define OBJ_LOVETESTER_H

#include "Game.h"

typedef enum {
    LOVETESTER_LIST_NONE,
    LOVETESTER_LIST_SONIC,
    LOVETESTER_LIST_TAILS,
    LOVETESTER_LIST_KNUX,
    LOVETESTER_LIST_EGGMAN,
    LOVETESTER_LIST_AMY,
    LOVETESTER_LIST_HEART,
    LOVETESTER_LIST_HEARTBROKEN,
} LoveTesterDisplays;

// Object Class
struct ObjectLoveTester {
    RSDK_OBJECT
    Hitbox hitboxEntry;
    Hitbox hitboxL;
    Hitbox hitboxR;
    uint16 aniFrames;
    uint16 sfxScore;
    Vector2 tvOffsetTop;
    Vector2 tvOffsetBottom;
    Vector2 lightOffset[10];
};

// Entity Class
struct EntityLoveTester {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateLights);
    uint8 activePlayers;
    Vector2 startPos;
    EntityPlayer *playerPtr;
    bool32 matchingFinished;
    bool32 isTVActiveTop;
    uint8 tvDisplayTop;
    uint8 tvFrameTop;
    bool32 isTVActiveBottom;
    uint8 tvDisplayBottom;
    uint8 tvFrameBottom;
    uint8 nextDisplayBottom;
    int32 timer;
    int32 lightsID;
    Animator mainAnimator;
    Animator lightAnimator[10];
};

// Object Struct
extern ObjectLoveTester *LoveTester;

// Standard Entity Events
void LoveTester_Update(void);
void LoveTester_LateUpdate(void);
void LoveTester_StaticUpdate(void);
void LoveTester_Draw(void);
void LoveTester_Create(void *data);
void LoveTester_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LoveTester_EditorDraw(void);
void LoveTester_EditorLoad(void);
#endif
void LoveTester_Serialize(void);

// Extra Entity Functions
void LoveTester_SetupHitboxes(void);
void LoveTester_SetupLightOffsets(void);
void LoveTester_DrawSprites(void);
void LoveTester_DrawTVDisplay(uint8 displayList, uint8 frame, bool32 isTVActive);
void LoveTester_CheckPlayerCollisions_Solid(void);
void LoveTester_CheckPlayerCollisions_Entry(bool32 allowSidekick);
void LoveTester_GiveScore(EntityPlayer *player);
void LoveTester_CreateHeartParticles(void);

void LoveTester_State_Init(void);
void LoveTester_State_WaitForActivated(void);
void LoveTester_State_SetupTopDisplay(void);
void LoveTester_State_SetupMatching(void);
void LoveTester_State_UnluckyMatch(void);
void LoveTester_State_GoodMatch(void);
void LoveTester_State_BadMatch(void);
void LoveTester_State_ReleasePlayers(void);

void LoveTester_State_HeartParticles(void);

void LoveTester_StateLights_FlashSlow(void);
void LoveTester_StateLights_FlashMed(void);
void LoveTester_StateLights_FlashFast(void);

#endif //! OBJ_LOVETESTER_H
