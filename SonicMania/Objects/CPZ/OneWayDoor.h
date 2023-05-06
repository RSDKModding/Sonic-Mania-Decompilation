#ifndef OBJ_ONEWAYDOOR_H
#define OBJ_ONEWAYDOOR_H

#include "Game.h"

// Object Class
struct ObjectOneWayDoor {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxTop;
    Hitbox hitboxBottom;
    Hitbox hitboxRange;
};

// Entity Class
struct EntityOneWayDoor {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 offsetY;
    Animator animator;
};

// Object Struct
extern ObjectOneWayDoor *OneWayDoor;

// Standard Entity Events
void OneWayDoor_Update(void);
void OneWayDoor_LateUpdate(void);
void OneWayDoor_StaticUpdate(void);
void OneWayDoor_Draw(void);
void OneWayDoor_Create(void *data);
void OneWayDoor_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void OneWayDoor_EditorDraw(void);
void OneWayDoor_EditorLoad(void);
#endif
void OneWayDoor_Serialize(void);

// Extra Entity Functions
void OneWayDoor_HandlePlayerInteractions(void);

void OneWayDoor_State_MoveDown(void);
void OneWayDoor_State_MoveUp(void);

void OneWayDoor_Draw_MMZ(void);
void OneWayDoor_Draw_CPZ(void);

#endif //! OBJ_ONEWAYDOOR_H
