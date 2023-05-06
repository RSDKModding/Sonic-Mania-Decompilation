#ifndef OBJ_CONVEYORWHEEL_H
#define OBJ_CONVEYORWHEEL_H

#include "Game.h"

// Object Class
struct ObjectConveyorWheel {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
};

// Entity Class
struct EntityConveyorWheel {
    RSDK_ENTITY
    Animator animator;
};

// Object Struct
extern ObjectConveyorWheel *ConveyorWheel;

// Standard Entity Events
void ConveyorWheel_Update(void);
void ConveyorWheel_LateUpdate(void);
void ConveyorWheel_StaticUpdate(void);
void ConveyorWheel_Draw(void);
void ConveyorWheel_Create(void *data);
void ConveyorWheel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ConveyorWheel_EditorDraw(void);
void ConveyorWheel_EditorLoad(void);
#endif
void ConveyorWheel_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_CONVEYORWHEEL_H
