#ifndef OBJ_HCZONEWAYDOOR_H
#define OBJ_HCZONEWAYDOOR_H

#include "Game.h"

typedef enum {
    HCZONEWAYDOOR_UP,
    HCZONEWAYDOOR_LEFT,
    HCZONEWAYDOOR_DOWN,
    HCZONEWAYDOOR_RIGHT,
} HCZOneWayDoorOrientations;

// Object Class
struct ObjectHCZOneWayDoor {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxClack;
};

// Entity Class
struct EntityHCZOneWayDoor {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    Vector2 size;
    Vector2 initialSize;
    int32 movePos;
    int32 length;
    int32 duration;
    uint8 orientation;
    uint8 detectDirection;
    Hitbox hitboxSolid;
    Hitbox hitboxTrigger;
    Animator animator;
};

// Object Struct
extern ObjectHCZOneWayDoor *HCZOneWayDoor;

// Standard Entity Events
void HCZOneWayDoor_Update(void);
void HCZOneWayDoor_LateUpdate(void);
void HCZOneWayDoor_StaticUpdate(void);
void HCZOneWayDoor_Draw(void);
void HCZOneWayDoor_Create(void *data);
void HCZOneWayDoor_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HCZOneWayDoor_EditorDraw(void);
void HCZOneWayDoor_EditorLoad(void);
#endif
void HCZOneWayDoor_Serialize(void);

// Extra Entity Functions
void HCZOneWayDoor_SetupHitboxes(void);
void HCZOneWayDoor_DrawSprites(void);

#endif //! OBJ_HCZONEWAYDOOR_H
