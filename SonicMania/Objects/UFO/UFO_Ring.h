#ifndef OBJ_UFO_RING_H
#define OBJ_UFO_RING_H

#include "Game.h"

// Object Class
struct ObjectUFO_Ring {
    RSDK_OBJECT
    int32 unused1;
    int32 unused2;
    uint16 aniFrames;
    uint16 sfxRing;
};

// Entity Class
struct EntityUFO_Ring {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 height;
    int32 bounceVelocity;
    int32 worldX;
    int32 worldY;
    Animator animator;
};

// Object Struct
extern ObjectUFO_Ring *UFO_Ring;

// Standard Entity Events
void UFO_Ring_Update(void);
void UFO_Ring_LateUpdate(void);
void UFO_Ring_StaticUpdate(void);
void UFO_Ring_Draw(void);
void UFO_Ring_Create(void *data);
void UFO_Ring_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Ring_EditorDraw(void);
void UFO_Ring_EditorLoad(void);
#endif
void UFO_Ring_Serialize(void);

// Extra Entity Functions
void UFO_Ring_PlayRingSfx(void);
void UFO_Ring_LoseRings(EntityUFO_Player *player);
// States
void UFO_Ring_State_NormalRing(void);
void UFO_Ring_State_Sparkle(void);
void UFO_Ring_State_LoseRing(void);

#endif //! OBJ_UFO_RING_H
