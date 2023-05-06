#ifndef OBJ_MECHABU_H
#define OBJ_MECHABU_H

#include "Game.h"

// Object Class
struct ObjectMechaBu {
    RSDK_OBJECT
    TABLE(int32 sawOffsets[10], { -0x150000, -0xF0000, -0x170000, -0xE0000, -0x190000, -0xA0000, -0x1A0000, -0x50000, -0x1A0000, -0x40000 });
    Hitbox hitboxSaw;
    Hitbox hitboxUnused;
    uint16 aniFrames;
    uint16 sfxSawUp;
    uint16 sfxSawDown;
};

// Entity Class
struct EntityMechaBu {
    RSDK_ENTITY
    StateMachine(state);
    PlaneFilterTypes planeFilter;
    uint8 timer;
    uint8 sawTimer;
    Vector2 startPos;
    uint8 startDir;
    Vector2 sawPos;
    Animator badnikAnimator;
    Animator hornAnimator;
    Animator sawAnimator;
};

// Object Struct
extern ObjectMechaBu *MechaBu;

// Standard Entity Events
void MechaBu_Update(void);
void MechaBu_LateUpdate(void);
void MechaBu_StaticUpdate(void);
void MechaBu_Draw(void);
void MechaBu_Create(void *data);
void MechaBu_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MechaBu_EditorDraw(void);
void MechaBu_EditorLoad(void);
#endif
void MechaBu_Serialize(void);

// Extra Entity Functions
void MechaBu_DebugSpawn(void);
void MechaBu_DebugDraw(void);

void MechaBu_CheckPlayerCollisions(void);
void MechaBu_CheckOffScreen(void);
Vector2 MechaBu_GetSawOffset(void);

void MechaBu_State_Init(void);
void MechaBu_State_Moving(void);
void MechaBu_State_Stopped(void);
void MechaBu_State_Falling(void);

#endif //! OBJ_MECHABU_H
