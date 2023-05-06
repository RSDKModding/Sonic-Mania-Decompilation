#ifndef OBJ_LAVAFALL_H
#define OBJ_LAVAFALL_H

#include "Game.h"

// Object Class
struct ObjectLavaFall {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    uint8 lavaSfxTimer;
    bool32 playingLavaSfx;
    uint16 sfxLava;
};

// Entity Class
struct EntityLavaFall {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator;
    uint16 interval;
    uint16 intervalOffset;
    uint16 duration;
    int32 timer;
};

// Object Struct
extern ObjectLavaFall *LavaFall;

// Standard Entity Events
void LavaFall_Update(void);
void LavaFall_LateUpdate(void);
void LavaFall_StaticUpdate(void);
void LavaFall_Draw(void);
void LavaFall_Create(void *data);
void LavaFall_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LavaFall_EditorDraw(void);
void LavaFall_EditorLoad(void);
#endif
void LavaFall_Serialize(void);

// Extra Entity Functions
void LavaFall_State_Idle(void);
void LavaFall_State_LavaFall(void);
void LavaFall_State_Lava(void);

#endif //! OBJ_LAVAFALL_H
