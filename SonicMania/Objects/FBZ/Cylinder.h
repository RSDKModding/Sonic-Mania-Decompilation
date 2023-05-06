#ifndef OBJ_CYLINDER_H
#define OBJ_CYLINDER_H

#include "Game.h"

typedef enum {
    CYLINDER_TUBE_H,
    CYLINDER_TUBE_V,
    CYLINDER_SPIRAL_LR,
    CYLINDER_SPIRAL_RL,
    CYLINDER_INKROLLER,
    CYLINDER_FIXED_V_L,
    CYLINDER_FIXED_V_R,
    CYLINDER_PILLAR,
} CylinderTypes;

// Object Class
struct ObjectCylinder {
    RSDK_OBJECT
    // unused, all frames are calculated on the fly
    TABLE(int32 frameTable[24], { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13 });
    Animator animator;
    uint16 aniFrames;
};

// Entity Class
struct EntityCylinder {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int32 length;
    int32 radius;
    int32 unused1;
    int32 playerStatus[PLAYER_COUNT];
    int32 playerAngles[PLAYER_COUNT];
    Hitbox hitboxRange;
    Hitbox hitboxSize;
    Hitbox hitboxL;
    Hitbox hitboxR;
};

// Object Struct
extern ObjectCylinder *Cylinder;

// Standard Entity Events
void Cylinder_Update(void);
void Cylinder_LateUpdate(void);
void Cylinder_StaticUpdate(void);
void Cylinder_Draw(void);
void Cylinder_Create(void *data);
void Cylinder_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Cylinder_EditorDraw(void);
void Cylinder_EditorLoad(void);
#endif
void Cylinder_Serialize(void);

// Extra Entity Functions
void Cylinder_State_TubeH(void);
void Cylinder_State_TubeV(void);
void Cylinder_State_Spiral(void);
void Cylinder_State_InkRoller(void);
void Cylinder_State_Pillar(void);

void Cylinder_PlayerState_InkRoller_Stand(void);
void Cylinder_PlayerState_InkRoller_Roll(void);
void Cylinder_PlayerState_Pillar(void);
void Cylinder_PlayerState_Spiral(void);

#endif //! OBJ_CYLINDER_H
