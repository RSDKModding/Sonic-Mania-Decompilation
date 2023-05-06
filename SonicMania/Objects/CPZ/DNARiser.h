#ifndef OBJ_DNARISER_H
#define OBJ_DNARISER_H

#include "Game.h"

// Object Class
struct ObjectDNARiser {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxGrab;
    uint16 sfxScan;
    uint16 sfxBurst;
    uint16 sfxTiny[6];
    Hitbox hitbox;
};

// Entity Class
struct EntityDNARiser {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    bool32 noBubbleScaling;
    bool32 popped;
    Vector2 popPos;
    int32 bubbleScale;
    Animator bubbleAnimator;
    Animator popAnimator;
    uint16 startHelixPos;
    int16 timer;
    uint8 scaleTimer;
    bool32 canDestroySelf;
    int32 scanSfxDir;
    uint16 sfxTimer;
    uint8 lastSfxID;
    int32 height;
    Vector2 speed;
    uint8 activePlayers;
    int32 risePos;
    int32 amplitude;
    Vector2 startPos;
    Vector2 maxSpeed;
    uint16 helixPos;
    EntityDNARiser *firstChild;
    EntityDNARiser *child;
    EntityDNARiser *sibling;
    EntityDNARiser *lastChild;
    EntityDNARiser *parent;
};

// Object Struct
extern ObjectDNARiser *DNARiser;

// Standard Entity Events
void DNARiser_Update(void);
void DNARiser_LateUpdate(void);
void DNARiser_StaticUpdate(void);
void DNARiser_Draw(void);
void DNARiser_Create(void *data);
void DNARiser_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DNARiser_EditorDraw(void);
void DNARiser_EditorLoad(void);
#endif
void DNARiser_Serialize(void);

// Extra Entity Functions
void DNARiser_State_BubbleBurst(void);
void DNARiser_SetupBurst(void);
Vector2 DNARiser_CalculateScale(Vector2 *scalePtr);

void DNARiser_State_Init(void);
void DNARiser_State_HandleInteractions(void);
void DNARiser_State_HelixRise(void);
void DNARiser_State_HelixBurst(void);
void DNARiser_State_ResetRiser(void);
void DNARiser_State_OrbSetup(void);
void DNARiser_State_OrbIdle(void);
void DNARiser_State_OrbFall(void);

void DNARiser_Draw_Main(void);
void DNARiser_Draw_Helix(void);

#endif //! OBJ_DNARISER_H
