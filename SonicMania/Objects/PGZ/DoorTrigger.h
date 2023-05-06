#ifndef OBJ_DOORTRIGGER_H
#define OBJ_DOORTRIGGER_H

#include "Game.h"

typedef enum {
    DOORTRIGGER_ORIENATION_L,
    DOORTRIGGER_ORIENATION_R,
    DOORTRIGGER_ORIENATION_U,
    DOORTRIGGER_ORIENATION_D,
} DoorTriggerOrientations;

// Object Class
struct ObjectDoorTrigger {
    RSDK_OBJECT
    Hitbox hitboxBulb[2];
    uint16 aniFrames;
    uint16 sfxShatter;
};

// Entity Class
struct EntityDoorTrigger {
    RSDK_ENTITY
    uint8 orientation;
    uint8 id;
    Animator baseAnimator;
    Animator bulbAnimator;
};

// Object Struct
extern ObjectDoorTrigger *DoorTrigger;

// Standard Entity Events
void DoorTrigger_Update(void);
void DoorTrigger_LateUpdate(void);
void DoorTrigger_StaticUpdate(void);
void DoorTrigger_Draw(void);
void DoorTrigger_Create(void *data);
void DoorTrigger_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DoorTrigger_EditorDraw(void);
void DoorTrigger_EditorLoad(void);
#endif
void DoorTrigger_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_DOORTRIGGER_H
