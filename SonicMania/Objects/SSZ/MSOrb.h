#ifndef OBJ_MSORB_H
#define OBJ_MSORB_H

#include "Game.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxOrb;
} ObjectMSOrb;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Animator animator;
    int32 unused1;
    int32 unused2;
} EntityMSOrb;

// Object Struct
extern ObjectMSOrb *MSOrb;

// Standard Entity Events
void MSOrb_Update(void);
void MSOrb_LateUpdate(void);
void MSOrb_StaticUpdate(void);
void MSOrb_Draw(void);
void MSOrb_Create(void *data);
void MSOrb_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MSOrb_EditorDraw(void);
void MSOrb_EditorLoad(void);
#endif
void MSOrb_Serialize(void);

// Extra Entity Functions
void MSOrb_CheckPlayerCollisions(void);
void MSOrb_State_Orb(void);
void MSOrb_Draw_Orb(void);

#endif //! OBJ_MSORB_H
