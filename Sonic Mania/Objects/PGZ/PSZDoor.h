#ifndef OBJ_PSZDOOR_H
#define OBJ_PSZDOOR_H

#include "SonicMania.h"

typedef enum {
    PSZDOOR_ORIENATION_D,
    PSZDOOR_ORIENATION_U,
    PSZDOOR_ORIENATION_L,
    PSZDOOR_ORIENATION_R,
}PSZDoorOrientations;

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxes[2];
    uint16 aniFrames;
    uint16 sfxOpen;
} ObjectPSZDoor;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 orientation;
    bool32 open;
    int32 speed;
    uint8 id;
    int32 automatic;
    EntityDoorTrigger *trigger;
    bool32 shouldMove;
    Vector2 doorPos;
    Vector2 offset;
    Vector2 startPos; //its set once and never used
    Vector2 targetPos;
    Animator doorAnimator;
    Animator cogAnimator;
} EntityPSZDoor;

// Object Struct
extern ObjectPSZDoor *PSZDoor;

// Standard Entity Events
void PSZDoor_Update(void);
void PSZDoor_LateUpdate(void);
void PSZDoor_StaticUpdate(void);
void PSZDoor_Draw(void);
void PSZDoor_Create(void* data);
void PSZDoor_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PSZDoor_EditorDraw(void);
void PSZDoor_EditorLoad(void);
#endif
void PSZDoor_Serialize(void);

// Extra Entity Functions
int32 PSZDoor_GetRemainingDistance(void);

#endif //!OBJ_PSZDOOR_H
