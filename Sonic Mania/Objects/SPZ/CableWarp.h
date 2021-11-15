#ifndef OBJ_CABLEWARP_H
#define OBJ_CABLEWARP_H

#include "SonicMania.h"

typedef enum {
    CABLEWARP_CAP,
    CABLEWARP_ENTRY,
    CABLEWARP_NODE,
    CABLEWARP_TRANSPORT,
}CableWarpTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxCharge;
    uint16 sfxPulse;
    uint16 sfxLedgeBreak;
} ObjectCableWarp;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 timer;
    Entity *playerPtr;
    uint16 slotID;
    uint16 nextSlot;
    Animator animator;
} EntityCableWarp;

// Object Struct
extern ObjectCableWarp *CableWarp;

// Standard Entity Events
void CableWarp_Update(void);
void CableWarp_LateUpdate(void);
void CableWarp_StaticUpdate(void);
void CableWarp_Draw(void);
void CableWarp_Create(void* data);
void CableWarp_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void CableWarp_EditorDraw(void);
void CableWarp_EditorLoad(void);
#endif
void CableWarp_Serialize(void);

// Extra Entity Functions
void CableWarp_State0_Unknown(void);
void CableWarp_State1_Unknown(void);

void CableWarp_State3_Unknown1(void);
void CableWarp_State3_Unknown2(void);
void CableWarp_State3_Unknown3(void);
void CableWarp_State3_Unknown4(void);
void CableWarp_State3_Unknown5(void);

#endif //!OBJ_CABLEWARP_H
