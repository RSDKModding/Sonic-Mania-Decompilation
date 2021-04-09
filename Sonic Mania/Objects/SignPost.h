#ifndef OBJ_SIGNPOST_H
#define OBJ_SIGNPOST_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    Hitbox hitbox;
    Hitbox itemBoxHitbox;
    ushort sfx_SignPost;
    ushort sfx_SignPost2P;
    ushort sfx_Twinkle;
    ushort sfx_BubbleBounce;
    ushort sfx_Slide;
    byte maxPlayerCount;
} ObjectSignPost;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte type;
    ushort field_5D;
    char field_5F;
    Vector2 vsBoundsSize;
    Vector2 vsBoundsOffset;
    int vsExtendTop;
    int vsExtendBottom;
    int gap78;
    int gravityStrength;
    int spinCount;
    int spinSpeed;
    int maxAngle;
    int sparkleType;
    int itemBounceCount;
    Vector2 playerPosStore[4];
    Animator eggPlateAnim;
    Animator facePlateAnim;
    Animator sidebarData;
    Animator postTopData;
    Animator standData;
    byte activePlayers;
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
