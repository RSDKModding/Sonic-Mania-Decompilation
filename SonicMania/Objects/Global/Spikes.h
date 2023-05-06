#ifndef OBJ_SPIKES_H
#define OBJ_SPIKES_H

#include "Game.h"

typedef enum {
    SPIKES_UP,
    SPIKES_DOWN,
    SPIKES_LEFT,
    SPIKES_RIGHT,
} SpikeTypes;

typedef enum {
    SPIKES_MOVE_STATIC,
    SPIKES_MOVE_HIDDEN,
    SPIKES_MOVE_APPEAR,
    SPIKES_MOVE_SHOWN,
    SPIKES_MOVE_DISAPPEAR,
    SPIKES_MOVE_DISAPPEAR_FOREVER,
    SPIKES_MOVE_HIDDEN_FOREVER,
} SpikeMoveTypes;

// Object Class
struct ObjectSpikes {
    RSDK_OBJECT
    StateMachine(stateDraw);
    Animator verticalAnimator;
    Animator horizontalAnimator;
    uint16 aniFrames;
    int32 unused1; // both set on StageLoad but then never used
    int32 unused2; // both set on StageLoad but then never used
    uint16 sfxMove;
    uint16 sfxSpike;
};

// Entity Class
struct EntitySpikes {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    bool32 moving;
    uint8 count;
    uint8 stagger;
    int16 timer;
    int32 planeFilter;
    int32 stateMove;
    int32 moveOffset;
    Vector2 collisionOffset;
    int16 glintTimer;
    uint8 shatterTimer;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectSpikes *Spikes;

// Standard Entity Events
void Spikes_Update(void);
void Spikes_LateUpdate(void);
void Spikes_StaticUpdate(void);
void Spikes_Draw(void);
void Spikes_Create(void *data);
void Spikes_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Spikes_EditorDraw(void);
void Spikes_EditorLoad(void);
#endif
void Spikes_Serialize(void);

// Extra Entity Functions
void Spikes_Draw_Global(void);
void Spikes_Draw_Stage(void);

void Spikes_Shatter(int32 velX, int32 velY);
#if MANIA_USE_PLUS
void Spikes_CheckHit(EntityPlayer *player, int32 playerVelX, int32 playerVelY);
#endif

#endif //! OBJ_SPIKES_H
