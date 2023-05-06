#ifndef OBJ_TAEMERALD_H
#define OBJ_TAEMERALD_H

#include "Game.h"

// Object Class
struct ObjectTAEmerald {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityTAEmerald {
    RSDK_ENTITY
    StateMachine(state);
    uint8 color;
    int32 timer;
    Vector2 originPos;
    Animator animator;
};

// Object Struct
extern ObjectTAEmerald *TAEmerald;

// Standard Entity Events
void TAEmerald_Update(void);
void TAEmerald_LateUpdate(void);
void TAEmerald_StaticUpdate(void);
void TAEmerald_Draw(void);
void TAEmerald_Create(void *data);
void TAEmerald_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TAEmerald_EditorDraw(void);
void TAEmerald_EditorLoad(void);
#endif
void TAEmerald_Serialize(void);

// Extra Entity Functions
void TAEmerald_State_Oscillate(void);
void TAEmerald_State_MoveCircle(void);

#endif //! OBJ_TAEMERALD_H
