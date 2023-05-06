#ifndef OBJ_CABLEWARP_H
#define OBJ_CABLEWARP_H

#include "Game.h"

typedef enum {
    CABLEWARP_ENTRY,
    CABLEWARP_EXIT,
    CABLEWARP_NODE,
    CABLEWARP_TRANSPORT,
} CableWarpTypes;

// Object Class
struct ObjectCableWarp {
    RSDK_OBJECT
    Hitbox hitboxPlug;
    Hitbox hitboxTransport;
    uint16 aniFrames;
    uint16 sfxCharge;
    uint16 sfxPulse;
    uint16 sfxLedgeBreak;
};

// Entity Class
struct EntityCableWarp {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 timer;
    EntityPlayer *player;
    uint16 slotID;
    uint16 nextSlot;
    Animator animator;
};

// Object Struct
extern ObjectCableWarp *CableWarp;

// Standard Entity Events
void CableWarp_Update(void);
void CableWarp_LateUpdate(void);
void CableWarp_StaticUpdate(void);
void CableWarp_Draw(void);
void CableWarp_Create(void *data);
void CableWarp_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CableWarp_EditorDraw(void);
void CableWarp_EditorLoad(void);
#endif
void CableWarp_Serialize(void);

// Extra Entity Functions
void CableWarp_State_CablePlug(void);
void CableWarp_State_CheckPlayerEntry(void);

void CableWarp_StateTransport_BeginEnter(void);
void CableWarp_StateTransport_Enter(void);
void CableWarp_StateTransport_MoveToNextNode(void);
void CableWarp_StateTransport_EndNode(void);
void CableWarp_StateTransport_Exit(void);

#endif //! OBJ_CABLEWARP_H
