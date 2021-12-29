#ifndef OBJ_SEESAW_H
#define OBJ_SEESAW_H

#include "SonicMania.h"

// Object Class
struct ObjectSeeSaw {
    RSDK_OBJECT
    TABLE(int32 value1[48], { -36, -36, -38, -40, -42, -44, -42, -40, -38, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, -25, -24, -23, -22,
                            -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -2,  -2,  -2,  -2 });
    Vector2 value2;
    int32 value3;
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxSpring;
};

// Entity Class
struct EntitySeeSaw {
    RSDK_ENTITY
    StateMachine(state);
    uint8 side;
    Vector2 orbPos;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    Animator animator1;
    Animator animator2;
    Animator animator3;
};

// Object Struct
extern ObjectSeeSaw *SeeSaw;

// Standard Entity Events
void SeeSaw_Update(void);
void SeeSaw_LateUpdate(void);
void SeeSaw_StaticUpdate(void);
void SeeSaw_Draw(void);
void SeeSaw_Create(void* data);
void SeeSaw_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SeeSaw_EditorDraw(void);
void SeeSaw_EditorLoad(void);
#endif
void SeeSaw_Serialize(void);

// Extra Entity Functions
void SeeSaw_SetupHitbox(int playerX, bool32 flag);
void SeeSaw_Unknown2(void);
void SeeSaw_Unknown3(void);
void SeeSaw_Unknown4(void);
void SeeSaw_Unknown5(void);

#endif //!OBJ_SEESAW_H
