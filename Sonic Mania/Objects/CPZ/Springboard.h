#ifndef OBJ_SPRINGBOARD_H
#define OBJ_SPRINGBOARD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int array1[3], { -0x40000, -0xA0000, -0x80000 });
    TABLE(int array2[40], { 0,        0,        0,        0,        0,        0,        0,        0,        -0x10000, -0x10000,
                            -0x10000, -0x10000, -0x10000, -0x10000, -0x10000, -0x10000, -0x10000, -0x10000, -0x10000, -0x10000,
                            -0x10000, -0x10000, -0x10000, -0x10000, -0x20000, -0x20000, -0x20000, -0x20000, -0x20000, -0x20000,
                            -0x20000, -0x20000, -0x30000, -0x30000, -0x30000, -0x30000, -0x30000, -0x30000, -0x40000, -0x40000 });
    TABLE(int array3[28], { 8, 9, 10, 11, 12, 13, 14, 15, 16, 16, 17, 18, 19, 20, 20, 21, 21, 22, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24 });
    TABLE(int array4[28], { 8, 9, 10, 11, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 14, 14, 15, 15, 16, 16, 16, 16, 15, 15, 14, 14, 13, 13 });
    ushort aniFrames;
    ushort sfxSpring;
} ObjectSpringboard;

// Entity Class
typedef struct {
	RSDK_ENTITY
    int force;
    int field_5C;
    byte playerBits;
    Animator animator;
} EntitySpringboard;

// Object Struct
extern ObjectSpringboard *Springboard;

// Standard Entity Events
void Springboard_Update(void);
void Springboard_LateUpdate(void);
void Springboard_StaticUpdate(void);
void Springboard_Draw(void);
void Springboard_Create(void* data);
void Springboard_StageLoad(void);
void Springboard_EditorDraw(void);
void Springboard_EditorLoad(void);
void Springboard_Serialize(void);

// Extra Entity Functions
void Springboard_DebugSpawn(void);
void Springboard_DebugDraw(void);

#endif //!OBJ_SPRINGBOARD_H
