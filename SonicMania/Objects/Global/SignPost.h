#ifndef OBJ_SIGNPOST_H
#define OBJ_SIGNPOST_H

#include "Game.h"

typedef enum {
    SIGNPOST_RUNPAST,
    SIGNPOST_DROP,
    SIGNPOST_COMP,
    SIGNPOST_DECOR,
} SignPostTypes;

typedef enum {
    SIGNPOSTANI_SONIC,
    SIGNPOSTANI_TAILS,
    SIGNPOSTANI_KNUX,
#if MANIA_USE_PLUS
    SIGNPOSTANI_MIGHTY,
    SIGNPOSTANI_RAY,
#endif
    SIGNPOSTANI_EGGMAN,
    SIGNPOSTANI_POST,
} SignPostAniIDs;

// Object Class
struct ObjectSignPost {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxSignPost;
    Hitbox hitboxItemBox;
    uint16 sfxSignPost;
    uint16 sfxSignPost2P;
    uint16 sfxTwinkle;
    uint16 sfxBubbleBounce;
    uint16 sfxSlide;
    uint8 maxPlayerCount;
};

// Entity Class
struct EntitySignPost {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    Vector2 vsBoundsSize;
    Vector2 vsBoundsOffset;
    int32 vsExtendTop;
    int32 vsExtendBottom;
    int32 unused1;
    int32 gravityStrength;
    int32 spinCount;
    int32 spinSpeed;
    int32 maxAngle;
    int32 sparkleType;
    int32 itemBounceCount;
    Vector2 playerPosStore[PLAYER_COUNT];
    Animator eggPlateAnimator;
    Animator facePlateAnimator;
    Animator sidebarAnimator;
    Animator postTopAnimator;
    Animator standAnimator;
    uint8 activePlayers;
    bool32 spawnedByDebugMode;
};

// Object Struct
extern ObjectSignPost *SignPost;

// Standard Entity Events
void SignPost_Update(void);
void SignPost_LateUpdate(void);
void SignPost_StaticUpdate(void);
void SignPost_Draw(void);
void SignPost_Create(void *data);
void SignPost_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SignPost_EditorDraw(void);
void SignPost_EditorLoad(void);
#endif
void SignPost_Serialize(void);

// Extra Entity Functions
void SignPost_DebugSpawn(void);
void SignPost_DebugDraw(void);

void SignPost_HandleSpin(void);
void SignPost_HandleSparkles(void);
void SignPost_HandleCamBounds(void);
void SignPost_CheckTouch(void);

// States
void SignPost_State_Init(void);
void SignPost_State_AwaitTouch(void);
void SignPost_State_Spin(void);
void SignPost_State_SpinVS(void);
void SignPost_State_Launched(void);
void SignPost_State_Falling(void);
void SignPost_State_Done(void);

#endif //! OBJ_SIGNPOST_H
