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
    void(*state)();
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
    AnimationData eggPlateAnim;
    AnimationData facePlateAnim;
    AnimationData sidebarData;
    AnimationData postTopData;
    AnimationData standData;
    byte activePlayers;
    bool32 debugObj;
} EntitySignPost;

// Object Struct
extern ObjectSignPost *SignPost;

// Standard Entity Events
void SignPost_Update();
void SignPost_LateUpdate();
void SignPost_StaticUpdate();
void SignPost_Draw();
void SignPost_Create(void* data);
void SignPost_StageLoad();
void SignPost_EditorDraw();
void SignPost_EditorLoad();
void SignPost_Serialize();

// Extra Entity Functions
void SignPost_DebugSpawn();
void SignPost_DebugDraw();

void SignPost_SpinSpeed();
void SignPost_SpawnSparkle();
void SignPost_State_SetupCompetition();
void SignPost_State_Competition();
void SignPost_State_Land();
void SignPost_State_CompetitionFinish();
void SignPost_State_Launched();
void SignPost_State_Fall();
void SignPost_State_Finish();
void SignPost_CheckTouch();
void SignPost_HandleCompetition();

#endif //!OBJ_SIGNPOST_H
