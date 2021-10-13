#ifndef OBJ_SEESAW_H
#define OBJ_SEESAW_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int value1[48], { -36, -36, -38, -40, -42, -44, -42, -40, -38, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, -25, -24, -23, -22,
                            -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -2,  -2,  -2,  -2 });
    Vector2 value2;
    int value3;
    Hitbox value4;
    Hitbox value5;
    ushort value6;
    ushort value7;
} ObjectSeeSaw;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySeeSaw;

// Object Struct
extern ObjectSeeSaw *SeeSaw;

// Standard Entity Events
void SeeSaw_Update(void);
void SeeSaw_LateUpdate(void);
void SeeSaw_StaticUpdate(void);
void SeeSaw_Draw(void);
void SeeSaw_Create(void* data);
void SeeSaw_StageLoad(void);
void SeeSaw_EditorDraw(void);
void SeeSaw_EditorLoad(void);
void SeeSaw_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SEESAW_H
