#ifndef OBJ_CYLINDER_H
#define OBJ_CYLINDER_H

#include "SonicMania.h"

typedef enum {
    CYLINDER_TUBE_H,
    CYLINDER_TUBE_V,
    CYLINDER_SPIRAL,
    CYLINDER_SPIRAL_FLIPPED,
    CYLINDER_INKROLLER,
    CYLINDER_UNUSED1,
    CYLINDER_UNUSED2,
    CYLINDER_PILLAR,
}CylinderTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 frameArray[24], { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13 });
    Animator animator;
    uint16 aniFrames;
} ObjectCylinder;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int32 length;
    int32 radius;
    int32 unused1;
    int32 playerStatuses[4];
    int32 playerAngles[4];
    Hitbox hitboxRange;
    Hitbox hitboxSize;
    Hitbox hitboxL;
    Hitbox hitboxR;
} EntityCylinder;

// Object Struct
extern ObjectCylinder *Cylinder;

// Standard Entity Events
void Cylinder_Update(void);
void Cylinder_LateUpdate(void);
void Cylinder_StaticUpdate(void);
void Cylinder_Draw(void);
void Cylinder_Create(void* data);
void Cylinder_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
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

void Cylinder_Player_State_InkRoller_Stand(void);
void Cylinder_Player_State_InkRoller_Roll(void);
void Cylinder_Player_State_Pillar(void);
void Cylinder_Player_State_Unknown1(void);

#endif //!OBJ_CYLINDER_H
