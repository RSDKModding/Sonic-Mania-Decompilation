#ifndef OBJ_FLINGRAMP_H
#define OBJ_FLINGRAMP_H

#include "Game.h"

// Object Class
struct ObjectFlingRamp {
    RSDK_OBJECT
    Hitbox hitboxRamp;
};

// Entity Class
struct EntityFlingRamp {
    RSDK_ENTITY
};

// Object Struct
extern ObjectFlingRamp *FlingRamp;

// Standard Entity Events
void FlingRamp_Update(void);
void FlingRamp_LateUpdate(void);
void FlingRamp_StaticUpdate(void);
void FlingRamp_Draw(void);
void FlingRamp_Create(void *data);
void FlingRamp_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FlingRamp_EditorDraw(void);
void FlingRamp_EditorLoad(void);
#endif
void FlingRamp_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_FLINGRAMP_H
