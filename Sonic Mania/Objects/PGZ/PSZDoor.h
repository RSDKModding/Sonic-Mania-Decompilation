#ifndef OBJ_PSZDOOR_H
#define OBJ_PSZDOOR_H

#include "SonicMania.h"

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
    int speed;
    uint8 id;
    int automatic;
    EntityDoorTrigger *trigger;
    int field_70;
    Vector2 startPos;
    Vector2 offset;
    Vector2 field_84;
    Vector2 field_8C;
    Animator animator1;
    Animator animator2;
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
void PSZDoor_EditorDraw(void);
void PSZDoor_EditorLoad(void);
void PSZDoor_Serialize(void);

// Extra Entity Functions
int PSZDoor_Unknown1(void);

#endif //!OBJ_PSZDOOR_H
