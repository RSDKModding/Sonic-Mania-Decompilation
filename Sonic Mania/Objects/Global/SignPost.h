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
    uint16 spriteIndex;
    Hitbox hitbox;
    Hitbox itemBoxHitbox;
    uint16 sfx_SignPost;
    uint16 sfx_SignPost2P;
    uint16 sfx_Twinkle;
    uint16 sfx_BubbleBounce;
    uint16 sfx_Slide;
    uint8 maxPlayerCount;
} ObjectSignPost;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint16 field_5D;
    char field_5F;
    Vector2 vsBoundsSize;
    Vector2 vsBoundsOffset;
    int32 vsExtendTop;
    int32 vsExtendBottom;
    int32 field_78;
    int32 gravityStrength;
    int32 spinCount;
    int32 spinSpeed;
    int32 maxAngle;
    int32 sparkleType;
    int32 itemBounceCount;
    Vector2 playerPosStore[4];
    Animator eggPlateAnim;
    Animator facePlateAnim;
    Animator sidebarData;
    Animator postTopData;
    Animator standData;
    uint8 activePlayers;
    bool32 debugObj;
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
void SignPost_EditorDraw(void);
void SignPost_EditorLoad(void);
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
