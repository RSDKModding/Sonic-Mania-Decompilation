#ifndef OBJ_HCZONEWAYDOOR_H
#define OBJ_HCZONEWAYDOOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort sfxClack;
} ObjectHCZOneWayDoor;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 posUnknown1;
    Vector2 posUnknown2;
    Vector2 posUnknown3;
    int field_74;
    int length;
    int duration;
    byte orientation;
    byte detectDirection;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Animator animator;
} EntityHCZOneWayDoor;

// Object Struct
extern ObjectHCZOneWayDoor *HCZOneWayDoor;

// Standard Entity Events
void HCZOneWayDoor_Update(void);
void HCZOneWayDoor_LateUpdate(void);
void HCZOneWayDoor_StaticUpdate(void);
void HCZOneWayDoor_Draw(void);
void HCZOneWayDoor_Create(void* data);
void HCZOneWayDoor_StageLoad(void);
void HCZOneWayDoor_EditorDraw(void);
void HCZOneWayDoor_EditorLoad(void);
void HCZOneWayDoor_Serialize(void);

// Extra Entity Functions
void HCZOneWayDoor_Unknown1(void);
void HCZOneWayDoor_DrawSprites(void);

#endif //!OBJ_HCZONEWAYDOOR_H
