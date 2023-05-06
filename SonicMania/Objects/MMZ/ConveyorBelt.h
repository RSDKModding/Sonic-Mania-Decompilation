#ifndef OBJ_CONVEYORBELT_H
#define OBJ_CONVEYORBELT_H

#include "Game.h"

// Object Class
struct ObjectConveyorBelt {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityConveyorBelt {
    RSDK_ENTITY
    Vector2 size;
    int32 speed;
    Animator animator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectConveyorBelt *ConveyorBelt;

// Standard Entity Events
void ConveyorBelt_Update(void);
void ConveyorBelt_LateUpdate(void);
void ConveyorBelt_StaticUpdate(void);
void ConveyorBelt_Draw(void);
void ConveyorBelt_Create(void *data);
void ConveyorBelt_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ConveyorBelt_EditorDraw(void);
void ConveyorBelt_EditorLoad(void);
#endif
void ConveyorBelt_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_CONVEYORBELT_H
