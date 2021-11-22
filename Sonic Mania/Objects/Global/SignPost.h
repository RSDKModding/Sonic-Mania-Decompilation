#ifndef OBJ_SIGNPOST_H
#define OBJ_SIGNPOST_H

#include "SonicMania.h"

typedef enum {
    SIGNPOSTANI_SONIC,
    SIGNPOSTANI_TAILS,
    SIGNPOSTANI_KNUX,
#if RETRO_USE_PLUS
    SIGNPOSTANI_MIGHTY,
    SIGNPOSTANI_RAY,
#endif
    SIGNPOSTANI_EGGMAN,
    SIGNPOSTANI_POST,
}SignPostAniIDs;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    Hitbox itemBoxHitbox;
    uint16 sfxSignPost;
    uint16 sfxSignPost2P;
    uint16 sfxTwinkle;
    uint16 sfxBubbleBounce;
    uint16 sfxSlide;
    uint8 maxPlayerCount;
} ObjectSignPost;

// Entity Class
typedef struct {
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
    Vector2 playerPosStore[PLAYER_MAX];
    Animator eggPlateAnimator;
    Animator facePlateAnimator;
    Animator sidebarAnimator;
    Animator postTopAnimator;
    Animator standAnimator;
    uint8 activePlayers;
    bool32 spawnedByDebugMode;
} EntitySignPost;

// Object Struct
extern ObjectSignPost *SignPost;

// Standard Entity Events
void SignPost_Update(void);
void SignPost_LateUpdate(void);
void SignPost_StaticUpdate(void);
void SignPost_Draw(void);
void SignPost_Create(void* data);
void SignPost_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SignPost_EditorDraw(void);
void SignPost_EditorLoad(void);
#endif
void SignPost_Serialize(void);

// Extra Entity Functions
void SignPost_DebugSpawn(void);
void SignPost_DebugDraw(void);

void SignPost_SpinSpeed(void);
void SignPost_SpawnSparkle(void);
void SignPost_State_SetupCompetition(void);
void SignPost_State_Competition(void);
void SignPost_State_Land(void);
void SignPost_State_CompetitionFinish(void);
void SignPost_State_Launched(void);
void SignPost_State_Fall(void);
void SignPost_State_Finish(void);
void SignPost_CheckTouch(void);
void SignPost_HandleCompetition(void);

#endif //!OBJ_SIGNPOST_H
