#ifndef OBJ_TURBINE_H
#define OBJ_TURBINE_H

#include "Game.h"

typedef enum { TURBINE_HANDLES, TURBINE_SPIKES, TURBINE_WALLDECOR } TurbineTypes;

// Object Class
struct ObjectTurbine {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxTurbine;
    Hitbox hitboxHandle;
    Hitbox hitboxSpikes;
    bool32 playingTurbineSfx;
};

// Entity Class
struct EntityTurbine {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 activePlayers;
    int32 playerAngles[PLAYER_COUNT];
    int32 playerTimers[PLAYER_COUNT];
    Animator animator;
};

// Object Struct
extern ObjectTurbine *Turbine;

// Standard Entity Events
void Turbine_Update(void);
void Turbine_LateUpdate(void);
void Turbine_StaticUpdate(void);
void Turbine_Draw(void);
void Turbine_Create(void *data);
void Turbine_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Turbine_EditorDraw(void);
void Turbine_EditorLoad(void);
#endif
void Turbine_Serialize(void);

// Extra Entity Functions
void Turbine_State_Handles(void);
void Turbine_State_Spikes(void);

#endif //! OBJ_TURBINE_H
