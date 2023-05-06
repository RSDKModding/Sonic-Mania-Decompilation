#ifndef OBJ_SWITCHDOOR_H
#define OBJ_SWITCHDOOR_H

#include "Game.h"

typedef enum {
    SWITCHDOOR_GO_INSIDE1,
    SWITCHDOOR_GO_INSIDE2,
    SWITCHDOOR_GO_INSIDE2_ALT,
} SwitchDoorGoModes;

// Object Class
struct ObjectSwitchDoor {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
};

// Entity Class
struct EntitySwitchDoor {
    RSDK_ENTITY
    int32 go;
    bool32 reversible;
    bool32 activated;
    Animator animator;
};

// Object Struct
extern ObjectSwitchDoor *SwitchDoor;

// Standard Entity Events
void SwitchDoor_Update(void);
void SwitchDoor_LateUpdate(void);
void SwitchDoor_StaticUpdate(void);
void SwitchDoor_Draw(void);
void SwitchDoor_Create(void *data);
void SwitchDoor_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SwitchDoor_EditorDraw(void);
void SwitchDoor_EditorLoad(void);
#endif
void SwitchDoor_Serialize(void);

// Extra Entity Functions
void SwitchDoor_DrawSprites(void);

#endif //! OBJ_SWITCHDOOR_H
