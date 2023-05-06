#ifndef OBJ_JELLYGNITE_H
#define OBJ_JELLYGNITE_H

#include "Game.h"

typedef enum {
    JELLYGNITE_ANI_FLOATING,
    JELLYGNITE_ANI_ANGRY,
    JELLYGNITE_ANI_FLASHING,
} JellygniteAnimIDs;

// Object Class
struct ObjectJellygnite {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox hitboxUnused;
    uint16 aniFrames;
    uint16 sfxGrab;
    uint16 sfxElectrify;
};

// Entity Class
struct EntityJellygnite {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    uint8 timer;
    uint8 grabDelay;
    int32 frontTentacleAngle;
    int32 oscillateAngle;
    EntityPlayer *grabbedPlayer;
    uint8 shakeTimer;
    uint8 shakeCount;
    uint8 lastShakeFlags;
    uint8 prevAnimationID;
    Animator bodyAnimator;
    Animator frontTentacleAnimator;
    Animator backTentacleAnimator;
};

// Object Struct
extern ObjectJellygnite *Jellygnite;

// Standard Entity Events
void Jellygnite_Update(void);
void Jellygnite_LateUpdate(void);
void Jellygnite_StaticUpdate(void);
void Jellygnite_Draw(void);
void Jellygnite_Create(void *data);
void Jellygnite_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Jellygnite_EditorDraw(void);
void Jellygnite_EditorLoad(void);
#endif
void Jellygnite_Serialize(void);

// Extra Entity Functions
void Jellygnite_DebugSpawn(void);
void Jellygnite_DebugDraw(void);

void Jellygnite_SetupAnimations(uint8 animationID);

void Jellygnite_CheckPlayerCollisions(void);
void Jellygnite_HandlePlayerStruggle(void);
bool32 Jellygnite_CheckInWater(EntityPlayer *player);

void Jellygnite_DrawBackTentacle(void);
void Jellygnite_DrawFrontTentacle(void);

void Jellygnite_State_Init(void);
void Jellygnite_State_Swimming(void);
void Jellygnite_State_GrabbedPlayer(void);
void Jellygnite_State_Explode(void);

#endif //! OBJ_JELLYGNITE_H
