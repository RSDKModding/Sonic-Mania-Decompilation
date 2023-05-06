#ifndef OBJ_AMOEBADROID_H
#define OBJ_AMOEBADROID_H

#include "Game.h"

#define AMOEBADROID_BLOB_COUNT (8)

typedef enum {
    AMOEBADROID_BOSS,
    AMOEBADROID_BLOB_BIG,
    AMOEBADROID_BLOB_SMALL,
    AMOEBADROID_POOLSPLASH,
    AMOEBADROID_POOLSPLASH_DELAY,
} AmoebaDroidTypes;

// Object Class
struct ObjectAmoebaDroid {
    RSDK_OBJECT
    TABLE(int32 debrisInfo1[17], { 4, 0, 0, -0x20000, -0x28000, 1, 0, 0x20000, -0x28000, 2, 0, -0x28000, -0x20000, 3, 0, 0x28000, -0x20000 });
    TABLE(int32 debrisInfo2[13], { 3, 4, 0, 0, -0x20000, 5, 0, -0x28000, -0x20000, 5, 1, 0x28000, -0x20000 });
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxGather;
    uint16 sfxBounce;
    uint16 sfxRelease;
    uint16 aniFrames;
    uint16 waterFrames;
};

// Entity Class
struct EntityAmoebaDroid {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 invincibleTimer;
    int32 health;
    int32 blobAngleX;
    int32 blobAngleY;
    int32 blobAmplitude;
    int32 blobRadius;
    Vector2 offsetPos;
    EntityAmoebaDroid *blobs[AMOEBADROID_BLOB_COUNT];
    EntityAmoebaDroid *parent;
    Animator animator;
    Animator attractorTopAnimator;
    Animator attractorSideAnimator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectAmoebaDroid *AmoebaDroid;

// Standard Entity Events
void AmoebaDroid_Update(void);
void AmoebaDroid_LateUpdate(void);
void AmoebaDroid_StaticUpdate(void);
void AmoebaDroid_Draw(void);
void AmoebaDroid_Create(void *data);
void AmoebaDroid_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void AmoebaDroid_EditorDraw(void);
void AmoebaDroid_EditorLoad(void);
#endif
void AmoebaDroid_Serialize(void);

// Extra Entity Functions
void AmoebaDroid_HandleSmallBlobMovement(void);
void AmoebaDroid_HandleSmallBlobRelease(bool32 interact);

// Interaction funcs
void AmoebaDroid_Hit(void);
void AmoebaDroid_Explode(void);
void AmoebaDroid_CheckHit(void);
void AmoebaDroid_CheckPlayerHit(void);

// Draw States
void AmoebaDroid_Draw_AmoebaDroid(void);
void AmoebaDroid_Draw_BigBlob(void);

// Boss States
void AmoebaDroid_State_SetupArena(void);
void AmoebaDroid_State_SetupWaterLevel(void);
void AmoebaDroid_State_DropIn(void);
void AmoebaDroid_State_DropIntoPool(void);
void AmoebaDroid_State_SurfaceFromPool(void);
void AmoebaDroid_State_ChooseAttack(void);
void AmoebaDroid_State_SwimLeft(void);
void AmoebaDroid_State_SwimRight(void);
void AmoebaDroid_State_ExitPool(void);
void AmoebaDroid_State_BounceAttack(void);
void AmoebaDroid_State_GatherBlobs(void);
void AmoebaDroid_State_SpinBlobs(void);

// Blob States
void AmoebaDroid_State_BigBlob(void);
void AmoebaDroid_State_SmallBlob(void);
void AmoebaDroid_State_BigBlob_Disappear(void);

// Splash States
void AmoebaDroid_State_PoolSplash_Delayed(void);
void AmoebaDroid_State_PoolSplash(void);

// Boss States (Continued)
void AmoebaDroid_State_Destroyed(void);
void AmoebaDroid_State_DropSignPost(void);

#endif //! OBJ_AMOEBADROID_H
