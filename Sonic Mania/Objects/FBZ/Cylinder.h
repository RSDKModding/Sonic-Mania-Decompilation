#ifndef OBJ_CYLINDER_H
#define OBJ_CYLINDER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int frameArray[24], { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13 });
    Animator animator;
    uint16 aniFrames;
} ObjectCylinder;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int length;
    int radius;
    int field_68;
    int playerFrameIDs[4];
    int playerAngles[4];
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
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
void Cylinder_EditorDraw(void);
void Cylinder_EditorLoad(void);
void Cylinder_Serialize(void);

// Extra Entity Functions
void Cylinder_State_Unknown1(void);
void Cylinder_State_Unknown2(void);
void Cylinder_State_Unknown3(void);
void Cylinder_State_Unknown4(void);
void Cylinder_State_Unknown5(void);

void Cylinder_Player_State_Unknown2(void);
void Cylinder_Player_State_Unknown3(void);
void Cylinder_Player_State_Unknown4(void);
void Cylinder_Player_State_Unknown1(void);

#endif //!OBJ_CYLINDER_H
