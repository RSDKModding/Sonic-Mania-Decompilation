#ifndef OBJ_RISINGLAVA_H
#define OBJ_RISINGLAVA_H

#include "Game.h"

// Object Class
struct ObjectRisingLava {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
    uint16 sfxRumble;
};

// Entity Class
struct EntityRisingLava {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 size;
    Vector2 offset;
    int32 limit;
    Hitbox hitbox;
};

// Object Struct
extern ObjectRisingLava *RisingLava;

// Standard Entity Events
void RisingLava_Update(void);
void RisingLava_LateUpdate(void);
void RisingLava_StaticUpdate(void);
void RisingLava_Draw(void);
void RisingLava_Create(void *data);
void RisingLava_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void RisingLava_EditorDraw(void);
void RisingLava_EditorLoad(void);
#endif
void RisingLava_Serialize(void);

// Extra Entity Functions
void RisingLava_State_CheckRiseStart(void);
void RisingLava_State_RiseShake(void);
void RisingLava_State_StoppedRising(void);

#endif //! OBJ_RISINGLAVA_H
