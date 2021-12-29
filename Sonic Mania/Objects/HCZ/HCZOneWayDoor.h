#ifndef OBJ_HCZONEWAYDOOR_H
#define OBJ_HCZONEWAYDOOR_H

#include "SonicMania.h"

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
    Vector2 posUnknown1;
    Vector2 posUnknown2;
    Vector2 posUnknown3;
    int32 field_74;
    int32 length;
    int32 duration;
    uint8 orientation;
    uint8 detectDirection;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Animator animator;
};

// Object Struct
extern ObjectHCZOneWayDoor *HCZOneWayDoor;

// Standard Entity Events
void HCZOneWayDoor_Update(void);
void HCZOneWayDoor_LateUpdate(void);
void HCZOneWayDoor_StaticUpdate(void);
void HCZOneWayDoor_Draw(void);
void HCZOneWayDoor_Create(void* data);
void HCZOneWayDoor_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HCZOneWayDoor_EditorDraw(void);
void HCZOneWayDoor_EditorLoad(void);
#endif
void HCZOneWayDoor_Serialize(void);

// Extra Entity Functions
void HCZOneWayDoor_Unknown1(void);
void HCZOneWayDoor_DrawSprites(void);

#endif //!OBJ_HCZONEWAYDOOR_H
